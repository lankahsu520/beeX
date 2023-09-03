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
#include <math.h> //map

#include "beex_api.h"

#include "ZW_classcmd.h"

CommandClassName CommandClassArray[LEN_OF_2BYTES_DEFINE];
SensorName_t SensorNameArray[LEN_OF_1BYTE_DEFINE];

char* translate_node_type(uint8_t node_type)
{
	switch (node_type)
	{
		case 0x00:
			return "NODE_TYPE_ZWAVEPLUS_NODE";
			break;
		case 0x02:
			return "NODE_TYPE_ZWAVEPLUS_FOR_IP_GATEWAY";
			break;
		default:
			return "NODE_TYPE_XXX";
			break;
	}
}

char* translate_role_type(uint8_t role_type)
{
	switch (role_type)
	{
		case ROLE_TYPE_CONTROLLER_CENTRAL_STATIC:
			return "ROLE_TYPE_CONTROLLER_CENTRAL_STATIC";
			break;
		case ROLE_TYPE_CONTROLLER_SUB_STATIC:
			return "ROLE_TYPE_CONTROLLER_SUB_STATIC";
			break;
		case ROLE_TYPE_CONTROLLER_PORTABLE:
			return "ROLE_TYPE_CONTROLLER_PORTABLE";
			break;
		case ROLE_TYPE_CONTROLLER_PORTABLE_REPORTING:
			return "ROLE_TYPE_CONTROLLER_PORTABLE_REPORTING";
			break;
		case ROLE_TYPE_SLAVE_PORTABLE:
			return "ROLE_TYPE_SLAVE_PORTABLE";
			break;
		case ROLE_TYPE_SLAVE_ALWAYS_ON:
			return "ROLE_TYPE_SLAVE_ALWAYS_ON";
			break;
		case ROLE_TYPE_SLAVE_SLEEPING_REPORTING:
			return "ROLE_TYPE_SLAVE_SLEEPING_REPORTING";
			break;
		case ROLE_TYPE_SLAVE_SLEEPING_LISTENING:
			return "ROLE_TYPE_SLAVE_SLEEPING_LISTENING";
			break;
		default:
			return "ROLE_TYPE_XXX";
			break;
	}
}

char* translate_device_class(uint8_t basic)
{
	switch (basic)
	{
		case BASIC_TYPE_CONTROLLER:
			return "BASIC_TYPE_CONTROLLER";
			break;
		case BASIC_TYPE_ROUTING_SLAVE:
			return "BASIC_TYPE_ROUTING_SLAVE";
			break;
		case BASIC_TYPE_SLAVE:
			return "BASIC_TYPE_SLAVE";
			break;
		case BASIC_TYPE_STATIC_CONTROLLER:
			return "BASIC_TYPE_STATIC_CONTROLLER";
			break;
		default:
			return "BASIC_TYPE_XXX";
			break;
	}
}

char *translate_specific(uint8_t generic, uint8_t specific)
{
	if (specific == 0x00)
	{
		return "SPECIFIC_TYPE_NOT_USED";
	}

	switch (generic)
	{
		case GENERIC_TYPE_GENERIC_CONTROLLER: // 0x01
			switch (specific)
			{
				case SPECIFIC_TYPE_PORTABLE_REMOTE_CONTROLLER: // 0x01
					return "SPECIFIC_TYPE_PORTABLE_REMOTE_CONTROLLER";
					break;
				case SPECIFIC_TYPE_PORTABLE_SCENE_CONTROLLER: // 0x02
					return "SPECIFIC_TYPE_PORTABLE_SCENE_CONTROLLER";
					break;
				case SPECIFIC_TYPE_PORTABLE_INSTALLER_TOOL: // 0x03
					return "SPECIFIC_TYPE_PORTABLE_INSTALLER_TOOL";
					break;
				case SPECIFIC_TYPE_REMOTE_CONTROL_AV: // 0x04
					return "SPECIFIC_TYPE_REMOTE_CONTROL_AV";
					break;
				case SPECIFIC_TYPE_REMOTE_CONTROL_SIMPLE: // 0x06
					return "SPECIFIC_TYPE_REMOTE_CONTROL_SIMPLE";
					break;
			}
			break;
		case GENERIC_TYPE_STATIC_CONTROLLER: // 0x02
			switch (specific)
			{
				case SPECIFIC_TYPE_PC_CONTROLLER: // 0x01
					return "SPECIFIC_TYPE_PC_CONTROLLER";
					break;
				case SPECIFIC_TYPE_SCENE_CONTROLLER: // 0x02
					return "SPECIFIC_TYPE_SCENE_CONTROLLER";
					break;
				case SPECIFIC_TYPE_STATIC_INSTALLER_TOOL: // 0x03
					return "SPECIFIC_TYPE_STATIC_INSTALLER_TOOL";
					break;
				case SPECIFIC_TYPE_SET_TOP_BOX: // 0x04
					return "SPECIFIC_TYPE_SET_TOP_BOX";
					break;
				case SPECIFIC_TYPE_SUB_SYSTEM_CONTROLLER: // 0x05
					return "SPECIFIC_TYPE_SUB_SYSTEM_CONTROLLER";
					break;
				case SPECIFIC_TYPE_TV: // 0x06
					return "SPECIFIC_TYPE_TV";
					break;
				case SPECIFIC_TYPE_GATEWAY: // 0x07
					return "SPECIFIC_TYPE_GATEWAY";
					break;
			}
			break;
		case GENERIC_TYPE_AV_CONTROL_POINT: // 0x03
			switch (specific)
			{
				case SPECIFIC_TYPE_SATELLITE_RECEIVER: // 0x04
					return "SPECIFIC_TYPE_SATELLITE_RECEIVER";
					break;
				case SPECIFIC_TYPE_SATELLITE_RECEIVER_V2: // 0x11
					return "SPECIFIC_TYPE_SATELLITE_RECEIVER_V2";
					break;
				case SPECIFIC_TYPE_DOORBELL: // 0x12
					return "SPECIFIC_TYPE_DOORBELL";
					break;
			}
			break;
		case GENERIC_TYPE_DISPLAY: // 0x04
			switch (specific)
			{
				case SPECIFIC_TYPE_SIMPLE_DISPLAY: // 0x01
					return "SPECIFIC_TYPE_SIMPLE_DISPLAY";
					break;
			}
			break;
		case GENERIC_TYPE_NETWORK_EXTENDER: // 0x05
			switch (specific)
			{
				case SPECIFIC_TYPE_SECURE_EXTENDER: // 0x01
					return "SPECIFIC_TYPE_SECURE_EXTENDER";
					break;
			}
			break;
		case GENERIC_TYPE_APPLIANCE: // 0x06
			switch (specific)
			{
				case SPECIFIC_TYPE_GENERAL_APPLIANCE: // 0x01
					return "SPECIFIC_TYPE_GENERAL_APPLIANCE";
					break;
				case SPECIFIC_TYPE_KITCHEN_APPLIANCE: // 0x02
					return "SPECIFIC_TYPE_KITCHEN_APPLIANCE";
					break;
				case SPECIFIC_TYPE_LAUNDRY_APPLIANCE: // 0x03
					return "SPECIFIC_TYPE_LAUNDRY_APPLIANCE";
					break;
			}
			break;
		case GENERIC_TYPE_SENSOR_NOTIFICATION: // 0x07
			switch (specific)
			{
				case SPECIFIC_TYPE_NOTIFICATION_SENSOR: // 0x01
					return "SPECIFIC_TYPE_NOTIFICATION_SENSOR";
					break;
			}
			break;
		case GENERIC_TYPE_THERMOSTAT: // 0x08
			switch (specific)
			{
				case SPECIFIC_TYPE_THERMOSTAT_HEATING: // 0x01
					return "SPECIFIC_TYPE_THERMOSTAT_HEATING";
					break;
				case SPECIFIC_TYPE_THERMOSTAT_GENERAL: // 0x02
					return "SPECIFIC_TYPE_THERMOSTAT_GENERAL";
					break;
				case SPECIFIC_TYPE_SETBACK_SCHEDULE_THERMOSTAT: // 0x03
					return "SPECIFIC_TYPE_SETBACK_SCHEDULE_THERMOSTAT";
					break;
				case SPECIFIC_TYPE_SETPOINT_THERMOSTAT: // 0x04
					return "SPECIFIC_TYPE_SETPOINT_THERMOSTAT";
					break;
				case SPECIFIC_TYPE_SETBACK_THERMOSTAT: // 0x05
					return "SPECIFIC_TYPE_SETBACK_THERMOSTAT";
					break;
				case SPECIFIC_TYPE_THERMOSTAT_GENERAL_V2: // 0x06
					return "SPECIFIC_TYPE_THERMOSTAT_GENERAL_V2";
					break;
			}
			break;
		case GENERIC_TYPE_WINDOW_COVERING: // 0x09
			switch (specific)
			{
				case SPECIFIC_TYPE_SIMPLE_WINDOW_COVERING: // 0x01
					return "SPECIFIC_TYPE_SIMPLE_WINDOW_COVERING";
					break;
			}
			break;
		case GENERIC_TYPE_REPEATER_SLAVE: // 0x0F
			switch (specific)
			{
				case SPECIFIC_TYPE_REPEATER_SLAVE: // 0x01
					return "SPECIFIC_TYPE_REPEATER_SLAVE";
					break;
				case SPECIFIC_TYPE_VIRTUAL_NODE: // 0x02
					return "SPECIFIC_TYPE_VIRTUAL_NODE";
					break;
			}
			break;
		case GENERIC_TYPE_SWITCH_BINARY: // 0x10
			switch (specific)
			{
				case SPECIFIC_TYPE_POWER_SWITCH_BINARY: // 0x01
					return "SPECIFIC_TYPE_POWER_SWITCH_BINARY";
					break;
				case SPECIFIC_TYPE_COLOR_TUNABLE_BINARY: // 0x02
					return "SPECIFIC_TYPE_COLOR_TUNABLE_BINARY";
					break;
				case SPECIFIC_TYPE_SCENE_SWITCH_BINARY: // 0x03
					return "SPECIFIC_TYPE_SCENE_SWITCH_BINARY";
					break;
				case SPECIFIC_TYPE_POWER_STRIP: // 0x04
					return "SPECIFIC_TYPE_POWER_STRIP";
					break;
				case SPECIFIC_TYPE_SIREN: // 0x05
					return "SPECIFIC_TYPE_SIREN";
					break;
				case SPECIFIC_TYPE_VALVE_OPEN_CLOSE: // 0x06
					return "SPECIFIC_TYPE_VALVE_OPEN_CLOSE";
					break;
				case SPECIFIC_TYPE_IRRIGATION_CONTROLLER: // 0x07
					return "SPECIFIC_TYPE_IRRIGATION_CONTROLLER";
					break;
			}
			break;
		case GENERIC_TYPE_SWITCH_MULTILEVEL: // 0x11
			switch (specific)
			{
				case SPECIFIC_TYPE_POWER_SWITCH_MULTILEVEL: // 0x01
					return "SPECIFIC_TYPE_POWER_SWITCH_MULTILEVEL";
					break;
				case SPECIFIC_TYPE_COLOR_TUNABLE_MULTILEVEL: // 0x02
					return "SPECIFIC_TYPE_COLOR_TUNABLE_MULTILEVEL";
					break;
				case SPECIFIC_TYPE_MOTOR_MULTIPOSITION: // 0x03
					return "SPECIFIC_TYPE_MOTOR_MULTIPOSITION";
					break;
				case SPECIFIC_TYPE_SCENE_SWITCH_MULTILEVEL: // 0x04
					return "SPECIFIC_TYPE_SCENE_SWITCH_MULTILEVEL";
					break;
				case SPECIFIC_TYPE_CLASS_A_MOTOR_CONTROL: // 0x05
					return "SPECIFIC_TYPE_CLASS_A_MOTOR_CONTROL";
					break;
				case SPECIFIC_TYPE_CLASS_B_MOTOR_CONTROL: // 0x06
					return "SPECIFIC_TYPE_CLASS_B_MOTOR_CONTROL";
					break;
				case SPECIFIC_TYPE_CLASS_C_MOTOR_CONTROL: // 0x07
					return "SPECIFIC_TYPE_CLASS_C_MOTOR_CONTROL";
					break;
				case SPECIFIC_TYPE_FAN_SWITCH: // 0x08
					return "SPECIFIC_TYPE_FAN_SWITCH";
					break;
			}
			break;
		case GENERIC_TYPE_SWITCH_REMOTE: // 0x12
			switch (specific)
			{
				case SPECIFIC_TYPE_SWITCH_REMOTE_BINARY: // 0x01
					return "SPECIFIC_TYPE_SWITCH_REMOTE_BINARY";
					break;
				case SPECIFIC_TYPE_SWITCH_REMOTE_MULTILEVEL: // 0x02
					return "SPECIFIC_TYPE_SWITCH_REMOTE_MULTILEVEL";
					break;
				case SPECIFIC_TYPE_SWITCH_REMOTE_TOGGLE_BINARY: // 0x03
					return "SPECIFIC_TYPE_SWITCH_REMOTE_TOGGLE_BINARY";
					break;
				case SPECIFIC_TYPE_SWITCH_REMOTE_TOGGLE_MULTILEVEL: // 0x04
					return "SPECIFIC_TYPE_SWITCH_REMOTE_TOGGLE_MULTILEVEL";
					break;
			}
			break;
		case GENERIC_TYPE_SWITCH_TOGGLE: // 0x13
			switch (specific)
			{
				case SPECIFIC_TYPE_SWITCH_TOGGLE_BINARY: // 0x01
					return "SPECIFIC_TYPE_SWITCH_TOGGLE_BINARY";
					break;
				case SPECIFIC_TYPE_SWITCH_TOGGLE_MULTILEVEL: // 0x02
					return "SPECIFIC_TYPE_SWITCH_TOGGLE_MULTILEVEL";
					break;
			}
			break;
		case GENERIC_TYPE_ZIP_NODE: // 0x15
			switch (specific)
			{
				case SPECIFIC_TYPE_ZIP_TUN_NODE: // 0x01
					return "SPECIFIC_TYPE_ZIP_TUN_NODE";
					break;
				case SPECIFIC_TYPE_ZIP_ADV_NODE: // 0x02
					return "SPECIFIC_TYPE_ZIP_ADV_NODE";
					break;
			}
			break;
		case GENERIC_TYPE_VENTILATION: // 0x16
			switch (specific)
			{
				case SPECIFIC_TYPE_RESIDENTIAL_HRV: // 0x01
					return "SPECIFIC_TYPE_RESIDENTIAL_HRV";
					break;
			}
			break;
		case GENERIC_TYPE_SECURITY_PANEL: // 0x17
			switch (specific)
			{
				case SPECIFIC_TYPE_ZONED_SECURITY_PANEL: // 0x01
					return "SPECIFIC_TYPE_ZONED_SECURITY_PANEL";
					break;
			}
			break;
		case GENERIC_TYPE_WALL_CONTROLLER: // 0x18
			switch (specific)
			{
				case SPECIFIC_TYPE_BASIC_WALL_CONTROLLER: // 0x01
					return "SPECIFIC_TYPE_BASIC_WALL_CONTROLLER";
					break;
			}
			break;
		case GENERIC_TYPE_SENSOR_BINARY: // 0x20
			switch (specific)
			{
				case SPECIFIC_TYPE_ROUTING_SENSOR_BINARY: // 0x01
					return "SPECIFIC_TYPE_ROUTING_SENSOR_BINARY";
					break;
			}
			break;
		case GENERIC_TYPE_SENSOR_MULTILEVEL: // 0x21
			switch (specific)
			{
				case SPECIFIC_TYPE_ROUTING_SENSOR_MULTILEVEL: // 0x01
					return "SPECIFIC_TYPE_ROUTING_SENSOR_MULTILEVEL";
					break;
				case SPECIFIC_TYPE_CHIMNEY_FAN: // 0x02
					return "SPECIFIC_TYPE_CHIMNEY_FAN";
					break;
			}
			break;
		case GENERIC_TYPE_METER_PULSE: // 0x30
			break;
		case GENERIC_TYPE_METER: // 0x31
			switch (specific)
			{
				case SPECIFIC_TYPE_SIMPLE_METER: // 0x01
					return "SPECIFIC_TYPE_SIMPLE_METER";
					break;
				case SPECIFIC_TYPE_ADV_ENERGY_CONTROL: // 0x02
					return "SPECIFIC_TYPE_ADV_ENERGY_CONTROL";
					break;
				case SPECIFIC_TYPE_WHOLE_HOME_METER_SIMPLE: // 0x03
					return "SPECIFIC_TYPE_WHOLE_HOME_METER_SIMPLE";
					break;
			}
			break;
		case GENERIC_TYPE_ENTRY_CONTROL: // 0x40
			switch (specific)
			{
				case SPECIFIC_TYPE_DOOR_LOCK: // 0x01
					return "SPECIFIC_TYPE_DOOR_LOCK";
					break;
				case SPECIFIC_TYPE_ADVANCED_DOOR_LOCK: // 0x02
					return "SPECIFIC_TYPE_ADVANCED_DOOR_LOCK";
					break;
				case SPECIFIC_TYPE_SECURE_KEYPAD_DOOR_LOCK: // 0x03
					return "SPECIFIC_TYPE_SECURE_KEYPAD_DOOR_LOCK";
					break;
				case SPECIFIC_TYPE_SECURE_KEYPAD_DOOR_LOCK_DEADBOLT: // 0x04
					return "SPECIFIC_TYPE_SECURE_KEYPAD_DOOR_LOCK_DEADBOLT";
					break;
				case SPECIFIC_TYPE_SECURE_DOOR: // 0x05
					return "SPECIFIC_TYPE_SECURE_DOOR";
					break;
				case SPECIFIC_TYPE_SECURE_GATE: // 0x06
					return "SPECIFIC_TYPE_SECURE_GATE";
					break;
				case SPECIFIC_TYPE_SECURE_BARRIER_ADDON: // 0x07
					return "SPECIFIC_TYPE_SECURE_BARRIER_ADDON";
					break;
				case SPECIFIC_TYPE_SECURE_BARRIER_OPEN_ONLY: // 0x08
					return "SPECIFIC_TYPE_SECURE_BARRIER_OPEN_ONLY";
					break;
				case SPECIFIC_TYPE_SECURE_BARRIER_CLOSE_ONLY: // 0x09
					return "SPECIFIC_TYPE_SECURE_BARRIER_CLOSE_ONLY";
					break;
				case SPECIFIC_TYPE_SECURE_LOCKBOX: // 0x0A
					return "SPECIFIC_TYPE_SECURE_LOCKBOX";
					break;
				case SPECIFIC_TYPE_SECURE_KEYPAD: // 0x0B
					return "SPECIFIC_TYPE_SECURE_KEYPAD";
					break;
			}
			break;
		case GENERIC_TYPE_SEMI_INTEROPERABLE: // 0x50
			switch (specific)
			{
				case SPECIFIC_TYPE_ENERGY_PRODUCTION: // 0x01
					return "SPECIFIC_TYPE_ENERGY_PRODUCTION";
					break;
			}
			break;
		case GENERIC_TYPE_SENSOR_ALARM: // 0xA1
			switch (specific)
			{
				case SPECIFIC_TYPE_BASIC_ROUTING_ALARM_SENSOR: // 0x01
					return "SPECIFIC_TYPE_BASIC_ROUTING_ALARM_SENSOR";
					break;
				case SPECIFIC_TYPE_ROUTING_ALARM_SENSOR: // 0x02
					return "SPECIFIC_TYPE_ROUTING_ALARM_SENSOR";
					break;
				case SPECIFIC_TYPE_BASIC_ZENSOR_NET_ALARM_SENSOR: // 0x03
					return "SPECIFIC_TYPE_BASIC_ZENSOR_NET_ALARM_SENSOR";
					break;
				case SPECIFIC_TYPE_ZENSOR_NET_ALARM_SENSOR: // 0x04
					return "SPECIFIC_TYPE_ZENSOR_NET_ALARM_SENSOR";
					break;
				case SPECIFIC_TYPE_ADV_ZENSOR_NET_ALARM_SENSOR: // 0x05
					return "SPECIFIC_TYPE_ADV_ZENSOR_NET_ALARM_SENSOR";
					break;
				case SPECIFIC_TYPE_BASIC_ROUTING_SMOKE_SENSOR: // 0x06
					return "SPECIFIC_TYPE_BASIC_ROUTING_SMOKE_SENSOR";
					break;
				case SPECIFIC_TYPE_ROUTING_SMOKE_SENSOR: // 0x07
					return "SPECIFIC_TYPE_ROUTING_SMOKE_SENSOR";
					break;
				case SPECIFIC_TYPE_BASIC_ZENSOR_NET_SMOKE_SENSOR: // 0x08
					return "SPECIFIC_TYPE_BASIC_ZENSOR_NET_SMOKE_SENSOR";
					break;
				case SPECIFIC_TYPE_ZENSOR_NET_SMOKE_SENSOR: // 0x09
					return "SPECIFIC_TYPE_ZENSOR_NET_SMOKE_SENSOR";
					break;
				case SPECIFIC_TYPE_ADV_ZENSOR_NET_SMOKE_SENSOR: // 0x0A
					return "SPECIFIC_TYPE_ADV_ZENSOR_NET_SMOKE_SENSOR";
					break;
				case SPECIFIC_TYPE_ALARM_SENSOR: // 0x0B
					return "SPECIFIC_TYPE_ALARM_SENSOR";
					break;
			}
			break;
		case GENERIC_TYPE_NON_INTEROPERABLE: // 0xFF
			break;
		default:
			break;
	}
	return "XXX";
}

