# Add
## Sequence

   ```sequence
   echo -> dongle: MCTT (JKEY_ISSUEID_ADDING)
   dongle --> echo: MCTT (JKEY_ISSUEID_ADDING-ZWAVE_EVT_ADDING_START)
   dongle --> echo: MCTT (JKEY_ISSUEID_ADDING-ZWAVE_EVT_ADDING_ADDED)
   dongle --> echo: MCTT (JKEY_ISSUEID_ADDING-ZWAVE_EVT_ADDING_DONE)
   ```

## API
   ```
#define JVAL_METHODID_EVENT                      0
#define JVAL_METHODID_PUT                        1

#define JKEY_ISSUEID_ADDING                      0x00000002

#define ZWAVE_EVT_ADDING_START                  0
#define ZWAVE_EVT_ADDING_SSA_SET                80
#define ZWAVE_EVT_ADDING_CSA                    81
#define ZWAVE_EVT_ADDING_SSA                    82
#define ZWAVE_EVT_ADDING_SEC                    83
#define ZWAVE_EVT_ADDING_DONE                   97
#define ZWAVE_EVT_ADDING_ADDED                  98
#define ZWAVE_EVT_ADDING_FAIL                   99
   ```
### Command - JVAL_METHODID_PUT & JKEY_ISSUEID_ADDING
   ```
void controller_act_adding(Commander_t *commander, char *dsk)
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
   ```
#### Sample
   ```
reporter_set_nodeid_ex(reporter, MY_CTL_ID(uid), JVAL_EPID_BASE);
commander_cp_to(commander, reporter);
commander_cp_frm(commander, master_reporter_get());
controller_act_adding(commander, (char*)dsk);
   ```

### Report - JVAL_METHODID_EVENT & JKEY_ISSUEID_ADDING
   ```
void controller_status_added(Reporter_t *reporter)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_ADDED; // value
	n_issueitem.data_len = idx;

void controller_status_adding_done(Reporter_t *reporter)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_DONE; // value
	n_issueitem.data_len = idx;

void controller_status_adding_sec(Reporter_t *reporter, uint8_t highest_lvl, uint8_t curr_lvl)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_SEC; // value
	n_issueitem.data[idx++] = highest_lvl; // value
	n_issueitem.data[idx++] = curr_lvl; // value
	n_issueitem.data_len = idx;

void controller_status_adding_csa(Reporter_t *reporter, char *csa_pin)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_CSA; // value
	SAFE_SPRINTF((char*)n_issueitem.data + idx, "%s", csa_pin);
	idx += SAFE_STRLEN(csa_pin);
	n_issueitem.data_len = idx;

void controller_status_adding_ssa_set(Reporter_t *reporter, uint8_t res)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_SSA_SET; // value
	n_issueitem.data[idx++] = res;
	n_issueitem.data_len = idx;

void controller_status_adding_ssa(Reporter_t *reporter,char *dsk)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_SSA; // value
	SAFE_SPRINTF((char*)n_issueitem.data + idx, "%s", dsk);
	idx += SAFE_STRLEN(dsk);
	n_issueitem.data_len = idx;

void controller_status_adding_xxx(Reporter_t *reporter, uint8_t flag)
	idx = 0;
	n_issueitem.data[idx++] = flag; // value
	n_issueitem.data_len = idx;

void controller_status_adding_fail(Reporter_t *reporter)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_FAIL; // value
	n_issueitem.data_len = idx;

void controller_status_adding(Reporter_t *reporter)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ADDING_START; // value
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
reporter_set_nodeid_ex(ZWARE_REPORTER(), zware_ctx->ctl_id, JVAL_EPID_BASE);
controller_status_adding(ZWARE_REPORTER());

reporter_set_nodeid_ex(ZWARE_REPORTER(), noded_p->nodeid, JVAL_EPID_BASE);
controller_status_added(ZWARE_REPORTER());

reporter_set_nodeid_ex(ZWARE_REPORTER(), zware_ctx->ctl_id, JVAL_EPID_BASE);
controller_status_adding_done(ZWARE_REPORTER());
   ```

