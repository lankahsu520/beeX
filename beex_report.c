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
#include "beex_tracker.h"

//static char my_mac[LEN_OF_MAC] = MAC_NULL;
//static char my_uuid[LEN_OF_UUID] = UUID_NULL;
//static uint8_t my_ctl_id = JVAL_NODEID_CONTROLLER;

static int is_report = 0;

void commander_set_methodid(Commander_t *commander, uint16_t methodid)
{
	POINTER_CHECK(commander);

	commander->methodid = methodid;
}

void commander_set_protocolid(Commander_t *commander, uint16_t protocolid)
{
	POINTER_CHECK(commander);

	commander->protocolid = protocolid;
}

void commander_set_to_nodeid(Commander_t *commander, char *c_nodeid, uint8_t epid)
{
	POINTER_CHECK(commander);

	SAFE_SPRINTF(commander->id_to.c_nodeid, "%s", c_nodeid);
	commander->id_to.epid = epid;
}

void commander_set_to_nodeid_ex(Commander_t *commander, uint16_t nodeid, uint8_t epid)
{
	char c_nodeid[LEN_OF_UUID]="";
	SAFE_SPRINTF(c_nodeid, "%d", nodeid);
	commander_set_to_nodeid(commander, c_nodeid, epid);
}

void commander_set_to_mac(Commander_t *commander, char *c_macid)
{
	POINTER_CHECK(commander);
	POINTER_CHECK(c_macid);

	SAFE_SPRINTF(commander->id_to.c_macid, "%s", c_macid);
}

void commander_set_to_uuid(Commander_t *commander, char *c_uuid)
{
	POINTER_CHECK(commander);
	POINTER_CHECK(c_uuid);

	SAFE_SPRINTF(commander->id_to.c_uuid, "%s", c_uuid);
}

void commander_set_frm_nodeid(Commander_t *commander, char *c_nodeid, uint8_t epid)
{
	POINTER_CHECK(commander);

	SAFE_SPRINTF(commander->id_frm.c_nodeid, "%s", c_nodeid);
	commander->id_frm.epid = epid;
}

void commander_set_frm_nodeid_ex(Commander_t *commander, uint16_t nodeid, uint8_t epid)
{
	char c_nodeid[LEN_OF_UUID]="";
	SAFE_SPRINTF(c_nodeid, "%d", nodeid);
	commander_set_frm_nodeid(commander, c_nodeid, epid);
}

void commander_set_frm_mac(Commander_t *commander, char *c_macid)
{
	POINTER_CHECK(commander);
	POINTER_CHECK(c_macid);

	SAFE_SPRINTF(commander->id_frm.c_macid, "%s", c_macid);
}

void commander_set_frm_uuid(Commander_t *commander, char *c_uuid)
{
	POINTER_CHECK(commander);
	POINTER_CHECK(c_uuid);

	SAFE_SPRINTF(commander->id_frm.c_uuid, "%s", c_uuid);
}

void commander_cp_to(Commander_t *commander, Reporter_t *reporter)
{
	POINTER_CHECK(commander);
	POINTER_CHECK(reporter);

	SAFE_MEMCPY(&commander->id_to, &reporter->id_frm, sizeof(Identity_t), sizeof(Identity_t));
}

void commander_cp_frm(Commander_t *commander, Reporter_t *reporter)
{
	POINTER_CHECK(commander);
	POINTER_CHECK(reporter);

	SAFE_MEMCPY(&commander->id_frm, &reporter->id_frm, sizeof(Identity_t), sizeof(Identity_t));
}

void reporter_set_nodeid(Reporter_t *reporter, char *c_nodeid, uint8_t epid)
{
	POINTER_CHECK(reporter);

	SAFE_SPRINTF(reporter->id_frm.c_nodeid, "%s", c_nodeid);
	reporter->id_frm.epid = epid;
}

void reporter_set_nodeid_ex(Reporter_t *reporter, uint16_t nodeid, uint8_t epid)
{
	char c_nodeid[LEN_OF_UUID]="";
	SAFE_SPRINTF(c_nodeid, "%d", nodeid);
	reporter_set_nodeid(reporter, c_nodeid, epid);
}

void reporter_set_methodid(Reporter_t *reporter, uint16_t methodid)
{
	POINTER_CHECK(reporter);

	reporter->methodid = methodid;
}

void reporter_set_protocolid(Reporter_t *reporter, uint16_t protocolid)
{
	POINTER_CHECK(reporter);

	reporter->protocolid = protocolid;
}

void reporter_set_mac(Reporter_t *reporter, char *c_macid)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(c_macid);

	SAFE_SPRINTF(reporter->id_frm.c_macid, "%s", c_macid);
}

void reporter_set_uuid(Reporter_t *reporter, char *c_uuid)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(c_uuid);

	SAFE_SPRINTF(reporter->id_frm.c_uuid, "%s", c_uuid);
}

void reporter_close(void)
{
	tracker_close();
}

void reporter_init(void *userdata, char *macid)
{
	//POINTER_CHECK(userdata);
	POINTER_CHECK(macid);

	tracker_init(userdata, macid);
}

void do_report_enable(int enable)
{
	is_report = enable;
}

static int do_report_ready(uint32_t issue)
{
	int ret = 0;
	//if ( (issue <JKEY_ISSUEID_GATEWAY_MASK) || (is_report) )
	if (is_report)
	{
		return 1;
	}
	return ret;
}

static void do_report(IssueItem_t *n_issueitem)
{
	if (n_issueitem)
	{
		int send = 0;
		int payload_len = (int)sizeof(IssueItem_t) - (256-n_issueitem->data_len);
		uint32_t issueid = n_issueitem->issueid;

		if (do_report_ready(issueid))
		{
			tracker_send((char*)n_issueitem, payload_len);
			send ++;
		}

		if ( (is_report == 1) && ( send == 0 ) )
		{
			DBG_IF_LN("please send (issue: 0x%08x) !!!", issueid);
		}
	}
}

static void issueitem_command_gen(Commander_t *commander, uint32_t issueid, uint16_t mode, uint16_t submode, uint16_t category, IssueItem_t *n_issueitem)
{
	POINTER_CHECK(commander);
	POINTER_CHECK(n_issueitem);

	Commander_t *commanderX = &n_issueitem->commander;
	SAFE_MEMCPY(commanderX, commander, sizeof(Commander_t), sizeof(Commander_t));

	n_issueitem->issueid = issueid; // issueid
	n_issueitem->mode = mode; // mode
	n_issueitem->submode = submode; // submode
	n_issueitem->category = category; // category
	n_issueitem->timestamp = (uint32_t)time(NULL); // time
}

