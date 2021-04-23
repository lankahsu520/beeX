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

int honeycomb_topic2issueitem(Honeycomb_t *honeycomb_ctx, IssueItem_t *c_issueitem, char *topic)
{
	int idx = 0;
	if ((honeycomb_ctx) && (c_issueitem) && (topic))
	{
		Commander_t *commander = &c_issueitem->commander;

		char *saveptr = NULL;
		char *token = SAFE_STRTOK_R(topic, "/", &saveptr);
		char c_nodeid[LEN_OF_UUID] = JVAL_C_NODEID_NULL;
		int epid = 0;

		while (token)
		{
			switch (idx)
			{
				case 0:
					commander_set_methodid(commander, strtol(token, NULL, 10));
					break;
				case 1:
					commander_set_protocolid(commander, strtol(token, NULL, 10));
					break;
				case 2:
					commander_set_to_mac(commander, token);
					break;
				case 3:
					commander_set_to_uuid(commander, token);
					break;
				case 4:
					SAFE_SPRINTF(c_nodeid, "%s", token);
					break;
				case 5:
					epid = strtol(token, NULL, 10);
					break;
				case 6:
					c_issueitem->issueid = strtol(token, NULL, 16);
					break;
				default:
					break;
			}
			token = SAFE_STRTOK_R(NULL, "/", &saveptr);
			idx++;
		}

		commander_set_to_nodeid(commander, c_nodeid, epid);

		DBG_IF_LN("(%d/%d/%s/%s/%s/%d)", commander->methodid, commander->protocolid, commander->id_to.c_macid, commander->id_to.c_uuid, commander->id_to.c_nodeid, commander->id_to.epid);
	}

	return idx;
}

// (MQTT sub) --> queen_bee -> honeycomb (MCTT Server)
void honeycomb_topic_get(Honeycomb_t *honeycomb_ctx, char *topic, char *payload)
{
	if ((honeycomb_ctx) && (topic) && (payload))
	{
		IssueItem_t n_issueitem = {0};

		int idx = honeycomb_topic2issueitem(honeycomb_ctx, &n_issueitem, topic);
		if (idx > 0)
		{
			Commander_t *commander = &n_issueitem.commander;
			commander->methodid = JVAL_METHODID_RESPONSE;

			TopicX_t topicx = {0};
			honeycomb_lookup_jissueid_helper(honeycomb_ctx, &n_issueitem, ISSUE_TYPE_ID_COMMANDER, JSON_ACTID_READ, &topicx);
			if (topicx.jissueid)
			{
				honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(topicx.jissueid), honeycomb_ctx->topic_issue_cb);
			}
			else if (topicx.jepid)
			{
				honeycomb_topic_pub(honeycomb_ctx, topicx.topic_epid, JSON_COPY(topicx.jepid), honeycomb_ctx->topic_issue_cb);
			}
			else if (topicx.jnodeid)
			{
				honeycomb_topic_pub(honeycomb_ctx, topicx.topic_nodeid, JSON_COPY(topicx.jnodeid), honeycomb_ctx->topic_issue_cb);
			}
			else if (topicx.juuid)
			{
				honeycomb_topic_pub(honeycomb_ctx, topicx.topic_uuid, JSON_COPY(topicx.juuid), honeycomb_ctx->topic_issue_cb);
			}
			else if (topicx.jmac)
			{
				honeycomb_topic_pub(honeycomb_ctx, topicx.topic_mac, JSON_COPY(topicx.jmac), honeycomb_ctx->topic_issue_cb);
			}
			else
			{
				DBG_WN_LN("honeycomb_lookup_jissueid_helper - ISSUE_TYPE_ID_COMMANDER error !!!");
			}
		}
	}
}

// (MQTT sub) --> queen_bee -> honeycomb (MCTT Server) -> dongle
void honeycomb_topic_put(Honeycomb_t *honeycomb_ctx, char *topic, char *payload)
{
	if ((honeycomb_ctx) && (topic) && (payload))
	{
		IssueItem_t n_issueitem = {0};
		Commander_t *commander = &n_issueitem.commander;

		int idx = honeycomb_topic2issueitem(honeycomb_ctx, &n_issueitem, topic);

		if ( idx == 7 )
		{
			json_t *jobj = NULL;
			json_t *jissueid = JSON_LOADS_EASY(payload);

			DBG_DB_LN("(idx: %d, Commander_t: %d, %s,%s,%s,%d,%08X)", idx, commander->methodid, commander->id_to.c_macid, commander->id_to.c_uuid, commander->id_to.c_nodeid, commander->id_to.epid, n_issueitem.issueid );
			switch (n_issueitem.issueid)
			{
				// 0x00000002
				case JKEY_ISSUEID_ADDING:
					{
						const char *dsk = JSON_OBJ_GET_STR(jissueid, jobj, JKEY_REPORT_NODE_DSK);
						controller_act_adding(commander, (char *)dsk);
					}
					break;
				// 0x00000003
				case JKEY_ISSUEID_REMOVING:
					controller_act_removing(commander);
					break;
				// 0x00000004
				case JKEY_ISSUEID_ABORTING:
					controller_act_aborting(commander);
					break;
				// 0x00000008
				case JKEY_ISSUEID_RESETING:
					controller_act_reseting(commander);
					break;
				// 0x00092501
				case JKEY_ISSUEID_CC_SWITCH:
					{
						json_int_t tgt_val = (uint8_t)JSON_OBJ_GET_INT_DEF(jissueid, jobj, JKEY_COMM_TGT_VAL, 0);
						zwifd_act_switch(commander, (uint8_t)tgt_val);
					}
					break;
				// 0x00092601
				case JKEY_ISSUEID_CC_DIMMER:
					{
						json_int_t tgt_val = (uint8_t)JSON_OBJ_GET_INT_DEF(jissueid, jobj, JKEY_COMM_TGT_VAL, 0);
						json_int_t dur = (uint8_t)JSON_OBJ_GET_INT_DEF(jissueid, jobj, JKEY_COMM_DUR, 0);
						zwifd_act_switch_multilevel(commander, (uint8_t)tgt_val, (uint8_t)dur);
					}
					break;
				default:
					break;
			}
			JSON_FREE(jissueid);
		}

	}
}