char *translate_generic(uint8_t generic)
{
	switch (generic)
	{
		case GENERIC_TYPE_GENERIC_CONTROLLER: // 0x01
			return "GENERIC_TYPE_GENERIC_CONTROLLER";
			break;
		case GENERIC_TYPE_STATIC_CONTROLLER: // 0x02
			return "GENERIC_TYPE_STATIC_CONTROLLER";
			break;
		case GENERIC_TYPE_AV_CONTROL_POINT: // 0x03
			return "GENERIC_TYPE_AV_CONTROL_POINT";
			break;
		case GENERIC_TYPE_DISPLAY: // 0x04
			return "GENERIC_TYPE_DISPLAY";
			break;
		case GENERIC_TYPE_NETWORK_EXTENDER: // 0x05
			return "GENERIC_TYPE_NETWORK_EXTENDER";
			break;
		case GENERIC_TYPE_APPLIANCE: // 0x06
			return "GENERIC_TYPE_APPLIANCE";
			break;
		case GENERIC_TYPE_SENSOR_NOTIFICATION: // 0x07
			return "GENERIC_TYPE_SENSOR_NOTIFICATION";
			break;
		case GENERIC_TYPE_THERMOSTAT: // 0x08
			return "GENERIC_TYPE_THERMOSTAT";
			break;
		case GENERIC_TYPE_WINDOW_COVERING: // 0x09
			return "GENERIC_TYPE_WINDOW_COVERING";
			break;
		case GENERIC_TYPE_REPEATER_SLAVE: // 0x0F
			return "GENERIC_TYPE_REPEATER_SLAVE";
			break;
		case GENERIC_TYPE_SWITCH_BINARY: // 0x10
			return "GENERIC_TYPE_SWITCH_BINARY";
			break;
		case GENERIC_TYPE_SWITCH_MULTILEVEL: // 0x11
			return "GENERIC_TYPE_SWITCH_MULTILEVEL";
			break;
		case GENERIC_TYPE_SWITCH_REMOTE: // 0x12
			return "GENERIC_TYPE_SWITCH_REMOTE";
			break;
		case GENERIC_TYPE_SWITCH_TOGGLE: // 0x13
			return "GENERIC_TYPE_SWITCH_TOGGLE";
			break;
		case GENERIC_TYPE_ZIP_NODE: // 0x15
			return "GENERIC_TYPE_ZIP_NODE";
			break;
		case GENERIC_TYPE_VENTILATION: // 0x16
			return "GENERIC_TYPE_VENTILATION";
			break;
		case GENERIC_TYPE_SECURITY_PANEL: // 0x17
			return "GENERIC_TYPE_SECURITY_PANEL";
			break;
		case GENERIC_TYPE_WALL_CONTROLLER: // 0x18
			return "GENERIC_TYPE_WALL_CONTROLLER";
			break;
		case GENERIC_TYPE_SENSOR_BINARY: // 0x20
			return "GENERIC_TYPE_SENSOR_BINARY";
			break;
		case GENERIC_TYPE_SENSOR_MULTILEVEL: // 0x21
			return "GENERIC_TYPE_SENSOR_MULTILEVEL";
			break;
		case GENERIC_TYPE_METER_PULSE: // 0x30
			return "GENERIC_TYPE_METER_PULSE";
			break;
		case GENERIC_TYPE_METER: // 0x31
			return "GENERIC_TYPE_METER";
			break;
		case GENERIC_TYPE_ENTRY_CONTROL: // 0x40
			return "GENERIC_TYPE_ENTRY_CONTROL";
			break;
		case GENERIC_TYPE_SEMI_INTEROPERABLE: // 0x50
			return "GENERIC_TYPE_SEMI_INTEROPERABLE";
			break;
		case GENERIC_TYPE_SENSOR_ALARM: // 0xA1
			return "GENERIC_TYPE_SENSOR_ALARM";
			break;
		case GENERIC_TYPE_NON_INTEROPERABLE: // 0xFF
			return "GENERIC_TYPE_NON_INTEROPERABLE";
			break;
		default:
			return "XXX";
			break;
	}
}

void translate_color_component_mask(uint16_t color_component_mask, char *mask_name)
{
	if (color_component_mask & (uint16_t)pow(2, COLOR_COMPONENT_WARM_WHITE))
	{
		strcat(mask_name, "Warm White/");
	}
	if (color_component_mask & (uint16_t)pow(2, COLOR_COMPONENT_COLD_WHITE))
	{
		strcat(mask_name, "Cold White/");
	}
	if (color_component_mask & (uint16_t)pow(2, COLOR_COMPONENT_RED))
	{
		strcat(mask_name, "Red/");
	}
	if (color_component_mask & (uint16_t)pow(2, COLOR_COMPONENT_GREEN))
	{
		strcat(mask_name, "Green/");
	}
	if (color_component_mask & (uint16_t)pow(2, COLOR_COMPONENT_BLUE))
	{
		strcat(mask_name, "Blue/");
	}
	if (color_component_mask & (uint16_t)pow(2, COLOR_COMPONENT_AMBER))
	{
		strcat(mask_name, "Amber/");
	}
	if (color_component_mask & (uint16_t)pow(2, COLOR_COMPONENT_CYAN))
	{
		strcat(mask_name, "Cyan/");
	}
	if (color_component_mask & (uint16_t)pow(2, COLOR_COMPONENT_PURPLE))
	{
		strcat(mask_name, "Purple/");
	}
	if (color_component_mask & (uint16_t)pow(2, COLOR_COMPONENT_INDEXED_COLOR))
	{
		strcat(mask_name, "Indexed Color/");
	}

	if (mask_name[strlen(mask_name)-1] =='/')
	{
		mask_name[strlen(mask_name)-1] = '\0';
	}
}

char *translate_color_name(uint16_t id)
{
	switch (id)
	{
		case COLOR_COMPONENT_WARM_WHITE:
			return "Warm White";
			break;
		case COLOR_COMPONENT_COLD_WHITE:
			return "Cold White";
			break;
		case COLOR_COMPONENT_RED:
			return "Red";
			break;
		case COLOR_COMPONENT_GREEN:
			return "Green";
			break;
		case COLOR_COMPONENT_BLUE:
			return "Blue";
			break;
		case COLOR_COMPONENT_AMBER:
			return "Amber";
			break;
		case COLOR_COMPONENT_CYAN:
			return "Cyan";
			break;
		case COLOR_COMPONENT_PURPLE:
			return "Purple";
			break;
		case COLOR_COMPONENT_INDEXED_COLOR:
			return "Indexed Color";
			break;
		default:
			return "XXX";
			break;
	}
}

void translate_propty(uint8_t propty, char *propty_name)
{
	if (propty & IF_PROPTY_SECURE)
	{
		strcat(propty_name, "IF_PROPTY_SECURE/");
	}
	if (propty & IF_PROPTY_UNSECURE)
	{
		strcat(propty_name, "IF_PROPTY_UNSECURE/");
	}
	if (propty & IF_PROPTY_HIDDEN)
	{
		strcat(propty_name, "IF_PROPTY_HIDDEN/");
	}

	if (propty_name[strlen(propty_name)-1] =='/')
	{
		propty_name[strlen(propty_name)-1] = '\0';
	}
}

void translate_ctl_role(uint8_t ctl_role, char *ctl_role_name)
{
	if (ctl_role & ZWNET_CTLR_ROLE_PROXY)
	{
		strcat(ctl_role_name, "ZWNET_CTLR_ROLE_PROXY/");
	}
	if (ctl_role & ZWNET_CTLR_ROLE_INCL)
	{
		strcat(ctl_role_name, "ZWNET_CTLR_ROLE_INCL/");
	}
	if (ctl_role & ZWNET_CTLR_ROLE_PRI)
	{
		strcat(ctl_role_name, "ZWNET_CTLR_ROLE_PRI/");
	}

	if (ctl_role_name[strlen(ctl_role_name)-1] =='/')
	{
		ctl_role_name[strlen(ctl_role_name)-1] = '\0';
	}
}

char* translate_op_add_node(uint8_t add_node)
{
	switch (add_node)
	{
		case OP_DONE:
			return "OP_DONE";
			break;
		case OP_ADD_NODE_PROTOCOL_DONE:
			return "OP_ADD_NODE_PROTOCOL_DONE";
			break;
		case OP_ADD_NODE_PROTOCOL_START:
			return "OP_ADD_NODE_PROTOCOL_START";
			break;
		case OP_ADD_NODE_GET_NODE_INFO:
			return "OP_ADD_NODE_GET_NODE_INFO";
			break;
		case OP_FAILED:
			return "OP_FAILED";
			break;
#ifdef DEVICE_INTERVIEW_NOTIFY
		case OP_ADD_NODE_ZWNET_STA_GET_GW_DSK:
			return "OP_ADD_NODE_ZWNET_STA_GET_GW_DSK";
			break;
		case OP_ADD_NODE_ZWNET_STA_GET_NODE_INFO:
			return "OP_ADD_NODE_ZWNET_STA_GET_NODE_INFO";
			break;
		case OP_ADD_NODE_ZWNET_STA_ASSIGN_RR:
			return "OP_ADD_NODE_ZWNET_STA_ASSIGN_RR";
			break;
		case OP_ADD_NODE_ZWNET_STA_GET_VER_CC:
			return "OP_ADD_NODE_ZWNET_STA_GET_VER_CC";
			break;
		case OP_ADD_NODE_ZWNET_STA_GET_ZWPLUS_INFO:
			return "OP_ADD_NODE_ZWNET_STA_GET_ZWPLUS_INFO";
			break;
		case OP_ADD_NODE_ZWNET_STA_GET_VER:
			return "OP_ADD_NODE_ZWNET_STA_GET_VER";
			break;
		case OP_ADD_NODE_ZWNET_STA_GET_MANF_PDT_ID:
			return "OP_ADD_NODE_ZWNET_STA_GET_MANF_PDT_ID";
			break;
		case OP_ADD_NODE_ZWNET_STA_WKUP_CFG:
			return "OP_ADD_NODE_ZWNET_STA_WKUP_CFG";
			break;
		case OP_ADD_NODE_ZWNET_STA_MULTI_INSTANCE:
			return "OP_ADD_NODE_ZWNET_STA_MULTI_INSTANCE";
			break;
		case OP_ADD_NODE_ZWNET_STA_MULTI_CHANNEL:
			return "OP_ADD_NODE_ZWNET_STA_MULTI_CHANNEL";
			break;

		case OP_ADD_NODE_ACT_END_POINT:
			return "OP_ADD_NODE_ACT_END_POINT";
			break;
		case OP_ADD_NODE_ZWNET_STA_GET_EP_INFO:
			return "OP_ADD_NODE_ZWNET_STA_GET_EP_INFO";
			break;

		case OP_ADD_NODE_S_EP_STA_GET_ZWPLUS_INFO:
			return "OP_ADD_NODE_S_EP_STA_GET_ZWPLUS_INFO";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_MAX_GROUP:
			return "OP_ADD_NODE_S_EP_STA_GET_MAX_GROUP";
			break;
		case OP_ADD_NODE_S_EP_STA_SET_GROUP_LIFELINE:
			return "OP_ADD_NODE_S_EP_STA_SET_GROUP_LIFELINE";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_GROUP_MEMBER:
			return "OP_ADD_NODE_S_EP_STA_GET_GROUP_MEMBER";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_GROUP_INFO:
			return "OP_ADD_NODE_S_EP_STA_GET_GROUP_INFO";
			break;
		case OP_ADD_NODE_S_EP_STA_SET_CFG_PARAM:
			return "OP_ADD_NODE_S_EP_STA_SET_CFG_PARAM";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_SENSOR:
			return "OP_ADD_NODE_S_EP_STA_GET_SENSOR";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_BIN_SENSOR:
			return "OP_ADD_NODE_S_EP_STA_GET_BIN_SENSOR";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_ALARM:
			return "OP_ADD_NODE_S_EP_STA_GET_ALARM";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_USERCODE:
			return "OP_ADD_NODE_S_EP_STA_GET_USERCODE";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_METER:
			return "OP_ADD_NODE_S_EP_STA_GET_METER";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_METER_DESC:
			return "OP_ADD_NODE_S_EP_STA_GET_METER_DESC";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_DOORLOCK:
			return "OP_ADD_NODE_S_EP_STA_GET_DOORLOCK";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_THRMO_FAN_MD:
			return "OP_ADD_NODE_S_EP_STA_GET_THRMO_FAN_MD";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_THRMO_MD:
			return "OP_ADD_NODE_S_EP_STA_GET_THRMO_MD";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_THRMO_SETP:
			return "OP_ADD_NODE_S_EP_STA_GET_THRMO_SETP";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_MUL_SWITCH:
			return "OP_ADD_NODE_S_EP_STA_GET_MUL_SWITCH";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_CSC:
			return "OP_ADD_NODE_S_EP_STA_GET_CSC";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_SWITCH:
			return "OP_ADD_NODE_S_EP_STA_GET_SWITCH";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_THRMO_OP:
			return "OP_ADD_NODE_S_EP_STA_GET_THRMO_OP";
			break;
		case OP_ADD_NODE_S_EP_STA_GET_BATTERY:
			return "OP_ADD_NODE_S_EP_STA_GET_BATTERY";
			break;
#endif
		default:
			return "OP_ADD_XXX";
			break;
	}
}

char* translate_op_rp_node(uint8_t rp_node)
{
	switch (rp_node)
	{
		case OP_DONE:
			return "OP_DONE";
			break;
		case OP_RP_NODE_PROTOCOL_DONE:
			return "OP_RP_NODE_PROTOCOL_DONE";
			break;
		case OP_RP_NODE_GET_NODE_INFO:
			return "OP_RP_NODE_GET_NODE_INFO";
			break;
		case OP_FAILED:
			return "OP_FAILED";
			break;
		default:
			return "OP_RP_XXX";
			break;
	}
}

char* translate_op_initiate(uint8_t initiate)
{
	switch (initiate)
	{
		case OP_DONE:
			return "OP_DONE";
			break;
		case OP_INI_PROTOCOL_DONE:
			return "OP_INI_PROTOCOL_DONE";
			break;
		case OP_INI_GET_NODE_INFO:
			return "OP_INI_GET_NODE_INFO";
			break;
		case OP_FAILED:
			return "OP_FAILED";
			break;
		default:
			return "OP_INI_XXX";
			break;
	}
}

char* translate_op_update(uint8_t update)
{
	switch (update)
	{
		case OP_DONE:
			return "OP_DONE";
			break;
		case OP_NU_TOPOLOGY:
			return "OP_NU_TOPOLOGY";
			break;
		case OP_NU_NEIGHBOR:
			return "OP_NU_NEIGHBOR";
			break;
		case OP_NU_GET_NODE_INFO:
			return "OP_NU_GET_NODE_INFO";
			break;
		case OP_FAILED:
			return "OP_FAILED";
			break;
		default:
			return "OP_NU_XXX";
			break;
	}
}

char* translate_op_fw(uint8_t fw)
{
	switch (fw)
	{
		case OP_FW_UPLOAD_STARTED:
			return "OP_FW_UPLOAD_STARTED";
			break;
		case OP_FW_UPLOADING:
			return "OP_FW_UPLOADING";
			break;
		case OP_FAILED:
			return "OP_FAILED";
			break;
		default:
			return "OP_FW_XXX";
			break;
	}
}

char* translate_op_health(uint8_t health)
{
	switch (health)
	{
		case OP_HEALTH_CHK_STARTED:
			return "OP_HEALTH_CHK_STARTED";
			break;
		case OP_HEALTH_CHK_PROGRESS:
			return "OP_HEALTH_CHK_PROGRESS";
			break;
		case OP_HEALTH_CHK_CMPLT:
			return "OP_HEALTH_CHK_CMPLT";
			break;
		case OP_FAILED:
			return "OP_FAILED";
			break;
		default:
			return "OP_HEALTH_CHK_XXX";
			break;
	}
}

#if (0)
char* translate_zw_fw_upgd(uint8_t upgrade_flg)
{
	switch (upgrade_flg)
	{
		case ZW_FW_UPGD_NO:
			return "ZW_FW_UPGD_NO";
			break;
		case ZW_FW_UPGD_YES:
			return "ZW_FW_UPGD_YES";
			break;
		default:
			return "ZW_FW_UPGD_XXX";
			break;
	}
}

char* translate_zw_fw_updt_err(uint8_t status)
{
	switch (status)
	{
		case ZW_FW_UPDT_ERR_INVALID:
			return "ZW_FW_UPDT_ERR_INVALID";
			break;
		case ZW_FW_UPDT_ERR_AUTHEN:
			return "ZW_FW_UPDT_ERR_AUTHEN";
			break;
		case ZW_FW_UPDT_ERR_FRAG_SZ:
			return "ZW_FW_UPDT_ERR_FRAG_SZ";
			break;
		case ZW_FW_UPDT_ERR_UPGRD:
			return "ZW_FW_UPDT_ERR_UPGRD";
			break;
		case ZW_FW_UPDT_ERR_HW_VER:
			return "ZW_FW_UPDT_ERR_HW_VER";
			break;
		case ZW_FW_UPDT_ERR_VALID:
			return "ZW_FW_UPDT_ERR_VALID";
			break;
		default:
			return "ZW_FW_UPDT_ERR_XXX";
			break;
	}
}

