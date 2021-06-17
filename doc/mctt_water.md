# Water
## Sequence

   ```sequence
   dongle --> echo: MCTT (JKEY_ISSUEID_WATER)
   ```

## API
   ```
#define JVAL_METHODID_EVENT                      0
#define JVAL_METHODID_PUT                        1

#define JKEY_ISSUEID_WATER                       0x00010006 // water
   ```
### Command - JVAL_METHODID_EVENT & JKEY_ISSUEID_WATER
   ```

   ```
#### Sample
   ```

   ```
### Report - JVAL_METHODID_EVENT & JKEY_ISSUEID_WATER
   ```
typedef struct
{
	uint8_t     type;           /**< Vendor specific alarm type*/
	uint8_t     level;          /**< Vendor specific alarm level*/
	uint8_t     ex_info;        /**< Flag to indicate the following extended info fields are valid. 1= valid; 0= invalid (no extended info fields)*/
	/*extended info fields*/
	uint8_t     ex_zensr_nodeid;/**< Zensor Net source node id. This field is 0 if device is not based on Zensor Net*/
	uint8_t     ex_status;      /**< Z-wave alarm status (ZW_ALRM_STS_XXX) */
	uint8_t     ex_type;        /**< Z-wave alarm type (ZW_ALRM_XXX) */
	uint8_t     ex_event;       /**< Z-wave alarm event (ZW_ALRM_EVT_XXX) */
	uint8_t     ex_has_sequence;/**< Flag to indicate whether this report has sequence number as stored in "ex_sequence_no". 1=valid; 0 = invalid */
	uint8_t     ex_sequence_no; /**< Z-wave alarm sequence number. Only valid if field ex_has_sequence is 1.*/
	uint8_t     ex_evt_len;     /**< Z-wave alarm event parameter length. Zero if the event has no parameter */
	uint8_t     ex_evt_type;    /**< Z-wave alarm event parameter type (ZW_ALRM_PARAM_XXX) */
	uint8_t     ex_evt_prm[1];  /**< Z-wave alarm event parameter place holder*/
}
zwalrm_t, *zwalrm_p;

static void zwifd_alrm_xxx_report(Reporter_t *reporter, uint32_t issueid, uint16_t mode, uint16_t submode, uint8_t category, zwalrm_p alrm_p)
	idx = 0;
	n_issueitem.data[idx++] = alrm_p->ex_event; // value
	n_issueitem.data[idx++] = alrm_p->ex_has_sequence; // value
	n_issueitem.data[idx++] = alrm_p->ex_sequence_no; // value
	n_issueitem.data[idx++] = alrm_p->ex_evt_type; // value
	n_issueitem.data[idx++] = alrm_p->ex_evt_len; // value
	if (alrm_p->ex_evt_len)
	{
		memcpy(n_issueitem.data+idx, alrm_p->ex_evt_prm, alrm_p->ex_evt_len);
		idx += alrm_p->ex_evt_len;
	}
	n_issueitem.data_len = idx;

void zwifd_alrm_water_report(Reporter_t *reporter, zwalrm_p alrm_p)
void zwifd_alrm_water_leak_report_bin(Reporter_t *reporter)
	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_WATER;
	alrm.ex_event = ZW_ALRM_EVT_LEAK;
void zwifd_alrm_water_off_report_bin(Reporter_t *reporter)
	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_WATER;
	alrm.ex_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
   ```
#### Sample
   ```
reporter_set_nodeid_ex(ZWARE_REPORTER(), ifd_p->nodeid, ifd_p->epid);
zwifd_alrm_water_report(ZWARE_REPORTER(), alarm_info);

zwifd_alrm_water_leak_report_bin(ZWARE_REPORTER());
zwifd_alrm_water_off_report_bin(ZWARE_REPORTER());
   ```

# Log
## MCTT
   ```
20210414 142149 [0/2-ZWAVES2] - <-- 9C65F9361C00/CA225443/0x0002/0 - zevent=0x0002.0,water,off
   ```

## TOPIC
   ```
[6523/6524] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/CA225443/2/0/00010006, value: {"name":"Flood Sensor","val":"idle"})
   ```