void honeycomb_topic_pub(Honeycomb_t *honeycomb_ctx, char *topic, json_t *jvalue, honeycomb_topic_fn *topic_cb)
{
	if ((honeycomb_ctx) && (honeycomb_ctx->topic_issue_cb) && (topic) && (jvalue))
	{
		char *value = JSON_DUMPS_FLAGS(jvalue, JSON_FLAGS_EASY|JSON_REAL_PRECISION(BEEX_REAL_PRECISION));
		//char *value = JSON_DUMPS_FLAGS(jvalue, JSON_FLAGS_EASY);

		if (topic_cb) topic_cb(topic, value);
		SAFE_FREE(value);
	}
	JSON_FREE(jvalue);
}

#if (0)
static json_int_t honeycomb_node_ready(json_t *jnodeid)
{
	json_int_t ret = 0;
	if (jnodeid)
	{
		json_t *jobj = NULL;

		ret = JSON_OBJ_GET_INT_DEF(jnodeid, jobj, JKEY_REPORT_READY, 0);
	}

	return ret;
}
#endif

static void honeycomb_notify_aborting(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;
	uint8_t val = (unsigned int)c_issueitem->data[idx];
	//char *valp = c_issueitem->data;

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, "JKEY_ISSUEID_ABORTING");
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_VALUE, val);

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_adding(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	Reporter_t *reporter = &c_issueitem->reporter;
	char *c_macid = reporter->id_frm.c_macid;
	char *c_uuid = reporter->id_frm.c_uuid;
	char *c_nodeid = reporter->id_frm.c_nodeid;
	//uint8_t epid = reporter->id_frm.epid;

	uint8_t val = (unsigned int)c_issueitem->data[idx];

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, "JKEY_ISSUEID_ADDING");
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_VALUE, val);

	switch (val)
	{
		case ZWAVE_EVT_ADDING_START:
			break;
		case ZWAVE_EVT_ADDING_ADDED:
			{
				if (topicx.jnodeid)
				{
					DBG_WN_LN("Added nodeid !!! (%s/%s/%s)", c_macid, c_uuid, c_nodeid);
					JSON_OBJ_SET_INT(topicx.jnodeid, JKEY_REPORT_READY, 0);
					JSON_OBJ_SET_STR(topicx.jnodeid, JKEY_COMM_STATUS, JVAL_NODE_ALIVE);

					AddedNodeid_t *added_nodeid = (AddedNodeid_t*)SAFE_CALLOC(1, sizeof(AddedNodeid_t));
					added_nodeid->jnodeid = topicx.jnodeid;
					SAFE_SPRINTF(added_nodeid->topic_nodeid, "%s", topicx.topic_nodeid);
					clist_push(honeycomb_ctx->added_list, added_nodeid);
				}
			}
			break;
		case ZWAVE_EVT_ADDING_DONE:
			while (clist_length(honeycomb_ctx->added_list) > 0 )
			{
				AddedNodeid_t *added_nodeid = clist_pop(honeycomb_ctx->added_list);

				JSON_OBJ_SET_INT(added_nodeid->jnodeid, JKEY_REPORT_READY, 1);
				honeycomb_topic_pub(honeycomb_ctx, added_nodeid->topic_nodeid, JSON_COPY(added_nodeid->jnodeid), honeycomb_ctx->topic_add_node_cb);

				SAFE_FREE(added_nodeid);
			}
			honeycomb_save(honeycomb_ctx);
			break;
		case ZWAVE_EVT_ADDING_FAIL:
			break;
		case ZWAVE_EVT_ADDING_CSA:
			break;
		case ZWAVE_EVT_ADDING_SSA_SET:
			break;
		case ZWAVE_EVT_ADDING_SSA:
			break;
		case ZWAVE_EVT_ADDING_SEC:
			break;
		default:
			break;
	}

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_basic(void *userdata, IssueItem_t *c_issueitem)
{
}

static void honeycomb_notify_basic_set(void *userdata, IssueItem_t *c_issueitem)
{
}

