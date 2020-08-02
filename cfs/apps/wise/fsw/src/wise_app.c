/*=======================================================================================
** File Name:  wise_app.c
**
** Title:  Function Definitions for WISE Application
**
** $Author:    fsw
** $Revision: 1.1 $
** $Date:      2020-04-25
**
** Purpose:  This source file contains all necessary function definitions to run WISE
**           application.
**
** Functions Defined:
**    Function X - Brief purpose of function X
**    Function Y - Brief purpose of function Y
**    Function Z - Brief purpose of function Z
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to all functions in the file.
**    2. List the external source(s) and event(s) that can cause the funcs in this
**       file to execute.
**    3. List known limitations that apply to the funcs in this file.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-04-25 | fsw | Build #: Code Started
**
**=====================================================================================*/

/*
** Pragmas
*/

/*
** Include Files
*/
#include <string.h>

#include "cfe.h"

#include "wise_platform_cfg.h"
#include "wise_mission_cfg.h"
#include "wise_app.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/

/*
** External Global Variables
*/

/*
** Global Variables
*/
WISE_AppData_t  g_WISE_AppData;

/*
** Local Variables
*/

/*
** Local Function Definitions
*/

/*=====================================================================================
** Name: WISE_InitEvent
**
** Purpose: To initialize and register event table for WISE application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_EVS_Register
**    CFE_ES_WriteToSysLog
**
** Called By:
**    WISE_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_WISE_AppData.EventTbl
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw
**
** History:  Date Written  2020-04-25
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 WISE_InitEvent()
{
    int32  iStatus=CFE_SUCCESS;

    /* Create the event table */
    memset((void*)g_WISE_AppData.EventTbl, 0x00, sizeof(g_WISE_AppData.EventTbl));

    g_WISE_AppData.EventTbl[0].EventID = WISE_RESERVED_EID;
    g_WISE_AppData.EventTbl[1].EventID = WISE_INF_EID;
    g_WISE_AppData.EventTbl[2].EventID = WISE_INIT_INF_EID;
    g_WISE_AppData.EventTbl[3].EventID = WISE_ILOAD_INF_EID;
    g_WISE_AppData.EventTbl[4].EventID = WISE_CDS_INF_EID;
    g_WISE_AppData.EventTbl[5].EventID = WISE_CMD_INF_EID;

    g_WISE_AppData.EventTbl[ 6].EventID = WISE_ERR_EID;
    g_WISE_AppData.EventTbl[ 7].EventID = WISE_INIT_ERR_EID;
    g_WISE_AppData.EventTbl[ 8].EventID = WISE_ILOAD_ERR_EID;
    g_WISE_AppData.EventTbl[ 9].EventID = WISE_CDS_ERR_EID;
    g_WISE_AppData.EventTbl[10].EventID = WISE_CMD_ERR_EID;
    g_WISE_AppData.EventTbl[11].EventID = WISE_PIPE_ERR_EID;
    g_WISE_AppData.EventTbl[12].EventID = WISE_MSGID_ERR_EID;
    g_WISE_AppData.EventTbl[13].EventID = WISE_MSGLEN_ERR_EID;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(g_WISE_AppData.EventTbl,
                               WISE_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("WISE - Failed to register with EVS (0x%08X)\n", (unsigned int) iStatus);
    }

    return (iStatus);
}

