 /*******************************************************************************
  Clock System Service implementation

  Company:
    Microchip Technology Inc.

  File Name:
    sys_clk_pic32mx.c

  Summary:
    Clock system service implementation for PIC32 MX devices.

  Description:
   	This file Implements the functions specific to PIC32 MX devices.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

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
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************
#include <stddef.h>
#include "system_config.h"
#include "system/devcon/sys_devcon.h"
#include "system/clk/src/sys_clk_pic32mx.h"
#include "framework/system/clk/src/sys_clk_local.h"
#include <stdlib.h>


// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************

/******************************************************************************
  Function:
    OSC_PERIPHERAL_BUS _SYS_CLK_GetOscPeripheralBus ( CLK_BUSES_REFERENCE peripheralBus )

  Summary:
    Helper function to remap periheral bus enums to PLIB OSC enums

  Description:
    Helper function to remap periheral bus enums to PLIB OSC enums

  Remarks:
    None.
*/
OSC_PERIPHERAL_BUS _SYS_CLK_GetOscPeripheralBus ( CLK_BUSES_PERIPHERAL peripheralBus )
{
    OSC_PERIPHERAL_BUS oscBus = OSC_PERIPHERAL_BUS_1;

    switch(peripheralBus)
    {
        case CLK_BUS_PERIPHERAL_1:
            oscBus = OSC_PERIPHERAL_BUS_1;
            break;
        default:
            break;
    }

    return oscBus;
}

/******************************************************************************
  Function:
    unsigned int SYS_CLK_SystemClockSet ( SYS_CLK_SOURCE clockSource,
                                            unsigned long clockFrequency )
  Summary:
    Sets the clock source and frequency for the system clock.

  Description:
    This function sets the clock frequency and source for the system clock.

  Remarks:
    None.
*/

bool _SYS_CLK_SystemClockSet ( CLK_SOURCES_SYSTEM systemSource,
	uint32_t systemClockHz, bool waitUntilComplete, uint32_t *clockClosest )
{
    unsigned char calculatedRegValue = 0, valueToPLLMULT = 0, valueToPLLODIV = 0;
    unsigned long achievedFreq = 0;
    int8_t result;
    
    switch ( systemSource )
    {
        case SYS_CLK_SOURCE_FRC_BY_DIV:
            /* As we know the output and input frequencies, get the multiplier
            needed to get the output */
            calculatedRegValue = ( SYS_CLK_FRC_CLOCK / systemClockHz );

            /* Find the closest register value in the hardware */
            achievedFreq = SYS_CLK_FRCClosestDivisorGet ( calculatedRegValue,
                                                    &valueToPLLMULT, systemClockHz );
            break;
        case SYS_CLK_SOURCE_FRC_BY_16:
            /* The requested clock is FRC/16 */
            achievedFreq = SYS_CLK_FRC_CLOCK / 16;
            systemClockHz = achievedFreq;
            break;

        case SYS_CLK_SOURCE_LPRC:
            /* Processor's internal LPRC clock */
            achievedFreq = SYS_CLK_LPRC_CLOCK;
            systemClockHz = achievedFreq;
            break;

        case SYS_CLK_SOURCE_SECONDARY:
            achievedFreq = SYS_CLK_CONFIG_SECONDARY_XTAL;
            systemClockHz = achievedFreq;
            break;
        case SYS_CLK_SOURCE_PRIMARY_SYSPLL:
            /* Find the closest register value in the hardware */
            achievedFreq = SYS_CLK_ClosestFactorsGet ( SYS_CLK_CONFIG_PRIMARY_XTAL,
                    systemClockHz, &valueToPLLMULT, &valueToPLLODIV );
            break;

        case SYS_CLK_SOURCE_PRIMARY:
            /* If the System clock is directly using the from the Primary oscillator
            clock, return the configured primary oscillator frequency */
            achievedFreq = SYS_CLK_CONFIG_PRIMARY_XTAL;
            systemClockHz = achievedFreq;
            break;

        case SYS_CLK_SOURCE_FRC_SYSPLL:
            /* Source of system clock is internal fast RC with PLL.
             Find the closest register value in the hardware */
            achievedFreq = SYS_CLK_ClosestFactorsGet ( SYS_CLK_FRC_CLOCK,
                    systemClockHz, &valueToPLLMULT, &valueToPLLODIV );
            break;

        case SYS_CLK_SOURCE_FRC:
            /* Source of system clock is internal fast RC. If the System clock
            is directly using the fast RC clock, return the Fast RC clock */
            achievedFreq = SYS_CLK_FRC_CLOCK;
            systemClockHz = achievedFreq;
            break;
        default:
            break;
    }
    /* Check whether the difference between the achievable frequency and the
    frequecy requested is within the allowed range */
    if ( true == SYS_CLK_FrequencyErrorCheck ( systemClockHz, achievedFreq ) )
    {
        result = SYS_CLK_SystemClockSourceSwitch ( systemSource, valueToPLLMULT,
                    valueToPLLODIV, waitUntilComplete );

        /* Update the new clock source and clock frequency for the system
        clock */
        if ( result == -1 )
        {
            clkObject.systemClockStatus = SYS_CLK_OPERATION_FAIL_SWITCHING_DISABLED;
            return false;
        }

        /* Update the clock source and frequency */
        clkObject.systemClockSource = result;
        clkObject.systemClock = achievedFreq;
        clkObject.systemClosestClock = achievedFreq;
    }
    else
    {
        /* Requested frequency is not achievable. Update the error code and closest
        possible frequency */
        clkObject.systemClosestClock = achievedFreq;
        clkObject.systemClockStatus = SYS_CLK_OPERATION_FAIL_NOT_ACHIEVABLE;
        return false;
    }

    /* Control is here, means switching operation is either complete or in
    progress, not failure */
    return true;
}