static void honeycomb_notify_battery(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	uint8_t val = (unsigned int)c_issueitem->data[idx];
	//char *valp = c_issueitem->data;

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, JVAL_NAME_BATTERY);
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_VALUE, val);

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_configuration(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	//uint8_t val = (unsigned int)c_issueitem->data[idx];
	char *valp = c_issueitem->data;

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	uint8_t param_num = (uint8_t)valp[idx++];
	uint8_t use_default = (uint8_t)valp[idx++];
	uint8_t size = (uint8_t)valp[idx++];
	uint8_t *data = (uint8_t *)valp+idx;

	int32_t value = byte2big_endian(size, data);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, JVAL_NAME_CONFIGURATION);

	char c_param_num[LEN_OF_VAL16] = "";
	SAFE_SPRINTF(c_param_num, "%d", param_num);
	json_t *jparam_num = JSON_OBJ_GET_OBJ_EX(jissueid, c_param_num);
	JSON_OBJ_SET_INT(jparam_num, JKEY_COMM_USE_DEFAULT, use_default);
	JSON_OBJ_SET_INT(jparam_num, JKEY_COMM_SIZE, size);
	JSON_OBJ_SET_INT(jparam_num, JKEY_COMM_VALUE, value);

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_door(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	uint8_t val = (unsigned int)c_issueitem->data[idx];
	//char *valp = c_issueitem->data;

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, JVAL_NAME_DOOR);
	switch (val)
	{
		case ZWAVE_EVT_WINDOW_DOOR_OPEN:
			JSON_OBJ_SET_STR(jissueid, JKEY_COMM_VAL, JVAL_COMM_OPEN);
			break;
		case ZWAVE_EVT_WINDOW_DOOR_CLOSED:
			JSON_OBJ_SET_STR(jissueid, JKEY_COMM_VAL, JVAL_COMM_CLOSED);
			break;
	}

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_luminance(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	//uint8_t val = (unsigned int)c_issueitem->data[idx];
	char *valp = c_issueitem->data;

	uint8_t type = valp[idx++];
	uint8_t prec = valp[idx++];
	uint8_t unit = valp[idx++];
	uint8_t size = valp[idx++];
	uint8_t *data = (uint8_t *)valp+idx;

	int32_t value = byte2big_endian(size, data);
	int32_t precision = pow(10, prec);

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, JVAL_NAME_LUMINANCE_SENSOR);
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_TYPE, type);
	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_TYPE_NAME, translate_sensor_type(type));
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_UNIT, unit);
	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_UNIT_NAME, translate_sensor_unit(type, unit));
	JSON_OBJ_SET_REAL(jissueid, JKEY_COMM_VALUE, (float)value/(float)precision);

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_humidity(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	//uint8_t val = (unsigned int)c_issueitem->data[idx];
	char *valp = c_issueitem->data;

	uint8_t type = valp[idx++];
	uint8_t prec = valp[idx++];
	uint8_t unit = valp[idx++];
	uint8_t size = valp[idx++];
	uint8_t *data = (uint8_t *)valp+idx;

	int32_t value = byte2big_endian(size, data);
	int32_t precision = pow(10, prec);

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, JVAL_NAME_HUMIDITY_SENSOR);
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_TYPE, type);
	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_TYPE_NAME, translate_sensor_type(type));
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_UNIT, unit);
	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_UNIT_NAME, translate_sensor_unit(type, unit));
	JSON_OBJ_SET_REAL(jissueid, JKEY_COMM_VALUE, (float)value/(float)precision);

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_learning(void *userdata, IssueItem_t *c_issueitem)
{
}

static void honeycomb_notify_dimmer(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	//uint8_t val = (unsigned int)c_issueitem->data[idx];
	char *valp = c_issueitem->data;

	uint8_t curr_val = valp[idx++]; // 0=off; 0xFE=Unknown; 0x01~0x63 = percentage (%);0xFE = Unknown; 0xFF = On
	uint8_t tgt_val = valp[idx++]; // same curr_val
	uint8_t dur = valp[idx++]; // Duration: 0 = already at the target; 0x01~0x7F = seconds; 0x80~0xFD = 1~126 minutes; 0xFE = Unknown duration; 0xFF = reserved

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, JVAL_NAME_DIMMER);
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_CURR_VAL, curr_val);
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_TGT_VAL, tgt_val);
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_DUR, dur);

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_meter(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	//uint8_t val = (unsigned int)c_issueitem->data[idx];
	char *valp = c_issueitem->data;
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

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	char c_typex[LEN_OF_VAL16] = "";
	SAFE_SPRINTF(c_typex, "%d", typex);
	json_t *jtypex = JSON_OBJ_GET_OBJ_EX(jissueid, c_typex);
	JSON_OBJ_SET_STR(jtypex, JKEY_COMM_NAME, translate_meter_type(typex));

	char c_unit[LEN_OF_VAL16] = "";
	SAFE_SPRINTF(c_unit, "%d", unit);
	json_t *junit = JSON_OBJ_GET_OBJ_EX(jtypex, c_unit);
	JSON_OBJ_SET_STR(junit, JKEY_COMM_NAME, translate_meter_unit(typex, unit));
	JSON_OBJ_SET_INT(junit, JKEY_COMM_RATE_TYPE, rate_type);
	JSON_OBJ_SET_INT(junit, JKEY_COMM_DELTA_TIME, delta_time);
	JSON_OBJ_SET_REAL(junit, JKEY_COMM_VALUE, (float)value/(float)precision);
	JSON_OBJ_SET_REAL(junit, JKEY_COMM_PRV_VALUE, (float)prv_value/(float)precision);

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_motion(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	uint8_t val = (unsigned int)c_issueitem->data[idx];
	//char *valp = c_issueitem->data;

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, JVAL_NAME_MOTION_SENSOR);
	switch (val)
	{
		case ZWAVE_EVT_INACTIVE_CLEAR:
			JSON_OBJ_SET_STR(jissueid, JKEY_COMM_VAL, JVAL_COMM_IDLE);
			break;
		case ZWAVE_EVT_MOTION_DET_L:
			JSON_OBJ_SET_STR(jissueid, JKEY_COMM_VAL, JVAL_COMM_ON);
			break;
		case ZWAVE_EVT_MOTION_DET:
		default:
			JSON_OBJ_SET_STR(jissueid, JKEY_COMM_VAL, JVAL_COMM_ON);
			break;
	}

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_node(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	uint8_t val = (unsigned int)c_issueitem->data[idx];
	char *valp = c_issueitem->data;

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, "JKEY_ISSUEID_NODE");
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_VALUE, val);

	{
		idx++;
		switch (val)
		{
			case ZWAVE_EVT_NODE_ALIVE:
				JSON_OBJ_SET_STR(topicx.jnodeid, JKEY_COMM_STATUS, JVAL_NODE_ALIVE);
				break;
			case ZWAVE_EVT_NODE_SLEEPING:
				JSON_OBJ_SET_STR(topicx.jnodeid, JKEY_COMM_STATUS, JVAL_NODE_SLEEPING);
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

					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_PROPTY, propty);
					JSON_OBJ_SET_STR(jissueid, JKEY_REPORT_NODE_PROPTY_NAME, translate_node_propty(propty, 1));
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_VENDOR_VID, vid);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_VENDOR_PTYPE, ptype);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_VENDOR_PID, pid);
					char c_proto_ver[LEN_OF_VAL16] = "";
					SAFE_SPRINTF(c_proto_ver, "%u.%02u", (unsigned)(proto_ver >> 8), (unsigned)(proto_ver & 0xFF));
					JSON_OBJ_SET_STR(jissueid, JKEY_REPORT_NODE_PROTO_VER, c_proto_ver);
					char c_app_ver[LEN_OF_VAL16] = "";
					SAFE_SPRINTF(c_app_ver, "%u.%02u", (unsigned)(app_ver >> 8), (unsigned)(app_ver & 0xFF));
					JSON_OBJ_SET_STR(jissueid, JKEY_REPORT_NODE_APP_VER, c_app_ver);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_LIB_TYPE, lib_type);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_CATEGORY, category);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_SENSOR, sensor);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_SLEEP_CAP, sleep_cap);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_LISTEN, listen);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_WAKEUP_INTERVAL, wkup_intv);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_S2_KEYS_VALID, s2_keys_valid);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_S2_GRNT_KEYS, s2_grnt_keys);
					JSON_OBJ_SET_STR(jissueid, JKEY_REPORT_NODE_S2_GRNT_KEYS_NAME, translate_requested_keys(s2_grnt_keys, 1));
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

					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_ZWPLUS_VER, zwplus_ver);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_TYPE, node_type);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_ROLE_TYPE, role_type);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_INSTR_ICON, instr_icon);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_USR_ICON, usr_icon);
				}
				break;
			case ZWAVE_EVT_NODE_GENERIC:
				{
					uint8_t generic = valp[idx++];
					uint8_t specific = valp[idx++];

					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_GENERIC, generic);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_SPECIFIC, specific);
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

					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_VENDOR_VID, vid);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_VENDOR_PTYPE, ptype);
					JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_NODE_VENDOR_PID, pid);
				}
				break;
			default:
				break;
		}
	}

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_nop(void *userdata, IssueItem_t *c_issueitem)
{
}

