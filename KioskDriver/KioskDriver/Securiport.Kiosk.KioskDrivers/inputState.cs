using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Securiport.Kiosk.KioskDrivers
{
    public struct inputState
    {
        internal delegate void KioskInputStatusChangedEventHandler(object sender, string propertyName, object value);
        internal event KioskInputStatusChangedEventHandler InputStateChanged;

        private void OnInputStatusChanged(bool previous, bool now, string name)
        {
            if (previous != now)
            {
                InputStateChanged?.Invoke(this, name, now);
            }
        }

        private bool _modeselect_run;
        public bool modeselect_run
        {
            get { return _modeselect_run; }
            set { if (_modeselect_run != value) { _modeselect_run = value; OnInputStatusChanged(!value, value, "modeselect_run"); } }
        }

        private bool _modeselect_service;
        public bool modeselect_service
        {
            get { return _modeselect_service; }
            set { if (_modeselect_service != value) { _modeselect_service = value; OnInputStatusChanged(!value, value, "modeselect_service"); } }
        }
        private bool _proximity;
        public bool proximity
        {
            get { return _proximity; }
            set { if (_proximity != value) { _proximity = value; OnInputStatusChanged(!value, value, "proximity"); } }
        }
        private bool _helpsw;
        public bool helpsw
        {
            get { return _helpsw; }
            set { if (_helpsw != value) { _helpsw = value; OnInputStatusChanged(!value, value, "helpsw"); } }
        }

        private bool _upscharge;
        public bool upscharge
        {
            get { return _upscharge; }
            set { if (_upscharge != value) { _upscharge = value; OnInputStatusChanged(!value, value, "upscharge"); } }
        }
        private bool _upsdischarge;
        public bool upsdischarge
        {
            get { return _upsdischarge; }
            set { if (_upsdischarge != value) { _upsdischarge = value; OnInputStatusChanged(!value, value, "upsdischarge"); } }
        }
        private bool _upsfault;
        public bool upsfault
        {
            get { return _upsfault; }
            set { if (_upsfault != value) { _upsfault = value; OnInputStatusChanged(!value, value, "upsfault"); } }
        }
        private bool _tampertop;
        public bool tampertop
        {
            get { return _tampertop; }
            set { if (_tampertop != value) { _tampertop = value; OnInputStatusChanged(!value, value, "tampertop"); } }
        }
        private bool _tamperbot;
        public bool tamperbot
        {
            get { return _tamperbot; }
            set { if (_tamperbot != value) { _tamperbot = value; OnInputStatusChanged(!value, value, "tamperbot"); } }
        }
        private bool _systemreset;
        public bool systemreset
        {
            get { return _systemreset; }
            set { if (_systemreset != value) { _systemreset = value; OnInputStatusChanged(!value, value, "systemreset"); } }
        }

        public bool Equal(inputState a)
        {
            if (this.modeselect_run != a.modeselect_run) return false;
            if (this.modeselect_service != a.modeselect_service) return false;
            if (this.proximity != a.proximity) return false;
            if (this.helpsw != a.helpsw) return false;
            if (this.upscharge != a.upscharge) return false;
            if (this.upsdischarge != a.upsdischarge) return false;
            if (this.upsfault != a.upsfault) return false;
            if (this.tampertop != a.tampertop) return false;
            if (this.tamperbot != a.tamperbot) return false;
            if (this.systemreset != a.systemreset) return false;
            return true;
        }
    };
}
