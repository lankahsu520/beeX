# Abort
## Sequence

   ```sequence
   echo -> dongle: MCTT (JKEY_ISSUEID_ABORTING)
   dongle --> echo: MCTT (ZWAVE_EVT_ABORTING_DONE)
   ```

## API
   ```
#define JVAL_METHODID_EVENT                      0
#define JVAL_METHODID_PUT                        1

#define JKEY_ISSUEID_ABORTING                    0x00000004

#define ZWAVE_EVT_ABORTING_START                0
#define ZWAVE_EVT_ABORTING_DONE                 97
#define ZWAVE_EVT_ABORTING_ABORTED              98
#define ZWAVE_EVT_ABORTING_FAIL                 99
   ```
### Command - JVAL_METHODID_PUT & JKEY_ISSUEID_ABORTING
   ```
void controller_act_aborting(Commander_t *commander)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ABORTING_START; // value
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
commander_set_to_nodeid(commander, JVAL_NODEID_BROADCAST, JVAL_EPID_BASE);
controller_act_aborting(commander);
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ABORTING_START; // value
	n_issueitem.data_len = idx;
   ```
### Report - JVAL_METHODID_EVENT & JKEY_ISSUEID_ABORTING
   ```
void controller_status_aborting_done(Reporter_t *reporter)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ABORTING_DONE; // value
	n_issueitem.data_len = idx;
void controller_status_aborting_fail(Reporter_t *reporter)
	idx = 0;
	n_issueitem.data[idx++] = ZWAVE_EVT_ABORTING_FAIL; // value
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
reporter_set_nodeid(ZWARE_REPORTER(), zware_ctx->ctl_id, JVAL_EPID_BASE);
controller_status_aborting_done(ZWARE_REPORTER());
   ```

# Log
## MCTT
   ```
20210414 131901 [1/0-BASE] - --> 9C65F9361C00/FFFFFFFF-FFFF-FFFF-FFFF/0xFFFF/0->9C65F9361C00/E380D91E/0x0001/0 - command=aborting,0x00
20210414 131901 [0/2-ZWAVES2] - <-- 9C65F9361C00/E380D91E/0x0001/0 - state=aborting,done
   ```

## TOPIC
   ```
[2917/2919] bee_topic_put_cb:38 - (topic: 1/0/9C65F9361C00/E380D91E/1/0/00000004, payload: {})
[2917/2918] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/E380D91E/1/0/00000004, value: {"name":"JKEY_ISSUEID_ABORTING","value":97})
   ```