static void honeycomb_notify_nw(void *userdata, IssueItem_t *c_issueitem)
{
}

static void honeycomb_notify_op(void *userdata, IssueItem_t *c_issueitem)
{
}

static void honeycomb_notify_replacing(void *userdata, IssueItem_t *c_issueitem)
{
}

static void honeycomb_notify_rmfailed(void *userdata, IssueItem_t *c_issueitem)
{
}

static void honeycomb_notify_removing(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	Reporter_t *reporter = &c_issueitem->reporter;
	char *c_macid = reporter->id_frm.c_macid;
	char *c_uuid = reporter->id_frm.c_uuid;
	char *c_nodeid = reporter->id_frm.c_nodeid;
	//uint8_t epid = reporter->id_frm.epid;

	uint8_t val = (unsigned int)c_issueitem->data[idx];

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_READ, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, "JKEY_ISSUEID_REMOVING");
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_VALUE, val);

	switch (val)
	{
		case ZWAVE_EVT_REMOVING_START:
			break;
		case ZWAVE_EVT_REMOVING_REMOVED:
			{
				if (topicx.juuid)
				{
					JSON_OBJ_DEL(topicx.juuid, c_nodeid);
					DBG_WN_LN("Removed nodeid !!! (%s/%s/%s)", c_macid, c_uuid, c_nodeid);

					honeycomb_topic_pub(honeycomb_ctx, topicx.topic_nodeid, JSON_OBJ_NEW(), honeycomb_ctx->topic_del_node_cb);
				}
			}
			break;
		case ZWAVE_EVT_REMOVING_DONE:
			honeycomb_save(honeycomb_ctx);
			break;
		case ZWAVE_EVT_REMOVING_FAIL:
			break;
		default:
			break;
	}
}

static void honeycomb_notify_reseting(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	Reporter_t *reporter = &c_issueitem->reporter;
	char *c_macid = reporter->id_frm.c_macid;
	char *c_uuid = reporter->id_frm.c_uuid;
	//char *c_nodeid = reporter->id_frm.c_nodeid;
	//uint8_t epid = reporter->id_frm.epid;

	uint8_t val = (unsigned int)c_issueitem->data[idx];

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, "JKEY_ISSUEID_ADDING");
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_VALUE, val);

	switch (val)
	{
		case ZWAVE_EVT_RESETING_START:
			honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
			break;
		case ZWAVE_EVT_RESETING_REMOVED:
			{
				JSON_OBJ_DEL(topicx.jmac, c_uuid);

				honeycomb_save(honeycomb_ctx);
				DBG_WN_LN("Removed uuid !!! (%s/%s)", c_macid, c_uuid);

				honeycomb_topic_pub(honeycomb_ctx, topicx.topic_uuid, JSON_OBJ_NEW(), honeycomb_ctx->topic_del_uuid_cb);
			}
			break;
		case ZWAVE_EVT_RESETING_DONE:
			{
				honeycomb_save(honeycomb_ctx);
				DBG_WN_LN("Added uuid !!! (%s/%s)", c_macid, c_uuid);

				honeycomb_topic_pub(honeycomb_ctx, topicx.topic_uuid, JSON_COPY(topicx.juuid), honeycomb_ctx->topic_add_uuid_cb);
			}
			break;
		case ZWAVE_EVT_RESETING_FAIL:
			break;
		default:
			break;
	}
}

