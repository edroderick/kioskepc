using System;
using System.IO.Ports;
using System.Threading;

namespace Kiosk
{
    public class KioskDriver
    {
        static bool _continue;

        private SerialPort port = new SerialPort();
        private int _baudRate = 9600;
        private int _dataBits = 8;
        private Handshake _handshake = Handshake.None;
        private Parity _parity = Parity.None;
        private string _portName = "COM9";
        private StopBits _stopBits = Stopbits.One;
        private int _readTimeout = 500;
        private int _writeTimeout = 500;

        public int BaudRate { get { return _baudRate; } set { _baudRate = value; } } 
        public int DataBits { get { return _dataBits; } set { _dataBits = value; } } 
        public Handshake Handshake { get { return _handshake; } set { _handshake = value; } } 
        public Parity Parity { get { return _parity; } set { _parity = value; } } 
        public string PortName { get { return _portName; } set { _portName = value; } } 
        public int ReadTimeout { get {return _readTimeout; } set { _readTimeout = value; } }
        public int WriteTimeout { get {return _writeTimeout; } set { _writeTimeout = value; } }
        
        public static void Main()
        {
            Open();
            _continue = true;

            Thread readThread = new Thread(Read);
            readThread.Start();

        }

        public bool Open()
        {
            try
            {
                _serialPort.BaudRate = _baudRate; 
                _serialPort.DataBits = _dataBits; 
                _serialPort.Handshake = _handshake; 
                _serialPort.Parity = _parity; 
                _serialPort.PortName = _portName; 
                _serialPort.StopBits = _stopBits; 
                _serialPort.DataReceived += new SerialDataReceivedEventHandler(_serialPort_DataReceived); 

                _serialPort.Open();
            }
            catch {return false;}
            return true;
        }

        public static void Read()
        {
            while(_continue)
            {
                try
                {
                    string message = _serialPort.ReadExisting();
                    console.WriteLine(message);
                }
                catch (TimeoutException){}
            }
        }
    }
}
