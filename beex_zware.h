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
#ifndef __BEEX_ZWARE_H__
#define __BEEX_ZWARE_H__

#ifdef __cplusplus
extern "C"
{
#endif

//******************************************************************************
//** define **
//******************************************************************************
// 112
#ifndef _ZIP_API_PTE_DAVID_
struct _zwnet;
typedef struct _zwnet	*zwnet_p;///<Opaque network handle
#endif

// 121
typedef struct _zwnoded	    *zwnoded_p;		/**<Node descriptor */

/**
@defgroup Net Network APIs
A Z-Wave Network is seen through a local controller node which can manage
control and data in the mesh network.
@ingroup zwarecapi
@{
*/


/** @name Zw_error_codes
* Z-Wave error code definition
@{
*/
#define ZW_ERR_CACHE_AVAIL        2    /**< The data is available in the cache (no live get from the target device) */
#define ZW_ERR_QUEUED			  1	   /**< The command is queued (no transmission status callback) */
#define ZW_ERR_NONE				  0    /**< Operation succeeded */
#define ZW_ERR_FAILED			 -1	   /**< Operation failed */
#define ZW_ERR_WRONG_IF         -10    /**< Wrong interface */
#define ZW_ERR_NO_RESP          -11    /**< No response from controller */
#define ZW_ERR_MEMORY           -12    /**< Out of memory */
#define ZW_ERR_NODE_NOT_FOUND   -13    /**< Node not found*/
#define ZW_ERR_CLASS_NOT_FOUND  -14    /**< Command class not found*/
#define ZW_ERR_INTF_NOT_FOUND   -15    /**< Interface to a class not found*/
#define ZW_ERR_INTF_NO_REP_HDLR -16    /**< Interface report handler not found*/
#define ZW_ERR_LAST_OP_NOT_DONE -17    /**< Last operation uncompleted yet, try again*/
#define ZW_ERR_NOT_IN_LIST      -18    /**< Node not in protocol layer failed node ID list*/
#define ZW_ERR_OP_FAILED        -19    /**< The requested operation failed*/
#define ZW_ERR_EP_NOT_FOUND     -20    /**< Endpoint not found*/
#define ZW_ERR_RPT_NOT_FOUND    -21    /**< The report command of an interface not found*/
#define ZW_ERR_NET_NOT_FOUND    -22    /**< Network not found*/
#define ZW_ERR_CMD_VERSION      -23    /**< Incorrect command class version*/
#define ZW_ERR_PENDING			-24	   /**< Operation pending, it cannot be canceled now */
#define ZW_ERR_VALUE			-25	   /**< The value is invalid */
#define ZW_ERR_QUEUE_FULL		-26	   /**< The queue is full */
#define ZW_ERR_UNSUPPORTED		-27	   /**< The requested function is unsupported for this node */
#define ZW_ERR_FILE_OPEN	    -28	   /**< Open file error */
#define ZW_ERR_FILE_WRITE	    -29	   /**< Write file error */
#define ZW_ERR_FILE_EOF	        -30	   /**< The end-of-file was reached*/
#define ZW_ERR_FILE	            -31	   /**< File is corrupted */
#define ZW_ERR_FILE_HOME_ID     -32	   /**< File home id doesn't match */
#define ZW_ERR_EXPIRED          -33    /**< Expired */
#define ZW_ERR_NO_RES           -34    /**< No resource for mutex, semaphore, timer, etc */
#define ZW_ERR_EVENT            -35    /**< Event is not processed */
#define ZW_ERR_TOO_LARGE        -36    /**< Data size is too large */
#define ZW_ERR_TOO_SMALL        -37    /**< Data size is too small */
#define ZW_ERR_TIMEOUT          -38    /**< Timeout */
#define ZW_ERR_TRANSMIT         -39    /**< Transmission failed */
#define ZW_ERR_NONCE_NOT_FOUND  -40    /**< Security nonce not found */
#define ZW_ERR_AUTH             -41    /**< Authentication error */
#define ZW_ERR_SEQ_NUMBER       -42    /**< Incorrect sequence number */
#define ZW_ERR_BUSY             -43    /**< Busy, try again later */
#define ZW_ERR_SEC_SCHEME       -44    /**< Security scheme unsupported */
#define ZW_ERR_TRANSPORT_INI    -45    /**< Initialization error on transport layer */
#define ZW_ERR_FRAME_INI        -46    /**< Initialization error on frame layer */
#define ZW_ERR_SESSION_INI      -47    /**< Initialization error on session layer */
#define ZW_ERR_APPL_INI         -48    /**< Initialization error on application layer */
#define ZW_ERR_UNEXPECTED       -49    /**< The error was unexpected under normal circumstances*/
#define ZW_ERR_NETWORK_IF       -50    /**< Network interface not configured properly */
#define ZW_ERR_IP_ADDR			-51	   /**< The IP address is invalid */
#define ZW_ERR_VERSION          -52    /**< Wrong version number*/
#define ZW_ERR_INTF_NO_DATA     -53    /**< Interface data is missing*/
#define ZW_ERR_FILE_READ	    -54	   /**< Read file error */
#define ZW_ERR_PARSE_FILE	    -55	   /**< Parsing file failed */
#define ZW_ERR_MISSING_ENTRY	-56	   /**< Missing mandatory entry */
#define ZW_ERR_DEVCFG_NOT_FOUND	-57	   /**< Device specific configuration record not found */
#define ZW_ERR_DISALLOWED       -58    /**< The operation is disallowed under certain circumstances*/
#define ZW_ERR_PSK_TOO_SHORT    -59    /**< DTLS pre-shared key length is too short*/
#define ZW_ERR_NO_CACHE_AVAIL   -60    /**< The data is unavailable in the cache */
#define ZW_ERR_NOT_APPLICABLE   -61    /**< Not applicable and should be skipped or ignored */
#define ZW_ERR_SHUTDOWN         -62    /**< The system is shutting down */
#define ZW_ERR_POST_SET_POLL    -63    /**< Post-set poll error */
#define ZW_ERR_ORDER            -64    /**< Out of order */
#define ZW_ERR_CMD_UNSUPP_TGT   -65    /**< Command sent by the association group is unsupported at target node or endpoint */

// 348
/** @name Device_categories
* Device Categories definition
@{
*/
#define DEV_CATEGORY_UNKNOWN        0	   /**< Unknown or unassigned category */
#define DEV_SENSOR_ALARM            1	   /**< Sensor alarm */
#define DEV_ON_OFF_SWITCH           2	   /**< On/off switch */
#define DEV_POWER_STRIP             3	   /**< Power strip */
#define DEV_SIREN                   4	   /**< Siren */
#define DEV_VALVE                   5	   /**< Valve */
#define DEV_SIMPLE_DISPLAY          6	   /**< Simple display */
#define DEV_DOORLOCK_KEYPAD         7	   /**< Door lock with keypad */
#define DEV_SUB_ENERGY_METER        8	   /**< Sub energy meter */
#define DEV_ADV_WHL_HOME_ENER_METER 9	   /**< Advanced whole home energy meter */
#define DEV_SIM_WHL_HOME_ENER_METER 10	   /**< Simple whole home energy meter */
#define DEV_SENSOR                  11	   /**< Sensor */
#define DEV_LIGHT_DIMMER            12	   /**< Light dimmer switch */
#define DEV_WIN_COVERING_NO_POS     13	   /**< Window covering no position/endpoint */
#define DEV_WIN_COVERING_EP         14	   /**< Window covering end point aware */
#define DEV_WIN_COVERING_POS_EP     15	   /**< Window covering position/end point aware */
#define DEV_FAN_SWITCH              16	   /**< Fan switch */
#define DEV_RMT_CTL_MULTIPURPOSE    17	   /**< Remote control - multipurpose */
#define DEV_RMT_CTL_AV              18	   /**< Remote control - AV */
#define DEV_RMT_CTL_SIMPLE          19	   /**< Remote control - simple */
#define DEV_UNRECOG_GATEWAY         20	   /**< Gateway (unrecognized by client) */
#define DEV_CENTRAL_CTLR            21	   /**< Central controller */
#define DEV_SET_TOP_BOX             22	   /**< Set top box */
#define DEV_TV                      23	   /**< TV */
#define DEV_SUB_SYS_CTLR            24	   /**< Sub system controller */
#define DEV_GATEWAY                 25	   /**< Gateway */
#define DEV_THERMOSTAT_HVAC         26	   /**< Thermostat - HVAC */
#define DEV_THERMOSTAT_SETBACK      27	   /**< Thermostat - setback */
#define DEV_WALL_CTLR               28	   /**< Wall controller */
/**
@}
*/


/** Z/IP network management definition */
#define NODE_ADD_STATUS_SECURITY_FAILED     9   /**< Node has been included but the secure inclusion failed.*/


/** @name Network_operation
* Network operation definition
@{
*/
#define ZWNET_OP_NONE           0   /**< No operation is executing*/
#define ZWNET_OP_INITIALIZE     1   /**< Initialization operation*/
#define ZWNET_OP_ADD_NODE       2   /**< Add node operation*/
#define ZWNET_OP_RM_NODE        3   /**< Remove node operation*/
#define ZWNET_OP_RP_NODE        4   /**< Replace failed node operation*/
#define ZWNET_OP_RM_FAILED_ID   5   /**< Remove failed node id operation*/
#define ZWNET_OP_INITIATE       6   /**< Initiation operation by controller*/
#define ZWNET_OP_UPDATE         7   /**< Update network topology from the SUC/SIS*/
#define ZWNET_OP_RESET          8   /**< Restore to factory default setting*/
#define ZWNET_OP_MIGRATE_SUC    9   /**< Create primary controller by a SUC*/
#define ZWNET_OP_MIGRATE        10  /**< Migrate primary controller operation*/
#define ZWNET_OP_ASSIGN         11  /**< assign or deassign SUC/SIS operation*/
//#define ZWNET_OP_LOAD_NW_INFO   12  /**< Load detailed network and node info operation*/
#define ZWNET_OP_NODE_UPDATE    13  /**< Update node info*/
#define ZWNET_OP_SEND_NIF       14  /**< Send node info frame*/
#define ZWNET_OP_NW_CHANGED     15  /**< Network change detection*/
#define ZWNET_OP_NODE_CACHE_UPT 16  /**< Update node cache info. (For internal use only)*/
#define ZWNET_OP_SAVE_NW        17  /**< Save network and node information to persistent storage. (For internal use only)*/
#define ZWNET_OP_SLEEP_NODE_UPT 18  /**< Update sleeping detailed node information when it is awake. (For internal use only)*/
#define ZWNET_OP_FW_UPDT        19  /**< Firmware update*/
#define ZWNET_OP_HEALTH_CHK     20  /**< Network health check*/

/**
@}
*/

/** Final progress status for all operations */
#define     OP_DONE                     0       /**< Operation completed*/
#define     OP_FAILED                   0x00FF  /**< Operation failed*/

/** Progress status of all getting node information */
#define     OP_GET_NI_TOTAL_NODE_MASK   0xFF00  /**< Mask to get the total number of nodes*/
#define     OP_GET_NI_NODE_CMPLT_MASK   0x00FF  /**< Mask to get the number of completed nodes*/

/** Progress status of adding node and migrating primary controller */
#define     OP_ADD_NODE_PROTOCOL_DONE   1   /**< Protocol part of adding node done*/
#define     OP_ADD_NODE_GET_NODE_INFO   2   /**< Getting node detailed information*/
#define     OP_ADD_NODE_PROTOCOL_START  3   /**< Smart Start add node Z-wave protocol started*/

#ifdef DEVICE_INTERVIEW_NOTIFY
typedef enum
{
	OP_ADD_NODE_ZWNET_STA_GET_GW_DSK=5,
	OP_ADD_NODE_ZWNET_STA_GET_NODE_INFO,

	OP_ADD_NODE_ZWNET_STA_ASSIGN_RR=10,
	OP_ADD_NODE_ZWNET_STA_GET_VER_CC,
	OP_ADD_NODE_ZWNET_STA_GET_ZWPLUS_INFO,
	OP_ADD_NODE_ZWNET_STA_GET_VER,
	OP_ADD_NODE_ZWNET_STA_GET_MANF_PDT_ID,
	OP_ADD_NODE_ZWNET_STA_WKUP_CFG,
	OP_ADD_NODE_ZWNET_STA_MULTI_INSTANCE,
	OP_ADD_NODE_ZWNET_STA_MULTI_CHANNEL,

	OP_ADD_NODE_ACT_END_POINT,
	OP_ADD_NODE_ZWNET_STA_GET_EP_INFO,

	OP_ADD_NODE_S_EP_STA_XXX=31,

#ifdef DEVICE_INTERVIEW_ORDER
	OP_ADD_NODE_S_EP_STA_GET_MAX_GROUP,
	OP_ADD_NODE_S_EP_STA_SET_GROUP_LIFELINE,
	OP_ADD_NODE_S_EP_STA_GET_GROUP_MEMBER,
	OP_ADD_NODE_S_EP_STA_SET_CFG_PARAM,
#else
	OP_ADD_NODE_S_EP_STA_SET_CFG_PARAM,
	OP_ADD_NODE_S_EP_STA_GET_MAX_GROUP,
	OP_ADD_NODE_S_EP_STA_SET_GROUP_LIFELINE,
	OP_ADD_NODE_S_EP_STA_GET_GROUP_MEMBER,
#endif
	OP_ADD_NODE_S_EP_STA_GET_CSC,

	OP_ADD_NODE_S_EP_STA_GET_ZWPLUS_INFO,
	OP_ADD_NODE_S_EP_STA_GET_SENSOR,

	OP_ADD_NODE_S_EP_STA_GET_GROUP_INFO,

	OP_ADD_NODE_S_EP_STA_GET_THRMO_FAN_MD,
	OP_ADD_NODE_S_EP_STA_GET_THRMO_MD,
	OP_ADD_NODE_S_EP_STA_GET_THRMO_SETP,

	OP_ADD_NODE_S_EP_STA_GET_MUL_SWITCH,

	OP_ADD_NODE_S_EP_STA_GET_ALARM,

	OP_ADD_NODE_S_EP_STA_GET_USERCODE,

	OP_ADD_NODE_S_EP_STA_GET_METER,
	OP_ADD_NODE_S_EP_STA_GET_METER_DESC,

	OP_ADD_NODE_S_EP_STA_GET_BIN_SENSOR,
	OP_ADD_NODE_S_EP_STA_GET_DOORLOCK,

	OP_ADD_NODE_S_EP_STA_GET_THRMO_OP,
	OP_ADD_NODE_S_EP_STA_GET_SWITCH,
	OP_ADD_NODE_S_EP_STA_GET_BATTERY,
} OP_ADD_NODE_NOTIFY;
#endif

/** Progress status of replacing failed node */
#define     OP_RP_NODE_PROTOCOL_DONE    1   /**< Protocol part of replacing node done*/
#define     OP_RP_NODE_GET_NODE_INFO    2   /**< Getting node detailed information*/

/** Progress status of initiating */
#define     OP_INI_PROTOCOL_DONE        1   /**< Protocol part of initiating done*/
#define     OP_INI_GET_NODE_INFO        2   /**< Getting node detailed information*/

/** Progress status of network update */
#define     OP_NU_TOPOLOGY              1   /**< Network topology update started*/
#define     OP_NU_NEIGHBOR              2   /**< Node neighbor update started*/
#define     OP_NU_GET_NODE_INFO         3   /**< Node information update started*/

/** Progress status of firmware update */
#define     OP_FW_UPLOAD_STARTED        1   /**< Uploading firmware to device started */
#define     OP_FW_UPLOADING             2   /**< Uploading firmware to device in progress with additional info,
                                                 see ZWNET_STS_INFO_FW_UPDT_PRG */

/** Progress status of network health check */
#define     OP_HEALTH_CHK_STARTED       1   /**< Network health check started */
#define     OP_HEALTH_CHK_PROGRESS      2   /**< Network health check in progress with additional info,
                                                 see ZWNET_STS_INFO_HEALTH_CHK_PRG*/
#define     OP_HEALTH_CHK_CMPLT         3   /**< Network health check completed with additional info,
                                                 see ZWNET_STS_INFO_HEALTH_CHK_RPT*/

/** Controller role bit-mask*/
#define ZWNET_CTLR_ROLE_PROXY           0x01   /**< Support Network Management Proxy*/
#define ZWNET_CTLR_ROLE_INCL            0x02   /**< Support Network Management Inclusion*/
#define ZWNET_CTLR_ROLE_PRI             0x04   /**< Support Network Management Primary*/

/** Controller capabilities bit-mask*/
#define ZWNET_CTLR_CAP_S2               0x01   /**< Support Security 2 protocol */
#define ZWNET_CTLR_CAP_INC_ON_BEHALF    0x02   /**< Support inclusion on-behalf */
#define ZWNET_CTLR_CAP_SMART_START      0x04   /**< Support Smart Start */
#define ZWNET_CTLR_CAP_IMA              0x08   /**< Support IMA (Z-Wave Network Installation and maintenance) */

/** Controller Z-wave role*/
#define ZW_ROLE_UNKNOWN         0   /**< Unknown */
#define ZW_ROLE_SIS             1   /**< SIS*/
#define ZW_ROLE_INCLUSION       2   /**< Inclusion*/
#define ZW_ROLE_PRIMARY         3   /**< Primary*/
#define ZW_ROLE_SECONDARY       4   /**< Secondary*/

/** Network descriptor */
typedef struct
{
	uint32_t	    id;			/**< home id */
	uint8_t	        ctl_id;		/**< Z/IP controller node id*/
	uint8_t	        ctl_role;   /**< Z/IP controller role. A bit-mask of ZWNET_CTLR_ROLE_XXX */
	uint8_t	        ctl_cap;    /**< Z/IP controller capabilities. A bit-mask of ZWNET_CTLR_CAP_XXX */
	uint8_t	        ctl_zw_role;/**< Z/IP controller Z-wave role, ZW_ROLE_XXX */
	void            *user;      /**< User specified information that was passed to zwnet_init() */
	void            *plt_ctx;   /**< Platform context for printing of output text messages*/
}
zwnetd_t, *zwnetd_p;

// 562
#ifndef MAX_DSK_STR_LEN
#define MAX_DSK_STR_LEN      47      /**< Maximum DSK string length */
#endif

// 798
#ifdef DEVICE_ASSOCIATION_SAVE
#define IF_REC_MAX_MEMBER_OF_GROUP 8
#endif
#define IF_REC_MAX_GROUPS       8   /**< Maximum number of groups for the controller to set its node id into the groups*/

/** Group record */
typedef struct
{
	uint8_t     grp_cnt;                  /**< Number of group id in the grp_id array*/
	uint8_t     grp_id[IF_REC_MAX_GROUPS];/**< Group id for the controller to set its node id into the group*/

} if_rec_grp_t;

/** @name Central Scene Key attributes
* Central Scene Key attributes definition
@{
*/
#define ZW_CSC_KEY_ATTRIB_KEY_PRESSED_1_TIME	0x00    /**< A key is pressed and released before time out */
#define ZW_CSC_KEY_ATTRIB_KEY_RELEASED			0x01    /**< A key is released. Termination of a Key Held Down sequence.*/
#define ZW_CSC_KEY_ATTRIB_KEY_HELDDOWN			0x02    /**< a key is pressed and not released before time out
															Event used to signal continuation of key held down operation.*/
#define ZW_CSC_KEY_ATTRIB_KEY_PRESSED_2_TIME	0x03    /**< A key is pressed twice and no more key presses follow */
#define ZW_CSC_KEY_ATTRIB_KEY_PRESSED_3_TIME	0x04    /**< A key is pressed 3 times  and no more key presses follow */
#define ZW_CSC_KEY_ATTRIB_KEY_PRESSED_4_TIME	0x05    /**< A key is pressed 4 times  and no more key presses follow */
#define ZW_CSC_KEY_ATTRIB_KEY_PRESSED_5_TIME	0x06    /**< A key is pressed 5 times  and no more key presses follow */
/**
@}
*/

/** @name Bin_sensor_type
* Binary sensor Type definition
@{
*/
#define ZW_BSENSOR_TYPE_GP       0x01    /**< General purpose sensor*/
#define ZW_BSENSOR_TYPE_SMOKE    0x02    /**< Smoke sensor*/
#define ZW_BSENSOR_TYPE_CO       0x03    /**< CO sensor*/
#define ZW_BSENSOR_TYPE_CO2      0x04    /**< CO2 sensor*/
#define ZW_BSENSOR_TYPE_HEAT     0x05    /**< Heat sensor*/
#define ZW_BSENSOR_TYPE_WATER    0x06    /**< Water sensor*/
#define ZW_BSENSOR_TYPE_FREEZE   0x07    /**< Freeze sensor*/
#define ZW_BSENSOR_TYPE_TAMPER   0x08    /**< Tamper sensor*/
#define ZW_BSENSOR_TYPE_AUX      0x09    /**< Aux sensor*/
#define ZW_BSENSOR_TYPE_DR_WIN   0x0A    /**< Door/Window sensor*/
#define ZW_BSENSOR_TYPE_TILT     0x0B    /**< Tilt sensor*/
#define ZW_BSENSOR_TYPE_MOTION   0x0C    /**< Motion sensor*/
#define ZW_BSENSOR_TYPE_GLASS_BRK 0x0D    /**< Glass break sensor*/

#define ZW_BSENSOR_TYPE_1ST_SUP  0xFF    /**<  Return first sensor type on supported list*/
/**
@}
*/

// 1256
/** @name Security_key_bitmask
* Security key bitmask
@{
*/
#define SEC_KEY_BITMSK_S2_K0     0x01    /**< S2: class key 0 (Unauthenticated)*/
#define SEC_KEY_BITMSK_S2_K1     0x02    /**< S2: class key 1 (Authenticated)*/
#define SEC_KEY_BITMSK_S2_K2     0x04    /**< S2: class key 2 (Access Control)*/
#define SEC_KEY_BITMSK_S0        0x80    /**< S0: legacy security network key (Unauthenticated)*/
#define SEC_KEY_ALL_S2  (SEC_KEY_BITMSK_S2_K0 | SEC_KEY_BITMSK_S2_K1 | SEC_KEY_BITMSK_S2_K2)    /**< All S2 keys*/
#define SEC_KEY_ALL_S2_SO       (SEC_KEY_ALL_S2 | SEC_KEY_BITMSK_S0)        /**< All security keys*/

// 1480
#define ZWNET_NODE_STS_SLEEP      2   ///< Node is sleeping
#define ZWNET_NODE_STS_DOWN       1   ///< Node is down
#define ZWNET_NODE_STS_UP         0   ///< Node is alive

// 1700
/**
@defgroup If_SPoll Post-set Polling Interface APIs
Used to create and delete post-set polling commands to a device
@ingroup zwarecapi
@{
*/
#define ZWPSET_REASON_TGT_HIT       0   ///< Set target was hit
#define ZWPSET_REASON_TIMEOUT       1   ///< Timeout, set target was not hit
#define ZWPSET_REASON_UNSUPPORTED   2   ///< Post-set polling is unsupported for this interface
#define ZWPSET_REASON_DEVICE_RMV    3   ///< Device removed
#define ZWPSET_REASON_BUSY          4   ///< Device is busy.  Try again the zwif_xxx_set() function call later.
#define ZWPSET_REASON_SEND_FAILED   5   ///< Failed to send command.
#define ZWPSET_REASON_FAILED        6   ///< Failed to hit the target for some reason.

// 1744
#define MAX_DEV_ID_LEN      31      /**< Maximum device id length */

// 1762
/** device id */
typedef struct
{
	uint8_t   type;                         ///< device id type: DEV_ID_TYPE_XXX
	uint8_t   format;                       ///< device id data format: DEV_ID_FMT_XXX
	uint8_t   len;                          ///< device id length
	uint8_t   dev_id[MAX_DEV_ID_LEN + 1];   ///< device id

} dev_id_t;

/**
@}
@defgroup Node Node APIs
Nodes have certain attributes indicating its role and location in the network
and endpoints which indicate its utility to the user
@ingroup zwarecapi
@{
*/

/** Extended version information */
typedef struct
{
	uint8_t		hw_ver;		    /**< Hardware version*/
	uint8_t		fw_cnt;		    /**< Number of firmwares in the device, excluding the Z-wave firmware*/
	uint16_t    rsvd;           /**< Padding to make sure fw_ver is 4-byte aligned */
	uint16_t	fw_ver[1];		/**< Firmware versions place holder as indicated in fw_cnt*/
}
ext_ver_t;

/** Node property bit-mask definitions */
#define NODE_PROPTY_SECURE_CAP_S0   0x01    /**< Node capable to be included securely S0*/
#define NODE_PROPTY_SECURE_CAP_S2   0x02    /**< Node capable to be included securely S2*/
#define NODE_PROPTY_ADD_SECURE      0x04    /**< Node is included securely*/
#define NODE_PROPTY_ADD_INSECURE    0x08    /**< Node is included insecurely*/


/** Node Descriptor - read only for client */
typedef struct  _zwnoded
{
	uint8_t		nodeid;			/**< Node ID */
	uint8_t		propty;	        /**< Properties of the node (bit-mask): NODE_PROPTY_XXX */
	uint16_t	vid;			/**< Vendor ID */
	uint16_t	type;			/**< Vendor Product Type */
	uint16_t	pid;			/**< Product ID */
	zwnet_p     net;            /**< Network that the node belongs to */
	dev_id_t    dev_id;         /**< Device ID */
	uint16_t	proto_ver;		/**< Z-Wave Protocol Version*/
	uint16_t	app_ver;		/**< Application Version*/
	uint8_t		lib_type;		/**< Z-Wave Library Type*/
	uint8_t     category;	    /**< Device category, DEV_XXX */
	uint8_t		sensor;		    /**< Flag to indicate whether the node is a sensor (FLIRS) */
	uint8_t     sleep_cap;      /**< Flag to indicate the node is capable to sleep  (i.e. non-listening and support Wake up command class) */
	uint8_t     listen;         /**< Flag to indicate the node is always listening */
	int32_t     wkup_intv;      /**< Wake up interval in seconds. Negative value = invalid or unknown */
	uint8_t		s2_keys_valid;  /**< Flag to indicate whether s2_grnt_keys is valid */
	uint8_t		s2_grnt_keys;   /**< Security 2 granted keys (bit-mask), see SEC_KEY_BITMSK_XXX
                                     NOTE: This is valid only s2_keys_valid = 1*/
	char		s2_dsk[MAX_DSK_STR_LEN + 1];     /**< S2 DSK. If s2_dsk[0] == '\0', the DSK is unavailable for this node */

#ifdef DEVICE_BASIC_CLASS_SHOW
	uint8_t basic; /**< Basic Device Class */
#endif
}
zwnoded_t;

// 1956
/** Z-wave+ info */
typedef struct
{
	uint8_t		zwplus_ver;		/**< Z-Wave+ version. Zero indicates this node is non-Z-Wave+ */
	uint8_t		node_type;		/**< Z-Wave+ node type*/
	uint8_t		role_type;		/**< Z-Wave+ role type*/
	uint16_t	instr_icon;		/**< Installer icon type */
	uint16_t	usr_icon;		/**< User icon type */

}
zwplus_info_t;

// 2032
/**
@}
@defgroup If Interface APIs
Interface with functionalities that correspond to Z-wave command class
@ingroup zwarecapi
@{
*/

/** interface property bit-mask definitions */
#define IF_PROPTY_SECURE            0x01    /**< Interface can be access securely bit-mask*/
#define IF_PROPTY_UNSECURE          0x02    /**< Interface can be access unsecurely bit-mask*/
#define IF_PROPTY_ALRM_EVT_CLR      0x04    /**< Interface is capable to sent event clear notification. This bit-mask
                                                 is only valid for alarm/notification command class */
#define IF_PROPTY_BSNSR_EVT_CLR     0x04    /**< Interface is capable to sent event clear. This bit-mask
                                                 is only valid for binary sensor command class */
#define IF_PROPTY_ALRM_SNSR_EVT_CLR 0x04    /**< Interface is capable to sent event clear. This bit-mask
                                                 is only valid for alarm sensor command class */
#define IF_PROPTY_HIDDEN_POLL       0x40    /**< Interface is hidden but device polling is enabled. NOTE: This property is for internal use only,
                                                 user application MUST NOT use this.*/
#define IF_PROPTY_HIDDEN            0x80    /**< Interface is hidden. NOTE: This property is for internal use only,
                                                 user application MUST NOT use this.*/


// 2154
/** Group member */
typedef struct
{
	uint8_t     node_id;    /**< node id*/
	uint8_t     ep_id;      /**< endpoint id*/
} grp_member_t;

// 2457
/**
@}
@defgroup If_Wku Wake Up Interface APIs
Wake up APIs are for battery powered device that sleep most of the time
@ingroup zwarecapi
@{
*/

/** wake up interval settings in seconds */
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

// 2509
/**
@}
@defgroup If_Swt Switch Interface APIs
Switches can be switched on/off
@ingroup zwarecapi
@{
*/

/** Switch data */
typedef struct
{
	uint8_t curr_val;  	/**< current value: 0 = off; 0xFE = Unknown; 0xFF = On. */
	uint8_t tgt_val;	/**< target value with same interpretation as curr_val */
	uint8_t dur;	    /**< Duration: 0 = already at the target; 0x01~0x7F = seconds; 0x80~0xFD = 1~126 minutes;
                                       0xFE = Unknown duration; 0xFF = reserved*/
}
zwswitch_t, *zwswitch_p;

// 2570
/**
@}
@defgroup If_Lvl Level Interface APIs
Levels can be set and auto-(in/de)cremented with start/stop
@ingroup zwarecapi
@{
*/

/** Multilevel switch data */
typedef struct
{
	uint8_t curr_val;  	/**< current value: 0 = off; 0x01~0x63 = percentage (%);0xFE = Unknown; 0xFF = On. */
	uint8_t tgt_val;	/**< target value with same interpretation as curr_val */
	uint8_t dur;	    /**< Duration: 0 = already at the target; 0x01~0x7F = seconds; 0x80~0xFD = 1~126 minutes;
                                       0xFE = Unknown duration; 0xFF = reserved*/
}
zwlevel_dat_t, *zwlevel_dat_p;

// 2655
/** @name Level_switch_type
* Switch type definition
@{
*/
#define    SW_TYPE_NOT_SUP	        0x00	/**< Not present or supported */
#define    SW_TYPE_OFF_ON	        0x01	/**< When level =0 the switch is off,  level=0x63/0xFF the switch is on*/
#define    SW_TYPE_DOWN_UP	        0x02	/**< When level =0 the switch is down,  level=0x63/0xFF the switch is up*/
#define    SW_TYPE_CLOSE_OPEN	    0x03	/**< When level =0 the switch is close,  level=0x63/0xFF the switch is open*/
#define    SW_TYPE_COUNTER_CLOCKWISE 0x04	/**< When level =0 the switch is turning counter-clockwise,
                                                 level=0x63/0xFF the switch is turning clockwise*/
#define    SW_TYPE_LEFT_RIGHT	    0x05	/**< When level =0 the switch is left,  level=0x63/0xFF the switch is right*/
#define    SW_TYPE_REV_FWD	        0x06	/**< When level =0 the switch is reverse,  level=0x63/0xFF the switch is forward*/
#define    SW_TYPE_PULL_PUSH	    0x07	/**< When level =0 the switch is pulled,  level=0x63/0xFF the switch is pushed*/
/**
@}
*/
// 2917
/** @name Sensor_type
* Sensor Type definition
@{
*/
#define ZW_SENSOR_TYPE_TEMP      0x01    /**< Temperature sensor*/
#define ZW_SENSOR_TYPE_GP        0x02    /**< General purpose sensor*/
#define ZW_SENSOR_TYPE_LUM       0x03    /**< Luminance sensor*/
#define ZW_SENSOR_TYPE_POW       0x04    /**< Power sensor*/
#define ZW_SENSOR_TYPE_HUMD      0x05    /**< Relative humidity sensor*/
#define ZW_SENSOR_TYPE_VELO      0x06    /**< Velocity sensor*/
#define ZW_SENSOR_TYPE_DIR       0x07    /**< Direction sensor*/
#define ZW_SENSOR_TYPE_ATM       0x08    /**< Atmospheric pressure sensor*/
#define ZW_SENSOR_TYPE_BARO      0x09    /**< Barometric pressure sensor*/
#define ZW_SENSOR_TYPE_SLR       0x0A    /**< Solar radiation sensor*/
#define ZW_SENSOR_TYPE_DEW       0x0B    /**< Dew point sensor*/
#define ZW_SENSOR_TYPE_RAIN      0x0C    /**< Rain rate sensor*/
#define ZW_SENSOR_TYPE_TIDE      0x0D    /**< Tide level sensor*/
#define ZW_SENSOR_TYPE_WGT       0x0E    /**< Weight sensor*/
#define ZW_SENSOR_TYPE_VOLT      0x0F    /**< Voltage sensor*/
#define ZW_SENSOR_TYPE_CUR       0x10    /**< Current sensor*/
#define ZW_SENSOR_TYPE_CO2       0x11    /**< CO2-level sensor*/
#define ZW_SENSOR_TYPE_AIR       0x12    /**< Air flow sensor*/
#define ZW_SENSOR_TYPE_TANK      0x13    /**< Tank capacity sensor*/
#define ZW_SENSOR_TYPE_DIST      0x14    /**< Distance sensor*/
#define ZW_SENSOR_TYPE_AGL       0x15    /**< Angle Position sensor*/
#define ZW_SENSOR_TYPE_ROT       0x16    /**< Rotation sensor*/
#define ZW_SENSOR_TYPE_WTR_TEMP  0x17    /**< Water temperature sensor*/
#define ZW_SENSOR_TYPE_SOIL_TEMP 0x18    /**< Soil temperature sensor*/
#define ZW_SENSOR_TYPE_SEIS_INT  0x19    /**< Seismic intensity sensor*/
#define ZW_SENSOR_TYPE_SEIS_MAG  0x1A    /**< Seismic magnitude sensor*/
#define ZW_SENSOR_TYPE_UV        0x1B    /**< Ultraviolet sensor*/
#define ZW_SENSOR_TYPE_ELEC_RES  0x1C    /**< Electrical resistivity sensor*/
#define ZW_SENSOR_TYPE_ELEC_COND 0x1D    /**< Electrical conductivity sensor*/
#define ZW_SENSOR_TYPE_LOUDNESS  0x1E    /**< Loudness sensor*/
#define ZW_SENSOR_TYPE_MOIST     0x1F    /**< Moisture sensor*/
#define ZW_SENSOR_TYPE_FREQ      0x20    /**< Frequency sensor*/
#define ZW_SENSOR_TYPE_TIME      0x21    /**< Time sensor*/
#define ZW_SENSOR_TYPE_TGT_TEMP  0x22    /**< Target temperature sensor*/
#define ZW_SENSOR_TYPE_PM_2_5    0x23    /**< Particulate matter 2.5 sensor*/
#define ZW_SENSOR_TYPE_F_CH2O    0x24    /**< Formaldehyde CH2O-level sensor*/
#define ZW_SENSOR_TYPE_RAD_CONT  0x25    /**< Radon Concentration sensor*/
#define ZW_SENSOR_TYPE_METH_DENS 0x26    /**< Methane Density CH4 sensor*/
#define ZW_SENSOR_TYPE_VOC       0x27    /**< Volatile Organic Compound sensor*/
#define ZW_SENSOR_TYPE_CO_LVL    0x28    /**< Carbon Monoxide CO-level sensor*/
#define ZW_SENSOR_TYPE_SOIL_HUMD 0x29    /**< Soil Humidity sensor*/
#define ZW_SENSOR_TYPE_SOIL_REAC 0x2A    /**< Soil Reactivity sensor*/
#define ZW_SENSOR_TYPE_SOIL_SAL  0x2B    /**< Soil Salinity sensor*/
#define ZW_SENSOR_TYPE_HEART_RT  0x2C    /**< Heart Rate sensor*/
#define ZW_SENSOR_TYPE_BLOOD_PRS 0x2D    /**< Blood Pressure sensor*/
#define ZW_SENSOR_TYPE_MUSCLE_MS 0x2E    /**< Muscle Mass sensor*/
#define ZW_SENSOR_TYPE_FAT_MS    0x2F    /**< Fat Mass sensor*/
#define ZW_SENSOR_TYPE_BONE_MS   0x30    /**< Bone Mass sensor*/
#define ZW_SENSOR_TYPE_TBW       0x31    /**< Total Body Water sensor*/
#define ZW_SENSOR_TYPE_BMR       0x32    /**< Basic Metabolic Rate sensor*/
#define ZW_SENSOR_TYPE_BMI       0x33    /**< Body Mass Index sensor*/
#define ZW_SENSOR_TYPE_ACCEL_X   0x34    /**< Acceleration, X-axis sensor*/
#define ZW_SENSOR_TYPE_ACCEL_Y   0x35    /**< Acceleration, Y-axis sensor*/
#define ZW_SENSOR_TYPE_ACCEL_Z   0x36    /**< Acceleration, Z-axis sensor*/
#define ZW_SENSOR_TYPE_SMOKE_DEN 0x37    /**< Smoke Density sensor*/
#define ZW_SENSOR_TYPE_WATER_FLW 0x38    /**< Water Flow sensor*/
#define ZW_SENSOR_TYPE_WATER_PRS 0x39    /**< Water Pressure sensor*/
#define ZW_SENSOR_TYPE_RF_SGN    0x3A    /**< RF Signal Strength sensor*/
#define ZW_SENSOR_TYPE_PM_10     0x3B    /**< Particulate Matter 10 sensor*/
#define ZW_SENSOR_TYPE_RESPI_RATE       0x3C    /**< Respiratory rate sensor*/
#define ZW_SENSOR_TYPE_REL_MOD          0x3D    /**< Relative Modulation level sensor*/
#define ZW_SENSOR_TYPE_BOILER_WTR_TEMP  0x3E    /**< Boiler water temperature sensor*/
#define ZW_SENSOR_TYPE_DHW_TEMP         0x3F    /**< Domestic Hot Water (DHW) temperature sensor*/
#define ZW_SENSOR_TYPE_OUTSIDE_TEMP     0x40    /**< Outside temperature sensor*/
#define ZW_SENSOR_TYPE_EXHAUST_TEMP     0x41    /**< Exhaust temperature sensor*/
#define ZW_SENSOR_TYPE_WATER_CHLOR_LVL  0x42    /**< Water Chlorine level sensor*/
#define ZW_SENSOR_TYPE_WATER_ACID       0x43    /**< Water acidity sensor*/
#define ZW_SENSOR_TYPE_WATER_OXI_RED    0x44    /**< Water Oxidation reduction potential sensor*/

///@}

/** @name Sensor_unit
* Sensor Unit definition
@{
*/
#define ZW_SENSOR_UNIT_TEMP_CEL       0x00    /**< Air temperature unit: Celsius (C)*/
#define ZW_SENSOR_UNIT_TEMP_FAHR      0x01    /**< Air temperature unit: Fahrenheit (F)*/

#define ZW_SENSOR_UNIT_GP_PERC        0x00    /**< General purpose unit: Percentage value*/
#define ZW_SENSOR_UNIT_GP_NODIM       0x01    /**< General purpose unit: Dimensionless value*/

#define ZW_SENSOR_UNIT_LUM_PERC       0x00    /**< Luminance unit: Percentage value*/
#define ZW_SENSOR_UNIT_LUM_LUX        0x01    /**< Luminance unit: Lux*/

#define ZW_SENSOR_UNIT_POW_W          0x00    /**< Power unit: W*/
#define ZW_SENSOR_UNIT_POW_BTU        0x01    /**< Power unit: Btu/h*/

#define ZW_SENSOR_UNIT_HUMD_PERC      0x00    /**< Relative humidity unit: Percentage value*/
#define ZW_SENSOR_UNIT_HUMD_ABS       0x01    /**< Absolute humidity unit: (g/m3) */

#define ZW_SENSOR_UNIT_VELO_MS        0x00    /**< Velocity unit: m/s*/
#define ZW_SENSOR_UNIT_VELO_MPH       0x01    /**< Velocity unit: mph*/

#define ZW_SENSOR_UNIT_DIR_DEG        0x00    /**< Direction unit: 0 to 360 degrees. 0 = no wind, 90 = east,
                                                   180 = south, 270 = west, and 360 = north*/

#define ZW_SENSOR_UNIT_ATM_KPA        0x00    /**< Atmospheric pressure unit: kPa*/
#define ZW_SENSOR_UNIT_ATM_HG         0x01    /**< Atmospheric pressure unit: inches of Mercury*/

#define ZW_SENSOR_UNIT_BARO_KPA       0x00    /**< Barometric pressure unit: kPa*/
#define ZW_SENSOR_UNIT_BARO_HG        0x01    /**< Barometric pressure unit: inches of Mercury*/

#define ZW_SENSOR_UNIT_SLR_WM2        0x00    /**< Solar radiation unit: W/m2*/

#define ZW_SENSOR_UNIT_DEW_CEL        0x00    /**< Dew point unit: Celsius (C)*/
#define ZW_SENSOR_UNIT_DEW_FAHR       0x01    /**< Dew point unit: Fahrenheit (F)*/

#define ZW_SENSOR_UNIT_RAIN_MMH       0x00    /**< Rain rate unit: mm/h*/
#define ZW_SENSOR_UNIT_RAIN_INH       0x01    /**< Rain rate unit: in/h*/

#define ZW_SENSOR_UNIT_TIDE_M         0x00    /**< Tide level unit: m*/
#define ZW_SENSOR_UNIT_TIDE_FT        0x01    /**< Tide level unit: feet*/

#define ZW_SENSOR_UNIT_WGT_KG         0x00    /**< Weight unit: kg*/
#define ZW_SENSOR_UNIT_WGT_LBS        0x01    /**< Weight unit: pounds*/

#define ZW_SENSOR_UNIT_VOLT_V         0x00    /**< Voltage unit: V*/
#define ZW_SENSOR_UNIT_VOLT_MV        0x01    /**< Voltage unit: mV*/

#define ZW_SENSOR_UNIT_CUR_A          0x00    /**< Current unit: A*/
#define ZW_SENSOR_UNIT_CUR_MA         0x01    /**< Current unit: mA*/

#define ZW_SENSOR_UNIT_CO2_PPM        0x00    /**< CO2-level unit: ppm*/

#define ZW_SENSOR_UNIT_AIR_M3H        0x00    /**< Air flow unit: m3/h*/
#define ZW_SENSOR_UNIT_AIR_CFM        0x01    /**< Air flow unit: cfm (cubic feet per minute)*/

#define ZW_SENSOR_UNIT_TANK_L         0x00    /**< Tank capacity unit: l*/
#define ZW_SENSOR_UNIT_TANK_CBM       0x01    /**< Tank capacity unit: cbm*/
#define ZW_SENSOR_UNIT_TANK_GAL       0x02    /**< Tank capacity unit: US gallons*/

#define ZW_SENSOR_UNIT_DIST_M         0x00    /**< Distance unit: m*/
#define ZW_SENSOR_UNIT_DIST_CM        0x01    /**< Distance unit: cm*/
#define ZW_SENSOR_UNIT_DIST_FT        0x02    /**< Distance unit: feet*/

#define ZW_SENSOR_UNIT_AGL_PERC       0x00    /**< Angle Position unit: Percentage value*/
#define ZW_SENSOR_UNIT_AGL_DEGN       0x01    /**< Angle Position unit: Degrees relative to
                                                   north pole of standing eye view*/
#define ZW_SENSOR_UNIT_AGL_DEGS       0x02    /**< Angle Position unit: Degrees relative to
                                                   south pole of standing eye view*/

#define ZW_SENSOR_UNIT_ROT_RPM        0x00    /**< Rotation unit: rpm (revolutions per minute)*/
#define ZW_SENSOR_UNIT_ROT_HZ         0x01    /**< Rotation unit: Hz (Hertz)*/

#define ZW_SENSOR_UNIT_WTR_TEMP_CEL   0x00    /**< Water temperature unit: Celsius (C)*/
#define ZW_SENSOR_UNIT_WTR_TEMP_FAHR  0x01    /**< Water temperature unit: Fahrenheit (F)*/

#define ZW_SENSOR_UNIT_SOIL_TEMP_CEL  0x00    /**< Soil temperature unit: Celsius (C)*/
#define ZW_SENSOR_UNIT_SOIL_TEMP_FAHR 0x01    /**< Soil temperature unit: Fahrenheit (F)*/

#define ZW_SENSOR_UNIT_SEIS_INT_MERC  0x00    /**< Seismic intensity unit: Mercalli*/
#define ZW_SENSOR_UNIT_SEIS_INT_EMCRO 0x01    /**< Seismic intensity unit: European Macroseismic*/
#define ZW_SENSOR_UNIT_SEIS_INT_LIEDU 0x02    /**< Seismic intensity unit: Liedu*/
#define ZW_SENSOR_UNIT_SEIS_INT_SHNDO 0x03    /**< Seismic intensity unit: Shindo*/

#define ZW_SENSOR_UNIT_SEIS_MAG_LOCAL 0x00    /**< Seismic magnitude unit: Local (ML)*/
#define ZW_SENSOR_UNIT_SEIS_MAG_MOM   0x01    /**< Seismic magnitude unit: Moment (MW)*/
#define ZW_SENSOR_UNIT_SEIS_MAG_SWAVE 0x02    /**< Seismic magnitude unit: Surface wave (MS)*/
#define ZW_SENSOR_UNIT_SEIS_MAG_BWAVE 0x03    /**< Seismic magnitude unit: Body wave (MB)*/

#define ZW_SENSOR_UNIT_UV_INDEX       0x00    /**< Ultraviolet unit: UV index*/

#define ZW_SENSOR_UNIT_ELEC_RES_OHMM  0x00    /**< Electrical resistivity unit: ohm metre*/

#define ZW_SENSOR_UNIT_ELEC_COND_SIEM 0x00    /**< Electrical conductivity unit: siemens per metre (S/m)*/

#define ZW_SENSOR_UNIT_LOUDNESS_ABS   0x00    /**< Loudness unit: Absolute loudness (dB)*/
#define ZW_SENSOR_UNIT_LOUDNESS_A_WT  0x01    /**< Loudness unit: A-weighted decibels (dBA)*/

#define ZW_SENSOR_UNIT_MOIST_PERC     0x00    /**< Moisture unit: Percentage value*/
#define ZW_SENSOR_UNIT_MOIST_VOL_WTR  0x01    /**< Moisture unit: Volume water content (m3/m3)*/
#define ZW_SENSOR_UNIT_MOIST_IMPD     0x02    /**< Moisture unit: Impedance (k ohm)*/
#define ZW_SENSOR_UNIT_MOIST_WTR_ACT  0x03    /**< Moisture unit: Water activity (aw)*/

#define ZW_SENSOR_UNIT_FREQ_HZ        0x00    /**< Frequency unit: Hz - Must be used until 4.294967295 GHz*/
#define ZW_SENSOR_UNIT_FREQ_KHZ       0x01    /**< Frequency unit: kHz- Must be used until 4.294967295 GHz*/

#define ZW_SENSOR_UNIT_TIME_SEC       0x00    /**< Time unit: seconds*/

#define ZW_SENSOR_UNIT_TGT_TEMP_CEL   0x00    /**< Target temperature unit: Celsius (C)*/
#define ZW_SENSOR_UNIT_TGT_TEMP_FAHR  0x01    /**< Target temperature unit: Fahrenheit (F)*/

#define ZW_SENSOR_UNIT_PM_2_5_MM3     0x00    /**< Particulate matter 2.5 unit: mole per cubic meter (mol/m3) */
#define ZW_SENSOR_UNIT_PM_2_5_UGM3    0x01    /**< Particulate matter 2.5 unit: Absolute microgram/cubic meter (ug/m3)*/

#define ZW_SENSOR_UNIT_F_CH2O_MM3     0x00    /**< Formaldehyde CH2O-level unit: mole per cubic meter (mol/m3) */

#define ZW_SENSOR_UNIT_RAD_CONT_BQM3  0x00    /**< Radon Concentration unit: Becquerel/cubic meter (bq/m3)*/
#define ZW_SENSOR_UNIT_RAD_CONT_PCIL  0x01    /**< Radon Concentration unit: picocuries/liter (pCi/L)*/

#define ZW_SENSOR_UNIT_METH_DENS_MM3  0x00    /**< Methane Density CH4 unit: mole per cubic meter (mol/m3) */

#define ZW_SENSOR_UNIT_VOC_MM3        0x00    /**< Volatile Organic unit: mole per cubic meter (mol/m3) */

#define ZW_SENSOR_UNIT_CO_LVL_MM3     0x00    /**< Carbon Monoxide CO-level unit: mole per cubic meter (mol/m3) */

#define ZW_SENSOR_UNIT_SOIL_HUMD_PERC 0x00    /**< Soil Humidity unit: Percentage value */

#define ZW_SENSOR_UNIT_SOIL_REAC_PH   0x00    /**< Soil Reactivity unit: acidity (pH) */

#define ZW_SENSOR_UNIT_SOIL_SAL_MM3   0x00    /**< Soil Salinity unit: mole per cubic meter (mol/m3) */

#define ZW_SENSOR_UNIT_HEART_RT_BPM   0x00    /**< Heart Rate unit: beats/minute (Bpm) */

#define ZW_SENSOR_UNIT_BLOOD_PRS_SYS  0x00    /**< Blood Pressure unit: Systolic mmHg (upper number) */
#define ZW_SENSOR_UNIT_BLOOD_PRS_DIA  0x01    /**< Blood Pressure unit: Diastolic mmHg (lower number)*/

#define ZW_SENSOR_UNIT_MUSCLE_MS_KG   0x00    /**< Muscle Mass unit: kilogram (kg) */

#define ZW_SENSOR_UNIT_FAT_MS_KG      0x00    /**< Fat Mass unit: kilogram (kg) */

#define ZW_SENSOR_UNIT_BONE_MS_KG     0x00    /**< Bone Mass unit: kilogram (kg) */

#define ZW_SENSOR_UNIT_TBW_KG         0x00    /**< Total Body Water unit: kilogram (kg) */

#define ZW_SENSOR_UNIT_BMR_J          0x00    /**< Basic Metabolic Rate unit: joule (J)*/

#define ZW_SENSOR_UNIT_BMI_IDX        0x00    /**< Body Mass Index unit: BMI Index */

#define ZW_SENSOR_UNIT_ACCEL_X_MS2    0x00    /**< Acceleration, X-axis unit: (m/s2) */

#define ZW_SENSOR_UNIT_ACCEL_Y_MS2    0x00    /**< Acceleration, Y-axis unit: (m/s2) */

#define ZW_SENSOR_UNIT_ACCEL_Z_MS2    0x00    /**< Acceleration, Z-axis unit: (m/s2) */

#define ZW_SENSOR_UNIT_SMOKE_DEN_PERC 0x00    /**< Smoke Density unit: Percentage value */

#define ZW_SENSOR_UNIT_WATER_FLW_LHR  0x00    /**< Water Flow unit: liter/hour (l/h) */

#define ZW_SENSOR_UNIT_WATER_PRS_KPA  0x00    /**< Water Pressure unit: kilopascal (kPa)*/

#define ZW_SENSOR_UNIT_RF_SGN_RSSI    0x00    /**< RF Signal Strength unit: RSSI (Percentage value) */
#define ZW_SENSOR_UNIT_RF_SGN_DBM     0x01    /**< RF Signal Strength unit: (dBm) */

#define ZW_SENSOR_UNIT_PM_10_MOLE     0x00    /**< Mole per cubic meter (mol/m3)*/
#define ZW_SENSOR_UNIT_PM_10_UG       0x01    /**< Microgram per cubic meter (�g/m3)*/

#define ZW_SENSOR_UNIT_RESPI_RATE_BPM 0x00    /**< Breaths per minute (bpm)*/

#define ZW_SENSOR_UNIT_REL_MOD_PERC   0x00    /**< Percentage value (%)*/

#define ZW_SENSOR_UNIT_BOILER_WTR_TEMP_C 0x00  /**< Celcius (C)*/

#define ZW_SENSOR_UNIT_DHW_TEMP_C     0x00     /**< Celcius (C)*/

#define ZW_SENSOR_UNIT_OUTSIDE_TEMP_C 0x00     /**< Celcius (C)*/

#define ZW_SENSOR_UNIT_EXHAUST_TEMP_C 0x00     /**< Celcius (C)*/

#define ZW_SENSOR_UNIT_WATER_CHLOR_LVL_MGL    0x00    /**< Milligram per liter (mg/l)*/

#define ZW_SENSOR_UNIT_WATER_ACID_PH  0x00     /**< Acidity (pH)*/

#define ZW_SENSOR_UNIT_WATER_OXI_RED_MV       0x00    /**< MilliVolt (mV)*/

///@}

/** sensor data */
typedef struct
{
	uint8_t     type;		/**< ZW_SENSOR_TYPE_XXX */
	uint8_t     precision;  /**< Decimal places of the value.  The decimal
                                 value 1025 with precision 2 is therefore equal to 10.25.*/
	uint8_t     unit;	    /**< ZW_SENSOR_UNIT_XXX */
	uint8_t     size;	    /**< data size: 1,2,or 4 bytes*/
	uint8_t     data[4];	/**< sensor data (a signed number) with the first byte
                                 is the most significant byte*/
}
zwsensor_t, *zwsensor_p;

// 3298
/** Device configuration single parameter*/
typedef struct
{
	uint8_t     param_num;	    /**< parameter number */
	uint8_t     size;	        /**< data size: 1,2,or 4 bytes*/
	uint8_t     data[4];	    /**< data (a signed or unsigned number, depending on parameter number data format) with the
                                     first byte (i.e. data[0]) is the most significant byte*/
	uint8_t     use_default;	/**< only valid for configuration set command. Parameter flag: 1=use default factory setting and ignore data[];
                                     0=use the value in data[]*/
} zwconfig_t, *zwconfig_p;

// 3463

/**
@}
@defgroup If_Mtr Meter Interface APIs
Meters can be reset
@ingroup zwarecapi
@{
*/

/** Meter Type definition */
#define ZW_METER_TYPE_ELEC           0x01    /**< Electric meter*/
#define ZW_METER_TYPE_GAS            0x02    /**< Gas meter*/
#define ZW_METER_TYPE_WATER          0x03    /**< Water meter*/


/** Meter Unit definition */
#define ZW_METER_UNIT_ELEC_KWH       0x00    /**< Electric meter unit: kWh*/
#define ZW_METER_UNIT_ELEC_KVAH      0x01    /**< Electric meter unit: kVAh*/
#define ZW_METER_UNIT_ELEC_W         0x02    /**< Electric meter unit: W*/
#define ZW_METER_UNIT_ELEC_PULSE     0x03    /**< Electric meter unit: pulse count*/
#define ZW_METER_UNIT_ELEC_V         0x04    /**< Electric meter unit: V*/
#define ZW_METER_UNIT_ELEC_A         0x05    /**< Electric meter unit: A*/
#define ZW_METER_UNIT_ELEC_PF        0x06    /**< Electric meter unit: power factor*/

#define ZW_METER_UNIT_GAS_CM         0x00    /**< Gas meter unit: cubic meters*/
#define ZW_METER_UNIT_GAS_CF         0x01    /**< Gas meter unit: cubic feet*/
#define ZW_METER_UNIT_GAS_PULSE      0x03    /**< Gas meter unit: pulse count*/

#define ZW_METER_UNIT_WATER_CM       0x00    /**< Water meter unit: cubic meters*/
#define ZW_METER_UNIT_WATER_CF       0x01    /**< Water meter unit: cubic feet*/
#define ZW_METER_UNIT_WATER_GAL      0x02    /**< Water meter unit: US gallons*/
#define ZW_METER_UNIT_WATER_PULSE    0x03    /**< Water meter unit: pulse count*/


/** Meter Supported Unit Bit-mask definition : 1=supported, 0=not supported */
#define ZW_METER_SUP_UNIT_ELEC_KWH       0x01    /**< Electric meter unit: kWh*/
#define ZW_METER_SUP_UNIT_ELEC_KVAH      0x02    /**< Electric meter unit: kVAh*/
#define ZW_METER_SUP_UNIT_ELEC_W         0x04    /**< Electric meter unit: W*/
#define ZW_METER_SUP_UNIT_ELEC_PULSE     0x08    /**< Electric meter unit: pulse count*/
#define ZW_METER_SUP_UNIT_ELEC_V         0x10    /**< Electric meter unit: V*/
#define ZW_METER_SUP_UNIT_ELEC_A         0x20    /**< Electric meter unit: A*/
#define ZW_METER_SUP_UNIT_ELEC_PF        0x40    /**< Electric meter unit: power factor*/

#define ZW_METER_SUP_UNIT_GAS_CM         0x01    /**< Gas meter unit: cubic meters*/
#define ZW_METER_SUP_UNIT_GAS_CF         0x02    /**< Gas meter unit: cubic feet*/
#define ZW_METER_SUP_UNIT_GAS_PULSE      0x08    /**< Gas meter unit: pulse count*/

#define ZW_METER_SUP_UNIT_WATER_CM       0x01    /**< Water meter unit: cubic meters*/
#define ZW_METER_SUP_UNIT_WATER_CF       0x02    /**< Water meter unit: cubic feet*/
#define ZW_METER_SUP_UNIT_WATER_GAL      0x04    /**< Water meter unit: US gallons*/
#define ZW_METER_SUP_UNIT_WATER_PULSE    0x08    /**< Water meter unit: pulse count*/


/** Meter Rate Type definition */
#define ZW_METER_RATE_IMPORT           0x01    /**< Import: Meter Value is a consumed measurement*/
#define ZW_METER_RATE_EXPORT           0x02    /**< Export: Meter Value is a produced measurement*/


#define ZW_ADMIN_STR_MAX    32  /**< maximum number of characters in the administration number*/
#define ZW_ID_STR_MAX       32  /**< maximum number of characters in the identification of customer*/
#define ZW_METER_CAP_MON	1	/**< id & admin number available */
#define ZW_METER_CAP_CFG	2	/**< admin number can be set */

/** meter descriptor */
typedef struct
{
	uint8_t	caps;			                /**< ZW_METER_CAP_xxx or-mask */
	char	id[ZW_ID_STR_MAX + 1];			/**< id, used for identification of customer and metering device */
	char	admin[ZW_ADMIN_STR_MAX + 1];    /**< admin number used to identify customer */
}
zwmeter_t, *zwmeter_p;


/** meter data */
typedef struct
{
	uint8_t     type;		/**< ZW_METER_TYPE_XXX */
	uint8_t     precision;  /**< Decimal places of the value.  The decimal
                                 value 1025 with precision 2 is therefore equal to 10.25.*/
	uint8_t     unit;	    /**< ZW_METER_UNIT_XXX */
	uint8_t     size;	    /**< data size: 1,2,or 4 bytes*/
	uint8_t     data[4];	/**< meter data (a signed number) with the first byte
                                 is the most significant byte*/
	uint16_t    delta_time; /**< elapsed time in seconds between the �Meter Value and the �Previous
                                 Meter Value measurements.  If delta_time = 0, it means no �Previous
                                 Meter Value measurement*/
	uint8_t     prv_data[4];/**< previous meter data (a signed number) with the first byte
                                 is the most significant byte. This field is valid only if delta_time > 0*/
	uint8_t     rate_type;	/**< rate type: ZW_METER_RATE_XXX */

}
zwmeter_dat_t, *zwmeter_dat_p;


/** meter capabilities */
typedef struct
{
	uint8_t     type;		/**< ZW_METER_TYPE_XXX */
	uint8_t     reset_cap;	/**< capability to reset all accumulated values stored in the meter device.
                                 1=reset supported, 0=not supported. See zwif_meter_reset()*/
	uint8_t     unit_sup;   /**< supported unit bit-mask : ZW_METER_SUP_UNIT_XXX */
}
zwmeter_cap_t, *zwmeter_cap_p;

// 4196
/** Basic data */
typedef struct
{
	uint8_t curr_val;  	/**< current value: 0 = off; 0x01~0x63 = percentage (%);0xFE = Unknown; 0xFF = On. */
	uint8_t tgt_val;	/**< target value with same interpretation as curr_val */
	uint8_t dur;	    /**< Duration: 0 = already at the target; 0x01~0x7F = seconds; 0x80~0xFD = 1~126 minutes;
                                       0xFE = Unknown duration; 0xFF = reserved*/
}
zwbasic_t, *zwbasic_p;

// 4228
/**
@}
@defgroup If_Doorlock Door Lock Interface APIs
Used to secure/unsecure a lock type as well as setting the configuration of an advanced Z-Wave door lock device
@ingroup zwarecapi
@{
*/

/** @name Door_lock_op_mode
* Door lock operation mode definition
@{
*/
#define ZW_DOOR_UNSEC           0x00    /**< Door unsecured*/
#define ZW_DOOR_UNSEC_TMOUT     0x01    /**< Door unsecured with timeout. Fallback to secured mode after timeout has expired*/
#define ZW_DOOR_UNSEC_IN        0x10    /**< Door unsecured for inside door handles*/
#define ZW_DOOR_UNSEC_IN_TMOUT  0x11    /**< Door unsecured for inside door handles with timeout*/
#define ZW_DOOR_UNSEC_OUT       0x20    /**< Door unsecured for outside door handles*/
#define ZW_DOOR_UNSEC_OUT_TMOUT 0x21    /**< Door unsecured for outside door handles with timeout*/
#define ZW_DOOR_UNKNOWN         0xFE    /**< Unknown state. This could happend while in transition to a new mode.
                                             Example: Bolt is not fully retracted/engaged*/
#define ZW_DOOR_SEC             0xFF    /**< Door secured*/

// 4263
/** Door condition bit-mask */
#define ZW_COND_DOOR_MASK       0x01    /**< Bit-mask for door status.
                                             After masking bit=0 means door open; else closed*/
#define ZW_COND_BOLT_MASK       0x02    /**< Bit-mask for bolt status.
                                             After masking bit=0 means bolt locked; else unlocked*/
#define ZW_COND_LATCH_MASK      0x04    /**< Bit-mask for latch status.
                                             After masking bit=0 means latch open; else closed*/
/** Door lock operation mode*/
typedef struct
{
	uint8_t     mode;           /**< Current door lock mode (ZW_DOOR_XXX) */
	uint8_t     out_mode;       /**< Outside door handles mode. It's a 4-bit mask; bit=0 for disabled, bit=1 for enabled.
                                     When disabled, the actual handle cannot open the door locally.
                                     When enabled, the actual handle can open the door locally*/
	uint8_t     in_mode;        /**< Inside door handles mode. It's a 4-bit mask; bit=0 for disabled, bit=1 for enabled*/
	uint8_t     cond;           /**< Door condition. See ZW_COND_XXX_MASK */
	uint8_t     tmout_min;      /**< The remaining time in minutes before the door lock will automatically be locked again.
                                     Value of 0xFE means timeout is not supported*/
	uint8_t     tmout_sec;      /**< The remaining time in seconds before the door lock will automatically be locked again.
                                     Value of 0xFE means timeout is not supported*/
	uint8_t     tgt_mode;	    /**< Target door lock mode (ZW_DOOR_XXX) */
	uint8_t     dur;	        /**< Duration: 0 = already at the target; 0x01~0x7F = seconds; 0x80~0xFD = 1~126 minutes;
                                     0xFE = Unknown duration; 0xFF = reserved*/
}
zwdlck_op_t, *zwdlck_op_p;

// 4315
/** @name Door_lock_op_type
* Door lock operation type definition
@{
*/
#define ZW_DOOR_OP_CONST    0x01    /**< Constant operation*/
#define ZW_DOOR_OP_TIMED    0x02    /**< Timed operation*/
/**
@}
*/

/** Door lock configuration */
typedef struct
{
	uint8_t     type;           /**< Door lock operation type (ZW_DOOR_OP_XXX) */
	uint8_t     out_sta;        /**< Outside door handles state. It's a 4-bit mask; bit=0 for disable, bit=1 for enable
                                     When disabled, the actual handle cannot open the door locally.
                                     When enabled, the actual handle can open the door locally*/
	uint8_t     in_sta;         /**< Inside door handles state. It's a 4-bit mask; bit=0 for disable, bit=1 for enable*/
	uint8_t     tmout_min;      /**< Lock timeout in minutes. Valid value: 0 to 253. Value of 0xFE means timeout is not supported*/
	uint8_t     tmout_sec;      /**< Lock timeout in seconds. Valid value: 0 to 59. Value of 0xFE means timeout is not supported*/
}
zwdlck_cfg_t, *zwdlck_cfg_p;

// 4365
/**
@}
@defgroup If_Usrcode User Code Interface APIs
Used to manage user codes required to unlock a lock
@ingroup zwarecapi
@{
*/

/** @name User_id_status
* User ID status definition
@{
*/
#define ZW_USRCOD_AVAIL         0x00    /**< Available (not set)*/
#define ZW_USRCOD_UNAVAIL       0x01    /**< Occupied*/
#define ZW_USRCOD_RSVD          0x02    /**< Reserved by administrator*/
#define ZW_USRCOD_NO_STS        0xFE    /**< Status unavailable*/
/**
@}
*/

#define MAX_USRCOD_LENGTH       10      /**< Max. user code length*/
/** User code */
typedef struct
{
	uint8_t     id;                     /**< User identifier */
	uint8_t     id_sts;                 /**< User id status*/
	uint8_t     code_len;               /**< User code length*/
	uint8_t     code[MAX_USRCOD_LENGTH];/**< User code; minimum length = 4, maximum length = 10*/
}
zwusrcod_t, *zwusrcod_p;

// 4454
/**
@}
@defgroup If_Alarm Alarm Interface APIs
Used to report alarm or service conditions
@ingroup zwarecapi
@{
*/

/** @name Alarm_type
* Z-wave alarm type definition
@{
*/
#define ZW_ALRM_SMOKE       0x01    /**< Smoke alarm*/
#define ZW_ALRM_CO          0x02    /**< Carbon monoxide alarm*/
#define ZW_ALRM_CO2         0x03    /**< Carbon dioxide alarm*/
#define ZW_ALRM_HEAT        0x04    /**< Heat alarm*/
#define ZW_ALRM_WATER       0x05    /**< Water alarm*/
#define ZW_ALRM_LOCK        0x06    /**< Lock access control alarm*/
#define ZW_ALRM_BURGLAR     0x07    /**< Burglar alarm or home security*/
#define ZW_ALRM_POWER       0x08    /**< Power management alarm*/
#define ZW_ALRM_SYSTEM      0x09    /**< System alarm*/
#define ZW_ALRM_EMERGENCY   0x0A    /**< Emergency alarm*/
#define ZW_ALRM_CLOCK       0x0B    /**< Alarm clock*/
#define ZW_ALRM_APPLIANCE   0x0C    /**< Home appliance alarm*/
#define ZW_ALRM_HEALTH      0x0D    /**< Home health alarm*/
#define ZW_ALRM_SIREN       0x0E    /**< Siren alarm*/
#define ZW_ALRM_WATER_VLV   0x0F    /**< Water Valve alarm*/
#define ZW_ALRM_WEATHER     0x10    /**< Weather alarm*/
#define ZW_ALRM_IRRIGATION  0x11    /**< Irrigation alarm*/
#define ZW_ALRM_GAS         0x12    /**< Gas alarm*/
#define ZW_ALRM_PEST_CTL    0x13    /**< Pest control*/
#define ZW_ALRM_LIGHT_SNSR  0x14    /**< Light sensor*/

#define ZW_ALRM_REQ_PEND    0xFF    /**< Request pending notification*/
#define ZW_ALRM_FIRST       0xFF    /**< Used by the zwif_alrm_get() to retrieve the
                                         first alarm detection from the supported list*/
/**
@}
*/

/** @name Alarm_event
* Z-wave alarm event definition
@{
*/
/** Z-wave alarm event for all types of alarms*/
#define ZW_ALRM_EVT_UNKNOWN             0xFE    /**< Unknown event*/
#define ZW_ALRM_EVT_INACTIVE_CLEAR      0x00    /**< Event inactive (push mode) / Previous events cleared (pull mode)*/

/** Z-wave alarm event for smoke alarm*/
#define ZW_ALRM_EVT_SMOKE_L             0x01    /**< Smoke detected with location info*/
#define ZW_ALRM_EVT_SMOKE               0x02    /**< Smoke detected with unknown location info*/
#define ZW_ALRM_EVT_SMOKE_TEST          0x03    /**< Smoke alarm test*/
#define ZW_ALRM_EVT_SMOKE_REPLA         0x04    /**< Replacement required*/
#define ZW_ALRM_EVT_SMOKE_REPLA_EOL     0x05    /**< Replacement required, End-of-life*/
#define ZW_ALRM_EVT_SMOKE_MAINTNC_PLAN  0x07    /**< Maintenance required, planned periodic inspection*/
#define ZW_ALRM_EVT_SMOKE_MAINTNC_DUST  0x08    /**< Maintenance required, dust in device*/

/** Z-wave alarm event for carbon monoxide alarm*/
#define ZW_ALRM_EVT_CO_L                0x01    /**< Carbon monoxide detected with location info*/
#define ZW_ALRM_EVT_CO                  0x02    /**< Carbon monoxide detected with unknown location info*/
#define ZW_ALRM_EVT_CO_TEST             0x03    /**< Carbon monoxide test. Event parameter value: 1=OK, 2=Failed*/
#define ZW_ALRM_EVT_CO_REPLA            0x04    /**< Replacement required*/
#define ZW_ALRM_EVT_CO_REPLA_EOL        0x05    /**< Replacement required, End-of-life*/
#define ZW_ALRM_EVT_CO_SILENCED         0x06    /**< Alarm silenced*/
#define ZW_ALRM_EVT_CO_MAINTNC_PLAN     0x07    /**< Maintenance required, planned periodic inspection*/

/** Z-wave alarm event for carbon dioxide alarm*/
#define ZW_ALRM_EVT_CO2_L               0x01    /**< Carbon dioxide detected with location info*/
#define ZW_ALRM_EVT_CO2                 0x02    /**< Carbon dioxide detected with unknown location info*/
#define ZW_ALRM_EVT_CO2_TEST            0x03    /**< Carbon dioxide test. Event parameter value: 1=OK, 2=Failed*/
#define ZW_ALRM_EVT_CO2_REPLA           0x04    /**< Replacement required*/
#define ZW_ALRM_EVT_CO2_REPLA_EOL       0x05    /**< Replacement required, End-of-life*/
#define ZW_ALRM_EVT_CO2_SILENCED        0x06    /**< Alarm silenced*/
#define ZW_ALRM_EVT_CO2_MAINTNC_PLAN    0x07    /**< Maintenance required, planned periodic inspection*/

/** Z-wave alarm event for heat alarm*/
#define ZW_ALRM_EVT_OVERHEAT_L          0x01    /**< Overheat detected with location info*/
#define ZW_ALRM_EVT_OVERHEAT            0x02    /**< Overheat detected with unknown location info*/
#define ZW_ALRM_EVT_TEMP_RISE_L         0x03    /**< Rapid temperature rise detected with location info*/
#define ZW_ALRM_EVT_TEMP_RISE           0x04    /**< Rapid temperature rise detected with unknown location info*/
#define ZW_ALRM_EVT_UNDRHEAT_L          0x05    /**< Underheat detected with location info*/
#define ZW_ALRM_EVT_UNDRHEAT            0x06    /**< Underheat detected with unknown location info*/
#define ZW_ALRM_EVT_HT_TEST             0x07    /**< Heat alarm test*/
#define ZW_ALRM_EVT_HT_REPLA_EOL        0x08    /**< Replacement required, End-of-life*/
#define ZW_ALRM_EVT_HT_SILENCED         0x09    /**< Alarm silenced*/
#define ZW_ALRM_EVT_HT_MAINTNC_DUST     0x0A    /**< Maintenance required, dust in device*/
#define ZW_ALRM_EVT_HT_MAINTNC_PLAN     0x0B    /**< Maintenance required, planned periodic inspection*/
#define ZW_ALRM_EVT_RAPID_TEMP_FALL_L   0x0C    /**< Rapid temparature fall with location info*/
#define ZW_ALRM_EVT_RAPID_TEMP_FALL     0x0D    /**< Rapid temparature fall*/

/** Z-wave alarm event for water alarm*/
#define ZW_ALRM_EVT_LEAK_L              0x01    /**< Water leak detected with location info*/
#define ZW_ALRM_EVT_LEAK                0x02    /**< Water leak detected with unknown location info*/
#define ZW_ALRM_EVT_LVL_L               0x03    /**< Water level dropped detected with location info*/
#define ZW_ALRM_EVT_LVL                 0x04    /**< Water level dropped detected with unknown location info*/
#define ZW_ALRM_EVT_REPLACE_WATER_FILTER    0x05    /**< Replace Water Filter*/
#define ZW_ALRM_EVT_WATER_FLOW          0x06    /**< Water flow. Event parameter value: 1=no data, 2=below low treshold,
                                                     3=above high treshold, 4=max*/
#define ZW_ALRM_EVT_WATER_PRESSURE      0x07    /**< Water pressure. Event parameter meaning same as those for water flow. */
#define ZW_ALRM_EVT_WATER_TEMP          0x08    /**< Water temparature. Event parameter value: 1=no data, 2=below low treshold,
                                                     3=above high treshold*/
#define ZW_ALRM_EVT_WATER_LEVEL         0x09    /**< Water level. Event parameter meaning same as those for water temparature. */
#define ZW_ALRM_EVT_SUMP_PUMP_ACTV      0x0A    /**< Sump pump active*/
#define ZW_ALRM_EVT_SUMP_PUMP_FAIL      0x0B    /**< Sump pump failure*/

/** Z-wave alarm event for lock access control alarm*/
#define ZW_ALRM_EVT_MANUAL_LCK          0x01    /**< Manual lock operation*/
#define ZW_ALRM_EVT_MANUAL_ULCK         0x02    /**< Manual unlock operation*/
#define ZW_ALRM_EVT_RF_LCK              0x03    /**< RF lock operation*/
#define ZW_ALRM_EVT_RF_ULCK             0x04    /**< RF unlock operation*/
#define ZW_ALRM_EVT_KEYPAD_LCK          0x05    /**< Keypad lock operation with user identifier info*/
#define ZW_ALRM_EVT_KEYPAD_ULCK         0x06    /**< Keypad unlock operation with user identifier info*/
#define ZW_ALRM_EVT_MANUAL_NOT_FUL_LCK  0x07    /**< Manual not fully locked operation*/
#define ZW_ALRM_EVT_RF_NOT_FUL_LCK      0x08    /**< RF not fully locked operation*/
#define ZW_ALRM_EVT_AUTO_LCK            0x09    /**< Auto lock locked operation*/
#define ZW_ALRM_EVT_AUTO_NOT_FUL_OPER   0x0A    /**< Auto lock not fully operation*/
#define ZW_ALRM_EVT_LCK_JAMMED          0x0B    /**< Lock jammed*/
#define ZW_ALRM_EVT_ALL_CODE_DEL        0x0C    /**< All user codes deleted*/
#define ZW_ALRM_EVT_1_CODE_DEL          0x0D    /**< Single user code deleted*/
#define ZW_ALRM_EVT_CODE_ADDED          0x0E    /**< New user code added*/
#define ZW_ALRM_EVT_CODE_DUP            0x0F    /**< New user code not added due to duplicate code*/
#define ZW_ALRM_EVT_KEYPAD_DISABLED     0x10    /**< Keypad temporary disabled*/
#define ZW_ALRM_EVT_KEYPAD_BUSY         0x11    /**< Keypad busy*/
#define ZW_ALRM_EVT_NEW_PROG_CODE       0x12    /**< New program code entered - unique code for lock configuration*/
#define ZW_ALRM_EVT_USR_CODE_LIMIT      0x13    /**< Manually enter user access code exceeds code limit*/
#define ZW_ALRM_EVT_RF_ULCK_INVLD_CODE  0x14    /**< Unlock by RF with invalid user code*/
#define ZW_ALRM_EVT_RF_LCK_INVLD_CODE   0x15    /**< Locked by RF with invalid user code*/
#define ZW_ALRM_EVT_WINDOW_DOOR_OPEN    0x16    /**< Window/door is open*/
#define ZW_ALRM_EVT_WINDOW_DOOR_CLOSED  0x17    /**< Window/door is closed*/
#define ZW_ALRM_EVT_WIN_DR_HDL_OPEN     0x18    /**< Window/door handle is open*/
#define ZW_ALRM_EVT_WIN_DR_HDL_CLOSED   0x19    /**< Window/door handle is closed*/
/**< 0x1A - 0x3F Reserved*/
#define ZW_ALRM_EVT_BARRIER_INIT					0x40    /**< Barrier performing Initialization process. Event parameter value:
                                                                 0=Completed, 0xFF=In progress*/
#define ZW_ALRM_EVT_BARRIER_OP_FORCE_EXCEED			0x41    /**< Barrier operation (Open / Close) force has been exceeded.*/
#define ZW_ALRM_EVT_BARRIER_MOTOR_TIME_EXCEED		0x42    /**< Barrier motor has exceeded manufacturer's operational time limit.
                                                                 Event parameter value: 0 to 0x7F = 0 to 127 seconds; 0x80 to 0xFE = 1 to 127 minutes*/
#define ZW_ALRM_EVT_BARRIER_MECHANIC_EXCEED			0x43    /**< Barrier operation has exceeded physical mechanical limits.
                                                                 (For example: barrier has opened past the open limit)*/
#define ZW_ALRM_EVT_BARRIER_OP_FAILED				0x44    /**< Barrier unable to perform requested operation due to UL requirements.*/
#define ZW_ALRM_EVT_BARRIER_OP_DISABLED				0x45    /**< Barrier Unattended operation has been disabled per UL requirements*/
#define ZW_ALRM_EVT_BARRIER_MALFUNC					0x46    /**< Barrier failed to perform Requested operation, device malfunction*/
#define ZW_ALRM_EVT_BARRIER_VACAT_MODE				0x47    /**< Barrier Vacation Mode. Event parameter value: 0=disabled, 0xFF=enabled*/
#define ZW_ALRM_EVT_BARRIER_BEAM_OBST				0x48    /**< Barrier Safety Beam Obstacle. Event parameter value: 0=no obstruction, 0xFF=obstruction*/
#define ZW_ALRM_EVT_BARRIER_SNR_ERR					0x49    /**< Barrier Sensor Not Detected / Supervisory Error. Event parameter value:
                                                                 0=sensor not defined, 1 to 0xFF=sensor ID*/
#define ZW_ALRM_EVT_BARRIER_LOW_BATT				0x4A    /**< Barrier Sensor Low Battery Warning. Event parameter value: 0=sensor not defined,
                                                                 1 to 0xFF=sensor ID*/
#define ZW_ALRM_EVT_BARRIER_SHORT					0x4B    /**< Barrier detected short in Wall Station wires*/
#define ZW_ALRM_EVT_BARRIER_NON_ZWAVE				0x4C    /**< Barrier associated with non-Z-wave remote control.*/


/** Z-wave alarm event for burglar alarm*/
#define ZW_ALRM_EVT_INTRUSION_L         0x01    /**< Intrusion detected with location info*/
#define ZW_ALRM_EVT_INTRUSION           0x02    /**< Intrusion detected with unknown location info*/
#define ZW_ALRM_EVT_TMPR_COVER          0x03    /**< Tampering, product covering removed*/
#define ZW_ALRM_EVT_TMPR_CODE           0x04    /**< Tampering, Invalid Code*/
#define ZW_ALRM_EVT_GLASS_L             0x05    /**< Glass breakage detected with location info*/
#define ZW_ALRM_EVT_GLASS               0x06    /**< Glass breakage detected with unknown location info*/
#define ZW_ALRM_EVT_MOTION_DET_L        0x07    /**< Motion detected with location info*/
#define ZW_ALRM_EVT_MOTION_DET          0x08    /**< Motion detected with unknown location info*/

/** Z-wave alarm event for power management alarm*/
#define ZW_ALRM_EVT_POWER               0x01    /**< Power has been applied*/
#define ZW_ALRM_EVT_AC_OFF              0x02    /**< AC mains disconnected*/
#define ZW_ALRM_EVT_AC_ON               0x03    /**< AC mains re-connected*/
#define ZW_ALRM_EVT_SURGE               0x04    /**< Surge Detection*/
#define ZW_ALRM_EVT_VOLT_DROP           0x05    /**< Voltage Drop/Drift detected*/
#define ZW_ALRM_EVT_OVER_CURRENT        0x06    /**< Over-current detected*/
#define ZW_ALRM_EVT_OVER_VOLT           0x07    /**< Over-voltage detected*/
#define ZW_ALRM_EVT_OVER_LOAD           0x08    /**< Over-load detected*/
#define ZW_ALRM_EVT_LOAD_ERR            0x09    /**< Load error*/
#define ZW_ALRM_EVT_REPLACE_BATT_SOON   0x0A    /**< Replace battery soon*/
#define ZW_ALRM_EVT_REPLACE_BATT_NOW    0x0B    /**< Replace battery now*/
#define ZW_ALRM_EVT_BATT_CHARGING       0x0C    /**< Battery is charging*/
#define ZW_ALRM_EVT_BATT_CHARGED        0x0D    /**< Battery is fully charged*/
#define ZW_ALRM_EVT_CHARGE_BATT_SOON    0x0E    /**< Charge battery soon*/
#define ZW_ALRM_EVT_CHARGE_BATT_NOW     0x0F    /**< Charge battery now!*/
#define ZW_ALRM_EVT_BKUP_BATT_LOW       0x10    /**< Back-up battery is low*/
#define ZW_ALRM_EVT_BATT_FLUID_LOW      0x11    /**< Battery fluid is low*/

/** Z-wave alarm event for system alarm*/
#define ZW_ALRM_EVT_HW                  0x01    /**< System hardware failure*/
#define ZW_ALRM_EVT_SW                  0x02    /**< System software failure*/
#define ZW_ALRM_EVT_HW_OEM_CODE         0x03    /**< System hardware failure with OEM proprietary failure code*/
#define ZW_ALRM_EVT_SW_OEM_CODE         0x04    /**< System software failure with OEM proprietary failure code*/

/** Z-wave alarm event for emergency alarm*/
#define ZW_ALRM_EVT_POLICE              0x01    /**< Contact police*/
#define ZW_ALRM_EVT_FIRE                0x02    /**< Contact fire service*/
#define ZW_ALRM_EVT_MEDICAL             0x03    /**< Contact medical service*/

/** Z-wave alarm event for alarm clock*/
#define ZW_ALRM_EVT_WKUP                0x01    /**< Wake up alert*/
#define ZW_ALRM_EVT_TIMER_ENDED         0x02    /**< Timer ended*/
#define ZW_ALRM_EVT_TIME_REMAINING      0x03    /**< Time remaining. Event parameter value (3 bytes): byte-0 unit = hours,
                                                     byte-1 unit = minutes, byte-2 unit = seconds*/

/** Z-wave alarm event for Appliance*/
#define ZW_ALRM_EVT_PROG_STARTED		0x01    /**< Program started*/
#define ZW_ALRM_EVT_PROG_IN_PROGRESS	0x02    /**< Program in progress*/
#define ZW_ALRM_EVT_PROG_END			0x03    /**< Program completed*/
#define ZW_ALRM_EVT_REPLACE_FILTER		0x04    /**< Replace main filter*/
#define ZW_ALRM_EVT_SET_TEMP_ERR		0x05    /**< Failure to set target temperature*/
#define ZW_ALRM_EVT_SUPPLYING_WATER		0x06    /**< Supplying water*/
#define ZW_ALRM_EVT_WATER_SUPPLY_ERR	0x07    /**< Water supply failure*/
#define ZW_ALRM_EVT_BOILING				0x08    /**< Boiling*/
#define ZW_ALRM_EVT_BOILING_ERR			0x09    /**< Boiling failure*/
#define ZW_ALRM_EVT_WASHING				0x0A    /**< Washing*/
#define ZW_ALRM_EVT_WASHING_ERR			0x0B    /**< Washing failure*/
#define ZW_ALRM_EVT_RINSING				0x0C    /**< Rinsing*/
#define ZW_ALRM_EVT_RINSING_ERR			0x0D    /**< Rinsing failure*/
#define ZW_ALRM_EVT_DRAINING			0x0E    /**< Draining*/
#define ZW_ALRM_EVT_DRAINING_ERR		0x0F    /**< Draining failure*/
#define ZW_ALRM_EVT_SPINNING			0x10    /**< Spinning*/
#define ZW_ALRM_EVT_SPINNING_ERR		0x11    /**< Spinning failure*/
#define ZW_ALRM_EVT_DRYING				0x12    /**< Drying*/
#define ZW_ALRM_EVT_DRYING_ERR			0x13    /**< Drying failure*/
#define ZW_ALRM_EVT_FAN_ERR				0x14    /**< Fan failure*/
#define ZW_ALRM_EVT_COMPRESSOR_ERR		0x15    /**< Compressor failure*/

/** Z-wave alarm event for Home Health*/
#define ZW_ALRM_EVT_LEAVING_BED						0x01    /**< Leaving Bed*/
#define ZW_ALRM_EVT_SITTING_ON_BED					0x02    /**< Sitting on bed*/
#define ZW_ALRM_EVT_LYING_ON_BED					0x03    /**< Lying on bed*/
#define ZW_ALRM_EVT_POSTURE_CHANGED					0x04    /**< Posture changed*/
#define ZW_ALRM_EVT_SITTING_ON_BED_EDGE				0x05    /**< Sitting on edge of bed*/
#define ZW_ALRM_EVT_VOLATILE_ORGANIC_COMPOUND_LVL   0x06    /**< Volatile Organic Compound level. Event parameter value (pollution level):
                                                                 1=clean, 2=Slightly polluted, 3=Moderately polluted, 4=Highly polluted,*/

/** Z-wave alarm event for Siren*/
#define ZW_ALRM_EVT_SIREN_ACT						0x01    /**< Siren Active*/

/** Z-wave alarm event for Water Valve*/
#define ZW_ALRM_EVT_VALVE_OP						0x01    /**< Valve Operation. Event parameter value: 0=closed, 1=open*/
#define ZW_ALRM_EVT_MSTR_VALVE_OP					0x02    /**< Master Valve Operation. Event parameter value: 0=closed, 1=open*/
#define ZW_ALRM_EVT_SHORT_CCT						0x03    /**< Valve Short Circuit*/
#define ZW_ALRM_EVT_MSTR_SHORT_CCT					0x04    /**< Master Valve Short Circuit*/
#define ZW_ALRM_EVT_CUR_ALRM						0x05    /**< Valve Current Alarm. Event parameter value: 1=no data, 2=below low treshold,
                                                                 3=above high treshold, 4=max*/
#define ZW_ALRM_EVT_MSTR_CUR_ALRM					0x06    /**< Master Valve Current Alarm. Event parameter value same as Valve Current Alarm*/

/** Z-wave alarm event for Weather*/
#define ZW_ALRM_EVT_RAIN							0x01    /**< Rain*/
#define ZW_ALRM_EVT_MOIST							0x02    /**< Moisture*/
#define ZW_ALRM_EVT_FREEZE							0x03    /**< Freeze*/

/** Z-wave alarm event for Irrigation*/
#define ZW_ALRM_EVT_SCHED_START						0x01    /**< Schedule Started. Event parameter value is schedule ID*/
#define ZW_ALRM_EVT_SCHED_FIN						0x02    /**< Schedule Finished. Event parameter value is schedule ID*/
#define ZW_ALRM_EVT_VLV_TBL_RUN_START				0x03    /**< Valve Table Run Started. Event parameter value is valve table ID*/
#define ZW_ALRM_EVT_VLV_TBL_RUN_FIN					0x04    /**< Valve Table Run Finished. Event parameter value is valve table ID*/
#define ZW_ALRM_EVT_DEV_UNCONFIG					0x05    /**< Device is not Configured*/

/** Z-wave alarm event for Gas*/
#define ZW_ALRM_EVT_COMBUST_GAS_DET_L				0x01    /**< Combustible Gas detected with location info*/
#define ZW_ALRM_EVT_COMBUST_GAS_DET					0x02    /**< Combustible Gas detected with unknown location info*/
#define ZW_ALRM_EVT_TOXIC_GAS_DET_L					0x03    /**< Toxic Gas detected with location info*/
#define ZW_ALRM_EVT_TOXIC_GAS_DET					0x04    /**< Toxic Gas detected with unknown location info*/
#define ZW_ALRM_EVT_GAS_ALRM_TEST					0x05    /**< Gas Alarm Test*/
#define ZW_ALRM_EVT_GAS_ALRM_REPLACE				0x06    /**< Replacement Required*/

/** Z-wave alarm event for Pest Control*/
#define ZW_ALRM_EVT_TRAP_ARMED_L					0x01    /**< Trap armed with location info*/
#define ZW_ALRM_EVT_TRAP_ARMED						0x02    /**< Trap armed*/
#define ZW_ALRM_EVT_TRAP_REARM_REQ_L				0x03    /**< Trap re-arm required with location info*/
#define ZW_ALRM_EVT_TRAP_REARM_REQ					0x04    /**< Trap re-arm required*/
#define ZW_ALRM_EVT_PEST_DET_L						0x05    /**< Pest detected with location info*/
#define ZW_ALRM_EVT_PEST_DET						0x06    /**< Pest detected*/
#define ZW_ALRM_EVT_PEST_EXTERMINATED_L				0x07    /**< Pest exterminated with location info*/
#define ZW_ALRM_EVT_PEST_EXTERMINATED				0x08    /**< Pest exterminated*/

/** Z-wave alarm event for Light Sensor*/
#define ZW_ALRM_EVT_LIGHT_DET						0x01    /**< Light detected*/
#define ZW_ALRM_EVT_COLOR_TRANS_DET					0x02    /**< Light color transition detected*/

/**
@}
*/

/** @name Alarm_param
* Z-wave alarm parameter type definition
@{
*/
#define ZW_ALRM_PARAM_LOC               1    /**< Node location UTF-8 string (NULL terminated)*/
#define ZW_ALRM_PARAM_USRID             2    /**< User id*/
#define ZW_ALRM_PARAM_OEM_ERR_CODE      3    /**< OEM proprietary system failure code */
#define ZW_ALRM_PARAM_PROPRIETARY		4	 /**< Proprietary event parameters */
#define ZW_ALRM_PARAM_EVENT_ID		    5	 /**< Event id which is no more active*/
#define ZW_ALRM_PARAM_UNKNOWN			0xFF /**< Unknown alarm event parameters. It could be from a higher version of CC,
												  or the device violates the spec and send parameters when Spec does not define*/
/**
@}
*/

/** @name Alarm_status
* Z-wave alarm status definition
@{
*/
#define ZW_ALRM_STS_DEACTIVATED         0    /**< Unsolicited alarm report is deactivated (push mode) or
                                                  report message carries valid notification information (pull mode)*/
#define ZW_ALRM_STS_ACTIVATED           0xFF /**< Unsolicited alarm report is activated (push mode) */
#define ZW_ALRM_STS_NO_PEND_NOTICE      0xFE /**< Report message does not carry valid notification information. The queue is empty (pull mode) */
/**
@}
*/

/** Alarm */
typedef struct
{
	uint8_t     type;           /**< Vendor specific alarm type*/
	uint8_t     level;          /**< Vendor specific alarm level*/
	uint8_t     ex_info;        /**< Flag to indicate the following extended info fields are valid.
                                     1= valid; 0= invalid (no extended info fields)*/
	/*extended info fields*/
	uint8_t     ex_zensr_nodeid;/**< Zensor Net source node id. This field is 0 if device is not based on Zensor Net*/
	uint8_t     ex_status;      /**< Z-wave alarm status (ZW_ALRM_STS_XXX) */
	uint8_t     ex_type;        /**< Z-wave alarm type (ZW_ALRM_XXX) */
	uint8_t     ex_event;       /**< Z-wave alarm event (ZW_ALRM_EVT_XXX) */
	uint8_t		ex_has_sequence;/**< Flag to indicate whether this report has sequence number as stored in "ex_sequence_no".
									 1=valid; 0 = invalid */
	uint8_t		ex_sequence_no;	/**< Z-wave alarm sequence number. Only valid if field ex_has_sequence is 1.*/
	uint8_t     ex_evt_len;     /**< Z-wave alarm event parameter length. Zero if the event has no parameter */
	uint8_t     ex_evt_type;    /**< Z-wave alarm event parameter type (ZW_ALRM_PARAM_XXX) */
	uint8_t     ex_evt_prm[1];  /**< Z-wave alarm event parameter place holder*/
}
zwalrm_t, *zwalrm_p;

// 4924
/** @name Thermostat_fan_mode
* Thermostat Fan Mode definition
@{
*/
#define ZW_THRMO_FAN_MD_AUTO_LO     0       /**< Turn the manual fan operation off unless turned on by the furnace or AC.
                                                 Lower speed is selected in case it is a two-speed fan.*/
#define ZW_THRMO_FAN_MD_LOW         1       /**< Turn the manual fan to low speed*/
#define ZW_THRMO_FAN_MD_AUTO_HI     2       /**< Turn the manual fan operation off unless turned on by the furnace or AC.
                                                 High speed is selected in case it is a two-speed fan.*/
#define ZW_THRMO_FAN_MD_HI          3       /**< Turn the manual fan to high speed*/
#define ZW_THRMO_FAN_MD_AUTO_MID    4       /**< Turn the manual fan operation off unless turned on by the furnace or AC.
                                                 Medium speed is selected in case it is a three-speed fan.*/
#define ZW_THRMO_FAN_MD_MID         5       /**< Turn the manual fan to medium speed*/
#define ZW_THRMO_FAN_MD_CIR         6       /**< Turn the manual fan operation off unless turned on by the circulation
                                                 algorithms.*/
#define ZW_THRMO_FAN_MD_HUM_CIR     7       /**< Turn the manual fan operation off unless turned on by the humidity
                                                 circulation algorithms.*/
#define ZW_THRMO_FAN_MD_LEFT_RIGHT  8       /**< Turn the manual fan operation off unless turned on by the manufacturer specific
                                                 "left & right" circulation algorithms.*/
#define ZW_THRMO_FAN_MD_UP_DOWN     9       /**< Turn the manual fan operation off unless turned on by the manufacturer specific
                                                 "up & down" circulation algorithms.*/
#define ZW_THRMO_FAN_MD_QUIET       10      /**< Turn the manual fan operation off unless turned on by the manufacturer specific
                                                 "quiet" algorithms.*/
/**
@}
*/

// 5017
/** @name Thermostat_fan_state
*  Thermostat Fan State definition
@{
*/
#define ZW_THRMO_FAN_STA_IDLE       0       /**< Idle */
#define ZW_THRMO_FAN_STA_LOW        1       /**< Running / Running Low - Lower speed is selected in case it is a two-speed fan*/
#define ZW_THRMO_FAN_STA_HI         2       /**< Running High - High speed is selected in case it is a two-speed fan*/
#define ZW_THRMO_FAN_STA_MEDIUM     3       /**< Running Medium*/
#define ZW_THRMO_FAN_STA_CIR        4       /**< Circulation Mode*/
#define ZW_THRMO_FAN_STA_CIR_HUMID  5       /**< Humidity Circulation Mode*/
#define ZW_THRMO_FAN_STA_CIR_RL     6       /**< Right-Left Circulation Mode*/
#define ZW_THRMO_FAN_STA_CIR_UP_DN  7       /**< Up-down Circulation Mode*/
#define ZW_THRMO_FAN_STA_CIR_QUIET  8       /**< Quiet Circulation Mode*/

// 5060
/** @name Thermostat_mode
*  Thermostat Mode definition
@{
*/
#define ZW_THRMO_MD_OFF             0       /**< System is off */
#define ZW_THRMO_MD_HEAT            1       /**< Only heating will occur*/
#define ZW_THRMO_MD_COOL            2       /**< Only cooling will occur */
#define ZW_THRMO_MD_AUTO            3       /**< Auto*/
#define ZW_THRMO_MD_AUX_HEAT        4       /**< Auxiliary/Emergency Heat- the thermostat may be put into Aux
                                                 heat mode simply to use a more efficient secondary heat source when
                                                 there are no failures of the compressor or heat pump unit itself.*/
#define ZW_THRMO_MD_RESUME          5       /**< The system will resume from last active mode.*/
#define ZW_THRMO_MD_FAN             6       /**< Only cycle fan to circulate air. */
#define ZW_THRMO_MD_FURNACE         7       /**< Only furnace */
#define ZW_THRMO_MD_DRY_AIR         8       /**< The system will cycle cooling in relation to the room and set point
                                                 temperatures in order to remove moisture from ambient*/
#define ZW_THRMO_MD_MOIST_AIR       9       /**< Humidification */
#define ZW_THRMO_MD_AUTO_CHANGEOVER 10      /**< Heating or cooling will come on according to the auto changeover setpoint*/
#define ZW_THRMO_MD_ENE_SAVE_HEAT   11      /**< Energy Save Mode Heating will occur */
#define ZW_THRMO_MD_ENE_SAVE_COOL   12      /**< Energy Save Mode Cooling will occur.*/
#define ZW_THRMO_MD_AWAY            13      /**< special Heating Mode, i.e. preventing water from freezing in
                                                 forced water systems.*/
#define ZW_THRMO_MD_FULL_PWR        15      /**< SPEED UP / FULL POWER heating or cooling mode will be activated when
                                                 temperature exceeds FULL POWER set point*/
#define ZW_THRMO_MD_MANF_SPECIFIC   31      /**< Vendor specific thermostat mode*/


/**
@}
*/

// 5155
/** @name Thermostat_op_state
*  Thermostat Operating State definition
@{
*/
#define ZW_THRMO_OP_STA_IDLE       0       /**< Idle */
#define ZW_THRMO_OP_STA_HEAT       1       /**< Heating*/
#define ZW_THRMO_OP_STA_COOL       2       /**< Cooling*/
#define ZW_THRMO_OP_STA_FAN        3       /**< Fan only */
#define ZW_THRMO_OP_STA_PD_HEAT    4       /**< Pending Heat*/
#define ZW_THRMO_OP_STA_PD_COOL    5       /**< Pending Cool*/
#define ZW_THRMO_OP_STA_VENT       6       /**< Vent/Economizer */
#define ZW_THRMO_OP_STA_AUX_HEAT   7       /**< Aux heating*/
#define ZW_THRMO_OP_STA_HEAT_2     8       /**< Second stage heating*/
#define ZW_THRMO_OP_STA_COOL_2     9       /**< Second stage cooling*/
#define ZW_THRMO_OP_STA_AUX_HEAT_2 10      /**< Second stage aux heating*/
#define ZW_THRMO_OP_STA_AUX_HEAT_3 11      /**< Third stage aux heating*/
/**
@}
*/

// 5332
/**
@}
@defgroup If_Thrm_Setpoint Thermostat Setpoint Interface APIs
Used to control and read settings of thermostat setpoint
@ingroup zwarecapi
@{
*/

/** @name Thermostat_setpoint_type
*  Thermostat Setpoint Type definition
@{
*/
#define ZW_THRMO_SETP_TYP_HEATING           1       /**< Heating */
#define ZW_THRMO_SETP_TYP_COOLING           2       /**< Cooling */
#define ZW_THRMO_SETP_TYP_FURNACE           7       /**< Furnace */
#define ZW_THRMO_SETP_TYP_DRY               8       /**< Dry air */
#define ZW_THRMO_SETP_TYP_MOIST             9       /**< Moist air */
#define ZW_THRMO_SETP_TYP_AUTO_CHANGEOVER   10      /**< Auto changeover */
#define ZW_THRMO_SETP_TYP_ENE_SAVE_HEAT     11      /**< Energy Save Heating */
#define ZW_THRMO_SETP_TYP_ENE_SAVE_COOL     12      /**< Energy Save Cooling */
#define ZW_THRMO_SETP_TYP_AWAY_HEAT         13      /**< Away heating */
#define ZW_THRMO_SETP_TYP_AWAY_COOL         14      /**< Away cooling */
#define ZW_THRMO_SETP_TYP_FULL_POWER        15      /**< Full power */
/**
@}
*/

/** @name Thermostat_setpoint_unit
*  Thermostat Setpoint Unit definition
@{
*/
#define ZW_THRMO_SETP_UNIT_C           0       /**< Celsius */
#define ZW_THRMO_SETP_UNIT_F           1       /**< Fahrenheit */
/**
@}
*/


/** Thermostat setpoint data */
typedef struct
{
	uint8_t     type;		/**< ZW_THRMO_SETP_TYP_XXX */
	uint8_t     precision;  /**< Decimal places of the value.  The decimal
                                 value 1025 with precision 2 is therefore equal to 10.25.*/
	uint8_t     unit;	    /**< ZW_THRMO_SETP_UNIT_XXX */
	uint8_t     size;	    /**< Data size: 1,2,or 4 bytes*/
	uint8_t     data[4];	/**< Setpoint data (a signed number) with the first byte as MSB */
}
zwsetp_t, *zwsetp_p;

// 5669
/**
@}
@defgroup If_Protection  Protection Interface APIs
Protection command that can be used to protect a device from unauthorized control
@ingroup zwarecapi
@{
*/

/** @name Local_protect_state
*  Local Protection State definition
@{
*/
#define ZW_LPROT_UNPROT         0       /**< Unprotected - The device is not protected,
                                             and can be operated normally via the user interface. */
#define ZW_LPROT_SEQ            1       /**< Protection by sequence - The device is protected by
                                             altering the way the device normally is operated into a more
                                             complicated sequence of actions */
#define ZW_LPROT_NO_CTL         2       /**< No operation possible - It is not possible at all to control
                                             a device directly via the user interface.*/

/**
@}
*/

/** @name RF_protect_state
*  RF Protection State (version 2) definition
@{
*/
#define ZW_RFPROT_UNPROT        0       /**< Unprotected - The device must accept and respond to all RF Commands.*/
#define ZW_RFPROT_NO_CTL        1       /**< No RF control - all runtime Commands are ignored by the device.
                                             The device must still respond with status on requests.*/
#define ZW_RFPROT_NO_RESP       2       /**< No RF response at all. The device will not even reply to status requests.*/
/**
@}
*/

// 5866
/** Application Busy Status */
#define ZW_BSY_STS_TRY          0       /**< Try again later */
#define ZW_BSY_STS_TRY_WAIT     1       /**< Try again in Wait Time seconds */
#define ZW_BSY_STS_Q            2       /**< Request queued, will be executed later */

// 5888
/** Application Rejected Status */
#define ZW_RJ_STS_REJECT        0       /**< Supported command rejected by the application in the receiving node */

// 6717
/**
@}
@defgroup Csc Central Scene Interface APIs
Central Scene notification includes sequence number, scene number and key attributes.
Central Scene notification is only available through unsolicited report. There is no get command
to retrieve this report.
@ingroup zwarecapi
@{
*/

/** Central Scene Notification */
typedef struct
{
	uint8_t     seqNo;          /**< Sequence number. Incremented each time a new report is issued by the device. */
	uint8_t     keyAttr;        /**< Key attribute. ZW_CSC_KEY_ATTRIB_XXX.*/
	uint8_t     sceneNo;        /**< Scene Number. Actual scene identifier.*/
	uint8_t     slow_rfsh;      /**< Slow refresh of "Key Held Down" notification. Non-zero=enable; 0=disable.
                                     If disabled:
                                     A new "Key Held Down" notification MUST be sent by CSC every 200ms until the key is released.
                                     If not receiving a new "Key Held Down" notification within 400ms after the most recent
                                     "Key Held Down" notification, a "Key Release" notification must be assumed.

                                     If enabled:
                                     A new "Key Held Down" notification MUST be sent by CSC every 55 seconds until the key is released.
                                     If not receiving a new "Key Held Down" notification within 60 seconds after the most recent
                                     "Key Held Down" notification, a "Key Release" notification must be assumed.
                                */
}
zwcsc_notif_t, *zwcsc_notif_p;

//******************************************************************************
//** function **
//******************************************************************************



#ifdef __cplusplus
}
#endif
#endif