static void honeycomb_notify_switch(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	//uint8_t val = (unsigned int)c_issueitem->data[idx];
	char *valp = c_issueitem->data;

	uint8_t curr_val = valp[idx++]; // 0=off; 0xFE=Unknown; 0xFF=On
	uint8_t tgt_val = valp[idx++]; // same curr_val
	uint8_t dur = valp[idx++]; // 0=done; 0x01~0x7F=seconds; 0x80~0xFD=1~126 minutes; 0xFE=Unknown duration; 0xFF=reserved

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, JVAL_NAME_SWITCH);
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_CURR_VAL, curr_val);
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_TGT_VAL, tgt_val);
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_DUR, dur);

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_tempature(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	//uint8_t val = (unsigned int)c_issueitem->data[idx];
	char *valp = c_issueitem->data;

	uint8_t type = valp[idx++];
	uint8_t prec = valp[idx++];
	uint8_t unit = valp[idx++];
	uint8_t size = valp[idx++];
	uint8_t *data = (uint8_t *)valp+idx;

	int32_t value = byte2big_endian(size, data);
	int32_t precision = pow(10, prec);

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, JVAL_NAME_TEMPERATURE_SENSOR);
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_TYPE, type);
	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_TYPE_NAME, translate_sensor_type(type));
	JSON_OBJ_SET_INT(jissueid, JKEY_COMM_UNIT, unit);
	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_UNIT_NAME, translate_sensor_unit(type, unit));
	JSON_OBJ_SET_REAL(jissueid, JKEY_COMM_VALUE, (float)value/(float)precision);

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_tamper(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	uint8_t val = (unsigned int)c_issueitem->data[idx];
	//char *valp = c_issueitem->data;

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, JVAL_NAME_TAMPER_SENSOR);
	switch (val)
	{
		case ZWAVE_EVT_INACTIVE_CLEAR:
			JSON_OBJ_SET_STR(jissueid, JKEY_COMM_VAL, JVAL_COMM_IDLE);
			break;
		case ZWAVE_EVT_TMPR_COVER:
		default:
			JSON_OBJ_SET_STR(jissueid, JKEY_COMM_VAL, JVAL_COMM_OPEN);
			break;
	}

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_wake_up_interval(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	//uint8_t val = (unsigned int)c_issueitem->data[idx];
	char *valp = c_issueitem->data;

	uint32_t imin = byte2big_endian(4, (uint8_t *)valp+idx); idx+=4;
	uint32_t imax = byte2big_endian(4, (uint8_t *)valp+idx); idx+=4;
	uint32_t idef = byte2big_endian(4, (uint8_t *)valp+idx); idx+=4;
	uint32_t interval = byte2big_endian(4, (uint8_t *)valp+idx); idx+=4;
	uint32_t cur = byte2big_endian(4, (uint8_t *)valp+idx); idx+=4;
	uint8_t nodeid = valp[idx];

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, JVAL_NAME_WAKE_UP_INTERVAL);

	JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_WAKEUP_MIN, imin);
	JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_WAKEUP_MAX, imax);
	JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_WAKEUP_DEF, idef);
	JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_WAKEUP_INTERVAL, interval);
	JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_WAKEUP_CUR, cur);
	JSON_OBJ_SET_INT(jissueid, JKEY_REPORT_WAKEUP_NOTIFY, nodeid);

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_wake_up_notification(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	//int idx = 0;

	//uint8_t val = (unsigned int)c_issueitem->data[idx];
	//char *valp = c_issueitem->data;

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, JVAL_NAME_WAKE_UP_SENSOR);

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static void honeycomb_notify_water(void *userdata, IssueItem_t *c_issueitem)
{
	Honeycomb_t *honeycomb_ctx = (Honeycomb_t *)userdata;
	int idx = 0;

	uint8_t val = (unsigned int)c_issueitem->data[idx];
	//char *valp = c_issueitem->data;

	TopicX_t topicx = {0};
	json_t *jissueid = honeycomb_lookup_jissueid_helper(honeycomb_ctx, c_issueitem, ISSUE_TYPE_ID_REPORTER, JSON_ACTID_APPEND, &topicx);

	JSON_OBJ_SET_STR(jissueid, JKEY_COMM_NAME, JVAL_NAME_FLOOD);
	switch (val)
	{
		case ZWAVE_EVT_INACTIVE_CLEAR:
			JSON_OBJ_SET_STR(jissueid, JKEY_COMM_VAL, JVAL_COMM_IDLE);
			break;
		case ZWAVE_EVT_LVL_L:
		case ZWAVE_EVT_LVL:
			JSON_OBJ_SET_STR(jissueid, JKEY_COMM_VAL, JVAL_WATER_LEVEL_DROPPED);
			break;
		case ZWAVE_EVT_LEAK_L:
		case ZWAVE_EVT_LEAK:
		default:
			JSON_OBJ_SET_STR(jissueid, JKEY_COMM_VAL, JVAL_WATER_LEAK);
			break;
	}

	honeycomb_topic_pub(honeycomb_ctx, topicx.topic_issueid, JSON_COPY(jissueid), honeycomb_ctx->topic_issue_cb);
}