# Log
## MCTT
   ```
20210414 132526 [1/0-BASE] - --> 9C65F9361C00/FFFFFFFF-FFFF-FFFF-FFFF/0xFFFF/0->9C65F9361C00/E380D91E/0x0001/0 - command=adding,0x00
20210414 132526 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - zevent=adding,start
20210414 132533 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0002/0 - zevent=0x0002.0,added
20210414 132533 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_PROTOCOL_DONE
20210414 132533 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_GET_NODE_INFO
20210414 132533 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_ZWNET_STA_GET_GW_DSK
20210414 132534 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_ZWNET_STA_GET_NODE_INFO
20210414 132534 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_ZWNET_STA_ASSIGN_RR
20210414 132540 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_ZWNET_STA_GET_VER_CC
20210414 132540 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_ZWNET_STA_GET_ZWPLUS_INFO
20210414 132541 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_ZWNET_STA_GET_VER
20210414 132542 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_ZWNET_STA_GET_MANF_PDT_ID
20210414 132542 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_ACT_END_POINT
20210414 132542 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_S_EP_STA_GET_MAX_GROUP
20210414 132542 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_S_EP_STA_SET_GROUP_LIFELINE
20210414 132544 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_S_EP_STA_GET_GROUP_MEMBER
20210414 132545 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_S_EP_STA_SET_CFG_PARAM
20210414 132545 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_S_EP_STA_GET_ZWPLUS_INFO
20210414 132547 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_S_EP_STA_GET_GROUP_INFO
20210414 132548 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,OP_ADD_NODE_S_EP_STA_GET_MUL_SWITCH
20210414 132548 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0002/0 - zwinfo=0x0002.0,nodeinfo,5-S0 (0x01)/Security (0x04),0x013c,0x0005,0x0031,4.38,1.02,3,0,0,0,1,-1,1,0x80-S0 (0x80)/Non-Security
20210414 132548 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0002/0 - zwinfo=0x0002.0,0x11-GENERIC_TYPE_SWITCH_MULTILEVEL,0x01-SPECIFIC_TYPE_POWER_SWITCH_MULTILEVEL
20210414 132548 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0002/0 - zwinfo=0x0002.0,1,0x00-NODE_TYPE_ZWAVEPLUS_NODE,0x05-ROLE_TYPE_CONTROLLER_CENTRAL_STATIC,0x600,0x600
20210414 132548 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0002/0 - zevent=0x0002.0,group_sup,2
20210414 132548 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0002/0 - zevent=0x0002.0,group,1-group,5-max_cnt,1-cnt,1.1
20210414 132548 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0002/0 - zevent=0x0002.0,group,2-group,5-max_cnt,1-cnt,1.1
20210414 132548 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0002/0 - zevent=0x0002.0,dimmer,0-off,254-unknown,254-unknown
20210414 132548 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0002/0 - zevent=0x0002.0,cfg,1-use_default,0-param_num,1
20210414 132548 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0002/0 - zevent=0x0002.0,cfg,2-use_default,0-param_num,1
20210414 132548 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0002/0 - zevent=0x0002.0,cfg,4-use_default,0-param_num,1
20210414 132548 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=ready,ZWNET_OP_NONE,0x02
20210414 132548 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=normal
20210414 132550 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=adding,done
20210414 132550 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=op,ZWNET_OP_NW_CHANGED
20210414 132550 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=op,ZWNET_OP_SAVE_NW
20210414 132550 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=op,ZWNET_OP_SAVE_NW
   ```

