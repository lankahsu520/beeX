# Node
## Sequence

   ```sequence
   dongle --> echo: MCTT (JKEY_ISSUEID_NODE)
   ```

## API
   ```
#define JVAL_METHODID_EVENT                      0
#define JVAL_METHODID_PUT                        1

#define JKEY_ISSUEID_NODE                        0x00000010
   ```
### Command - JKEY_ISSUEID_COMMAND|JKEY_ISSUEID_NODE
   ```
void controller_act_node_see_you(Commander_t *commander)
void controller_act_node_hello(Commander_t *commander)
   ```
#### Sample
   ```

   ```
### Report - JKEY_ISSUEID_NODE
   ```
void controller_status_node_info(Reporter_t *reporter, zwnoded_p noded_p)
void controller_status_node_vendor(Reporter_t *reporter, uint16_t vid, uint16_t ptype, uint16_t pid)
void controller_status_node_alive(Reporter_t *reporter)
void controller_status_node_sleeping(Reporter_t *reporter)
void controller_status_node_type(Reporter_t *reporter, zwplus_info_t *zwplus_info_p)
void controller_status_node_generic(Reporter_t *reporter, uint8_t generic, uint8_t specific)
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