static Notify_t notify_honeycomb = {
	//.watch_item_cb[NOTIFY_FN_ID_READY] = master_notify_ready,
	.watch_item_cb[NOTIFY_FN_ID_ADDING] = honeycomb_notify_adding,
	.watch_item_cb[NOTIFY_FN_ID_REMOVING] = honeycomb_notify_removing,
	.watch_item_cb[NOTIFY_FN_ID_ABORTING] = honeycomb_notify_aborting,
	.watch_item_cb[NOTIFY_FN_ID_LEARNING] = honeycomb_notify_learning,
	.watch_item_cb[NOTIFY_FN_ID_NW] = honeycomb_notify_nw,
	.watch_item_cb[NOTIFY_FN_ID_RESETING] = honeycomb_notify_reseting,
	.watch_item_cb[NOTIFY_FN_ID_OP] = honeycomb_notify_op,

	.watch_item_cb[NOTIFY_FN_ID_REPLACING] = honeycomb_notify_replacing,
	.watch_item_cb[NOTIFY_FN_ID_RMFAILED] = honeycomb_notify_rmfailed,

	.watch_item_cb[NOTIFY_FN_ID_NODE] = honeycomb_notify_node,
	.watch_item_cb[NOTIFY_FN_ID_NOP] = honeycomb_notify_nop,

	.watch_item_cb[NOTIFY_FN_ID_TAMPER] = honeycomb_notify_tamper,
	.watch_item_cb[NOTIFY_FN_ID_DR_WIN] = honeycomb_notify_door,
	.watch_item_cb[NOTIFY_FN_ID_MOTION] = honeycomb_notify_motion,

	.watch_item_cb[NOTIFY_FN_ID_WATER] = honeycomb_notify_water,
	.watch_item_cb[NOTIFY_FN_ID_TEMPATURE] = honeycomb_notify_tempature,
	.watch_item_cb[NOTIFY_FN_ID_LUMINANCE] = honeycomb_notify_luminance,
	.watch_item_cb[NOTIFY_FN_ID_HUMIDITY] = honeycomb_notify_humidity,

	.watch_item_cb[NOTIFY_FN_ID_BASIC] = honeycomb_notify_basic,
	.watch_item_cb[NOTIFY_FN_ID_BASIC_SET] = honeycomb_notify_basic_set,
	.watch_item_cb[NOTIFY_FN_ID_SWITCH] = honeycomb_notify_switch,
	.watch_item_cb[NOTIFY_FN_ID_DIMMER] = honeycomb_notify_dimmer,
	.watch_item_cb[NOTIFY_FN_ID_METER] = honeycomb_notify_meter,
	.watch_item_cb[NOTIFY_FN_ID_CONFIGURATION] = honeycomb_notify_configuration,
	.watch_item_cb[NOTIFY_FN_ID_BATTERY] = honeycomb_notify_battery,

	.watch_item_cb[NOTIFY_FN_ID_WAKE_UP_INTERVAL] = honeycomb_notify_wake_up_interval,
	.watch_item_cb[NOTIFY_FN_ID_WAKE_UP_NOTIFICATION] = honeycomb_notify_wake_up_notification,
};

json_t *honeycomb_lookup_juuid_helper(Honeycomb_t *honeycomb_ctx, IssueItem_t *c_issueitem, ISSUE_TYPE_ID itype, JSON_ACTID act, TopicX_t *topicx_ctx)
{
	json_t *jmac = NULL;
	json_t *juuid = NULL;

	if ( (honeycomb_ctx) && (topicx_ctx) )
	{
		uint16_t methodid = JVAL_METHODID_EVENT;
		uint16_t protocolid = JVAL_PROTOCOLID_BASE;
		char *c_macid = NULL;
		char *c_uuid = NULL;
		if (itype==ISSUE_TYPE_ID_COMMANDER)
		{
			Commander_t *commander = &c_issueitem->commander;
			methodid = commander->methodid;
			protocolid = commander->protocolid;
			c_macid = commander->id_to.c_macid;
			c_uuid = commander->id_to.c_uuid;
		}
		else
		{
			Reporter_t *reporter = &c_issueitem->reporter;
			methodid = reporter->methodid;
			protocolid = reporter->protocolid;
			c_macid = reporter->id_frm.c_macid;
			c_uuid = reporter->id_frm.c_uuid;
		}

		if (SAFE_STRCMP(c_macid, JVAL_C_MAC_BROADCAST) == 0)
		{
			DBG_ER_LN("Skip !!! (c_macid: %s)", c_macid);
		}
		else if (SAFE_STRCMP(c_macid, JVAL_C_MAC_NULL) == 0)
		{
			DBG_ER_LN("Skip !!! (c_macid: %s)", c_macid);
		}
		else if ( (SAFE_STRCMP(honeycomb_ctx->iface_mac, c_macid) == 0) || (honeycomb_ctx->keepothers==1) )
		{
			if (act==JSON_ACTID_APPEND)
			{
				jmac = JSON_OBJ_GET_OBJ_EX(honeycomb_ctx->jroot, c_macid);
			}
			else
			{
				jmac = JSON_OBJ_GET_OBJ(honeycomb_ctx->jroot, c_macid);
			}

			SAFE_SPRINTF(topicx_ctx->topic_mac, "%d/%d/%s", methodid, protocolid, c_macid);

			if (jmac)
			{
				if (SAFE_STRCMP(c_uuid, JVAL_C_UUID_NULL) == 0)
				{
				}
				else if (SAFE_STRCMP(c_uuid, JVAL_C_UUID_BROADCAST) == 0)
				{
				}
				else
				{
					SAFE_SPRINTF(topicx_ctx->topic_uuid, "%s/%s", topicx_ctx->topic_mac, c_uuid);

					switch (act)
					{
						case JSON_ACTID_DEL:
							JSON_OBJ_DEL(jmac, c_uuid);
							break;
						case JSON_ACTID_APPEND:
							juuid = JSON_OBJ_GET_OBJ_EX(jmac, c_uuid);

							JSON_OBJ_SET_STR(jmac, JKEY_COMM_NAME, JVAL_NAME_MAC_ADDRESS);
							JSON_OBJ_SET_STR(juuid, JKEY_COMM_NAME, JVAL_NAME_UUID);
							break;
						case JSON_ACTID_READ:
						default:
							juuid = JSON_OBJ_GET_OBJ(jmac, c_uuid);
							break;
					}
				}
			}
		}

		topicx_ctx->jmac = jmac;
		topicx_ctx->juuid = juuid;
	}

	return juuid;
}