static void issueitem_report_gen(Reporter_t *reporter, uint32_t issueid, uint16_t mode, uint16_t submode, uint16_t category, IssueItem_t *n_issueitem)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(n_issueitem);

	Reporter_t *reporterX = &n_issueitem->reporter;
	SAFE_MEMCPY(reporterX, reporter, sizeof(Reporter_t), sizeof(Reporter_t));

	n_issueitem->issueid = issueid; // issueid
	n_issueitem->mode = mode; // mode
	n_issueitem->submode = submode; // submode
	n_issueitem->category = category; // category
	n_issueitem->timestamp = (uint32_t)time(NULL); // time
}

void controller_gw_ex(Reporter_t *reporter, uint8_t act, char *data, unsigned short data_len)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_GW, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = act; // value
	if ( (data_len) && (data) )
	{
		SAFE_MEMCPY(n_issueitem.data+idx, data, data_len, data_len);
		idx += data_len;
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_gw_cfg(Reporter_t *reporter, uint8_t param_num, uint32_t value)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	char data[0xFF] = {0};

	data[idx++] = param_num;
	big_endian2byte(4, value, (unsigned char *)data+idx );
	idx += 4;
	controller_gw_ex(reporter, ZWAVE_EVT_GW_CFG, data, idx);
}

void controller_gw_debug(Reporter_t *reporter, uint8_t lvl)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	char data[0xFF] = {0};

	data[idx++] = lvl;
	controller_gw_ex(reporter, ZWAVE_EVT_GW_DEBUG, data, idx);
}

void controller_gw_watch_off(Reporter_t *reporter, uint8_t watch_off)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	char data[0xFF] = {0};

	data[idx++] = watch_off;
	controller_gw_ex(reporter, ZWAVE_EVT_GW_WATCH_OFF, data, idx);
}

void controller_gw_echo(Reporter_t *reporter, char *echo)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(echo);

	if (SAFE_STRLEN(echo)>0)
	{
		controller_gw_ex(reporter, ZWAVE_EVT_GW_ECHO, echo, SAFE_STRLEN(echo));
	}
}