/******************************************************************************
  Function:
    uint32_t _SYS_CLK_PLLInputDividerRead ( void )

  Summary:
    Reads the value of the PLL Input Divider.

  Description:
    This function will read from the configuration the current value for the
    PLL Input Divider.  The value is translated from the configuration bit into
 *  the intended divider value.

  Remarks:
    None.
*/

uint32_t _SYS_CLK_PLLInputDividerRead ( void )
{
    uint32_t inputDiv = 0;
    
    #if defined (PLIB_OSC_ExistsSysPLLInputDivisor)
    if (PLIB_OSC_ExistsSysPLLInputDivisor( OSC_PLIB_ID))
    {
        inputDiv = PLIB_OSC_SysPLLInputDivisorGet( OSC_PLIB_ID );
    }
    #else
        inputDiv = DEVCFG2bits.FPLLIDIV + 1;
    #endif

    return inputDiv;
}


/******************************************************************************
  Function:
    uint32_t _SYS_CLK_PLLMultiplierRead ( void )

  Summary:
    Reads the value of the PLL Multiplier.

  Description:
    This function will read from the configuration the current value for the
    PLL Input Divider.  The value is translated from the configuration bit into
 *  the intended divider value.

  Remarks:
    None.
*/

uint32_t _SYS_CLK_PLLMultiplierRead ( void )
{
    return PLIB_OSC_SysPLLMultiplierGet( OSC_PLIB_ID );
}


/******************************************************************************
  Function:
    uint32_t _SYS_CLK_PLLOutputDividerRead ( void )

  Summary:
    Reads the value of the PLL Multiplier.

  Description:
    This function will read from the configuration the current value for the
    PLL Output Divider.  The value is translated from the configuration bit into
 *  the intended divider value.

  Remarks:
    None.
*/

uint32_t _SYS_CLK_PLLOutputDividerRead ( void )
{
    return PLIB_OSC_SysPLLOutputDivisorGet( OSC_PLIB_ID );
}


