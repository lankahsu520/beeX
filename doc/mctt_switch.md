# Switch
## Sequence

   ```sequence
   echo -> dongle: MCTT (JKEY_ISSUEID_CC_SWITCH)
   dongle --> echo: MCTT (JKEY_ISSUEID_CC_SWITCH)
   ```

## API
   ```
#define JVAL_METHODID_EVENT                      0
#define JVAL_METHODID_PUT                        1

#define JKEY_ISSUEID_CC_SWITCH                   0x00092501 // switch
   ```
### Command - JVAL_METHODID_PUT & JKEY_ISSUEID_CC_SWITCH
   ```
val: non-zero=on; 0=off

void zwifd_act_switch(Commander_t *commander, uint8_t val)
	idx = 0;
	n_issueitem.data[idx++] = val; // value
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
reporter_set_nodeid_ex(reporter, UID_to_NODEID(uid), epid);
commander_cp_to(commander, reporter);
commander_cp_frm(commander, master_reporter_get());
zwifd_act_switch(commander, val);
   ```
### Report - JVAL_METHODID_EVENT & JKEY_ISSUEID_CC_SWITCH
   ```
typedef struct
{
	uint8_t curr_val; /**< current value: 0 = off; 0xFE = Unknown; 0xFF = On. */
	uint8_t tgt_val; /**< target value with same interpretation as curr_val */
	uint8_t dur; /**< Duration: 0 = already at the target; 0x01~0x7F = seconds; 0x80~0xFD = 1~126 minutes; 0xFE = Unknown duration; 0xFF = reserved*/
}
zwswitch_t, *zwswitch_p;

void zwifd_switch_report(Reporter_t *reporter, zwswitch_p val_p, time_t ts)
	idx = 0;
	n_issueitem.data[idx++] = val_p->curr_val; // value
	n_issueitem.data[idx++] = val_p->tgt_val; // value
	n_issueitem.data[idx++] = val_p->dur; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
reporter_set_nodeid_ex(ZWARE_REPORTER(), ifd_p->nodeid, ifd_p->epid);
zwifd_switch_report(ZWARE_REPORTER(), val_p, ts);
   ```

# Log
## MCTT
   ```
20210414 140952 [1/0-BASE] - --> 9C65F9361C00/FFFFFFFF-FFFF-FFFF-FFFF/0xFFFF/0->9C65F9361C00/CED69735/0x0002/0 - command=switch,0xFF
[5367/5368] order_cb_zwecho:140 - JKEY_ISSUEID_CC_SWITCH (val: 255)
20210414 140954 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0002/0 - zevent=0x0002.0,switch,255-on,254-unknown,254-unknown
20210414 140954 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0002/0 - zevent=0x0002.0,meter,1-ZW_METER_TYPE_ELEC,2-W,1-ZW_METER_RATE_IMPORT,0-delta_time,0.100,0.000
   ```

## TOPIC
   ```
[5226/5228] bee_topic_put_cb:38 - (topic: 1/0/9C65F9361C00/CED69735/2/0/00092501, payload: {"tgt_val":255})
[5226/5227] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/CED69735/2/0/00092501, value: {"curr_val":255,"dur":254,"name":"Switch","tgt_val":254})
[5226/5227] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/CED69735/2/0/00093201, value: {"1":{"0":{"delta_time":298,"name":"kWh","prv_value":0.0,"rate_type":1,"value":0.0},"2":{"delta_time":0,"name":"W","prv_value":0.0,"rate_type":1,"value":0.1},"4":{"delta_time":0,"name":"V","prv_value":0.0,"rate_type":1,"value":112.0},"5":{"delta_time":0,"name":"A","prv_value":0.0,"rate_type":1,"value":0.0},"6":{"delta_time":0,"name":"power factor","prv_value":0.0,"rate_type":1,"value":0.0},"name":"ZW_METER_TYPE_ELEC"}})
   ```

