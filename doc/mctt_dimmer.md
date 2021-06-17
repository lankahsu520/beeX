# Dimmer
## Sequence

   ```sequence
   echo -> dongle: MCTT (JKEY_ISSUEID_CC_DIMMER)
   dongle --> echo: MCTT (JKEY_ISSUEID_CC_DIMMER)
   ```

## API
   ```
#define JVAL_METHODID_EVENT                      0
#define JVAL_METHODID_PUT                        1

#define JKEY_ISSUEID_CC_DIMMER                   0x00092601 // level
   ```
### Command - JVAL_METHODID_PUT & JKEY_ISSUEID_CC_DIMMER
   ```
val: 0=off, 0xFF=on(previous level), 1-99=%
dur: Dimming duration.  0=instantly;  0x01 to 0x7F = 1 second (0x01) to 127 seconds (0x7F); 0x80 to 0xFE = 1 minute (0x80) to 127 minutes (0xFE); 0xFF = factory default rate.

void zwifd_act_switch_multilevel(Commander_t *commander, uint8_t val, uint8_t dur)
	idx = 0;
	n_issueitem.data[idx++] = val; // value
	n_issueitem.data[idx++] = dur; // value
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
reporter_set_nodeid_ex(reporter, UID_to_NODEID(uid), epid);
commander_cp_to(commander, reporter);
commander_cp_frm(commander, master_reporter_get());
zwifd_act_switch_multilevel(commander, val, dur);
   ```
### Report - JVAL_METHODID_EVENT & JKEY_ISSUEID_CC_DIMMER
   ```
typedef struct
{
	uint8_t curr_val; /**< current value: 0 = off; 0x01~0x63 = percentage (%);0xFE = Unknown; 0xFF = On. */
	uint8_t tgt_val; /**< target value with same interpretation as curr_val */
	uint8_t dur; /**< Duration: 0 = already at the target; 0x01~0x7F = seconds; 0x80~0xFD = 1~126 minutes; 0xFE = Unknown duration; 0xFF = reserved*/
}
zwlevel_dat_t, *zwlevel_dat_p;

void zwifd_switch_multilevel_report(Reporter_t *reporter, zwlevel_dat_p val_p, time_t ts)
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
zwifd_switch_multilevel_report(ZWARE_REPORTER(), val_p, ts);
   ```

# Log
## MCTT
   ```
20210414 141150 [1/0-BASE] - --> 9C65F9361C00/FFFFFFFF-FFFF-FFFF-FFFF/0xFFFF/0->9C65F9361C00/CED69735/0x0003/0 - command=dimmer,0x00,0x05
20210414 141150 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0003/0 - 0x000926ff=0x000926FF,0x02
20210414 141156 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0003/0 - zevent=0x0003.0,dimmer,0-off,254-unknown,254-unknown
20210414 141208 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0002/0 - zevent=0x0002.0,meter,1-ZW_METER_TYPE_ELEC,0-kWh,1-ZW_METER_RATE_IMPORT,302-delta_time,0.000,0.000
   ```

## TOPIC
   ```
[5226/5228] bee_topic_put_cb:38 - (topic: 1/0/9C65F9361C00/CED69735/3/0/00092601, payload: {"dur":5,"tgt_val":0})
[5226/5227] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/CED69735/3/0/00092601, value: {"curr_val":0,"dur":254,"name":"Dimmer","tgt_val":254})
   ```
