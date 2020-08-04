/*=======================================================================================
** File Name:  therm_app.h
**
** Title:  Header File for THERM Application
**
** $Author:    JASH
** $Revision: 1.1 $
** $Date:      2020-07-29
**
** Purpose:  To define THERM's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-07-29 | JASH | Build #: Code Started
**
**=====================================================================================*/

#ifndef _THERM_APP_H_
#define _THERM_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "therm_platform_cfg.h"
#include "therm_mission_cfg.h"
#include "therm_private_ids.h"
#include "therm_private_types.h"
#include "therm_perfids.h"
#include "therm_msgids.h"
#include "therm_msg.h"

#include <wise_msgids.h>


/*
** Local Defines
*/
#define THERM_TIMEOUT_MSEC (1000)

/*
** WISE Defines
*/

/* The instrument safe range is 0 - 45 degrees C.  */
/* We are choosing to maintain the operational temperature 15 - 30 C*/
/* To account for component turn on/off times, the upper and lower*/
/* temperature limits will be set as 18 - 27 C*/

#define WISE_TEMP_MAX_THRESH (5)
#define WISE_TEMP_MIN_THRESH (3)
#define WISE_TEMP_MAX ((30-WISE_TEMP_MAX_THRESH)*10)
#define WISE_TEMP_MIN ((15+WISE_TEMP_MIN_THRESH)*10)

/* Thermal System Definitions */

#define WISE_HTR_A (0)
#define WISE_HTR_B (1)

#define WISE_HTR_VALID(x) ((x >= WISE_HTR_A) && (x <= WISE_HTR_B))

#define WISE_HTR_ON  (0)
#define WISE_HTR_OFF (1)


#define WISE_LVR_A (0)
#define WISE_LVR_B (1)

#define WISE_LVR_VALID(x) ((x >= WISE_LVR_A) && (x <= WISE_LVR_B))

#define WISE_LVR_OPEN   (0)
#define WISE_LVR_CLOSED (1)

#define WISE_LVR_MIN_CHARGE (5*100)

#define WISE_MAX_LVR_ATTEMPTS (3)


/* Observation System Definitions */

#define WISE_SBC_OFF       (0)
#define WISE_SBC_POWERED   (1)
#define WISE_SBC_OBSERVING (2)
#define WISE_SBC_ERROR     (3)

#define WISE_INTR_DMG_NONE   (0)
#define WISE_INSTR_DMG_MINOR (1)
#define WISE_INSTR_DMG_MAJOR (2)

#define WISE_ACTIVE_CAP_A (0)
#define WISE_ACTIVE_CAP_B (1)
#define WISE_ACTIVE_CAP_C (2)

#define WISE_CAP_CHARGING    (0)
#define WISE_CAP_LEAKING     (1)
#define WISE_CAP_DISCHARGING (2)
#define WISE_CAP_BROKEN      (3)

/*
** Local Structure Declarations
*/
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[THERM_EVT_CNT];

    /* CFE scheduling pipe */
    CFE_SB_PipeId_t  SchPipeId;
    uint16           usSchPipeDepth;
    char             cSchPipeName[OS_MAX_API_NAME];

    /* CFE command pipe */
    CFE_SB_PipeId_t  CmdPipeId;
    uint16           usCmdPipeDepth;
    char             cCmdPipeName[OS_MAX_API_NAME];

    /* CFE telemetry pipe */
    CFE_SB_PipeId_t  TlmPipeId;
    uint16           usTlmPipeDepth;
    char             cTlmPipeName[OS_MAX_API_NAME];

    /* Task-related */
    uint32  uiRunStatus;

    /* Input data - from I/O devices or subscribed from other apps' output data.
       Data structure should be defined in therm/fsw/src/therm_private_types.h */
    THERM_InData_t   InData;

    /* Output data - to be published at the end of a Wakeup cycle.
       Data structure should be defined in therm/fsw/src/therm_private_types.h */
    THERM_OutData_t  OutData;

    /* Housekeeping telemetry - for downlink only.
       Data structure should be defined in therm/fsw/src/therm_msg.h */
    THERM_HkTlm_t  HkTlm;

    /* WISE commanding - for sending commands to WISE */
    THERM_To_WISE_ParmCmd_t WISECmd;

    uint8              heaterTglA;
    uint8              heaterTglB;
} THERM_AppData_t;

/*
** External Global Variables
*/

/*
** Global Variables
*/

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/
int32  THERM_InitApp(void);
int32  THERM_InitEvent(void);
int32  THERM_InitData(void);
int32  THERM_InitPipe(void);

void  THERM_AppMain(void);

void  THERM_CleanupCallback(void);

int32  THERM_RcvMsg(int32 iBlocking);

void  THERM_ProcessNewData(void);
void  THERM_ProcessNewCmds(void);
void  THERM_ProcessNewAppCmds(CFE_SB_Msg_t*);

void  THERM_ReportHousekeeping(void);
void  THERM_SendOutData(void);

boolean  THERM_VerifyCmdLength(CFE_SB_Msg_t*, uint16);

#endif /* _THERM_APP_H_ */

/*=======================================================================================
** End of file therm_app.h
**=====================================================================================*/