/******************************************************************************
  Function:
    uint32_t _SYS_CLK_FRCDividerRead ( void )

  Summary:
    Reads the value of the FRC Divider.

  Description:
    This function will read from the configuration the current value for the
    FRC Divider.  The value is translated from the configuration bit into
    the intended divider value.

  Remarks:
    None.
*/

uint32_t _SYS_CLK_FRCDividerRead ( void )
{
    return PLIB_OSC_FRCDivisorGet( OSC_PLIB_ID );
}


/******************************************************************************
  Function:
    uint32_t _SYS_CLK_SystemClockRead ( void )
  Summary:
    Gets the system clock from the hardware.

  Description:
    This function gets the System clock frequency from the hardware by using
    register reads.

  Remarks:
    None.
*/

uint32_t _SYS_CLK_SystemClockRead ( void )
{
    CLK_SOURCES_SYSTEM systemSource;
    uint32_t systemClockHz = 0, multiplier = 0, divisor = 0, pllInput = 0;
    uint32_t inputDivisor = 0;

    /* Read the current system clock source from the register */
    systemSource = PLIB_OSC_CurrentSysClockGet ( OSC_PLIB_ID );
    inputDivisor = _SYS_CLK_PLLInputDividerRead();

    if ( ( systemSource == SYS_CLK_SOURCE_PRIMARY_SYSPLL ) ||
            ( systemSource == SYS_CLK_SOURCE_FRC_SYSPLL ) )
    {
        /* Check if the source is a PLL based one, validate the input frequency */
        pllInput = SYS_CLK_CONFIG_PRIMARY_XTAL / inputDivisor;

        if ( ( pllInput < OSC_SYSPLL_UFIN_MIN  ) || ( pllInput > OSC_SYSPLL_UFIN_MAX ) )
        {
            clkObject.systemClockStatus = SYS_CLK_PLL_INVALID_INP_FREQUENCY;
            return 0;
        }
    }
    /* we need the source later for other clocks  */
    clkObject.systemClockSource = systemSource;
    switch ( systemSource )
    {
        case SYS_CLK_SOURCE_FRC_BY_DIV:
            /* System clock is FRC clock divided by the configured divisor value */
            divisor = _SYS_CLK_FRCDividerRead();
            
            /* FRC clock is a constant. Do direct division */
            systemClockHz = ( SYS_CLK_FRC_CLOCK / divisor );
            break;
        case SYS_CLK_SOURCE_FRC_BY_16:
            /* The System clock clock is FRC/16 */
            systemClockHz =( SYS_CLK_FRC_CLOCK / 16 );
            break;

        case SYS_CLK_SOURCE_LPRC:
            /* System clock is processor's internal LPRC clock */
            systemClockHz = SYS_CLK_LPRC_CLOCK;
            break;
        case SYS_CLK_SOURCE_SECONDARY:
            /* System clock is processor's Secondary oscillator. The secondary
               oscillator frequency is user configured */
            systemClockHz = SYS_CLK_CONFIG_SECONDARY_XTAL;
            break;
        case SYS_CLK_SOURCE_PRIMARY_SYSPLL:
            /* Source of system clock is primary oscillator multiplied by
            the PLL value and divided by the divisor configured by software.
            Software cannot access the input divisor, so that must be configured
            by the user */
            multiplier = _SYS_CLK_PLLMultiplierRead();
            divisor = _SYS_CLK_PLLOutputDividerRead();

            systemClockHz = ( SYS_CLK_CONFIG_PRIMARY_XTAL * multiplier ) /
                    ( divisor * inputDivisor );
            break;
        /* Source of system clock is primary oscillator */
        case SYS_CLK_SOURCE_PRIMARY:
            /* If the System clock is directly using the Secondary oscillator
            clock, return the configured primary oscillator frequency */
            systemClockHz = SYS_CLK_CONFIG_PRIMARY_XTAL;
            break;
        case SYS_CLK_SOURCE_FRC_SYSPLL:
            /* Source of system clock is internal fast RC with PLL and divisors */
            multiplier = _SYS_CLK_PLLMultiplierRead();
            divisor = _SYS_CLK_PLLOutputDividerRead();

            systemClockHz = ( SYS_CLK_FRC_CLOCK * multiplier ) /
                    ( divisor * inputDivisor );
            break;
        /* Source of system clock is internal fast RC */
        case SYS_CLK_SOURCE_FRC:
            /* If the System clock is directly using the fast RC clock,
            return the Fast RC clock */
            systemClockHz = SYS_CLK_FRC_CLOCK;
            break;
        default:
            /* Something wrong */
            systemClockHz = 0;
            break;
    }

    return systemClockHz;
}


