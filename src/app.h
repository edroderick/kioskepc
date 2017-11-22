/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
//DOM-IGNORE-END

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
	/* Application's state machine's initial state. */
	APP_STATE_INIT=0,
    APP_STATE_DISCONNECTED,
    APP_STATE_PING,
    APP_STATE_UPDATE_INPUTS,
    APP_STATE_STATUS_UPDATE,
	APP_STATE_READ_USB,
    APP_STATE_PROCESS_CMD,
    APP_STATE_PROCESS_RSP,
    APP_STATE_CHECK_TIMEOUT,

} APP_STATES;

typedef enum{
    FULL,
    BLINK_50,
    BLINK_RAPID,
    
} LIGHT_PATTERN;

typedef enum
{
    RED,
    BLUE,
    GREEN,
    YELLOW,
    WHITE,
    PURPLE,
    CYAN,
    OFF,

} STATUS_COLOR;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    APP_STATES state;

    /* Device layer handle returned by device layer open function */
    USB_DEVICE_HANDLE deviceHandle;

    /* Set Line Coding Data */
    USB_CDC_LINE_CODING setLineCodingData;

    /* Device configured state */
    bool isConfigured;

    /* Get Line Coding Data */
    USB_CDC_LINE_CODING getLineCodingData;

    /* Control Line State */
    USB_CDC_CONTROL_LINE_STATE controlLineStateData;

    /* Read transfer handle */
    USB_DEVICE_CDC_TRANSFER_HANDLE readTransferHandle;

    /* Length of data read */
    uint32_t readLen;
    
    /* Length of read data processed */
    uint32_t readProcessedLen;

    /* Write transfer handle */
    USB_DEVICE_CDC_TRANSFER_HANDLE writeTransferHandle;
    
    /* Length of data to be written */
    uint32_t writeLen;
    uint32_t lastmsgLen;
   
    bool needresponse;
    
    SYS_TMR_HANDLE sysResponseTimerHandle;
    bool responsetimer;
    
    SYS_TMR_HANDLE heartbeatTimerHandle;
    bool connected;
    bool heartbeat;
    uint8_t heartbeat_timeouts;
    
  
} APP_DATA;

typedef struct{
    bool upsAlarm;
    bool upsCharge;
    bool upsDischarge;
    bool proxSensor;
    
    bool help_sw_ignore;
    bool help_sw_sof_event;
    unsigned int help_sw_debounce_timer;  
    bool helpSW;
    
    bool system_reset_ignore;
    bool system_reset_sof_event;
    unsigned int system_reset_debounce_timer;
    bool systemReset;
    
    bool tamper_top_ignore;
    bool tamper_top_sof_event;
    unsigned int tamper_top_debounce_timer;
    bool tamperTop;
    
    bool tamper_bot_ignore;
    bool tamper_bot_sof_event;
    unsigned int tamper_bot_debounce_timer;
    bool tamperBot;

    bool opmode_run_ignore;
    bool opmode_run_sof_event;
    unsigned int opmode_run_debounce_timer;
    bool op_run_sw;
    
    bool opmode_service_ignore;
    bool opmode_service_sof_event;
    unsigned int opmode_service_debounce_timer;
    bool op_service_sw;
    
    unsigned int debounce_max; 
   

} SYSTEM_INPUTS;
        
typedef struct{
    STATUS_COLOR statusColor;
    LIGHT_PATTERN statusPattern;
    bool flashOn;
    int flashDutyCycle;
    bool fan1On;
    //int fan1PWM;
    bool fan2On;
    //int fan2PWM;
    
} SYSTEM_OUTPUTS;

typedef struct{
    uint8_t start;
    //uint8_t reset;
    //uint8_t flash;
    
} CMD;

typedef struct{
    uint8_t start;
    uint8_t invalidchk;
   
} RSP;

typedef struct{
    uint8_t start;
    uint8_t cmd;
    uint8_t data[];
} MSG;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/
	
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void APP_Initialize ( void );

void APP_Reset(void);

static uint8_t checkSum( uint8_t message[] );

void APP_Tasks( void );

void addOutBuffer(char msg[], uint8_t size);

bool compareInputState( void );

bool compareOutputState( void );

static void processCMD(void);

static void processRSP(void);

void readInputs(void);

static void sendUpdate(void);

#endif /* _APP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