char* translate_zw_fw_updt_cmplt(uint8_t status)
{
	switch (status)
	{
		case ZW_FW_UPDT_CMPLT_ERR_CHKS:
			return "ZW_FW_UPDT_CMPLT_ERR_CHKS";
			break;
		case ZW_FW_UPDT_CMPLT_ERR_DOWNLOAD:
			return "ZW_FW_UPDT_CMPLT_ERR_DOWNLOAD";
			break;
		case ZW_FW_UPDT_CMPLT_ERR_VID:
			return "ZW_FW_UPDT_CMPLT_ERR_VID";
			break;
		case ZW_FW_UPDT_CMPLT_ERR_FW_ID:
			return "ZW_FW_UPDT_CMPLT_ERR_FW_ID";
			break;
		case ZW_FW_UPDT_CMPLT_ERR_FW_TGT:
			return "ZW_FW_UPDT_CMPLT_ERR_FW_TGT";
			break;
		case ZW_FW_UPDT_CMPLT_ERR_FILE_HDR:
			return "ZW_FW_UPDT_CMPLT_ERR_FILE_HDR";
			break;
		case ZW_FW_UPDT_CMPLT_ERR_FILE_H_FMT:
			return "ZW_FW_UPDT_CMPLT_ERR_FILE_H_FMT";
			break;
		case ZW_FW_UPDT_CMPLT_ERR_MEM:
			return "ZW_FW_UPDT_CMPLT_ERR_MEM";
			break;
		case ZW_FW_UPDT_CMPLT_ERR_HW_VER:
			return "ZW_FW_UPDT_CMPLT_ERR_HW_VER";
			break;
		case ZW_FW_UPDT_CMPLT_OK_WAIT:
			return "ZW_FW_UPDT_CMPLT_OK_WAIT";
			break;
		case ZW_FW_UPDT_CMPLT_OK_NO_RESTART:
			return "ZW_FW_UPDT_CMPLT_OK_NO_RESTART";
			break;
		case ZW_FW_UPDT_CMPLT_OK_RESTART:
			return "ZW_FW_UPDT_CMPLT_OK_RESTART";
			break;
		default:
			return "ZW_FW_UPDT_CMPLT_XXX";
			break;
	}
}

char* translate_zw_fw_updt_restart(uint8_t status)
{
	switch (status)
	{
		case ZW_FW_UPDT_RESTART_OK:
			return "ZW_FW_UPDT_RESTART_OK";
			break;
		case ZW_FW_UPDT_RESTART_FAILED:
			return "ZW_FW_UPDT_RESTART_FAILED";
			break;
		default:
			return "ZW_FW_UPDT_RESTART_XXX";
			break;
	}
}
#endif

char* translate_zw_err(int result)
{
	switch (result)
	{
		case ZW_ERR_CACHE_AVAIL:
			return "ZW_ERR_CACHE_AVAIL";
			break;
		case ZW_ERR_QUEUED:
			return "ZW_ERR_QUEUED";
			break;
		case ZW_ERR_NONE:
			return "ZW_ERR_NONE";
			break;
		case ZW_ERR_FAILED:
			return "ZW_ERR_FAILED";
			break;
		case ZW_ERR_WRONG_IF:
			return "ZW_ERR_WRONG_IF";
			break;
		case ZW_ERR_NO_RESP:
			return "ZW_ERR_NO_RESP";
			break;
		case ZW_ERR_MEMORY:
			return "ZW_ERR_MEMORY";
			break;
		case ZW_ERR_NODE_NOT_FOUND:
			return "ZW_ERR_NODE_NOT_FOUND";
			break;
		case ZW_ERR_CLASS_NOT_FOUND:
			return "ZW_ERR_CLASS_NOT_FOUND";
			break;
		case ZW_ERR_INTF_NOT_FOUND:
			return "ZW_ERR_INTF_NOT_FOUND";
			break;
		case ZW_ERR_INTF_NO_REP_HDLR:
			return "ZW_ERR_INTF_NO_REP_HDLR";
			break;
		case ZW_ERR_LAST_OP_NOT_DONE:
			return "ZW_ERR_LAST_OP_NOT_DONE";
			break;
		case ZW_ERR_NOT_IN_LIST:
			return "ZW_ERR_NOT_IN_LIST";
			break;
		case ZW_ERR_OP_FAILED:
			return "ZW_ERR_OP_FAILED";
			break;
		case ZW_ERR_EP_NOT_FOUND:
			return "ZW_ERR_EP_NOT_FOUND";
			break;
		case ZW_ERR_RPT_NOT_FOUND:
			return "ZW_ERR_RPT_NOT_FOUND";
			break;
		case ZW_ERR_NET_NOT_FOUND:
			return "ZW_ERR_NET_NOT_FOUND";
			break;
		case ZW_ERR_CMD_VERSION:
			return "ZW_ERR_CMD_VERSION";
			break;
		case ZW_ERR_PENDING:
			return "ZW_ERR_PENDING";
			break;
		case ZW_ERR_VALUE:
			return "ZW_ERR_VALUE";
			break;
		case ZW_ERR_QUEUE_FULL:
			return "ZW_ERR_QUEUE_FULL";
			break;
		case ZW_ERR_UNSUPPORTED:
			return "ZW_ERR_UNSUPPORTED";
			break;
		case ZW_ERR_FILE_OPEN:
			return "ZW_ERR_FILE_OPEN";
			break;
		case ZW_ERR_FILE_WRITE:
			return "ZW_ERR_FILE_WRITE";
			break;
		case ZW_ERR_FILE_EOF:
			return "ZW_ERR_FILE_EOF";
			break;
		case ZW_ERR_FILE:
			return "ZW_ERR_FILE";
			break;
		case ZW_ERR_FILE_HOME_ID:
			return "ZW_ERR_FILE_HOME_ID";
			break;
		case ZW_ERR_EXPIRED:
			return "ZW_ERR_EXPIRED";
			break;
		case ZW_ERR_NO_RES:
			return "ZW_ERR_NO_RES";
			break;
		case ZW_ERR_EVENT:
			return "ZW_ERR_EVENT";
			break;
		case ZW_ERR_TOO_LARGE:
			return "ZW_ERR_TOO_LARGE";
			break;
		case ZW_ERR_TOO_SMALL:
			return "ZW_ERR_TOO_SMALL";
			break;
		case ZW_ERR_TIMEOUT:
			return "ZW_ERR_TIMEOUT";
			break;
		case ZW_ERR_TRANSMIT:
			return "ZW_ERR_TRANSMIT";
			break;
		case ZW_ERR_NONCE_NOT_FOUND:
			return "ZW_ERR_NONCE_NOT_FOUND";
			break;
		case ZW_ERR_AUTH:
			return "ZW_ERR_AUTH";
			break;
		case ZW_ERR_SEQ_NUMBER:
			return "ZW_ERR_SEQ_NUMBER";
			break;
		case ZW_ERR_BUSY:
			return "ZW_ERR_BUSY";
			break;
		case ZW_ERR_SEC_SCHEME:
			return "ZW_ERR_SEC_SCHEME";
			break;
		case ZW_ERR_TRANSPORT_INI:
			return "ZW_ERR_TRANSPORT_INI";
			break;
		case ZW_ERR_FRAME_INI:
			return "ZW_ERR_FRAME_INI";
			break;
		case ZW_ERR_SESSION_INI:
			return "ZW_ERR_SESSION_INI";
			break;
		case ZW_ERR_APPL_INI:
			return "ZW_ERR_APPL_INI";
			break;
		case ZW_ERR_UNEXPECTED:
			return "ZW_ERR_UNEXPECTED";
			break;
		case ZW_ERR_NETWORK_IF:
			return "ZW_ERR_NETWORK_IF";
			break;
		case ZW_ERR_IP_ADDR:
			return "ZW_ERR_IP_ADDR";
			break;
		case ZW_ERR_VERSION:
			return "ZW_ERR_VERSION";
			break;
		case ZW_ERR_INTF_NO_DATA:
			return "ZW_ERR_INTF_NO_DATA";
			break;
		case ZW_ERR_FILE_READ:
			return "ZW_ERR_FILE_READ";
			break;
		case ZW_ERR_PARSE_FILE:
			return "ZW_ERR_PARSE_FILE";
			break;
		case ZW_ERR_MISSING_ENTRY:
			return "ZW_ERR_MISSING_ENTRY";
			break;
		case ZW_ERR_DEVCFG_NOT_FOUND:
			return "ZW_ERR_DEVCFG_NOT_FOUND";
			break;
		case ZW_ERR_DISALLOWED:
			return "ZW_ERR_DISALLOWED";
			break;
		case ZW_ERR_PSK_TOO_SHORT:
			return "ZW_ERR_PSK_TOO_SHORT";
			break;
		case ZW_ERR_NO_CACHE_AVAIL:
			return "ZW_ERR_NO_CACHE_AVAIL";
			break;
		case ZW_ERR_NOT_APPLICABLE:
			return "ZW_ERR_NOT_APPLICABLE";
			break;
		case ZW_ERR_SHUTDOWN:
			return "ZW_ERR_SHUTDOWN";
			break;
		default:
			return "ZW_ERR_XXX";
			break;
	}
}

char* translate_node_sts(uint8_t sts)
{
	switch (sts)
	{
		case ZWNET_NODE_STS_DOWN:
			return "ZWNET_NODE_STS_DOWN";
			break;
		case ZWNET_NODE_STS_UP:
			return "ZWNET_NODE_STS_UP";
			break;
#ifdef ZWNET_NODE_STS_SLEEP
		case ZWNET_NODE_STS_SLEEP:
			return "ZWNET_NODE_STS_SLEEP";
			break;
#endif
		default:
			return "ZWNET_NODE_STS_XXX";
			break;
	}
}

char* translate_node_category(uint8_t category)
{
	switch (category)
	{
		// 0
		case DEV_CATEGORY_UNKNOWN:
			return "DEV_CATEGORY_UNKNOWN";
			break;
		// 1
		case DEV_SENSOR_ALARM:
			return "DEV_SENSOR_ALARM";
			break;
		// 2
		case DEV_ON_OFF_SWITCH:
			return "DEV_ON_OFF_SWITCH";
			break;
		// 3
		case DEV_POWER_STRIP:
			return "DEV_POWER_STRIP";
			break;
		// 4
		case DEV_SIREN:
			return "DEV_SIREN";
			break;
		// 5
		case DEV_VALVE:
			return "DEV_VALVE";
			break;
		// 6
		case DEV_SIMPLE_DISPLAY:
			return "DEV_SIMPLE_DISPLAY";
			break;
		// 7
		case DEV_DOORLOCK_KEYPAD:
			return "DEV_DOORLOCK_KEYPAD";
			break;
		// 8
		case DEV_SUB_ENERGY_METER:
			return "DEV_SUB_ENERGY_METER";
			break;
		// 9
		case DEV_ADV_WHL_HOME_ENER_METER:
			return "DEV_ADV_WHL_HOME_ENER_METER";
			break;
		// 10
		case DEV_SIM_WHL_HOME_ENER_METER:
			return "DEV_SIM_WHL_HOME_ENER_METER";
			break;
		// 11
		case DEV_SENSOR:
			return "DEV_SENSOR";
			break;
		// 12
		case DEV_LIGHT_DIMMER:
			return "DEV_LIGHT_DIMMER";
			break;
		// 13
		case DEV_WIN_COVERING_NO_POS:
			return "DEV_WIN_COVERING_NO_POS";
			break;
		// 14
		case DEV_WIN_COVERING_EP:
			return "DEV_WIN_COVERING_EP";
			break;
		// 15
		case DEV_WIN_COVERING_POS_EP:
			return "DEV_WIN_COVERING_POS_EP";
			break;
		// 16
		case DEV_FAN_SWITCH:
			return "DEV_FAN_SWITCH";
			break;
		// 17
		case DEV_RMT_CTL_MULTIPURPOSE:
			return "DEV_RMT_CTL_MULTIPURPOSE";
			break;
		// 18
		case DEV_RMT_CTL_AV:
			return "DEV_RMT_CTL_AV";
			break;
		// 19
		case DEV_RMT_CTL_SIMPLE:
			return "DEV_RMT_CTL_SIMPLE";
			break;
		// 20
		case DEV_UNRECOG_GATEWAY:
			return "DEV_UNRECOG_GATEWAY";
			break;
		// 21
		case DEV_CENTRAL_CTLR:
			return "DEV_CENTRAL_CTLR";
			break;
		// 22
		case DEV_SET_TOP_BOX:
			return "DEV_SET_TOP_BOX";
			break;
		// 23
		case DEV_TV:
			return "DEV_TV";
			break;
		// 24
		case DEV_SUB_SYS_CTLR:
			return "DEV_SUB_SYS_CTLR";
			break;
		// 25
		case DEV_GATEWAY:
			return "DEV_GATEWAY";
			break;
		// 26
		case DEV_THERMOSTAT_HVAC:
			return "DEV_THERMOSTAT_HVAC";
			break;
		// 27
		case DEV_THERMOSTAT_SETBACK:
			return "DEV_THERMOSTAT_SETBACK";
			break;
		// 28
		case DEV_WALL_CTLR:
			return "DEV_WALL_CTLR";
			break;
		default:
			return "DEV_XXX";
			break;
	}
}

char* translate_nw_op(uint8_t op)
{
	switch (op)
	{
		// 0x00
		case ZWNET_OP_NONE:
			return "ZWNET_OP_NONE";
			break;
		// 0x01
		case ZWNET_OP_INITIALIZE:
			return "ZWNET_OP_INITIALIZE";
			break;
		// 0x02
		case ZWNET_OP_ADD_NODE:
			return "ZWNET_OP_ADD_NODE";
			break;
		// 0x03
		case ZWNET_OP_RM_NODE:
			return "ZWNET_OP_RM_NODE";
			break;
		// 0x04
		case ZWNET_OP_RP_NODE:
			return "ZWNET_OP_RP_NODE";
			break;
		// 0x05
		case ZWNET_OP_RM_FAILED_ID:
			return "ZWNET_OP_RM_FAILED_ID";
			break;
		// 0x06
		case ZWNET_OP_INITIATE:
			return "ZWNET_OP_INITIATE";
			break;
		// 0x07
		case ZWNET_OP_UPDATE:
			return "ZWNET_OP_UPDATE";
			break;
		// 0x08
		case ZWNET_OP_RESET:
			return "ZWNET_OP_RESET";
			break;
		// 0x09
		case ZWNET_OP_MIGRATE_SUC:
			return "ZWNET_OP_MIGRATE_SUC";
			break;
		// 0x0A
		case ZWNET_OP_MIGRATE:
			return "ZWNET_OP_MIGRATE";
			break;
		// 0x0B
		case ZWNET_OP_ASSIGN:
			return "ZWNET_OP_ASSIGN";
			break;
		// 0x0D
		case ZWNET_OP_NODE_UPDATE:
			return "ZWNET_OP_NODE_UPDATE";
			break;
		// 0x0E
		case ZWNET_OP_SEND_NIF:
			return "ZWNET_OP_SEND_NIF";
			break;
		// 0x0F
		case ZWNET_OP_NW_CHANGED:
			return "ZWNET_OP_NW_CHANGED";
			break;
		// 0x10
		case ZWNET_OP_NODE_CACHE_UPT:
			return "ZWNET_OP_NODE_CACHE_UPT";
			break;
		// 0x11
		case ZWNET_OP_SAVE_NW:
			return "ZWNET_OP_SAVE_NW";
			break;
		// 0x12
		case ZWNET_OP_SLEEP_NODE_UPT:
			return "ZWNET_OP_SLEEP_NODE_UPT";
			break;
		// 0x13
		case ZWNET_OP_FW_UPDT:
			return "ZWNET_OP_FW_UPDT";
			break;
		// 0x14
		case ZWNET_OP_HEALTH_CHK:
			return "ZWNET_OP_HEALTH_CHK";
			break;

		default:
			return "ZWNET_OP_XXX";
			break;
	}
}

char* translate_alarm_type(uint8_t type)
{
	switch (type)
	{
		case ZW_ALRM_SMOKE:
			return "ZW_ALRM_SMOKE";
			break;
		case ZW_ALRM_CO:
			return "ZW_ALRM_CO";
			break;
		case ZW_ALRM_CO2:
			return "ZW_ALRM_CO2";
			break;
		case ZW_ALRM_HEAT:
			return "ZW_ALRM_HEAT";
			break;
		case ZW_ALRM_WATER:
			return "ZW_ALRM_WATER";
			break;
		case ZW_ALRM_LOCK:
			return "ZW_ALRM_LOCK";
			break;
		case ZW_ALRM_BURGLAR:
			return "ZW_ALRM_BURGLAR";
			break;
		case ZW_ALRM_POWER:
			return "ZW_ALRM_POWER";
			break;
		case ZW_ALRM_SYSTEM:
			return "ZW_ALRM_SYSTEM";
			break;
		case ZW_ALRM_EMERGENCY:
			return "ZW_ALRM_EMERGENCY";
			break;
		case ZW_ALRM_CLOCK:
			return "ZW_ALRM_CLOCK";
			break;
		case ZW_ALRM_FIRST:
			return "ZW_ALRM_FIRST";
			break;
		default:
			return "ZW_ALRM_XXX";
			break;
	}
}