/*=====================================================================================
** Name: WISE_InitPipe
**
** Purpose: To initialize all message pipes and subscribe to messages for WISE application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_CreatePipe
**    CFE_SB_Subscribe
**    CFE_ES_WriteToSysLog
**
** Called By:
**    WISE_InitApp
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_WISE_AppData.usSchPipeDepth
**    g_WISE_AppData.cSchPipeName
**    g_WISE_AppData.SchPipeId
**    g_WISE_AppData.usCmdPipeDepth
**    g_WISE_AppData.cCmdPipeName
**    g_WISE_AppData.CmdPipeId
**    g_WISE_AppData.usTlmPipeDepth
**    g_WISE_AppData.cTlmPipeName
**    g_WISE_AppData.TlmPipeId
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw
**
** History:  Date Written  2020-04-25
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 WISE_InitPipe()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe */
    g_WISE_AppData.usSchPipeDepth = WISE_SCH_PIPE_DEPTH;
    memset((void*)g_WISE_AppData.cSchPipeName, '\0', sizeof(g_WISE_AppData.cSchPipeName));
    strncpy(g_WISE_AppData.cSchPipeName, "WISE_SCH_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to Wakeup messages */
    iStatus = CFE_SB_CreatePipe(&g_WISE_AppData.SchPipeId,
                                 g_WISE_AppData.usSchPipeDepth,
                                 g_WISE_AppData.cSchPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(WISE_WAKEUP_MID, g_WISE_AppData.SchPipeId, CFE_SB_Default_Qos, 1);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("WISE - Sch Pipe failed to subscribe to WISE_WAKEUP_MID. (0x%08X)\n", (unsigned int) iStatus);
            goto WISE_InitPipe_Exit_Tag;
        }

    }
    else
    {
        CFE_ES_WriteToSysLog("WISE - Failed to create SCH pipe (0x%08X)\n", (unsigned int) iStatus);
        goto WISE_InitPipe_Exit_Tag;
    }

    /* Init command pipe */
    g_WISE_AppData.usCmdPipeDepth = WISE_CMD_PIPE_DEPTH ;
    memset((void*)g_WISE_AppData.cCmdPipeName, '\0', sizeof(g_WISE_AppData.cCmdPipeName));
    strncpy(g_WISE_AppData.cCmdPipeName, "WISE_CMD_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&g_WISE_AppData.CmdPipeId,
                                 g_WISE_AppData.usCmdPipeDepth,
                                 g_WISE_AppData.cCmdPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(WISE_CMD_MID, g_WISE_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("WISE - CMD Pipe failed to subscribe to WISE_CMD_MID. (0x%08X)\n", (unsigned int) iStatus);
            goto WISE_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_Subscribe(WISE_SEND_HK_MID, g_WISE_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("WISE - CMD Pipe failed to subscribe to WISE_SEND_HK_MID. (0x%08X)\n", (unsigned int) iStatus);
            goto WISE_InitPipe_Exit_Tag;
        }

    }
    else
    {
        CFE_ES_WriteToSysLog("WISE - Failed to create CMD pipe (0x%08X)\n", (unsigned int) iStatus);
        goto WISE_InitPipe_Exit_Tag;
    }

    /* Init telemetry pipe */
    g_WISE_AppData.usTlmPipeDepth = WISE_TLM_PIPE_DEPTH;
    memset((void*)g_WISE_AppData.cTlmPipeName, '\0', sizeof(g_WISE_AppData.cTlmPipeName));
    strncpy(g_WISE_AppData.cTlmPipeName, "WISE_TLM_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to telemetry messages on the telemetry pipe */
    iStatus = CFE_SB_CreatePipe(&g_WISE_AppData.TlmPipeId,
                                 g_WISE_AppData.usTlmPipeDepth,
                                 g_WISE_AppData.cTlmPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* TODO:  Add CFE_SB_Subscribe() calls for other apps' output data here.
        **
        ** Examples:
        **     CFE_SB_Subscribe(GNCEXEC_OUT_DATA_MID, g_WISE_AppData.TlmPipeId);
        */
    }
    else
    {
        CFE_ES_WriteToSysLog("WISE - Failed to create TLM pipe (0x%08X)\n", (unsigned int) iStatus);
        goto WISE_InitPipe_Exit_Tag;
    }

WISE_InitPipe_Exit_Tag:
    return (iStatus);
}

/*=====================================================================================
** Name: WISE_InitData
**
** Purpose: To initialize global variables used by WISE application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_InitMsg
**
** Called By:
**    WISE_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_WISE_AppData.InData
**    g_WISE_AppData.OutData
**    g_WISE_AppData.HkTlm
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw
**
** History:  Date Written  2020-04-25
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 WISE_InitData()
{
    int32  iStatus=CFE_SUCCESS;

    /* Init input data */
    memset((void*)&g_WISE_AppData.InData, 0x00, sizeof(g_WISE_AppData.InData));

    /* Init output data */
    memset((void*)&g_WISE_AppData.OutData, 0x00, sizeof(g_WISE_AppData.OutData));
    CFE_SB_InitMsg(&g_WISE_AppData.OutData,
                   WISE_OUT_DATA_MID, sizeof(g_WISE_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&g_WISE_AppData.HkTlm, 0x00, sizeof(g_WISE_AppData.HkTlm));
    CFE_SB_InitMsg(&g_WISE_AppData.HkTlm,
                   WISE_HK_TLM_MID, sizeof(g_WISE_AppData.HkTlm), TRUE);

    return (iStatus);
}

/*=====================================================================================
** Name: WISE_InitApp
**
** Purpose: To initialize all data local to and used by WISE application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_WriteToSysLog
**    CFE_EVS_SendEvent
**    OS_TaskInstallDeleteHandler
**    WISE_InitEvent
**    WISE_InitPipe
**    WISE_InitData
**
** Called By:
**    WISE_AppMain
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw
**
** History:  Date Written  2020-04-25
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 WISE_InitApp()
{
    int32  iStatus=CFE_SUCCESS;

    g_WISE_AppData.uiRunStatus = CFE_ES_APP_RUN;

    iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("WISE - Failed to register the app (0x%08X)\n", (unsigned int) iStatus);
        goto WISE_InitApp_Exit_Tag;
    }

    if ((WISE_InitEvent() != CFE_SUCCESS) ||
        (WISE_InitPipe() != CFE_SUCCESS) ||
        (WISE_InitData() != CFE_SUCCESS))
    {
        iStatus = -1;
        goto WISE_InitApp_Exit_Tag;
    }

    /* Install the cleanup callback */
    OS_TaskInstallDeleteHandler((void*)&WISE_CleanupCallback);


    /* Initialize local variables */
    gWiseActiveCap = WISE_CAP_A;
    gWiseTemp = WISE_START_TEMP;

    gWiseCapCharge[WISE_CAP_A] = 0;
    gWiseCapCharge[WISE_CAP_B] = 0;
    gWiseCapCharge[WISE_CAP_C] = 0;

    gWiseCapState[WISE_CAP_A] = WISE_CAP_CHARGING;
    gWiseCapState[WISE_CAP_B] = WISE_CAP_CHARGING;
    gWiseCapState[WISE_CAP_C] = WISE_CAP_CHARGING;


    gWiseCapDelay[WISE_CAP_A] = 0;
    gWiseCapDelay[WISE_CAP_B] = 0;
    gWiseCapDelay[WISE_CAP_C] = 0;

    gWiseHtrState[WISE_HTR_A] = WISE_HTR_OFF;
    gWiseHtrState[WISE_HTR_B] = WISE_HTR_OFF;

    gWiseLvrState[WISE_LVR_A] = WISE_LVR_CLOSED;
    gWiseLvrState[WISE_LVR_B] = WISE_LVR_CLOSED;
    gWiseLvrStuck[WISE_LVR_A] = 0;
    gWiseLvrStuck[WISE_LVR_B] = 0;

    gWiseObsPending = 0;
    gWiseObsWritten = 0;
    gWiseObsCount = 0;

    gWiseIntrState = WISE_INTR_DMG_NONE;

    gWiseSbcState = WISE_SBC_OFF;

    gWiseDamageCount = 0;

    gNumDischargingCap = 0;
    gNumChargingCap = 3;

WISE_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(WISE_INIT_INF_EID, CFE_EVS_INFORMATION,
                          "WISE - Application initialized");
    }
    else
    {
        CFE_ES_WriteToSysLog("WISE - Application failed to initialize\n");
    }

    return (iStatus);
}

