# Reseting
## Sequence

   ```sequence
   echo -> dongle: MCTT (JKEY_ISSUEID_RESETING)
   dongle --> echo: MCTT (JKEY_ISSUEID_RESETING-ZWAVE_EVT_RESETING_START)
   dongle --> echo: MCTT (JKEY_ISSUEID_RESETING-ZWAVE_EVT_RESETING_REMOVED)
   dongle --> echo: MCTT (JKEY_ISSUEID_RESETING-ZWAVE_EVT_RESETING_DONE)
   ```

## API
   ```
#define JVAL_METHODID_EVENT                      0
#define JVAL_METHODID_PUT                        1

#define JKEY_ISSUEID_RESETING                    0x00000008

#define ZWAVE_EVT_RESETING_START                0
#define ZWAVE_EVT_RESETING_DONE                 97
#define ZWAVE_EVT_RESETING_REMOVED              98
#define ZWAVE_EVT_RESETING_FAIL                 99
   ```
### Command - JVAL_METHODID_EVENT & JKEY_ISSUEID_RESETING
   ```
void controller_act_reseting(Commander_t *commander)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_RESETING_START; // value
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
commander_set_to_nodeid(commander, JVAL_NODEID_BROADCAST, JVAL_EPID_BASE);
controller_act_reseting(commander);
   ```

### Report - JVAL_METHODID_EVENT & JKEY_ISSUEID_RESETING
   ```
void controller_status_reset_rm(Reporter_t *reporter, char *homeid)
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

void controller_status_reset_done(Reporter_t *reporter)
	// ** value **
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_REPLACING_DONE; // value
	n_issueitem.data_len = idx;

void controller_status_reset_fail(Reporter_t *reporter)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_RESETING_FAIL; // value
	n_issueitem.data_len = idx;

void controller_status_reseting(Reporter_t *reporter)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_RESETING_START; // value
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
reporter_set_nodeid_ex(ZWARE_REPORTER(), zware_ctx->ctl_id, JVAL_EPID_BASE);
controller_status_reseting(ZWARE_REPORTER());

reporter_set_nodeid_ex(ZWARE_REPORTER(), zware_ctx->ctl_id, JVAL_EPID_BASE);
controller_status_reset_rm(ZWARE_REPORTER(), zware_ctx->uuid);

reporter_set_nodeid_ex(ZWARE_REPORTER(), zware_ctx->ctl_id, JVAL_EPID_BASE);
controller_status_reset_done(ZWARE_REPORTER());
   ```

# Log
## MCTT
   ```
20210414 140518 [1/0-BASE] - --> 9C65F9361C00/FFFFFFFF-FFFF-FFFF-FFFF/0xFFFF/0->9C65F9361C00/CFBB8F5A/0x0001/0 - command=reseting,0x00
20210414 140518 [0/2-ZWAVES2] - <-- 9C65F9361C00/CFBB8F5A/0x0001/0 - state=reseting
20210414 140524 [0/2-ZWAVES2] - <-- 9C65F9361C00/CFBB8F5A/0x0001/0 - state=reset,CFBB8F5A
20210414 140524 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0001/0 - zwinfo=homeid,CED69735,0x01,3-ZWNET_CTLR_ROLE_PROXY/ZWNET_CTLR_ROLE_INCL,3-ZWNET_CTLR_CAP_S2 (0x01)/ZWNET_CTLR_CAP_INC_ON_BEHALF (0x02),1-ZW_ROLE_SIS,0x01
20210414 140524 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0001/0 - zwinfo=serial,5,0x08,5,0,0x3c
20210414 140524 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0001/0 - state=normal
20210414 140526 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0001/0 - state=reset,done
20210414 140526 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0001/0 - state=op,ZWNET_OP_SAVE_NW
   ```

## TOPIC
   ```
[5226/5228] bee_topic_put_cb:38 - (topic: 1/0/9C65F9361C00/CFBB8F5A/1/0/00000008, payload: {})
[5226/5227] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/CFBB8F5A/1/0/00000008, value: {"name":"JKEY_ISSUEID_ADDING","value":0})
[5226/5227] honeycomb_save:1069 - (json_f_mctt: /mnt/mctt_honeycomb.json)
[5226/5227] honeycomb_notify_reseting:673 - Removed uuid !!! (9C65F9361C00/CFBB8F5A)
[5226/5227] bee_topic_del_uuid_cb:58 - (topic: 0/2/9C65F9361C00/CFBB8F5A, value: {})
[5226/5227] honeycomb_save:1069 - (json_f_mctt: /mnt/mctt_honeycomb.json)
[5226/5227] honeycomb_notify_reseting:683 - Added uuid !!! (9C65F9361C00/CED69735)
[5226/5227] bee_topic_add_uuid_cb:47 - (topic: 0/2/9C65F9361C00/CED69735, value: {"1":{"0":{"00000008":{"name":"JKEY_ISSUEID_ADDING","value":97},"name":"Endpoint ID"},"Protocol ID":2,"lastbeattime_utc":1618380326,"name":"Node ID","protocol_name":"ZWAVES2"},"name":"UUID"})
   ```
