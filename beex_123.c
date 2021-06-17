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
#include <signal.h>
#include <getopt.h>

#include "beex_123.h"

// ** app **
static int is_quit = 0;

#ifdef USE_BEEX_123_UV
static uv_loop_t *uv_loop = NULL;
uv_timer_t uv_timer_1sec_fd;
//uv_timer_t uv_timer_60secs_fd;
uv_timer_t uv_timer_30mins_fd;
#endif

char iface_dev[LEN_OF_NAME_DEV] = "eth0";
char iface_mac[LEN_OF_MAC]= "";
char save_path[LEN_OF_FILENAME256] = "";

#ifdef USE_MCTT_BEE
// (MQTT sub) --> queen_bee
void bee_subscribe_get(Honeycomb_t *honeycomb_ctx, char *topic, char *payload)
{
	DBG_IF_LN("(topic: %s, payload: %s)", topic, payload);
	honeycomb_subscribe_get(honeycomb_ctx, topic, payload);
}

// (MQTT sub) --> queen_bee -> honeycomb (MCTT Server) -> dongle
void bee_subscribe_put(Honeycomb_t *honeycomb_ctx, char *topic, char *payload)
{
	DBG_IF_LN("(topic: %s, payload: %s)", topic, payload);
	honeycomb_act_helper(honeycomb_ctx, topic, payload);
}

int bee_topic_add_uuid_caller(char *topic, char *value)
{
	int ret = 0;
	if ( (topic) && (value) )
	{
		DBG_IF_LN("(topic: %s, value: %s)", topic, value);
	}

	return ret;
}

int bee_topic_del_uuid_caller(char *topic, char *value)
{
	int ret = 0;
	if ( (topic) && (value) )
	{
		DBG_IF_LN("(topic: %s, value: %s)", topic, value);
	}

	return ret;
}

int bee_topic_add_node_caller(char *topic, char *value)
{
	int ret = 0;
	if ( (topic) && (value) )
	{
		DBG_IF_LN("(topic: %s, value: %s)", topic, value);
	}

	return ret;
}

int bee_topic_del_node_caller(char *topic, char *value)
{
	int ret = 0;
	if ( (topic) && (value) )
	{
		DBG_IF_LN("(topic: %s, value: %s)", topic, value);
	}

	return ret;
}

// dongle -> honeycomb (MCTT Server) -> queen_bee -> (MQTT pub)
int bee_topic_issue_caller(char *topic, char *value)
{
	int ret = 0;
	if ( (topic) && (value) )
	{
		DBG_IF_LN("(topic: %s, value: %s)", topic, value);
	}

	return ret;
}

