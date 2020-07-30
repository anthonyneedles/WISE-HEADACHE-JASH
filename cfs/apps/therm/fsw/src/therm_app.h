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



/*
** Local Defines
*/
#define THERM_TIMEOUT_MSEC    1000

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

    /* TODO:  Add declarations for additional private data here */
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
    