## TOPIC
   ```
[2917/2919] bee_topic_put_cb:38 - (topic: 1/0/9C65F9361C00/E380D91E/1/0/00000002, payload: {})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":0})
[2917/2918] honeycomb_notify_adding:174 - Added nodeid !!! (9C65F9361C00/E380D91E/2)
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/2/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":98})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":1})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":2})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":5})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":6})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":10})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":11})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":12})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":13})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":14})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":18})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":32})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":33})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":34})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":35})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":37})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":39})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000002, value: {"name":"JKEY_ISSUEID_ADDING","value":43})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/2/0/00000010, value: {"app_ver":"1.02","category":0,"interval":-1,"lib_type":3,"listen":1,"name":"JKEY_ISSUEID_NODE","pid":49,"propty":5,"propty_name":"S0 (0x01)/Security (0x04)","proto_ver":"4.38","ptype":5,"s2_grnt_keys":128,"s2_grnt_keys_name":"S0 (0x80)/Non-Security","s2_keys_valid":1,"sensor":0,"sleep_cap":0,"value":97,"vid":316})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/2/0/00000010, value: {"app_ver":"1.02","category":0,"generic":17,"interval":-1,"lib_type":3,"listen":1,"name":"JKEY_ISSUEID_NODE","pid":49,"propty":5,"propty_name":"S0 (0x01)/Security (0x04)","proto_ver":"4.38","ptype":5,"s2_grnt_keys":128,"s2_grnt_keys_name":"S0 (0x80)/Non-Security","s2_keys_valid":1,"sensor":0,"sleep_cap":0,"specific":1,"value":95,"vid":316})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/2/0/00000010, value: {"app_ver":"1.02","category":0,"generic":17,"instr_icon":1536,"interval":-1,"lib_type":3,"listen":1,"name":"JKEY_ISSUEID_NODE","node_type":0,"pid":49,"propty":5,"propty_name":"S0 (0x01)/Security (0x04)","proto_ver":"4.38","ptype":5,"role_type":5,"s2_grnt_keys":128,"s2_grnt_keys_name":"S0 (0x80)/Non-Security","s2_keys_valid":1,"sensor":0,"sleep_cap":0,"specific":1,"usr_icon":1536,"value":93,"vid":316,"zwplus_ver":1})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/2/0/00092601, value: {"curr_val":0,"dur":254,"name":"Dimmer","tgt_val":254})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/2/0/00097001, value: {"1":{"size":1,"use_default":0,"value":1},"name":"Configuration"})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/2/0/00097001, value: {"1":{"size":1,"use_default":0,"value":1},"2":{"size":1,"use_default":0,"value":1},"name":"Configuration"})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/2/0/00097001, value: {"1":{"size":1,"use_default":0,"value":1},"2":{"size":1,"use_default":0,"value":1},"4":{"size":1,"use_default":0,"value":1},"name":"Configuration"})
[2917/2918] bee_topic_add_node_cb:69 - (topic: 0/2/9C65F9361C00/E380D91E/2, value: {"0":{"00000002":{"name":"JKEY_ISSUEID_ADDING","value":98},"00000010":{"app_ver":"1.02","category":0,"generic":17,"instr_icon":1536,"interval":-1,"lib_type":3,"listen":1,"name":"JKEY_ISSUEID_NODE","node_type":0,"pid":49,"propty":5,"propty_name":"S0 (0x01)/Security (0x04)","proto_ver":"4.38","ptype":5,"role_type":5,"s2_grnt_keys":128,"s2_grnt_keys_name":"S0 (0x80)/Non-Security","s2_keys_valid":1,"sensor":0,"sleep_cap":0,"specific":1,"usr_icon":1536,"value":93,"vid":316,"zwplus_ver":1},"00092601":{"curr_val":0,"dur":254,"name":"Dimmer","tgt_val":254},"00097001":{"1":{"size":1,"use_default":0,"value":1},"2":{"size":1,"use_default":0,"value":1},"4":{"size":1,"use_default":0,"value":1},"name":"Configuration"},"name":"Endpoint ID"},"lastbeattime_utc":1618377948,"name":"Node ID","ready":1,"status":"alive"})
[2917/2918] honeycomb_save:1054 - (json_f_mctt: /mnt/mctt_honeycomb.json)
   ```
