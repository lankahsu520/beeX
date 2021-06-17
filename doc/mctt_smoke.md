# Smoke
## Sequence

   ```sequence
   dongle --> echo: MCTT (JKEY_ISSUEID_SMOKE)
   ```

## API
   ```
#define JVAL_METHODID_EVENT                      0
#define JVAL_METHODID_PUT                        1

#define JKEY_ISSUEID_SMOKE                       0x00010002 // smoke
   ```
### Command - JVAL_METHODID_PUT & JKEY_ISSUEID_SMOKE
   ```

   ```
#### Sample
   ```

   ```
### Report - JVAL_METHODID_EVENT & JKEY_ISSUEID_SMOKE
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

void zwifd_alrm_smoke_report(Reporter_t *reporter, zwalrm_p alrm_p)
void zwifd_alrm_smoke_on_report_bin(Reporter_t *reporter)
	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_SMOKE;
	alrm.ex_event = ZW_ALRM_EVT_SMOKE;
void zwifd_alrm_smoke_off_report_bin(Reporter_t *reporter)
	zwalrm_t alrm = {0};
	alrm.ex_type = ZW_ALRM_SMOKE;
	alrm.ex_event = ZW_ALRM_EVT_INACTIVE_CLEAR;
   ```
#### Sample
   ```
reporter_set_nodeid_ex(ZWARE_REPORTER(), ifd_p->nodeid, ifd_p->epid);
zwifd_alrm_smoke_report(ZWARE_REPORTER(), alarm_info);

zwifd_alrm_smoke_on_report_bin(ZWARE_REPORTER());
zwifd_alrm_smoke_off_report_bin(ZWARE_REPORTER());
   ```

# Log
## MCTT
   ```
20210503 213242 [0/2-ZWAVES2] - <-- B827EB40D4FD/FA3EAE73/4/0 - zevent=4.0,smoke,3-ZW_ALRM_EVT_SMOKE_TEST
   ```

## TOPIC
   ```
[23616/23618] bee_topic_issue_cb:146 - (topic: 0/2/B827EB40D4FD/FA3EAE73/4/0/00010002, value: {"name":"Smoke Sensor","val":"ZW_ALRM_EVT_SMOKE_TEST","value":3})
   ```