char* translate_alarm_event(uint8_t type, uint8_t event, uint8_t prefix)
{
	switch (type)
	{
		case ZW_ALRM_SMOKE:
			switch (event)
			{
				case ZW_ALRM_EVT_INACTIVE_CLEAR:
					if (prefix)
					{
						return "ZW_ALRM_SMOKE/ZW_ALRM_EVT_INACTIVE_CLEAR";
					}
					return "ZW_ALRM_EVT_INACTIVE_CLEAR";
					break;
				case ZW_ALRM_EVT_SMOKE_L:
					if (prefix)
					{
						return "ZW_ALRM_SMOKE/ZW_ALRM_EVT_SMOKE_L";
					}
					return "ZW_ALRM_EVT_SMOKE_L";
					break;
				case ZW_ALRM_EVT_SMOKE:
					if (prefix)
					{
						return "ZW_ALRM_SMOKE/ZW_ALRM_EVT_SMOKE";
					}
					return "ZW_ALRM_EVT_SMOKE";
					break;
				case ZW_ALRM_EVT_SMOKE_TEST:
					if (prefix)
					{
						return "ZW_ALRM_SMOKE/ZW_ALRM_EVT_SMOKE_TEST";
					}
					return "ZW_ALRM_EVT_SMOKE_TEST";
					break;
				case ZW_ALRM_EVT_SMOKE_REPLA:
					if (prefix)
					{
						return "ZW_ALRM_SMOKE/ZW_ALRM_EVT_SMOKE_REPLA";
					}
					return "ZW_ALRM_EVT_SMOKE_REPLA";
					break;
				default:
					if (prefix)
					{
						return "ZW_ALRM_SMOKE/ZW_ALRM_EVT_UNKNOWN";
					}
					return "ZW_ALRM_EVT_UNKNOWN";
					break;
			}
			break;
		case ZW_ALRM_CO:
			switch (event)
			{
				case ZW_ALRM_EVT_INACTIVE_CLEAR:
					if (prefix)
					{
						return "ZW_ALRM_CO/ZW_ALRM_EVT_INACTIVE_CLEAR";
					}
					return "ZW_ALRM_EVT_INACTIVE_CLEAR";
					break;
				case ZW_ALRM_EVT_CO_L:
					if (prefix)
					{
						return "ZW_ALRM_CO/ZW_ALRM_EVT_CO_L";
					}
					return "ZW_ALRM_EVT_CO_L";
					break;
				case ZW_ALRM_EVT_CO:
					if (prefix)
					{
						return "ZW_ALRM_CO/ZW_ALRM_EVT_CO";
					}
					return "ZW_ALRM_EVT_CO";
					break;
				case ZW_ALRM_EVT_CO_TEST:
					if (prefix)
					{
						return "ZW_ALRM_CO/ZW_ALRM_EVT_CO_TEST";
					}
					return "ZW_ALRM_EVT_CO_TEST";
					break;
				case ZW_ALRM_EVT_CO_REPLA:
					if (prefix)
					{
						return "ZW_ALRM_CO/ZW_ALRM_EVT_CO_REPLA";
					}
					return "ZW_ALRM_EVT_CO_REPLA";
					break;
				default:
					if (prefix)
					{
						return "ZW_ALRM_CO/ZW_ALRM_EVT_UNKNOWN";
					}
					return "ZW_ALRM_EVT_UNKNOWN";
					break;
			}
			break;
		case ZW_ALRM_CO2:
			switch (event)
			{
				case ZW_ALRM_EVT_INACTIVE_CLEAR:
					if (prefix)
					{
						return "ZW_ALRM_CO2/ZW_ALRM_EVT_INACTIVE_CLEAR";
					}
					return "ZW_ALRM_EVT_INACTIVE_CLEAR";
					break;
				case ZW_ALRM_EVT_CO2_L:
					if (prefix)
					{
						return "ZW_ALRM_CO2/ZW_ALRM_EVT_CO2_L";
					}
					return "ZW_ALRM_EVT_CO2_L";
					break;
				case ZW_ALRM_EVT_CO2:
					if (prefix)
					{
						return "ZW_ALRM_CO2/ZW_ALRM_EVT_CO2";
					}
					return "ZW_ALRM_EVT_CO2";
					break;
				case ZW_ALRM_EVT_CO2_REPLA:
					if (prefix)
					{
						return "ZW_ALRM_CO2/ZW_ALRM_EVT_CO2_REPLA";
					}
					return "ZW_ALRM_EVT_CO2_REPLA";
					break;
				default:
					if (prefix)
					{
						return "ZW_ALRM_CO2/ZW_ALRM_EVT_UNKNOWN";
					}
					return "ZW_ALRM_EVT_UNKNOWN";
					break;
			}
			break;
		case ZW_ALRM_HEAT:
			switch (event)
			{
				case ZW_ALRM_EVT_INACTIVE_CLEAR:
					if (prefix)
					{
						return "ZW_ALRM_HEAT/ZW_ALRM_EVT_INACTIVE_CLEAR";
					}
					return "ZW_ALRM_EVT_INACTIVE_CLEAR";
					break;
				case ZW_ALRM_EVT_OVERHEAT_L:
					if (prefix)
					{
						return "ZW_ALRM_HEAT/ZW_ALRM_EVT_OVERHEAT_L";
					}
					return "ZW_ALRM_EVT_OVERHEAT_L";
					break;
				case ZW_ALRM_EVT_OVERHEAT:
					if (prefix)
					{
						return "ZW_ALRM_HEAT/ZW_ALRM_EVT_OVERHEAT";
					}
					return "ZW_ALRM_EVT_OVERHEAT";
					break;
				case ZW_ALRM_EVT_TEMP_RISE_L:
					if (prefix)
					{
						return "ZW_ALRM_HEAT/ZW_ALRM_EVT_TEMP_RISE_L";
					}
					return "ZW_ALRM_EVT_TEMP_RISE_L";
					break;
				case ZW_ALRM_EVT_TEMP_RISE:
					if (prefix)
					{
						return "ZW_ALRM_HEAT/ZW_ALRM_EVT_TEMP_RISE";
					}
					return "ZW_ALRM_EVT_TEMP_RISE";
					break;
				case ZW_ALRM_EVT_UNDRHEAT_L:
					if (prefix)
					{
						return "ZW_ALRM_HEAT/ZW_ALRM_EVT_UNDRHEAT_L";
					}
					return "ZW_ALRM_EVT_UNDRHEAT_L";
					break;
				case ZW_ALRM_EVT_UNDRHEAT:
					if (prefix)
					{
						return "ZW_ALRM_HEAT/ZW_ALRM_EVT_UNDRHEAT";
					}
					return "ZW_ALRM_EVT_UNDRHEAT";
					break;
				default:
					if (prefix)
					{
						return "ZW_ALRM_HEAT/ZW_ALRM_EVT_UNKNOWN";
					}
					return "ZW_ALRM_EVT_UNKNOWN";
					break;
			}
			break;
		case ZW_ALRM_WATER:
			switch (event)
			{
				case ZW_ALRM_EVT_INACTIVE_CLEAR:
					if (prefix)
					{
						return "ZW_ALRM_WATER/ZW_ALRM_EVT_INACTIVE_CLEAR";
					}
					return "ZW_ALRM_EVT_INACTIVE_CLEAR";
					break;
				case ZW_ALRM_EVT_LEAK_L:
					if (prefix)
					{
						return "ZW_ALRM_WATER/ZW_ALRM_EVT_LEAK_L";
					}
					return "ZW_ALRM_EVT_LEAK_L";
					break;
				case ZW_ALRM_EVT_LEAK:
					if (prefix)
					{
						return "ZW_ALRM_WATER/ZW_ALRM_EVT_LEAK";
					}
					return "ZW_ALRM_EVT_LEAK";
					break;
				case ZW_ALRM_EVT_LVL_L:
					if (prefix)
					{
						return "ZW_ALRM_WATER/ZW_ALRM_EVT_LVL_L";
					}
					return "ZW_ALRM_EVT_LVL_L";
					break;
				case ZW_ALRM_EVT_LVL:
					if (prefix)
					{
						return "ZW_ALRM_WATER/ZW_ALRM_EVT_LVL";
					}
					return "ZW_ALRM_EVT_LVL";
					break;
				default:
					if (prefix)
					{
						return "ZW_ALRM_WATER/ZW_ALRM_EVT_UNKNOWN";
					}
					return "ZW_ALRM_EVT_UNKNOWN";
					break;
			}
			break;
		case ZW_ALRM_LOCK:
			switch (event)
			{
				case ZW_ALRM_EVT_INACTIVE_CLEAR:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_INACTIVE_CLEAR";
					}
					return "ZW_ALRM_EVT_INACTIVE_CLEAR";
					break;
				case ZW_ALRM_EVT_MANUAL_LCK:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_MANUAL_LCK";
					}
					return "ZW_ALRM_EVT_MANUAL_LCK";
					break;
				case ZW_ALRM_EVT_MANUAL_ULCK:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_MANUAL_ULCK";
					}
					return "ZW_ALRM_EVT_MANUAL_ULCK";
					break;
				case ZW_ALRM_EVT_RF_LCK:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_RF_LCK";
					}
					return "ZW_ALRM_EVT_RF_LCK";
					break;
				case ZW_ALRM_EVT_RF_ULCK:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_RF_ULCK";
					}
					return "ZW_ALRM_EVT_RF_ULCK";
					break;
				case ZW_ALRM_EVT_KEYPAD_LCK:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_KEYPAD_LCK";
					}
					return "ZW_ALRM_EVT_KEYPAD_LCK";
					break;
				case ZW_ALRM_EVT_KEYPAD_ULCK:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_KEYPAD_ULCK";
					}
					return "ZW_ALRM_EVT_KEYPAD_ULCK";
					break;
				case ZW_ALRM_EVT_MANUAL_NOT_FUL_LCK:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_MANUAL_NOT_FUL_LCK";
					}
					return "ZW_ALRM_EVT_MANUAL_NOT_FUL_LCK";
					break;
				case ZW_ALRM_EVT_RF_NOT_FUL_LCK:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_RF_NOT_FUL_LCK";
					}
					return "ZW_ALRM_EVT_RF_NOT_FUL_LCK";
					break;
				case ZW_ALRM_EVT_AUTO_LCK:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_AUTO_LCK";
					}
					return "ZW_ALRM_EVT_AUTO_LCK";
					break;
				case ZW_ALRM_EVT_AUTO_NOT_FUL_OPER:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_AUTO_NOT_FUL_OPER";
					}
					return "ZW_ALRM_EVT_AUTO_NOT_FUL_OPER";
					break;
				case ZW_ALRM_EVT_LCK_JAMMED:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_LCK_JAMMED";
					}
					return "ZW_ALRM_EVT_LCK_JAMMED";
					break;
				case ZW_ALRM_EVT_ALL_CODE_DEL:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_ALL_CODE_DEL";
					}
					return "ZW_ALRM_EVT_ALL_CODE_DEL";
					break;
				case ZW_ALRM_EVT_1_CODE_DEL:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_1_CODE_DEL";
					}
					return "ZW_ALRM_EVT_1_CODE_DEL";
					break;
				case ZW_ALRM_EVT_CODE_ADDED:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_CODE_ADDED";
					}
					return "ZW_ALRM_EVT_CODE_ADDED";
					break;
				case ZW_ALRM_EVT_CODE_DUP:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_CODE_DUP";
					}
					return "ZW_ALRM_EVT_CODE_DUP";
					break;
				case ZW_ALRM_EVT_KEYPAD_DISABLED:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_KEYPAD_DISABLED";
					}
					return "ZW_ALRM_EVT_KEYPAD_DISABLED";
					break;
				case ZW_ALRM_EVT_KEYPAD_BUSY:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_KEYPAD_BUSY";
					}
					return "ZW_ALRM_EVT_KEYPAD_BUSY";
					break;
				case ZW_ALRM_EVT_NEW_PROG_CODE:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_NEW_PROG_CODE";
					}
					return "ZW_ALRM_EVT_NEW_PROG_CODE";
					break;
				case ZW_ALRM_EVT_USR_CODE_LIMIT:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_USR_CODE_LIMIT";
					}
					return "ZW_ALRM_EVT_USR_CODE_LIMIT";
					break;
				case ZW_ALRM_EVT_RF_ULCK_INVLD_CODE:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_RF_ULCK_INVLD_CODE";
					}
					return "ZW_ALRM_EVT_RF_ULCK_INVLD_CODE";
					break;
				case ZW_ALRM_EVT_RF_LCK_INVLD_CODE:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_RF_LCK_INVLD_CODE";
					}
					return "ZW_ALRM_EVT_RF_LCK_INVLD_CODE";
					break;
				case ZW_ALRM_EVT_WINDOW_DOOR_OPEN:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_WINDOW_DOOR_OPEN";
					}
					return "ZW_ALRM_EVT_WINDOW_DOOR_OPEN";
					break;
				case ZW_ALRM_EVT_WINDOW_DOOR_CLOSED:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_WINDOW_DOOR_CLOSED";
					}
					return "ZW_ALRM_EVT_WINDOW_DOOR_CLOSED";
					break;
				default:
					if (prefix)
					{
						return "ZW_ALRM_LOCK/ZW_ALRM_EVT_UNKNOWN";
					}
					return "ZW_ALRM_EVT_UNKNOWN";
					break;
			}
			break;
		case ZW_ALRM_BURGLAR:
			switch (event)
			{
				case ZW_ALRM_EVT_INACTIVE_CLEAR:
					if (prefix)
					{
						return "ZW_ALRM_BURGLAR/ZW_ALRM_EVT_INACTIVE_CLEAR";
					}
					return "ZW_ALRM_EVT_INACTIVE_CLEAR";
					break;
				case ZW_ALRM_EVT_INTRUSION_L:
					if (prefix)
					{
						return "ZW_ALRM_BURGLAR/ZW_ALRM_EVT_INTRUSION_L";
					}
					return "ZW_ALRM_EVT_INTRUSION_L";
					break;
				case ZW_ALRM_EVT_INTRUSION:
					if (prefix)
					{
						return "ZW_ALRM_BURGLAR/ZW_ALRM_EVT_INTRUSION";
					}
					return "ZW_ALRM_EVT_INTRUSION";
					break;
				case ZW_ALRM_EVT_TMPR_COVER:
					if (prefix)
					{
						return "ZW_ALRM_BURGLAR/ZW_ALRM_EVT_TMPR_COVER";
					}
					return "ZW_ALRM_EVT_TMPR_COVER";
					break;
				case ZW_ALRM_EVT_TMPR_CODE:
					if (prefix)
					{
						return "ZW_ALRM_BURGLAR/ZW_ALRM_EVT_TMPR_CODE";
					}
					return "ZW_ALRM_EVT_TMPR_CODE";
					break;
				case ZW_ALRM_EVT_GLASS_L:
					if (prefix)
					{
						return "ZW_ALRM_BURGLAR/ZW_ALRM_EVT_GLASS_L";
					}
					return "ZW_ALRM_EVT_GLASS_L";
					break;
				case ZW_ALRM_EVT_GLASS:
					if (prefix)
					{
						return "ZW_ALRM_BURGLAR/ZW_ALRM_EVT_GLASS";
					}
					return "ZW_ALRM_EVT_GLASS";
					break;
				case ZW_ALRM_EVT_MOTION_DET_L:
					if (prefix)
					{
						return "ZW_ALRM_BURGLAR/ZW_ALRM_EVT_MOTION_DET_L";
					}
					return "ZW_ALRM_EVT_MOTION_DET_L";
					break;
				case ZW_ALRM_EVT_MOTION_DET:
					if (prefix)
					{
						return "ZW_ALRM_BURGLAR/ZW_ALRM_EVT_MOTION_DET";
					}
					return "ZW_ALRM_EVT_MOTION_DET";
					break;
				default:
					if (prefix)
					{
						return "ZW_ALRM_BURGLAR/ZW_ALRM_EVT_UNKNOWN";
					}
					return "ZW_ALRM_EVT_UNKNOWN";
					break;
			}
			break;
		case ZW_ALRM_POWER:
			switch (event)
			{
				case ZW_ALRM_EVT_INACTIVE_CLEAR:
					if (prefix)
					{
						return "ZW_ALRM_POWER/ZW_ALRM_EVT_INACTIVE_CLEAR";
					}
					return "ZW_ALRM_EVT_INACTIVE_CLEAR";
					break;
				case ZW_ALRM_EVT_POWER:
					if (prefix)
					{
						return "ZW_ALRM_POWER/ZW_ALRM_EVT_POWER";
					}
					return "ZW_ALRM_EVT_POWER";
					break;
				case ZW_ALRM_EVT_AC_OFF:
					if (prefix)
					{
						return "ZW_ALRM_POWER/ZW_ALRM_EVT_AC_OFF";
					}
					return "ZW_ALRM_EVT_AC_OFF";
					break;
				case ZW_ALRM_EVT_AC_ON:
					if (prefix)
					{
						return "ZW_ALRM_POWER/ZW_ALRM_EVT_AC_ON";
					}
					return "ZW_ALRM_EVT_AC_ON";
					break;
				case ZW_ALRM_EVT_SURGE:
					if (prefix)
					{
						return "ZW_ALRM_POWER/ZW_ALRM_EVT_SURGE";
					}
					return "ZW_ALRM_EVT_SURGE";
					break;
				case ZW_ALRM_EVT_VOLT_DROP:
					if (prefix)
					{
						return "ZW_ALRM_POWER/ZW_ALRM_EVT_VOLT_DROP";
					}
					return "ZW_ALRM_EVT_VOLT_DROP";
					break;
				case ZW_ALRM_EVT_OVER_CURRENT:
					if (prefix)
					{
						return "ZW_ALRM_POWER/ZW_ALRM_EVT_OVER_CURRENT";
					}
					return "ZW_ALRM_EVT_OVER_CURRENT";
					break;
				case ZW_ALRM_EVT_OVER_VOLT:
					if (prefix)
					{
						return "ZW_ALRM_POWER/ZW_ALRM_EVT_OVER_VOLT";
					}
					return "ZW_ALRM_EVT_OVER_VOLT";
					break;
				case ZW_ALRM_EVT_OVER_LOAD:
					if (prefix)
					{
						return "ZW_ALRM_POWER/ZW_ALRM_EVT_OVER_LOAD";
					}
					return "ZW_ALRM_EVT_OVER_LOAD";
					break;
				case ZW_ALRM_EVT_LOAD_ERR:
					if (prefix)
					{
						return "ZW_ALRM_POWER/ZW_ALRM_EVT_LOAD_ERR";
					}
					return "ZW_ALRM_EVT_LOAD_ERR";
					break;
				case ZW_ALRM_EVT_REPLACE_BATT_SOON:
					if (prefix)
					{
						return "ZW_ALRM_POWER/ZW_ALRM_EVT_REPLACE_BATT_SOON";
					}
					return "ZW_ALRM_EVT_REPLACE_BATT_SOON";
					break;
				case ZW_ALRM_EVT_REPLACE_BATT_NOW:
					if (prefix)
					{
						return "ZW_ALRM_POWER/ZW_ALRM_EVT_REPLACE_BATT_NOW";
					}
					return "ZW_ALRM_EVT_REPLACE_BATT_NOW";
					break;
				default:
					if (prefix)
					{
						return "ZW_ALRM_POWER/ZW_ALRM_EVT_UNKNOWN";
					}
					return "ZW_ALRM_EVT_UNKNOWN";
					break;
			}
			break;
		case ZW_ALRM_SYSTEM:
			switch (event)
			{
				case ZW_ALRM_EVT_INACTIVE_CLEAR:
					if (prefix)
					{
						return "ZW_ALRM_SYSTEM/ZW_ALRM_EVT_INACTIVE_CLEAR";
					}
					return "ZW_ALRM_EVT_INACTIVE_CLEAR";
					break;
				case ZW_ALRM_EVT_HW:
					if (prefix)
					{
						return "ZW_ALRM_SYSTEM/ZW_ALRM_EVT_HW";
					}
					return "ZW_ALRM_EVT_HW";
					break;
				case ZW_ALRM_EVT_SW:
					if (prefix)
					{
						return "ZW_ALRM_SYSTEM/ZW_ALRM_EVT_SW";
					}
					return "ZW_ALRM_EVT_SW";
					break;
				case ZW_ALRM_EVT_HW_OEM_CODE:
					if (prefix)
					{
						return "ZW_ALRM_SYSTEM/ZW_ALRM_EVT_HW_OEM_CODE";
					}
					return "ZW_ALRM_EVT_HW_OEM_CODE";
					break;
				case ZW_ALRM_EVT_SW_OEM_CODE:
					if (prefix)
					{
						return "ZW_ALRM_SYSTEM/ZW_ALRM_EVT_SW_OEM_CODE";
					}
					return "ZW_ALRM_EVT_SW_OEM_CODE";
					break;
				default:
					if (prefix)
					{
						return "ZW_ALRM_SYSTEM/ZW_ALRM_EVT_UNKNOWN";
					}
					return "ZW_ALRM_EVT_UNKNOWN";
					break;
			}
			break;
		case ZW_ALRM_EMERGENCY:
			switch (event)
			{
				case ZW_ALRM_EVT_INACTIVE_CLEAR:
					if (prefix)
					{
						return "ZW_ALRM_EMERGENCY/ZW_ALRM_EVT_INACTIVE_CLEAR";
					}
					return "ZW_ALRM_EVT_INACTIVE_CLEAR";
					break;
				case ZW_ALRM_EVT_POLICE:
					if (prefix)
					{
						return "ZW_ALRM_EMERGENCY/ZW_ALRM_EVT_POLICE";
					}
					return "ZW_ALRM_EVT_POLICE";
					break;
				case ZW_ALRM_EVT_FIRE:
					if (prefix)
					{
						return "ZW_ALRM_EMERGENCY/ZW_ALRM_EVT_FIRE";
					}
					return "ZW_ALRM_EVT_FIRE";
					break;
				case ZW_ALRM_EVT_MEDICAL:
					if (prefix)
					{
						return "ZW_ALRM_EMERGENCY/ZW_ALRM_EVT_MEDICAL";
					}
					return "ZW_ALRM_EVT_MEDICAL";
					break;
				default:
					if (prefix)
					{
						return "ZW_ALRM_EMERGENCY/ZW_ALRM_EVT_UNKNOWN";
					}
					return "ZW_ALRM_EVT_UNKNOWN";
					break;
			}
			break;
		case ZW_ALRM_CLOCK:
			switch (event)
			{
				case ZW_ALRM_EVT_INACTIVE_CLEAR:
					if (prefix)
					{
						return "ZW_ALRM_CLOCK/ZW_ALRM_EVT_INACTIVE_CLEAR";
					}
					return "ZW_ALRM_EVT_INACTIVE_CLEAR";
					break;
				case ZW_ALRM_EVT_WKUP:
					if (prefix)
					{
						return "ZW_ALRM_CLOCK/ZW_ALRM_EVT_WKUP";
					}
					return "ZW_ALRM_EVT_WKUP";
					break;
				case ZW_ALRM_EVT_TIMER_ENDED:
					if (prefix)
					{
						return "ZW_ALRM_CLOCK/ZW_ALRM_EVT_TIMER_ENDED";
					}
					return "ZW_ALRM_EVT_TIMER_ENDED";
					break;
				default:
					if (prefix)
					{
						return "ZW_ALRM_CLOCK/ZW_ALRM_EVT_UNKNOWN";
					}
					return "ZW_ALRM_EVT_UNKNOWN";
					break;
			}
			break;
		case ZW_ALRM_FIRST:
			return "ZW_ALRM_FIRST";
			break;
		default:
			return "ZW_ALRM_EVT_UNKNOWN";
			break;
	}
}

