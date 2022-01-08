# Infrared
## Sequence

   ```sequence
   echo -> dongle: MCTT (JKEY_ISSUEID_CC_INFRARED)
   dongle --> echo: MCTT (JKEY_ISSUEID_CC_INFRARED)
   ```

## API
   ```
#define JVAL_METHODID_EVENT                      0
#define JVAL_METHODID_PUT                        1

#define JKEY_ISSUEID_CC_INFRARED                 0x000A9501 // infrared
   ```
### Command - JVAL_METHODID_PUT & JKEY_ISSUEID_CC_INFRARED
   ```
typedef struct
{
	uint8_t cmd;
	uint8_t state;
	uint8_t key[LEN_OF_NAME32];
	uint16_t size;
	uint8_t data[LEN_OF_BUF1024];
} ir_manager_t, *ir_manager_p;

void zwifd_act_infrared(Commander_t *commander, ir_manager_p val_p)
	idx = 0;
	n_issueitem.data[idx++] = val_p->cmd; // value
	n_issueitem.data[idx++] = val_p->state; // value
	SAFE_MEMCPY(n_issueitem.data+idx, val_p->key, LEN_OF_NAME32, LEN_OF_NAME32);
	idx += LEN_OF_NAME32;
	big_endian2byte(2, val_p->size, (unsigned char *)n_issueitem.data+idx); // size
	idx += 2;
	if (val_p->size>0)
	{
		SAFE_MEMCPY(n_issueitem.data+idx, val_p->data, val_p->size, LEN_OF_BUF1024);
		idx += val_p->size; // value
	}
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
reporter_set_nodeid_ex(reporter, UID_to_NODEID(uid), epid);
commander_cp_to(commander, reporter);
commander_cp_frm(commander, master_reporter_get());
zwifd_act_infrared(commander, &ir_manager);
   ```
### Report - JVAL_METHODID_EVENT & JKEY_ISSUEID_CC_INFRARED
   ```
typedef struct
{
	uint8_t cmd;
	uint8_t state;
	uint8_t key[LEN_OF_NAME32];
	uint16_t size;
	uint8_t data[LEN_OF_BUF1024];
} ir_manager_t, *ir_manager_p;

void zwifd_infrared_report(Reporter_t *reporter, ir_manager_p val_p, time_t ts)
	idx = 0;
	n_issueitem.data[idx++] = val_p->cmd; // value
	n_issueitem.data[idx++] = val_p->state; // value
	SAFE_MEMCPY(n_issueitem.data+idx, val_p->key, LEN_OF_NAME32, LEN_OF_NAME32);
	idx += LEN_OF_NAME32;
	big_endian2byte(2, val_p->size, (unsigned char *)n_issueitem.data+idx); // size
	idx += 2;
	SAFE_MEMCPY(n_issueitem.data+idx, val_p->data, val_p->size, LEN_OF_BUF1024);
	idx += val_p->size; // value
	n_issueitem.timestamp = (uint32_t)ts;
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```
reporter_set_nodeid_ex(ZWARE_REPORTER(), ifd_p->nodeid, ifd_p->epid);
zwifd_infrared_report(ZWARE_REPORTER(), val_p, ts);
   ```

# Log
## MCTT
   ```
20210711 095153 [1/65533-ECHO] - --> 080027331B19/FFFFFFFF-FFFF-FFFF-FFFF/FFFFFFFF-FFFF-FFFF-FFFF/0->080027331B19/FFFFFFFF-FFFF-FFFF-FFFF/0/0 - command=infrared,0x52,0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
20210711 095153 [0/51-TIQIAA] - <-- 080027331B19/3414603F-1648-43BA-90F4-8EFE81917DAE/1/0 - zevent=1.0,infrared,R,19-RECV,,0
20210711 095154 [0/51-TIQIAA] - <-- 080027331B19/3414603F-1648-43BA-90F4-8EFE81917DAE/1/0 - zevent=1.0,infrared,C,19-RECV,,0
20210711 095159 [0/51-TIQIAA] - <-- 080027331B19/3414603F-1648-43BA-90F4-8EFE81917DAE/1/0 - zevent=1.0,infrared,R,19-RECV,,0
20210711 095200 [0/51-TIQIAA] - <-- 080027331B19/3414603F-1648-43BA-90F4-8EFE81917DAE/1/0 - zevent=1.0,infrared,C,19-RECV,,0
20210711 095201 [0/51-TIQIAA] - <-- 080027331B19/3414603F-1648-43BA-90F4-8EFE81917DAE/1/0 - zevent=1.0,infrared,R,19-RECV,,0
20210711 095202 [0/51-TIQIAA] - <-- 080027331B19/3414603F-1648-43BA-90F4-8EFE81917DAE/1/0 - zevent=1.0,infrared,C,19-RECV,,0
20210711 095209 [0/51-TIQIAA] - <-- 080027331B19/3414603F-1648-43BA-90F4-8EFE81917DAE/1/0 - zevent=1.0,infrared,D,19-RECV,,270
   ```

## TOPIC
   ```
[7316/7325] bee_subscribe_put:93 - (topic: 1/B827EB40D4FD/3414603F-1648-43BA-90F4-8EFE81917DAE/1/0/000A9501, payload: {"cmd":83,"data":"FFFFFFFFBE7F7F1BA521A566A566A521A521A521A521A567A567A567A521A567A521A567A567A521A521A567A521A521A567A521A521A521A567A521A567A566A521A566A566A566A57F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F55FFFFFFFFBF7F0BA57F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F2DFFFFFFFFBF7F0BA57F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F2CFFFFFFFFBE7F0BA57F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F2DFFFFFFFFBE7F0DA413","key":"off","state":9})

   ```

