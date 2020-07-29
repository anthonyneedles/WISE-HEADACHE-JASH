/************************************************************************
** File:
**    whe_events.h
**
** Purpose: 
**  Define WHE Events IDs
**
** Notes:
**
**
*************************************************************************/
#ifndef _whe_events_h_
#define _whe_events_h_


#define WHE_RESERVED_EID              0


/* ERROR EVENTS */
#define WHE_EID_ERR_INVALID_MSGID     1    /** Unknown command received.     */
#define WHE_EID_ERR_LEN               2    /** Bad Length (?)                */
#define WHE_EID_ERR_CMD_WHEN_NOT_RDY  3    /** Starting obs, when not ready. */
#define WHE_EID_ERR_CMD_IN_OBS        4    /** Starting obs when in obs.     */
#define WHE_EID_ERR_EXCEED_OBS TEMP   5    /** Observation lost due to temp. */
#define WHE_EID_ERR_POWER_TOO_LOW     6    /** Not enough power to do cmd.   */
#define WHE_EID_ERR_CAP_LEAKING       7    /** Capacitor is leaking          */
#define WHE_EID_ERR_CAP_BROKEN        8    /** Capacitor is broken           */
#define WHE_EID_ERR_SBC_MINOR         9    /** SBC sustained minor damage    */
#define WHE_EID_ERR_SBC_MAJOR        10    /** SBC sustained major damage    */
#define WHE_EID_ERR_COMMAND          11    /** Bad command received. */

/* INFORMATIONAL EVENTS */
#define WHE_EID_INF_STARTUP          12   /** System started up.            */
#define WHE_EID_INF_CMD_NOP          13   /** NO-OP command ran.            */
#define WHE_EID_INF_INSTR_DAMAGED    14    /** Incremented damage counter.   */
#define WHE_EID_INF_INSTR_DESTROYED  15    /** The instrument was lost.      */
#define WHE_EID_INF_STATE_CHANGE     16    /** The instrument state changed. */
#define WHE_EID_INF_CMD_RST          17   /* Conters reset. */

#endif /* _whe_events_h_ */

/************************/
/*  End of File Comment */
/************************/