/*=====================================================================================
** Name: WISE_CleanupCallback
**
** Purpose: To handle any neccesary cleanup prior to application exit
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    TBD
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw
**
** History:  Date Written  2020-04-25
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void WISE_CleanupCallback()
{
    /* TODO:  Add code to cleanup memory and other cleanup here */
}

/*=====================================================================================
** Name: WISE_RcvMsg
**
** Purpose: To receive and process messages for WISE application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**    CFE_ES_PerfLogEntry
**    CFE_ES_PerfLogExit
**    WISE_ProcessNewCmds
**    WISE_ProcessNewData
**    WISE_SendOutData
**
** Called By:
**    WISE_Main
**
** Global Inputs/Reads:
**    g_WISE_AppData.SchPipeId
**
** Global Outputs/Writes:
**    g_WISE_AppData.uiRunStatus
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw
**
** History:  Date Written  2020-04-25
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 WISE_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(WISE_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, g_WISE_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(WISE_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
	{
            case WISE_WAKEUP_MID:
                WISE_ProcessNewCmds();
                WISE_ProcessNewData();

                /* TODO:  Add more code here to handle other things when app wakes up */
            	WISE_ProcessWakeup();
            	WISE_ReportHousekeeping();

                /* The last thing to do at the end of this Wakeup cycle should be to
                   automatically publish new output. */
                WISE_SendOutData();
                break;

            default:
                CFE_EVS_SendEvent(WISE_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "WISE - Recvd invalid SCH msgId (0x%08X)", MsgId);
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* If there's no incoming message, you can do something here, or nothing */
    }
    else
    {
        /* This is an example of exiting on an error.
        ** Note that a SB read error is not always going to result in an app quitting.
        */
        CFE_EVS_SendEvent(WISE_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "WISE: SB pipe read error (0x%08X), app will exit", (unsigned int) iStatus);
        g_WISE_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}

/*=====================================================================================
** Name: WISE_ProcessNewData
**
** Purpose: To process incoming data subscribed by WISE application
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**
** Called By:
**    WISE_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    None
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw
**
** History:  Date Written  2020-04-25
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void WISE_ProcessNewData()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   TlmMsgPtr=NULL;
    CFE_SB_MsgId_t  TlmMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&TlmMsgPtr, g_WISE_AppData.TlmPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            TlmMsgId = CFE_SB_GetMsgId(TlmMsgPtr);
            switch (TlmMsgId)
            {
                /* TODO:  Add code to process all subscribed data here
                **
                ** Example:
                **     case NAV_OUT_DATA_MID:
                **         WISE_ProcessNavData(TlmMsgPtr);
                **         break;
                */

                default:
                    CFE_EVS_SendEvent(WISE_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "WISE - Recvd invalid TLM msgId (0x%08X)", TlmMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(WISE_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "WISE: CMD pipe read error (0x%08X)", iStatus);
            g_WISE_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}

