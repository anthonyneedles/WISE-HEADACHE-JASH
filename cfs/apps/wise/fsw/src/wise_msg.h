/*=======================================================================================
** File Name:  wise_msg.h
**
** Title:  Message Definition Header File for WISE Application
**
** $Author:    fsw
** $Revision: 1.1 $
** $Date:      2020-04-25
**
** Purpose:  To define WISE's command and telemetry message defintions 
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-04-25 | fsw | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _WISE_MSG_H_
#define _WISE_MSG_H_

/*
** Pragmas
*/

/*
** Include Files
*/



/*
** Local Defines
*/

/*
** WISE command codes
*/
#define WISE_NOOP_CC                 0

#define WISE_CAP_ACTIVE_CC			 1
#define WISE_CAP_DISCHARGE_CC		 2
#define WISE_HTR_TOGGLE_CC		     3
#define WISE_LVR_TOGGLE_CC           4
#define WISE_OBS_START_CC            5
#define WISE_OBS_STOP_CC             6
#define WISE_POWER_SBC_CC            7
#define WISE_SEND_OBS_CC             8
#define WISE_TLM_RESET_CNTS_CC       9

/*
** Local Structure Declarations
*/
typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              usCmdCnt;
    uint8              usCmdErrCnt;

    /* TODO:  Add declarations for additional housekeeping data here */

    uint16			    wiseSbcState;
    uint16				wiseDamage;
    uint16				wiseActiveCap;
    uint16				wiseCapA_State;
    uint16				wiseCapA_Charge;
    uint16				wiseCapB_State;
    uint16				wiseCapB_Charge;
    uint16				wiseCapC_State;
    uint16				wiseCapC_Charge;
    uint16				wiseHtrA_State;
    uint16				wiseHtrB_State;
    uint16				wiseLvrA_State;
    uint16				wiseLvrB_State;
    uint16              wiseSSR_Count;
    int16				wiseTemp;
} WISE_HkTlm_t;


typedef struct
{
	uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];
	uint16 target;
} WISE_ParmCmd_t;

typedef struct
{
	uint8 TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8 cmdCode;
	uint16 target;
} THERM_WISE_OutTlm_t;



#endif /* _WISE_MSG_H_ */

/*=======================================================================================
** End of file wise_msg.h
**=====================================================================================*/
    
