using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Securiport.Kiosk.KioskDrivers
{
    internal class KioskLibHelper
    {
        private static readonly Lazy<KioskLibHelper> lazy =
          new Lazy<KioskLibHelper>(() => new KioskLibHelper());
        public static KioskLibHelper Instance { get { return lazy.Value; } }

        private SerialPort _port;
        private System.Timers.Timer _timer;

        private inputState state_in = new inputState();
        static bool heartbeat = false;
        private bool needresponse = false;
        private byte[] lastMSG = new byte[] { }; //implement some kind of better buffer

        public delegate void KioskStatusChangedEventHandler(object sender, inputState state, string name, object value);
        public event KioskStatusChangedEventHandler DriverStateChanged;

        private KioskLibHelper()
        {
            _port = new SerialPort();
            _port.BaudRate = 9600;

            _port.DataReceived += SerialPort_DataReceived;
            _port.PinChanged += SerialPort_PinChanged;

            _timer = new System.Timers.Timer(2000);
            _timer.Elapsed += BackgroundTimer_Elapsed;
            _timer.AutoReset = true;
            _timer.Enabled = true;

            state_in = new inputState();
            state_in.InputStateChanged += State_in_InputStateChanged;
        }

        private void State_in_InputStateChanged(object sender, string propertyName, object value)
        {
            OnStateChanged(propertyName, value);
        }

        private void OnStateChanged(string name, object value)
        {
            DriverStateChanged?.Invoke(this, state_in, name, value);
        }

        private bool _timerRunning = false;
        private void BackgroundTimer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            if (_timerRunning) return;
            _timerRunning = true;
            if (needresponse)
            {
                if (_port.IsOpen)
                {
                    _port.Write(lastMSG, 0, lastMSG.Length);
                }
            }
            _timerRunning = false;
        }

        public  SerialPort Port {  get { return _port; } }
        public bool Open(string portName)
        {
            bool result = false;

            try
            {
                _port.PortName = portName;
                _port.Open();
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            return result;
        }

        public bool Close()
        {
            bool result = false;

            try
            {
                if (_port.IsOpen)
                {
                    _port.Close();
                }

                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            return result;
        }

        private void SerialPort_PinChanged(object sender, SerialPinChangedEventArgs e)
        {
            switch (e.EventType)
            {
                case SerialPinChange.Break:
                    Close();
                    break;
                case SerialPinChange.CDChanged:
                    break;
                case SerialPinChange.CtsChanged:
                    break;
                case SerialPinChange.DsrChanged:
                    break;
                case SerialPinChange.Ring:
                    break;
                default:
                    break;
            }
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                SerialPort port = (SerialPort)sender;
                int numberOfDataReceived = port.BytesToRead;
                if (numberOfDataReceived > 0)
                {
                    byte[] inbuffer = new byte[numberOfDataReceived];
                    port.Read(inbuffer, 0, numberOfDataReceived);

                    if (inbuffer != null)
                    {
                        Console.WriteLine(BitConverter.ToString(inbuffer));
                        if (inbuffer[0] == 0xFF)
                        {
                            ProcessCMD(inbuffer);
                        }
                        else
                        {
                            ProcessRSP(inbuffer);
                        }
                    }
                }

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        private void ProcessCMD(byte[] msg)
        {
            byte command = msg[1];
            byte checksum_read = msg[3 + msg[2]];
            byte[] data = msg.Skip(3).Take(msg[2]).ToArray();
            byte[] checksum_array = msg.Skip(1).Take(2 + msg[2]).ToArray();
            byte[] msgdata = new byte[1];

            if (checksum(checksum_array) != checksum_read)
            {
                //Console.WriteLine("bad check");
            }
            else
            {
                eKioskStatus state = (eKioskStatus)command;
                switch (command)
                {
                    case 0x01:
                        msgdata[0] = 0x01;

                        if (data[0] == 0x01)
                        {
                            state_in.modeselect_run = true;
                            state_in.modeselect_service = false;
                        }

                        if (data[0] == 0x02)
                        {
                            state_in.modeselect_run = false;
                            state_in.modeselect_service = true;
                        }
                        break;

                    case 0x10:
                        msgdata[0] = 0x01;

                        if (data[0] == 0x01)
                        {
                            state_in.proximity = true;
                        }
                        else
                        {
                            state_in.proximity = false;
                        }
                        break;

                    case 0x0e:
                        msgdata[0] = 0x01;

                        if (data[0] == 0x01)
                        {
                            state_in.helpsw = true;
                        }
                        else
                        {
                            state_in.helpsw = false;
                        }
                        break;

                    case 0x12:
                        msgdata[0] = 0x01;

                        if (data[0] == 0x01)
                        {
                            state_in.upscharge = true;
                        }
                        else
                        {
                            state_in.upscharge = false;
                        }
                        break;

                    case 0x13:
                        msgdata[0] = 0x01;

                        if (data[0] == 0x01)
                        {
                            state_in.upsdischarge = true;
                        }
                        else
                        {
                            state_in.upsdischarge = false;
                        }
                        break;

                    case 0x14:
                        msgdata[0] = 0x01;

                        if (data[0] == 0x01)
                        {
                            state_in.upsfault = true;
                        }
                        else
                        {
                            state_in.upsfault = false;
                        }
                        break;

                    case 0x16:
                        heartbeat = true;
                        msgdata[0] = 0x01;
                        break;

                    case 0x17:
                        msgdata[0] = 0x01;

                        if (data[0] == 0x01)
                        {
                            state_in.tampertop = true;
                        }
                        else
                        {
                            state_in.tampertop = false;
                        }
                        break;

                    case 0x19:
                        msgdata[0] = 0x01;

                        if (data[0] == 0x01)
                        {
                            state_in.tamperbot = true;
                        }
                        else
                        {
                            state_in.tamperbot = false;
                        }
                        break;

                    case 0x23:
                        msgdata[0] = 0x01;

                        if (data[0] == 0x01)
                        {
                            state_in.systemreset = true;
                        }
                        else
                        {
                            state_in.systemreset = false;
                        }
                        break;
                }

                sendRSP(command, msgdata);
            }

        }

        private void ProcessRSP(byte[] msg)
        {

            if (needresponse)
            {
                if (msg[1] == lastMSG[1])
                {
                    needresponse = false;
                }

                if (_port.IsOpen == false) return;
            }
        }

        private void sendRSP(byte cmd, byte[] data)
        {
            if (_port.IsOpen == false) return;

            byte[] msg = new byte[data.Length + 4];
            byte[] msg_end = new byte[data.Length + 3];

            msg[0] = 0xFE;
            msg[1] = cmd;
            msg[2] = (byte)data.Length;
            msg_end[0] = cmd;
            msg_end[1] = (byte)data.Length;

            int i = 1;
            foreach (byte b in data)
            {
                msg_end[1 + i] = data[i - 1];
                msg[2 + i] = data[i - 1];
                i++;
            }

            msg[i + 2] = checksum(msg_end);

            //Console.WriteLine(BitConverter.ToString(msg));

            _port.Write(msg, 0, msg.Length);
        }

        public void sendCMD(byte cmd, params byte[] data)
        {
            if (_port.IsOpen == false) return;

            byte[] msg = new byte[data.Length + 4];
            byte[] msg_end = new byte[data.Length + 3];

            msg[0] = 0xFF;
            msg[1] = cmd;
            msg[2] = (byte)data.Length;
            msg_end[0] = cmd;
            msg_end[1] = (byte)data.Length;

            int i = 1;
            foreach (byte b in data)
            {
                msg_end[1 + i] = data[i - 1];
                msg[2 + i] = data[i - 1];
                i++;
            }

            msg[i + 2] = checksum(msg_end);

            //Console.WriteLine(BitConverter.ToString(msg));

            needresponse = true;
            lastMSG = msg;
            _port.Write(msg, 0, msg.Length);
        }

        private byte checksum(byte[] msg)
        {
            byte sum = 0;
            unchecked
            {
                foreach (byte b in msg)
                {
                    sum += b;
                    //Console.WriteLine(sum);
                }
            }
            return (byte)~sum;
        }
    }
}
