/*******************************************************************************
** File: whe.c
**
** Purpose:
**   This file contains the source code for the WHE App.
**
*******************************************************************************/

/*
**   Include Files:
*/

#include "whe.h"
#include "whe_perfids.h"
#include "whe_msgids.h"
#include "whe_msg.h"
#include "whe_events.h"
#include "whe_version.h"

/*
** global data
*/

whe_hk_tlm_t       WHE_HkTelemetryPkt;
CFE_SB_PipeId_t    WHE_CommandPipe;
CFE_SB_MsgPtr_t    WHEMsgPtr;

whe_t WHE_Info;

static CFE_EVS_BinFilter_t  WHE_EventFilters[] =
       {  /* Event ID    mask */
//          {WHE_STARTUP_INF_EID,       0x0000},
  //        {WHE_COMMAND_ERR_EID,       0x0000},
    //      {WHE_COMMANDNOP_INF_EID,    0x0000},
      //    {WHE_COMMANDRST_INF_EID,    0x0000},
       };


#define SET_STATE(x) WHE_Info.sbc_state=x;
#define GET_STATE() WHE_Info.sbc_state

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* WHE_AppMain() -- Application entry point and main process loop          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void WHE_AppMain( void )
{
    int32  status;
    uint32 RunStatus = CFE_ES_APP_RUN;

    CFE_ES_PerfLogEntry(WHE_PERF_ID);

    WHE_AppInit();

    /*
    ** WHE Runloop
    */
    while (CFE_ES_RunLoop(&RunStatus) == TRUE)
    {
        CFE_ES_PerfLogExit(WHE_PERF_ID);

        /* Pend on receipt of command packet -- timeout set to 500 millisecs */
        status = CFE_SB_RcvMsg(&WHEMsgPtr, WHE_CommandPipe, 500);
        
        CFE_ES_PerfLogEntry(WHE_PERF_ID);

        if (status == CFE_SUCCESS)
        {
            WHE_ProcessCommandPacket();
        }

    }

    CFE_ES_ExitApp(RunStatus);

} /* End of WHE_AppMain() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* WHE_AppInit() --  initialization                                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void WHE_AppInit(void)
{
	int32 status;

    /*
    ** Register the app with Executive services
    */
    CFE_ES_RegisterApp() ;

    /*
    ** Register the events
    */ 
    CFE_EVS_Register(WHE_EventFilters,
                     sizeof(WHE_EventFilters)/sizeof(CFE_EVS_BinFilter_t),
                     CFE_EVS_BINARY_FILTER);

    /*
    ** Create the Software Bus command pipe and subscribe to housekeeping
    **  messages
    */
    CFE_SB_CreatePipe(&WHE_CommandPipe, WHE_PIPE_DEPTH,"WHE_CMD_PIPE");
    CFE_SB_Subscribe(WHE_CMD_MID, WHE_CommandPipe);
    CFE_SB_Subscribe(WHE_SEND_HK_MID, WHE_CommandPipe);
    status = CFE_SB_Subscribe(WHE_1HZ_WAKEUP_MID, WHE_CommandPipe);

    if(status != CFE_SUCCESS)
    {
    	  CFE_ES_WriteToSysLog("WHE_APP:Error subscribing to wakeup message:RC=0x%08X\n", (unsigned int) status);

    }

    WHE_ResetCounters();

    WHE_Info.sbc_state = SBC_OFF;
    WHE_Info.instr_dmg = DMG_NONE;
    WHE_Info.temp = rand() % 30;
    WHE_Info.louver = LOUVER_CLOSE;
    WHE_Info.cap_a.charge = 0;
    WHE_Info.cap_a.state = CAP_CHARGING;
    WHE_Info.htr = HTR_OFF;
    WHE_Info.cap_a.dischrg_cnt = 0;
    WHE_Info.cap_b.charge = 0;
    WHE_Info.cap_b.state = CAP_CHARGING;
    WHE_Info.cap_b.dischrg_cnt = 0;
    WHE_Info.act_cap = CAP_A;
    WHE_Info.dmg_cnt = 0;
    WHE_Info.pwr_cnt = 0;
    WHE_Info.suc_obs = 0;
    WHE_Info.obs_cnt = 0;



    CFE_EVS_SendEvent (WHE_EID_INF_STARTUP, CFE_EVS_INFORMATION,
               "WHE App Initialized. Version %d.%d.%d.%d",
                WHE_MAJOR_VERSION,
				WHE_MINOR_VERSION,
				WHE_REVISION,
				WHE_MISSION_REV);
				

} /* End of WHE_AppInit() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  WHE_ProcessCommandPacket                                        */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the WHE    */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void WHE_ProcessCommandPacket(void)
{
    CFE_SB_MsgId_t  MsgId;

    MsgId = CFE_SB_GetMsgId(WHEMsgPtr);

    switch (MsgId)
    {
        case WHE_CMD_MID:
            WHE_ProcessGroundCommand();
            break;

        case WHE_SEND_HK_MID:
            WHE_ReportHousekeeping();
            break;

        case WHE_1HZ_WAKEUP_MID:
        	    WHE_ProcessWakeup();
        	    break;

        default:
            WHE_HkTelemetryPkt.whe_command_error_count++;
            CFE_EVS_SendEvent(WHE_EID_ERR_COMMAND,CFE_EVS_ERROR,
			"WHE: invalid command packet,MID = 0x%x", MsgId);
            break;
    }

    return;

} /* End WHE_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* WHE_ProcessGroundCommand() -- WHE ground commands                    */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