char* translate_alarm_event_report(uint8_t type, uint8_t event)
{
	switch (type)
	{
		case ZW_ALRM_SMOKE:
			return JKEY_REPORT_SMOKE;
			break;
		case ZW_ALRM_CO:
			return JKEY_REPORT_CO;
			break;
		case ZW_ALRM_CO2:
			return JKEY_REPORT_CO2;
			break;
		case ZW_ALRM_HEAT:
			return JKEY_REPORT_HEAT;
			break;
		case ZW_ALRM_WATER:
			return JKEY_REPORT_WATER;
			break;
		case ZW_ALRM_LOCK:
			return JKEY_REPORT_DOOR;
			break;
		case ZW_ALRM_BURGLAR:
			switch (event)
			{
				case ZW_ALRM_EVT_TMPR_COVER:
				case ZW_ALRM_EVT_TMPR_CODE:
					return JKEY_REPORT_TAMPER;
					break;
				case ZW_ALRM_EVT_MOTION_DET_L:
				case ZW_ALRM_EVT_MOTION_DET:
					return JKEY_REPORT_MOTION;
					break;
				default:
					return JKEY_REPORT_XXX;
					break;
			}
			break;
		case ZW_ALRM_POWER:
			switch (event)
			{
				case ZW_ALRM_EVT_POWER:
					return JKEY_REPORT_POWER_OVER_LOAD;
					break;
				case ZW_ALRM_EVT_OVER_LOAD:
					return JKEY_REPORT_POWER;
					break;
				default:
					return JKEY_REPORT_XXX;
					break;
			}
			break;
		case ZW_ALRM_SYSTEM:
			return JKEY_REPORT_SYSTEM;
			break;
		case ZW_ALRM_EMERGENCY:
			return JKEY_REPORT_EMERGENCY;
			break;
		case ZW_ALRM_CLOCK:
			return JKEY_REPORT_CLOCK;
			break;
		case ZW_ALRM_FIRST:
			return JKEY_REPORT_FIRST;
			break;
		default:
			return JKEY_REPORT_XXX;
			break;
	}
}

char* translate_alarm_ex_evt_type(uint8_t ex_evt_type)
{
	switch (ex_evt_type)
	{
		case ZW_ALRM_PARAM_LOC:
			return "ZW_ALRM_PARAM_LOC";
			break;
		case ZW_ALRM_PARAM_USRID:
			return "ZW_ALRM_PARAM_USRID";
			break;
		case ZW_ALRM_PARAM_OEM_ERR_CODE:
			return "ZW_ALRM_PARAM_OEM_ERR_CODE";
			break;
		case ZW_ALRM_PARAM_PROPRIETARY:
			return "ZW_ALRM_PARAM_PROPRIETARY";
			break;
		case ZW_ALRM_PARAM_EVENT_ID:
			return "ZW_ALRM_PARAM_EVENT_ID";
			break;
		case ZW_ALRM_PARAM_UNKNOWN:
			return "ZW_ALRM_PARAM_UNKNOWN";
			break;
		default:
			return "ZW_ALRM_PARAM_XXX";
			break;
	}
}

char* translate_alarm_sts(uint8_t status)
{
	switch (status)
	{
		case ZW_ALRM_STS_DEACTIVATED:
			return "ZW_ALRM_STS_DEACTIVATED";
			break;
		case ZW_ALRM_STS_ACTIVATED:
			return "ZW_ALRM_STS_ACTIVATED";
			break;
		case ZW_ALRM_STS_NO_PEND_NOTICE:
			return "ZW_ALRM_STS_NO_PEND_NOTICE";
			break;
		default:
			return "ZW_ALRM_STS_XXX";
			break;
	}
}

char* translate_application_busy_sts(uint8_t status)
{
	switch (status)
	{
		case ZW_BSY_STS_TRY:
			return "ZW_BSY_STS_TRY";
			break;
		case ZW_BSY_STS_TRY_WAIT:
			return "ZW_BSY_STS_TRY_WAIT";
			break;
		case ZW_BSY_STS_Q:
			return "ZW_BSY_STS_Q";
			break;
		default:
			return "ZW_BSY_STS_XXX";
			break;
	}
}

char* translate_application_reject_sts(uint8_t status)
{
	switch (status)
	{
		case ZW_RJ_STS_REJECT:
			return "ZW_RJ_STS_REJECT";
			break;
		default:
			return "ZW_RJ_STS_XXX";
			break;
	}
}

char* translate_av_keyAttr(uint8_t keyAttr)
{
	switch (keyAttr)
	{
		case 0x00:
			return "Key_Down";
			break;
		case 0x01:
			return "Key_Up";
			break;
		case 0x02:
			return "Keep_Alive";
			break;
		default:
			return "XXX";
			break;
	}
}

char* translate_basic_val(uint8_t val)
{
	switch (val)
	{
		// 0xFE
		case 0xFE:
			return "unknown";
			break;
		case 0:
			return "off";
			break;
		case 0xFF:
			return "on";
			break;
		default:
			return "%";
			break;
	}
}

char* translate_battery_level(uint8_t level)
{
	switch (level)
	{
		// 0xFF
		case 0xFF:
			return "low";
			break;
		default:
			return "level";
			break;
	}
}

char* translate_csc_keyAttr(uint8_t keyAttr)
{
	switch (keyAttr)
	{
		case ZW_CSC_KEY_ATTRIB_KEY_PRESSED_1_TIME:
			return "PRESSED_1_TIME";
			break;
		case ZW_CSC_KEY_ATTRIB_KEY_RELEASED:
			return "RELEASED";
			break;
		case ZW_CSC_KEY_ATTRIB_KEY_HELDDOWN:
			return "HELDDOWN";
			break;
		case ZW_CSC_KEY_ATTRIB_KEY_PRESSED_2_TIME:
			return "PRESSED_2_TIME";
			break;
		case ZW_CSC_KEY_ATTRIB_KEY_PRESSED_3_TIME:
			return "PRESSED_3_TIME";
			break;
		case ZW_CSC_KEY_ATTRIB_KEY_PRESSED_4_TIME:
			return "PRESSED_4_TIME";
			break;
		case ZW_CSC_KEY_ATTRIB_KEY_PRESSED_5_TIME:
			return "PRESSED_5_TIME";
			break;
		default:
			return "XXX";
			break;
	}
}

char* translate_bsensor_report(uint8_t type)
{
	switch (type)
	{
		// 0x01
		case ZW_BSENSOR_TYPE_GP:
			return JKEY_REPORT_GP;
			break;
		// 0x02
		case ZW_BSENSOR_TYPE_SMOKE:
			return JKEY_REPORT_SMOKE;
			break;
		// 0x03
		case ZW_BSENSOR_TYPE_CO:
			return JKEY_REPORT_CO;
			break;
		// 0x04
		case ZW_BSENSOR_TYPE_CO2:
			return JKEY_REPORT_CO2;
			break;
		// 0x05
		case ZW_BSENSOR_TYPE_HEAT:
			return JKEY_REPORT_HEAT;
			break;
		// 0x06
		case ZW_BSENSOR_TYPE_WATER:
			return JKEY_REPORT_WATER;
			break;
		// 0x07
		case ZW_BSENSOR_TYPE_FREEZE:
			return JKEY_REPORT_FREEZE;
			break;
		// 0x08
		case ZW_BSENSOR_TYPE_TAMPER:
			return JKEY_REPORT_TAMPER;
			break;
		// 0x09
		case ZW_BSENSOR_TYPE_AUX:
			return JKEY_REPORT_AUX;
			break;
		// 0x0A
		case ZW_BSENSOR_TYPE_DR_WIN:
			return JKEY_REPORT_DOOR;
			break;
		// 0x0B
		case ZW_BSENSOR_TYPE_TILT:
			return JKEY_REPORT_TILT;
			break;
		// 0x0C
		case ZW_BSENSOR_TYPE_MOTION:
			return JKEY_REPORT_MOTION;
			break;
		// 0x0D
		case ZW_BSENSOR_TYPE_GLASS_BRK:
			return JKEY_REPORT_GLASS;
			break;
		// 0xFF
		case ZW_BSENSOR_TYPE_1ST_SUP:
			return JKEY_REPORT_1ST_SUP;
			break;
		default:
			return JKEY_REPORT_XXX;
			break;
	}
}

char* translate_bsensor_type(uint8_t type)
{
	switch (type)
	{
		// 0x01
		case ZW_BSENSOR_TYPE_GP:
			return "ZW_BSENSOR_TYPE_GP";
			break;
		// 0x02
		case ZW_BSENSOR_TYPE_SMOKE:
			return "ZW_BSENSOR_TYPE_SMOKE";
			break;
		// 0x03
		case ZW_BSENSOR_TYPE_CO:
			return "ZW_BSENSOR_TYPE_CO";
			break;
		// 0x04
		case ZW_BSENSOR_TYPE_CO2:
			return "ZW_BSENSOR_TYPE_CO2";
			break;
		// 0x05
		case ZW_BSENSOR_TYPE_HEAT:
			return "ZW_BSENSOR_TYPE_HEAT";
			break;
		// 0x06
		case ZW_BSENSOR_TYPE_WATER:
			return "ZW_BSENSOR_TYPE_WATER";
			break;
		// 0x07
		case ZW_BSENSOR_TYPE_FREEZE:
			return "ZW_BSENSOR_TYPE_FREEZE";
			break;
		// 0x08
		case ZW_BSENSOR_TYPE_TAMPER:
			return "ZW_BSENSOR_TYPE_TAMPER";
			break;
		// 0x09
		case ZW_BSENSOR_TYPE_AUX:
			return "ZW_BSENSOR_TYPE_AUX";
			break;
		// 0x0A
		case ZW_BSENSOR_TYPE_DR_WIN:
			return "ZW_BSENSOR_TYPE_DR_WIN";
			break;
		// 0x0B
		case ZW_BSENSOR_TYPE_TILT:
			return "ZW_BSENSOR_TYPE_TILT";
			break;
		// 0x0C
		case ZW_BSENSOR_TYPE_MOTION:
			return "ZW_BSENSOR_TYPE_MOTION";
			break;
		// 0x0D
		case ZW_BSENSOR_TYPE_GLASS_BRK:
			return "ZW_BSENSOR_TYPE_GLASS_BRK";
			break;
		// 0xFF
		case ZW_BSENSOR_TYPE_1ST_SUP:
			return "ZW_BSENSOR_TYPE_1ST_SUP";
			break;
		default:
			return "ZW_BSENSOR_TYPE_XXX";
			break;
	}
}

char* translate_methodid(uint16_t methodid)
{
	switch (methodid)
	{
		case JVAL_METHODID_EVENT:
			return "MethodID Event";
			break;
		case JVAL_METHODID_PUT:
			return "MethodID PUT";
			break;
		case JVAL_METHODID_GET:
			return "MethodID GET";
			break;
		default:
			return "MethodID XXX";
			break;
	}
}

char* translate_protocolid(uint16_t protocolid)
{
	switch (protocolid)
	{
		case JVAL_PROTOCOLID_ZWAVE:
			return "ZWAVE";
			break;
		case JVAL_PROTOCOLID_ZWAVES2:
			return "ZWAVES2";
			break;
		case JVAL_PROTOCOLID_ZIGBEE:
			return "ZIGBEE";
			break;
		case JVAL_PROTOCOLID_PAN27:
			return "PAN27";
			break;
		case JVAL_PROTOCOLID_BLUETOOTH:
			return "BLUETOOTH";
			break;
		case JVAL_PROTOCOLID_ONVIF:
			return "ONVIF";
			break;
		case JVAL_PROTOCOLID_WIFI_PHILIO:
			return "WIFI_PHILIO";
			break;
		case JVAL_PROTOCOLID_ZIGBEE_PHILIO:
			return "ZIGBEE_PHILIO";
			break;
		case JVAL_PROTOCOLID_TIQIAA:
			return "TIQIAA";
			break;
		case JVAL_PROTOCOLID_HONEY_MAEKET:
			return "HONEY_MAEKET";
			break;
		case JVAL_PROTOCOLID_HONEYCOMB:
			return "HONEYCOMB";
			break;
		case JVAL_PROTOCOLID_ECHO:
			return "ECHO";
			break;
		case JVAL_PROTOCOLID_MASTER:
			return "MASTER";
			break;
		case JVAL_PROTOCOLID_BROADCAST:
			return "BROADCAST";
			break;
		case JVAL_PROTOCOLID_BASE:
			return "BASE";
			break;
		default:
			return "Protocol XXX";
			break;
	}
}

uint8_t translate_sensor_mask_to_unit(uint8_t sensor_unit_mask)
{
	int i=0;
	int ret = 0xff;
	for (i=0; i<4; i++)
	{
		if (sensor_unit_mask & (1 << i))
		{
			//Found supported unit
			ret = i;
			break;
		}
	}
	return ret;
}

char* translate_sensor_unit(uint8_t type, uint8_t unit)
{
	return SensorNameArray[type].unit[unit];
}

const char* translate_sensor_report(uint8_t type)
{
	return SensorNameArray[type].report;
}

char* translate_sensor_type(uint8_t type)
{
	return SensorNameArray[type].name;
}

char* translate_usrcod_sts(uint8_t sts)
{
	switch (sts)
	{
		case ZW_USRCOD_AVAIL:
			return "ZW_USRCOD_AVAIL";
			break;
		case ZW_USRCOD_UNAVAIL:
			return "ZW_USRCOD_UNAVAIL";
			break;
		case ZW_USRCOD_RSVD:
			return "ZW_USRCOD_RSVD";
			break;
		case ZW_USRCOD_NO_STS:
			return "ZW_USRCOD_NO_STS";
			break;
		default:
			return "ZW_USRCOD_XXX";
			break;
	}
}

char* translate_door_mode(uint8_t mode)
{
	switch (mode)
	{
		case ZW_DOOR_UNSEC:
			return "ZW_DOOR_UNSEC";
			break;
		case ZW_DOOR_UNSEC_TMOUT:
			return "ZW_DOOR_UNSEC_TMOUT";
			break;
		case ZW_DOOR_UNSEC_IN:
			return "ZW_DOOR_UNSEC_IN";
			break;
		case ZW_DOOR_UNSEC_IN_TMOUT:
			return "ZW_DOOR_UNSEC_IN_TMOUT";
			break;
		case ZW_DOOR_UNSEC_OUT:
			return "ZW_DOOR_UNSEC_OUT";
			break;
		case ZW_DOOR_UNSEC_OUT_TMOUT:
			return "ZW_DOOR_UNSEC_OUT_TMOUT";
			break;
		case ZW_DOOR_SEC:
			return "ZW_DOOR_SEC";
			break;
		default:
			return "ZW_DOOR_XXX";
			break;
	}
}

void translate_door_cond(uint8_t cond, char *cond_name)
{
	if (cond & ZW_COND_DOOR_MASK)
	{
		strcat(cond_name, "door closed/");
	}
	else
	{
		strcat(cond_name, "door open/");
	}
	if (cond & ZW_COND_BOLT_MASK)
	{
		strcat(cond_name, "bolt unlocked/");
	}
	else
	{
		strcat(cond_name, "bolt locked/");
	}
	if (cond & ZW_COND_LATCH_MASK)
	{
		strcat(cond_name, "latch closed/");
	}
	else
	{
		strcat(cond_name, "latch open/");
	}

	if (cond_name[strlen(cond_name)-1] =='/')
	{
		cond_name[strlen(cond_name)-1] = '\0';
	}
}

char* translate_door_op_type(uint8_t type)
{
	switch (type)
	{
		case ZW_DOOR_OP_CONST:
			return "ZW_DOOR_OP_CONST";
			break;
		case ZW_DOOR_OP_TIMED:
			return "ZW_DOOR_OP_TIMED";
			break;
		default:
			return "ZW_DOOR_OP_XXX";
			break;
	}
}

char* translate_meter_type(uint8_t type)
{
	switch (type)
	{
		case ZW_METER_TYPE_ELEC:
			return "ZW_METER_TYPE_ELEC";
			break;
		case ZW_METER_TYPE_GAS:
			return "ZW_METER_TYPE_GAS";
			break;
		case ZW_METER_TYPE_WATER:
			return "ZW_METER_TYPE_WATER";
			break;
		default:
			return "ZW_METER_TYPE_XXX";
			break;
	}
}

char* translate_meter_unit(uint8_t type, uint8_t unit)
{
	switch (type)
	{
		case ZW_METER_TYPE_ELEC:
			switch (unit)
			{
				case ZW_METER_UNIT_ELEC_KWH:
					return "kWh";
					break;
				case ZW_METER_UNIT_ELEC_KVAH:
					return "kVAh";
					break;
				case ZW_METER_UNIT_ELEC_W:
					return "W";
					break;
				case ZW_METER_UNIT_ELEC_PULSE:
					return "pulse count";
					break;
				case ZW_METER_UNIT_ELEC_V:
					return "V";
					break;
				case ZW_METER_UNIT_ELEC_A:
					return "A";
					break;
				case ZW_METER_UNIT_ELEC_PF:
					return "power factor";
					break;
				default:
					return "ZW_METER_UNIT_XXX";
					break;
			}
			break;
		case ZW_METER_TYPE_GAS:
			switch (unit)
			{
				case ZW_METER_UNIT_GAS_CM:
					return "cubic meters";
					break;
				case ZW_METER_UNIT_GAS_CF:
					return "cubic feet";
					break;
				case ZW_METER_UNIT_GAS_PULSE:
					return "pulse count";
					break;
				default:
					return "ZW_METER_UNIT_XXX";
					break;
			}
			break;
		case ZW_METER_TYPE_WATER:
			switch (unit)
			{
				case ZW_METER_UNIT_WATER_CM:
					return "cubic meters";
					break;
				case ZW_METER_UNIT_WATER_CF:
					return "cubic feet";
					break;
				case ZW_METER_UNIT_WATER_GAL:
					return "US gallons";
					break;
				case ZW_METER_UNIT_WATER_PULSE:
					return "pulse count";
					break;
				default:
					return "ZW_METER_UNIT_XXX";
					break;
			}
			break;
		default:
			return "ZW_METER_UNIT_XXX";
			break;
	}
}

void translate_meter_unit_sup(uint8_t type, uint8_t unit, char *unit_name)
{
	switch (type)
	{
		case ZW_METER_TYPE_ELEC:
			if (unit & ZW_METER_SUP_UNIT_ELEC_KWH)
			{
				strcat(unit_name, "kWh/");
			}
			if (unit & ZW_METER_SUP_UNIT_ELEC_KVAH)
			{
				strcat(unit_name, "kVAh/");
			}
			if (unit & ZW_METER_SUP_UNIT_ELEC_W)
			{
				strcat(unit_name, "W/");
			}
			if (unit & ZW_METER_SUP_UNIT_ELEC_PULSE)
			{
				strcat(unit_name, "pulse count/");
			}
			if (unit & ZW_METER_SUP_UNIT_ELEC_V)
			{
				strcat(unit_name, "V/");
			}
			if (unit & ZW_METER_SUP_UNIT_ELEC_A)
			{
				strcat(unit_name, "A/");
			}
			if (unit & ZW_METER_SUP_UNIT_ELEC_PF)
			{
				strcat(unit_name, "power factor/");
			}
			break;
		case ZW_METER_TYPE_GAS:
			if (unit & ZW_METER_SUP_UNIT_GAS_CM)
			{
				strcat(unit_name, "cubic meters/");
			}
			if (unit & ZW_METER_SUP_UNIT_GAS_CM)
			{
				strcat(unit_name, "cubic feet/");
			}
			if (unit & ZW_METER_SUP_UNIT_GAS_CM)
			{
				strcat(unit_name, "pulse count/");
			}
			break;
		case ZW_METER_TYPE_WATER:
			if (unit & ZW_METER_SUP_UNIT_WATER_CM)
			{
				strcat(unit_name, "cubic meters/");
			}
			if (unit & ZW_METER_SUP_UNIT_WATER_CF)
			{
				strcat(unit_name, "cubic feet/");
			}
			if (unit & ZW_METER_SUP_UNIT_WATER_GAL)
			{
				strcat(unit_name, "US gallons/");
			}
			if (unit & ZW_METER_SUP_UNIT_WATER_PULSE)
			{
				strcat(unit_name, "Upulse count/");
			}
			break;
		default:
			break;
	}

	if (unit_name[strlen(unit_name)-1] =='/')
	{
		unit_name[strlen(unit_name)-1] = '\0';
	}
}