static void bee_notify_aborting(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_adding(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_basic(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_basic_set(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_battery(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_csc(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_configuration(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_door(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_luminance(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_humidity(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_learning(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_dimmer(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_meter(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_motion(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_node(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_nop(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_nw(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_op(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_replacing(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_rmfailed(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_removing(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_reseting(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_smoke(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_switch(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_tempature(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_tamper(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_wake_up_interval(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_wake_up_notification(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static void bee_notify_water(void *userdata, IssueItem_t *c_issueitem)
{
	DBG_DB_LN(DBG_TXT_ENTER);
}

static Notify_t notify_bee = {
	//.watch_item_cb[NOTIFY_FN_ID_READY] = master_notify_ready,
	.watch_item_cb[NOTIFY_FN_ID_ADDING] = bee_notify_adding,
	.watch_item_cb[NOTIFY_FN_ID_REMOVING] = bee_notify_removing,
	.watch_item_cb[NOTIFY_FN_ID_ABORTING] = bee_notify_aborting,
	.watch_item_cb[NOTIFY_FN_ID_LEARNING] = bee_notify_learning,
	.watch_item_cb[NOTIFY_FN_ID_NW] = bee_notify_nw,
	.watch_item_cb[NOTIFY_FN_ID_RESETING] = bee_notify_reseting,
	.watch_item_cb[NOTIFY_FN_ID_OP] = bee_notify_op,

	.watch_item_cb[NOTIFY_FN_ID_REPLACING] = bee_notify_replacing,
	.watch_item_cb[NOTIFY_FN_ID_RMFAILED] = bee_notify_rmfailed,

	.watch_item_cb[NOTIFY_FN_ID_NODE] = bee_notify_node,
	.watch_item_cb[NOTIFY_FN_ID_NOP] = bee_notify_nop,

	.watch_item_cb[NOTIFY_FN_ID_TAMPER] = bee_notify_tamper,
	.watch_item_cb[NOTIFY_FN_ID_DR_WIN] = bee_notify_door,
	.watch_item_cb[NOTIFY_FN_ID_MOTION] = bee_notify_motion,

	.watch_item_cb[NOTIFY_FN_ID_SMOKE] = bee_notify_smoke,
	.watch_item_cb[NOTIFY_FN_ID_WATER] = bee_notify_water,
	.watch_item_cb[NOTIFY_FN_ID_TEMPATURE] = bee_notify_tempature,
	.watch_item_cb[NOTIFY_FN_ID_LUMINANCE] = bee_notify_luminance,
	.watch_item_cb[NOTIFY_FN_ID_HUMIDITY] = bee_notify_humidity,

	.watch_item_cb[NOTIFY_FN_ID_BASIC] = bee_notify_basic,
	.watch_item_cb[NOTIFY_FN_ID_BASIC_SET] = bee_notify_basic_set,
	.watch_item_cb[NOTIFY_FN_ID_SWITCH] = bee_notify_switch,
	.watch_item_cb[NOTIFY_FN_ID_DIMMER] = bee_notify_dimmer,
	.watch_item_cb[NOTIFY_FN_ID_METER] = bee_notify_meter,
	.watch_item_cb[NOTIFY_FN_ID_CENTRAL_SCENE_NOTIFICATION] = bee_notify_csc,
	.watch_item_cb[NOTIFY_FN_ID_CONFIGURATION] = bee_notify_configuration,
	.watch_item_cb[NOTIFY_FN_ID_BATTERY] = bee_notify_battery,

	.watch_item_cb[NOTIFY_FN_ID_WAKE_UP_INTERVAL] = bee_notify_wake_up_interval,
	.watch_item_cb[NOTIFY_FN_ID_WAKE_UP_NOTIFICATION] = bee_notify_wake_up_notification,
};

static Reporter_t reporter_bee = {
	.methodid = JVAL_METHODID_EVENT,

	//.id_frm.c_macid = c_macid,
	//.id_frm.c_uuid = c_uuid,
	.id_frm.c_nodeid = JVAL_C_NODEID_1,
	.id_frm.epid = 0
};

static Commander_t commander_bee = {
	.methodid = JVAL_METHODID_PUT,

	//.id_frm.c_macid = c_macid,
	//.id_frm.c_uuid = c_uuid,
	.id_frm.c_nodeid = JVAL_C_NODEID_0,
	.id_frm.epid = JVAL_EPID_BASE,

	//.id_to.c_macid = c_macid,
	//.id_to.c_uuid = c_uuid,
	.id_to.c_nodeid = JVAL_C_NODEID_BROADCAST,
	.id_to.epid = JVAL_EPID_BASE, 
};

Honeycomb_t beex123_data = {
	.keepothers = 0,
	.uuid_root = BEEX_UUID,
	.order = 0,
	.watch = 0,
	.logging = 0,

	.reporter = &reporter_bee,
	.commander = &commander_bee,
	.notify = &notify_bee,

	.topic_add_uuid_caller = bee_topic_add_uuid_caller,
	.topic_del_uuid_caller = bee_topic_del_uuid_caller,
	.topic_add_node_caller = bee_topic_add_node_caller,
	.topic_del_node_caller = bee_topic_del_node_caller,
	.topic_issue_caller = bee_topic_issue_caller,
};

#endif

static int app_quit(void)
{
	return is_quit;
}

#ifdef USE_BEEX_123_UV
void timer_1sec_loop(uv_timer_t *handle)
{
	static int count = 0;
	count++;
	//DBG_TR_LN("(count: %d)", count);

	time_t now_t = time((time_t *)NULL);
	struct tm *now_tm = localtime(&now_t);
	DBG_DB_LN("(%02d:%02d:%02d)", now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec);

	if (app_quit()==1)
	{
		//SAFE_UV_TIMER_STOP(handle);
		SAFE_UV_TIMER_CLOSE(handle, NULL);
		DBG_WN_LN("%s (%s)", DBG_TXT_BYE_BYE, TAG);
	}
	else
	{
#ifdef USE_MCTT_BEE_DEMO
		uint8_t nodeid = 3;
		uint8_t epid = 0;
		reporter_set_nodeid_ex(&reporter_bee, nodeid, epid);
		if ( (now_t%2) == 0 )
		{
			zwifd_alrm_tamper_open_report_bin(&reporter_bee);
		}
		else
		{
			zwifd_alrm_tamper_closed_report_bin(&reporter_bee);
		}
#endif
	}
}

#if (0)
void timer_60secs_loop(uv_timer_t *handle)
{
	static int count = 0;
	count++;
	DBG_TR_LN("(count: %d)", count);

	if (app_quit()==1)
	{
		//SAFE_UV_TIMER_STOP(handle);
		SAFE_UV_TIMER_CLOSE(handle, NULL);
		DBG_WN_LN("%s (%s)", DBG_TXT_BYE_BYE, TAG);
	}
}
#endif

void timer_30mins_loop(uv_timer_t *handle)
{
	app_save();

	if (app_quit()==1)
	{
		//SAFE_UV_TIMER_STOP(handle);
		SAFE_UV_TIMER_CLOSE(handle, NULL);
		DBG_WN_LN("%s (%s)", DBG_TXT_BYE_BYE, TAG);
	}
}

void app_stop_uv(uv_async_t *handle, int force)
{
	static int is_free = 0;
	if ( (is_free==0) && (app_quit()==1) )
	{
		is_free = 1;
		if (uv_loop)
		{
			SAFE_UV_TIMER_CLOSE(&uv_timer_1sec_fd, NULL);
			//SAFE_UV_TIMER_CLOSE(&uv_timer_60secs_fd, NULL);
			SAFE_UV_TIMER_CLOSE(&uv_timer_30mins_fd, NULL);

			if (handle)
			{
				SAFE_UV_CLOSE(handle, NULL);
			}

			if (force)
			{
				SAFE_UV_LOOP_CLOSE(uv_loop);
			}
		}
	}
}

#ifdef USE_ASYNC_CREATE
uv_async_t uv_async_fd;

void async_loop(uv_async_t *handle)
{
	DBG_IF_LN(DBG_TXT_ENTER);

	app_stop_uv(handle, 0);
}
#endif

#endif

void app_save(void)
{
#ifdef USE_YK_P2P_MACRO
	{// we dont need to save every 30mins.
		honeycomb_save(&beex123_data);
	}
#endif
}

static void app_set_quit(int mode)
{
	is_quit = mode;
}

static void app_stop(void)
{
	if (app_quit()==0)
	{
		app_set_quit(1);

#ifdef USE_MCTT_BEE
		{
			honeycomb_stop(&beex123_data);
			honeycomb_close(&beex123_data);
		}
#endif

#ifdef USE_BEEX_123_UV
#ifdef USE_ASYNC_CREATE
		SAFE_UV_ASYNC(&uv_async_fd);
#else
		app_stop_uv(NULL, 1);
#endif
#endif
		DBG_WN_LN("%s (%s)", DBG_TXT_BYE_BYE, TAG);
	}
}

static void app_loop(void)
{
	{
		chainX_if_hwaddr(iface_dev, iface_mac, "");
		//SAFE_SPRINTF(iface_mac, "%s", MAC_BROADCAST);
	}

#ifdef USE_MCTT_BEE
	{
		SAFE_SPRINTF(beex123_data.iface_mac, "%s", iface_mac);
		SAFE_SPRINTF(beex123_data.json_f_mctt, "%s/%s", save_path, FILENAME_OF_MCTT_HONEYCOMB);

		commander_set_frm_mac(&commander_bee, iface_mac);

		honeycomb_open(&beex123_data);
		honeycomb_register(&beex123_data);
	}
#endif

#ifdef USE_BEEX_123_UV
	{
		SAFE_UV_LOOP_INIT(uv_loop);

#ifdef USE_ASYNC_CREATE
		SAFE_UV_ASYNC_INIT(uv_loop, &uv_async_fd, async_loop);
#endif

		SAFE_UV_TIMER_INIT(uv_loop, &uv_timer_1sec_fd);
		SAFE_UV_TIMER_START(&uv_timer_1sec_fd, timer_1sec_loop, 1000, 1000); // 1st: 1, 2nd: 1+1, 3rd: 1+1+1, 4th: 1+1+1+1 .....
		// please use the another callback function.
		//SAFE_UV_TIMER_INIT(uv_loop, &uv_timer_60secs_fd);
		//SAFE_UV_TIMER_START(&uv_timer_60secs_fd, timer_60secs_loop, 60*1000, 60*1000); // 1st: 1, 2nd: 1+60, 3rd: 1+60+60, 4th: 1+60+60+60 .....
		SAFE_UV_TIMER_INIT(uv_loop, &uv_timer_30mins_fd);
		SAFE_UV_TIMER_START(&uv_timer_30mins_fd, timer_30mins_loop, 30*60*1000, 30*60*1000); // 1st:30, 2nd: 30+30, 3rd: 30+30+30, 4th: 30+30+30+30 .....

		SAFE_UV_LOOP_RUN(uv_loop);
		SAFE_UV_LOOP_CLOSE(uv_loop);
	}
#else
	while ( is_quit == 0)
	{
#ifdef USE_MCTT_BEE_DEMO
		time_t now_t = time((time_t *)NULL);
		uint8_t nodeid = 3;
		uint8_t epid = 0;
		reporter_set_nodeid_ex(&reporter_bee, nodeid, epid);
		if ( (now_t%2) == 0 )
		{
			zwifd_alrm_tamper_open_report_bin(&reporter_bee);
		}
		else
		{
			zwifd_alrm_tamper_closed_report_bin(&reporter_bee);
		}
#endif
		sleep(1);
	}
#endif

	goto exit_loop;

exit_loop:
	app_stop();
}

static int app_init(void)
{
	int ret = 0;

	return ret;
}

static void app_exit(void)
{
	app_stop();
}

static void app_signal_handler(int signum)
{
	DBG_ER_LN("(signum: %d)", signum);
	switch (signum)
	{
		case SIGINT:
		case SIGTERM:
		case SIGHUP:
			app_stop();
			break;
		case SIGPIPE:
			break;

		case SIGUSR1:
			break;

		case SIGUSR2:
			dbg_lvl_round();
			DBG_ER_LN("dbg_lvl_get(): %d", dbg_lvl_get());
			DBG_ER_LN("(Version: %s)", version_show());
			break;
	}
}

static void app_signal_register(void)
{
	signal(SIGINT, app_signal_handler );
	signal(SIGTERM, app_signal_handler );
	signal(SIGHUP, app_signal_handler );
	signal(SIGUSR1, app_signal_handler );
	signal(SIGUSR2, app_signal_handler );

	signal(SIGPIPE, SIG_IGN );
}

int option_index = 0;
const char* short_options = "d:s:i:h";
static struct option long_options[] =
{
	{ "debug",       required_argument,   NULL,    'd'  },
	{ "savepath",    required_argument,   NULL,    's'  },
	{ "iface",       required_argument,   NULL,    'i'  },

	{ "help",        no_argument,         NULL,    'h'  },
	{ 0,             0,                      0,    0    }
};

static void app_showusage(int exit_code)
{
	printf( "Usage: %s\n"
					"  -d, --debug       debug level\n"
					"  -s, --savepath    save path\n"
					"  -i, --iface       iface\n"
					"  -h, --help\n", TAG);
	printf( "Version: %s\n", version_show());
	printf( "Example:\n"
					"  %s -s /tmp -d 2 \n", TAG);
	exit(exit_code);
}

static void app_ParseArguments(int argc, char **argv)
{
	int opt;

	while((opt = getopt_long (argc, argv, short_options, long_options, &option_index)) != -1)
	{
		switch (opt)
		{
			case 's':
				if (optarg)
				{
					SAFE_SPRINTF(save_path, "%s", optarg);
				}
				break;
			case 'i':
				if (optarg)
				{
					SAFE_SPRINTF(iface_dev, "%s", optarg);
				}
				break;
			case 'd':
				if (optarg)
				{
					dbg_lvl_set(atoi(optarg));
				}
				break;
			default:
				app_showusage(-1);
				break;
		}
	}
}

// ./bin/beex_123 -d 2 -s /tmp -i enp0s9 
int main(int argc, char *argv[])
{
	app_ParseArguments(argc, argv);
	app_signal_register();
	atexit(app_exit);

	SAFE_STDOUT_NONE();
	if ( app_init() == -1 )
	{
		return -1;
	}

	app_loop();

	DBG_WN_LN(DBG_TXT_BYE_BYE);
	return 0;
}