/*=====================================================================================
** Name: WISE_ProcessNewCmds
**
** Purpose: To process incoming command messages for WISE application
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**    WISE_ProcessNewAppCmds
**    WISE_ReportHousekeeping
**
** Called By:
**    WISE_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    None
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw
**
** History:  Date Written  2020-04-25
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void WISE_ProcessNewCmds()
{
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, g_WISE_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case WISE_CMD_MID:
                    WISE_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                case WISE_SEND_HK_MID:
                    WISE_ReportHousekeeping();
                    break;

                /* TODO:  Add code to process other subscribed commands here
                **
                ** Example:
                **     case CFE_TIME_DATA_CMD_MID:
                **         WISE_ProcessTimeDataCmd(CmdMsgPtr);
                **         break;
                */

                default:
                    CFE_EVS_SendEvent(WISE_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "WISE - Recvd invalid CMD msgId (0x%08X)", CmdMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(WISE_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "WISE: CMD pipe read error (0x%08X)", iStatus);
            g_WISE_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}

/*=====================================================================================
** Name: WISE_ProcessNewAppCmds
**
** Purpose: To process command messages targeting WISE application
**
** Arguments:
**    CFE_SB_Msg_t*  MsgPtr - new command message pointer
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_GetCmdCode
**    CFE_EVS_SendEvent
**
** Called By:
**    WISE_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_WISE_AppData.HkTlm.usCmdCnt
**    g_WISE_AppData.HkTlm.usCmdErrCnt
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw
**
** History:  Date Written  2020-04-25
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void WISE_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32  uiCmdCode=0;
    WISE_ParmCmd_t *CmdPtr = NULL;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);

        switch (uiCmdCode)
        {
            case WISE_NOOP_CC:
                g_WISE_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "WISE - Recvd NOOP cmd (%lu)", uiCmdCode);
                break;

            case WISE_CAP_ACTIVE_CC:
            	if((gWiseSbcState != WISE_SBC_OFF) && (gWiseSbcState != WISE_SBC_ERROR))
            	{
            		CmdPtr = (WISE_ParmCmd_t*) MsgPtr;
            		WISE_SetActiveCap(CmdPtr->target);
            	}
            	break;

            case WISE_CAP_DISCHARGE_CC:
            	if((gWiseSbcState != WISE_SBC_OFF) && (gWiseSbcState != WISE_SBC_ERROR))
            	{
            		CmdPtr = (WISE_ParmCmd_t*) MsgPtr;
            		WISE_DischargeCapacitor(CmdPtr->target);
            	}
            	break;

            case WISE_HTR_TOGGLE_CC:
            	if((gWiseSbcState != WISE_SBC_OFF) && (gWiseSbcState != WISE_SBC_ERROR))
            	{
            		CmdPtr = (WISE_ParmCmd_t*) MsgPtr;
            		WISE_ToggleHtr(CmdPtr->target);
            	}
            	break;

            case WISE_LVR_TOGGLE_CC:
            	if((gWiseSbcState != WISE_SBC_OFF) && (gWiseSbcState != WISE_SBC_ERROR))
            	{
            		CmdPtr = (WISE_ParmCmd_t*) MsgPtr;
            		WISE_ToggleLvr(CmdPtr->target);
            	}
            	break;

            case WISE_OBS_START_CC:
            	if((gWiseSbcState != WISE_SBC_OFF) && (gWiseSbcState != WISE_SBC_ERROR))
            	{
            		WISE_StartObs();
            	}
            	break;

            case WISE_OBS_STOP_CC:
            	if((gWiseSbcState != WISE_SBC_OFF) && (gWiseSbcState != WISE_SBC_ERROR))
            	{
            		WISE_StopObs();
            	}
            	break;

            case WISE_POWER_SBC_CC:
            	if(gWiseSbcState == WISE_SBC_OFF)
            	{
            		gWiseSbcState = WISE_SBC_POWERED;
            		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
            		                                  "WISE - POWER_SBC: Powering SBC");
            	}
            	else
            	{
            		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
            		                  "WISE - POWER_SBC: Failed. SBC currently in %s state",gSbcStateNames[gWiseSbcState]);
            	}
            	break;

            case WISE_SEND_OBS_CC:
            	if((gWiseSbcState != WISE_SBC_OFF) && (gWiseSbcState != WISE_SBC_ERROR))
            	{
            		WISE_SendObs();
            	}
            	break;

            case WISE_TLM_RESET_CNTS_CC:
                g_WISE_AppData.HkTlm.usCmdCnt = 0;
                g_WISE_AppData.HkTlm.usCmdErrCnt = 0;
                CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "WISE - Recvd TLM_RESET_CNTS cmd (%lu)", uiCmdCode);
            	break;


            /* TODO:  Add code to process the rest of the WISE commands here */

            default:
                g_WISE_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(WISE_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "WISE - Recvd invalid cmdId (%lu)", uiCmdCode);
                break;
        }
    }
}

