# Wake Up Notification
## Sequence

   ```sequence
   dongle --> echo: MCTT (JKEY_ISSUEID_CC_WAKE_UP_NOTIFICATION)
   ```

## API
   ```
#define JVAL_METHODID_EVENT                      0
#define JVAL_METHODID_PUT                        1

#define JKEY_ISSUEID_CC_WAKE_UP_INTERVAL         0x00098401 // wakeup_interval
   ```
### Command - JVAL_METHODID_PUT & JKEY_ISSUEID_CC_WAKE_UP_NOTIFICATION
   ```

   ```
#### Sample
   ```

   ```
### Report - JVAL_METHODID_EVENT & JKEY_ISSUEID_CC_WAKE_UP_NOTIFICATION
   ```
void zwifd_wakeup_notification_report(Reporter_t *reporter, time_t ts)
	idx = 0;
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
reporter_set_nodeid_ex(ZWARE_REPORTER(), ifd_p->nodeid, ifd_p->epid);
zwifd_wakeup_notification_report(ZWARE_REPORTER(), ts);
   ```

# Log
## MCTT
   ```
20210414 134916 [0/2-ZWAVES2] - <-- 9C65F9361C00/D808ACB4/0x0002/0 - zevent=0x0002.0,wakeup
   ```

## TOPIC
   ```
[3988/3989] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/D808ACB4/2/0/00098402, value: {"name":"Wakeup Sensor"})
   ```
