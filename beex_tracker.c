/***************************************************************************
 * Copyright (C) 2017 - 2020, Lanka Hsu, <lankahsu@gmail.com>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
#include <math.h>

#include "beex_api.h"

static ChainXCtx_t *chainX_mctt = NULL;
static uint8_t tracker_order = 0;
static uint8_t tracker_watch = 0;
static uint8_t tracker_logging = 0;

#define MAX_OF_ISSUE_LOG 1000

static char my_macid[LEN_OF_MAC] = JVAL_C_MAC_NULL;

CLIST(NotifyListHead);
CLIST(LogginHead);

ChainXCtx_t *tracker_get_chainX(void )
{
	return chainX_mctt;
}

char *tracker_get_frm_ip(void)
{
	struct sockaddr_in *add_frm = (struct sockaddr_in *)chainX_addr_from_get(chainX_mctt);
	return inet_ntoa(add_frm->sin_addr);
}

void tracker_set_mac(char *macid)
{
	SAFE_SPRINTF_EX(my_macid, "%s", macid);
}

void tracker_notify_set_uuid(Notify_t *notify, char *c_uuid)
{
	SAFE_SPRINTF_EX(notify->c_uuid, "%s", c_uuid);
}

static clist_t tracker_logging_head(void)
{
	return LogginHead;
}

static void tracker_logging_free(clist_t head)
{
	clist_free(head);
}

static void tracker_logging_open(clist_t head)
{
	clist_init(head);
}

static void tracker_logging_push(IssueItem_t *c_issueitem)
{
	clist_t head = tracker_logging_head();

	IssueLogging_t *issue_new = (IssueLogging_t*)SAFE_CALLOC(1, sizeof(IssueLogging_t));
	SAFE_MEMCPY(&issue_new->c_issueitem, c_issueitem, sizeof(IssueItem_t), sizeof(IssueItem_t));
	clist_push(head, issue_new);

	int count = clist_length(head);
	while (count>MAX_OF_ISSUE_LOG)
	{
		IssueLogging_t *issue_entry = (IssueLogging_t *)clist_pop(head);
		if (issue_entry) SAFE_FREE(issue_entry);
		count = clist_length(head);
	}
}

void tracker_logging_set_on(void)
{
	tracker_logging = 1;
}

void tracker_logging_set_off(void)
{
	tracker_logging = 0;
}

void tracker_logging_show(void)
{
	clist_t head = tracker_logging_head();

	IssueLogging_t *curr = NULL;

	for (curr = clist_head(head); curr != NULL; curr = clist_item_next(curr))
	{
		IssueItem_t *c_issueitem = &curr->c_issueitem;
		Reporter_t *reporter = &c_issueitem->reporter;
		//uint32_t issueid = c_issueitem->issueid;

		switch (reporter->methodid)
		{
			case JVAL_METHODID_EVENT:
				tracker_watch_simple(NULL, c_issueitem, 1);
				break;
			case JVAL_METHODID_PUT:
				tracker_order_simple(NULL, c_issueitem, 1);
				break;
			default:
				break;
		}
	}
}

static clist_t tracker_notify_head(void)
{
	return NotifyListHead;
}

static void tracker_notify_free(clist_t head)
{
	//clist_free(head);
}

static void tracker_notify_open(clist_t head)
{
	clist_init(head);
}

void tracker_notify_register(Notify_t *notify)
{
	clist_t head = tracker_notify_head();

	clist_push(head, notify);
}

static void tracker_others_cb(void *userdata, IssueItem_t *c_issueitem)
{
	Reporter_t *reporter = &c_issueitem->reporter;
	//uint16_t methodid = reporter->methodid;
	//char *c_macid = reporter->id_frm.c_macid;
	//uint16_t protocolid = reporter->id_frm.protocolid;
	char *c_uuid = reporter->id_frm.c_uuid;
	//char *c_nodeid = reporter->id_frm.c_nodeid;
	//uint8_t epid = reporter->id_frm.epid;

	clist_t head = tracker_notify_head();
	Notify_t *cur = NULL;

	for (cur = clist_head(head); cur != NULL; cur = clist_item_next(cur))
	{
		if ( (SAFE_STRCMP(c_uuid, JVAL_C_UUID_BROADCAST) == 0)  || (SAFE_STRCMP(cur->c_uuid, JVAL_C_UUID_BROADCAST) == 0) || (SAFE_STRCMP(cur->c_uuid, c_uuid) == 0) )
		{
			if (cur->others_cb) cur->others_cb(userdata, c_issueitem);
		}
	}
}

void tracker_others_simple(void *userdata, IssueItem_t *c_issueitem, uint8_t dbg)
{
	if (dbg==0) tracker_others_cb(userdata, c_issueitem);
}

static void tracker_order_func_cb(void *userdata, IssueItem_t *c_issueitem, ORDER_FN_ID fn_id)
{
	Commander_t *commander = &c_issueitem->commander;
	//uint16_t methodid = commander->methodid;
	//char *c_macid = commander->id_to.c_macid;
	//uint16_t protocolid = commander->id_to.protocolid;
	char *c_uuid = commander->id_to.c_uuid;
	//char *c_nodeid = commander->id_to.c_nodeid;
	//uint8_t epid = commander->id_to.epid;

	clist_t head = tracker_notify_head();
	Notify_t *cur = NULL;

	if (fn_id>=ORDER_FN_ID_MAX) return;

	for (cur = clist_head(head); cur != NULL; cur = clist_item_next(cur))
	{
		if ( (SAFE_STRCMP(c_uuid, JVAL_C_UUID_BROADCAST) == 0)  || (SAFE_STRCMP(cur->c_uuid, JVAL_C_UUID_BROADCAST) == 0) || (SAFE_STRCMP(cur->c_uuid, c_uuid) == 0) )
		{
			if (cur->order_item_cb[fn_id]) cur->order_item_cb[fn_id](userdata, c_issueitem);
		}
	}
}

static void tracker_order_cb(void *userdata, IssueItem_t *c_issueitem)
{
	Commander_t *commander = &c_issueitem->commander;
	//uint16_t methodid = commander->methodid;
	//char *c_macid = commander->id_to.c_macid;
	//uint16_t protocolid = commander->id_to.protocolid;
	char *c_uuid = commander->id_to.c_uuid;
	//char *c_nodeid = commander->id_to.c_nodeid;
	//uint8_t epid = commander->id_to.epid;

	clist_t head = tracker_notify_head();
	Notify_t *cur = NULL;

	for (cur = clist_head(head); cur != NULL; cur = clist_item_next(cur))
	{
		//DBG_TR_LN("(c_uuid: %s, cur->c_uuid: %s)", c_uuid, cur->c_uuid);
		if ( (SAFE_STRCMP(c_uuid, JVAL_C_UUID_BROADCAST) == 0) || (SAFE_STRCMP(cur->c_uuid, JVAL_C_UUID_BROADCAST) == 0) || (SAFE_STRCMP(cur->c_uuid, c_uuid) == 0) )
		{
			if (cur->order_cb) cur->order_cb(userdata, c_issueitem);
		}
	}
}

void tracker_order_simple(void *userdata, IssueItem_t *c_issueitem, uint8_t dbg)
{
	int idx = 0;

	Commander_t *commander = &c_issueitem->commander;
	uint16_t methodid = commander->methodid;
	uint16_t protocolid = commander->protocolid;

	char *c_macid_frm = commander->id_frm.c_macid;
	char *c_uuid_frm = commander->id_frm.c_uuid;
	char *c_nodeid_frm = commander->id_frm.c_nodeid;
	uint8_t epid_frm = commander->id_frm.epid;

	char *c_macid_to = commander->id_to.c_macid;
	char *c_uuid_to = commander->id_to.c_uuid;
	char *c_nodeid_to = commander->id_to.c_nodeid;
	uint8_t epid_to = commander->id_to.epid;

	uint32_t issueid = c_issueitem->issueid;
	uint16_t data_len = c_issueitem->data_len;
	uint32_t timestamp = c_issueitem->timestamp;
	uint8_t val = (unsigned int)c_issueitem->data[idx];
	char *valp = c_issueitem->data;

	char tag[LEN_OF_VAL32] = "";
	char response[LEN_OF_BUF1024] = "";
	char buff[LEN_OF_BUF256] = "";
	char node_str[LEN_OF_UUID+8] = "";
	SAFE_SPRINTF_EX(node_str, "%s.%d", c_nodeid_to, epid_to);
	SAFE_SPRINTF_EX(response, "0x%08X,%u", issueid, val);
	SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

	switch (JKEY_ISSUEID_GATEWAY_MASK & issueid)
	{
		
		// 0x00000000
		case JKEY_ISSUEID_GW:
			SAFE_SPRINTF_EX(response, "%s,", JKEY_REPORT_GATEWAY);
			BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

			if (dbg==0) tracker_order_func_cb(userdata, c_issueitem, ORDER_FN_ID_GW);
			break;
		// 0x00000002
		case JKEY_ISSUEID_ADDING:
			SAFE_SPRINTF_EX(response, "%s,", JKEY_REPORT_OP_ADDING);
			BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

			if (dbg==0) tracker_order_func_cb(userdata, c_issueitem, ORDER_FN_ID_ADDING);
			break;
		// 0x00000003
		case JKEY_ISSUEID_REMOVING:
			SAFE_SPRINTF_EX(response, "%s,", JKEY_REPORT_OP_REMOVING);
			BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

			if (dbg==0) tracker_order_func_cb(userdata, c_issueitem, ORDER_FN_ID_REMOVING);
			break;
		// 0x00000004
		case JKEY_ISSUEID_ABORTING:
			SAFE_SPRINTF_EX(response, "%s,", JKEY_REPORT_OP_ABORTING);
			BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

			if (dbg==0) tracker_order_func_cb(userdata, c_issueitem, ORDER_FN_ID_ABORTING);
			break;
		// 0x00000005
		case JKEY_ISSUEID_LEARNING:
			SAFE_SPRINTF_EX(response, "%s,", JKEY_REPORT_OP_LEARNING);
			BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

			if (dbg==0) tracker_order_func_cb(userdata, c_issueitem, ORDER_FN_ID_LEARNING);
			break;
		// 0x00000007
		case JKEY_ISSUEID_NW:
			SAFE_SPRINTF_EX(response, "%s,", "nw");
			BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

			if (dbg==0) tracker_order_func_cb(userdata, c_issueitem, ORDER_FN_ID_NW);
			break;
		// 0x00000008
		case JKEY_ISSUEID_RESETING:
			SAFE_SPRINTF_EX(response, "%s,", JKEY_REPORT_OP_RESETING);
			BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

			if (dbg==0) tracker_order_func_cb(userdata, c_issueitem, ORDER_FN_ID_RESETING);
			break;
		// 0x0000000A
		case JKEY_ISSUEID_REPLACING:
			SAFE_SPRINTF_EX(response, "%s,", JKEY_REPORT_OP_REPLACING_FAILED);
			BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

			if (dbg==0) tracker_order_func_cb(userdata, c_issueitem, ORDER_FN_ID_REPLACING);
			break;
		// 0x0000000B
		case JKEY_ISSUEID_RMFAILED:
			SAFE_SPRINTF_EX(response, "%s,", JKEY_REPORT_OP_REMOVING_FAILED);
			BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

			if (dbg==0) tracker_order_func_cb(userdata, c_issueitem, ORDER_FN_ID_RMFAILED);
			break;
		// 0x00000010
		case JKEY_ISSUEID_NODE:
			SAFE_SPRINTF_EX(response, "%s,", "node");
			BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

			if (dbg==0) tracker_order_func_cb(userdata, c_issueitem, ORDER_FN_ID_NODE);
			break;
		// 0x00000011
		case JKEY_ISSUEID_NOP:
			SAFE_SPRINTF_EX(response, "%s,", JKEY_REPORT_NOP);
			BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

			if (dbg==0) tracker_order_func_cb(userdata, c_issueitem, ORDER_FN_ID_NOP);
			break;
		// 0x00092001
		case JKEY_ISSUEID_CC_BASIC:
			SAFE_SPRINTF_EX(response, "%s,", JKEY_REPORT_BASIC);
			BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

			if (dbg==0) tracker_order_func_cb(userdata, c_issueitem, ORDER_FN_ID_BASIC);
			break;
		// 0x00092501
		case JKEY_ISSUEID_CC_SWITCH:
			SAFE_SPRINTF_EX(response, "%s,", JKEY_REPORT_SWITCH);
			BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

			if (dbg==0) tracker_order_func_cb(userdata, c_issueitem, ORDER_FN_ID_SWITCH);
			break;
		// 0x00092601
		case JKEY_ISSUEID_CC_DIMMER:
			SAFE_SPRINTF_EX(response, "%s,", JKEY_REPORT_DIMMER);
			BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

			if (dbg==0) tracker_order_func_cb(userdata, c_issueitem, ORDER_FN_ID_LEVEL);
			break;

		// 0x000A9501
		case JKEY_ISSUEID_CC_INFRARED:
			SAFE_SPRINTF_EX(response, "%s,", JKEY_REPORT_INFRARED);
			BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_COMMAND );

			if (dbg==0) tracker_order_func_cb(userdata, c_issueitem, ORDER_FN_ID_INFRARED);
			break;

		default:
			break;
	}

	if (tracker_order==1)
	{
		char id_frm[LEN_OF_BUF256] = "";
		SAFE_SPRINTF_EX(id_frm, "%s/%s/%s/%d", c_macid_frm, c_uuid_frm, c_nodeid_frm, epid_frm);
		char id_to[LEN_OF_BUF256] = "";
		SAFE_SPRINTF_EX(id_to, "%s/%s/%s/%d", c_macid_to, c_uuid_to, c_nodeid_to, epid_to);
		ZWORDER_DBG("%s [%d/%d-%s] - --> %s->%s - %s=%s", time_now_short((time_t)timestamp), methodid, protocolid, translate_protocolid(protocolid), id_frm, id_to, tag, response);
	}

	if (dbg==0) tracker_order_cb(userdata, c_issueitem);
}

void tracker_order_set_on(void)
{
	tracker_order = 1;
}

void tracker_order_set_off(void)
{
	tracker_order = 0;
}

static void tracker_watch_func_cb(void *userdata, IssueItem_t *c_issueitem, NOTIFY_FN_ID fn_id)
{
	Reporter_t *reporter = &c_issueitem->reporter;
	//uint16_t methodid = reporter->methodid;
	//char *c_macid = reporter->id_frm.c_macid;
	//uint16_t protocolid = reporter->id_frm.protocolid;
	char *c_uuid = reporter->id_frm.c_uuid;
	//char *c_nodeid = reporter->id_frm.c_nodeid;
	//uint8_t epid = reporter->id_frm.epid;

	clist_t head = tracker_notify_head();
	Notify_t *cur = NULL;

	if (fn_id>=NOTIFY_FN_ID_MAX) return;

	for (cur = clist_head(head); cur != NULL; cur = clist_item_next(cur))
	{
		if ( (SAFE_STRCMP(c_uuid, JVAL_C_UUID_BROADCAST) == 0)  || (SAFE_STRCMP(cur->c_uuid, JVAL_C_UUID_BROADCAST) == 0) || (SAFE_STRCMP(cur->c_uuid, c_uuid) == 0) )
		{
			if (cur->watch_item_cb[fn_id]) cur->watch_item_cb[fn_id](userdata, c_issueitem);
		}
	}
}

static void tracker_watch_cb(void *userdata, IssueItem_t *c_issueitem)
{
	Reporter_t *reporter = &c_issueitem->reporter;
	//uint16_t methodid = reporter->methodid;
	//char *c_macid = reporter->id_frm.c_macid;
	char *c_uuid = reporter->id_frm.c_uuid;
	//char *c_nodeid = reporter->id_frm.c_nodeid;
	//uint8_t epid = reporter->id_frm.epid;

	clist_t head = tracker_notify_head();
	Notify_t *cur = NULL;

	for (cur = clist_head(head); cur != NULL; cur = clist_item_next(cur))
	{
		if ( (SAFE_STRCMP(c_uuid, JVAL_C_UUID_BROADCAST) == 0)  || (SAFE_STRCMP(cur->c_uuid, JVAL_C_UUID_BROADCAST) == 0) || (SAFE_STRCMP(cur->c_uuid, c_uuid) == 0) )
		{
			if (cur->watch_cb) cur->watch_cb(userdata, c_issueitem);
		}
	}
}

void tracker_watch_simple(void *userdata, IssueItem_t *c_issueitem, uint8_t dbg)
{
	int idx = 0;

	Reporter_t *reporter = &c_issueitem->reporter;
	uint16_t methodid = reporter->methodid;
	uint16_t protocolid = reporter->protocolid;

	char *c_macid_frm = reporter->id_frm.c_macid;
	char *c_uuid_frm = reporter->id_frm.c_uuid;
	char *c_nodeid_frm = reporter->id_frm.c_nodeid;
	uint8_t epid_frm = reporter->id_frm.epid;

	uint32_t issueid = c_issueitem->issueid;
	//uint16_t mode = c_issueitem->mode;
	//uint16_t submode = c_issueitem->submode;
	//uint16_t category = c_issueitem->category;
	uint32_t timestamp = c_issueitem->timestamp;
	uint16_t data_len = c_issueitem->data_len;
	uint8_t val = (unsigned int)c_issueitem->data[idx];
	char *valp = c_issueitem->data;

	char tag[LEN_OF_VAL32] = "";
	char response[LEN_OF_BUF1024] = "";
	char buff[LEN_OF_BUF256] = "";
	char node_str[LEN_OF_UUID+8] = "";
	SAFE_SPRINTF_EX(node_str, "%s.%d", c_nodeid_frm, epid_frm);
	SAFE_SPRINTF_EX(response, "0x%08X,", issueid);
	BUFF_DUMP_BUFF(valp, data_len, ",", "0x", buff, LEN_OF_BUF256);
	SAFE_STRCAT(response, buff);
	switch (issueid)
	{
		// 0x00000000
		case JKEY_ISSUEID_GW:
			idx++;
			switch (val)
			{
				// 95
				case ZWAVE_EVT_GW_CFG:
					{
						uint8_t param_num = valp[idx++];
						uint32_t value = byte2big_endian(4, (uint8_t *)valp+idx);
						SAFE_SPRINTF_EX(response, "%s,%u,%d", JKEY_REPORT_CFG, param_num, value);
					}
					break;
				// 96
				case ZWAVE_EVT_GW_CMD:
					{
						SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_COMMAND, valp+idx);
					}
					break;
				// 97
				case ZWAVE_EVT_GW_DEBUG:
					{
						SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_DEBUG, valp+idx);
					}
					break;
				// 98
				case ZWAVE_EVT_GW_WATCH_OFF:
					{
						SAFE_SPRINTF_EX(response, "%s,%u", JKEY_REPORT_WATCH_OFF, valp[idx++]);
					}
					break;
				// 99
				case ZWAVE_EVT_GW_ECHO:
					{
						SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_ECHO, valp+idx);
					}
					break;
				default:
					{
						SAFE_SPRINTF_EX(response, "0x%08x,%u", JKEY_ISSUEID_GW, val);
					}
					break;
			}
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_ZWINFO );

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_GW);
			break;
		// 0x00000001
		case JKEY_ISSUEID_READY:
			{
				uint8_t last_op = valp[idx++];
				uint16_t nodeid_ready = byte2big_endian(2, (uint8_t *)valp+idx);
				idx+=2;
				SAFE_SPRINTF_EX(response, "%s,%s,0x%02X", JKEY_REPORT_READY, translate_nw_op(last_op), nodeid_ready);
				SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );

				if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_READY);
			}
			break;
		// 0x00000002
		case JKEY_ISSUEID_ADDING:
			idx++;
			switch (val)
			{
				case ZWAVE_EVT_ADDING_START:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_ADDING, JVAL_REPORT_START);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_ADDING_ADDED:
					SAFE_SPRINTF_EX(response, "%s,%s", node_str, JKEY_REPORT_OP_ADDED );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_ADDING_DONE:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_ADDING, JVAL_REPORT_DONE);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				case ZWAVE_EVT_ADDING_FAIL:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_ADDING, JVAL_COMM_FAIL);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				case ZWAVE_EVT_ADDING_CSA:
					SAFE_SPRINTF_EX(response, "%s,%s,%s", JKEY_REPORT_OP_ADDING, JVAL_SECURITY_CSA, valp+idx);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				case ZWAVE_EVT_ADDING_SSA_SET:
					SAFE_SPRINTF_EX(response, "%s,%s,%u", JKEY_REPORT_OP_ADDING, JVAL_SECURITY_SET, valp[idx++]);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				case ZWAVE_EVT_ADDING_SSA:
					SAFE_SPRINTF_EX(response, "%s,%s,%s", JKEY_REPORT_OP_ADDING, JVAL_SECURITY_SSA, valp+idx);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				case ZWAVE_EVT_ADDING_SEC:
					{
						uint8_t highest_lvl = valp[idx++];
						uint8_t curr_lvl = valp[idx++];
						SAFE_SPRINTF_EX(response, "%s,%s,%d,%d,%s", JKEY_REPORT_OP_ADDING, JKEY_REPORT_SEC, highest_lvl, curr_lvl, (curr_lvl<highest_lvl)? JVAL_SECURITY_LOWER: JVAL_SECURITY_NORMAL );
						SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					}
					break;
				default:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_ADDING, translate_op_add_node(val) );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
			}

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_ADDING);
			break;
		// 0x00000003
		case JKEY_ISSUEID_REMOVING:
			idx++;
			switch (val)
			{
				case ZWAVE_EVT_REMOVING_START:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_REMOVING, JVAL_REPORT_START);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_REMOVING_REMOVED:
					SAFE_SPRINTF_EX(response, "%s,%s", node_str, JKEY_REPORT_OP_REMOVED );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_REMOVING_DONE:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_REMOVING, JVAL_REPORT_DONE);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				case ZWAVE_EVT_REMOVING_FAIL:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_REMOVING, JVAL_COMM_FAIL);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				default:
					SAFE_SPRINTF_EX(response, "%s,%d", JKEY_REPORT_OP_REMOVING, val );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
			}

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_REMOVING);
			break;
		// 0x00000004
		case JKEY_ISSUEID_ABORTING:
			idx++;
			switch (val)
			{
				case ZWAVE_EVT_ABORTING_ABORTED:
					SAFE_SPRINTF_EX(response, "%s,%s", node_str, JKEY_REPORT_OP_ABORTED );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_ABORTING_DONE:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_ABORTING, JVAL_REPORT_DONE);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				case ZWAVE_EVT_ABORTING_FAIL:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_ABORTING, JVAL_COMM_FAIL);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				default:
					SAFE_SPRINTF_EX(response, "%s,%d", JKEY_REPORT_OP_ABORTING, val );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
			}

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_ABORTING);
			break;
		// 0x00000005
		case JKEY_ISSUEID_LEARNING:
			idx++;
			switch (val)
			{
				case ZWAVE_EVT_LEARNING_START:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_LEARNING, JVAL_REPORT_START);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_LEARNING_LEARNED:
					SAFE_SPRINTF_EX(response, "%s,%s", node_str, JKEY_REPORT_OP_LEARNED );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_LEARNING_DONE:
					SAFE_SPRINTF_EX(response, "%s,%s,%d", JKEY_REPORT_OP_LEARNING, JVAL_REPORT_DONE, valp[idx++]);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				case ZWAVE_EVT_LEARNING_FAIL:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_LEARNING, JVAL_COMM_FAIL);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				case ZWAVE_EVT_ADDING_SSA_SET:
					SAFE_SPRINTF_EX(response, "%s,%s,%u", JKEY_REPORT_OP_ADDING, JVAL_SECURITY_SET, valp[idx++]);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				case ZWAVE_EVT_ADDING_SSA:
					SAFE_SPRINTF_EX(response, "%s,%s,%s", JKEY_REPORT_OP_LEARNING, JVAL_SECURITY_SSA, valp+idx);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				default:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_LEARNING, translate_op_initiate(val) );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
			}

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_LEARNING);
			break;
		// 0x00000007
		case JKEY_ISSUEID_NW:
			idx++;
			switch (val)
			{
				case ZWAVE_EVT_NW_NOROUTE:
					SAFE_SPRINTF_EX(response, "%s,%s", node_str, JKEY_REPORT_NW_NOROUTE );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_NW_BUSY:
					SAFE_SPRINTF_EX(response, "%s,%s", node_str, JKEY_REPORT_NW_BUSY );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_NW_NO_ACK:
					SAFE_SPRINTF_EX(response, "%s,%s", node_str, JKEY_REPORT_NW_NO_ACK );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_NW_SYS_ERR:
					SAFE_SPRINTF_EX(response, "%s,%s", node_str, JKEY_REPORT_NW_SYS_ERR );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_NW_BOOTUP:
					SAFE_SPRINTF_EX(response, "%s,%s", node_str, JKEY_REPORT_NW_BOOTUP );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_NW_SERIAL:
					{
						uint8_t moduleVer = valp[idx++]; // SERIAL_API_GET_INIT_DATA / ModuleVer
						uint8_t capability = valp[idx++]; // SERIAL_API_GET_INIT_DATA / Capabilities
						uint8_t chipType = valp[idx++]; // SERIAL_API_GET_INIT_DATA / ChipType
						uint8_t chipVer = valp[idx++]; // SERIAL_API_GET_INIT_DATA / ChipVer
						//int8_t nodelist[32]; // SERIAL_API_GET_INIT_DATA / NodeList
						//SAFE_MEMCPY(nodelist, valp+idx, 32, 32);
						idx+= 32;
						uint8_t cap = valp[idx++]; 

						SAFE_SPRINTF_EX(response, "%s,%u,0x%02x,%u,%u,0x%02x", JKEY_REPORT_NW_SERIAL, moduleVer, capability, chipType, chipVer, cap);
						SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_ZWINFO );
					}
					break;
				case ZWAVE_EVT_NW_HOMEID:
					{
						//uint8_t ctl_id = valp[idx++];
						idx++;
						uint8_t ctl_role = valp[idx++];
						uint8_t ctl_cap = valp[idx++];
						uint8_t ctl_zw_role = valp[idx++];
						uint16_t ctl_id = byte2big_endian(2, (uint8_t *)valp+idx);
						idx+=2;
						uint16_t suc = byte2big_endian(2, (uint8_t *)valp+idx);
						idx+=2;
						char ctl_role_name[128]="";
						translate_ctl_role(ctl_role, ctl_role_name);
						char *homeid = valp+idx;
						SAFE_SPRINTF_EX(response, "%s,%s,0x%02x,%u-%s,%u-%s,%u-%s,0x%02x", JKEY_REPORT_NW_HOMEID, homeid, ctl_id, ctl_role, ctl_role_name, ctl_cap, translate_ctl_capabilities(ctl_cap), ctl_zw_role, translate_ctl_zw_role(ctl_zw_role), suc);
						SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_ZWINFO );
					}
					break;
				case ZWAVE_EVT_NW_DEBUG:
					{
						SAFE_SPRINTF_EX(response, "%s,%d", JKEY_REPORT_DEBUG, valp[idx++]);
						SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_ZWINFO );
					}
					break;
				case ZWAVE_EVT_NW_WATCH_OFF:
					{
						SAFE_SPRINTF_EX(response, "%s,%d", JKEY_REPORT_WATCH_OFF, valp[idx++]);
						SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_ZWINFO );
					}
					break;
				case ZWAVE_EVT_NW_ECHO:
					{
						SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_ECHO, valp+1);
						SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_ZWINFO );
					}
					break;
				default:
					SAFE_SPRINTF_EX(response, "%s,%s", node_str, JKEY_REPORT_NW_XXX );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
			}

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_NW);
			break;
		// 0x00000008
		case JKEY_ISSUEID_RESETING:
			idx++;
			switch (val)
			{
				case ZWAVE_EVT_RESETING_START:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_RESETING, JVAL_REPORT_START);
					break;
				case ZWAVE_EVT_RESETING_DONE:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_RESET, JVAL_REPORT_DONE);
					break;
				case ZWAVE_EVT_RESETING_REMOVED:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_RESET, valp+idx);
					break;
				case ZWAVE_EVT_RESETING_FAIL:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_RESET, JVAL_COMM_FAIL);
					break;
				default:
					SAFE_SPRINTF_EX(response, JKEY_REPORT_OP_RESET);
					break;
			}
			SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_RESETING);
			break;
		// 0x00000009
		case JKEY_ISSUEID_OP:
			if (val==0)
			{
				SAFE_SPRINTF_EX(response, JKEY_REPORT_OP_NORMAL);
			}
			else
			{
				SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP, translate_nw_op(val) );
			}
			SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_OP);
			break;
		// 0x0000000A
		case JKEY_ISSUEID_REPLACING:
			idx++;
			switch (val)
			{
				case ZWAVE_EVT_REPLACING_START:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_REPLACING_FAILED, JVAL_REPORT_START);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				case ZWAVE_EVT_REPLACING_DONE:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_REPLACING_FAILED, JVAL_REPORT_DONE);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				case ZWAVE_EVT_REPLACING_REPLACED:
					SAFE_SPRINTF_EX(response, "%s,%s", node_str, JKEY_REPORT_OP_REPLACED_FAILED );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_REPLACING_FAIL:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_REPLACING_FAILED, JVAL_COMM_FAIL);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				default:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_REPLACING_FAILED, translate_op_rp_node(val) );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
			}

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_REPLACING);
			break;
		// 0x0000000B
		case JKEY_ISSUEID_RMFAILED:
			idx++;
			switch (val)
			{
				case ZWAVE_EVT_RMFAILED_REMOVED:
					SAFE_SPRINTF_EX(response, "%s,%s", node_str, JKEY_REPORT_OP_REMOVED_FAILED );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_RMFAILED_DONE:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_REMOVING_FAILED, JVAL_REPORT_DONE);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				case ZWAVE_EVT_RMFAILED_FAIL:
					SAFE_SPRINTF_EX(response, "%s,%s", JKEY_REPORT_OP_REMOVING_FAILED, JVAL_COMM_FAIL);
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
				default:
					SAFE_SPRINTF_EX(response, "%s,%d", JKEY_REPORT_OP_REMOVING_FAILED, val );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_COMM_STATE );
					break;
			}

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_RMFAILED);
			break;
		// 0x00000010
		case JKEY_ISSUEID_NODE:
			idx++;
			switch (val)
			{
				case ZWAVE_EVT_NODE_ALIVE:
					SAFE_SPRINTF_EX(response, "%s,%s", node_str, JVAL_NODE_ALIVE );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_NODE_SLEEPING:
					SAFE_SPRINTF_EX(response, "%s,%s", node_str, JVAL_NODE_SLEEPING );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
				case ZWAVE_EVT_NODE_INFO:
					{
						uint8_t propty = valp[idx++];
						uint16_t vid = byte2big_endian(2, (uint8_t *)valp+idx);
						idx+=2;
						uint16_t ptype = byte2big_endian(2, (uint8_t *)valp+idx);
						idx+=2;
						uint16_t pid = byte2big_endian(2, (uint8_t *)valp+idx);
						idx+=2;
						uint16_t proto_ver = byte2big_endian(2, (uint8_t *)valp+idx);
						idx+=2;
						uint16_t app_ver = byte2big_endian(2, (uint8_t *)valp+idx);
						idx+=2;
						uint8_t lib_type = valp[idx++];
						uint8_t category = valp[idx++];
						uint8_t sensor = valp[idx++];
						uint8_t sleep_cap = valp[idx++];
						uint8_t listen = valp[idx++];
						int32_t wkup_intv = (int32_t)byte2big_endian(4, (uint8_t *)valp+idx);
						idx+=4;
						uint8_t s2_keys_valid = valp[idx++];
						uint8_t s2_grnt_keys = valp[idx++];

						SAFE_SPRINTF_EX(response, "%s,%s,%d-%s,0x%04x,0x%04x,0x%04x,%u.%02u,%u.%02u,%u,%u,%u,%u,%u,%d,%u,0x%02x-%s", node_str, JKEY_REPORT_NODE_INFO,
							propty, translate_node_propty(propty, 1),
							vid, ptype, pid,
							(unsigned)(proto_ver >> 8), (unsigned)(proto_ver & 0xFF),
							(unsigned)(app_ver >> 8), (unsigned)(app_ver & 0xFF),
							lib_type, category, sensor, sleep_cap, listen, wkup_intv, 
							s2_keys_valid, s2_grnt_keys, translate_requested_keys(s2_grnt_keys, 1)
							);
						SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_ZWINFO );
					}
					break;
				case ZWAVE_EVT_NODE_TYPE:
					{
						uint8_t zwplus_ver = valp[idx++];
						uint8_t node_type = valp[idx++];
						uint8_t role_type = valp[idx++];
						uint16_t instr_icon = byte2big_endian(2, (uint8_t *)valp+idx);
						idx+=2;
						uint16_t usr_icon = byte2big_endian(2, (uint8_t *)valp+idx);
						idx+=2;

						SAFE_SPRINTF_EX(response, "%s,%u,0x%02x-%s,0x%02x-%s,0x%02x,0x%02x", node_str, zwplus_ver, node_type, translate_node_type(node_type), role_type, translate_role_type(node_type), instr_icon, usr_icon);
						SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_ZWINFO );
					}
					break;
				case ZWAVE_EVT_NODE_GENERIC:
					{
						uint8_t generic = valp[idx++];
						uint8_t specific = valp[idx++];

						SAFE_SPRINTF_EX(response, "%s,0x%02x-%s,0x%02x-%s", node_str, generic, translate_generic(generic), specific, translate_specific(generic, specific));
						SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_ZWINFO );
					}
					break;
				case ZWAVE_EVT_NODE_VENDOR:
					{
						uint16_t vid = byte2big_endian(2, (uint8_t *)valp+idx);
						idx+=2;
						uint16_t ptype = byte2big_endian(2, (uint8_t *)valp+idx);
						idx+=2;
						uint16_t pid = byte2big_endian(2, (uint8_t *)valp+idx);
						idx+=2;

						SAFE_SPRINTF_EX(response, "%s,%s,0x%04x,0x%04x,0x%04x", node_str, JKEY_REPORT_NODE_VENDOR, vid, ptype, pid);
						SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_ZWINFO );
					}
					break;
				default:
					SAFE_SPRINTF_EX(response, "%s,%s", node_str, JKEY_REPORT_NODE_XXX );
					SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
					break;
			}

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_NODE);
			break;
		// 0x00000011
		case JKEY_ISSUEID_NOP:
			SAFE_SPRINTF_EX(response, "%s,%s,", node_str, JKEY_REPORT_NOP );
			SAFE_STRCAT(response, buff);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_NOP);
			break;
		// 0x00010002
		case JKEY_ISSUEID_SMOKE:
			switch (val)
			{
				case ZWAVE_EVT_INACTIVE_CLEAR:
					SAFE_SPRINTF_EX(response, "%s,%s,%s", node_str, JKEY_REPORT_SMOKE, JVAL_COMM_OFF );
					break;
				case ZWAVE_EVT_SMOKE_L:
					SAFE_SPRINTF_EX(response, "%s,%s,%s,%s", node_str, JKEY_REPORT_SMOKE, JVAL_COMM_ON, valp+ZWAVE_IDX_ALRM_EX_EVT_PRM);
					break;
				case ZWAVE_EVT_SMOKE:
					SAFE_SPRINTF_EX(response, "%s,%s,%s", node_str, JKEY_REPORT_SMOKE, JVAL_COMM_ON );
					break;
				case ZWAVE_EVT_SMOKE_TEST:
				case ZWAVE_EVT_SMOKE_REPLA:
				default:
					SAFE_SPRINTF_EX(response, "%s,%s,%d-%s", node_str, JKEY_REPORT_SMOKE, val, translate_alarm_event(ZW_ALRM_SMOKE, val, 0));
					break;
			}
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_SMOKE);
			break;
		// 0x00010006
		case JKEY_ISSUEID_WATER:
			switch (val)
			{
				case ZWAVE_EVT_INACTIVE_CLEAR:
					SAFE_SPRINTF_EX(response, "%s,%s,%s", node_str, JKEY_REPORT_WATER, JVAL_COMM_OFF );
					break;
				case ZWAVE_EVT_LVL_L:
					SAFE_SPRINTF_EX(response, "%s,%s,%s,%s", node_str, JVAL_WATER_LEVEL_DROPPED, JVAL_COMM_ON, valp+ZWAVE_IDX_ALRM_EX_EVT_PRM);
					break;
				case ZWAVE_EVT_LVL:
					SAFE_SPRINTF_EX(response, "%s,%s,%s", node_str, JVAL_WATER_LEVEL_DROPPED, JVAL_COMM_ON );
					break;
				case ZWAVE_EVT_LEAK_L:
					SAFE_SPRINTF_EX(response, "%s,%s,%s,%s", node_str, JVAL_WATER_LEAK, JVAL_COMM_ON, valp+ZWAVE_IDX_ALRM_EX_EVT_PRM);
					break;
				case ZWAVE_EVT_LEAK:
				default:
					SAFE_SPRINTF_EX(response, "%s,%s,%s", node_str, JVAL_WATER_LEAK, JVAL_COMM_ON );
					break;
			}
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_WATER);
			break;
		// 0x00010008
		case JKEY_ISSUEID_TAMPER:
			switch (val)
			{
				case ZWAVE_EVT_INACTIVE_CLEAR:
					SAFE_SPRINTF_EX(response, "%s,%s,%s", node_str, JKEY_REPORT_TAMPER, JVAL_COMM_CLOSED );
					break;
				case ZWAVE_EVT_TMPR_COVER:
				default:
					SAFE_SPRINTF_EX(response, "%s,%s,%s", node_str, JKEY_REPORT_TAMPER, JVAL_COMM_OPEN );
					break;
			}
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_TAMPER);
			break;
		// 0x0001000A
		case JKEY_ISSUEID_DR_WIN:
			switch (val)
			{
				case ZWAVE_EVT_WINDOW_DOOR_OPEN:
					SAFE_SPRINTF_EX(response, "%s,%s,%s", node_str, JKEY_REPORT_DOOR, JVAL_COMM_OPEN );
					break;
				case ZWAVE_EVT_WINDOW_DOOR_CLOSED:
					SAFE_SPRINTF_EX(response, "%s,%s,%s", node_str, JKEY_REPORT_DOOR, JVAL_COMM_CLOSED);
					break;
			}
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_DR_WIN);
			break;
		// 0x0001000C
		case JKEY_ISSUEID_MOTION:
			switch (val)
			{
				case ZWAVE_EVT_INACTIVE_CLEAR:
					SAFE_SPRINTF_EX(response, "%s,%s,%s", node_str, JKEY_REPORT_MOTION, JVAL_COMM_OFF);
					break;
				case ZWAVE_EVT_MOTION_DET_L:
					SAFE_SPRINTF_EX(response, "%s,%s,%s,%s", node_str, JKEY_REPORT_MOTION, JVAL_COMM_ON, valp+ZWAVE_IDX_ALRM_EX_EVT_PRM);
					break;
				case ZWAVE_EVT_MOTION_DET:
				default:
					SAFE_SPRINTF_EX(response, "%s,%s,%s", node_str, JKEY_REPORT_MOTION, JVAL_COMM_ON );
					break;
			}
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_MOTION);
			break;
		// 0x00030001
		case JKEY_ISSUEID_TEMPERATURE:
			{
				uint8_t type = valp[idx++];
				uint8_t prec = valp[idx++];
				uint8_t unit = valp[idx++];
				uint8_t size = valp[idx++];
				uint8_t *data = (uint8_t *)valp+idx;

				int32_t value = byte2big_endian(size, data);
				int32_t precision = pow(10, prec);

				SAFE_SPRINTF_EX(response, "%s,%s,%u-%s,%.3f,%u-%s", node_str, JKEY_REPORT_TEMPERATURE, type, translate_sensor_type(type), (float)value/(float)precision, unit, translate_sensor_unit(type, unit));
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
			}

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_TEMPATURE);
			break;
		// 0x00030003
		case JKEY_ISSUEID_LUM:
			{
				uint8_t type = valp[idx++];
				uint8_t prec = valp[idx++];
				uint8_t unit = valp[idx++];
				uint8_t size = valp[idx++];
				uint8_t *data = (uint8_t *)valp+idx;

				int32_t value = byte2big_endian(size, data);
				int32_t precision = pow(10, prec);

				SAFE_SPRINTF_EX(response, "%s,%s,%u-%s,%.3f,%u-%s", node_str, JKEY_REPORT_LUMINANCE, type, translate_sensor_type(type), (float)value/(float)precision, unit, translate_sensor_unit(type, unit));
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );

				if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_LUMINANCE);
			}
			break;
		// 0x00030005
		case JKEY_ISSUEID_HUMD:
			{
				uint8_t type = valp[idx++];
				uint8_t prec = valp[idx++];
				uint8_t unit = valp[idx++];
				uint8_t size = valp[idx++];
				uint8_t *data = (uint8_t *)valp+idx;

				int32_t value = byte2big_endian(size, data);
				int32_t precision = pow(10, prec);
				
				SAFE_SPRINTF_EX(response, "%s,%s,%u-%s,%.3f,%u-%s", node_str, JKEY_REPORT_HUMIDITY, type, translate_sensor_type(type), (float)value/(float)precision, unit, translate_sensor_unit(type, unit));
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );

				if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_HUMIDITY);
			}
			break;
		// 0x00092001
		case JKEY_ISSUEID_CC_BASIC:
			{
				uint8_t curr_val = valp[idx++]; // 0=off; 0x01~0x63 = percentage (%); 0xFE=Unknown; 0xFF=On
				uint8_t tgt_val = valp[idx++]; // same curr_val
				uint8_t dur = valp[idx++]; // 0=done; 0x01~0x7F=seconds; 0x80~0xFD=1~126 minutes; 0xFE=Unknown duration; 0xFF=reserved

				SAFE_SPRINTF_EX(response, "%s,%s,%u-%s,%u-%s,%u-%s", node_str, JKEY_REPORT_BASIC, curr_val, translate_basic_val(curr_val), tgt_val, translate_basic_val(tgt_val), dur, translate_dur(dur));
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );

				if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_BASIC);
			}
			break;
		// 0x00092002
		case JKEY_ISSUEID_CC_BASIC_SET:
			SAFE_SPRINTF_EX(response, "%s,%s,%u", node_str, JKEY_REPORT_BASIC_SET, val);
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_BASIC_SET);
			break;
		// 0x00092501
		case JKEY_ISSUEID_CC_SWITCH:
		// 0x00092503
		case JKEY_ISSUEID_CC_SWITCH_PLUG:
			{
				uint8_t curr_val = valp[idx++]; // 0=off; 0xFE=Unknown; 0xFF=On
				uint8_t tgt_val = valp[idx++]; // same curr_val
				uint8_t dur = valp[idx++]; // 0=done; 0x01~0x7F=seconds; 0x80~0xFD=1~126 minutes; 0xFE=Unknown duration; 0xFF=reserved

				SAFE_SPRINTF_EX(response, "%s,%s,%u-%s,%u-%s,%u-%s", node_str, JKEY_REPORT_SWITCH, curr_val, translate_switch_val(val), tgt_val, translate_switch_val(tgt_val), dur, translate_dur(dur));
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
				if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_SWITCH);
			}
			break;
		// 0x00092502
		case JKEY_ISSUEID_CC_SWITCH_SIREN:
			{
				SAFE_SPRINTF_EX(response, "%s,%s,%d", node_str, JKEY_REPORT_SIREN, val );
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
			}
			break;
		// 0x00092601
		case JKEY_ISSUEID_CC_DIMMER:
			{
				uint8_t curr_val = valp[idx++];
				uint8_t tgt_val = valp[idx++];
				uint8_t dur = valp[idx++];

				SAFE_SPRINTF_EX(response, "%s,%s,%u-%s,%u-%s,%u-%s", node_str, JKEY_REPORT_DIMMER, curr_val, translate_multilevel_level(curr_val), tgt_val, translate_multilevel_level(tgt_val), dur, translate_dur(dur));
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
				if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_DIMMER);
			}
			break;
		// 0x00092701
		case JKEY_ISSUEID_CC_SWITCH_ALL:
			SAFE_SPRINTF_EX(response, "%s,%s,0x%02x", node_str, JKEY_REPORT_SWITCH_ALL, val );
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
			break;
		// 0x00093002
		case JKEY_ISSUEID_CC_BSENSOR_SUP:
			{
				uint8_t types_len = valp[idx++];
				uint8_t *types = (unsigned char *)valp+idx;

				int i = 0;
				SAFE_SPRINTF_EX(response, "%s,%s,%u", node_str, JKEY_REPORT_BSENSOR_SUP, types_len );
				for (i=0; i<types_len; i++ )
				{
					char tmpbuf[LEN_OF_BUF256] = "";
					uint8_t type_v = types[i];
					SAFE_SPRINTF_EX(tmpbuf, ",%u-%s", type_v, translate_bsensor_type(type_v));
					SAFE_STRCAT(response, tmpbuf);
				}
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
			}
			break;
		// 0x00093102
		case JKEY_ISSUEID_CC_SENSOR_SUP:
			{
				uint8_t types_len = valp[idx++];
				uint8_t *types = (unsigned char *)valp+idx;

				int i = 0;
				SAFE_SPRINTF_EX(response, "%s,%s,%u", node_str, JKEY_REPORT_SENSOR_SUP, types_len );
				for (i=0; i<types_len; i++ )
				{
					char tmpbuf[LEN_OF_BUF256]="";
					uint8_t type_v = types[i];
					SAFE_SPRINTF_EX(tmpbuf, ",%u-%s", type_v, translate_sensor_type(type_v));
					SAFE_STRCAT(response, tmpbuf);
				}
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
			}
			break;
		// 0x00093103
		case JKEY_ISSUEID_CC_SENSOR_UNIT_SUP:
			{
				uint8_t sensor_type = valp[idx++];
				uint8_t sensor_unit_msk = valp[idx++];

				int i = 0;
				SAFE_SPRINTF_EX(response, "%s,%s,%u-%s,%u", node_str, JKEY_REPORT_SENSOR_UNIT_SUP, sensor_type, translate_sensor_type( sensor_type ), sensor_unit_msk);
				for (i=0; i<4; i++)
				{
					if ( sensor_unit_msk & (uint8_t)pow(2, i))
					{
						char tmpbuf[LEN_OF_BUF256]="";
						SAFE_SPRINTF_EX(tmpbuf, ",%u-%s", i, translate_sensor_unit(sensor_type, i));
						SAFE_STRCAT(response, tmpbuf);
					}
				}
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
			}
			break;
		// 0x00093201
		case JKEY_ISSUEID_CC_METER:
			{
				uint8_t typex = valp[idx++];
				uint8_t prec = valp[idx++];
				uint8_t unit = valp[idx++];
				uint8_t size = valp[idx++];
				uint8_t *data = (unsigned char *)valp+idx;
				idx+=4;
				uint16_t delta_time = byte2big_endian(2, (uint8_t *)valp+idx);
				idx+=2;
				uint8_t *prv_data = (unsigned char *)valp+idx;
				idx+=4;
				uint8_t rate_type = valp[idx++];

				int32_t value = byte2big_endian(size, data);
				int32_t prv_value = byte2big_endian(size, prv_data);
				int32_t precision = pow(10, prec);

				//SAFE_SPRINTF_EX(response, "%s,%s,%u,%u,%u,%d,%.3f,%.3f", node_str, JSTR_REPORT_METER, typex, unit, rate_type, delta_time, (float)value/(float)precision, (float)prv_value/(float)precision);
				SAFE_SPRINTF_EX(response, "%s,%s,%u-%s,%u-%s,%u-%s,%d-delta_time,%.3f,%.3f", node_str, JKEY_REPORT_METER, 
					typex, translate_meter_type(typex), 
					unit, translate_meter_unit(typex, unit), 
					rate_type, translate_meter_rate(rate_type),
					delta_time,
					(float)value/(float)precision, (float)prv_value/(float)precision);
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
			}

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_METER);
			break;
		// 0x00093202
		case JKEY_ISSUEID_CC_METER_SUP:
			{
				uint8_t typex = valp[idx++];
				uint8_t reset_cap = (uint8_t)valp[idx++];
				uint8_t unit_sup = (uint8_t)valp[idx++];
				char unit_sup_name[128]="";
				translate_meter_unit_sup(typex, unit_sup, unit_sup_name);

				SAFE_SPRINTF_EX(response, "%s,%s,%u-%s,%u-%s,%u-%s", node_str, JKEY_REPORT_METER_SUP, 
					typex, translate_meter_type(typex), reset_cap, reset_cap==1?"reset supported": "not supported",
					unit_sup, unit_sup_name);
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
			}
			break;
		// 0x00095b01
		case JKEY_ISSUEID_CC_CENTRAL_SCENE_NOTIFICATION:
			{
				uint8_t seqNo = valp[idx++]; /**< Sequence number. Incremented each time a new report is issued by the device. */
				uint8_t keyAttr = valp[idx++]; /**< Key attribute. ZW_CSC_KEY_ATTRIB_XXX.*/
				uint8_t sceneNo = valp[idx++]; /**< Scene Number. Actual scene identifier.*/
				uint8_t slow_rfsh = valp[idx++]; /**< Slow refresh of "Key Held Down" notification. Non-zero=enable; 0=disable. */

				SAFE_SPRINTF_EX(response, "%s,%s,%u,%u-%s,%u,%u", node_str, JKEY_REPORT_CSC_NOTIFICATION, seqNo, keyAttr, translate_csc_keyAttr(keyAttr), sceneNo, slow_rfsh);
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
				if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_CENTRAL_SCENE_NOTIFICATION);
			}
			break;
		// 0x00095b02
		case JKEY_ISSUEID_CC_CENTRAL_SCENE_SUP:
			{
				uint8_t scene_cnt = valp[idx++]; // value
				uint8_t slow_rfsh = valp[idx++]; // value
				uint8_t sameKA = valp[idx++]; // value
				uint8_t KA_array_len = valp[idx++]; // value
				uint8_t *KA_array = (uint8_t *)valp+idx;

				int i = 0;
				SAFE_SPRINTF_EX(response, "%s,%s,%u,%u,%u,%u", node_str, JKEY_REPORT_CSC_SUP, scene_cnt, slow_rfsh, sameKA, KA_array_len);
				for (i=0; i<KA_array_len; i++)
				{
					char tmpbuf[LEN_OF_BUF256]="";
					SAFE_SPRINTF_EX(tmpbuf, ",%u", KA_array[i]);
					SAFE_STRCAT(response, tmpbuf);
				}

				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
			}
			break;
		// 0x00095b03
		case JKEY_ISSUEID_CC_CENTRAL_SCENE_CFG:
			{
				uint8_t slow_refresh = valp[0];
				SAFE_SPRINTF_EX(response, "%s,%s,%u", node_str, JKEY_REPORT_CSC_CFG, slow_refresh);
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
			}
			break;
		// 0x00097001
		case JKEY_ISSUEID_CC_CONFIGURATION:
			{
				uint8_t use_default = (uint8_t)valp[idx++];
				uint8_t param_num = (uint8_t)valp[idx++];
				uint8_t size = (uint8_t)valp[idx++];
				uint8_t *data = (uint8_t *)valp+idx;

				int32_t value = byte2big_endian(size, data);

				//SAFE_SPRINTF_EX(response, "%s,%s,%u,%u,%u,%u,%u,%u,%u", node_str, JSTR_REPORT_CFG, use_default, param_num, size, data[0], data[1], data[2], data[3]);
				SAFE_SPRINTF_EX(response, "%s,%s,%u-use_default,%u-param_num,%d", node_str, JKEY_REPORT_CFG, use_default, param_num, value);
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
				if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_CONFIGURATION);
			}
			break;
		// 0x00097501
		case JKEY_ISSUEID_CC_PROTECTION:
			{
				uint8_t local_prot = (uint8_t)valp[idx++];
				uint8_t rf_prot = (uint8_t)valp[idx++];
				SAFE_SPRINTF_EX(response, "%s,%s,%u-%s,%u-%s", node_str, JKEY_REPORT_PROTECTION, local_prot, translate_local_prot(local_prot), rf_prot, translate_rf_prot(rf_prot));
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
			}
			break;
		// 0x00098001
		case JKEY_ISSUEID_CC_BATTERY:
			// 0xFF-low, 0 to 100% (0x00 - 0x64)
			SAFE_SPRINTF_EX(response, "%s,%s,%u-%s", node_str, JKEY_REPORT_BATTERY, val, translate_battery_level(val) );
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );

			if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_BATTERY);
			break;
		// 0x00098401
		case JKEY_ISSUEID_CC_WAKE_UP_INTERVAL:
			{
				uint32_t imin = byte2big_endian(4, (uint8_t *)valp+idx); idx+=4;
				uint32_t imax = byte2big_endian(4, (uint8_t *)valp+idx); idx+=4;
				uint32_t idef = byte2big_endian(4, (uint8_t *)valp+idx); idx+=4;
				uint32_t interval = byte2big_endian(4, (uint8_t *)valp+idx); idx+=4;
				uint32_t cur = byte2big_endian(4, (uint8_t *)valp+idx); idx+=4;
				uint8_t nodeid = valp[idx];
			
				SAFE_SPRINTF_EX(response, "%s,%s,%d,%d,%d,%d,%d,%u", node_str, JKEY_REPORT_WAKEUP_INTERVAL, imin, imax, idef, interval, cur, nodeid);
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );

				if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_WAKE_UP_INTERVAL);
			}
			break;
		// 0x00098402
		case JKEY_ISSUEID_CC_WAKE_UP_NOTIFICATION:
			{
				SAFE_SPRINTF_EX(response, "%s,%s", node_str, JKEY_REPORT_WAKEUP);
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );

				if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_WAKE_UP_NOTIFICATION);
			}
			break;
		// 0x00098501
		case JKEY_ISSUEID_CC_GROUP:
			{
				uint8_t group = valp[idx++]; 
				uint8_t max_cnt = valp[idx++];
				uint8_t cnt = valp[idx++];
				int i = 0;

				SAFE_SPRINTF_EX(response, "%s,%s,%u-group,%u-max_cnt,%u-cnt", node_str, JKEY_REPORT_GROUP, group, max_cnt, cnt);
				if (cnt>0)
				{
					for (i=0; i<cnt; i++ )
					{
						char tmpbuf[16]="";
						uint8_t node_id = valp[idx+i*2];
						uint8_t ep_id = valp[idx+i*2];
						SAFE_SPRINTF_EX(tmpbuf, ",%u.%u", node_id, ep_id);
						SAFE_STRCAT(response, tmpbuf);
					}
				}
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
			}
			break;
		// 0x00098502
		case JKEY_ISSUEID_CC_GROUP_SUP:
			SAFE_SPRINTF_EX(response, "%s,%s,%d", node_str, JKEY_REPORT_GROUP_SUP, val );
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
			break;
		// 0x00098503
		case JKEY_ISSUEID_CC_GROUP_ACTVIE:
			SAFE_SPRINTF_EX(response, "%s,%s,%d", node_str, JKEY_REPORT_GROUP_ACTIVE, val );
			SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );
			break;
		case JKEY_ISSUEID_CC_INFRARED:
			{
				uint8_t cmd = valp[idx++];
				uint8_t state = valp[idx++];
				uint8_t key[LEN_OF_NAME32]="";
				SAFE_MEMCPY(key, valp+idx, LEN_OF_NAME32, LEN_OF_NAME32);
				idx+=LEN_OF_NAME32;
				uint16_t size = byte2big_endian(2, (uint8_t *)valp+idx);
				idx+=2;

				SAFE_SPRINTF_EX(response, "%s,%s,%c,%u-%s,%s,%d", node_str, JKEY_REPORT_INFRARED, cmd, state, translate_infrared_state(state), key, size);
				SAFE_SPRINTF_EX(tag, "%s", JKEY_REPORT_EVT );

				if (dbg==0) tracker_watch_func_cb(userdata, c_issueitem, NOTIFY_FN_ID_INFRARED);
			}
			break;
		default:
			SAFE_SPRINTF_EX(tag, "0x%08x", issueid );
			break;
	}

	if (tracker_watch==1)
	{
		char id_frm[LEN_OF_BUF256] = "";
		SAFE_SPRINTF_EX(id_frm, "%s/%s/%s/%d", c_macid_frm, c_uuid_frm, c_nodeid_frm, epid_frm);
		ZWWATCH_DBG("%s [%d/%d-%s] - <-- %s - %s=%s", time_now_short((time_t)timestamp), methodid, protocolid, translate_protocolid(protocolid), id_frm, tag, response);
	}

	if (dbg==0) tracker_watch_cb(userdata, c_issueitem);
}

