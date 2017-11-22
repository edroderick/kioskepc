using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Securiport.Kiosk.KioskDrivers
{
    public class KioskLib
    {
        public delegate void StatusChangedEventHandler(object sender, string name, object value);
        public event StatusChangedEventHandler StateChanged;

        private void OnStateChanged(string name, object value)
        {
            StateChanged?.Invoke(this, name, value);
        }

        public KioskLib()
        {
            KioskLibHelper.Instance.DriverStateChanged += KioskDriverStateChanged;
        }

        private void KioskDriverStateChanged(object sender, inputState state, string name, object value)
        {
            OnStateChanged(name, value);
        }

        public bool Open(string portName)
        {
           bool isOpen = KioskLibHelper.Instance.Open(portName);

            OnStateChanged("Connected on " , portName);

            return isOpen;
        }

        public bool Close()
        {
            bool isOk = KioskLibHelper.Instance.Close();
            OnStateChanged("Disconnected from ", KioskLibHelper.Instance.Port.PortName);

            return isOk;
        }

        public void SendCommand(eKioskCommand commandSet, bool isOn)
        {
            byte cmd = (byte)commandSet;
            byte data = (int)1;
            if (isOn == false) { data = 2; }
            KioskLibHelper.Instance.sendCMD(cmd, data);
            OnStateChanged("Command: " + commandSet.ToString(), isOn);
        }


        public void SendCommand(eKioskCommand commandSet, eKioskLight state)
        {
            byte cmd = (byte)commandSet;
            byte data = (byte)state;
            KioskLibHelper.Instance.sendCMD(cmd, data);

            OnStateChanged("Command: " + commandSet.ToString(), state);
        }

    }
}