/*=====================================================================================
** Name: WISE_ReportHousekeeping
**
** Purpose: To send housekeeping message
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    WISE_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  GSFC, fsw
**
** History:  Date Written  2020-04-25
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void WISE_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */

	g_WISE_AppData.HkTlm.wiseActiveCap = gWiseActiveCap;
	g_WISE_AppData.HkTlm.wiseSbcState = gWiseSbcState;
	g_WISE_AppData.HkTlm.wiseDamage = gWiseIntrState;
	g_WISE_AppData.HkTlm.wiseCapA_State = gWiseCapState[WISE_CAP_A];
	g_WISE_AppData.HkTlm.wiseCapA_Charge = (uint16) (gWiseCapCharge[WISE_CAP_A]*100.0);
	g_WISE_AppData.HkTlm.wiseCapB_State = gWiseCapState[WISE_CAP_B];
	g_WISE_AppData.HkTlm.wiseCapB_Charge = (uint16) (gWiseCapCharge[WISE_CAP_B]*100.0);
	g_WISE_AppData.HkTlm.wiseCapC_State = gWiseCapState[WISE_CAP_C];
	g_WISE_AppData.HkTlm.wiseCapC_Charge = (uint16) (gWiseCapCharge[WISE_CAP_C]*100.0);
	g_WISE_AppData.HkTlm.wiseHtrA_State = gWiseHtrState[WISE_HTR_A];
	g_WISE_AppData.HkTlm.wiseHtrB_State = gWiseHtrState[WISE_HTR_B];
	g_WISE_AppData.HkTlm.wiseTemp = (uint16) (gWiseTemp * 10.0);
	g_WISE_AppData.HkTlm.wiseLvrA_State = gWiseLvrState[WISE_LVR_A];
	g_WISE_AppData.HkTlm.wiseLvrB_State = gWiseLvrState[WISE_LVR_B];
	g_WISE_AppData.HkTlm.wiseSSR_Count = gWiseObsWritten;

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_WISE_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_WISE_AppData.HkTlm);
}

/*=====================================================================================
** Name: WISE_SendOutData
**
** Purpose: To publish 1-Wakeup cycle output data
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    WISE_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw
**
** History:  Date Written  2020-04-25
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void WISE_SendOutData()
{
    /* TODO:  Add code to update output data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_WISE_AppData.OutData);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_WISE_AppData.OutData);
}

/*=====================================================================================
** Name: WISE_VerifyCmdLength
**
** Purpose: To verify command length for a particular command message
**
** Arguments:
**    CFE_SB_Msg_t*  MsgPtr      - command message pointer
**    uint16         usExpLength - expected command length
**
** Returns:
**    boolean bResult - result of verification
**
** Routines Called:
**    TBD
**
** Called By:
**    WISE_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw
**
** History:  Date Written  2020-04-25
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
boolean WISE_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
{
    boolean bResult=FALSE;
    uint16  usMsgLen=0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            CFE_EVS_SendEvent(WISE_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "WISE - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            g_WISE_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}

/*=====================================================================================
** Name: WISE_AppMain
**
** Purpose: To define WISE application's entry point and main process loop
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_RunLoop
**    CFE_ES_PerfLogEntry
**    CFE_ES_PerfLogExit
**    CFE_ES_ExitApp
**    CFE_ES_WaitForStartupSync
**    WISE_InitApp
**    WISE_RcvMsg
**
** Called By:
**    TBD
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  fsw
**
** History:  Date Written  2020-04-25
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void WISE_AppMain()
{
    /* Register the application with Executive Services */
    CFE_ES_RegisterApp();

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(WISE_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (WISE_InitApp() != CFE_SUCCESS)
    {
        g_WISE_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    } else {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(WISE_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(WISE_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(WISE_MAIN_TASK_PERF_ID);
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&g_WISE_AppData.uiRunStatus) == TRUE)
    {
        WISE_RcvMsg(CFE_SB_PEND_FOREVER);
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(WISE_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(g_WISE_AppData.uiRunStatus);
}



/*
 * Sets the active capacitor.
 */
void WISE_SetActiveCap(uint8 cap)
{
	if( WISE_CAP_VALID(cap))
	{
		gWiseActiveCap = cap;
    	CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "WISE - Active Capacitor set to %s", gCapNames[cap]);
	}
	else
	{
    	CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
                          "WISE - Unknown Capacitor: %d", cap);
	}
}

/*
 * Discharges selected capacitor.
 */
void WISE_DischargeCapacitor(uint8 cap)
{

	if(!WISE_CAP_VALID(cap))
	{
    	CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
                          "WISE - DischargeCapacitor: Unknown Capacitor: %d", cap);
    	return;
	}

	/* Do not discharge active capacitor */
	if(gWiseCapState[cap] == WISE_CAP_DISCHARGING)
	{
		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
				"WISE_- Capacitor %s is already discharging", gCapNames[cap]);
	}
	else
	{
		WISE_SetCapState(cap, WISE_CAP_DISCHARGING);
		gWiseCapDelay[cap] = WISE_CAP_DISCHARGE_DELAY;
		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
				"WISE_- Discharging Capacitor %s in %d seconds", gCapNames[cap], WISE_CAP_DISCHARGE_DELAY);
	}

	/* Discharging active capacitor while observing? */
	if((cap == gWiseActiveCap) && (gWiseSbcState == WISE_SBC_OBSERVING))
	{
		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
				"WISE_- Discharging active capacitor while observing.");

		WISE_DestroyInstrument();
	}
}


/*
 * Toggles given heater.
 */
