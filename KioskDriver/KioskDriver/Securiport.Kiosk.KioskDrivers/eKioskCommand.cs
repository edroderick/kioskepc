using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Securiport.Kiosk.KioskDrivers
{
    public enum eKioskCommand
    {
        Reset = 0x01,
        Flash = 0x07,
        Status = 0x0a,
        Fan1 = 0x20,
        Fan2 = 0x22
    }
}