void controller_act_gw_ex(Commander_t *commander, uint8_t act, char *data, unsigned short data_len)
{
	POINTER_CHECK(commander);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_GW, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = act; // value
	if ( (data_len) && (data) )
	{
		SAFE_MEMCPY(n_issueitem.data+idx, data, data_len, data_len);
		idx += data_len;
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_act_gw_cfg(Commander_t *commander, uint8_t param_num, uint32_t value)
{
	POINTER_CHECK(commander);

	int idx = 0;
	char data[0xFF] = {0};

	data[idx++] = param_num;
	big_endian2byte(4, value, (unsigned char *)data+idx );
	idx += 4;
	controller_act_gw_ex(commander, ZWAVE_EVT_GW_CFG, data, idx);
}

void controller_act_gw_debug(Commander_t *commander, uint8_t lvl)
{
	POINTER_CHECK(commander);

	int idx = 0;
	char data[0xFF] = {0};

	data[idx++] = lvl;
	controller_act_gw_ex(commander, ZWAVE_EVT_GW_DEBUG, data, idx);
}

void controller_act_gw_watch_off(Commander_t *commander, uint8_t watch_off)
{
	POINTER_CHECK(commander);

	int idx = 0;
	char data[0xFF] = {0};

	data[idx++] = watch_off;
	controller_act_gw_ex(commander, ZWAVE_EVT_GW_WATCH_OFF, data, idx);
}

void controller_act_gw_echo(Commander_t *commander, char *echo)
{
	POINTER_CHECK(commander);
	POINTER_CHECK(echo);

	if (SAFE_STRLEN(echo)>0)
	{
		controller_act_gw_ex(commander, ZWAVE_EVT_GW_ECHO, echo, SAFE_STRLEN(echo));
	}
}

void controller_status_added(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_ADDING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_ADDED; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_adding_done(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_ADDING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_DONE; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_adding_sec(Reporter_t *reporter, uint8_t highest_lvl, uint8_t curr_lvl)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_ADDING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_SEC; // value
	n_issueitem.data[idx++] = highest_lvl; // value
	n_issueitem.data[idx++] = curr_lvl; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_adding_csa(Reporter_t *reporter, char *csa_pin)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(csa_pin);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_ADDING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_CSA; // value
	SAFE_SPRINTF((char*)n_issueitem.data + idx, "%s", csa_pin);
	idx += SAFE_STRLEN(csa_pin);
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_adding_ssa_set(Reporter_t *reporter, uint8_t res)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_ADDING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_SSA_SET; // value
	n_issueitem.data[idx++] = res;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_adding_ssa(Reporter_t *reporter,char *dsk)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(dsk);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_ADDING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_SSA; // value
	SAFE_SPRINTF((char*)n_issueitem.data + idx, "%s", dsk);
	idx += SAFE_STRLEN(dsk);
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_adding_xxx(Reporter_t *reporter, uint8_t flag)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_ADDING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = flag; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_adding_fail(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_ADDING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_FAIL; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_adding(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_ADDING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_START; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_act_adding(Commander_t *commander, char *dsk)
{
	POINTER_CHECK(commander);
	//POINTER_CHECK(dsk);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_ADDING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	if (dsk)
	{
		n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_SSA; // value
		SAFE_SPRINTF((char*)n_issueitem.data + idx, "%s", dsk);
		idx += SAFE_STRLEN(dsk);
	}
	else
	{
		n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_START; // value
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_removed(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_REMOVING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_REMOVING_REMOVED; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_removing_done(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_REMOVING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_REMOVING_DONE; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_removing_fail(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_REMOVING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_REMOVING_FAIL; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_removing(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_REMOVING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_REMOVING_START; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_act_removing(Commander_t *commander)
{
	POINTER_CHECK(commander);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_REMOVING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_REMOVING_START; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_rmfailed_rm(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_RMFAILED, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_RMFAILED_REMOVED; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_rmfailed_done(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_RMFAILED, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_RMFAILED_DONE; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_rmfailed_fail(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_RMFAILED, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_RMFAILED_FAIL; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_rmfailed(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_RMFAILED, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_RMFAILED_START; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_act_rmfailed(Commander_t *commander)
{
	POINTER_CHECK(commander);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_RMFAILED, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_RMFAILED_START; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_replaced(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_REPLACING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_REPLACING_REPLACED; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_replacing_done(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_REPLACING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_REPLACING_DONE; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_replacing_xxx(Reporter_t *reporter, uint8_t flag)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_REPLACING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = flag; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_replacing_fail(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_REPLACING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_REPLACING_FAIL; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_replacing(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_REPLACING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_REPLACING_START; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_act_replacing(Commander_t *commander, char *dsk)
{
	POINTER_CHECK(commander);
	//POINTER_CHECK(dsk);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_REPLACING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	if (dsk)
	{
		n_issueitem.data[idx++] = ZWAVE_EVT_REPLACING_SSA; // value
		SAFE_SPRINTF((char*)n_issueitem.data + idx, "%s", dsk);
		idx += SAFE_STRLEN(dsk);
	}
	else
	{
		n_issueitem.data[idx++] = ZWAVE_EVT_REPLACING_START; // value
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_reset_rm(Reporter_t *reporter, char *homeid)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(homeid);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_RESETING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_RESETING_REMOVED; // value
	int ilen = SAFE_STRLEN(homeid);
	if (ilen > 0)
	{
		SAFE_SPRINTF(n_issueitem.data+idx, "%s", homeid);
		idx += ilen;
	}
	idx ++;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_reset_done(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_RESETING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_RESETING_DONE; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_reset_fail(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_RESETING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_RESETING_FAIL; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_reseting(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_RESETING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_RESETING_START; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_act_reseting(Commander_t *commander)
{
	POINTER_CHECK(commander);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_RESETING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_RESETING_START; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_aborting_done(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_ABORTING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ABORTING_DONE; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_aborting_fail(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_ABORTING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ABORTING_FAIL; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_act_aborting(Commander_t *commander)
{
	POINTER_CHECK(commander);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_ABORTING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ABORTING_START; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_learned(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_LEARNING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_LEARNING_LEARNED; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_learning_done(Reporter_t *reporter, uint8_t reset)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_LEARNING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_LEARNING_DONE; // value
	n_issueitem.data[idx++] = reset; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_learning_ssa(Reporter_t *reporter, char *ssa_pin)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(ssa_pin);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_LEARNING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_LEARNING_SSA; // value
	SAFE_SPRINTF((char*)n_issueitem.data + idx, "%s", ssa_pin);
	idx += SAFE_STRLEN(ssa_pin);
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_learning_xxx(Reporter_t *reporter, uint8_t sts)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_LEARNING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = sts; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_learning_fail(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_LEARNING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_LEARNING_FAIL; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_learning(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_LEARNING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_LEARNING_START; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_act_learning(Commander_t *commander)
{
	POINTER_CHECK(commander);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_LEARNING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_LEARNING_START; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_ready(Reporter_t *reporter, uint8_t last_op, uint16_t nodeid)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_READY, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = last_op; // value
	big_endian2byte(2, nodeid, (unsigned char *)n_issueitem.data+idx );
	idx += 2;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_op(Reporter_t *reporter, uint8_t curr_op)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_OP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = curr_op; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_save(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	controller_status_op(reporter, ZWNET_OP_SAVE_NW);
}

void controller_status_normal(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	controller_status_op(reporter, ZWNET_OP_NONE);
}

void controller_status_node_info(Reporter_t *reporter, zwnoded_p noded_p)
{
	POINTER_CHECK(reporter);
	//POINTER_CHECK(noded_p);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_NODE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_NODE_INFO; // value

	if (noded_p)
	{
		n_issueitem.data[idx++] = noded_p->propty; // value
		big_endian2byte(2, noded_p->vid, (unsigned char *)n_issueitem.data+idx );
		idx += 2;
		big_endian2byte(2, noded_p->type, (unsigned char *)n_issueitem.data+idx );
		idx += 2;
		big_endian2byte(2, noded_p->pid, (unsigned char *)n_issueitem.data+idx );
		idx += 2;
		big_endian2byte(2, noded_p->proto_ver, (unsigned char *)n_issueitem.data+idx );
		idx += 2;
		big_endian2byte(2, noded_p->app_ver, (unsigned char *)n_issueitem.data+idx );
		idx += 2;
		n_issueitem.data[idx++] = noded_p->lib_type; // value
		n_issueitem.data[idx++] = noded_p->category; // value
		n_issueitem.data[idx++] = noded_p->sensor; // value
		n_issueitem.data[idx++] = noded_p->sleep_cap; // value
		n_issueitem.data[idx++] = noded_p->listen; // value
		big_endian2byte(4, noded_p->wkup_intv, (unsigned char *)n_issueitem.data+idx );
		idx += 4;
		n_issueitem.data[idx++] = noded_p->s2_keys_valid; // value
		n_issueitem.data[idx++] = noded_p->s2_grnt_keys; // value
	//#ifdef DEVICE_BASIC_CLASS_SHOW
		n_issueitem.data[idx++] = noded_p->basic; // value
	//#endif
	}
	else
	{
		idx += sizeof(zwnoded_t);
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_node_vendor(Reporter_t *reporter, uint16_t vid, uint16_t ptype, uint16_t pid)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_NODE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_NODE_VENDOR; // value
	big_endian2byte(2, vid, (unsigned char *)n_issueitem.data+idx );
	idx += 2;
	big_endian2byte(2, ptype, (unsigned char *)n_issueitem.data+idx );
	idx += 2;
	big_endian2byte(2, pid, (unsigned char *)n_issueitem.data+idx );
	idx += 2;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_node_alive(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_NODE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_NODE_ALIVE; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_node_sleeping(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_NODE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_NODE_SLEEPING; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_node_type(Reporter_t *reporter, zwplus_info_t *zwplus_info_p)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_NODE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_NODE_TYPE; // value
	n_issueitem.data[idx++] = zwplus_info_p->zwplus_ver; // value
	n_issueitem.data[idx++] = zwplus_info_p->node_type; // value
	n_issueitem.data[idx++] = zwplus_info_p->role_type; // value
	big_endian2byte(2, zwplus_info_p->instr_icon, (unsigned char *)n_issueitem.data+idx );
	idx += 2;
	big_endian2byte(2, zwplus_info_p->usr_icon, (unsigned char *)n_issueitem.data+idx );
	idx += 2;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_node_generic(Reporter_t *reporter, uint8_t generic, uint8_t specific)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_NODE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_NODE_GENERIC; // value
	n_issueitem.data[idx++] = generic; // value
	n_issueitem.data[idx++] = specific; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_act_node_see_you(Commander_t *commander)
{
	POINTER_CHECK(commander);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_NODE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_NODE_SEE_YOU; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_act_node_hello(Commander_t *commander)
{
	POINTER_CHECK(commander);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_NODE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_NODE_HELLO; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_nw_ex(Reporter_t *reporter, uint8_t act, char *data, unsigned short data_len)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_NW, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = act; // value
	if ( (data_len) && (data) )
	{
		SAFE_MEMCPY(n_issueitem.data+idx, data, data_len, data_len);
		idx += data_len;
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_nw_noroute(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_NW, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_NW_NOROUTE; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_nw_busy(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_NW, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_NW_BUSY; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_nw_no_ack(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_NW, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_NW_NO_ACK; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_nw_sys_err(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_NW, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_NW_SYS_ERR; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_nw_homeid(Reporter_t *reporter, zwnetd_p nw_desp, char *uuid, uint16_t ctl_id, uint16_t suc)
{
	POINTER_CHECK(reporter);
	//POINTER_CHECK(nw_desp);
	char homeid[LEN_OF_UUID] = JVAL_C_UUID_NULL;

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_NW, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_NW_HOMEID; // value
	if (nw_desp)
	{
		n_issueitem.data[idx++] = nw_desp->ctl_id; // value
		n_issueitem.data[idx++] = nw_desp->ctl_role; // value
		n_issueitem.data[idx++] = nw_desp->ctl_cap; // value
		n_issueitem.data[idx++] = nw_desp->ctl_zw_role; // value
		SAFE_SPRINTF(homeid, "%08X", nw_desp->id);
	}
	else
	{
		idx += sizeof(zwnetd_t);
	}
	big_endian2byte(2, ctl_id, (unsigned char *)n_issueitem.data+idx );
	idx += 2;
	big_endian2byte(2, suc, (unsigned char *)n_issueitem.data+idx );
	idx += 2;

	if ( (uuid) && SAFE_STRLEN(uuid) )
	{
		SAFE_SPRINTF(homeid, "%s", uuid);
	}

	int ilen = SAFE_STRLEN(homeid);
	SAFE_SPRINTF(n_issueitem.data+idx, "%s", homeid); // value
	idx += ilen;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_nw_serial(Reporter_t *reporter, SerialInitData_t *serial)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(serial);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_NW, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_NW_SERIAL; // value
	n_issueitem.data[idx++] = serial->moduleVer; // value
	n_issueitem.data[idx++] = serial->capability; // value
	n_issueitem.data[idx++] = serial->chipType; // value
	n_issueitem.data[idx++] = serial->chipVer; // value
	SAFE_MEMCPY(n_issueitem.data+idx, serial->nodelist, 32, 32);
	idx+= 32;
	n_issueitem.data[idx++] = serial->cap; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_nw_bootup(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_NW, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_NW_BOOTUP; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_status_nw_debug(Reporter_t *reporter, uint8_t lvl)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	char data[0xFF] = {0};

	data[idx++] = lvl;
	controller_status_nw_ex(reporter, ZWAVE_EVT_NW_DEBUG, data, idx);
}

void controller_status_nw_watch_off(Reporter_t *reporter, uint8_t watch_off)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	char data[0xFF] = {0};

	data[idx++] = watch_off;
	controller_status_nw_ex(reporter, ZWAVE_EVT_NW_WATCH_OFF, data, idx);
}

void controller_act_nw_ex(Commander_t *commander, uint8_t act, char *data, unsigned short data_len)
{
	POINTER_CHECK(commander);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_NW, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = act; // value
	if ( (data_len) && (data) )
	{
		SAFE_MEMCPY(n_issueitem.data+idx, data, data_len, data_len);
		idx += data_len;
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_probe_report(Reporter_t *reporter, int8_t tx_sts)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_NOP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = tx_sts; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void controller_act_nop(Commander_t *commander, char *data, unsigned short data_len)
{
	POINTER_CHECK(commander);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_NOP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	if ( (data_len) && (data) )
	{
		SAFE_MEMCPY(n_issueitem.data+idx, data, data_len, data_len);
		idx += data_len;
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_postset_report(Reporter_t *reporter, uint16_t cls_id, void *usr_param, int reason)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	uint32_t issueid = (JKEY_ISSUEID_CC_XXX_POSTSET | (cls_id << 8));
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, issueid, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = reason; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

static void zwifd_alrm_xxx_report(Reporter_t *reporter, uint32_t issueid, uint16_t mode, uint16_t submode, uint8_t category, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, issueid, mode, submode, category, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = alrm_p->ex_event; // value
	n_issueitem.data[idx++] = alrm_p->ex_has_sequence; // value
	n_issueitem.data[idx++] = alrm_p->ex_sequence_no; // value
	n_issueitem.data[idx++] = alrm_p->ex_evt_type; // value
	n_issueitem.data[idx++] = alrm_p->ex_evt_len; // value
	if (alrm_p->ex_evt_len)
	{
		SAFE_MEMCPY(n_issueitem.data+idx, alrm_p->ex_evt_prm, alrm_p->ex_evt_len, 0xFF-idx);
		idx += alrm_p->ex_evt_len;
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_alrm_water_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_WATER, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_water_leak_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_WATER;
	alrm.ex_event = ZW_ALRM_EVT_LEAK;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_WATER, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_water_off_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_WATER;
	alrm.ex_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_WATER, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_door_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_DR_WIN, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_door_open_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_LOCK;
	alrm.ex_event = ZW_ALRM_EVT_WINDOW_DOOR_OPEN;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_DR_WIN, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_door_closed_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_LOCK;
	alrm.ex_event = ZW_ALRM_EVT_WINDOW_DOOR_CLOSED;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_DR_WIN, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_glass_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_GLASS_BRK, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_glass_on_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_BURGLAR;
	alrm.ex_event = ZW_ALRM_EVT_GLASS;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_GLASS_BRK, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_glass_off_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_BURGLAR;
	alrm.ex_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_GLASS_BRK, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_tamper_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_TAMPER, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_tamper_open_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_BURGLAR;
	alrm.ex_event = ZW_ALRM_EVT_TMPR_COVER;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_TAMPER, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &alrm);
}

void zwifd_alrm_tamper_closed_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_BURGLAR;
	alrm.ex_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_TAMPER, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_smoke_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_SMOKE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_smoke_on_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_SMOKE;
	alrm.ex_event = ZW_ALRM_EVT_SMOKE;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_SMOKE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_smoke_off_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_SMOKE;
	alrm.ex_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_SMOKE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_motion_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_MOTION, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_motion_on_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_BURGLAR;
	alrm.ex_event = ZW_ALRM_EVT_MOTION_DET;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_MOTION, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_motion_off_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_BURGLAR;
	alrm.ex_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_MOTION, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_co_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_CO, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_co_on_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_CO;
	alrm.ex_event = ZW_ALRM_EVT_CO;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_CO, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_co_off_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_CO;
	alrm.ex_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_CO, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_co2_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_CO2, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_co2_on_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_CO2;
	alrm.ex_event = ZW_ALRM_EVT_CO2;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_CO2, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_co2_off_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_CO2;
	alrm.ex_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_CO2, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_intrusion_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_INTRUSION, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_heat_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_HEAT, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_heat_on_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_HEAT;
	alrm.ex_event = ZW_ALRM_EVT_OVERHEAT;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_HEAT, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_heat_off_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_HEAT;
	alrm.ex_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_HEAT, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_power_over_load_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	alrm_p->ex_event = ZW_ALRM_EVT_OVER_LOAD;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_POW, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_power_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_POW, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_power_off_report_bin(Reporter_t *reporter)
{
	POINTER_CHECK(reporter);

	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_BURGLAR;
	alrm.ex_event = ZW_ALRM_POWER;
	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_POW, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_BINARY, &alrm);
}

void zwifd_alrm_system_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_SYSTEM, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_emergency_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_EMERGENCY, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_clock_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_ALARM, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_first_report(Reporter_t *reporter, zwalrm_p alrm_p)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(alrm_p);

	zwifd_alrm_xxx_report(reporter, JKEY_ISSUEID_FIRST, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, alrm_p);
}

void zwifd_alrm_vendor_report(Reporter_t *reporter, uint8_t type, uint8_t level)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CUSTOMER, type, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_SIREN, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = type; // value
	n_issueitem.data[idx++] = level; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_av_set(Reporter_t *reporter, uint16_t *cmds, uint8_t num_cmds, uint8_t key_attr)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(cmds);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_SIMPLE_AV_CONTROL_SET, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = key_attr; // value
	n_issueitem.data[idx++] = num_cmds; // value
	int i = 0;
	for (i=0; i< num_cmds; i++)
	{
		big_endian2byte(2, cmds[i], (unsigned char *)n_issueitem.data +(idx+i*2) );
		idx += 2;
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_av_report(Reporter_t *reporter, uint16_t length, uint8_t *mask)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_SIMPLE_AV_CONTROL, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	big_endian2byte(2, length, (unsigned char *)n_issueitem.data + idx );
	idx += 2;

	int i = 0;
	for (i=0; i< length; i++)
	{
		n_issueitem.data[idx++] = mask[i]; // value
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_appl_busy_report(Reporter_t *reporter, uint8_t status, uint8_t wait_tm)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_APPLICATION_BUSY, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = status; // value
	n_issueitem.data[idx++] = wait_tm; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_appl_reject_report(Reporter_t *reporter, uint8_t status)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_APPLICATION_REJECT, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = status; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

zwbasic_t *basic_dat_assign(uint8_t curr_val, uint8_t tgt_val, uint8_t dur)
{
	static zwbasic_t basic_dat = {0};

	basic_dat.curr_val = curr_val;
	basic_dat.tgt_val = tgt_val;
	basic_dat.dur = dur;

	return &basic_dat;
}

void zwifd_basic_report(Reporter_t *reporter, zwbasic_p val_p, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(val_p);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_BASIC, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = val_p->curr_val; // value
	n_issueitem.data[idx++] = val_p->tgt_val; // value
	n_issueitem.data[idx++] = val_p->dur; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_basic_set(Reporter_t *reporter, uint8_t level, time_t ts)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_BASIC_SET, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = level; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_act_basic(Commander_t *commander, uint8_t val)
{
	POINTER_CHECK(commander);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_CC_BASIC, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = val; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}


void zwifd_battery_report(Reporter_t *reporter, uint8_t level, time_t ts)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_BATTERY, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = level; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_bsensor_siren_report(Reporter_t *reporter, uint8_t state, uint8_t type, time_t ts)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_SWITCH, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_SIREN, &n_issueitem);

	// ** value **
	idx = 0;
	if (state==0)
	{
		n_issueitem.data[idx++] = ZWAVE_EVT_0X00; // value
		n_issueitem.data[idx++] = ZWAVE_EVT_0X00; // value
	}
	else
	{
		n_issueitem.data[idx++] = ZWAVE_EVT_0XFF; // value
		n_issueitem.data[idx++] = ZWAVE_EVT_0XFF; // value
	}
	n_issueitem.data[idx++] = 0;

	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_bsensor_freeze_report(Reporter_t *reporter, uint8_t state, uint8_t type, time_t ts)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_FREEZE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = state; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_bsensor_gp_report(Reporter_t *reporter, uint8_t state, uint8_t type, time_t ts)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_GP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = state; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_bsensor_sup_report(Reporter_t *reporter, uint8_t type_len, uint8_t *type)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(type);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_BSENSOR_SUP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = type_len; // value
 	int i = 0;
	for (i=0; i< type_len; i++)
	{
		n_issueitem.data[idx++] = type[i]; // value
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_clock_report(Reporter_t *reporter, uint8_t weekday, uint8_t hour, uint8_t minute)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_CLOCK, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = weekday; // value
	n_issueitem.data[idx++] = hour; // value
	n_issueitem.data[idx++] = minute; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_color_report(Reporter_t *reporter, uint8_t colorComponentId, uint8_t value, uint8_t target_val, uint8_t dur)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_COLOR, colorComponentId, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = colorComponentId; // value
	n_issueitem.data[idx++] = value; // value
	n_issueitem.data[idx++] = target_val; // value
	n_issueitem.data[idx++] = dur; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_color_component_mask_report(Reporter_t *reporter, uint8_t colorComponentMask1, uint8_t colorComponentMask2)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_COLOR_MASK, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
 	n_issueitem.data[idx++] = colorComponentMask1; // value
	n_issueitem.data[idx++] = colorComponentMask2; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_color_component_sup_report(Reporter_t *reporter, uint8_t *comp_id, uint8_t comp_cnt)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(comp_id);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_COLOR_SUP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
 	n_issueitem.data[idx++] = comp_cnt; // value
 	memcpy( n_issueitem.data + idx, comp_id, comp_cnt);
	idx += comp_cnt; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_config_report(Reporter_t *reporter, zwconfig_p param)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(param);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_CONFIGURATION, param->param_num, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = param->param_num; // value
	n_issueitem.data[idx++] = param->use_default; // value
	n_issueitem.data[idx++] = param->size; // value
	n_issueitem.data[idx++] = param->data[0]; // value
	n_issueitem.data[idx++] = param->data[1]; // value
	n_issueitem.data[idx++] = param->data[2]; // value
	n_issueitem.data[idx++] = param->data[3]; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_act_config(Commander_t *commander, uint8_t param_num, uint8_t use_default, int32_t val)
{
	POINTER_CHECK(commander);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_CC_CONFIGURATION, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	uint8_t isize = 0;
	if (val >= -128 && val <= 127)
	{
		isize = 1;
	}
	else if (val >= -32768 && val <= 32767)
	{
		isize = 2;
	}
	else if (val >= -8388608 && val <= 8388607)
	{
		isize = 3;
	}
	else
	{
		isize= 4;
	}

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = param_num; // value
	n_issueitem.data[idx++] = use_default; // value
	n_issueitem.data[idx++] = isize; // value
	big_endian2byte(isize, (uint32_t)val, (unsigned char *)n_issueitem.data+idx);
	idx += isize;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_csc_report(Reporter_t *reporter, zwcsc_notif_p data, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(data);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_CENTRAL_SCENE_NOTIFICATION, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = data->seqNo; // value
	n_issueitem.data[idx++] = data->slow_rfsh; // value
	n_issueitem.data[idx++] = data->keyAttr; // value
	n_issueitem.data[idx++] = data->sceneNo; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_csc_cfg_report(Reporter_t *reporter, int slow_refresh, time_t ts)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_CENTRAL_SCENE_CFG, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = slow_refresh; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_csc_sup_report(Reporter_t *reporter, uint8_t scene_cnt, uint8_t sameKA, uint8_t KA_array_len, uint8_t *KA_array, uint8_t slow_rfsh)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(KA_array);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_CENTRAL_SCENE_SUP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = scene_cnt; // value
	n_issueitem.data[idx++] = slow_rfsh; // value
	n_issueitem.data[idx++] = sameKA; // value
	n_issueitem.data[idx++] = KA_array_len; // value
	int i = 0;
	for (i=0; i< KA_array_len; i++)
	{
		n_issueitem.data[idx++] = KA_array[i]; // value
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_dlck_report(Reporter_t *reporter, zwdlck_op_p op_p, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(op_p);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_DOOR_LOCK, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = op_p->mode; // value
	n_issueitem.data[idx++] = op_p->out_mode; // value
	n_issueitem.data[idx++] = op_p->in_mode; // value
	n_issueitem.data[idx++] = op_p->cond; // value
	n_issueitem.data[idx++] = op_p->tmout_min; // value
	n_issueitem.data[idx++] = op_p->tmout_sec; // value
	n_issueitem.data[idx++] = op_p->tgt_mode; // value
	n_issueitem.data[idx++] = op_p->dur; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_dlck_cfg_report(Reporter_t *reporter, zwdlck_cfg_p config, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(config);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_DOOR_LOCK_CFG, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = config->type; // value
	n_issueitem.data[idx++] = config->out_sta; // value
	n_issueitem.data[idx++] = config->in_sta; // value
	n_issueitem.data[idx++] = config->tmout_min; // value
	n_issueitem.data[idx++] = config->tmout_sec; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_group_report(Reporter_t *reporter, uint8_t group, uint8_t max_cnt, uint8_t cnt, grp_member_t *grp_member)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(grp_member);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_GROUP, group, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = group; // value
	n_issueitem.data[idx++] = max_cnt; // value
	n_issueitem.data[idx++] = cnt; // value
	int i = 0;
	for (i=0; i< cnt; i++)
	{
		n_issueitem.data[idx++] = grp_member[i].node_id; // value
		n_issueitem.data[idx++] = grp_member[i].ep_id; // value
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_group_sup_report(Reporter_t *reporter, uint8_t max_grp)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_GROUP_SUP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = max_grp; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_group_active_report(Reporter_t *reporter, uint8_t group)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_GROUP_ACTVIE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = group; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

zwmeter_dat_t *meter_dat_assign(uint8_t type, uint8_t unit, uint8_t rate_type,uint8_t precision, uint16_t delta_time, float val, float prv_val)
{
	static zwmeter_dat_t meter_dat = {0};

	meter_dat.type = type;
	meter_dat.precision = precision;
	meter_dat.unit = unit;
	meter_dat.size = 4;
	big_endian2byte(4, (int32_t)(val*pow(10, precision)), meter_dat.data);
	big_endian2byte(4, (int32_t)(prv_val*pow(10, precision)), meter_dat.prv_data);
	meter_dat.delta_time = delta_time;
	meter_dat.rate_type = rate_type;

	return &meter_dat;
}

void zwifd_meter_report(Reporter_t *reporter, zwmeter_dat_p data, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(data);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_METER, data->type, data->unit, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = data->type; // value
	n_issueitem.data[idx++] = data->precision; // value
	n_issueitem.data[idx++] = data->unit; // value
	n_issueitem.data[idx++] = data->size; // value
	n_issueitem.data[idx++] = data->data[0]; // value
	n_issueitem.data[idx++] = data->data[1]; // value
	n_issueitem.data[idx++] = data->data[2]; // value
	n_issueitem.data[idx++] = data->data[3]; // value
	big_endian2byte(2, data->delta_time, (unsigned char *)n_issueitem.data+idx);
	idx += 2;
	n_issueitem.data[idx++] = data->prv_data[0]; // value
	n_issueitem.data[idx++] = data->prv_data[1]; // value
	n_issueitem.data[idx++] = data->prv_data[2]; // value
	n_issueitem.data[idx++] = data->prv_data[3]; // value
	n_issueitem.data[idx++] = data->rate_type; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_meter_sup_report(Reporter_t *reporter, zwmeter_cap_p meter_cap)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(meter_cap);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_METER_SUP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = meter_cap->type; // value
	n_issueitem.data[idx++] = meter_cap->reset_cap; // value
	n_issueitem.data[idx++] = meter_cap->unit_sup; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_power_level_report(Reporter_t *reporter, uint8_t lvl, uint8_t timeout)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_POWER_LEVEL, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = lvl; // value
	n_issueitem.data[idx++] = timeout; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_power_level_test_report(Reporter_t *reporter, uint8_t node_id, uint8_t status, uint16_t frame_cnt)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_POWER_LEVEL_TEST, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = node_id; // value
	n_issueitem.data[idx++] = status; // value
	big_endian2byte(2, frame_cnt, (unsigned char *)n_issueitem.data+idx); // min
	idx += 2;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_protection_report(Reporter_t *reporter, uint8_t local_prot, uint8_t rf_prot, time_t ts)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_PROTECTION, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = local_prot; // value
	n_issueitem.data[idx++] = rf_prot; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

zwsensor_t *zwsensor_dat_assign(uint8_t type, uint8_t precision, uint8_t unit, float val)
{
	static zwsensor_t sensor_dat = {0};

	sensor_dat.type = type;
	sensor_dat.precision = precision;
	sensor_dat.unit = unit;
	sensor_dat.size = 4;
	big_endian2byte(4, (int32_t)(val*pow(10, precision)), sensor_dat.data);

	return &sensor_dat;
}

static void zwifd_sensor_xxx_report(Reporter_t *reporter, int issue, zwsensor_p data, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(data);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, issue, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = data->type; // value
	n_issueitem.data[idx++] = data->precision; // value
	n_issueitem.data[idx++] = data->unit; // value
	n_issueitem.data[idx++] = data->size; // value
	n_issueitem.data[idx++] = data->data[0]; // value
	n_issueitem.data[idx++] = data->data[1]; // value
	n_issueitem.data[idx++] = data->data[2]; // value
	n_issueitem.data[idx++] = data->data[3]; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_sensor_co_lvl_report(Reporter_t *reporter, zwsensor_p data, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(data);

	zwifd_sensor_xxx_report(reporter, JKEY_ISSUEID_CO_LVL, data, ts);
}

void zwifd_sensor_co2_lvl_report(Reporter_t *reporter, zwsensor_p data, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(data);

	zwifd_sensor_xxx_report(reporter, JKEY_ISSUEID_CO2_LVL, data, ts);
}

void zwifd_sensor_humidity_report(Reporter_t *reporter, zwsensor_p data, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(data);

	zwifd_sensor_xxx_report(reporter, JKEY_ISSUEID_HUMD, data, ts);
}

void zwifd_sensor_luminance_report(Reporter_t *reporter, zwsensor_p data, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(data);

	zwifd_sensor_xxx_report(reporter, JKEY_ISSUEID_LUM, data, ts);
}

void zwifd_sensor_temperature_report(Reporter_t *reporter, zwsensor_p data, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(data);

	zwifd_sensor_xxx_report(reporter, JKEY_ISSUEID_TEMPERATURE, data, ts);
}

void zwifd_sensor_uv_report(Reporter_t *reporter, zwsensor_p data, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(data);

	zwifd_sensor_xxx_report(reporter, JKEY_ISSUEID_UV, data, ts);
}

void zwifd_sensor_voc_report(Reporter_t *reporter, zwsensor_p data, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(data);

	zwifd_sensor_xxx_report(reporter, JKEY_ISSUEID_VOC, data, ts);
}

void zwifd_sensor_sup_report(Reporter_t *reporter, uint8_t type_len, uint8_t *type)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(type);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_SENSOR_SUP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = type_len; // value
	int i = 0;
	for (i=0; i< type_len; i++)
	{
		n_issueitem.data[idx++] = type[i]; // value
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_sensor_unit_sup_report(Reporter_t *reporter, uint8_t sensor_type, uint8_t sensor_unit_msk)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_SENSOR_UNIT_SUP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = sensor_type; // value
	n_issueitem.data[idx++] = sensor_unit_msk; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

zwswitch_t *switch_dat_assign(uint8_t curr_val, uint8_t tgt_val, uint8_t dur)
{
	static zwswitch_t switch_dat = {0};

	switch_dat.curr_val = curr_val;
	switch_dat.tgt_val = tgt_val;
	switch_dat.dur = dur;

	return &switch_dat;
}

void zwifd_switch_report(Reporter_t *reporter, zwswitch_p val_p, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(val_p);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_SWITCH, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = val_p->curr_val; // value
	n_issueitem.data[idx++] = val_p->tgt_val; // value
	n_issueitem.data[idx++] = val_p->dur; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_act_switch(Commander_t *commander, uint8_t val)
{
	POINTER_CHECK(commander);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_CC_SWITCH, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = val; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_plug_report(Reporter_t *reporter, zwswitch_p val_p, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(val_p);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_SWITCH, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_PLUG, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = val_p->curr_val; // value
	n_issueitem.data[idx++] = val_p->tgt_val; // value
	n_issueitem.data[idx++] = val_p->dur; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_switch_all_report(Reporter_t *reporter, uint8_t mode)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_SWITCH_ALL, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = mode; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

zwlevel_dat_t *switch_multilevel_dat_assign(uint8_t curr_val, uint8_t tgt_val, uint8_t dur)
{
	static zwlevel_dat_t level_dat = {0};

	level_dat.curr_val = curr_val;
	level_dat.tgt_val = tgt_val;
	level_dat.dur = dur;

	return &level_dat;
}

void zwifd_switch_multilevel_report(Reporter_t *reporter, zwlevel_dat_p val_p, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(val_p);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_DIMMER, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = val_p->curr_val; // value
	n_issueitem.data[idx++] = val_p->tgt_val; // value
	n_issueitem.data[idx++] = val_p->dur; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_act_switch_multilevel(Commander_t *commander, uint8_t val, uint8_t dur)
{
	POINTER_CHECK(commander);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_command_gen(commander, JKEY_ISSUEID_CC_DIMMER, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = val; // value
	n_issueitem.data[idx++] = dur; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_switch_multilevel_sup_report(Reporter_t *reporter, uint8_t pri_type, uint8_t sec_type)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_DIMMER_SUP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = pri_type; // value
	n_issueitem.data[idx++] = sec_type; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_thermostat_mode_report(Reporter_t *reporter, uint8_t mode, uint8_t *manf_dat, uint8_t manf_dat_cnt, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(manf_dat);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_THERMOSTAT_MODE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = mode; // value
	n_issueitem.data[idx++] = manf_dat_cnt; // value
	memcpy(n_issueitem.data+idx, manf_dat, manf_dat_cnt);
	idx += manf_dat_cnt; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_thermostat_mode_sup_report(Reporter_t *reporter, uint8_t mode_len, uint8_t *mode)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(mode);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_THERMOSTAT_MODE_SUP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = mode_len; // value
	int i = 0;
	for (i=0; i< mode_len; i++)
	{
		n_issueitem.data[idx++] = mode[i]; // value
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_thermostat_operating_report(Reporter_t *reporter, uint8_t state, time_t ts)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_THERMOSTAT_OPERATING, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = state; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_thermostat_setpoint_report(Reporter_t *reporter, zwsetp_p data, time_t ts)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(data);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_THERMOSTAT_SETPOINT, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = data->type; // value
	n_issueitem.data[idx++] = data->precision; // value
	n_issueitem.data[idx++] = data->unit; // value
	n_issueitem.data[idx++] = data->size; // value
	n_issueitem.data[idx++] = data->data[0]; // value
	n_issueitem.data[idx++] = data->data[1]; // value
	n_issueitem.data[idx++] = data->data[2]; // value
	n_issueitem.data[idx++] = data->data[3]; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_thermostat_setpoint_sup_report(Reporter_t *reporter, uint8_t type_len, uint8_t *type)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(type);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_THERMOSTAT_SETPOINT_SUP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = type_len; // value
	int i = 0;
	for (i=0; i< type_len; i++)
	{
		n_issueitem.data[idx++] = type[i]; // value
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_thermostat_setpoint_sup_range_report(Reporter_t *reporter, uint8_t type, zwsetp_p min, zwsetp_p max)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(min);
	POINTER_CHECK(max);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_THERMOSTAT_SETPOINT_SUP_RANGE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = type; // value
	n_issueitem.data[idx++] = min->precision; // value
	n_issueitem.data[idx++] = min->unit; // value
	n_issueitem.data[idx++] = min->size; // value
	n_issueitem.data[idx++] = min->data[0]; // value
	n_issueitem.data[idx++] = min->data[1]; // value
	n_issueitem.data[idx++] = min->data[2]; // value
	n_issueitem.data[idx++] = min->data[3]; // value

	n_issueitem.data[idx++] = max->precision; // value
	n_issueitem.data[idx++] = max->unit; // value
	n_issueitem.data[idx++] = max->size; // value
	n_issueitem.data[idx++] = max->data[0]; // value
	n_issueitem.data[idx++] = max->data[1]; // value
	n_issueitem.data[idx++] = max->data[2]; // value
	n_issueitem.data[idx++] = max->data[3]; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_thermostat_fan_mode_report(Reporter_t *reporter, uint8_t off, uint8_t mode)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_THERMOSTAT_FAN_MODE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = off; // value
	n_issueitem.data[idx++] = mode; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_thermostat_fan_mode_sup_report(Reporter_t *reporter, uint8_t off, uint8_t mode_len, uint8_t *mode)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(mode);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_THERMOSTAT_FAN_MODE_SUP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = off; // value
	n_issueitem.data[idx++] = mode_len; // value
	int i = 0;
	for (i=0; i< mode_len; i++)
	{
		n_issueitem.data[idx++] = mode[i]; // value
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_thermostat_fan_state_report(Reporter_t *reporter, uint8_t state)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_THERMOSTAT_FAN_STATE, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = state; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_usr_code_report(Reporter_t *reporter, zwusrcod_p usr_cod)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(usr_cod);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_USER_CODE, usr_cod->id, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
 	n_issueitem.data[idx++] = usr_cod->id; // value
 	n_issueitem.data[idx++] = usr_cod->id_sts; // value
 	n_issueitem.data[idx++] = usr_cod->code_len; // value
	int i = 0;
	for (i=0; i< usr_cod->code_len; i++)
	{
		n_issueitem.data[idx++] = usr_cod->code[i]; // value
	}
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_usr_sup_report(Reporter_t *reporter, uint8_t  usr_num)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_USER_CODE_SUP, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
 	n_issueitem.data[idx++] = usr_num; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_vercc_report(Reporter_t *reporter, uint16_t cls, uint8_t ver)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_VERSION_CC, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	big_endian2byte(2, cls, (unsigned char *)n_issueitem.data+idx);
	idx += 2;
	n_issueitem.data[idx++] = ver; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_verzw_report(Reporter_t *reporter, uint16_t proto_ver, uint16_t app_ver, uint8_t lib_type, ext_ver_t *ext_ver)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_VERSION_ZW, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = lib_type; // value
	big_endian2byte(2, proto_ver, (unsigned char *)n_issueitem.data+idx);
	idx += 2;
	big_endian2byte(2, app_ver, (unsigned char *)n_issueitem.data+idx);
	idx += 2;

#ifdef DEVICE_EXT_VER_SHOW
	if (ext_ver)
	{
		n_issueitem.data[idx++] = 1; // value
		n_issueitem.data[idx++] = ext_ver->hw_ver; // value
		n_issueitem.data[idx++] = ext_ver->fw_cnt; // value

		int i=0;
		for (i=0; i<ext_ver->fw_cnt; i++)
		{
			big_endian2byte(2, ext_ver->fw_ver[i], (unsigned char *)n_issueitem.data+idx);
			idx += 2;
		}
	}
#endif
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_wakeup_interval_report(Reporter_t *reporter, zwif_wakeup_p cap)
{
	POINTER_CHECK(reporter);
	POINTER_CHECK(cap);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_WAKE_UP_INTERVAL, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	big_endian2byte(4, cap->min, (unsigned char *)n_issueitem.data+idx); // min
	idx += 4;
	big_endian2byte(4, cap->max, (unsigned char *)n_issueitem.data+idx); // max
	idx += 4;
	big_endian2byte(4, cap->def, (unsigned char *)n_issueitem.data+idx); // def
	idx += 4;
	big_endian2byte(4, cap->interval, (unsigned char *)n_issueitem.data+idx); // interval
	idx += 4;
	big_endian2byte(4, cap->cur, (unsigned char *)n_issueitem.data+idx); // cur
	idx += 4;
	n_issueitem.data[idx++] = cap->node.nodeid; // value
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

void zwifd_wakeup_notification_report(Reporter_t *reporter, time_t ts)
{
	POINTER_CHECK(reporter);

	int idx = 0;
	IssueItem_t n_issueitem = {0};

	// ** header **
	issueitem_report_gen(reporter, JKEY_ISSUEID_CC_WAKE_UP_NOTIFICATION, JVAL_MODE_NORMAL, JVAL_SUBMODE_NORMAL, JVAL_CATEGORY_NORMAL, &n_issueitem);

	// ** value **
	idx = 0;
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;

	do_report(&n_issueitem);
}

