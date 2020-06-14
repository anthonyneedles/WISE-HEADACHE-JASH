/*******************************************************************************
** File: she.h
**
** Purpose:
**   This file is main hdr file for the WHE application.
**
**
*******************************************************************************/

#ifndef _whe_h_
#define _whe_h_

/*
** Required header files.
*/
#include "cfe.h"
#include "cfe_error.h"
#include "cfe_evs.h"
#include "cfe_sb.h"
#include "cfe_es.h"

#include <string.h>
#include <errno.h>
#include <unistd.h>

/***********************************************************************/

#define WHE_PIPE_DEPTH                     32

#define CAP_A 0
#define CAP_B 1

#define CAP_ACTIVE 0
#define CAP_DISCHRG 1

#define OBS_START 0
#define OBS_STOP 1

#define SBC_POWER 0

#define HTR_OFF 0
#define HTR_ON 1
#define LOUVER_CLOSE 0
#define LOUVER_OPEN 1

#define SBC_OFF 0
#define SBC_POWERED 1
#define SBC_OBSERVING 2
#define SBC_ERROR 3

#define DMG_NONE 0
#define DMG_MINOR 1
#define DMG_MAJOR 2

#define CAP_CHARGING 0
#define CAP_DISCHARGING 1
#define CAP_LEAKING 2
#define CAP_BROKEN 3

/************************************************************************
** Type Definitions
*************************************************************************/

typedef struct
{
	int state;
	int charge;
	int dischrg_cnt;

} cap_t;

typedef struct
{
	int sbc_state;
	int instr_dmg;
	cap_t cap_a;
	cap_t cap_b;
	float temp;
	int louver;
	int htr;
	int act_cap;
	int dmg_cnt;
	int pwr_cnt;
	int suc_obs;
	int obs_cnt;
} whe_t;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (WHE_AppMain), these
**       functions are not called from any other source module.
*/
void WHE_AppMain(void);
void WHE_AppInit(void);
void WHE_ProcessCommandPacket(void);
void WHE_ProcessGroundCommand(void);
void WHE_ReportHousekeeping(void);
void WHE_ResetCounters(void);

void WHE_UpdateObs(void);
void WHE_Destroy(void);
void WHE_Damage(void);
void WHE_UpdateTherm(void);
void WHE_UpdateCap(cap_t *cap, cap_t *other_cap);
int WHE_TestSBC(void);





void WHE_CapCmd(int cap, int cmd);
void WHE_DoorCmd(int cmd);
void WHE_ObsCmd(int cmd);
void WHE_SbcCmd(int cmd);
void WHE_ProcessWakeup(void);

boolean WHE_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 ExpectedLength);

#endif /* _whe_h_ */