void WISE_ToggleHtr(uint8 htr)
{
	if( WISE_HTR_VALID(htr) )
	{
		gWiseHtrState[htr] = (gWiseHtrState[htr] == WISE_HTR_ON) ? WISE_HTR_OFF : WISE_HTR_ON;
		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
		                          "WISE - Heater %s is now %s.", gHtrNames[htr], gHtrStateNames[gWiseHtrState[htr]]);
	}
	else
	{
    	CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
                          "WISE - Attempt to command unknown Heater: %d", htr);
	}
}


/*
 * Toggle given louver
 */
void WISE_ToggleLvr(uint8 lvr)
{

	if(!(WISE_LVR_VALID(lvr)))
	{
    	CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
                          "WISE - Attempt to command unknown Louver: %d", lvr);
	}

	/* Attempting to actuate louver will consume 1% off active capacitor */
	WISE_AlterCapCharge(gWiseActiveCap, -1);

	if(gWiseLvrStuck[lvr] == 1)
	{
		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
		                  "WISE - Louver %s stuck in %s position.", gLvrNames[lvr], gLvrStateNames[gWiseLvrState[lvr]]);
		return;
	}


	/* WISE_LVR_FAIL_PCT chance that the louver will not toggle. */
	if( (rand() % 100) < WISE_LVR_FAIL_PCT)
	{
		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
		                          "WISE - Louver %s failed to toggle.", gLvrNames[lvr]);
	}
	else
	{
		gWiseLvrState[lvr] = (gWiseLvrState[lvr] == WISE_LVR_OPEN) ? WISE_LVR_CLOSED : WISE_LVR_OPEN;
		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
		                          "WISE - Louver %s is now %s.", gLvrNames[lvr], gLvrStateNames[gWiseLvrState[lvr]]);
	}

	/* See if Louver sticks */
	if( (rand() % 100) < WISE_LVR_STICK_PCT)
	{
		gWiseLvrStuck[lvr] = 1;
		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
		                          "WISE - Louver %s is now stuck %s.", gLvrNames[lvr], gLvrStateNames[gWiseLvrState[lvr]]);
	}
}


/*
 * Start an observation.
 */
void WISE_StartObs()
{

	/* Make sure the active capacitor is not leaking/destoryed. */

	switch(gWiseSbcState)
	{
		case WISE_SBC_OFF:
			CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
			                  "WISE - Cannot observe when SBC is powered off");
			break;

		case WISE_SBC_POWERED:
			gWiseSbcState = WISE_SBC_OBSERVING;
			gWiseObsCount = WISE_OBS_COUNT;
			CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
						      "WISE - Starting Observation (%d left)", gWiseObsCount);

			if(gWiseCapState[gWiseActiveCap] != WISE_CAP_CHARGING)
			{
				/* Attempting to use leaking, discharging, or broken capacitor for obs. */
				CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
							      "WISE - Active capacitor in state %s during observation. ", gCapStateNames[gWiseActiveCap]);

				WISE_DestroyInstrument();
				return;
			}

			break;

		case WISE_SBC_OBSERVING:
			CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
			                  "WISE - Instrument already observing.");
			break;

		case WISE_SBC_ERROR:
		default:
			CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
						      "WISE - Instrument in an error state.");
			break;
	}
}


void WISE_StopObs()
{
	switch(gWiseSbcState)
	{
		case WISE_SBC_OFF:
			CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
			                  "WISE - SBC is powered off");
			break;

		case WISE_SBC_POWERED:

			CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
						      "WISE - Instrument is not observing");
			break;

		case WISE_SBC_OBSERVING:
			gWiseSbcState = WISE_SBC_POWERED;

			CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
			                  "WISE - Stopping observation (%d was left).", gWiseObsCount);
			gWiseObsCount = 0;
			break;

		case WISE_SBC_ERROR:
		default:
			CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
						      "WISE - Instrument in an error state.");
			break;
	}
}

void WISE_SendObs()
{

  if(gWiseSbcState != WISE_SBC_POWERED)
  {
		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
					      "WISE - Cannot send observation to SSR when SBC is in the %s state.", gSbcStateNames[gWiseSbcState]);
		return;
  }

  if(gWiseObsPending > 0)
  {
	  gWiseObsPending--;
	  gWiseObsWritten++;
	  CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
		  	            "WISE - Observation written to the SSR.");
  }
  else
  {
	  CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
			  	            "WISE - No observations pending to be written.");
  }
}


/*
 * Calculate thermal contributions for this second.
 */

