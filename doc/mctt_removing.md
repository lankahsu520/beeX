# Removing
## Sequence

   ```sequence
   echo -> dongle: MCTT (JKEY_ISSUEID_REMOVING)
   dongle --> echo: MCTT (JKEY_ISSUEID_REMOVING-ZWAVE_EVT_REMOVING_START)
   dongle --> echo: MCTT (JKEY_ISSUEID_REMOVING-ZWAVE_EVT_REMOVING_REMOVED)
   dongle --> echo: MCTT (JKEY_ISSUEID_REMOVING-ZWAVE_EVT_REMOVING_DONE)
   ```

## API
   ```
#define JVAL_METHODID_EVENT                      0
#define JVAL_METHODID_PUT                        1

#define JKEY_ISSUEID_REMOVING                    0x00000003

#define ZWAVE_EVT_REMOVING_START                0
#define ZWAVE_EVT_REMOVING_DONE                 97
#define ZWAVE_EVT_REMOVING_REMOVED              98
#define ZWAVE_EVT_REMOVING_FAIL                 99
   ```
### Command - JVAL_METHODID_PUT & JKEY_ISSUEID_REMOVING
   ```
void controller_act_removing(Commander_t *commander)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_REMOVING_START; // value
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
reporter_set_nodeid_ex(reporter, MY_CTL_ID(uid), JVAL_EPID_BASE);
commander_cp_to(commander, reporter);
commander_cp_frm(commander, master_reporter_get());
controller_act_removing(commander);
   ```

### Report - JVAL_METHODID_EVENT & JKEY_ISSUEID_REMOVING
   ```
void controller_status_removed(Reporter_t *reporter)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_REMOVING_REMOVED; // value
	n_issueitem.data_len = idx;

void controller_status_removing_done(Reporter_t *reporter)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_REMOVING_DONE; // value
	n_issueitem.data_len = idx;

void controller_status_removing_fail(Reporter_t *reporter)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_REMOVING_FAIL; // value
	n_issueitem.data_len = idx;

void controller_status_removing(Reporter_t *reporter)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_REMOVING_START; // value
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
reporter_set_nodeid_ex(ZWARE_REPORTER(), zware_ctx->ctl_id, JVAL_EPID_BASE);
controller_status_removing(ZWARE_REPORTER());

reporter_set_nodeid_ex(ZWARE_REPORTER(), noded_p->nodeid, JVAL_EPID_BASE);
controller_status_removed(ZWARE_REPORTER());

reporter_set_nodeid_ex(ZWARE_REPORTER(), zware_ctx->ctl_id, JVAL_EPID_BASE);
controller_status_removing_done(ZWARE_REPORTER());
   ```

# Log
## MCTT
   ```
20210414 133635 [1/0-BASE] - --> 9C65F9361C00/FFFFFFFF-FFFF-FFFF-FFFF/0xFFFF/0->9C65F9361C00/E380D91E/0x0001/0 - command=removing,0x00
20210414 133635 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - zevent=removing,start
20210414 133639 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0002/0 - zevent=0x0002.0,removed
20210414 133640 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=removing,done
20210414 133640 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=normal
20210414 133642 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=op,ZWNET_OP_NW_CHANGED
20210414 133642 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=op,ZWNET_OP_SAVE_NW
20210414 133642 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=op,ZWNET_OP_SAVE_NW
   ```

## TOPIC
   ```
[3613/3615] bee_topic_put_cb:38 - (topic: 1/0/9C65F9361C00/E380D91E/1/0/00000003, payload: {})
[3613/3614] honeycomb_notify_removing:626 - Removed nodeid !!! (9C65F9361C00/E380D91E/2)
[3613/3614] bee_topic_del_node_cb:80 - (topic: 0/9C65F9361C00/E380D91E/2, value: {})
[3613/3614] honeycomb_save:1057 - (json_f_mctt: /mnt/mctt_honeycomb.json)
   ```