/******************************************************************************
  Function:
    uint32_t _SYS_CLK_PeripheralClockRead ( CLK_BUSES_PERIPHERAL peripheralBus, uint32_t systemClockHz )

  Summary:
    Read the peripheral clock frequency from the hardware itself and initialize
    the internal variables of the System service.

  Description:
    This function will read the peripheral clock frequency from the hardware itself
    and initialize the internal variables of the System service.

  Remarks:
    None.
*/

uint32_t _SYS_CLK_PeripheralClockRead ( CLK_BUSES_PERIPHERAL peripheralBus, uint32_t systemClockHz )
{
    return SYS_CLK_BUS_PERIPHERAL_1;
}


/******************************************************************************
  Function:
    bool _SYS_CLK_PeripheralFrequencySet ( CLK_BUSES_PERIPHERAL peripheralBus,
        CLK_SOURCES_PERIPHERAL peripheralSource, uint32_t peripheralClockHz,
        bool waitUntilComplete, uint32_t *clockClosest ) )

  Summary:
    Sets the peripheral clock frequency.

  Description:
    This function sets the peripheral clock frequency.

  Remarks:
    None.
*/

bool _SYS_CLK_PeripheralFrequencySet ( CLK_BUSES_PERIPHERAL peripheralBus,
        uint32_t peripheralClockHz,
        uint32_t *clockClosest )
{
    uint8_t index = 0;
    uint16_t divisorMatch = 0, divisor = 0;
    unsigned long tempFrequency, prevFrequency = 0;

    /* All MX devices have a single peripheral bus at the moment */
    if (peripheralBus != CLK_BUS_PERIPHERAL_1)
    {
        *clockClosest = 0;
        return false;
    }

    for ( index=0; index<4; index++ )
    {
        divisor = 0x01 << index;
        /* Here the code is too specific to PIC32MX. The code may not be so easily
        usable for other controllers. But seems like this is the best way to avoid
        RAM usage for lookup tables */

        tempFrequency = ( clkObject.systemClock / divisor );

        if ( ( abs( peripheralClockHz - tempFrequency ) ) <
            ( abs ( peripheralClockHz - prevFrequency ) ) )
        {
            prevFrequency = tempFrequency;
            divisorMatch = divisor;
        }
    }
    if ( true == SYS_CLK_FrequencyErrorCheck ( peripheralClockHz, prevFrequency ) )
    {
        /* Unlock before you switch */
        SYS_DEVCON_SystemUnlock ( );

        PLIB_OSC_PBClockDivisorSet ( OSC_ID_0, peripheralBus, divisorMatch );

        SYS_DEVCON_SystemLock ( );

        clkObject.peripheralClockStatus[peripheralBus]
                = SYS_CLK_OPERATION_COMPLETE;
    }
    else
    {
        /* Requested frequency is not achievable. Update the error code and closest
        possible frequency */
        clkObject.peripheralClosestClock[peripheralBus] = prevFrequency;

        clkObject.peripheralClockStatus[peripheralBus]
                = SYS_CLK_OPERATION_FAIL_NOT_ACHIEVABLE;

        return false;
    }
    
    *clockClosest = prevFrequency;
    return true;
}