char* translate_meter_rate(uint8_t rate_type)
{
	switch (rate_type)
	{
		case ZW_METER_RATE_IMPORT:
			return "ZW_METER_RATE_IMPORT";
			break;
		case ZW_METER_RATE_EXPORT:
			return "ZW_METER_RATE_EXPORT";
			break;
		default:
			return "ZW_METER_RATE_XXX";
			break;
	}
}

char* translate_local_prot(uint8_t local_prot)
{
	switch (local_prot)
	{
		// 0
		case ZW_LPROT_UNPROT:
			return "ZW_LPROT_UNPROT";
			break;
		// 1
		case ZW_LPROT_SEQ:
			return "ZW_LPROT_SEQ";
			break;
		// 2
		case ZW_LPROT_NO_CTL:
			return "ZW_LPROT_NO_CTL";
			break;
		default:
			return "ZW_LPROT_NO_XXX";
			break;
	}
}

char* translate_rf_prot(uint8_t rf_prot)
{
	switch (rf_prot)
	{
		// 0
		case ZW_RFPROT_UNPROT:
			return "ZW_RFPROT_UNPROT";
			break;
		// 1
		case ZW_RFPROT_NO_CTL:
			return "ZW_RFPROT_NO_CTL";
			break;
		// 2
		case ZW_RFPROT_NO_RESP:
			return "ZW_RFPROT_NO_RESP";
			break;
		default:
			return "ZW_RFPROT_NO_XXX";
			break;
	}
}

char* translate_dur(uint8_t dur)
{
	switch (dur)
	{
		// 0xFE
		case 0xFE:
			return "unknown";
			break;
		// 0xFF
		case 0xFF:
			return "reserved";
			break;
		case 0:
			return "done";
			break;
		default:
			if ((0x01<=dur) && (dur<=0x7F))
			{
				return "seconds";
			}
			else
			{
				return "minutes";
			}
			break;
	}
}

char* translate_multilevel_level(uint8_t level)
{
	switch (level)
	{
		// 0xFE
		case 0xFE:
			return "unknown";
			break;
		// 0xFF
		case 0xFF:
			return "on";
			break;
		case 0:
			return "off";
			break;
		default:
			if ((level>= 0x01) && (level<=0x63))
			{
				return "%";
			}
			else
			{
				return "on";
			}
			break;
	}
}

char* translate_switch_val(uint8_t val)
{
	switch (val)
	{
		// 0xFE
		case 0xFE:
			return "unknown";
			break;
		case 0:
			return "off";
			break;
		default:
			return "on";
			break;
	}
}

char* translate_switch_type(uint8_t type)
{
	switch (type)
	{
		// 0x00
		case SW_TYPE_NOT_SUP:
			return "SW_TYPE_NOT_SUP";
			break;
		// 0x01
		case SW_TYPE_OFF_ON:
			return "SW_TYPE_OFF_ON";
			break;
		// 0x02
		case SW_TYPE_DOWN_UP:
			return "SW_TYPE_DOWN_UP";
			break;
		// 0x03
		case SW_TYPE_CLOSE_OPEN:
			return "SW_TYPE_CLOSE_OPEN";
			break;
		// 0x04
		case SW_TYPE_COUNTER_CLOCKWISE:
			return "SW_TYPE_COUNTER_CLOCKWISE";
			break;
		// 0x05
		case SW_TYPE_LEFT_RIGHT:
			return "SW_TYPE_LEFT_RIGHT";
			break;
		// 0x06
		case SW_TYPE_REV_FWD:
			return "SW_TYPE_REV_FWD";
			break;
		// 0x07
		case SW_TYPE_PULL_PUSH:
			return "SW_TYPE_PULL_PUSH";
			break;
		default:
			return "SW_TYPE_XXX";
			break;
	}
}

char* translate_thermostat_mode(uint8_t mode)
{
	switch (mode)
	{
		// 0
		case ZW_THRMO_MD_OFF:
			return "ZW_THRMO_MD_OFF";
			break;
		// 1
		case ZW_THRMO_MD_HEAT:
			return "ZW_THRMO_MD_HEAT";
			break;
		// 2
		case ZW_THRMO_MD_COOL:
			return "ZW_THRMO_MD_COOL";
			break;
		// 3
		case ZW_THRMO_MD_AUTO:
			return "ZW_THRMO_MD_AUTO";
			break;
		// 4
		case ZW_THRMO_MD_AUX_HEAT:
			return "ZW_THRMO_MD_AUX_HEAT";
			break;
		// 5
		case ZW_THRMO_MD_RESUME:
			return "ZW_THRMO_MD_RESUME";
			break;
		// 6
		case ZW_THRMO_MD_FAN:
			return "ZW_THRMO_MD_FAN";
			break;
		// 7
		case ZW_THRMO_MD_FURNACE:
			return "ZW_THRMO_MD_FURNACE";
			break;
		// 8
		case ZW_THRMO_MD_DRY_AIR:
			return "ZW_THRMO_MD_DRY_AIR";
			break;
		// 9
		case ZW_THRMO_MD_MOIST_AIR:
			return "ZW_THRMO_MD_MOIST_AIR";
			break;
		// 10
		case ZW_THRMO_MD_AUTO_CHANGEOVER:
			return "ZW_THRMO_MD_AUTO_CHANGEOVER";
			break;
		// 11
		case ZW_THRMO_MD_ENE_SAVE_HEAT:
			return "ZW_THRMO_MD_ENE_SAVE_HEAT";
			break;
		// 12
		case ZW_THRMO_MD_ENE_SAVE_COOL:
			return "ZW_THRMO_MD_ENE_SAVE_COOL";
			break;
		// 13
		case ZW_THRMO_MD_AWAY:
			return "ZW_THRMO_MD_AWAY";
			break;
#ifdef SUPPORT_COMMAND_CLASS_THERMOSTAT_MODE
		// 0x0E
		case ZW_THRMO_MD_RESERVED_0X0E:
			return "ZW_THRMO_MD_RESERVED_0X0E";
			break;
		// 0x0F
		case ZW_THRMO_MD_FULL_POWER:
			return "ZW_THRMO_MD_FULL_POWER";
			break;
		// 0x10
		case ZW_THRMO_MD_RESERVED_0X10:
			return "ZW_THRMO_MD_RESERVED_0X10";
			break;
		// 0x11
		case ZW_THRMO_MD_RESERVED_0X11:
			return "ZW_THRMO_MD_RESERVED_0X11";
			break;
		// 0x12
		case ZW_THRMO_MD_RESERVED_0X12:
			return "ZW_THRMO_MD_RESERVED_0X12";
			break;
		// 0x13
		case ZW_THRMO_MD_RESERVED_0X13:
			return "ZW_THRMO_MD_RESERVED_0X13";
			break;
		// 0x14
		case ZW_THRMO_MD_RESERVED_0X14:
			return "ZW_THRMO_MD_RESERVED_0X14";
			break;
		// 0x15
		case ZW_THRMO_MD_RESERVED_0X15:
			return "ZW_THRMO_MD_RESERVED_0X15";
			break;
		// 0x16
		case ZW_THRMO_MD_RESERVED_0X16:
			return "ZW_THRMO_MD_RESERVED_0X16";
			break;
		// 0x17
		case ZW_THRMO_MD_RESERVED_0X17:
			return "ZW_THRMO_MD_RESERVED_0X17";
			break;
		// 0x18
		case ZW_THRMO_MD_RESERVED_0X18:
			return "ZW_THRMO_MD_RESERVED_0X18";
			break;
		// 0x19
		case ZW_THRMO_MD_RESERVED_0X19:
			return "ZW_THRMO_MD_RESERVED_0X19";
			break;
		// 0x1A
		case ZW_THRMO_MD_RESERVED_0X1A:
			return "ZW_THRMO_MD_RESERVED_0X1A";
			break;
		// 0x1B
		case ZW_THRMO_MD_RESERVED_0X1B:
			return "ZW_THRMO_MD_RESERVED_0X1B";
			break;
		// 0x1C
		case ZW_THRMO_MD_RESERVED_0X1C:
			return "ZW_THRMO_MD_RESERVED_0X1C";
			break;
		// 0x1D
		case ZW_THRMO_MD_RESERVED_0X1D:
			return "ZW_THRMO_MD_RESERVED_0X1D";
			break;
		// 0x1E
		case ZW_THRMO_MD_RESERVED_0X1E:
			return "ZW_THRMO_MD_RESERVED_0X1E";
			break;
		// 0x1F
		case ZW_THRMO_MD_MANUFACTURER_SPECIFC:
			return "ZW_THRMO_MD_MANUFACTURER_SPECIFC";
			break;
#endif
		default:
			return "ZW_THRMO_MD_XXX";
			break;
	}
}

char* translate_thermostat_op_state(uint8_t state)
{
	switch (state)
	{
		// 0
		case ZW_THRMO_OP_STA_IDLE:
			return "ZW_THRMO_OP_STA_IDLE";
			break;
		// 1
		case ZW_THRMO_OP_STA_HEAT:
			return "ZW_THRMO_OP_STA_HEAT";
			break;
		// 2
		case ZW_THRMO_OP_STA_COOL:
			return "ZW_THRMO_OP_STA_COOL";
			break;
		// 3
		case ZW_THRMO_OP_STA_FAN:
			return "ZW_THRMO_OP_STA_FAN";
			break;
		// 4
		case ZW_THRMO_OP_STA_PD_HEAT:
			return "ZW_THRMO_OP_STA_PD_HEAT";
			break;
		// 5
		case ZW_THRMO_OP_STA_PD_COOL:
			return "ZW_THRMO_OP_STA_PD_COOL";
			break;
		// 6
		case ZW_THRMO_OP_STA_VENT:
			return "ZW_THRMO_OP_STA_VENT";
			break;
#ifdef SUPPORT_COMMAND_CLASS_THERMOSTAT_OPERATING_STATE
		// 7
		case ZW_THRMO_OP_STA_AUX_HEATING:
			return "ZW_THRMO_OP_STA_AUX_HEATING";
			break;
		// 8
		case ZW_THRMO_OP_STA_2ND_STAGE_HEATING:
			return "ZW_THRMO_OP_STA_2ND_STAGE_HEATING";
			break;
		// 9
		case ZW_THRMO_OP_STA_2ND_STAGE_COOLING:
			return "ZW_THRMO_OP_STA_2ND_STAGE_COOLING";
			break;
		// 0x0A
		case ZW_THRMO_OP_STA_2ND_STAGE_AUX_HEAT:
			return "ZW_THRMO_OP_STA_2ND_STAGE_AUX_HEAT";
			break;
		// 0x0B
		case ZW_THRMO_OP_STA_3RD_STAGE_AUX_HEAT:
			return "ZW_THRMO_OP_STA_3RD_STAGE_AUX_HEAT";
			break;
#endif
		default:
			return "ZW_THRMO_OP_STA_XXX";
			break;
	}
}

char* translate_thermostat_setpoint_type(uint8_t type)
{
	switch (type)
	{
		// 1
		case ZW_THRMO_SETP_TYP_HEATING:
			return "ZW_THRMO_SETP_TYP_HEATING";
			break;
		// 2
		case ZW_THRMO_SETP_TYP_COOLING:
			return "ZW_THRMO_SETP_TYP_COOLING";
			break;
		// 7
		case ZW_THRMO_SETP_TYP_FURNACE:
			return "ZW_THRMO_SETP_TYP_FURNACE";
			break;
		// 8
		case ZW_THRMO_SETP_TYP_DRY:
			return "ZW_THRMO_SETP_TYP_DRY";
			break;
		// 9
		case ZW_THRMO_SETP_TYP_MOIST:
			return "ZW_THRMO_SETP_TYP_MOIST";
			break;
		// 10
		case ZW_THRMO_SETP_TYP_AUTO_CHANGEOVER:
			return "ZW_THRMO_SETP_TYP_AUTO_CHANGEOVER";
			break;
		// 11
		case ZW_THRMO_SETP_TYP_ENE_SAVE_HEAT:
			return "ZW_THRMO_SETP_TYP_ENE_SAVE_HEAT";
			break;
		// 12
		case ZW_THRMO_SETP_TYP_ENE_SAVE_COOL:
			return "ZW_THRMO_SETP_TYP_ENE_SAVE_COOL";
			break;
		// 13
		case ZW_THRMO_SETP_TYP_AWAY_HEAT:
			return "ZW_THRMO_SETP_TYP_AWAY_HEAT";
			break;
		// 14
		case ZW_THRMO_SETP_TYP_AWAY_COOL:
			return "ZW_THRMO_SETP_TYP_AWAY_COOL";
			break;
		// 15
		case ZW_THRMO_SETP_TYP_FULL_POWER:
			return "ZW_THRMO_SETP_TYP_FULL_POWER";
			break;
		default:
			return "ZW_THRMO_SETP_TYP_XXX";
			break;
	}
}

char* translate_thermostat_fan_mode(uint8_t mode)
{
	switch (mode)
	{
		// 0
		case ZW_THRMO_FAN_MD_AUTO_LO:
			return "ZW_THRMO_FAN_MD_AUTO_LO";
			break;
		// 1
		case ZW_THRMO_FAN_MD_LOW:
			return "ZW_THRMO_FAN_MD_LOW";
			break;
		// 2
		case ZW_THRMO_FAN_MD_AUTO_HI:
			return "ZW_THRMO_FAN_MD_AUTO_HI";
			break;
		// 3
		case ZW_THRMO_FAN_MD_HI:
			return "ZW_THRMO_FAN_MD_HI";
			break;
		// 4
		case ZW_THRMO_FAN_MD_AUTO_MID:
			return "ZW_THRMO_FAN_MD_AUTO_MID";
			break;
		// 5
		case ZW_THRMO_FAN_MD_MID:
			return "ZW_THRMO_FAN_MD_MID";
			break;
		// 6
		case ZW_THRMO_FAN_MD_CIR:
			return "ZW_THRMO_FAN_MD_CIR";
			break;
		// 7
		case ZW_THRMO_FAN_MD_HUM_CIR:
			return "ZW_THRMO_FAN_MD_HUM_CIR";
			break;
		// 8
		case ZW_THRMO_FAN_MD_LEFT_RIGHT:
			return "ZW_THRMO_FAN_MD_LEFT_RIGHT";
			break;
		// 9
		case ZW_THRMO_FAN_MD_UP_DOWN:
			return "ZW_THRMO_FAN_MD_UP_DOWN";
			break;
		// 10
		case ZW_THRMO_FAN_MD_QUIET:
			return "ZW_THRMO_FAN_MD_QUIET";
			break;
		default:
			return "ZW_THRMO_SETP_UNIT_XXX";
			break;
	}
}

char* translate_thermostat_fan_state(uint8_t state)
{
	switch (state)
	{
		// 0
		case ZW_THRMO_FAN_STA_IDLE:
			return "ZW_THRMO_FAN_STA_IDLE";
			break;
		// 1
		case ZW_THRMO_FAN_STA_LOW:
			return "ZW_THRMO_FAN_STA_LOW";
			break;
		// 2
		case ZW_THRMO_FAN_STA_HI:
			return "ZW_THRMO_FAN_STA_HI";
			break;
		// 3
		case ZW_THRMO_FAN_STA_MEDIUM:
			return "ZW_THRMO_FAN_STA_MEDIUM";
			break;
		// 4
		case ZW_THRMO_FAN_STA_CIR:
			return "ZW_THRMO_FAN_STA_CIR";
			break;
		// 5
		case ZW_THRMO_FAN_STA_CIR_HUMID:
			return "ZW_THRMO_FAN_STA_CIR_HUMID";
			break;
		// 6
		case ZW_THRMO_FAN_STA_CIR_RL:
			return "ZW_THRMO_FAN_STA_CIR_RL";
			break;
		// 7
		case ZW_THRMO_FAN_STA_CIR_UP_DN:
			return "ZW_THRMO_FAN_STA_CIR_UP_DN";
			break;
		// 8
		case ZW_THRMO_FAN_STA_CIR_QUIET:
			return "ZW_THRMO_FAN_STA_CIR_QUIET";
			break;
		default:
			return "ZW_THRMO_FAN_STA_XXX";
			break;
	}
}

char* translate_thermostat_setpoint_unit(uint8_t unit)
{
	switch (unit)
	{
		case ZW_THRMO_SETP_UNIT_C:
			return "ZW_THRMO_SETP_UNIT_C";
			break;
		case ZW_THRMO_SETP_UNIT_F:
			return "ZW_THRMO_SETP_UNIT_F";
			break;
		default:
			return "ZW_THRMO_SETP_UNIT_XXX";
			break;
	}
}

const char * translate_power_level(uint8_t lvl)
{
	const char *ret;

	switch (lvl)
	{
		case 1:
			ret = "-1dBm";
			break;
		case 2:
			ret = "-2dBm";
			break;
		case 3:
			ret = "-3dBm";
			break;
		case 4:
			ret = "-4dBm";
			break;
		case 5:
			ret = "-5dBm";
			break;
		case 6:
			ret = "-6dBm";
			break;
		case 7:
			ret = "-7dBm";
			break;
		case 8:
			ret = "-8dBm";
			break;
		case 9:
			ret = "-9dBm";
			break;
		case 0:
		default:
			ret = "NormalPower";
			break;
	}

	return ret;
}

#if (0)
const char * translate_power_level_power(uint8_t power)
{
	const char *ret;
	switch (power)
	{
		case POWERLEVEL_TEST_NODE_SET_NORMALPOWER:
			ret = "POWERLEVEL_TEST_NODE_SET_NORMALPOWER";
			break;
		case POWERLEVEL_TEST_NODE_SET_MINUS1DBM:
			ret = "POWERLEVEL_TEST_NODE_SET_MINUS1DBM";
			break;
		case POWERLEVEL_TEST_NODE_SET_MINUS2DBM:
			ret = "POWERLEVEL_TEST_NODE_SET_MINUS2DBM";
			break;
		case POWERLEVEL_TEST_NODE_SET_MINUS3DBM:
			ret = "POWERLEVEL_TEST_NODE_SET_MINUS3DBM";
			break;
		case POWERLEVEL_TEST_NODE_SET_MINUS4DBM:
			ret = "POWERLEVEL_TEST_NODE_SET_MINUS4DBM";
			break;
		case POWERLEVEL_TEST_NODE_SET_MINUS5DBM:
			ret = "POWERLEVEL_TEST_NODE_SET_MINUS5DBM";
			break;
		case POWERLEVEL_TEST_NODE_SET_MINUS6DBM:
			ret = "POWERLEVEL_TEST_NODE_SET_MINUS6DBM";
			break;
		case POWERLEVEL_TEST_NODE_SET_MINUS7DBM:
			ret = "POWERLEVEL_TEST_NODE_SET_MINUS7DBM";
			break;
		case POWERLEVEL_TEST_NODE_SET_MINUS8DBM:
			ret = "POWERLEVEL_TEST_NODE_SET_MINUS8DBM";
			break;
		case POWERLEVEL_TEST_NODE_SET_MINUS9DBM:
			ret = "POWERLEVEL_TEST_NODE_SET_MINUS9DBM";
			break;
		default:
			ret = "POWERLEVEL_TEST_NODE_SET_XXX";
			break;
	}
	return ret;
}
#endif

