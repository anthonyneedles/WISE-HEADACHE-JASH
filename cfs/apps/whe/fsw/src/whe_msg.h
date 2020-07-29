/*******************************************************************************
** File:
**   whe_msg.h
**
** Purpose: 
**  Define WHE Messages and info
**
** Notes:
**
**
*******************************************************************************/
#ifndef _whe_msg_h_
#define _whe_msg_h_

/*
** WHE command codes
*/

#define WHE_NOOP_CC                 0

#define WHE_CAP_A_ACTIVE_CC         1
#define WHE_CAP_A_DISCHARGE_CC      2
#define WHE_CAP_B_ACTIVE_CC         3
#define WHE_CAP_B_DISCHARGE_CC      4

#define WHE_OBS_START_CC            5
#define WHE_OBS_STOP_CC             6

#define WHE_POWER_SBC_CC            7

#define WHE_THERM_HTR_OFF_CC        8
#define WHE_THERM_HTR_ON_CC         9
#define WHE_THERM_LOUVER_CLOSE_CC  10
#define WHE_THERM_LOUVER_OPEN_CC   11

#define WHE_TLM_RESET_CNTS_CC      12













/*************************************************************************/
/*
** Type definition (generic "no arguments" command)
*/
typedef struct
{
   uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];

} WHE_NoArgsCmd_t;

/*************************************************************************/
/*
** Type definition (WHE App housekeeping)
*/
typedef struct 
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              whe_command_error_count;
    uint8              whe_command_count;
    uint8			   whe_cap_a_charge;
    uint8			   whe_cap_a_state;
    uint8			   whe_cap_b_charge;
    uint8			   whe_cap_b_state;
    uint8			   whe_sbc_state;
    uint8			   whe_temp;
    uint8			   whe_louver;
    uint8			   whe_htr;
    uint8			   whe_act_cap;
    uint8			   whe_dmg_state;
    uint8                          whe_dmg_cnt;
    uint8                          whe_obs_cnt;
    uint8                          whe_suc_obs;
//    uint8              spare[2];

}   OS_PACK whe_hk_tlm_t  ;

#define WHE_HK_TLM_LNGTH   sizeof ( whe_hk_tlm_t )

#endif /* _whe_msg_h_ */

/************************/
/*  End of File Comment */
/************************/
