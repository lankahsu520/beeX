# Configuration
## Sequence

   ```sequence
   echo -> dongle: MCTT (JKEY_ISSUEID_CC_CONFIGURATION)
   dongle --> echo: MCTT (JKEY_ISSUEID_CC_CONFIGURATION)
   ```

## API
   ```
#define JVAL_METHODID_EVENT                      0
#define JVAL_METHODID_PUT                        1

#define JKEY_ISSUEID_CC_CONFIGURATION            0x00097001 // configuration
   ```
### Command - JKEY_ISSUEID_COMMAND|JKEY_ISSUEID_CC_CONFIGURATION
   ```
void zwifd_act_config(Commander_t *commander, uint8_t param_num, uint8_t use_default, int32_t val)
   ```
#### Sample
   ```

   ```
### Report - JKEY_ISSUEID_CC_CONFIGURATION
   ```
typedef struct
{
    uint8_t     param_num;	    /**< parameter number */
    uint8_t     size;	        /**< data size: 1,2,or 4 bytes*/
    uint8_t     data[4];	    /**< data (a signed or unsigned number, depending on parameter number data format) with the first byte (i.e. data[0]) is the most significant byte*/
    uint8_t     use_default;	/**< only valid for configuration set command. Parameter flag: 1=use default factory setting and ignore data[]; 0=use the value in data[]*/
} zwconfig_t, *zwconfig_p;

void zwifd_config_report(Reporter_t *reporter, zwconfig_p param)
	idx = 0;
	n_issueitem.data[idx++] = param->use_default; // value
	n_issueitem.data[idx++] = param->param_num; // value
	n_issueitem.data[idx++] = param->size; // value
	n_issueitem.data[idx++] = param->data[0]; // value
	n_issueitem.data[idx++] = param->data[1]; // value
	n_issueitem.data[idx++] = param->data[2]; // value
	n_issueitem.data[idx++] = param->data[3]; // value
	n_issueitem.data_len = idx;
   ```
#### Sample
   ```

   ```

# Log
## MCTT
   ```
   ```

## TOPIC
   ```
   ```