void tracker_watch_set_on(void)
{
	tracker_watch = 1;
}

void tracker_watch_set_off(void)
{
	tracker_watch = 0;
}

static void tracker_parser(void *userdata, unsigned char *payload, int payload_len)
{
	IssueItem_t *c_issueitem = (IssueItem_t *)payload;

	Commander_t *commander = &c_issueitem->commander;
	uint16_t methodid = commander->methodid;

	char *c_macid_frm = commander->id_frm.c_macid;
	//char *c_uuid_frm = commander->id_frm.c_uuid;
	//char *c_nodeid_frm = commander->id_frm.c_nodeid;
	//uint8_t epid_frm = commander->id_frm.epid;

	char *c_macid_to = commander->id_to.c_macid;
	//char *c_uuidd_to = commander->id_to.c_uuid;
	//char *c_nodeid_to = commander->id_to.c_nodeid;
	//uint8_t epidd_to = commander->id_to.epid;

	//uint32_t issueid = c_issueitem->issueid;
	//uint16_t mode = c_issueitem->mode;
	//uint16_t submode = c_issueitem->submode;
	//uint16_t category = c_issueitem->category;
	//uint32_t timestamp = c_issueitem->timestamp;
	//uint16_t data_len = c_issueitem->data_len;

	int match = 0;
	DBG_TR_LN("(my_macid: %s, macid_frm: %s, macid_to: %s)", my_macid, c_macid_frm, c_macid_to);
	switch (methodid)
	{
		case JVAL_METHODID_EVENT:
			if ( (SAFE_STRCMP(c_macid_frm, JVAL_C_MAC_BROADCAST) == 0) || (SAFE_STRCMP(my_macid, JVAL_C_MAC_BROADCAST) == 0) || (SAFE_STRCMP(my_macid, c_macid_frm) == 0) )
			{
				tracker_watch_simple(userdata, c_issueitem, 0);
				match = 1;
			}
			break;
		case JVAL_METHODID_PUT:
			if ( (SAFE_STRCMP(c_macid_to, JVAL_C_MAC_BROADCAST) == 0) || (SAFE_STRCMP(my_macid, JVAL_C_MAC_BROADCAST) == 0) || (SAFE_STRCMP(my_macid, c_macid_to) == 0) )
			{
				tracker_order_simple(userdata, c_issueitem, 0);
				match = 1;
			}
			break;
		default:
			if ( (SAFE_STRCMP(c_macid_to, JVAL_C_MAC_BROADCAST) == 0) || (SAFE_STRCMP(my_macid, JVAL_C_MAC_BROADCAST) == 0) || (SAFE_STRCMP(my_macid, c_macid_to) == 0) )
			{
				tracker_others_simple(userdata, c_issueitem, 0);
				match = 1;
			}
			break;
	}

	if ((match==1) && (tracker_logging))
	{
		tracker_logging_push(c_issueitem);
	}
}

void tracker_send(char *buff, int buff_len)
{
	if ( (chainX_mctt) && (chainX_quit_check(chainX_mctt)==0) )
	{
		mctt_publish(chainX_mctt, buff, buff_len);
	}
}

void tracker_close(void)
{
	if (chainX_mctt)
	{
		mctt_thread_close(chainX_mctt);
		chainX_mctt = NULL;
	}

	tracker_logging_free( tracker_logging_head() );
	tracker_notify_free( tracker_notify_head() );
}

void tracker_init(void *userdata, char *macid)
{
	chainX_mctt = mctt_thread_init(userdata, MCTT_IP, MCTT_PORT, tracker_parser);

	SAFE_SPRINTF_EX(my_macid, "%s", macid);

	tracker_logging_open( tracker_logging_head() );
	tracker_notify_open( tracker_notify_head() );

	translater_open();
}