const char * translate_power_level_status(uint8_t status)
{
	const char *ret;

	switch (status)
	{
		//case POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_NOT_A_NODEID:
		//	ret = "POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_NOT_A_NODEID";
		//	break;
		case POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_FAILED:
			ret = "POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_FAILED";
			break;
		case POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_SUCCES:
			ret = "POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_SUCCES";
			break;
		case POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_INPROGRESS:
			ret = "POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_INPROGRESS";
			break;
		default:
			ret = "POWERLEVEL_TEST_NODE_REPORT_XXX";
			break;
	}
	return ret;
}

const char *translate_manufacturer(uint16_t vid)
{
	return "";
}

const char *translate_product(uint16_t vid, uint16_t type, uint16_t pid)
{
	return "";
}

#if (0)
const char * translate_controller_state_to_string(int state)
{
	const char *ret;

	switch (state)
	{
		case ZWNET_OP_ADD_NODE:
			ret = "adding";
			break;
		case ZWNET_OP_RESET:
			ret = "resetting";
			break;
		case ZWNET_OP_RM_NODE:
			ret = "removing";
			break;
		default:
			ret = "none";
			break;
	}

	return ret;
}
#endif

const char *translate_cc(uint16_t cls)
{
	const char *ret;

	switch (cls)
	{
		case COMMAND_CLASS_ALARM:
			ret = "CC_ALARM";
			break;
		case COMMAND_CLASS_APPLICATION_STATUS:
			ret = "CC_APPLICATION_STATUS";
			break;
		case COMMAND_CLASS_ASSOCIATION:
			ret = "CC_ASSOCIATION";
			break;
		case COMMAND_CLASS_ASSOCIATION_GRP_INFO:
			ret = "CC_ASSOCIATION_GRP_INFO";
			break;
		case COMMAND_CLASS_BASIC:
			ret = "CC_BASIC";
			break;
		case COMMAND_CLASS_BATTERY:
			ret = "CC_BATTERY";
			break;
		case COMMAND_CLASS_CENTRAL_SCENE:
			ret = "CC_CENTRAL_SCENE";
			break;
		case COMMAND_CLASS_CLOCK:
			ret = "CC_CLOCK";
			break;
		case COMMAND_CLASS_CONFIGURATION:
			ret = "CC_CONFIGURATION";
			break;
		case COMMAND_CLASS_CONTROLLER_REPLICATION:
			ret = "CC_CONTROLLER_REPLICATION";
			break;
		case COMMAND_CLASS_CRC_16_ENCAP:
			ret = "CC_CRC_16_ENCAP";
			break;
		case COMMAND_CLASS_DEVICE_RESET_LOCALLY:
			ret = "CC_DEVICE_RESET_LOCALLY";
			break;
		case COMMAND_CLASS_DOOR_LOCK:
			ret = "CC_DOOR_LOCK";
			break;
		case COMMAND_CLASS_DOOR_LOCK_LOGGING:
			ret = "CC_DOOR_LOCK_LOGGING";
			break;
		case COMMAND_CLASS_FIRMWARE_UPDATE_MD:
			ret = "CC_FIRMWARE_UPDATE_MD";
			break;
		case COMMAND_CLASS_INCLUSION_CONTROLLER:
			ret = "CC_INCLUSION_CONTROLLER";
			break;
		case COMMAND_CLASS_IP_ASSOCIATION:
			ret = "CC_IP_ASSOCIATION";
			break;
		case COMMAND_CLASS_MAILBOX:
			ret = "CC_MAILBOX";
			break;
		case COMMAND_CLASS_MANUFACTURER_SPECIFIC:
			ret = "CC_MANUFACTURER_SPECIFIC";
			break;
		case COMMAND_CLASS_METER:
			ret = "CC_METER";
			break;
		case COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2:
			ret = "CC_MULTI_CHANNEL_ASSOCIATION_V2";
			break;
		case COMMAND_CLASS_MULTI_CHANNEL_V2:
			ret = "CC_MULTI_CHANNEL_V2";
			break;
		case COMMAND_CLASS_MULTI_CMD:
			ret = "CC_MULTI_CMD";
			break;
		case COMMAND_CLASS_NETWORK_MANAGEMENT_BASIC:
			ret = "CC_CLASS_NETWORK_MANAGEMENT_BASIC";
			break;
		case COMMAND_CLASS_NETWORK_MANAGEMENT_INCLUSION:
			ret = "CC_NETWORK_MANAGEMENT_INCLUSION";
			break;
		case COMMAND_CLASS_NETWORK_MANAGEMENT_INSTALLATION_MAINTENANCE:
			ret = "CC_NETWORK_MANAGEMENT_INSTALLATION_MAINTENANCE";
			break;
		case COMMAND_CLASS_NETWORK_MANAGEMENT_PROXY:
			ret = "CC_CLASS_NETWORK_MANAGEMENT_PROXY";
			break;
		case COMMAND_CLASS_NODE_NAMING:
			ret = "CC_NODE_NAMING";
			break;
		case COMMAND_CLASS_POWERLEVEL:
			ret = "CC_POWERLEVEL";
			break;
		case COMMAND_CLASS_PROTECTION:
			ret = "CC_PROTECTION";
			break;
		case COMMAND_CLASS_SCENE_ACTIVATION:
			ret = "CC_SCENE_ACTIVATION";
			break;
		case COMMAND_CLASS_SCENE_ACTUATOR_CONF:
			ret = "CC_SCENE_ACTUATOR_CONF";
			break;
		case COMMAND_CLASS_SCHEDULE_ENTRY_LOCK:
			ret = "CC_SCHEDULE_ENTRY_LOCK";
			break;
		case COMMAND_CLASS_SECURITY:
			ret = "CC_SECURITY";
			break;
		case COMMAND_CLASS_SECURITY_2:
			ret = "CC_SECURITY_2";
			break;
		case COMMAND_CLASS_SENSOR_BINARY:
			ret = "CC_SENSOR_BINARY";
			break;
		case COMMAND_CLASS_SENSOR_MULTILEVEL:
			ret = "CC_SENSOR_MULTILEVEL";
			break;
		case COMMAND_CLASS_SIMPLE_AV_CONTROL:
			ret = "CC_SIMPLE_AV_CONTROL";
			break;
		case COMMAND_CLASS_SUPERVISION:
			ret = "CC_SUPERVISION";
			break;
		case COMMAND_CLASS_SWITCH_ALL:
			ret = "CC_SWITCH_ALL";
			break;
		case COMMAND_CLASS_SWITCH_BINARY:
			ret = "CC_SWITCH_BINARY";
			break;
		case COMMAND_CLASS_SWITCH_COLOR:
			ret = "CC_SWITCH_COLOR";
			break;
		case COMMAND_CLASS_SWITCH_MULTILEVEL:
			ret = "CC_SWITCH_MULTILEVEL";
			break;
		case COMMAND_CLASS_THERMOSTAT_FAN_MODE:
			ret = "CC_THERMOSTAT_FAN_MODE";
			break;
		case COMMAND_CLASS_THERMOSTAT_FAN_STATE:
			ret = "CC_THERMOSTAT_FAN_STATE";
			break;
		case COMMAND_CLASS_THERMOSTAT_MODE:
			ret = "CC_THERMOSTAT_MODE";
			break;
		case COMMAND_CLASS_THERMOSTAT_OPERATING_STATE:
			ret = "CC_THERMOSTAT_OPERATING_STATE";
			break;
		case COMMAND_CLASS_THERMOSTAT_SETPOINT:
			ret = "CC_THERMOSTAT_SETPOINT";
			break;
		case COMMAND_CLASS_TIME:
			ret = "CC_TIME";
			break;
		case COMMAND_CLASS_TIME_PARAMETERS:
			ret = "CC_TIME_PARAMETERS";
			break;
		case COMMAND_CLASS_TRANSPORT_SERVICE:
			ret = "CC_TRANSPORT_SERVICE";
			break;
		case COMMAND_CLASS_USER_CODE:
			ret = "CC_USER_CODE";
			break;
		case COMMAND_CLASS_VERSION:
			ret = "CC_VERSION";
			break;
		case COMMAND_CLASS_WAKE_UP:
			ret = "CC_WAKE_UP";
			break;
		case COMMAND_CLASS_ZIP:
			ret = "CC_CLASS_ZIP";
			break;
		case COMMAND_CLASS_ZIP_GATEWAY:
			ret = "CC_ZIP_GATEWAY";
			break;
		case COMMAND_CLASS_ZIP_NAMING:
			ret = "CC_ZIP_NAMING";
			break;
		case COMMAND_CLASS_ZIP_PORTAL:
			ret = "CC_ZIP_PORTAL";
			break;
		case COMMAND_CLASS_ZWAVEPLUS_INFO:
			ret = "CC_ZWAVEPLUS_INFO";
			break;
		default:
			ret = "CC_XXX";
			break;
	}

	return ret;
}

char *translate_CommandClass(int cc)
{
	return CommandClassArray[cc].name;
}

static void translate_cfg_CommandClass(void)
{
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_BASIC].name, "CC_BASIC"); // 0x20

	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_CONTROLLER_REPLICATION].name, "CC_CONTROLLER_REPLICATION"); // 0x21
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_APPLICATION_STATUS].name, "CC_CLASS_APPLICATION_STATUS"); // 0x22
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_ZIP].name, "CC_CLASS_ZIP"); // 0x23
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_SWITCH_BINARY].name, "CC_SWITCH_BINARY"); // 0x25

	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_SWITCH_MULTILEVEL].name, "CC_SWITCH_MULTILEVEL"); // 0x26

	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_SWITCH_ALL].name, "CC_SWITCH_ALL"); // 0x27
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_SCENE_ACTIVATION].name, "CC_SCENE_ACTIVATION"); // 0x2b
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_SCENE_ACTUATOR_CONF].name, "CC_SCENE_ACTUATOR_CONF"); // 0x2c

	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_SENSOR_BINARY].name, "CC_SENSOR_BINARY"); // 0x30
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_SENSOR_MULTILEVEL].name, "CC_SENSOR_MULTILEVEL"); // 0x31
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_METER].name, "CC_METER"); // 0x32
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_SWITCH_COLOR].name, "CC_SWITCH_COLOR"); // 0x33
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_NETWORK_MANAGEMENT_INCLUSION].name, "CC_NETWORK_MANAGEMENT_INCLUSION"); // 0x34

	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_THERMOSTAT_MODE].name, "CC_THERMOSTAT_MODE"); // 0x40
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_THERMOSTAT_OPERATING_STATE].name, "CC_THERMOSTAT_OPERATING_STATE"); // 0x42
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_THERMOSTAT_SETPOINT].name, "CC_THERMOSTAT_SETPOINT"); // 0x43
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_THERMOSTAT_FAN_MODE].name, "CC_THERMOSTAT_FAN_MODE"); // 0x44
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_THERMOSTAT_FAN_STATE].name, "CC_THERMOSTAT_FAN_STATE"); // 0x45
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_DOOR_LOCK_LOGGING].name, "CC_DOOR_LOCK_LOGGING"); // 0x4c
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_NETWORK_MANAGEMENT_BASIC].name, "CC_CLASS_NETWORK_MANAGEMENT_BASIC"); // 0x4d
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_SCHEDULE_ENTRY_LOCK].name, "CC_SCHEDULE_ENTRY_LOCK"); // 0x4e

	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_NETWORK_MANAGEMENT_PROXY].name, "CC_CLASS_NETWORK_MANAGEMENT_PROXY"); // 0x52
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_TRANSPORT_SERVICE].name, "CC_TRANSPORT_SERVICE"); // 0x55
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_CRC_16_ENCAP].name, "CC_CRC_16_ENCAP"); // 0x56
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_ASSOCIATION_GRP_INFO].name, "CC_ASSOCIATION_GRP_INFO"); // 0x59
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_DEVICE_RESET_LOCALLY].name, "CC_DEVICE_RESET_LOCALLY"); // 0x5a
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_CENTRAL_SCENE].name, "CC_CENTRAL_SCENE"); // 0x5b
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_IP_ASSOCIATION].name, "CC_IP_ASSOCIATION"); // 0x5c
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_ZWAVEPLUS_INFO].name, "CC_ZWAVEPLUS_INFO"); // 0x5e
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_ZIP_GATEWAY].name, "CC_ZIP_GATEWAY"); // 0x5f

	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_MULTI_CHANNEL_V2].name, "CC_MULTI_CHANNEL_V2"); // 0x60
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_ZIP_PORTAL].name, "CC_ZIP_PORTAL"); // 0x61
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_DOOR_LOCK].name, "CC_DOOR_LOCK"); // 0x62

	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_USER_CODE].name, "CC_USER_CODE"); // 0x63
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_NETWORK_MANAGEMENT_INSTALLATION_MAINTENANCE].name, "CC_NETWORK_MANAGEMENT_INSTALLATION_MAINTENANCE"); // 0x67
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_ZIP_NAMING].name, "CC_ZIP_NAMING"); // 0x68
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_MAILBOX].name, "CC_MAILBOX"); // 0x69
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_SUPERVISION].name, "CC_SUPERVISION"); // 0x6c

	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_CONFIGURATION].name, "CC_CONFIGURATION"); // 0x70
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_ALARM].name, "CC_ALARM"); // 0x71
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_MANUFACTURER_SPECIFIC].name, "CC_MANUFACTURER_SPECIFIC"); // 0x72

	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_POWERLEVEL].name, "CC_POWERLEVEL"); // 0x73
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_INCLUSION_CONTROLLER].name, "CC_INCLUSION_CONTROLLER"); // 0x74

	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_PROTECTION].name, "CC_PROTECTION"); // 0x75
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_NODE_NAMING].name, "CC_NODE_NAMING"); // 0x77
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_FIRMWARE_UPDATE_MD].name, "CC_FIRMWARE_UPDATE_MD"); // 0x7a

	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_BATTERY].name, "CC_BATTERY"); // 0x80
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_CLOCK].name, "CC_CLOCK"); // 0x81
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_WAKE_UP].name, "CC_WAKE_UP"); // 0x84
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_ASSOCIATION].name, "CC_ASSOCIATION"); // 0x85
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_VERSION].name, "CC_VERSION"); // 0x86
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_TIME].name, "CC_TIME"); // 0x8a
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_TIME_PARAMETERS].name, "CC_TIME_PARAMETERS"); // 0x8b
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2].name, "CC_MULTI_CHANNEL_ASSOCIATION_V2"); // 0x8e
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_MULTI_CMD].name, "CC_MULTI_CMD"); // 0x8f

	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_SIMPLE_AV_CONTROL].name, "CC_SIMPLE_AV_CONTROL"); // 0x94

	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_SECURITY].name, "CC_SECURITY"); // 0x98
	SAFE_SPRINTF_EX(CommandClassArray[COMMAND_CLASS_SECURITY_2].name, "CC_SECURITY_2"); // 0x9f

#ifdef DEVICE_ENABLE_POLL_TEST
	CommandClassArray[COMMAND_CLASS_BASIC].pollfn = zwif_basic_get_poll;
	CommandClassArray[COMMAND_CLASS_SWITCH_MULTILEVEL].pollfn = zwif_level_get_poll;
	CommandClassArray[COMMAND_CLASS_DOOR_LOCK].pollfn = zwif_dlck_op_get_poll;
#endif
}