json_t *honeycomb_lookup_jnodeid_helper(Honeycomb_t *honeycomb_ctx, IssueItem_t *c_issueitem, ISSUE_TYPE_ID itype, JSON_ACTID act, TopicX_t *topicx_ctx)
{
	json_t *jnodeid = NULL;
	if ( (honeycomb_ctx) && (topicx_ctx) )
	{
		json_t *juuid = honeycomb_lookup_juuid_helper(honeycomb_ctx, c_issueitem, itype, act, topicx_ctx);

		if (juuid)
		{
			uint16_t protocolid = JVAL_PROTOCOLID_BASE;
			char *c_nodeid = NULL;
			if (itype==ISSUE_TYPE_ID_COMMANDER)
			{
				Commander_t *commander = &c_issueitem->commander;
				protocolid = commander->protocolid;
				c_nodeid = commander->id_to.c_nodeid;
			}
			else
			{
				Reporter_t *reporter = &c_issueitem->reporter;
				protocolid = reporter->protocolid;
				c_nodeid = reporter->id_frm.c_nodeid;
			}

			if (act==JSON_ACTID_APPEND)
			{
				jnodeid = JSON_OBJ_GET_OBJ_EX(juuid, c_nodeid);
			}
			else
			{
				jnodeid = JSON_OBJ_GET_OBJ(juuid, c_nodeid);
			}

			SAFE_SPRINTF(topicx_ctx->topic_nodeid, "%s/%s", topicx_ctx->topic_uuid, c_nodeid);

			switch (act)
			{
				case JSON_ACTID_APPEND:
					if (jnodeid)
					{
						time_t now_t = time(NULL);
						JSON_OBJ_SET_INT(jnodeid, JKEY_COMM_LAST_BEAT_TIME_UTC, now_t);

						JSON_OBJ_SET_STR(jnodeid, JKEY_COMM_NAME, JVAL_NAME_NODEID);
						JSON_OBJ_SET_INT(jnodeid, JVAL_NAME_PROTOCOLID, protocolid);
						JSON_OBJ_SET_STR(jnodeid, JKEY_COMM_PROTOCOL_NAME, translate_protocolid(protocolid));
					}
					break;
				default:
					break;
			}
		}

		topicx_ctx->jnodeid = jnodeid;
	}

	return jnodeid;
}

json_t *honeycomb_lookup_jepid_helper(Honeycomb_t *honeycomb_ctx, IssueItem_t *c_issueitem, ISSUE_TYPE_ID itype, JSON_ACTID act, TopicX_t *topicx_ctx)
{
	json_t *jepid = NULL;
	if ( (honeycomb_ctx) && (topicx_ctx) )
	{
		json_t *jnodeid = honeycomb_lookup_jnodeid_helper(honeycomb_ctx, c_issueitem, itype, act, topicx_ctx);

		if (jnodeid)
		{
			uint8_t epid = JVAL_EPID_BASE;
			if (itype==ISSUE_TYPE_ID_COMMANDER)
			{
				Commander_t *commander = &c_issueitem->commander;
				epid = commander->id_to.epid;
			}
			else
			{
				Reporter_t *reporter = &c_issueitem->reporter;
				epid = reporter->id_frm.epid;
			}

			char c_epid[LEN_OF_VAL16] = "";
			SAFE_SPRINTF(c_epid, "%d", epid);
			if (act==JSON_ACTID_APPEND)
			{
				jepid = JSON_OBJ_GET_OBJ_EX(jnodeid, c_epid);
			}
			else
			{
				jepid = JSON_OBJ_GET_OBJ(jnodeid, c_epid);
			}

			SAFE_SPRINTF(topicx_ctx->topic_epid, "%s/%s", topicx_ctx->topic_nodeid, c_epid);

			JSON_OBJ_SET_STR(jepid, JKEY_COMM_NAME, JVAL_NAME_EPID);
		}

		topicx_ctx->jepid = jepid;
	}

	return jepid;
}

json_t *honeycomb_lookup_jissueid_helper(Honeycomb_t *honeycomb_ctx, IssueItem_t *c_issueitem, ISSUE_TYPE_ID itype, JSON_ACTID act, TopicX_t *topicx_ctx)
{
	json_t *jissueid = NULL;
	if ( (honeycomb_ctx) && (topicx_ctx) )
	{
		json_t *jepid = honeycomb_lookup_jepid_helper(honeycomb_ctx, c_issueitem, itype, act, topicx_ctx);

		if (jepid)
		{
			uint32_t issueid = c_issueitem->issueid;
			char c_issueid[LEN_OF_VAL16] = "";
			SAFE_SPRINTF(c_issueid, "%08X", issueid);
			if (act==JSON_ACTID_APPEND)
			{
				jissueid = JSON_OBJ_GET_OBJ_EX(jepid, c_issueid);
			}
			else
			{
				jissueid = JSON_OBJ_GET_OBJ(jepid, c_issueid);
			}

			SAFE_SPRINTF(topicx_ctx->topic_issueid, "%s/%s", topicx_ctx->topic_epid, c_issueid);
		}

		topicx_ctx->jissueid = jissueid;
	}

	return jissueid;
}