void WHE_ProcessGroundCommand(void)
{
    uint16 CommandCode;

    CommandCode = CFE_SB_GetCmdCode(WHEMsgPtr);

    /* Process "known" WHE app ground commands */
    switch (CommandCode)
    {
        case WHE_NOOP_CC:
            WHE_HkTelemetryPkt.whe_command_count++;
            CFE_EVS_SendEvent(WHE_EID_INF_CMD_NOP,CFE_EVS_INFORMATION,
			"WHE: NOOP command");
            break;

        case WHE_TLM_RESET_CNTS_CC:
            WHE_ResetCounters();
            break;

        case WHE_CAP_A_ACTIVE_CC:
            CFE_EVS_SendEvent(WHE_EID_INF_CMD_NOP,CFE_EVS_INFORMATION,
			"WHE: CAP_A Active");

        	WHE_CapCmd(CAP_A,CAP_ACTIVE);
        		break;

        case WHE_CAP_A_DISCHARGE_CC:
        		WHE_CapCmd(CAP_A,CAP_DISCHRG);
        		break;

        case WHE_CAP_B_ACTIVE_CC:
        		WHE_CapCmd(CAP_B,CAP_ACTIVE);
        		break;

        case WHE_CAP_B_DISCHARGE_CC:
        		WHE_CapCmd(CAP_B,CAP_DISCHRG);
        		break;

        	case WHE_OBS_START_CC:
        		WHE_ObsCmd(OBS_START);
        		break;

        	case WHE_OBS_STOP_CC:
        		WHE_ObsCmd(OBS_STOP);
        		break;

        	case WHE_POWER_SBC_CC:
        		WHE_SbcCmd(SBC_POWER);
        		break;

        	case WHE_THERM_HTR_OFF_CC:
        		WHE_Info.htr = HTR_OFF;
        		break;

        	case WHE_THERM_HTR_ON_CC:
        		WHE_Info.htr = HTR_ON;
        		break;

        	case WHE_THERM_LOUVER_CLOSE_CC:
        		WHE_Info.louver = LOUVER_CLOSE;
        		break;

        	case WHE_THERM_LOUVER_OPEN_CC:
        		WHE_Info.louver = LOUVER_OPEN;
        		break;

        /* default case already found during FC vs length test */
        default:
            break;
    }
    return;

} /* End of WHE_ProcessGroundCommand() */