/******************************************************************************
  Function:
    unsigned long SYS_CLK_ClosestFactorsGet(    unsigned long sourceFrequency,
                                                unsigned long requestedFrequency,
                                                unsigned char *valueToPLLMULT,
                                                unsigned char *valueToPLLODIV )

  Summary:
    Gets the value of the multiplier and divisor which is closest to the
    expected value for the system PLL.

  Description:
    This function gets the value of the multiplier and divisor which is closest
    to the expected value for the system PLL.

  Remarks:
    None.
*/

unsigned long SYS_CLK_ClosestFactorsGet ( unsigned long sourceFrequency,
                                        unsigned long requestedFrequency,
                                        unsigned char *valueToPLLMULT,
                                        unsigned char *valueToPLLODIV )
{
    uint8_t multCount = 0, divCount = 0;
    uint16_t multiplier = 0, divisor = 0, inputDivisor = 0;
    unsigned long tempFrequency = 0;

    inputDivisor = _SYS_CLK_PLLInputDividerRead();

    for ( multCount=0; multCount < 8; ++multCount )
    {
        multiplier = multCount + 15;

        /* Here the code is too specific to PIC32MX. The code may not be so easily
        usable for other controllers. But seems like this is the best way to avoid
        RAM usage for lookup tables */
        if ( multiplier == 22 || multiplier == 23 )
        {
            multiplier = 24;
        }

        for ( divCount=0; divCount < 8; ++divCount )
        {
            if ( divCount == 7 )
            {
                divCount = 8;
            }

            divisor = 0x01 << divCount;

            tempFrequency = sourceFrequency / inputDivisor * multiplier /  divisor;

            if ( ( abs( requestedFrequency - tempFrequency ) ) <= SYS_CLK_CONFIG_FREQ_ERROR_LIMIT )
            {
                *valueToPLLMULT = multiplier;
                *valueToPLLODIV = divCount;
                return tempFrequency;
            }
        }
    }

    return -1;
}


/******************************************************************************
  Function:
   char SYS_CLK_FRCClosestDivisorGet( unsigned char regValue,
                                                    unsigned char *valueToPLLMULT )

  Summary:
    Gets the value of the FRC clock divisor which is closest to the
    expected value.
.
  Description:
    This function gets the value of the FRC clock divisor which is
    closest to the expected value.

  Remarks:
    None.
*/

uint32_t SYS_CLK_FRCClosestDivisorGet ( unsigned char regValue,
                            unsigned char *valueToPLLMULT, uint32_t requestedFrequency )
{
    uint8_t index = 0;
    uint16_t divisor = 0;
    uint32_t tempFrequency = 0, PrevFrequency =0;

        for ( index=0; index<8; index++ )
        {
          divisor = index;
          /* Here the code is too specific to PIC32MX. The code may not be so easily
          usable for other controllers. But seems like this is the best way to avoid
          RAM usage for lookup tables */
         if ( divisor == 7 )
         {
             divisor++;
         }
        tempFrequency = ( SYS_CLK_FRC_CLOCK / ( 0x01 << divisor ) );

            if ( ( abs( requestedFrequency - tempFrequency ) ) <
                    ( abs ( requestedFrequency - PrevFrequency ) ) )
            {
                    PrevFrequency = tempFrequency;
                    *valueToPLLMULT = index;
            }
        }
        return PrevFrequency;
}

/******************************************************************************
  Function:
    bool SYS_CLK_FrequencyErrorCheck( unsigned long inpFreq,
                                        unsigned long acheivedFreq )
  Summary:
    Checks whether the difference between the user requested clock rate and
    the maximum achievable clock is within limit.

  Description:
    This function checks whether the difference between the user requested clock
    rate and the maximum achievable clock is within limit.
   		true - The differece betwee clocks is within limit.
		false - The differece betwee clocks is not within limit.

  Remarks:
    None.
*/

bool SYS_CLK_FrequencyErrorCheck ( unsigned long inpFreq, unsigned long acheivedFreq )
{
    /* Get the difference between frequency requested and achieved  */
    if (abs(inpFreq - acheivedFreq) > SYS_CLK_CONFIG_FREQ_ERROR_LIMIT )
    {
        return false;
    }
    return true;
}


