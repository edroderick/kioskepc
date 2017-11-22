// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

APP_DATA appData;
SYSTEM_INPUTS input;
SYSTEM_INPUTS input_old;
SYSTEM_OUTPUTS output;
SYSTEM_OUTPUTS output_old;
CMD cmd;
RSP rsp;
MSG currentMSG;
uint8_t lastMSG[20]="";
MSG clearMSG;

int cmd_reset = 1;

USB_DEVICE_CDC_RESULT readResult;

/* Static buffers, suitable for DMA transfer */
#define APP_MAKE_BUFFER_DMA_READY  __attribute__((coherent)) __attribute__((aligned(16)))

static uint8_t APP_MAKE_BUFFER_DMA_READY writeBuffer[APP_USB_CDC_COM_PORT_SINGLE_WRITE_BUFFER_SIZE];
uint8_t clearData[20] = "";
static uint8_t APP_MAKE_BUFFER_DMA_READY readBuffer [APP_USB_CDC_COM_PORT_SINGLE_READ_BUFFER_SIZE];



// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************
 * USB CDC Device Events - Application Event Handler
 *******************************************************/

// <editor-fold defaultstate="collapsed" desc="USB CDC Device Events - Application Event Handler">
USB_DEVICE_CDC_EVENT_RESPONSE APP_USBDeviceCDCEventHandler
(
    USB_DEVICE_CDC_INDEX index ,
    USB_DEVICE_CDC_EVENT event ,
    void * pData,
    uintptr_t userData
)
{
    APP_DATA * appDataObject;
    appDataObject = (APP_DATA *)userData;
    USB_CDC_CONTROL_LINE_STATE * controlLineStateData;

    switch ( event )
    {
        case USB_DEVICE_CDC_EVENT_GET_LINE_CODING:

            /* This means the host wants to know the current line
             * coding. This is a control transfer request. Use the
             * USB_DEVICE_ControlSend() function to send the data to
             * host.  */

            USB_DEVICE_ControlSend(appDataObject->deviceHandle,
                    &appDataObject->getLineCodingData, sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_LINE_CODING:

            /* This means the host wants to set the line coding.
             * This is a control transfer request. Use the
             * USB_DEVICE_ControlReceive() function to receive the
             * data from the host */

            USB_DEVICE_ControlReceive(appDataObject->deviceHandle,
                    &appDataObject->setLineCodingData, sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_CONTROL_LINE_STATE:

            /* This means the host is setting the control line state.
             * Read the control line state. We will accept this request
             * for now. */

            controlLineStateData = (USB_CDC_CONTROL_LINE_STATE *)pData;
            appDataObject->controlLineStateData.dtr = controlLineStateData->dtr;
            appDataObject->controlLineStateData.carrier = controlLineStateData->carrier;

            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);

            break;

        case USB_DEVICE_CDC_EVENT_SEND_BREAK:

            /* This means that the host is requesting that a break of the
             * specified duration be sent.  */
            break;

        case USB_DEVICE_CDC_EVENT_READ_COMPLETE:
            /* This means that the host has sent some data*/
            appDataObject->readTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
            
            //readString[appDataObject->readProcessedLen] = readBuffer[0];
            if (appDataObject->readProcessedLen < appData.readLen)
            {
                appDataObject->readProcessedLen++;
            }
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:

            /* The data stage of the last control transfer is
             * complete. For now we accept all the data */

            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_SENT:

            /* This means the GET LINE CODING function data is valid. We dont
             * do much with this data in this demo. */
            break;

        case USB_DEVICE_CDC_EVENT_WRITE_COMPLETE:

            /* This means that the host has sent some data*/
            appDataObject->writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
            break;

        default:
            break;
    }

    return USB_DEVICE_CDC_EVENT_RESPONSE_NONE;
}
// </editor-fold>

/***********************************************
 * Application USB Device Layer Event Handler.
 ***********************************************/

// <editor-fold defaultstate="collapsed" desc="Application USB Device Layer Event Handler">
void APP_USBDeviceEventHandler ( USB_DEVICE_EVENT event, void * eventData, uintptr_t context )
{
    USB_DEVICE_EVENT_DATA_CONFIGURED *configuredEventData;

    switch ( event )
    {
        case USB_DEVICE_EVENT_SOF:
            /* switch debounce event flags*/
            input.tamper_top_sof_event = true;
            input.tamper_bot_sof_event = true;
            input.help_sw_sof_event = true;
            input.system_reset_sof_event = true;
            input.opmode_run_sof_event = true;
            input.opmode_service_sof_event = true;
            break;

        case USB_DEVICE_EVENT_RESET:

            appData.isConfigured = false;

            break;

        case USB_DEVICE_EVENT_CONFIGURED:

            /* Check the configuration. We only support configuration 1 */
            configuredEventData = (USB_DEVICE_EVENT_DATA_CONFIGURED*)eventData;
            if ( configuredEventData->configurationValue == 1)
            {
                /* Register the CDC Device application event handler here.
                 * Note how the appData object pointer is passed as the
                 * user data */

                USB_DEVICE_CDC_EventHandlerSet(USB_DEVICE_CDC_INDEX_0, APP_USBDeviceCDCEventHandler, (uintptr_t)&appData);

                /* Mark that the device is now configured */
                appData.isConfigured = true;

            }
            break;

        case USB_DEVICE_EVENT_POWER_DETECTED:

            /* VBUS was detected. We can attach the device */
            USB_DEVICE_Attach(appData.deviceHandle);
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:

            /* VBUS is not available any more. Detach the device. */
            USB_DEVICE_Detach(appData.deviceHandle);
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            break;

        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_ERROR:
        default:
            break;
    }
}
// </editor-fold>

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


static void responsetimer_callback (uintptr_t context, uint32_t  alarmCount )
{
    appData.responsetimer = true;
}

static void heartbeat_callback( uintptr_t context, uint32_t alarmCount)
{
    appData.heartbeat = true;
}

static void USB_TX_Task (void)
{
    if(!appData.isConfigured)
    {
        appData.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
    }
    else
    {
        /* Schedule a write if data is pending 
         */
        if ((appData.writeLen > 0) && (appData.writeTransferHandle == USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID))
        {
            USB_DEVICE_CDC_Write(USB_DEVICE_CDC_INDEX_0,
                                 &appData.writeTransferHandle,
                                 writeBuffer, 
                                 appData.writeLen,
                                 USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
            
            appData.writeLen = 0;
        }
    }
}

static void USB_RX_Task(void)
{
    if(!appData.isConfigured)
    {
        appData.readTransferHandle  = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
        appData.readProcessedLen    = 0;
    }
    else
    {
        /* Schedule a read if none is pending and all previously read data
           has been processed
         */
        //if((appData.readProcessedLen < appData.readLen) && (appData.readTransferHandle  == USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID))
        if(appData.readTransferHandle  == USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID)
        {
            USB_DEVICE_CDC_Read (USB_DEVICE_CDC_INDEX_0,
                                 &appData.readTransferHandle, 
                                 readBuffer,
                                 APP_USB_CDC_COM_PORT_SINGLE_READ_BUFFER_SIZE);
            
            appData.readProcessedLen = 0;
            
        };
    }
}

static uint8_t checkSum(uint8_t message[])
{
    uint8_t checksum = message[1] + message[2];
    
    uint8_t i;
    for(i=0;i<message[2];i++){
        checksum += message[3+i];
    }
    
    return checksum ^ 255;
}

void addOutBuffer(char msg[], uint8_t size)
{
    appData.lastmsgLen = size;
    memcpy(lastMSG, msg, size);
    appData.writeLen = size;
    memcpy(writeBuffer, msg, size);
}

bool compareInputState( void )
{
    return input.helpSW == input_old.helpSW && 
           input.op_run_sw == input_old.op_run_sw &&
           input.op_service_sw == input_old.op_service_sw &&
           input.systemReset == input_old.systemReset && 
           input.proxSensor == input_old.proxSensor && 
           input.tamperBot == input_old.tamperBot && 
           input.tamperTop == input_old.tamperTop && 
           input.upsAlarm == input_old.upsAlarm && 
           input.upsCharge == input_old.upsCharge && 
           input.upsDischarge == input_old.upsDischarge;
}

bool compareOutputState( void )
{
    return output.fan1On == output_old.fan1On &&
            output.fan2On == output_old.fan2On &&
            output.flashDutyCycle == output_old.flashDutyCycle &&
            output.flashOn == output_old.flashOn &&
            output.statusColor == output_old.statusColor &&
            output.statusPattern == output_old.statusPattern;
 }

static void setStatusIndicator(STATUS_COLOR color)
{
    switch (color)
    {
        case RED:
        {
            output.statusColor = RED;
            RED_CTRLOff(); //low = on
            GREEN_CTRLOn();
            BLUE_CTRLOn();
            break;
        }
        
        case GREEN:
        {
            output.statusColor = GREEN;            
            RED_CTRLOn(); //low = on
            GREEN_CTRLOff();
            BLUE_CTRLOn();
            break;
        }
            
        case BLUE:
        {
            output.statusColor = BLUE;            
            RED_CTRLOn(); //low = on
            GREEN_CTRLOn();
            BLUE_CTRLOff();
            break;
        }
        
        case YELLOW:
        {
            output.statusColor = YELLOW;            
            RED_CTRLOff(); //low = on
            GREEN_CTRLOff();
            BLUE_CTRLOn();
            break;
        }
                   
            
        case WHITE:
        {
            output.statusColor = WHITE;            
            RED_CTRLOff(); //low = on
            GREEN_CTRLOff();
            BLUE_CTRLOff();
            break;
        }
            
        case PURPLE:
        {
            output.statusColor = PURPLE;            
            RED_CTRLOff(); //low = on
            GREEN_CTRLOn();
            BLUE_CTRLOff();
            break;
        }
              
        case CYAN:
        {
            output.statusColor = CYAN;            
            RED_CTRLOn(); //low = on
            GREEN_CTRLOff();
            BLUE_CTRLOff();
            break;
        }
            
        case OFF:
        {
            output.statusColor = OFF;            
            RED_CTRLOn(); //low = on
            GREEN_CTRLOn();
            BLUE_CTRLOn();
            break;
        }
    }
}

/* Read all system inputs and update state*/
void readInputs (void)
{
    /* Poll all input pins and update input struct*/
    
    // Proximity 
    if (!SYS_PORTS_PinRead(PORTS_ID_0, PROX_DETECT_PORT, PROX_DETECT_PIN) && !input.proxSensor)
    //if (!SYS_PORTS_PinRead(PORTS_ID_0, PROX_DETECT_PORT, PROX_DETECT_PIN))
    {
        input.proxSensor = true;
    }
    else if (!SYS_PORTS_PinRead(PORTS_ID_0, PROX_CLEAR_PORT, PROX_CLEAR_PIN) && input.proxSensor)
    {
        input.proxSensor = false;
    }
    
    // UPS Charge 
    else if (!SYS_PORTS_PinRead(PORTS_ID_0, UPS_CHARGE_PORT, UPS_CHARGE_PIN) && !input.upsCharge)
    {
        input.upsCharge = true;
    }
    else if (SYS_PORTS_PinRead(PORTS_ID_0, UPS_CHARGE_PORT, UPS_CHARGE_PIN) && input.upsCharge)
    {
        input.upsCharge = false;
    }

    // UPS Discharge 
    else if (!SYS_PORTS_PinRead(PORTS_ID_0, UPS_DISCHARGE_PORT, UPS_DISCHARGE_PIN) && !input.upsDischarge)
    {
        input.upsDischarge = true;
    }
    else if (SYS_PORTS_PinRead(PORTS_ID_0, UPS_DISCHARGE_PORT, UPS_DISCHARGE_PIN) && input.upsDischarge)
    {
        input.upsDischarge = false;
    }
    
    // UPS Fault
    // might need to change logic to high pending ups configuration
    else if (!SYS_PORTS_PinRead(PORTS_ID_0, UPS_ALARM_PORT, UPS_ALARM_PIN) && !input.upsAlarm)
    {
        input.upsAlarm = true;
    }
    else if (SYS_PORTS_PinRead(PORTS_ID_0, UPS_ALARM_PORT, UPS_ALARM_PIN) && input.upsAlarm)
    {
        input.upsAlarm = false;
    }
    
    // Check and debounce help switch)
    else if (!SYS_PORTS_PinRead(PORTS_ID_0, HELP_SW_PORT, HELP_SW_PIN) && !input.helpSW)
    {
        if (input.help_sw_ignore)
        {
            if (input.help_sw_sof_event)
            {
                input.help_sw_debounce_timer ++;
                input.help_sw_sof_event = false;

                if (input.help_sw_debounce_timer = input.debounce_max)
                {
                    input.helpSW = true;
                    input.help_sw_debounce_timer = 0;
                    input.help_sw_ignore = false;
                }
            }
        }
        else
        {
            // fresh key press
            input.help_sw_ignore = true;
            input.help_sw_debounce_timer = 0;
        }
    }
    else if(SYS_PORTS_PinRead(PORTS_ID_0, HELP_SW_PORT, HELP_SW_PIN) && input.helpSW)
    {
        input.help_sw_ignore = false;
        input.help_sw_debounce_timer = 0;
        input.help_sw_sof_event = false;
        input.helpSW = false; 
    }
    
    // Check and debounce system reset)
    else if (!SYS_PORTS_PinRead(PORTS_ID_0, SYSTEM_RESET_PORT, SYSTEM_RESET_PIN) && !input.systemReset)
    {
        if (input.system_reset_ignore)
        {
            if (input.system_reset_sof_event)
            {
                input.system_reset_debounce_timer ++;
                input.system_reset_sof_event = false;

                if (input.system_reset_debounce_timer = input.debounce_max)
                {
                    input.systemReset = true;
                    input.system_reset_debounce_timer = 0;
                    input.system_reset_ignore = false;
                }
            }
        }
        else
        {
            // fresh key press
            input.system_reset_ignore = true;
            input.system_reset_debounce_timer = 0;
        }
    }
    else if (SYS_PORTS_PinRead(PORTS_ID_0, SYSTEM_RESET_PORT, SYSTEM_RESET_PIN) && input.systemReset)
    {
        input.system_reset_ignore = false;
        input.system_reset_debounce_timer = 0;
        input.system_reset_sof_event = false;
        input.systemReset = false; 
    }
    
    // Check and debounce tamper top switch)
    else if (!SYS_PORTS_PinRead(PORTS_ID_0, TAMPER_TOP_PORT, TAMPER_TOP_PIN) && !input.tamperTop)
    {
        if (input.tamper_top_ignore)
        {
            if (input.tamper_top_sof_event)
            {
                input.tamper_top_debounce_timer ++;
                input.tamper_top_sof_event = false;

                if (input.tamper_top_debounce_timer = input.debounce_max)
                {
                    input.tamperTop = true;
                    input.tamper_top_debounce_timer = 0;
                    input.tamper_top_ignore = false;
                }
            }
        }
        else
        {
            // fresh key press
            input.tamper_top_ignore = true;
            input.tamper_top_debounce_timer = 0;
        }
    }
    else if (SYS_PORTS_PinRead(PORTS_ID_0, TAMPER_TOP_PORT, TAMPER_TOP_PIN) && input.tamperTop)
    {
        input.tamper_top_ignore = false;
        input.tamper_top_debounce_timer = 0;
        input.tamper_top_sof_event = false;
        input.tamperTop = false; 
    }
    
    // Check and debounce tamper bot switch)
    else if (!SYS_PORTS_PinRead(PORTS_ID_0, TAMPER_BOT_PORT, TAMPER_BOT_PIN) && !input.tamperBot)
    {
        if (input.tamper_bot_ignore)
        {
            if (input.tamper_top_sof_event)
            {
                input.tamper_bot_debounce_timer ++;
                input.tamper_bot_sof_event = false;

                if (input.tamper_bot_debounce_timer = input.debounce_max)
                {
                    input.tamperBot = true;
                    input.tamper_bot_debounce_timer = 0;
                    input.tamper_bot_ignore = false;
                }
            }
        }
        else
        {
            // fresh key press
            input.tamper_bot_ignore = true;
            input.tamper_bot_debounce_timer = 0;
        }
    }
    else if (SYS_PORTS_PinRead(PORTS_ID_0, TAMPER_BOT_PORT, TAMPER_BOT_PIN) && input.tamperBot)
    {
        input.tamper_bot_ignore = false;
        input.tamper_bot_debounce_timer = 0;
        input.tamper_bot_sof_event = false;
        input.tamperBot = false; 
    }
    
    
    
    // Check and debounce OpMode:Run switch)
    else if (!SYS_PORTS_PinRead(PORTS_ID_0, OPMODE_RUN_PORT, OPMODE_RUN_PIN) && !input.op_run_sw)
    {
        if (input.opmode_run_ignore)
        {
            if (input.opmode_run_sof_event)
            {
                input.opmode_run_debounce_timer ++;
                input.opmode_run_sof_event = false;

                if (input.opmode_run_debounce_timer = input.debounce_max)
                {
                    input.op_run_sw = true;
                    input.opmode_run_debounce_timer = 0;
                    input.opmode_run_ignore = false;
                }
            }
        }
        else
        {
            // fresh key press
            input.opmode_run_ignore = true;
            input.opmode_run_debounce_timer = 0;
        }
    }
    else if (SYS_PORTS_PinRead(PORTS_ID_0, OPMODE_RUN_PORT, OPMODE_RUN_PIN) && input.op_run_sw)
    {
        input.opmode_run_ignore = false;
        input.opmode_run_debounce_timer = 0;
        input.opmode_run_sof_event = false;
        input.op_run_sw = false;
    }

    // Check and debounce MS:Service switch)
    else if (!SYS_PORTS_PinRead(PORTS_ID_0, OPMODE_SERVICE_PORT, OPMODE_SERVICE_PIN) && !input.op_service_sw)
    {
        if (input.opmode_service_ignore)
        {
            if (input.opmode_service_sof_event)
            {
                input.opmode_service_debounce_timer ++;
                input.opmode_service_sof_event = false;

                if (input.opmode_service_debounce_timer = input.debounce_max)
                {
                    input.op_service_sw = true;
                    input.opmode_service_debounce_timer = 0;
                    input.opmode_service_ignore = false;
                }
            }
        }
        else
        {
            // fresh key press
            input.opmode_service_ignore = true;
            input.opmode_service_debounce_timer = 0;
        }
    }
    else if (SYS_PORTS_PinRead(PORTS_ID_0, OPMODE_SERVICE_PORT, OPMODE_SERVICE_PIN) && input.op_run_sw)
    {
        input.opmode_service_ignore = false;
        input.opmode_service_debounce_timer = 0;
        input.opmode_service_sof_event = false;
        input.op_service_sw = false;
    }
     
}

/* Process new state and send message to host */
static void sendUpdate(void)
{
    uint8_t data;
    uint8_t length;
    uint8_t command;

    /* Proximity Sensor */
    if(input.proxSensor != input_old.proxSensor)
    {
        if (input.proxSensor)
        {
             data = 0x01;
        }
        else
        {    
             data = 0x02;
        }  
        command = 0x10;
        length = 0x01;

    }
    
    else if(input.helpSW != input_old.helpSW)
    {
        if (input.helpSW)
        {
             data = 0x01;
        }
        else
        {    
             data = 0x02;
        }  
        command = 0x0e;
        length = 0x01;

    }
    
    else if(input.tamperTop != input_old.tamperTop)
    {
        if (input.tamperTop)
        {
            data = 0x01;
        }
        else
        {
            data = 0x02;
        }
        command = 0x17;
        length = 0x01;
      
    }
    
    else if(input.tamperBot != input_old.tamperBot)
    {
        if (input.tamperBot)
        {
            data = 0x01;
        }
        else
        {
            data = 0x02;
        }
        command = 0x19;
        length = 0x01;
       
    }
    
    else if(input.systemReset != input_old.systemReset)
    {
        if (input.systemReset)
        {
            data = 0x01;
        }
        else
        {
            data = 0x02;
        }
        command = 0x23;
        length = 0x01;
    }
    
    else if(input.op_run_sw != input_old.op_run_sw)
    {
        if (input.op_run_sw)
        {
            data = 0x01;
        }
        else
        {
            data = 0x02;
        }
        command = 0x01;
        length = 0x01;
    }
     
    else if(input.op_service_sw != input_old.op_service_sw)
    {
        if (input.op_service_sw)
        {
            data = 0x01;
        }
        else
        {
            data = 0x02;
        }
        command = 0x02;
        length = 0x01;
    }
       
    else if(input.upsCharge != input_old.upsCharge)
    {
        if (input.upsCharge)
        {
            data = 0x01;
        }
        else
        {
            data = 0x02;
        }
        command = 0x12;
        length = 0x01;     
    }
    
    else if(input.upsDischarge != input_old.upsDischarge)
    {
        if (input.upsDischarge)
        {
            data = 0x01;
        }
        else
        {
            data = 0x02;
        }
        command = 0x13;
        length = 0x01;   
    }
    
    else if(input.upsAlarm != input_old.upsAlarm)
    {
        if (input.upsAlarm)
        {
            data = 0x01;
        }
        else
        {
            data = 0x02;
        }
        command = 0x14;
        length = 0x01; 
    }  
    
    char outmsg[] = {cmd.start, command, length, data};
    char outmsg2[] = {cmd.start, command, length, data, checkSum(outmsg)};
    addOutBuffer(outmsg2, sizeof(outmsg2));
    USB_TX_Task();
}

/* Read message from host and process*/
static void processCMD(void)
{
    uint8_t data_rsp;
    uint8_t length_rsp;
    uint8_t command = readBuffer[1];
    
    uint8_t data[readBuffer[2]];
    uint8_t chksm_read = readBuffer[3+readBuffer[2]];
    
    uint8_t i;
    for(i=0;i<readBuffer[2];i++){
        data[i] = readBuffer[3+i];
    }

    /*Command Instruction Set*/
    if (checkSum(readBuffer) != chksm_read)
    {
        data_rsp = rsp.invalidchk;
        length_rsp = 0x01;
    }
    else
    {
        switch (command)
        {
            /*Reset EPC*/
            case 0x02:
            {
                //might not work
                //add response message
                APP_Reset();
                appData.state = APP_STATE_UPDATE_INPUTS;
                break;
            }

            /*Camera Flash On/Off*/
            case 0x07:
            {
                if (data[0] == 0x01)
                {     
                    SYS_PORTS_PinClear(PORTS_ID_0, LED_FLASH_PORT, LED_FLASH_PIN);
                    output.flashOn = true; 
                    data_rsp = 0x01;
                    length_rsp = 0x01;
                }
                else if (data[0] == 0x02)
                {            
                    SYS_PORTS_PinSet(PORTS_ID_0,  LED_FLASH_PORT, LED_FLASH_PIN);
                    output.flashOn = false; 
                    data_rsp = 0x01;
                    length_rsp = 0x01;
                }
                break;
            }
            
            /* Status Indicator Color*/
            case 0x0a:
            {
                if (data[0] == 0x01)
                {
                    setStatusIndicator(OFF);
                }
                if (data[0] == 0x02)
                {
                    setStatusIndicator(RED);
                }
                if (data[0] == 0x03)
                {
                    setStatusIndicator(GREEN);
                }
                if (data[0] == 0x04)
                {
                    setStatusIndicator(BLUE);
                }
                if (data[0] == 0x05)
                {
                    setStatusIndicator(YELLOW);
                }
                if (data[0] == 0x06)
                {
                    setStatusIndicator(WHITE);
                }
                if (data[0] == 0x07)
                {
                    setStatusIndicator(PURPLE);
                }
                if (data[0] == 0x08)
                {
                    setStatusIndicator(CYAN);
                }
                data_rsp = 0x01;
                length_rsp = 0x01;
                break;
            }
            
            /* Fan 1 Control*/
            case 0x20:
            {
                if (data[0] == 0x01)
                {
                    output.fan1On = true;
                    FAN_1_PWMOn();
                }
                if (data[0] == 0x02)
                {
                    output.fan1On = false;
                    FAN_1_PWMOff();
                }
                data_rsp = 0x01;
                length_rsp = 0x01;
                break;
            }

            /* Fan 2 Control*/
            case 0x22:
            {
                if (data[0] == 0x01)
                {
                    output.fan2On = true;
                    FAN_2_PWMOn();
                }
                if (data[0] == 0x02)
                {
                    output.fan2On = false;
                    FAN_2_PWMOff();
                }
                data_rsp = 0x01;
                length_rsp = 0x01;
                break;
            }            
        }
    }
    
    /*Clear USB Input Buffer*/
    memcpy(readBuffer, clearData, sizeof(clearData));
    /*Send Response*/
    char outmsg[] = {rsp.start, command, length_rsp, data_rsp};
    char outmsg2[] = {rsp.start, command, length_rsp, data_rsp, checkSum(outmsg)};
    addOutBuffer(outmsg2, sizeof(outmsg2));
    USB_TX_Task();
    return;
}

/* Read response message and match with command*/
static bool validateRSP(void)
{
    uint8_t chksm_read = readBuffer[3+readBuffer[2]];

    //only checks checksum and that command bytes match.
    //might implement more checks
        
    /*Command Instruction Set*/
    if (checkSum(readBuffer) != chksm_read)
    {
        char outmsg[] = {rsp.start, readBuffer[1], 0x01, rsp.invalidchk};
        char outmsg2[] = {rsp.start, readBuffer[1], 0x01, rsp.invalidchk, checkSum(outmsg)};
        addOutBuffer(outmsg2, sizeof(outmsg2));
    }
    else
    { 
        return (lastMSG[1] == readBuffer[1]);
    }
}

static void ping(void)
{
    if(appData.heartbeat)
    {
        appData.heartbeat = false;
        appData.heartbeat_timeouts++;
        
        if (appData.heartbeat_timeouts <= 5)
        {
            char outmsg[] = {cmd.start, 0x16, 0x01, 0x01};
            char outmsg2[] = {cmd.start, 0x16, 0x01, 0x01, checkSum(outmsg)};
            addOutBuffer(outmsg2, sizeof(outmsg2));
            USB_TX_Task();
            appData.state = APP_STATE_UPDATE_INPUTS;
        }
        else
        {
            appData.state = APP_STATE_DISCONNECTED;
            APP_Reset();
        }
    }
    else
    {
        appData.state = APP_STATE_UPDATE_INPUTS;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************


void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    /* Device Layer Handle  */
    appData.deviceHandle = USB_DEVICE_HANDLE_INVALID ;

    /* Device configured status */
    appData.isConfigured = false;

    /* Initial get line coding state */
    appData.getLineCodingData.dwDTERate   = 9600;
    appData.getLineCodingData.bParityType =  0;
    appData.getLineCodingData.bParityType = 0;
    appData.getLineCodingData.bDataBits   = 8;

    /* Read Transfer Handle */
    appData.readTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;

    /* Write Transfer Handle */
    appData.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
    
    appData.sysResponseTimerHandle = SYS_TMR_HANDLE_INVALID;
    //appData.startTick = 0;
    
    /* Initialize resetable data */    
    APP_Reset();
    
}

void APP_Reset(void)
{   
    /* Initialize the read data */
    appData.readProcessedLen = 0;
    appData.readLen = 1;
    
    appData.lastmsgLen = 0;
    appData.needresponse = false;
    appData.responsetimer = false;

    //Initialize System Inputs
    input.opmode_run_ignore = false;
    input.opmode_run_sof_event = false;
    input.opmode_run_debounce_timer = 0;
    input.opmode_service_ignore = false;
    input.opmode_service_sof_event = false;
    input.opmode_service_debounce_timer = 0;
    
    input.help_sw_ignore = false;
    input.help_sw_sof_event = false;
    input.help_sw_debounce_timer = 0;
    input.helpSW = false;
    
    input.system_reset_ignore = false;
    input.system_reset_sof_event = false;
    input.system_reset_debounce_timer = 0;
    input.systemReset = false;    
    
    input.tamper_bot_ignore = false;
    input.tamper_bot_sof_event = false;
    input.tamper_bot_debounce_timer = 0;
    input.tamperBot = false;
    
    input.tamper_top_ignore = false;
    input.tamper_top_sof_event = false;
    input.tamper_top_debounce_timer = 0;
    input.tamperTop = false;
    
    input.proxSensor = false;
    input.upsAlarm = false;
    input.upsCharge = false;
    input.upsDischarge = false;
    
    input.debounce_max = 1280;
    
    //Initialize System Outputs
    // logic inverted for all except fans
    output.fan1On = false;
    FAN_1_PWMOff();
    output.fan2On = false;
    FAN_2_PWMOff();
    setStatusIndicator(RED);
    output.statusPattern = FULL;
    output.flashDutyCycle = 100;
    output.flashOn = false;
    FLASH_CTRLOn();
    RSVD_OUT_1On();
    RSVD_OUT_2On();
    RSVD_OUT_3On();
    RSVD_OUT_4On();  
    
    //Initialize Instruction set
    cmd.start = 0xff;
    //cmd.reset = 0x01;
    //cmd.flash = 0x07;

    rsp.invalidchk = 0x02;
    rsp.start = 0xfe;
    
    appData.heartbeat = true;
    appData.heartbeat_timeouts = 0;

}

/*Main Application State Machine*/
void APP_Tasks ( void )
{
  
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
 
            appData.sysResponseTimerHandle = SYS_TMR_CallbackPeriodic(500, 0, responsetimer_callback);
            appData.heartbeatTimerHandle = SYS_TMR_CallbackPeriodic(2000, 0, heartbeat_callback);

            /* Open the device layer */
            if (appData.deviceHandle == USB_DEVICE_HANDLE_INVALID)
            {
                appData.deviceHandle = USB_DEVICE_Open( USB_DEVICE_INDEX_0,
                                               DRV_IO_INTENT_READWRITE );
                appInitialized &= ( USB_DEVICE_HANDLE_INVALID != appData.deviceHandle );
            }
        
            if (appInitialized)
            {

                /* Register a callback with device layer to get event notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.deviceHandle,
                                           APP_USBDeviceEventHandler, 0);
            
                appData.state = APP_STATE_DISCONNECTED;
            }
            break;
        }
        case APP_STATE_PING:
        {
            ping();
            break;
        }
           
        case APP_STATE_DISCONNECTED:
        {
            if(appData.heartbeat)
            {
                appData.heartbeat = false;
                char outmsg[] = {cmd.start, 0x16, 0x01, 0x01};
                char outmsg2[] = {cmd.start, 0x16, 0x01, 0x01, checkSum(outmsg)};
                addOutBuffer(outmsg2, sizeof(outmsg2));
                USB_TX_Task();
            }
            USB_RX_Task();


            if ((readBuffer[0] == 0xfe) && (readBuffer[1] = 0x16))
            {
                setStatusIndicator(YELLOW);
                appData.connected = true;
                appData.state = APP_STATE_UPDATE_INPUTS; 
                memcpy(readBuffer, clearData, sizeof(clearData));
            }
        break;
        }
        
        case APP_STATE_UPDATE_INPUTS:
        {
            input_old = input;
            readInputs();
            if (compareInputState())
            {
                appData.state = APP_STATE_READ_USB;
            }
            else
            {
                appData.state = APP_STATE_STATUS_UPDATE;
            }
            break;
        }
        
        case APP_STATE_STATUS_UPDATE:
        {
            sendUpdate();
            appData.needresponse = true;
            appData.responsetimer = false;
            appData.state = APP_STATE_READ_USB;
            break;
        }
        
        case APP_STATE_READ_USB:
        {
            USB_RX_Task();
       
            if (readBuffer[0] == 0xff)
            {
                appData.state = APP_STATE_PROCESS_CMD;   
                break;
            }
            
            else if (readBuffer[0] == 0xfe)
            {
                appData.state = APP_STATE_PROCESS_RSP;
                break;
            }
            
            else
            {
                appData.state = APP_STATE_CHECK_TIMEOUT;
                break;
            }

            break;
        }
        
        case APP_STATE_PROCESS_CMD:
        {
            processCMD();

            appData.state = APP_STATE_PING;

            break;
        }
        
        case APP_STATE_PROCESS_RSP:
        {
            if(readBuffer[1]==0x16)
            {
                appData.heartbeat_timeouts = 0;
            }
            
            if (appData.needresponse)
            {   
                if(validateRSP())
                {
                    appData.needresponse = false;
                    appData.state = APP_STATE_PING;
                }
                else
                {
                    appData.state = APP_STATE_CHECK_TIMEOUT;
                }
            }
            else
            {
                appData.state = APP_STATE_UPDATE_INPUTS;
            }
            
            memcpy(readBuffer, clearData, sizeof(clearData));
            break;
        }
        
        case APP_STATE_CHECK_TIMEOUT:
        {
            if(appData.needresponse)
            {
                if (appData.responsetimer)
                {
                    addOutBuffer(lastMSG, appData.lastmsgLen);
                    appData.responsetimer = false;
                    USB_TX_Task();
                }
                appData.state = APP_STATE_READ_USB;
            }
            else
            {
                appData.state = APP_STATE_PING;
            }
            
            break;
        }
        
        /* The default state should never be executed. */
        default:
        {
            /* add error message
             * reset data, return to init. */
            APP_Reset();
            appData.state = APP_STATE_INIT;
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