void WISE_CalculateThermal()
{
	float offset = 0.0;

	/* Check for safe thermal environment */
	if(gWiseTemp < WISE_TEMP_MIN)
	{
		offset = WISE_TEMP_MIN - gWiseTemp;
	}
	else if(gWiseTemp > WISE_TEMP_MAX)
	{
		offset = gWiseTemp - WISE_TEMP_MAX;
	}

	if(offset != 0)
	{
		int destroy = (int)(offset * 5.0);
		int damage = (int) (offset * 20.0);
		int val = rand() % 100;

		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
					      "WISE - Instrument temp of %.fC outside of safe range %.2fC - %.2fC", gWiseTemp, WISE_TEMP_MIN, WISE_TEMP_MAX);

		if(val < destroy)
		{
			WISE_DestroyInstrument();
			return;
		}
		else if(val < damage)
		{
			WISE_DamageInstrument();
		}
	}

	if(gWiseSbcState == WISE_SBC_OBSERVING)
	{
		gWiseTemp += 0.4;
	}

	gWiseTemp += 0.25 * (float)gNumDischargingCap;

	if(gWiseLvrState[WISE_LVR_A] == WISE_LVR_OPEN)
	{
		gWiseTemp *= 0.975;
	}
	if(gWiseLvrState[WISE_LVR_B] == WISE_LVR_OPEN)
	{
		gWiseTemp *= 0.975;
	}

	if(gWiseHtrState[WISE_HTR_A] == WISE_HTR_ON)
	{
		gWiseTemp *= 1.05;
	}
	if(gWiseHtrState[WISE_HTR_B] == WISE_HTR_ON)
	{
		gWiseTemp *= 1.05;
	}
}

/*
 * Handle instrument destruction state changes.
 */
void WISE_DestroyInstrument()
{
	gWiseIntrState = WISE_INSTR_DMG_MAJOR;
	gWiseSbcState = WISE_SBC_ERROR;

	CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
				      "WISE - !!!!Instrument destroyed!!!!");
}

/*
 *  Apply damage counter.
 */
void WISE_DamageInstrument()
{
	gWiseDamageCount++;

	CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
				      "WISE - !!!!Instrument damaged: %d of %d!!!!", gWiseDamageCount, WISE_MAX_DAMAGE_COUNT);

	if(gWiseDamageCount > WISE_MAX_DAMAGE_COUNT)
	{
		WISE_DestroyInstrument();
	}
}


/*
 * Handle update for a single capacitor.
 */
void WISE_SimulateOneCapacitor(uint8 cap)
{
	/* Check Cap State */
	if(gWiseCapState[cap] == WISE_CAP_BROKEN)
	{
		return;
	}

	/* If the capacitor is over charged, it might leak or explode. */
	if(gWiseCapCharge[cap] > 100)
	{
		float val = rand() % 100;
		float leak = 0.0;
		float explode = 0.0;

		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
		    		      "WISE - Capacitor %s is over charged (%.2f).", gCapNames[cap], gWiseCapCharge[cap]);


		leak = (((float)gWiseCapCharge[cap]) - 100.0);
		explode = leak / 2.0;

		/* If we exploded... */
		if(val < explode)
		{
			/* Capacitor is now leaking */
			WISE_SetCapState(cap, WISE_CAP_BROKEN);
			CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
			    		      "WISE - Capacitor %s Exploded.", gCapNames[cap]);

			WISE_DestroyInstrument();
		}
		else if (val < leak)
		{
			/* Capacitor is now leaking */
			WISE_SetCapState(cap, WISE_CAP_LEAKING);

			CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
			    		      "WISE - Capacitor %s is leaking with charge of %.2f.", gCapNames[cap], gWiseCapCharge[cap]);

			WISE_DamageInstrument();
		}
	}

	/* Handle case where the capacitor is discharging */
	if(gWiseCapState[cap] == WISE_CAP_DISCHARGING)
	{
		if(gWiseCapDelay[cap] > 0)
		{
			gWiseCapDelay[cap] -= 1;
		}
		else if((cap == gWiseActiveCap) && (gWiseSbcState == WISE_SBC_OBSERVING))
		{
			CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
					"WISE_- Attempt to discharge active capacitor!");
			WISE_SetCapState(cap, WISE_CAP_BROKEN);
		    WISE_DestroyInstrument();
		    return;
		}
		else if(gWiseCapCharge[cap] > 10)
		{
			WISE_AlterCapCharge(cap, -10);
		}
		else
		{
			WISE_AlterCapCharge(cap, -gWiseCapCharge[cap]);

			WISE_SetCapState(cap, WISE_CAP_CHARGING);
			CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
			    		      "WISE - Capacitor %s completed discharge.", gCapNames[cap]);
		}
	}
	else if(gWiseCapState[cap] == WISE_CAP_CHARGING)
	{
		/* should be impossible for this to be 0 */
		if(gNumChargingCap  > 0)
		{
			WISE_AlterCapCharge(cap, (float)3.0/(float)gNumChargingCap);
		}
	}

}

/*
 * Capacitor state chart
 *
 *                CHARGE   LEAK    DISCHARG    BROKEN
 * CHARGE           No      Yes       Yes         Yes
 * LEAK             No       No        No         Yes
 * DISCHARGE       Yes      Yes        No         Yes
 * BROKEN          No        No        No         Yes
 *
 */
void WISE_SetCapState(uint8 cap, uint8 state)
{
	static uint8 matrix[4][4] = {{0,1,1,1},{0,0,0,1}, {1,1,0,1}, {0,0,0,1}};


	if(matrix[gWiseCapState[cap]][state] == 1)
	{
		gWiseCapState[cap] = state;
	}
	else
	{
		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
		    		      "WISE - Cannot change capacitor from state %s to %s", gCapStateNames[cap], gCapStateNames[state]);

	}
}

