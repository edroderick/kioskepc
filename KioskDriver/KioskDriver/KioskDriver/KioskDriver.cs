using System;
using System.IO.Ports;
using System.Threading;
using System.Timers;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KioskDriver
{
    public struct inputState
    {
        public bool modeselect_run;
        public bool modeselect_service;
        public bool proximity;
        public bool helpsw;
        public bool upscharge;
        public bool upsdischarge;
        public bool upsfault;
        public bool tampertop;
        public bool tamperbot;
        public bool systemreset;
    };

    public class Kiosk
    {
        static bool isconnected = false;
        static bool heartbeat = false;
        public static System.Timers.Timer timer; 

        static SerialPort port;

        static inputState state_in = new inputState();

        static bool needresponse = false;
        public static byte[] lastMSG = new byte[] {}; //implement some kind of better buffer

        public static void Main()
        {
            SetTimer(2000);

            OpenPort();


            //FlashOn();
            //TEST CODE
            while (true)
            {
                //FlashOn();
                //Thread.Sleep(1000);
                //FlashOff();
                //Thread.Sleep(1000);
                //StatusRed();
                //Thread.Sleep(1000);
                //StatusGreen();
                //Thread.Sleep(1000);
                //StatusBlue();
                //Thread.Sleep(1000);
                //StatusCyan();
                //Thread.Sleep(1000);
                //StatusPurple();
                //Thread.Sleep(1000);
                //StatusWhite();
                //Thread.Sleep(1000);
                //StatusYellow();
                //Thread.Sleep(1000);
                //StatusOff();
                Thread.Sleep(1000);

                //Thread.Sleep(100);
            }

        }

        //Command Instruction Set
        public static void Reset()
        {
            byte[] data = new byte[1];
            data[0] = 0x01;
            sendCMD(0x02, data);
        }

        public static void FlashOn()
        {
            byte[] data = new byte[1];
            data[0] = 0x01;
            sendCMD(0x07, data);
        }

        public static void FlashOff()
        {
            byte[] data = new byte[1];
            data[0] = 0x02;
            sendCMD(0x07, data);
        }

        public static void StatusOff()
        {
            byte[] data = new byte[1];
            data[0] = 0x01;
            sendCMD(0x0a, data);
        }

        public static void StatusRed()
        {
            byte[] data = new byte[1];
            data[0] = 0x02;
            sendCMD(0x0a, data);
        }

        public static void StatusGreen()
        {
            byte[] data = new byte[1];
            data[0] = 0x03;
            sendCMD(0x0a, data);
        }

        public static void StatusBlue()
        {
            byte[] data = new byte[1];
            data[0] = 0x04;
            sendCMD(0x0a, data);
        }

        public static void StatusYellow()
        {
            byte[] data = new byte[1];
            data[0] = 0x05;
            sendCMD(0x0a, data);
        }

        public static void StatusWhite()
        {
            byte[] data = new byte[1];
            data[0] = 0x06;
            sendCMD(0x0a, data);
        }

        public static void StatusPurple()
        {
            byte[] data = new byte[1];
            data[0] = 0x07;
            sendCMD(0x0a, data);
        }

        public static void StatusCyan()
        {
            byte[] data = new byte[1];
            data[0] = 0x08;
            sendCMD(0x0a, data);
        }

        public static void Fan1On()
        {
            byte[] data = new byte[1];
            data[0] = 0x01;
            sendCMD(0x20, data);
        }

        public static void Fan1Off()
        {
            byte[] data = new byte[1];
            data[0] = 0x02;
            sendCMD(0x20, data);
        }

        public static void Fan2On()
        {
            byte[] data = new byte[1];
            data[0] = 0x01;
            sendCMD(0x22, data);
        }

        public static void Fan2Off()
        {
            byte[] data = new byte[1];
            data[0] = 0x02;
            sendCMD(0x22, data);
        }

        public static void OpenPort()
        {
            port = new SerialPort("COM5", 9600, Parity.None, 8, StopBits.One);
            port.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);

            while (!isconnected)
            {
                try
                {
                    port.Open();

                    if (port.IsOpen)
                    {
                        while (!heartbeat) { };
                        isconnected = true;
                        Console.WriteLine("Connected");
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine("Error: COM Port not detected");
                    Thread.Sleep(1000);
                }

                
            }
        }

        public static void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            int bytes = sp.BytesToRead;
            byte[] inbuffer = new byte[bytes];
            sp.Read(inbuffer, 0, bytes);

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

        public static void SetTimer(int count)
        {
            timer = new System.Timers.Timer(count);
            timer.Elapsed += OnTimedEvent;
            timer.AutoReset = true;
            timer.Enabled = true;
        }

        public static void OnTimedEvent(Object source, ElapsedEventArgs e)
        {
            if(needresponse)
            {
                port.Write(lastMSG, 0, lastMSG.Length);
            }
        }

        public static void ProcessCMD(byte[] msg)
        {
            byte command = msg[1];
            byte checksum_read = msg[3 + msg[2]];
            byte[] data = msg.Skip(3).Take(msg[2]).ToArray();
            byte[] checksum_array = msg.Skip(1).Take(2 + msg[2]).ToArray();
            byte[] msgdata = new byte[1];

            if(checksum(checksum_array) != checksum_read)
            {
                //Console.WriteLine("bad check");
            }
            else
            {
                switch (command)
                {
                    case 0x01:
                        msgdata[0] = 0x01;

                        if(data[0] == 0x01)
                        {
                            state_in.modeselect_run = true;
                            state_in.modeselect_service = false;
                        }

                        if(data[0] == 0x02)
                        {
                            state_in.modeselect_run = false;
                            state_in.modeselect_service = true;
                        }
                        break;

                    case 0x10:
                        msgdata[0] = 0x01;
                        
                        if(data[0] == 0x01)
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

        public static void ProcessRSP(byte[] msg)
        {
            if (needresponse)
            {
                if(msg[1] == lastMSG[1])
                {
                    needresponse = false;
                }
            }
        }

        public static void sendCMD(byte cmd, byte[] data)
        {
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
            port.Write(msg, 0, msg.Length);
        }

        public static void sendRSP(byte cmd, byte[] data)
        {
            byte[] msg = new byte[data.Length + 4];
            byte[] msg_end = new byte[data.Length + 3];

            msg[0] = 0xFE;
            msg[1] = cmd;
            msg[2] = (byte)data.Length;
            msg_end[0] = cmd;
            msg_end[1] = (byte)data.Length;

            int i = 1;
            foreach(byte b in data)
            {
                msg_end[1 + i] = data[i-1];
                msg[2 + i] = data[i - 1];
                i++;
            }

            msg[i+2] = checksum(msg_end);

            //Console.WriteLine(BitConverter.ToString(msg));

            port.Write(msg,0,msg.Length);
        }

        public static byte checksum(byte[] msg)
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
