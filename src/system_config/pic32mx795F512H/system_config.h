/*******************************************************************************
  MPLAB Harmony System Configuration Header

  File Name:
    system_config.h

  Summary:
    Build-time configuration header for the system defined by this MPLAB Harmony
    project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    This configuration header must not define any prototypes or data
    definitions (or include any files that do).  It only provides macro
    definitions for build-time configuration options that are not instantiated
    until used by another MPLAB Harmony module or application.

    Created with MPLAB Harmony Version 2.02
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
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
// DOM-IGNORE-END

#ifndef _SYSTEM_CONFIG_H
#define _SYSTEM_CONFIG_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section Includes other configuration headers necessary to completely
    define this configuration.
*/


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: System Service Configuration
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* Common System Service Configuration Options
*/
#define SYS_VERSION_STR           "2.02"
#define SYS_VERSION               20200

// *****************************************************************************
/* Clock System Service Configuration Options
*/
#define SYS_CLK_FREQ                        80000000ul
#define SYS_CLK_BUS_PERIPHERAL_1            80000000ul
#define SYS_CLK_UPLL_BEFORE_DIV2_FREQ       48000000ul
#define SYS_CLK_CONFIG_PRIMARY_XTAL         8000000ul
#define SYS_CLK_CONFIG_SECONDARY_XTAL       32768ul
#define SYS_CLK_CONFIG_FREQ_ERROR_LIMIT     10
#define SYS_CLK_WAIT_FOR_SWITCH             true
#define SYS_CLK_ON_WAIT                     OSC_ON_WAIT_IDLE 
 
/*** Ports System Service Configuration ***/
#define SYS_PORT_AD1PCFG        ~0xffc0
#define SYS_PORT_CNPUE          0x1e000
#define SYS_PORT_CNEN           0x0
#define SYS_PORT_B_TRIS         0xFFE3
#define SYS_PORT_B_LAT          0x0000
#define SYS_PORT_B_ODC          0x0000

#define SYS_PORT_C_TRIS         0xFFFF
#define SYS_PORT_C_LAT          0x0000
#define SYS_PORT_C_ODC          0x0000

#define SYS_PORT_D_TRIS         0xFFFF
#define SYS_PORT_D_LAT          0x0000
#define SYS_PORT_D_ODC          0x0000

#define SYS_PORT_E_TRIS         0xFFDF
#define SYS_PORT_E_LAT          0x0000
#define SYS_PORT_E_ODC          0x0000

#define SYS_PORT_F_TRIS         0xFFFF
#define SYS_PORT_F_LAT          0x0000
#define SYS_PORT_F_ODC          0x0000

#define SYS_PORT_G_TRIS         0xFC3F
#define SYS_PORT_G_LAT          0x0000
#define SYS_PORT_G_ODC          0x0000


/*** Interrupt System Service Configuration ***/
#define SYS_INT                     true

// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************

 
// *****************************************************************************
// *****************************************************************************
// Section: Middleware & Other Library Configuration
// *****************************************************************************
// *****************************************************************************

/*** USB Driver Configuration ***/


/* Enables Device Support */
#define DRV_USBFS_DEVICE_SUPPORT      true

/* Disable Device Support */
#define DRV_USBFS_HOST_SUPPORT      false

/* Maximum USB driver instances */
#define DRV_USBFS_INSTANCES_NUMBER    1


/* Interrupt mode enabled */
#define DRV_USBFS_INTERRUPT_MODE      true


/* Number of Endpoints used */
#define DRV_USBFS_ENDPOINTS_NUMBER    3




/*** USB Device Stack Configuration ***/










/* The USB Device Layer will not initialize the USB Driver */
#define USB_DEVICE_DRIVER_INITIALIZE_EXPLICIT

/* Maximum device layer instances */
#define USB_DEVICE_INSTANCES_NUMBER     1

/* EP0 size in bytes */
#define USB_DEVICE_EP0_BUFFER_SIZE      64

/* Enable SOF Events */ 
#define USB_DEVICE_SOF_EVENT_ENABLE     









/* Maximum instances of CDC function driver */
#define USB_DEVICE_CDC_INSTANCES_NUMBER     1










/* CDC Transfer Queue Size for both read and
   write. Applicable to all instances of the
   function driver */
#define USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED 3





// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************
    
/*** Board Specific Configuration ***/

/* Outputs */
#define LED_FLASH_PORT PORT_CHANNEL_E
#define LED_FLASH_PIN PORTS_BIT_POS_5
#define STATUS_RED_PORT PORT_CHANNEL_B
#define STATUS_RED_PIN PORTS_BIT_POS_2
#define STATUS_GREEN_PORT PORT_CHANNEL_B
#define STATUS_GREEN_PIN PORTS_BIT_POS_3
#define STATUS_BLUE_PORT PORT_CHANNEL_B
#define STATUS_BLUE_PIN PORTS_BIT_POS_4
#define FAN_1_PORT PORT_CHANNEL_G
#define FAN_1_PIN PORTS_BIT_POS_9
#define FAN_2_PORT PORT_CHANNEL_G
#define FAN_2_PIN PORTS_BIT_POS_8
    
/* Inputs */
#define UPS_CHARGE_PORT PORT_CHANNEL_D
#define UPS_CHARGE_PIN PORTS_BIT_POS_1
#define UPS_DISCHARGE_PORT PORT_CHANNEL_D
#define UPS_DISCHARGE_PIN PORTS_BIT_POS_2
#define UPS_ALARM_PORT PORT_CHANNEL_D
#define UPS_ALARM_PIN PORTS_BIT_POS_3
#define PROX_DETECT_PORT PORT_CHANNEL_D
#define PROX_DETECT_PIN PORTS_BIT_POS_4
#define PROX_CLEAR_PORT PORT_CHANNEL_D
#define PROX_CLEAR_PIN PORTS_BIT_POS_5
#define HELP_SW_PORT PORT_CHANNEL_D
#define HELP_SW_PIN PORTS_BIT_POS_6  
#define OPMODE_RUN_PORT PORT_CHANNEL_D
#define OPMODE_RUN_PIN PORTS_BIT_POS_7
#define OPMODE_SERVICE_PORT PORT_CHANNEL_F
#define OPMODE_SERVICE_PIN PORTS_BIT_POS_0
#define TAMPER_TOP_PORT PORT_CHANNEL_F
#define TAMPER_TOP_PIN PORTS_BIT_POS_1
#define TAMPER_BOT_PORT PORT_CHANNEL_E
#define TAMPER_BOT_PIN PORTS_BIT_POS_0
#define SYSTEM_RESET_PORT PORT_CHANNEL_E
#define SYSTEM_RESET_PIN PORTS_BIT_POS_1
   
/*** Application Instance 0 Configuration ***/


#define APP_USB_CDC_COM_PORT_SINGLE_READ_BUFFER_SIZE  512
#define APP_USB_CDC_COM_PORT_SINGLE_WRITE_BUFFER_SIZE 512

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // _SYSTEM_CONFIG_H
/*******************************************************************************
 End of File
*/