static void translate_cfg_SensorName(void)
{
	// ** SensorName **
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TEMP].name, "ZW_SENSOR_TYPE_TEMP"); // 0x01
	SensorNameArray[ZW_SENSOR_TYPE_TEMP].report = JKEY_REPORT_TEMPERATURE;
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TEMP].unit[ZW_SENSOR_UNIT_TEMP_CEL], "Celsius"); // 0x01.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TEMP].unit[ZW_SENSOR_UNIT_TEMP_FAHR], "Fahrenheit"); // 0x01.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_GP].name, "ZW_SENSOR_TYPE_GP"); // 0x02
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_GP].unit[ZW_SENSOR_UNIT_GP_PERC], "%%"); // 0x02.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_GP].unit[ZW_SENSOR_UNIT_GP_NODIM], "ZW_SENSOR_UNIT_GP_NODIM"); // 0x02.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_LUM].name, "ZW_SENSOR_TYPE_LUM"); // 0x03
	SensorNameArray[ZW_SENSOR_TYPE_LUM].report = JKEY_REPORT_LUMINANCE;
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_LUM].unit[ZW_SENSOR_UNIT_LUM_PERC], "%%"); // 0x03.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_LUM].unit[ZW_SENSOR_UNIT_LUM_LUX], "Lux"); // 0x03.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_POW].name, "ZW_SENSOR_TYPE_POW"); // 0x04
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_POW].unit[ZW_SENSOR_UNIT_POW_W], "w"); // 0x04.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_POW].unit[ZW_SENSOR_UNIT_POW_BTU], "Btu/h"); // 0x04.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_HUMD].name, "ZW_SENSOR_TYPE_HUMD"); // 0x05
	SensorNameArray[ZW_SENSOR_TYPE_HUMD].report = JKEY_REPORT_HUMIDITY;
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_HUMD].unit[ZW_SENSOR_UNIT_HUMD_PERC], "%%"); // 0x05.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_HUMD].unit[ZW_SENSOR_UNIT_HUMD_ABS], "g/m3"); // 0x05.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_VELO].name, "ZW_SENSOR_TYPE_VELO"); // 0x06
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_VELO].unit[ZW_SENSOR_UNIT_VELO_MS], "m/s"); // 0x06.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_VELO].unit[ZW_SENSOR_UNIT_VELO_MPH], "mph"); // 0x06.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_DIR].name, "ZW_SENSOR_TYPE_DIR"); // 0x07
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_DIR].unit[ZW_SENSOR_UNIT_DIR_DEG], "degrees"); // 0x07.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ATM].name, "ZW_SENSOR_TYPE_ATM"); // 0x08
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ATM].unit[ZW_SENSOR_UNIT_ATM_KPA], "kPa"); // 0x08.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ATM].unit[ZW_SENSOR_UNIT_ATM_HG], "inches of Mercury"); // 0x08.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_BARO].name, "ZW_SENSOR_TYPE_BARO"); // 0x09
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_BARO].unit[ZW_SENSOR_UNIT_BARO_KPA], "kPa"); // 0x09.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_BARO].unit[ZW_SENSOR_UNIT_BARO_HG], "inches of Mercury"); // 0x09.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SLR].name, "ZW_SENSOR_TYPE_SLR"); // 0x0A
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SLR].unit[ZW_SENSOR_UNIT_SLR_WM2], "W/m2"); // 0x0A.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_DEW].name, "ZW_SENSOR_TYPE_DEW"); // 0x0B
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_DEW].unit[ZW_SENSOR_UNIT_DEW_CEL], "Celsius"); // 0x0B.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_DEW].unit[ZW_SENSOR_UNIT_DEW_FAHR], "Fahrenheit"); // 0x0B.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_RAIN].name, "ZW_SENSOR_TYPE_RAIN"); // 0x0C
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_RAIN].unit[ZW_SENSOR_UNIT_RAIN_MMH], "mm/h"); // 0x0C.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_RAIN].unit[ZW_SENSOR_UNIT_RAIN_INH], "in/h"); // 0x0C.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TIDE].name, "ZW_SENSOR_TYPE_TIDE"); // 0x0D
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TIDE].unit[ZW_SENSOR_UNIT_TIDE_M], "m"); // 0x0D.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TIDE].unit[ZW_SENSOR_UNIT_TIDE_FT], "feet"); // 0x0D.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_WGT].name, "ZW_SENSOR_TYPE_WGT"); // 0x0E
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_WGT].unit[ZW_SENSOR_UNIT_WGT_KG], "kg"); // 0x0E.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_WGT].unit[ZW_SENSOR_UNIT_WGT_LBS], "pounds"); // 0x0E.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_VOLT].name, "ZW_SENSOR_TYPE_VOLT"); // 0x0F
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_VOLT].unit[ZW_SENSOR_UNIT_VOLT_V], "V"); // 0x0F.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_VOLT].unit[ZW_SENSOR_UNIT_VOLT_MV], "mV"); // 0x0F.0x01

	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_CUR].name, "ZW_SENSOR_TYPE_CUR"); // 0x10
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_CUR].unit[ZW_SENSOR_UNIT_CUR_A], "A"); // 0x10.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_CUR].unit[ZW_SENSOR_UNIT_CUR_MA], "mA"); // 0x10.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_CO2].name, "ZW_SENSOR_TYPE_CO2"); // 0x11
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_CO2].unit[ZW_SENSOR_UNIT_CO2_PPM], "ppm"); // 0x11.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_AIR].name, "ZW_SENSOR_TYPE_AIR"); // 0x12
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_AIR].unit[ZW_SENSOR_UNIT_AIR_M3H], "m3/h"); // 0x12.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_AIR].unit[ZW_SENSOR_UNIT_AIR_CFM], "cfm"); // 0x12.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TANK].name, "ZW_SENSOR_TYPE_TANK"); // 0x13
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TANK].unit[ZW_SENSOR_UNIT_TANK_L], "l"); // 0x13.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TANK].unit[ZW_SENSOR_UNIT_TANK_CBM], "cbm"); // 0x13.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TANK].unit[ZW_SENSOR_UNIT_TANK_GAL], "US gallons"); // 0x13.0x02
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_DIST].name, "ZW_SENSOR_TYPE_DIST"); // 0x14
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_DIST].unit[ZW_SENSOR_UNIT_DIST_M], "m"); // 0x14.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_DIST].unit[ZW_SENSOR_UNIT_DIST_CM], "cm"); // 0x14.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_DIST].unit[ZW_SENSOR_UNIT_DIST_FT], "feet"); // 0x14.0x02
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_AGL].name, "ZW_SENSOR_TYPE_AGL"); // 0x15
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_AGL].unit[ZW_SENSOR_UNIT_AGL_PERC], "%%"); // 0x15.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_AGL].unit[ZW_SENSOR_UNIT_AGL_DEGN], "degrees"); // 0x15.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_AGL].unit[ZW_SENSOR_UNIT_AGL_DEGS], "degrees"); // 0x15.0x02
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ROT].name, "ZW_SENSOR_TYPE_ROT"); // 0x16
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ROT].unit[ZW_SENSOR_UNIT_ROT_RPM], "rpm"); // 0x16.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ROT].unit[ZW_SENSOR_UNIT_ROT_HZ], "Hz"); // 0x16.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_WTR_TEMP].name, "ZW_SENSOR_TYPE_WTR_TEMP"); // 0x17
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_WTR_TEMP].unit[ZW_SENSOR_UNIT_WTR_TEMP_CEL], "Celsius"); // 0x17.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_WTR_TEMP].unit[ZW_SENSOR_UNIT_WTR_TEMP_FAHR], "Fahrenheit"); // 0x17.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SOIL_TEMP].name, "ZW_SENSOR_TYPE_SOIL_TEMP"); // 0x18
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SOIL_TEMP].unit[ZW_SENSOR_UNIT_SOIL_TEMP_CEL], "Celsius"); // 0x18.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SOIL_TEMP].unit[ZW_SENSOR_UNIT_SOIL_TEMP_FAHR], "Fahrenheit"); // 0x18.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SEIS_INT].name, "ZW_SENSOR_TYPE_SEIS_INT"); // 0x19
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SEIS_INT].unit[ZW_SENSOR_UNIT_SEIS_INT_MERC], "Mercalli"); // 0x19.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SEIS_INT].unit[ZW_SENSOR_UNIT_SEIS_INT_EMCRO], "European Macroseismic"); // 0x19.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SEIS_INT].unit[ZW_SENSOR_UNIT_SEIS_INT_LIEDU], "Liedu"); // 0x19.0x02
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SEIS_INT].unit[ZW_SENSOR_UNIT_SEIS_INT_SHNDO], "Shindo"); // 0x19.0x03
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SEIS_MAG].name, "ZW_SENSOR_TYPE_SEIS_MAG"); // 0x1A
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SEIS_MAG].unit[ZW_SENSOR_UNIT_SEIS_MAG_LOCAL], "Local (ML)"); // 0x1A.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SEIS_MAG].unit[ZW_SENSOR_UNIT_SEIS_MAG_MOM], "Moment (MW)"); // 0x1A.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SEIS_MAG].unit[ZW_SENSOR_UNIT_SEIS_MAG_SWAVE], "Surface wave (MS)"); // 0x1A.0x02
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SEIS_MAG].unit[ZW_SENSOR_UNIT_SEIS_MAG_BWAVE], "Body wave (MB"); // 0x1A.0x03
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_UV].name, "ZW_SENSOR_TYPE_UV"); // 0x1B
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_UV].unit[ZW_SENSOR_UNIT_UV_INDEX], "UV index"); // 0x1B.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ELEC_RES].name, "ZW_SENSOR_TYPE_ELEC_RES"); // 0x1C
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ELEC_RES].unit[ZW_SENSOR_UNIT_ELEC_RES_OHMM], "ohm metre"); // 0x1C.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ELEC_COND].name, "ZW_SENSOR_TYPE_ELEC_COND"); // 0x1D
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ELEC_COND].unit[ZW_SENSOR_UNIT_ELEC_COND_SIEM], "siemens per metre (S/m)"); // 0x1D.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_LOUDNESS].name, "ZW_SENSOR_TYPE_LOUDNESS"); // 0x1E
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_LOUDNESS].unit[ZW_SENSOR_UNIT_LOUDNESS_ABS], "Absolute loudness (dB)"); // 0x1E.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_LOUDNESS].unit[ZW_SENSOR_UNIT_LOUDNESS_A_WT], "A-weighted decibels (dBA)"); // 0x1E.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_MOIST].name, "ZW_SENSOR_TYPE_MOIST"); // 0x1F
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_MOIST].unit[ZW_SENSOR_UNIT_MOIST_PERC], "%%"); // 0x1F.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_MOIST].unit[ZW_SENSOR_UNIT_MOIST_VOL_WTR], "Volume water content (m3/m3)"); // 0x1F.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_MOIST].unit[ZW_SENSOR_UNIT_MOIST_IMPD], "Impedance (k ohm)"); // 0x1F.0x02
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_MOIST].unit[ZW_SENSOR_UNIT_MOIST_WTR_ACT], "Water activity (aw)"); // 0x1F.0x03

	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_FREQ].name, "ZW_SENSOR_TYPE_FREQ"); // 0x20
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_FREQ].unit[ZW_SENSOR_UNIT_FREQ_HZ], "Hz"); // 0x20.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_FREQ].unit[ZW_SENSOR_UNIT_FREQ_KHZ], "kHz"); // 0x20.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TIME].name, "ZW_SENSOR_TYPE_TIME"); // 0x21
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TIME].unit[ZW_SENSOR_UNIT_TIME_SEC], "seconds"); // 0x21.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TGT_TEMP].name, "ZW_SENSOR_TYPE_TGT_TEMP"); // 0x22
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TGT_TEMP].unit[ZW_SENSOR_UNIT_TGT_TEMP_CEL], "Celsius"); // 0x22.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TGT_TEMP].unit[ZW_SENSOR_UNIT_TGT_TEMP_FAHR], "Fahrenheit"); // 0x22.0x01

	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_PM_2_5].name, "ZW_SENSOR_TYPE_PM_2_5"); // 0x23
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_PM_2_5].unit[ZW_SENSOR_UNIT_PM_2_5_MM3], "mol/m3"); // 0x23.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_PM_2_5].unit[ZW_SENSOR_UNIT_PM_2_5_UGM3], "ug/m3"); // 0x23.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_F_CH2O].name, "ZW_SENSOR_TYPE_F_CH2O"); // 0x24
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_F_CH2O].unit[ZW_SENSOR_UNIT_F_CH2O_MM3], "mol/m3"); // 0x24.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_RAD_CONT].name, "ZW_SENSOR_TYPE_RAD_CONT"); // 0x25
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_RAD_CONT].unit[ZW_SENSOR_UNIT_RAD_CONT_BQM3], "bq/m3"); // 0x25.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_RAD_CONT].unit[ZW_SENSOR_UNIT_RAD_CONT_PCIL], "pCi/L"); // 0x25.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_METH_DENS].name, "ZW_SENSOR_TYPE_METH_DENS"); // 0x26
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_METH_DENS].unit[ZW_SENSOR_UNIT_METH_DENS_MM3], "mol/m3"); // 0x26.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_VOC].name, "ZW_SENSOR_TYPE_VOC"); // 0x27
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_VOC].unit[ZW_SENSOR_UNIT_VOC_MM3], "mol/m3"); // 0x27.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_CO_LVL].name, "ZW_SENSOR_TYPE_CO_LVL"); // 0x28
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_CO_LVL].unit[ZW_SENSOR_UNIT_CO_LVL_MM3], "mol/m3"); // 0x28.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SOIL_HUMD].name, "ZW_SENSOR_TYPE_SOIL_HUMD"); // 0x29
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SOIL_HUMD].unit[ZW_SENSOR_UNIT_CO_LVL_MM3], "%%"); // 0x29.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SOIL_REAC].name, "ZW_SENSOR_TYPE_SOIL_REAC"); // 0x2A
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SOIL_REAC].unit[ZW_SENSOR_UNIT_SOIL_REAC_PH], "pH"); // 0x2A.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SOIL_SAL].name, "ZW_SENSOR_TYPE_SOIL_SAL"); // 0x2B
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SOIL_SAL].unit[ZW_SENSOR_UNIT_SOIL_SAL_MM3], "mol/m3"); // 0x2B.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_HEART_RT].name, "ZW_SENSOR_TYPE_HEART_RT"); // 0x2C
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_HEART_RT].unit[ZW_SENSOR_UNIT_HEART_RT_BPM], "Bpm"); // 0x2C.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_BLOOD_PRS].name, "ZW_SENSOR_TYPE_BLOOD_PRS"); // 0x2D
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_BLOOD_PRS].unit[ZW_SENSOR_UNIT_BLOOD_PRS_SYS], "mmHg"); // 0x2D.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_BLOOD_PRS].unit[ZW_SENSOR_UNIT_BLOOD_PRS_DIA], "mmHg"); // 0x2D.0x01
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_MUSCLE_MS].name, "ZW_SENSOR_TYPE_MUSCLE_MS"); // 0x2E
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_MUSCLE_MS].unit[ZW_SENSOR_UNIT_MUSCLE_MS_KG], "kg"); // 0x2E.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_FAT_MS].name, "ZW_SENSOR_TYPE_FAT_MS"); // 0x2F
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_FAT_MS].unit[ZW_SENSOR_UNIT_FAT_MS_KG], "kg"); // 0x2F.0x00

	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_BONE_MS].name, "ZW_SENSOR_TYPE_BONE_MS"); // 0x30
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_BONE_MS].unit[ZW_SENSOR_UNIT_BONE_MS_KG], "kg"); // 0x30.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TBW].name, "ZW_SENSOR_TYPE_TBW"); // 0x31
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_TBW].unit[ZW_SENSOR_UNIT_TBW_KG], "kg"); // 0x31.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_BMR].name, "ZW_SENSOR_TYPE_BMR"); // 0x32
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_BMR].unit[ZW_SENSOR_UNIT_BMR_J], "J"); // 0x32.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_BMI].name, "ZW_SENSOR_TYPE_BMI"); // 0x33
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_BMI].unit[ZW_SENSOR_UNIT_BMI_IDX], "idx"); // 0x33.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ACCEL_X].name, "ZW_SENSOR_TYPE_ACCEL_X"); // 0x34
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ACCEL_X].unit[ZW_SENSOR_UNIT_ACCEL_X_MS2], "m/s2"); // 0x34.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ACCEL_Y].name, "ZW_SENSOR_TYPE_ACCEL_Y"); // 0x35
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ACCEL_Y].unit[ZW_SENSOR_UNIT_ACCEL_Z_MS2], "m/s2"); // 0x35.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ACCEL_Z].name, "ZW_SENSOR_TYPE_ACCEL_Z"); // 0x36
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_ACCEL_Z].unit[ZW_SENSOR_UNIT_ACCEL_Z_MS2], "m/s2"); // 0x36.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SMOKE_DEN].name, "ZW_SENSOR_TYPE_SMOKE_DEN"); // 0x37
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_SMOKE_DEN].unit[ZW_SENSOR_UNIT_SMOKE_DEN_PERC], "%%"); // 0x37.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_WATER_FLW].name, "ZW_SENSOR_TYPE_WATER_FLW"); // 0x38
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_WATER_FLW].unit[ZW_SENSOR_UNIT_WATER_FLW_LHR], "(l/h"); // 0x38.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_WATER_PRS].name, "ZW_SENSOR_TYPE_WATER_PRS"); // 0x39
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_WATER_PRS].unit[ZW_SENSOR_UNIT_WATER_PRS_KPA], "kPa"); // 0x39.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_RF_SGN].name, "ZW_SENSOR_TYPE_RF_SGN"); // 0x3A
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_RF_SGN].unit[ZW_SENSOR_UNIT_RF_SGN_RSSI], "%%"); // 0x3A.0x00
	SAFE_SPRINTF_EX(SensorNameArray[ZW_SENSOR_TYPE_RF_SGN].unit[ZW_SENSOR_UNIT_RF_SGN_DBM], "dBm"); // 0x3A.0x00
}

void translater_open(void)
{
#if (0)
	int i = 0;
	for (i=0; i< LEN_OF_2BYTES_DEFINE; i++)
	{
		//CommandClassArray[i].id = i;
		SAFE_SPRINTF_EX(CommandClassArray[i].name, "XXX");
	}

	for (i=0; i< LEN_OF_1BYTE_DEFINE; i++)
	{
		//SensorNameArray[i].id = i;
		SAFE_SPRINTF_EX(SensorNameArray[i].name, "ZW_SENSOR_TYPE_XXX");
		SensorNameArray[i].report = JKEY_REPORT_XXX;
		int j = 0;
		for (j=0; j< 0xF; j++)
		{
			SAFE_SPRINTF_EX(SensorNameArray[i].unit[j], "UNIT_XXX");
		}
	}
#endif

	// ** COMMAND_CLASS **
	translate_cfg_CommandClass();

	// ** SensorName **
	translate_cfg_SensorName();
}

char* translate_ctl_capabilities(uint8_t ctl_cap)
{
	static char ctl_capabilities_name[128] = "";

	memset(ctl_capabilities_name, 0, sizeof(ctl_capabilities_name));
	if (ctl_cap & ZWNET_CTLR_CAP_S2)
	{
		strcat(ctl_capabilities_name, "ZWNET_CTLR_CAP_S2 (0x01)/");
	}
	if (ctl_cap & ZWNET_CTLR_CAP_INC_ON_BEHALF)
	{
		strcat(ctl_capabilities_name, "ZWNET_CTLR_CAP_INC_ON_BEHALF (0x02)/");
	}

	if (ctl_capabilities_name[strlen(ctl_capabilities_name)-1] =='/')
	{
		ctl_capabilities_name[strlen(ctl_capabilities_name)-1] = '\0';
	}
	return ctl_capabilities_name;
}

char* translate_ctl_zw_role(uint8_t ctl_zw_role)
{
	switch (ctl_zw_role)
	{
		// 0
		case ZW_ROLE_UNKNOWN:
			return "ZW_ROLE_UNKNOWN";
			break;
		// 1
		case ZW_ROLE_SIS:
			return "ZW_ROLE_SIS";
			break;
		// 2
		case ZW_ROLE_INCLUSION:
			return "ZW_ROLE_INCLUSION";
			break;
		// 3
		case ZW_ROLE_PRIMARY:
			return "ZW_ROLE_PRIMARY";
			break;
		// 4
		case ZW_ROLE_SECONDARY:
			return "ZW_ROLE_SECONDARY";
			break;
		default:
			return "ZW_ROLE_XXX";
			break;
	}
}

char* translate_postset_reason(int reason)
{
	switch (reason)
	{
		case ZWPSET_REASON_TGT_HIT:
			return "ZWPSET_REASON_TGT_HIT";
			break;
		case ZWPSET_REASON_TIMEOUT:
			return "ZWPSET_REASON_TIMEOUT";
			break;
		case ZWPSET_REASON_UNSUPPORTED:
			return "ZWPSET_REASON_UNSUPPORTED";
			break;
		case ZWPSET_REASON_DEVICE_RMV:
			return "ZWPSET_REASON_DEVICE_RMV";
			break;
		case ZWPSET_REASON_BUSY:
			return "ZWPSET_REASON_BUSY";
			break;
		case ZWPSET_REASON_SEND_FAILED:
			return "ZWPSET_REASON_SEND_FAILED";
			break;
		case ZWPSET_REASON_FAILED:
			return "ZWPSET_REASON_FAILED";
			break;
		default:
			return "ZWPSET_REASON_XXX";
			break;
	}
}

char* translate_node_propty(uint8_t propty, uint8_t id)
{
	static char propty_name[128] = "";

	memset(propty_name, 0, sizeof(propty_name));
	if (propty & NODE_PROPTY_SECURE_CAP_S0)
	{
		if (id)
		{
			strcat(propty_name, "S0 (0x01)/");
		}
		else
		{
			strcat(propty_name, "S0/");
		}
	}
	if (propty & NODE_PROPTY_SECURE_CAP_S2)
	{
		if (id)
		{
			strcat(propty_name, "S2 (0x02)/");
		}
		else
		{
			strcat(propty_name, "S2/");
		}
	}
	if (propty & NODE_PROPTY_ADD_SECURE)
	{
		if (id)
		{
			strcat(propty_name, "Security (0x04)/");
		}
		else
		{
			strcat(propty_name, "Security/");
		}
	}
	if (propty & NODE_PROPTY_ADD_INSECURE)
	{
		if (id)
		{
			strcat(propty_name, "Non-Security (0x08)/");
		}
		else
		{
			strcat(propty_name, "Non-Security/");
		}
	}

	if (propty_name[strlen(propty_name)-1] =='/')
	{
		propty_name[strlen(propty_name)-1] = '\0';
	}
	return propty_name;
}

char* translate_requested_keys(uint8_t req_keys, uint8_t id)
{
	static char req_keys_name[128] = "";

	memset(req_keys_name, 0, sizeof(req_keys_name));
	if (req_keys & SEC_KEY_BITMSK_S2_K2)
	{
		if (id)
		{
			strcat(req_keys_name, "S2_K2 (0x04)/");
		}
		else
		{
			strcat(req_keys_name, "S2_K2/");
		}
	}
	if (req_keys & SEC_KEY_BITMSK_S2_K1)
	{
		if (id)
		{
			strcat(req_keys_name, "S2_K1 (0x02)/");
		}
		else
		{
			strcat(req_keys_name, "S2_K1/");
		}
	}
	if (req_keys & SEC_KEY_BITMSK_S2_K0)
	{
		if (id)
		{
			strcat(req_keys_name, "S2_K0 (0x01)/");
		}
		else
		{
			strcat(req_keys_name, "S2_K0/");
		}
	}
	if (req_keys & SEC_KEY_BITMSK_S0)
	{
		if (id)
		{
			strcat(req_keys_name, "S0 (0x80)/");
		}
		else
		{
			strcat(req_keys_name, "S0/");
		}
	}

	strcat(req_keys_name, "Non-Security");
	//if ( req_keys_name[strlen(req_keys_name)-1] =='/' )
	//	req_keys_name[strlen(req_keys_name)-1] = '\0';

	return req_keys_name;
}

char* translate_infrared_state(uint8_t state)
{
	switch (state)
	{
		case 9: // TIQIAA_STATE_SEND
			return "SEND";
			break;
		case 19: // TIQIAA_STATE_RECV
			return "RECV";
			break;
		case 3: // TIQIAA_STATE_IDLE
		default:
			return "IDLE";
			break;
	}
}

