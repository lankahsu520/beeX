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
#ifndef __BEEX_REPORT_H__
#define __BEEX_REPORT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

#ifdef PJ_BEEX_ZWARE
#include "beex_zware.h"
#elif defined(PJ_HAS_ZWARE)
#include "zware/zip_api_pte.h"
#include "zware/zip_api.h"
#else
#include "beex_zware.h"
#endif
#include "beex_def.h"

//******************************************************************************
//** define **
//******************************************************************************
#define ZWRESULT_CHECK_VAL( x, y, z, format, args...) \
	if (y != 0) \
	{ \
		DBG_ER_LN("%s fail (%d %s) !!! "format, x, y, translate_zw_err(y), ## args); \
	} \
	else if (z==1) \
	{ \
		DBG_WN_LN("%s ok (%d %s) !!! "format, x, y, translate_zw_err(y), ## args); \
	}

#define ZWRESULT_CHECK( x, y, z) \
	if (y != 0) \
	{ \
		DBG_ER_LN("%s fail (%d %s) !!!", x, y, translate_zw_err(y)); \
	} \
	else if (z==1) \
	{ \
		DBG_WN_LN("%s ok (%d %s) !!!", x, y, translate_zw_err(y)); \
	}

#define ZWIF_DBG(format, args...) DBG_IF_LN_G(format, ## args)
#define ZWCB_DBG(format, args...) DBG_IF_LN_G(format, ## args)
#define ZWCB_IFD_DBG(ifd_p) \
	{ \
		if (dbg_more>=DBG_LVL_INFO) \
		{ \
			char propty_name[128]=""; \
			translate_propty(ifd_p->propty, propty_name); \
			ZWCB_DBG("(nodeid: %u.%u.%02x %s, ver: %u, propty: %02Xh %s)", ifd_p->nodeid, ifd_p->epid, ifd_p->cls, translate_cc(ifd_p->cls), ifd_p->ver, ifd_p->propty, propty_name); \
		} \
	}

#define ZWORDER_DBG(format, args...) DBG_LN_LB_0(format"\n", ## args)
#define ZWWATCH_DBG(format, args...) DBG_LN_LC_0(format"\n", ## args)

#define WATCH_DISABLE 1
#define WATCH_ENABLE 0

typedef enum
{
	ISSUE_TYPE_ID_REPORTER=0,
	ISSUE_TYPE_ID_COMMANDER,
	ISSUE_TYPE_ID_MAX,
} ISSUE_TYPE_ID;

typedef enum
{
	NODE_DIRTY_ID_NORMAL=0,
	NODE_DIRTY_ID_BOOTUP,
	NODE_DIRTY_ID_MAX,
} NODE_DIRTY_ID;

#ifndef LEN_OF_MAC
#define LEN_OF_MAC 24
#endif
#ifndef LEN_OF_UUID
#define LEN_OF_UUID 40
#endif

typedef enum
{
	ORDER_FN_ID_GW,
	ORDER_FN_ID_ADDING,
	ORDER_FN_ID_REMOVING,
	ORDER_FN_ID_ABORTING,
	ORDER_FN_ID_LEARNING,
	ORDER_FN_ID_NW,
	ORDER_FN_ID_RESETING,
	ORDER_FN_ID_REPLACING,
	ORDER_FN_ID_RMFAILED,
	ORDER_FN_ID_NODE,
	ORDER_FN_ID_NOP,

	ORDER_FN_ID_BASIC,

	ORDER_FN_ID_SWITCH,
	ORDER_FN_ID_LEVEL,

	ORDER_FN_ID_INFRARED,

	ORDER_FN_ID_MAX,
} ORDER_FN_ID;

typedef enum
{
	NOTIFY_FN_ID_GW,
	NOTIFY_FN_ID_READY,
	NOTIFY_FN_ID_ADDING,
	NOTIFY_FN_ID_REMOVING,
	NOTIFY_FN_ID_ABORTING,
	NOTIFY_FN_ID_LEARNING,
	NOTIFY_FN_ID_NW,
	NOTIFY_FN_ID_RESETING,
	NOTIFY_FN_ID_OP,

	NOTIFY_FN_ID_REPLACING,
	NOTIFY_FN_ID_RMFAILED,

	NOTIFY_FN_ID_NODE,
	NOTIFY_FN_ID_NOP,

	NOTIFY_FN_ID_TAMPER,
	NOTIFY_FN_ID_DR_WIN,
	NOTIFY_FN_ID_MOTION,

	NOTIFY_FN_ID_SMOKE,
	NOTIFY_FN_ID_WATER,
	NOTIFY_FN_ID_TEMPATURE,
	NOTIFY_FN_ID_LUMINANCE,
	NOTIFY_FN_ID_HUMIDITY,

	NOTIFY_FN_ID_BASIC,
	NOTIFY_FN_ID_BASIC_SET,

	NOTIFY_FN_ID_SWITCH,
	NOTIFY_FN_ID_DIMMER,
	NOTIFY_FN_ID_METER,
	NOTIFY_FN_ID_CENTRAL_SCENE_NOTIFICATION,
	NOTIFY_FN_ID_CONFIGURATION,
	NOTIFY_FN_ID_BATTERY,

	NOTIFY_FN_ID_WAKE_UP_INTERVAL,
	NOTIFY_FN_ID_WAKE_UP_NOTIFICATION,

	NOTIFY_FN_ID_INFRARED,

	NOTIFY_FN_ID_MAX,
} NOTIFY_FN_ID;

typedef struct Identity_Struct
{
	char c_macid[LEN_OF_MAC]; // LEN_OF_MAC 24
	char c_uuid[LEN_OF_UUID]; // LEN_OF_UUID 40
	char c_nodeid[LEN_OF_UUID];
	uint8_t epid; // 0xFF
} Identity_t;

typedef struct Commander_Struct
{
	uint16_t methodid;
	uint16_t protocolid;
	Identity_t id_frm;
	Identity_t id_to;
} Commander_t;

typedef struct Reporter_Struct
{
	uint16_t methodid;
	uint16_t protocolid;
	Identity_t id_frm;
} Reporter_t;

#define MAX_DATA_OF_ISSUE_ITEM (LEN_OF_BUF1024+128)
typedef struct IssueItem_Struct
{
	union
	{
		Reporter_t reporter;
		Commander_t commander;
	};

	uint32_t issueid;
	uint16_t mode;
	uint16_t submode;
	uint16_t category;
	uint32_t timestamp;
	// 71+4+2+2+2+4 = 85

	uint16_t data_len; // to support 0xFF 0xFF
	char data[MAX_DATA_OF_ISSUE_ITEM]; // 1024+128
} IssueItem_t;

typedef struct IssueLogging_Struct
{
	void* next;

	IssueItem_t c_issueitem;
} IssueLogging_t;

typedef void (*zw_report_fn)(void *userdata, IssueItem_t *c_issueitem);
typedef void (*notify_fn)(void *userdata, IssueItem_t *c_issueitem);

typedef struct Notify_Struct
{
	void* next;

	//char c_macid[LEN_OF_MAC]; // LEN_OF_MAC 24
	char c_uuid[LEN_OF_UUID]; // LEN_OF_UUID 40

	notify_fn order_item_cb[ORDER_FN_ID_MAX];
	notify_fn watch_item_cb[NOTIFY_FN_ID_MAX];

	notify_fn order_cb;
	notify_fn watch_cb;
	notify_fn others_cb;
} Notify_t;

//******************************************************************************
//** function **
//******************************************************************************
void tracker_set_mac(char *macid);

void tracker_notify_set_uuid(Notify_t *notify, char *c_uuid);
void tracker_notify_register(Notify_t *notify);
void tracker_logging_set_on(void);
void tracker_logging_set_off(void);
void tracker_logging_show(void);

void tracker_others_simple(void *userdata, IssueItem_t *c_issueitem, uint8_t dbg);

void tracker_order_simple(void *userdata, IssueItem_t *c_issueitem, uint8_t dbg);
void tracker_order_set_on(void);
void tracker_order_set_off(void);
void tracker_watch_simple(void *userdata, IssueItem_t *c_issueitem, uint8_t dbg);
void tracker_watch_set_on(void);
void tracker_watch_set_off(void);

//** commander **
void commander_set_methodid(Commander_t *commander, uint16_t methodid);
void commander_set_protocolid(Commander_t *commander, uint16_t protocolid);

void commander_set_to_nodeid(Commander_t *commander, char *c_nodeid, uint8_t epid);
void commander_set_to_nodeid_ex(Commander_t *commander, uint16_t nodeid, uint8_t epid);
void commander_set_to_mac(Commander_t *commander, char *c_macid);
void commander_set_to_uuid(Commander_t *commander, char *c_uuid);

void commander_set_frm_nodeid(Commander_t *commander, char *c_nodeid, uint8_t epid);
void commander_set_frm_nodeid_ex(Commander_t *commander, uint16_t nodeid, uint8_t epid);
void commander_set_frm_mac(Commander_t *commander, char *c_macid);
void commander_set_frm_uuid(Commander_t *commander, char *c_uuid);

void commander_cp_to(Commander_t *commander, Reporter_t *reporter);
void commander_cp_frm(Commander_t *commander, Reporter_t *reporter);

//** reporter **
void reporter_set_methodid(Reporter_t *reporter, uint16_t methodid);
void reporter_set_protocolid(Reporter_t *reporter, uint16_t protocolid);

void reporter_set_nodeid(Reporter_t *reporter, char *c_nodeid, uint8_t epid);
void reporter_set_nodeid_ex(Reporter_t *reporter, uint16_t nodeid, uint8_t epid);
void reporter_set_mac(Reporter_t *reporter, char *c_macid);
void reporter_set_uuid(Reporter_t *reporter, char *c_uuid);
void reporter_close(void);
void reporter_init(void *userdata, char *macid);
void do_report_enable(int enable);

void controller_gw_ex(Reporter_t *reporter, uint8_t act, char *data, unsigned short data_len);
void controller_gw_cfg(Reporter_t *reporter, uint8_t param_num, uint32_t value);
void controller_gw_debug(Reporter_t *reporter, uint8_t lvl);
void controller_gw_watch_off(Reporter_t *reporter, uint8_t watch_off);
void controller_gw_echo(Reporter_t *reporter, char *echo);
void controller_act_gw_ex(Commander_t *commander, uint8_t act, char *data, unsigned short data_len);
void controller_act_gw_cfg(Commander_t *commander, uint8_t param_num, uint32_t value);
void controller_act_gw_debug(Commander_t *commander, uint8_t lvl);
void controller_act_gw_watch_off(Commander_t *commander, uint8_t watch_off);
void controller_act_gw_echo(Commander_t *commander, char *echo);

void controller_status_added(Reporter_t *reporter);
void controller_status_adding_done(Reporter_t *reporter);
void controller_status_adding_sec(Reporter_t *reporter, uint8_t highest_lvl, uint8_t curr_lvl);
void controller_status_adding_csa(Reporter_t *reporter, char *csa_pin);
void controller_status_adding_ssa_set(Reporter_t *reporter, uint8_t res);
void controller_status_adding_ssa(Reporter_t *reporter, char *dsk);
void controller_status_adding_xxx(Reporter_t *reporter, uint8_t flag);
void controller_status_adding_fail(Reporter_t *reporter);
void controller_status_adding(Reporter_t *reporter);
void controller_act_adding(Commander_t *commander, char *dsk);

void controller_status_removed(Reporter_t *reporter);
void controller_status_removing_done(Reporter_t *reporter);
void controller_status_removing_fail(Reporter_t *reporter);
void controller_status_removing(Reporter_t *reporter);
void controller_act_removing(Commander_t *commander);

void controller_status_rmfailed_rm(Reporter_t *reporter);
void controller_status_rmfailed_done(Reporter_t *reporter);
void controller_status_rmfailed_fail(Reporter_t *reporter);
void controller_status_rmfailed(Reporter_t *reporter);
void controller_act_rmfailed(Commander_t *commander);

void controller_status_replaced(Reporter_t *reporter);
void controller_status_replacing_done(Reporter_t *reporter);
void controller_status_replacing_xxx(Reporter_t *reporter, uint8_t flag);
void controller_status_replacing_fail(Reporter_t *reporter);
void controller_status_replacing(Reporter_t *reporter);
void controller_act_replacing(Commander_t *commander, char *dsk);

void controller_status_reset_rm(Reporter_t *reporter, char *homeid);
void controller_status_reset_done(Reporter_t *reporter);
void controller_status_reset_fail(Reporter_t *reporter);
void controller_status_reseting(Reporter_t *reporter);
void controller_act_reseting(Commander_t *commander);

void controller_status_aborting_done(Reporter_t *reporter);
void controller_status_aborting_fail(Reporter_t *reporter);
void controller_act_aborting(Commander_t *commander);

void controller_status_learned(Reporter_t *reporter);
void controller_status_learning_done(Reporter_t *reporter, uint8_t reset);
void controller_status_learning_ssa(Reporter_t *reporter, char *ssa_pin);
void controller_status_learning_xxx(Reporter_t *reporter, uint8_t sts);
void controller_status_learning_fail(Reporter_t *reporter);
void controller_status_learning(Reporter_t *reporter);
void controller_act_learning(Commander_t *commander);

void controller_status_ready(Reporter_t *reporter, uint8_t last_op, uint16_t nodeid);

void controller_status_op(Reporter_t *reporter, uint8_t curr_op);
void controller_status_save(Reporter_t *reporter);
void controller_status_normal(Reporter_t *reporter);

void controller_status_node_info(Reporter_t *reporter, zwnoded_p noded_p);
void controller_status_node_vendor(Reporter_t *reporter, uint16_t vid, uint16_t ptype, uint16_t pid);
void controller_status_node_alive(Reporter_t *reporter);
void controller_status_node_sleeping(Reporter_t *reporter);
void controller_status_node_type(Reporter_t *reporter, zwplus_info_t *zwplus_info_p);
void controller_status_node_generic(Reporter_t *reporter, uint8_t generic, uint8_t specific);
void controller_act_node_see_you(Commander_t *commander);
void controller_act_node_hello(Commander_t *commander);

void controller_status_nw_ex(Reporter_t *reporter, uint8_t act, char *data, unsigned short data_len);
void controller_status_nw_noroute(Reporter_t *reporter);
void controller_status_nw_busy(Reporter_t *reporter);
void controller_status_nw_no_ack(Reporter_t *reporter);
void controller_status_nw_sys_err(Reporter_t *reporter);
void controller_status_nw_homeid(Reporter_t *reporter, zwnetd_p nw_desp, char *uuid, uint16_t ctl_id, uint16_t suc);
void controller_status_nw_serial(Reporter_t *reporter, SerialInitData_t *serial);
void controller_status_nw_bootup(Reporter_t *reporter);
void controller_status_nw_debug(Reporter_t *reporter, uint8_t lvl);
void controller_status_nw_watch_off(Reporter_t *reporter, uint8_t watch_off);
void controller_act_nw_ex(Commander_t *commander, uint8_t act, char *data, unsigned short data_len);

void zwifd_probe_report(Reporter_t *reporter, int8_t tx_sts);
void controller_act_nop(Commander_t *commander, char *data, unsigned short data_len);
void zwifd_postset_report(Reporter_t *reporter, uint16_t cls_id, void *usr_param, int reason);

// Alarm
void zwifd_alrm_water_report(Reporter_t *reporter, zwalrm_p alrm_p);
void zwifd_alrm_water_leak_report_bin(Reporter_t *reporter);
void zwifd_alrm_water_off_report_bin(Reporter_t *reporter);

void zwifd_alrm_door_report(Reporter_t *reporter, zwalrm_p alrm_p);
void zwifd_alrm_door_open_report_bin(Reporter_t *reporter);
void zwifd_alrm_door_closed_report_bin(Reporter_t *reporter);

void zwifd_alrm_glass_report(Reporter_t *reporter, zwalrm_p alrm_p);
void zwifd_alrm_glass_on_report_bin(Reporter_t *reporter);
void zwifd_alrm_glass_off_report_bin(Reporter_t *reporter);

void zwifd_alrm_tamper_report(Reporter_t *reporter, zwalrm_p alrm_p);
void zwifd_alrm_tamper_open_report_bin(Reporter_t *reporter);
void zwifd_alrm_tamper_closed_report_bin(Reporter_t *reporter);

void zwifd_alrm_smoke_report(Reporter_t *reporter, zwalrm_p alrm_p);
void zwifd_alrm_smoke_on_report_bin(Reporter_t *reporter);
void zwifd_alrm_smoke_off_report_bin(Reporter_t *reporter);

void zwifd_alrm_motion_report(Reporter_t *reporter, zwalrm_p alrm_p);
void zwifd_alrm_motion_on_report_bin(Reporter_t *reporter);
void zwifd_alrm_motion_off_report_bin(Reporter_t *reporter);

void zwifd_alrm_co_report(Reporter_t *reporter, zwalrm_p alrm_p);
void zwifd_alrm_co_on_report_bin(Reporter_t *reporter);
void zwifd_alrm_co_off_report_bin(Reporter_t *reporter);

void zwifd_alrm_co2_report(Reporter_t *reporter, zwalrm_p alrm_p);
void zwifd_alrm_co2_on_report_bin(Reporter_t *reporter);
void zwifd_alrm_co2_off_report_bin(Reporter_t *reporter);

void zwifd_alrm_intrusion_report(Reporter_t *reporter, zwalrm_p alrm_p);
void zwifd_alrm_heat_report(Reporter_t *reporter, zwalrm_p alrm_p);
void zwifd_alrm_heat_on_report_bin(Reporter_t *reporter);
void zwifd_alrm_heat_off_report_bin(Reporter_t *reporter);

void zwifd_alrm_power_over_load_report(Reporter_t *reporter, zwalrm_p alrm_p);
void zwifd_alrm_power_report(Reporter_t *reporter, zwalrm_p alrm_p);
void zwifd_alrm_power_off_report_bin(Reporter_t *reporter);

void zwifd_alrm_system_report(Reporter_t *reporter, zwalrm_p alrm_p);
void zwifd_alrm_emergency_report(Reporter_t *reporter, zwalrm_p alrm_p);
void zwifd_alrm_clock_report(Reporter_t *reporter, zwalrm_p alrm_p);
void zwifd_alrm_first_report(Reporter_t *reporter, zwalrm_p alrm_p);

void zwifd_alrm_vendor_report(Reporter_t *reporter, uint8_t type, uint8_t level);

// Av
void zwifd_av_set(Reporter_t *reporter, uint16_t *cmds, uint8_t num_cmds, uint8_t key_attr);
void zwifd_av_report(Reporter_t *reporter, uint16_t length, uint8_t *mask);

// Application
void zwifd_appl_busy_report(Reporter_t *reporter, uint8_t status, uint8_t wait_tm);
void zwifd_appl_reject_report(Reporter_t *reporter, uint8_t status);

// Basic
zwbasic_t *basic_dat_assign(uint8_t curr_val, uint8_t tgt_val, uint8_t dur);
void zwifd_basic_report(Reporter_t *reporter, zwbasic_p val_p, time_t ts);
void zwifd_basic_set(Reporter_t *reporter, uint8_t level, time_t ts);
void zwifd_act_basic(Commander_t *commander, uint8_t val);

// Battery
void zwifd_battery_report(Reporter_t *reporter, uint8_t level, time_t ts);

// Binary Sensor
void zwifd_bsensor_siren_report(Reporter_t *reporter, uint8_t state, uint8_t type, time_t ts);
void zwifd_bsensor_freeze_report(Reporter_t *reporter, uint8_t state, uint8_t type, time_t ts);
void zwifd_bsensor_gp_report(Reporter_t *reporter, uint8_t state, uint8_t type, time_t ts);
void zwifd_bsensor_sup_report(Reporter_t *reporter, uint8_t type_len, uint8_t *type);

// Clock
void zwifd_clock_report(Reporter_t *reporter, uint8_t weekday, uint8_t hour, uint8_t minute);

// Color
void zwifd_color_report(Reporter_t *reporter, uint8_t colorComponentId, uint8_t value, uint8_t target_val, uint8_t dur);
void zwifd_color_component_mask_report(Reporter_t *reporter, uint8_t colorComponentMask1, uint8_t colorComponentMask2);
void zwifd_color_component_sup_report(Reporter_t *reporter, uint8_t *comp_id, uint8_t comp_cnt);

// Config
void zwifd_config_report(Reporter_t *reporter, zwconfig_p param);
void zwifd_act_config(Commander_t *commander, uint8_t param_num, uint8_t use_default, int32_t val);

// Scene
void zwifd_csc_report(Reporter_t *reporter, zwcsc_notif_p data, time_t ts);
void zwifd_csc_cfg_report(Reporter_t *reporter, int slow_refresh, time_t ts);
void zwifd_csc_sup_report(Reporter_t *reporter, uint8_t scene_cnt, uint8_t sameKA, uint8_t KA_array_len, uint8_t *KA_array, uint8_t slow_rfsh);

// Door Lock
void zwifd_dlck_report(Reporter_t *reporter, zwdlck_op_p op_p, time_t ts);
void zwifd_dlck_cfg_report(Reporter_t *reporter, zwdlck_cfg_p config, time_t ts);

// Group
void zwifd_group_report(Reporter_t *reporter, uint8_t group, uint8_t max_cnt, uint8_t cnt, grp_member_t *grp_member);
void zwifd_group_sup_report(Reporter_t *reporter, uint8_t max_grp);
void zwifd_group_active_report(Reporter_t *reporter,  uint8_t group);

// Meter
zwmeter_dat_t *meter_dat_assign(uint8_t type, uint8_t unit, uint8_t rate_type,uint8_t precision, uint16_t delta_time, float val, float prv_val);
void zwifd_meter_report(Reporter_t *reporter, zwmeter_dat_p data, time_t ts);
void zwifd_meter_sup_report(Reporter_t *reporter, zwmeter_cap_p meter_cap);

void zwifd_power_level_report(Reporter_t *reporter, uint8_t lvl, uint8_t timeout);
void zwifd_power_level_test_report(Reporter_t *reporter, uint8_t node_id, uint8_t status, uint16_t frame_cnt);

void zwifd_protection_report(Reporter_t *reporter, uint8_t local_prot, uint8_t rf_prot, time_t ts);

zwsensor_t *zwsensor_dat_assign(uint8_t type, uint8_t precision, uint8_t unit, float val);

// Sensor
void zwifd_sensor_co_lvl_report(Reporter_t *reporter, zwsensor_p data, time_t ts);
void zwifd_sensor_co2_lvl_report(Reporter_t *reporter, zwsensor_p data, time_t ts);
void zwifd_sensor_humidity_report(Reporter_t *reporter, zwsensor_p data, time_t ts);
void zwifd_sensor_luminance_report(Reporter_t *reporter, zwsensor_p data, time_t ts);
void zwifd_sensor_temperature_report(Reporter_t *reporter, zwsensor_p data, time_t ts);
void zwifd_sensor_uv_report(Reporter_t *reporter, zwsensor_p data, time_t ts);
void zwifd_sensor_voc_report(Reporter_t *reporter, zwsensor_p data, time_t ts);

void zwifd_sensor_sup_report(Reporter_t *reporter, uint8_t type_len, uint8_t *type);
void zwifd_sensor_unit_sup_report(Reporter_t *reporter, uint8_t sensor_type, uint8_t sensor_unit_msk);

// Switch
zwswitch_t *switch_dat_assign(uint8_t curr_val, uint8_t tgt_val, uint8_t dur);
void zwifd_switch_report(Reporter_t *reporter, zwswitch_p val_p, time_t ts);
void zwifd_act_switch(Commander_t *commander, uint8_t val);

// Plug
void zwifd_plug_report(Reporter_t *reporter, zwswitch_p val_p, time_t ts);

// Switch All
void zwifd_switch_all_report(Reporter_t *reporter, uint8_t mode);

// Switch Multilevel
zwlevel_dat_t *switch_multilevel_dat_assign(uint8_t curr_val, uint8_t tgt_val, uint8_t dur);
void zwifd_switch_multilevel_report(Reporter_t *reporter, zwlevel_dat_p val, time_t ts);
void zwifd_act_switch_multilevel(Commander_t *commander, uint8_t val, uint8_t dur);
void zwifd_switch_multilevel_sup_report(Reporter_t *reporter,  uint8_t pri_type, uint8_t sec_type);

// Thermostat
void zwifd_thermostat_mode_report(Reporter_t *reporter, uint8_t mode, uint8_t *manf_dat, uint8_t manf_dat_cnt, time_t ts);
void zwifd_thermostat_mode_sup_report(Reporter_t *reporter, uint8_t mode_len, uint8_t *mode);
void zwifd_thermostat_operating_report(Reporter_t *reporter, uint8_t state, time_t ts);
void zwifd_thermostat_setpoint_report(Reporter_t *reporter, zwsetp_p data, time_t ts);
void zwifd_thermostat_setpoint_sup_report(Reporter_t *reporter, uint8_t type_len, uint8_t *type);
void zwifd_thermostat_setpoint_sup_range_report(Reporter_t *reporter, uint8_t type, zwsetp_p min, zwsetp_p max);
void zwifd_thermostat_fan_mode_report(Reporter_t *reporter, uint8_t off, uint8_t mode);
void zwifd_thermostat_fan_mode_sup_report(Reporter_t *reporter, uint8_t off, uint8_t mode_len, uint8_t *mode);
void zwifd_thermostat_fan_state_report(Reporter_t *reporter, uint8_t state);

// User Code
void zwifd_usr_code_report(Reporter_t *reporter, zwusrcod_p usr_cod);
void zwifd_usr_sup_report(Reporter_t *reporter, uint8_t  usr_num);

// Version
void zwifd_vercc_report(Reporter_t *reporter, uint16_t cls, uint8_t ver);

void zwifd_verzw_report(Reporter_t *reporter, uint16_t proto_ver, uint16_t app_ver, uint8_t lib_type, ext_ver_t *ext_ver);

// Wake up
void zwifd_wakeup_interval_report(Reporter_t *reporter, zwif_wakeup_p cap);
void zwifd_wakeup_notification_report(Reporter_t *reporter, time_t ts);

void zwifd_infrared_report(Reporter_t *reporter, ir_manager_p val_p, time_t ts);
void zwifd_act_infrared(Commander_t *commander, ir_manager_p val_p);

void zwifd_snd_switch_tone_play_report(Reporter_t *reporter, uint8_t tone_id, uint8_t tone_vol, time_t ts, uint16_t stat_num);
void zwifd_act_snd_switch_tone_play(Commander_t *commander, uint8_t tone_id, uint8_t tone_vol);

#ifdef __cplusplus
}
#endif
#endif