/*
 * Simulate behavior of all caps for this second.
 */
void WISE_SimulateCapacitors()
{
	WISE_UpdateCapStatus();

	if(gNumDischargingCap > 2)
	{
		float val = rand() % 100;

		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
		    		      "WISE - More than 2 capacitors discharging at once.");

		/* Instrument destroyed */
		if(val < 50)
		{
			WISE_DestroyInstrument();
			return;
		}
		else if (val < 75)
		{
			WISE_DamageInstrument();
		}
	}

   WISE_SimulateOneCapacitor(WISE_CAP_A);
   WISE_UpdateCapStatus();

   WISE_SimulateOneCapacitor(WISE_CAP_B);
   WISE_UpdateCapStatus();

   WISE_SimulateOneCapacitor(WISE_CAP_C);
   WISE_UpdateCapStatus();
}


/*
 * Process when instrument is in observation.
 */
void WISE_HandleObservation()
{
	/* Nothing to do if we are not observing. */
	if(gWiseSbcState != WISE_SBC_OBSERVING)
	{
		return;
	}

	/* Is the active capacitor unusable? */
	if(gWiseCapState[gWiseActiveCap] != WISE_CAP_CHARGING)
	{
		/* Attempting to use leaking, discharging, or broken capacitor for obs. */
		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
					      "WISE - Active capacitor in state %s during observation. ", gCapStateNames[gWiseActiveCap]);

		WISE_DestroyInstrument();
		return;
	}

	/* Is the active capacitor too low? */
	if(gWiseCapCharge[gWiseActiveCap] < 5)
	{
		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
	                      "WISE - Active Capacitor (%s) charge too low (%.2f).", gCapNames[gWiseActiveCap], gWiseCapCharge[gWiseActiveCap]);

		gWiseCapState[gWiseActiveCap] = WISE_CAP_BROKEN;
		WISE_DestroyInstrument();
		return;
	}

	/* Consume more power off the capacitor for the observation. */
	WISE_AlterCapCharge(gWiseActiveCap, -7);

	gWiseObsCount--;
	/* See if observation is cancelled due to thermal issues. */
	if( (gWiseTemp < WISE_OBS_TEMP_MIN) || (gWiseTemp > WISE_OBS_TEMP_MAX))
	{
		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
	                      "WISE - Corrupt observation. Temp %.2f not in range %.2f - %.2f.", gWiseTemp, WISE_OBS_TEMP_MIN, WISE_OBS_TEMP_MAX);
		WISE_StopObs();
	}
	else if(gWiseObsCount == 0)
	{
		gWiseObsPending++;
		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
	                      "WISE - Observation completed and pending SSR.");
		WISE_StopObs();
	}
}

/*
 * Handle overall simulation of instrument for the second.
 */
void WISE_ProcessWakeup()
{

	if((gWiseSbcState == WISE_SBC_OFF) || (gWiseSbcState == WISE_SBC_ERROR))
	{
		return;
	}

	CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_INFORMATION,
                      "WISE - Stepping Instrument Simulation");

	WISE_SimulateCapacitors();

	WISE_HandleObservation();

	WISE_CalculateThermal();
}


/*
 * Utility function
 */

void WISE_AlterCapCharge(uint8 cap, float change)
{
	float new_val;

	new_val = gWiseCapCharge[cap] + change;

	if(new_val < 0)
	{
		gWiseCapCharge[cap] = 0;
		CFE_EVS_SendEvent(WISE_CMD_INF_EID, CFE_EVS_ERROR,
		                  "WISE - Capacitor %s charge exhausted.", gCapNames[cap]);
		WISE_SetCapState(gWiseActiveCap, WISE_CAP_BROKEN);
		WISE_DestroyInstrument();
	}
	else
	{
		gWiseCapCharge[cap] = new_val;
	}
}

/*
 * Helper if we need to deal with percentages.
 */
void WISE_AlterCapChargePct(uint8 cap, float pct)
{
	WISE_AlterCapCharge(cap, (float)(gWiseCapCharge[cap] * pct));
}

/*
 * Brute force count state of charge.
 */
void WISE_UpdateCapStatus()
{
	gNumDischargingCap = 0;
	gNumChargingCap = 0;

	if(gWiseCapState[WISE_CAP_A] == WISE_CAP_CHARGING) {gNumChargingCap++;}
	if(gWiseCapState[WISE_CAP_B] == WISE_CAP_CHARGING) {gNumChargingCap++;}
	if(gWiseCapState[WISE_CAP_C] == WISE_CAP_CHARGING) {gNumChargingCap++;}

	if(gWiseCapState[WISE_CAP_A] == WISE_CAP_DISCHARGING) {gNumDischargingCap++;}
	if(gWiseCapState[WISE_CAP_B] == WISE_CAP_DISCHARGING) {gNumDischargingCap++;}
	if(gWiseCapState[WISE_CAP_C] == WISE_CAP_DISCHARGING) {gNumDischargingCap++;}

}


/*=======================================================================================
** End of file wise_app.c
**=====================================================================================*/

