# Meter
## Sequence

   ```sequence
   dongle --> echo: MCTT (JKEY_ISSUEID_CC_METER)
   ```

## API
   ```
#define JVAL_METHODID_EVENT                      0
#define JVAL_METHODID_PUT                        1

#define JKEY_ISSUEID_CC_METER                    0x00093201 // meter
   ```
### Command - JVAL_METHODID_PUT & JKEY_ISSUEID_CC_METER
   ```

   ```
#### Sample
   ```

   ```
### Report - JVAL_METHODID_EVENT & JKEY_ISSUEID_CC_METER
   ```
typedef struct
{
	uint8_t  type; /**< ZW_METER_TYPE_XXX */
	uint8_t  precision; /**< Decimal places of the value.  The decimal value 1025 with precision 2 is therefore equal to 10.25.*/
	uint8_t  unit; /**< ZW_METER_UNIT_XXX */
 	uint8_t  size; /**< data size: 1,2,or 4 bytes*/
	uint8_t  data[4]; /**< meter data (a signed number) with the first byte is the most significant byte*/
	uint16_t delta_time; /**< elapsed time in seconds between the Meter Value and the Previous Meter Value measurements. If delta_time = 0, it means no Previous Meter Value measurement*/
	uint8_t  prv_data[4]; /**< previous meter data (a signed number) with the first byte is the most significant byte. This field is valid only if delta_time > 0*/
	uint8_t  rate_type; /**< rate type: ZW_METER_RATE_XXX */
}
zwmeter_dat_t, *zwmeter_dat_p;

void zwifd_meter_report(Reporter_t *reporter, zwmeter_dat_p data, time_t ts)
	idx = 0;
	n_issueitem.data[idx++] = data->type; // value
	n_issueitem.data[idx++] = data->precision; // value
	n_issueitem.data[idx++] = data->unit; // value
	n_issueitem.data[idx++] = data->size; // value
	n_issueitem.data[idx++] = data->data[0]; // value
	n_issueitem.data[idx++] = data->data[1]; // value
	n_issueitem.data[idx++] = data->data[2]; // value
	n_issueitem.data[idx++] = data->data[3]; // value
	big_endian2byte(2, data->delta_time, (unsigned char *)n_issueitem.data+idx);
	idx += 2;
	n_issueitem.data[idx++] = data->prv_data[0]; // value
	n_issueitem.data[idx++] = data->prv_data[1]; // value
	n_issueitem.data[idx++] = data->prv_data[2]; // value
	n_issueitem.data[idx++] = data->prv_data[3]; // value
	n_issueitem.data[idx++] = data->rate_type; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
reporter_set_nodeid(ZWARE_REPORTER(), ifd_p->nodeid, ifd_p->epid);
zwifd_meter_report(ZWARE_REPORTER(), data, ts);
   ```

# Log
## MCTT
   ```
20210414 140708 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0002/0 - zevent=0x0002.0,meter_sup,1-ZW_METER_TYPE_ELEC,1-reset supported,117-kWh/W/V/A/power factor
20210414 140706 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0002/0 - zevent=0x0002.0,meter,1-ZW_METER_TYPE_ELEC,0-kWh,1-ZW_METER_RATE_IMPORT,298-delta_time,0.000,0.000
20210414 140706 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0002/0 - zevent=0x0002.0,meter,1-ZW_METER_TYPE_ELEC,2-W,1-ZW_METER_RATE_IMPORT,0-delta_time,0.000,0.000
20210414 140707 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0002/0 - zevent=0x0002.0,meter,1-ZW_METER_TYPE_ELEC,4-V,1-ZW_METER_RATE_IMPORT,0-delta_time,112.200,0.000
20210414 140707 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0002/0 - zevent=0x0002.0,meter,1-ZW_METER_TYPE_ELEC,5-A,1-ZW_METER_RATE_IMPORT,0-delta_time,0.000,0.000
20210414 140707 [0/2-ZWAVES2] - <-- 9C65F9361C00/CED69735/0x0002/0 - zevent=0x0002.0,meter,1-ZW_METER_TYPE_ELEC,6-power factor,1-ZW_METER_RATE_IMPORT,0-delta_time,0.000,0.000
   ```

## TOPIC
   ```
[5226/5227] bee_topic_issue_cb:92 - (topic: 0/2/9C65F9361C00/CED69735/2/0/00093201, value: {"1":{"0":{"delta_time":298,"name":"kWh","prv_value":0.0,"rate_type":1,"value":0.0},"2":{"delta_time":0,"name":"W","prv_value":0.0,"rate_type":1,"value":0.0},"4":{"delta_time":0,"name":"V","prv_value":0.0,"rate_type":1,"value":112.0},"5":{"delta_time":0,"name":"A","prv_value":0.0,"rate_type":1,"value":0.0},"6":{"delta_time":0,"name":"power factor","prv_value":0.0,"rate_type":1,"value":0.0},"name":"ZW_METER_TYPE_ELEC"}})
   ```