static int honeycomb_restore_file(Honeycomb_t *honeycomb_ctx)
{
	int ret = 0;

	if ( (honeycomb_ctx) && (SAFE_STRLEN(honeycomb_ctx->json_f_mctt) > 0) )
	{
		JSON_FREE(honeycomb_ctx->jroot);
		//SAFE_MEMSET(macro_ctx, 0, sizeof(MACRO_t));

		DBG_IF_LN("(json_f_mctt: %s)", honeycomb_ctx->json_f_mctt);
		honeycomb_ctx->jroot = JSON_LOADFILE_EASY_OR_NEW(honeycomb_ctx->json_f_mctt);
		honeycomb_ctx->jmac = JSON_OBJ_GET_OBJ_EX(honeycomb_ctx->jroot, honeycomb_ctx->iface_mac);
	}

	return ret;
}

void honeycomb_register(Honeycomb_t *honeycomb_ctx)
{
#if (0)
	{
		Notify_t *notify = &bee_notify;
		tracker_notify_set_uuid(notify, honeycomb_ctx->uuid_root);
		notify->order_item_cb[ORDER_FN_ID_GW] = bee_order_cb_gw;
		notify->order_item_cb[ORDER_FN_ID_RESETING] = bee_order_cb_reset;
		notify->order_item_cb[ORDER_FN_ID_SWITCH] = bee_order_cb_switch;
		tracker_notify_register(notify);
	}
#else
	{
		Notify_t *notify = honeycomb_ctx->notify;
		tracker_notify_set_uuid(notify, honeycomb_ctx->uuid_root);
		tracker_notify_register(notify);
	}
#endif
}

void honeycomb_save(Honeycomb_t *honeycomb_ctx)
{
	if ( (honeycomb_ctx) && (SAFE_STRLEN(honeycomb_ctx->json_f_mctt) > 0) )
	{
		DBG_IF_LN("(json_f_mctt: %s)", honeycomb_ctx->json_f_mctt);
		//JSON_DUMP_FILE_EASY(honeycomb_ctx->jroot, honeycomb_ctx->json_f_mctt);
		JSON_DUMP_FILE_FLAGS(honeycomb_ctx->jroot, honeycomb_ctx->json_f_mctt, JSON_FLAGS_EASY|JSON_REAL_PRECISION(BEEX_REAL_PRECISION));
	}
}

void honeycomb_stop(Honeycomb_t *honeycomb_ctx)
{
	if (honeycomb_ctx)
	{
		honeycomb_ctx->isquit = 1;
	}
}

void honeycomb_close(Honeycomb_t *honeycomb_ctx)
{
	if ( (honeycomb_ctx) && (honeycomb_ctx->isfree == 0) )
	{
		honeycomb_ctx->isfree = 1;

		honeycomb_save(honeycomb_ctx);
		JSON_FREE(honeycomb_ctx->jroot);

		clist_free(honeycomb_ctx->added_list);
		reporter_close();
	}
}

void honeycomb_open(Honeycomb_t *honeycomb_ctx)
{
	reporter_init(honeycomb_ctx, honeycomb_ctx->iface_mac);
	do_report_enable(1);

	if (honeycomb_ctx->order)
	{
		tracker_order_set_on();
	}
	if (honeycomb_ctx->watch)
	{
		tracker_watch_set_on();
	}
	if (honeycomb_ctx->logging)
	{
		tracker_logging_set_on();
	}

	if ( (honeycomb_ctx) && (SAFE_STRLEN(honeycomb_ctx->json_f_mctt) > 0) )
	{
		honeycomb_restore_file(honeycomb_ctx);

		tracker_notify_set_uuid(&notify_honeycomb, honeycomb_ctx->uuid_root);
		tracker_notify_register(&notify_honeycomb);
	}

	CLIST_STRUCT_INIT(honeycomb_ctx, added_list);

	if (1)
	{
		SAFE_SPRINTF(honeycomb_ctx->uuid_root, "%s", JVAL_C_UUID_BROADCAST);
		//os_random_uuid(my_uuid);

		{
			Reporter_t *reporter = honeycomb_ctx->reporter;
			reporter_set_methodid(reporter, JVAL_METHODID_EVENT);
			reporter_set_protocolid(reporter, JVAL_PROTOCOLID_HONEYCOMB);

			reporter_set_mac(reporter, honeycomb_ctx->iface_mac);
			reporter_set_uuid(reporter, honeycomb_ctx->uuid_root);
			reporter_set_nodeid(reporter, JVAL_C_NODEID_BROADCAST, JVAL_EPID_BASE);
		}

		{
			Commander_t *commander = honeycomb_ctx->commander;
			commander_set_methodid(commander, JVAL_METHODID_PUT);
			commander_set_protocolid(commander, JVAL_PROTOCOLID_HONEYCOMB);

			commander_set_to_mac(commander, honeycomb_ctx->iface_mac);
			commander_set_to_uuid(commander, honeycomb_ctx->uuid_root);
			commander_set_to_nodeid(commander, JVAL_C_NODEID_BROADCAST, JVAL_EPID_BASE);

			commander_set_frm_mac(commander, honeycomb_ctx->iface_mac);
			commander_set_frm_uuid(commander, honeycomb_ctx->uuid_root);
			commander_set_frm_nodeid(commander, JVAL_C_NODEID_BROADCAST, JVAL_EPID_BASE);
		}
	}
}
