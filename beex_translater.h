/***************************************************************************
 * Copyright (C) 2017 - 2020, Lanka Hsu, <lankahsu@gmail.com>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
#ifndef __BEEX_TRANSLATER_H__
#define __BEEX_TRANSLATER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#if (0)
#ifdef DEVICE_ENABLE_POLL_TEST
typedef int (*zwif_pool_fn)(zwifd_p ifd_p, zwpoll_req_t *poll_req);
#endif
#endif

#define LEN_OF_2BYTES_DEFINE 0xffff
#define LEN_OF_1BYTE_DEFINE  0xff
#define LEN_OF_ZNAME         64

typedef struct CommandClassName_Struct
{
	//int id;
	char name[LEN_OF_ZNAME];
#ifdef DEVICE_ENABLE_POLL_TEST
	zwif_pool_fn pollfn;
#endif
} CommandClassName;
extern CommandClassName CommandClassArray[LEN_OF_2BYTES_DEFINE];

typedef struct SensorName_Struct
{
	//int id;
	char name[LEN_OF_NAME64];
	char unit[0xF+1][LEN_OF_NAME64];
	const char *report;
} SensorName_t;
extern SensorName_t SensorNameArray[LEN_OF_1BYTE_DEFINE];

char* translate_node_type(uint8_t node_type);
char* translate_role_type(uint8_t role_type);
char* translate_device_class(uint8_t basic);

char *translate_specific(uint8_t generic, uint8_t specific);
char *translate_generic(uint8_t generic);

void translate_color_component_mask(uint16_t color_component_mask, char *mask_name);
char *translate_color_name(uint16_t id);
void translate_propty(uint8_t propty, char *propty_name);
void translate_ctl_role(uint8_t ctl_role, char *ctl_role_name);

char* translate_op_add_node(uint8_t add_node);
char* translate_op_rp_node(uint8_t rp_node);
char* translate_op_initiate(uint8_t initiate);
char* translate_op_update(uint8_t update);
char* translate_op_fw(uint8_t fw);
char* translate_op_health(uint8_t health);

#if (0)
char* translate_zw_fw_upgd(uint8_t upgrade_flg);
char* translate_zw_fw_updt_err(uint8_t status);
char* translate_zw_fw_updt_cmplt(uint8_t status);
char* translate_zw_fw_updt_restart(uint8_t status);
#endif

char* translate_zw_err(int result);

char* translate_node_category(uint8_t category);
char* translate_node_sts(uint8_t sts);
char* translate_nw_op(uint8_t op);
char* translate_alarm_type(uint8_t type);
char* translate_alarm_event(uint8_t type, uint8_t event, uint8_t prefix);
char* translate_alarm_event_report(uint8_t type, uint8_t event);
char* translate_alarm_ex_evt_type(uint8_t ex_evt_type);
char* translate_alarm_sts(uint8_t status);

char* translate_av_keyAttr(uint8_t keyAttr);

char* translate_application_busy_sts(uint8_t status);
char* translate_application_reject_sts(uint8_t status);

char* translate_csc_keyAttr(uint8_t keyAttr);

char* translate_basic_val(uint8_t val);

char* translate_battery_level(uint8_t level);

char* translate_bsensor_report(uint8_t type);
char* translate_bsensor_type(uint8_t type);

char* translate_methodid(uint16_t methodid);
char* translate_protocolid(uint16_t protocolid);

uint8_t translate_sensor_mask_to_unit(uint8_t sensor_unit_msk);
char* translate_sensor_unit(uint8_t type, uint8_t unit);
const char* translate_sensor_report(uint8_t type);
char* translate_sensor_type(uint8_t type);

char* translate_usrcod_sts(uint8_t sts);

char* translate_door_mode(uint8_t mode);
void translate_door_cond(uint8_t cond, char *cond_name);
char* translate_door_op_type(uint8_t type);

char* translate_meter_type(uint8_t type);
char* translate_meter_unit(uint8_t type, uint8_t unit);
void translate_meter_unit_sup(uint8_t type, uint8_t unit, char *unit_name);
char* translate_meter_rate(uint8_t rate_type);

char* translate_local_prot(uint8_t local_prot);
char* translate_rf_prot(uint8_t rf_prot);

char* translate_dur(uint8_t dur);

char* translate_multilevel_level(uint8_t level);

char* translate_switch_val(uint8_t val);
char* translate_switch_type(uint8_t type);

char* translate_thermostat_mode(uint8_t mode);
char* translate_thermostat_op_state(uint8_t state);
char* translate_thermostat_setpoint_type(uint8_t type);
char* translate_thermostat_fan_mode(uint8_t mode);
char* translate_thermostat_fan_state(uint8_t state);
char* translate_thermostat_setpoint_unit(uint8_t unit);

const char * translate_power_level(uint8_t lvl);
#if (0)
const char * translate_power_level_power(uint8_t power);
#endif
const char * translate_power_level_status(uint8_t status);

const char *translate_manufacturer(uint16_t vid);
const char *translate_product(uint16_t vid, uint16_t type, uint16_t pid);
const char *translate_cc(uint16_t cls);

#if (0)

const char * translate_controller_state_to_string(int state);
#endif
char *translate_CommandClass(int cc);

void translater_open(void);

char* translate_ctl_capabilities(uint8_t ctl_cap);
char* translate_ctl_zw_role(uint8_t ctl_zw_role);

char* translate_postset_reason(int reason);
char* translate_node_propty(uint8_t propty, uint8_t id);
char* translate_requested_keys(uint8_t req_keys, uint8_t id);


#ifdef __cplusplus
}
#endif
#endif