void WHE_ProcessWakeup()
{
   // CFE_EVS_SendEvent(WHE_EID_INF_CMD_NOP,CFE_EVS_INFORMATION,
	//"WHE: Awake");

	if(WHE_Info.instr_dmg == DMG_MAJOR)
	{
		 CFE_EVS_SendEvent(WHE_EID_INF_CMD_RST, CFE_EVS_INFORMATION,
				"WHE: Instrument Destroyed");
		return;
	}

	/* Check if powering up */
	if((WHE_Info.sbc_state == SBC_OFF) && (WHE_Info.pwr_cnt > 0))
	{
		WHE_Info.pwr_cnt--;
		if(WHE_Info.pwr_cnt <= 0)
		{
			WHE_Info.sbc_state = SBC_POWERED;
		}
	}

	/* Update the capacitors */
	WHE_UpdateCap(&(WHE_Info.cap_a), &(WHE_Info.cap_b));
	WHE_UpdateCap(&(WHE_Info.cap_b), &(WHE_Info.cap_a));

	/* Update thermal */
	WHE_UpdateTherm();

	/* Update obs */
	WHE_UpdateObs();

	WHE_ReportHousekeeping();
}

void WHE_UpdateObs()
{
	if(WHE_Info.sbc_state == SBC_OBSERVING)
	{
		WHE_Info.temp += .4;

		if(WHE_Info.act_cap == CAP_A)
		{
			WHE_Info.cap_a.charge -= 5;
			if(WHE_Info.cap_a.charge <= 5)
			{
				WHE_Damage(); //EJB was destroy
			}
		}
		else
		{
			WHE_Info.cap_b.charge -= 5;
			if(WHE_Info.cap_b.charge <= 5)
			{
				WHE_Damage(); // EJB was destroy
			}
		}
		if(WHE_Info.obs_cnt > 0)
		{
			WHE_Info.obs_cnt--;
		}
		else
		{
			WHE_Info.suc_obs++;
			WHE_Info.sbc_state = SBC_POWERED;
		}

		// Bad temp, abort observation.
		if(WHE_Info.temp > 20)
		{
			WHE_Info.sbc_state = SBC_POWERED;
		}

		// Dangerous temp. Abort observation.
		if(WHE_Info.temp > 35)
		{
			WHE_Damage();
		}
	}
}

void WHE_Destroy()
{
	WHE_Info.instr_dmg = DMG_MAJOR;
	WHE_Info.sbc_state = SBC_ERROR;
}

void WHE_Damage()
{
	WHE_Info.instr_dmg = DMG_MINOR;
	WHE_Info.dmg_cnt++;
	if(WHE_Info.dmg_cnt > 3)
	{
		WHE_Destroy();
	}
}

void WHE_UpdateTherm()
{
	int diff = 0;
	int val = rand();

	if(WHE_Info.temp < 0)
	{
		diff = 0 - WHE_Info.temp;
	}
	else if (WHE_Info.temp > 35)
	{
		diff = WHE_Info.temp - 35;
	}
	else
	{
		diff = 0;
	}

	if(val < (diff * 5))
	{
		WHE_Destroy();
	}
	else if(val < (diff * 20))
	{
		WHE_Damage();
	}

	if(WHE_Info.louver == LOUVER_OPEN)
	{
		WHE_Info.temp *= .95;
	}
	if(WHE_Info.htr == HTR_ON)
	{
		WHE_Info.temp *= 1.1;
	}

}

