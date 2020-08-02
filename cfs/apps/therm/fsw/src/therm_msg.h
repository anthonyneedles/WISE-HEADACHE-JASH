/*=======================================================================================
** File Name:  therm_msg.h
**
** Title:  Message Definition Header File for THERM Application
**
** $Author:    JASH
** $Revision: 1.1 $
** $Date:      2020-07-29
**
** Purpose:  To define THERM's command and telemetry message defintions
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-07-29 | JASH | Build #: Code Started
**
**=====================================================================================*/

#ifndef _THERM_MSG_H_
#define _THERM_MSG_H_

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
** THERM command codes
*/
#define THERM_NOOP_CC                 0
#define THERM_RESET_CC                1
#define THERM_RST_LVR_CNT_CC          2

/*
** WISE command codes THERM uses
*/
#define WISE_HTR_TOGGLE_CC		     3
#define WISE_LVR_TOGGLE_CC         4

/*
** Local Structure Declarations
*/
typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              usCmdCnt;
    uint8              usCmdErrCnt;

    /* TODO:  Add declarations for additional housekeeping data here */
    uint8              lvrAAttempts;
    uint8              lvrBAttempts;

} THERM_HkTlm_t;

typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              usCmdCnt;
    uint8              usCmdErrCnt;

    /* TODO:  Add declarations for additional housekeeping data here */

    uint16             wiseSbcState;
    uint16             wiseDamage;
    uint16             wiseActiveCap;
    uint16             wiseCapA_State;
    uint16             wiseCapA_Charge;
    uint16             wiseCapB_State;
    uint16             wiseCapB_Charge;
    uint16             wiseCapC_State;
    uint16             wiseCapC_Charge;
    uint16             wiseHtrA_State;
    uint16             wiseHtrB_State;
    uint16             wiseLvrA_State;
    uint16             wiseLvrB_State;
    uint16             wiseSSR_Count;
    int16              wiseTemp;
} WISE_HkTlm_t;

typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              cmdCode;
    uint16             target;
} THERM_WISE_OutTlm_t;

typedef struct
{
    uint8              CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8              lrvSelect; // 0 for LRV A
                                  // 1 for LRV B
} THERM_RST_LVR_CNT_t;


#endif /* _THERM_MSG_H_ */

/*=======================================================================================
** End of file therm_msg.h
**=====================================================================================*/