/******************************************************************************
  Function:
    void SYS_CLK_SystemClockSourceSwitch ( SYS_CLK_SOURCE clockSource )

  Summary:
    Initiates the clock switch for the system clock.

  Description:
    This function initiates the clock switch for the system clock.

  Remarks:
    None.
*/

int8_t SYS_CLK_SystemClockSourceSwitch ( CLK_SOURCES_SYSTEM clockSource,
                                        unsigned char valueToPLLMULT,
                                        unsigned char valueToPLLODIV,
                                        bool waitUntilComplete )
{
    uint32_t delay= 100;
    /* Check whether PLL and clock selections are locked */
    if( PLIB_OSC_PLLClockIsLocked ( OSC_PLIB_ID ) != true )
    {
        /* Unlock before you switch */
        SYS_DEVCON_SystemUnlock ( );

        /* Special treatment if the source is a PLL based source. Do not change
        multipliers after switching. Switch to a FRC source first, then alter
        the multiplier values. */
        if ( ( ( clockSource == SYS_CLK_SOURCE_PRIMARY_SYSPLL ) ) ||
            ( clockSource == SYS_CLK_SOURCE_FRC_SYSPLL ) )
        {
            PLIB_OSC_SysClockSelect ( OSC_PLIB_ID, SYS_CLK_SOURCE_FRC );

            /* Ensure that we are not on a PLL based source before switching */

            /* Write the values before switching. Datasheet says so. */
            PLIB_OSC_SysPLLMultiplierSelect ( OSC_PLIB_ID, valueToPLLMULT );
            PLIB_OSC_SysPLLOutputDivisorSet ( OSC_PLIB_ID, valueToPLLODIV );

            /* Switch to a PLL source */
            PLIB_OSC_SysClockSelect ( OSC_PLIB_ID, clockSource );

            delay = SYS_CLK_SYSPLL_LOCK_DELAY_80MHZ;

            /* Wait till the PLL settles down / locked  */
            clkObject.systemClockStatus = SYS_CLK_OPERATION_IN_PROGRESS;

            /* Device expects this delay. We are changing the System clock source here,
            On debugging sometimes the code just stops without this delay. Have to
            understand this further and fine tune this delay. */
            Nop();
            Nop();
            Nop();

            if ( waitUntilComplete == true )
            {
                #if defined (PLIB_OSC_ExistsPLLLockStatus)
                while ( PLIB_OSC_PLLIsLocked ( OSC_PLIB_ID, OSC_PLL_SYSTEM ) != true )
                #endif
                {
                    if ( delay-- <= 0 )
                    {
                        /* Rarest case. Either the PLL lock failed or the bit is showing
                        the wrong status, A restart needed, an error status update will
                        not help */
                        clkObject.systemClockStatus = SYS_CLK_OPERATION_FAIL_PLL_LOCK_TIMEOUT;

                        //SYS_ASSERT("PLL lock timeout");
                        Nop();
                        /* Reaching here means clock switching is not enabled
                        through configuration bits */
                        return -1;
                    }
                }
                clkObject.systemClockStatus = SYS_CLK_OPERATION_COMPLETE;
            }
        }
        else
        {
            /* If the new oscillator is not a PLL source, the one we are running on may
            be. So switch first before setting the divisor */
            PLIB_OSC_SysClockSelect ( OSC_PLIB_ID, clockSource );

            if ( clockSource == SYS_CLK_SOURCE_FRC_BY_DIV )
            {
                PLIB_OSC_FRCDivisorSelect ( OSC_PLIB_ID, valueToPLLMULT );
            }
        }
        /* Lock it before going back */
        SYS_DEVCON_SystemLock ( );
    
        /* Return something which is not equal to '-1'. Let us return the clock source
        itself */
        return clockSource;
    }
    return -1;
}

