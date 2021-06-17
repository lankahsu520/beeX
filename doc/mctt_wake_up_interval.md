# Wake Up Interval
## Sequence

   ```sequence
   dongle --> echo: MCTT (JKEY_ISSUEID_CC_WAKE_UP_INTERVAL)
   ```

## API
   ```
#define JKEY_ISSUEID_CC_WAKE_UP_INTERVAL         0x00098401 // wakeup_interval
   ```
### Command - JVAL_METHODID_PUT & JKEY_ISSUEID_CC_WAKE_UP_INTERVAL
   ```

   ```
#### Sample
   ```

   ```
### Report - JVAL_METHODID_EVENT & JKEY_ISSUEID_CC_WAKE_UP_INTERVAL
   ```
typedef struct
{
	uint32_t	min;		/**< minimum */
	uint32_t	max;		/**< maximum */
	uint32_t	def;		/**< default */
	uint32_t	interval;	/**< steps between min and max */
	uint32_t	cur;		/**< current */
	zwnoded_t	node;       /**< node to notify wake up */
}
zwif_wakeup_t, *zwif_wakeup_p;

void zwifd_wakeup_interval_report(Reporter_t *reporter, zwif_wakeup_p cap)
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
   ```
#### Sample
   ```
reporter_set_nodeid_ex(ZWARE_REPORTER(), ifd_p->nodeid, ifd_p->epid);
zwifd_wakeup_interval_report(ZWARE_REPORTER(), cap);
   ```

# Log
## MCTT
   ```
20210406 124147 - <-- 9C65F9361C00/0/0x02-ZWAVES2/D5942FD4/0x0004/0 - zevent=0x0004.0,interval,1800,432000,86400,1800,86400,1
   ```

## TOPIC
   ```
[3613/3614] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/D808ACB4/2/0/00098401, value: {"cur":86400,"def":86400,"interval":1800,"max":432000,"min":1800,"name":"Wakeup Interval","notify":0})
   ```