void WHE_UpdateCap(cap_t *cap, cap_t *other_cap)
{
	int val;

	if (WHE_Info.sbc_state == SBC_OFF)
	{
		return;
	}

	if(cap->state == CAP_BROKEN)
	{
		return;
	}

/*	if(WHE_Info.sbc_state == SBC_OBSERVING)
	{
	   
	   WHE_Info.temp += .5;
	}

*/
	val = rand() % 100;

	if((cap->state == CAP_DISCHARGING) && (other_cap->state == CAP_DISCHARGING))
	{
		if(val <= 80)
		{
			WHE_Destroy();
			return;
		}
	}

	if(cap->state == CAP_DISCHARGING)
	{
		if(cap->dischrg_cnt > 0)
		{
			cap->dischrg_cnt--;
		}
		else
		{
			cap->charge -= 10;
			WHE_Info.temp += 1;
			if(cap->charge <= 0)
			{
				cap->charge = 0;
				cap->state = CAP_CHARGING;
			}
		}
	}

	if(cap->state == CAP_CHARGING)
	{
		if(other_cap->state != CAP_CHARGING)
		{
			cap->charge += 2;
		}
		else
		{
			cap->charge += 1;
		}
	}

	if(cap->state != CAP_BROKEN)
	{
		int chance;

		if(cap->charge > 100)
		{
			val = rand() % 100;
			chance = (cap->charge-100)/2.0;
			if(val < chance)
			{
				cap->state = CAP_BROKEN;
				WHE_Destroy();
			}
			else
			{
				chance = cap->charge-100;
				if(val < chance)
				{
					cap->state = CAP_LEAKING;
					WHE_Damage();
				}
			}
		}
	}

}

void WHE_CapCmd(int cap, int cmd)
{
	if((WHE_Info.sbc_state == SBC_OFF) ||
	   (WHE_Info.sbc_state == SBC_ERROR))
	{
		return;
	}

	if(cmd == CAP_ACTIVE)
	{
		WHE_Info.act_cap = cap;
	}
	else
	{
		if(cap == CAP_A)
		{
			if(WHE_Info.cap_a.state == CAP_CHARGING)
			{
				WHE_Info.cap_a.dischrg_cnt = 3;
				WHE_Info.cap_a.state = CAP_DISCHARGING;
			}
		}
		else
		{
			if(WHE_Info.cap_b.state == CAP_CHARGING)
			{
				WHE_Info.cap_b.dischrg_cnt = 3;
				WHE_Info.cap_b.state = CAP_DISCHARGING;
			}
		}
	}
}

void WHE_DoorCmd(int cmd)
{
	WHE_Info.louver = cmd;
}

void WHE_ObsCmd(int cmd)
{
	if((WHE_Info.sbc_state == SBC_OFF) || (WHE_Info.sbc_state == SBC_ERROR))
	{
		return;
	}

	if(cmd == OBS_START)
	{
		if(WHE_Info.sbc_state != SBC_OBSERVING)
		{
			if(WHE_TestSBC() != 0)
			{
				WHE_Info.sbc_state = SBC_OBSERVING;
				WHE_Info.obs_cnt = 15;
			}
		}
	}
	else if (WHE_Info.sbc_state == SBC_OBSERVING)
	{
		WHE_Info.sbc_state = SBC_POWERED;
		WHE_Info.obs_cnt = 0;
	}
}

int WHE_TestSBC()
{
	int success = 0;

	if(WHE_Info.act_cap == CAP_A)
	{
		if(WHE_Info.cap_a.state == CAP_CHARGING)
		{
			success = WHE_Info.cap_a.charge > (15 * 5);
		}
	}
	else
	{
		if(WHE_Info.cap_b.state == CAP_CHARGING)
		{
			success = WHE_Info.cap_b.charge > (15 * 5);
		}
	}

	success = success && (WHE_Info.temp <= 15) && (WHE_Info.temp > 0);

//	return success;
return 1;
}

