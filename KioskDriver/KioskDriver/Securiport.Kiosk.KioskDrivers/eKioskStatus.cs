using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Securiport.Kiosk.KioskDrivers
{
    public enum eKioskStatus
    {
        ModeSelectRunMode = 0x01,
        Proximity = 0x10,
        HelpSW = 0x0e,
        UPSCharge = 0x12,
        UPSDischarge = 0x13,
        UPSFault = 0x14,
        HeartBit = 0x16,
        TamperTop = 0x17,
        TamperBottom = 0x19,
        SystemReset = 0x23
    }
}
