# Luminance
## Sequence

   ```sequence
   dongle --> echo: MCTT (JKEY_ISSUEID_LUM)
   ```

## API
   ```
#define JVAL_METHODID_EVENT                      0
#define JVAL_METHODID_PUT                        1

#define JKEY_ISSUEID_LUM                         0x00030003 // Luminance sensor
   ```
### Command - JVAL_METHODID_PUT & JKEY_ISSUEID_LUM
   ```

   ```
#### Sample
   ```

   ```
### Report - JVAL_METHODID_EVENT & JKEY_ISSUEID_LUM
   ```
typedef struct
{
	uint8_t     type;		/**< ZW_SENSOR_TYPE_XXX */
	uint8_t     precision;  /**< Decimal places of the value.  The decimal value 1025 with precision 2 is therefore equal to 10.25.*/
	uint8_t     unit;	    /**< ZW_SENSOR_UNIT_XXX */
	uint8_t     size;	    /**< data size: 1,2,or 4 bytes*/
	uint8_t     data[4];	/**< sensor data (a signed number) with the first byte is the most significant byte*/
}
zwsensor_t, *zwsensor_p;

void zwifd_sensor_luminance_report(Reporter_t *reporter, zwsensor_p data, time_t ts)
	idx = 0;
	n_issueitem.data[idx++] = data->type; // value
	n_issueitem.data[idx++] = data->precision; // value
	n_issueitem.data[idx++] = data->unit; // value
	n_issueitem.data[idx++] = data->size; // value
	n_issueitem.data[idx++] = data->data[0]; // value
	n_issueitem.data[idx++] = data->data[1]; // value
	n_issueitem.data[idx++] = data->data[2]; // value
	n_issueitem.data[idx++] = data->data[3]; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
reporter_set_nodeid_ex(ZWARE_REPORTER(), ifd_p->nodeid, ifd_p->epid);
zwifd_sensor_luminance_report(ZWARE_REPORTER(), data, ts);
   ```

# Log
## MCTT
   ```
20210414 133912 [0/2-ZWAVES2] - <-- 9C65F9361C00/D808ACB4/0x0002/0 - zevent=0x0002.0,luminance,3-ZW_SENSOR_TYPE_LUM,94.000,0-%
   ```

## TOPIC
   ```
[3613/3614] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/D808ACB4/2/0/00030003, value: {"class":"Luminance Sensor","type":3,"type_name":"ZW_SENSOR_TYPE_LUM","unit":0,"unit_name":"%","value":94.0})
   ```
