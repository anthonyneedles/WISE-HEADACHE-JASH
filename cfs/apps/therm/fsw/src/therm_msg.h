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

/*
** Local Structure Declarations
*/
typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              usCmdCnt;
    uint8              usCmdErrCnt;

    /* TODO:  Add declarations for additional housekeeping data here */

} THERM_HkTlm_t;


#endif /* _THERM_MSG_H_ */

/*=======================================================================================
** End of file therm_msg.h
**=====================================================================================*/
    