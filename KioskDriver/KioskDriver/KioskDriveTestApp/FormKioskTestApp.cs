using Securiport.Kiosk.KioskDrivers;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace KioskDriveTestApp
{
    public partial class FormKioskTestApp : Form
    {
        public FormKioskTestApp()
        {
            InitializeComponent();
        }

        private string _messageHistory = string.Empty;
        private delegate void UpdateMessageDelegate(string msg);

        private KioskLib _kioskDevice = null;

        private void FormKioskTestApp_Load(object sender, EventArgs e)
        {
            ReloadPortName();

            _kioskDevice = new KioskLib();
            _kioskDevice.StateChanged += _kioskDevice_StateChanged;
        }

        private List<string> _listOfMsg = new List<string>();
        private void _kioskDevice_StateChanged(object sender, string name, object value)
        {
            KioskLib kiosDevice = (KioskLib)sender;
            string msg = name + " " + value.ToString();

            _listOfMsg.Insert(0, DateTime.Now.ToString("hh:mm:ss.fff") + " " + msg);
            if(_listOfMsg.Count > 1000)
            {
                _listOfMsg.RemoveRange(1000, _listOfMsg.Count - 1000);
            }
            string[] history = _listOfMsg.ToArray();
            string historyMessage = string.Join("\r\n", history);
            UpdateMessage(historyMessage);
        }



        private void ReloadPortName()
        {
            string[] portnames = SerialPort.GetPortNames();
            comboBoxSerialPort.Items.Clear();
            foreach (string name in portnames)
            {
                comboBoxSerialPort.Items.Add(name);
            }
            comboBoxSerialPort.Text = "COM5";
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            if("Connect".Equals(buttonConnect.Text))
            {
                buttonConnect.BackColor = Color.Green;
                string portName = comboBoxSerialPort.Text;

                if (string.IsNullOrEmpty(portName) == false)
                {
                    _kioskDevice.Open(portName);
                }
                buttonConnect.Text = "Disconnect";
            } else
            {
                buttonConnect.BackColor = Color.FromKnownColor(KnownColor.Control);
                buttonConnect.Text = "Connect";
                _kioskDevice.Close();
            }
        }

        private void radioButtonStatusOff_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton radioButton = (RadioButton)sender;
            if(radioButton.Checked)
            {
                _kioskDevice.SendCommand(eKioskCommand.Status, eKioskLight.Off);
            }
        }

        private void radioButtonStatusRed_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton radioButton = (RadioButton)sender;
            if (radioButton.Checked)
            {
                _kioskDevice.SendCommand(eKioskCommand.Status, eKioskLight.Red);
            }
        }

        private void radioButtonStatusGreen_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton radioButton = (RadioButton)sender;
            if (radioButton.Checked)
            {
                _kioskDevice.SendCommand(eKioskCommand.Status, eKioskLight.Green);
            }
        }

        private void radioButtonStatusBlue_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton radioButton = (RadioButton)sender;
            if (radioButton.Checked)
            {
                _kioskDevice.SendCommand(eKioskCommand.Status, eKioskLight.Blue);
            }
        }

        private void radioButtonStatusYellow_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton radioButton = (RadioButton)sender;
            if (radioButton.Checked)
            {
                _kioskDevice.SendCommand(eKioskCommand.Status, eKioskLight.Yellow);
            }
        }

        private void radioButtonStatusWhite_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton radioButton = (RadioButton)sender;
            if (radioButton.Checked)
            {
                _kioskDevice.SendCommand(eKioskCommand.Status, eKioskLight.White);
            }
        }

        private void radioButtonStatusPurple_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton radioButton = (RadioButton)sender;
            if (radioButton.Checked)
            {
                _kioskDevice.SendCommand(eKioskCommand.Status, eKioskLight.Purple);
            }
        }

        private void radioButtonStatusCyan_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton radioButton = (RadioButton)sender;
            if (radioButton.Checked)
            {
                _kioskDevice.SendCommand(eKioskCommand.Status, eKioskLight.Cyan);
            }
        }

        private void UpdateMessage(string msg)
        {
            if(this.InvokeRequired)
            {
                this.Invoke(new UpdateMessageDelegate(UpdateMessage), msg);
            } else
            {
                textBoxHistory.Text = msg;
            }
        }

        private void buttonReset_Click(object sender, EventArgs e)
        {
            _kioskDevice.SendCommand(eKioskCommand.Reset, true);
        }

        private void checkBoxFan1On_CheckedChanged(object sender, EventArgs e)
        {
            _kioskDevice.SendCommand(eKioskCommand.Fan1, checkBoxFan1On.Checked);
        }

        private void checkBoxFan2On_CheckedChanged(object sender, EventArgs e)
        {
            _kioskDevice.SendCommand(eKioskCommand.Fan2, checkBoxFan2On.Checked);
        }

        private void checkBoxFlashOn_CheckedChanged(object sender, EventArgs e)
        {
            _kioskDevice.SendCommand(eKioskCommand.Flash, checkBoxFlashOn.Checked);

            //KioskLibHelper.Instance.SendCommand(eKioskCommand.Flash, checkBoxFlashOn.Checked);
        }

        private void buttonClear_Click(object sender, EventArgs e)
        {
            _listOfMsg.Clear();
            UpdateMessage(string.Empty);
        }
    }
}