void WHE_SbcCmd(int cmd)
{
	if(cmd == SBC_POWER)
	{
		if(WHE_Info.sbc_state == SBC_OFF)
		{
			WHE_Info.pwr_cnt = 5;
		}
	}
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  WHE_ReportHousekeeping                                              */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void WHE_ReportHousekeeping(void)
{
    CFE_SB_InitMsg(&WHE_HkTelemetryPkt,
    				   WHE_HK_TLM_MID,
				   WHE_HK_TLM_LNGTH, TRUE);

	WHE_HkTelemetryPkt.whe_cap_a_charge = WHE_Info.cap_a.charge;
	WHE_HkTelemetryPkt.whe_cap_a_state = WHE_Info.cap_a.state;
	WHE_HkTelemetryPkt.whe_cap_b_charge = WHE_Info.cap_b.charge;
	WHE_HkTelemetryPkt.whe_cap_b_state = WHE_Info.cap_b.state;
	WHE_HkTelemetryPkt.whe_sbc_state = WHE_Info.sbc_state;
	WHE_HkTelemetryPkt.whe_temp = (int8) WHE_Info.temp;
	WHE_HkTelemetryPkt.whe_louver = WHE_Info.louver;
	WHE_HkTelemetryPkt.whe_htr = WHE_Info.htr;
	WHE_HkTelemetryPkt.whe_act_cap = WHE_Info.act_cap;
	WHE_HkTelemetryPkt.whe_dmg_state = WHE_Info.instr_dmg;
	WHE_HkTelemetryPkt.whe_dmg_cnt = WHE_Info.dmg_cnt;
	WHE_HkTelemetryPkt.whe_obs_cnt = WHE_Info.obs_cnt;
	WHE_HkTelemetryPkt.whe_suc_obs = WHE_Info.suc_obs;


/*    WHE_HkTelemetryPkt.whe_cap_a_charge = 1;
    WHE_HkTelemetryPkt.whe_cap_a_state = 2;
    WHE_HkTelemetryPkt.whe_cap_b_charge = 3;
    WHE_HkTelemetryPkt.whe_cap_b_state = 4;
    WHE_HkTelemetryPkt.whe_sbc_state = 5;
    WHE_HkTelemetryPkt.whe_temp = (int8) 6;
    WHE_HkTelemetryPkt.whe_louver = 7;
    WHE_HkTelemetryPkt.whe_htr = 8;
    WHE_HkTelemetryPkt.whe_act_cap = 9;
    WHE_HkTelemetryPkt.whe_dmg_state = 10;
*/

	CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &WHE_HkTelemetryPkt);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &WHE_HkTelemetryPkt);
    return;

} /* End of WHE_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  WHE_ResetCounters                                               */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void WHE_ResetCounters(void)
{
    /* Status of commands processed by the WHE App */
    WHE_HkTelemetryPkt.whe_command_count       = 0;
    WHE_HkTelemetryPkt.whe_command_error_count = 0;

    CFE_EVS_SendEvent(WHE_EID_INF_CMD_RST, CFE_EVS_INFORMATION,
		"WHE: RESET command");
    return;

} /* End of WHE_ResetCounters() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* WHE_VerifyCmdLength() -- Verify command packet length                   */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
boolean WHE_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength)
{     
    boolean result = TRUE;

    uint16 ActualLength = CFE_SB_GetTotalMsgLength(msg);

    /*
    ** Verify the command packet length.
    */
    if (ExpectedLength != ActualLength)
    {
        CFE_SB_MsgId_t MessageID   = CFE_SB_GetMsgId(msg);
        uint16         CommandCode = CFE_SB_GetCmdCode(msg);

        CFE_EVS_SendEvent(WHE_EID_ERR_LEN, CFE_EVS_ERROR,
           "Invalid msg length: ID = 0x%X,  CC = %d, Len = %d, Expected = %d",
              MessageID, CommandCode, ActualLength, ExpectedLength);
        result = FALSE;
        WHE_HkTelemetryPkt.whe_command_error_count++;
    }

    return(result);

} /* End of WHE_VerifyCmdLength() */

