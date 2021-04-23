# Battery
## Sequence

   ```sequence
   dongle --> echo: MCTT (JKEY_ISSUEID_CC_BATTERY)
   ```

## API
   ```
#define JVAL_METHODID_EVENT                      0
#define JVAL_METHODID_PUT                        1

#define JKEY_ISSUEID_CC_BATTERY                  0x00098001 // battery
   ```
### Command - JVAL_METHODID_PUT & JKEY_ISSUEID_CC_BATTERY
   ```

   ```
#### Sample
   ```

   ```
### Report - JVAL_METHODID_EVENT & JKEY_ISSUEID_CC_BATTERY
   ```
level: level. Interpretation of level is device specific
ts: time stamp.  If this is zero, the callback has no data and hence other parameter values should be ignored.

void zwifd_battery_report(Reporter_t *reporter, uint8_t level, time_t ts)
	idx = 0;
	n_issueitem.data[idx++] = level; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
reporter_set_nodeid(ZWARE_REPORTER(), ifd_p->nodeid, ifd_p->epid);
zwifd_battery_report(ZWARE_REPORTER(), level, ts);
   ```

# Log
## MCTT
   ```
20210414 133912 [0/2-ZWAVES2] - <-- 9C65F9361C00/D808ACB4/0x0002/0 - zevent=0x0002.0,battery,100-level
   ```

## TOPIC
   ```
[3613/3614] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/D808ACB4/2/0/00098001, value: {"name":"Battery","value":100})
   ```

