/*=======================================================================================
** File Name:  wise_app.h
**
** Title:  Header File for WISE Application
**
** $Author:    fsw
** $Revision: 1.1 $
** $Date:      2020-04-25
**
** Purpose:  To define WISE's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-04-25 | fsw | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _WISE_APP_H_
#define _WISE_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "wise_platform_cfg.h"
#include "wise_mission_cfg.h"
#include "wise_private_ids.h"
#include "wise_private_types.h"
#include "wise_perfids.h"
#include "wise_msgids.h"
#include "wise_msg.h"



/*
** Local Defines
*/


#define WISE_TIMEOUT_MSEC    1000


/* Application Configurations */

/* Louvers will fail to actuate 25% of the time. */
#define WISE_LVR_FAIL_PCT (25)

/* Louvers will stick in their commanded position 5% of the time. */
#define WISE_LVR_STICK_PCT (5)

/* The instrument safe range is 0 - 45 degrees C.  */
#define WISE_TEMP_MAX (45.0)
#define WISE_TEMP_MIN (0.0)


/* Observations can happen between 15-35 degrees C. */
#define WISE_OBS_TEMP_MAX (35.0)
#define WISE_OBS_TEMP_MIN (15.0)


/* Temp starts +/- 5 from the temp midpoint. */
#define WISE_START_TEMP ((rand() % 10) + (((WISE_OBS_TEMP_MAX - WISE_OBS_TEMP_MIN) / 2.0) - 5.0))


/* Observations take 10 seconds to complete. */
#define WISE_OBS_COUNT (10)

/* 6 minor damage tokens result in instrument destruction. */
#define WISE_MAX_DAMAGE_COUNT (6)


/* Capacitor discharge takes 3 seconds to initiate. */
#define WISE_CAP_DISCHARGE_DELAY (3)



/* Power System Definitions. */

#define WISE_CAP_A 0
#define WISE_CAP_B 1
#define WISE_CAP_C 2
#define WISE_CAP_VALID(x) ((x >= WISE_CAP_A) && (x <= WISE_CAP_C))

#define WISE_CAP_CHARGING    (0)
#define WISE_CAP_LEAKING     (1)
#define WISE_CAP_DISCHARGING (2)
#define WISE_CAP_BROKEN      (3)



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


/* Observation System Definitions */

#define WISE_SBC_OFF       (0)
#define WISE_SBC_POWERED   (1)
#define WISE_SBC_OBSERVING (2)
#define WISE_SBC_ERROR     (3)

#define WISE_INTR_DMG_NONE   (0)
#define WISE_INSTR_DMG_MINOR (1)
#define WISE_INSTR_DMG_MAJOR (2)


/*
** Local Structure Declarations
*/
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[WISE_EVT_CNT];

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
       Data structure should be defined in wise/fsw/src/wise_private_types.h */
    WISE_InData_t   InData;

    /* Output data - to be published at the end of a Wakeup cycle.
       Data structure should be defined in wise/fsw/src/wise_private_types.h */
    WISE_OutData_t  OutData;

    /* Housekeeping telemetry - for downlink only.
       Data structure should be defined in wise/fsw/src/wise_msg.h */
    WISE_HkTlm_t  HkTlm;

    /* TODO:  Add declarations for additional private data here */
} WISE_AppData_t;

/*
** External Global Variables
*/

/*
** Global Variables
*/

/*
** Local Variables
*/

char *gCapNames[3] = {"CAP_A", "CAP_B", "CAP_C"};
char *gHtrNames[2] = {"HTR_A", "HTR_B"};
char *gLvrNames[2] = {"LVR_A", "LVR_B"};


char *gCapStateNames[4] = {"CHARGING","LEAKING","DISCHARGING","BROKEN"};
char *gSbcStateNames[4] = {"OFF","POWERED","OBSERVING","ERROR"};
char *gHtrStateNames[2] = {"ON", "OFF"};
char *gLvrStateNames[2] = {"OPEN", "CLOSED"};

uint8  gWiseActiveCap;
float  gWiseTemp;
float  gWiseCapCharge[3];
uint8  gWiseCapState[3];
uint8  gWiseCapDelay[3];
uint8  gWiseHtrState[2];
uint8  gWiseLvrState[2];
uint8  gWiseIntrState;
uint8  gWiseSbcState;
uint8  gWiseObsPending;
uint8  gWiseObsWritten;
uint8  gWiseObsCount;
uint8  gWiseDamageCount;
uint8  gWiseLvrStuck[2];
uint8  gNumDischargingCap;
uint8  gNumChargingCap;


/*
** Local Function Prototypes
*/
int32  WISE_InitApp(void);
int32  WISE_InitEvent(void);
int32  WISE_InitData(void);
int32  WISE_InitPipe(void);

void  WISE_AppMain(void);

void  WISE_CleanupCallback(void);

int32  WISE_RcvMsg(int32 iBlocking);

void  WISE_ProcessNewData(void);
void  WISE_ProcessNewCmds(void);
void  WISE_ProcessNewAppCmds(CFE_SB_Msg_t*);

void  WISE_ReportHousekeeping(void);
void  WISE_SendOutData(void);

boolean  WISE_VerifyCmdLength(CFE_SB_Msg_t*, uint16);




void WISE_SetActiveCap(uint8 cap);
void WISE_DischargeCapacitor(uint8 cap);
void WISE_ToggleHtr(uint8 htr);
void WISE_ToggleLvr(uint8 lvr);
void WISE_StartObs(void);
void WISE_StopObs(void);
void WISE_SendObs(void);
void WISE_DestroyInstrument(void);
void WISE_DamageInstrument(void);
void WISE_SimulateOneCapacitor(uint8 cap_idx);
void WISE_SimulateCapacitors(void);
void WISE_ProcessWakeup(void);
void WISE_UpdateCapStatus(void);
void WISE_SetCapState(uint8 cap, uint8 state);
void WISE_AlterCapCharge(uint8 cap, float change);
void WISE_AlterCapChargePct(uint8 cap, float pct);
void WISE_CalculateThermal(void);
void WISE_HandleObservation(void);
void ProcessThermMsg(THERM_WISE_OutTlm_t*);

#endif /* _WISE_APP_H_ */

/*=======================================================================================
** End of file wise_app.h
**=====================================================================================*/
    
