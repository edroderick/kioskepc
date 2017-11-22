namespace KioskDriveTestApp
{
    partial class FormKioskTestApp
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.textBoxHistory = new System.Windows.Forms.TextBox();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.comboBoxSerialPort = new System.Windows.Forms.ComboBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radioButtonStatusCyan = new System.Windows.Forms.RadioButton();
            this.radioButtonStatusPurple = new System.Windows.Forms.RadioButton();
            this.radioButtonStatusWhite = new System.Windows.Forms.RadioButton();
            this.radioButtonStatusYellow = new System.Windows.Forms.RadioButton();
            this.radioButtonStatusBlue = new System.Windows.Forms.RadioButton();
            this.radioButtonStatusGreen = new System.Windows.Forms.RadioButton();
            this.radioButtonStatusRed = new System.Windows.Forms.RadioButton();
            this.radioButtonStatusOff = new System.Windows.Forms.RadioButton();
            this.checkBoxFan1On = new System.Windows.Forms.CheckBox();
            this.checkBoxFan2On = new System.Windows.Forms.CheckBox();
            this.checkBoxFlashOn = new System.Windows.Forms.CheckBox();
            this.buttonReset = new System.Windows.Forms.Button();
            this.buttonClear = new System.Windows.Forms.Button();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBoxHistory
            // 
            this.textBoxHistory.Location = new System.Drawing.Point(10, 194);
            this.textBoxHistory.Multiline = true;
            this.textBoxHistory.Name = "textBoxHistory";
            this.textBoxHistory.Size = new System.Drawing.Size(436, 300);
            this.textBoxHistory.TabIndex = 0;
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(341, 12);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(105, 24);
            this.buttonConnect.TabIndex = 1;
            this.buttonConnect.Text = "Connect";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // comboBoxSerialPort
            // 
            this.comboBoxSerialPort.FormattingEnabled = true;
            this.comboBoxSerialPort.Location = new System.Drawing.Point(214, 12);
            this.comboBoxSerialPort.Name = "comboBoxSerialPort";
            this.comboBoxSerialPort.Size = new System.Drawing.Size(121, 24);
            this.comboBoxSerialPort.TabIndex = 2;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.radioButtonStatusCyan);
            this.groupBox1.Controls.Add(this.radioButtonStatusPurple);
            this.groupBox1.Controls.Add(this.radioButtonStatusWhite);
            this.groupBox1.Controls.Add(this.radioButtonStatusYellow);
            this.groupBox1.Controls.Add(this.radioButtonStatusBlue);
            this.groupBox1.Controls.Add(this.radioButtonStatusGreen);
            this.groupBox1.Controls.Add(this.radioButtonStatusRed);
            this.groupBox1.Controls.Add(this.radioButtonStatusOff);
            this.groupBox1.Location = new System.Drawing.Point(131, 52);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(315, 107);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Status";
            // 
            // radioButtonStatusCyan
            // 
            this.radioButtonStatusCyan.AutoSize = true;
            this.radioButtonStatusCyan.BackColor = System.Drawing.SystemColors.ControlDark;
            this.radioButtonStatusCyan.ForeColor = System.Drawing.Color.Cyan;
            this.radioButtonStatusCyan.Location = new System.Drawing.Point(216, 73);
            this.radioButtonStatusCyan.Name = "radioButtonStatusCyan";
            this.radioButtonStatusCyan.Size = new System.Drawing.Size(57, 20);
            this.radioButtonStatusCyan.TabIndex = 7;
            this.radioButtonStatusCyan.TabStop = true;
            this.radioButtonStatusCyan.Text = "Cyan";
            this.radioButtonStatusCyan.UseVisualStyleBackColor = false;
            this.radioButtonStatusCyan.CheckedChanged += new System.EventHandler(this.radioButtonStatusCyan_CheckedChanged);
            // 
            // radioButtonStatusPurple
            // 
            this.radioButtonStatusPurple.AutoSize = true;
            this.radioButtonStatusPurple.BackColor = System.Drawing.SystemColors.ControlDark;
            this.radioButtonStatusPurple.ForeColor = System.Drawing.Color.Magenta;
            this.radioButtonStatusPurple.Location = new System.Drawing.Point(89, 73);
            this.radioButtonStatusPurple.Name = "radioButtonStatusPurple";
            this.radioButtonStatusPurple.Size = new System.Drawing.Size(65, 20);
            this.radioButtonStatusPurple.TabIndex = 6;
            this.radioButtonStatusPurple.TabStop = true;
            this.radioButtonStatusPurple.Text = "Purple";
            this.radioButtonStatusPurple.UseVisualStyleBackColor = false;
            this.radioButtonStatusPurple.CheckedChanged += new System.EventHandler(this.radioButtonStatusPurple_CheckedChanged);
            // 
            // radioButtonStatusWhite
            // 
            this.radioButtonStatusWhite.AutoSize = true;
            this.radioButtonStatusWhite.BackColor = System.Drawing.SystemColors.ControlDark;
            this.radioButtonStatusWhite.ForeColor = System.Drawing.Color.White;
            this.radioButtonStatusWhite.Location = new System.Drawing.Point(89, 47);
            this.radioButtonStatusWhite.Name = "radioButtonStatusWhite";
            this.radioButtonStatusWhite.Size = new System.Drawing.Size(60, 20);
            this.radioButtonStatusWhite.TabIndex = 5;
            this.radioButtonStatusWhite.TabStop = true;
            this.radioButtonStatusWhite.Text = "White";
            this.radioButtonStatusWhite.UseVisualStyleBackColor = false;
            this.radioButtonStatusWhite.CheckedChanged += new System.EventHandler(this.radioButtonStatusWhite_CheckedChanged);
            // 
            // radioButtonStatusYellow
            // 
            this.radioButtonStatusYellow.AutoSize = true;
            this.radioButtonStatusYellow.BackColor = System.Drawing.SystemColors.ControlDark;
            this.radioButtonStatusYellow.ForeColor = System.Drawing.Color.Yellow;
            this.radioButtonStatusYellow.Location = new System.Drawing.Point(89, 21);
            this.radioButtonStatusYellow.Name = "radioButtonStatusYellow";
            this.radioButtonStatusYellow.Size = new System.Drawing.Size(66, 20);
            this.radioButtonStatusYellow.TabIndex = 4;
            this.radioButtonStatusYellow.TabStop = true;
            this.radioButtonStatusYellow.Text = "Yellow";
            this.radioButtonStatusYellow.UseVisualStyleBackColor = false;
            this.radioButtonStatusYellow.CheckedChanged += new System.EventHandler(this.radioButtonStatusYellow_CheckedChanged);
            // 
            // radioButtonStatusBlue
            // 
            this.radioButtonStatusBlue.AutoSize = true;
            this.radioButtonStatusBlue.BackColor = System.Drawing.SystemColors.ControlDark;
            this.radioButtonStatusBlue.ForeColor = System.Drawing.Color.Blue;
            this.radioButtonStatusBlue.Location = new System.Drawing.Point(214, 47);
            this.radioButtonStatusBlue.Name = "radioButtonStatusBlue";
            this.radioButtonStatusBlue.Size = new System.Drawing.Size(53, 20);
            this.radioButtonStatusBlue.TabIndex = 3;
            this.radioButtonStatusBlue.TabStop = true;
            this.radioButtonStatusBlue.Text = "Blue";
            this.radioButtonStatusBlue.UseVisualStyleBackColor = false;
            this.radioButtonStatusBlue.CheckedChanged += new System.EventHandler(this.radioButtonStatusBlue_CheckedChanged);
            // 
            // radioButtonStatusGreen
            // 
            this.radioButtonStatusGreen.AutoSize = true;
            this.radioButtonStatusGreen.BackColor = System.Drawing.SystemColors.ControlDark;
            this.radioButtonStatusGreen.ForeColor = System.Drawing.Color.Green;
            this.radioButtonStatusGreen.Location = new System.Drawing.Point(210, 21);
            this.radioButtonStatusGreen.Name = "radioButtonStatusGreen";
            this.radioButtonStatusGreen.Size = new System.Drawing.Size(63, 20);
            this.radioButtonStatusGreen.TabIndex = 2;
            this.radioButtonStatusGreen.TabStop = true;
            this.radioButtonStatusGreen.Text = "Green";
            this.radioButtonStatusGreen.UseVisualStyleBackColor = false;
            this.radioButtonStatusGreen.CheckedChanged += new System.EventHandler(this.radioButtonStatusGreen_CheckedChanged);
            // 
            // radioButtonStatusRed
            // 
            this.radioButtonStatusRed.AutoSize = true;
            this.radioButtonStatusRed.BackColor = System.Drawing.SystemColors.ControlDark;
            this.radioButtonStatusRed.ForeColor = System.Drawing.Color.Red;
            this.radioButtonStatusRed.Location = new System.Drawing.Point(6, 47);
            this.radioButtonStatusRed.Name = "radioButtonStatusRed";
            this.radioButtonStatusRed.Size = new System.Drawing.Size(52, 20);
            this.radioButtonStatusRed.TabIndex = 1;
            this.radioButtonStatusRed.TabStop = true;
            this.radioButtonStatusRed.Text = "Red";
            this.radioButtonStatusRed.UseVisualStyleBackColor = false;
            this.radioButtonStatusRed.CheckedChanged += new System.EventHandler(this.radioButtonStatusRed_CheckedChanged);
            // 
            // radioButtonStatusOff
            // 
            this.radioButtonStatusOff.AutoSize = true;
            this.radioButtonStatusOff.BackColor = System.Drawing.SystemColors.ControlDark;
            this.radioButtonStatusOff.Location = new System.Drawing.Point(6, 21);
            this.radioButtonStatusOff.Name = "radioButtonStatusOff";
            this.radioButtonStatusOff.Size = new System.Drawing.Size(42, 20);
            this.radioButtonStatusOff.TabIndex = 0;
            this.radioButtonStatusOff.TabStop = true;
            this.radioButtonStatusOff.Text = "Off";
            this.radioButtonStatusOff.UseVisualStyleBackColor = false;
            this.radioButtonStatusOff.CheckedChanged += new System.EventHandler(this.radioButtonStatusOff_CheckedChanged);
            // 
            // checkBoxFan1On
            // 
            this.checkBoxFan1On.AutoSize = true;
            this.checkBoxFan1On.Location = new System.Drawing.Point(12, 87);
            this.checkBoxFan1On.Name = "checkBoxFan1On";
            this.checkBoxFan1On.Size = new System.Drawing.Size(80, 20);
            this.checkBoxFan1On.TabIndex = 4;
            this.checkBoxFan1On.Text = "Fan 1 On";
            this.checkBoxFan1On.UseVisualStyleBackColor = true;
            this.checkBoxFan1On.CheckedChanged += new System.EventHandler(this.checkBoxFan1On_CheckedChanged);
            // 
            // checkBoxFan2On
            // 
            this.checkBoxFan2On.AutoSize = true;
            this.checkBoxFan2On.Location = new System.Drawing.Point(12, 113);
            this.checkBoxFan2On.Name = "checkBoxFan2On";
            this.checkBoxFan2On.Size = new System.Drawing.Size(80, 20);
            this.checkBoxFan2On.TabIndex = 5;
            this.checkBoxFan2On.Text = "Fan 2 On";
            this.checkBoxFan2On.UseVisualStyleBackColor = true;
            this.checkBoxFan2On.CheckedChanged += new System.EventHandler(this.checkBoxFan2On_CheckedChanged);
            // 
            // checkBoxFlashOn
            // 
            this.checkBoxFlashOn.AutoSize = true;
            this.checkBoxFlashOn.Location = new System.Drawing.Point(12, 139);
            this.checkBoxFlashOn.Name = "checkBoxFlashOn";
            this.checkBoxFlashOn.Size = new System.Drawing.Size(80, 20);
            this.checkBoxFlashOn.TabIndex = 6;
            this.checkBoxFlashOn.Text = "Flash On";
            this.checkBoxFlashOn.UseVisualStyleBackColor = true;
            this.checkBoxFlashOn.CheckedChanged += new System.EventHandler(this.checkBoxFlashOn_CheckedChanged);
            // 
            // buttonReset
            // 
            this.buttonReset.Location = new System.Drawing.Point(10, 57);
            this.buttonReset.Name = "buttonReset";
            this.buttonReset.Size = new System.Drawing.Size(101, 23);
            this.buttonReset.TabIndex = 7;
            this.buttonReset.Text = "Reset";
            this.buttonReset.UseVisualStyleBackColor = true;
            this.buttonReset.Click += new System.EventHandler(this.buttonReset_Click);
            // 
            // buttonClear
            // 
            this.buttonClear.Location = new System.Drawing.Point(345, 165);
            this.buttonClear.Name = "buttonClear";
            this.buttonClear.Size = new System.Drawing.Size(101, 23);
            this.buttonClear.TabIndex = 8;
            this.buttonClear.Text = "Clear";
            this.buttonClear.UseVisualStyleBackColor = true;
            this.buttonClear.Click += new System.EventHandler(this.buttonClear_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 509);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(458, 22);
            this.statusStrip1.TabIndex = 9;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(138, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(70, 16);
            this.label1.TabIndex = 10;
            this.label1.Text = "Serial Port";
            // 
            // FormKioskTestApp
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(458, 531);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.buttonClear);
            this.Controls.Add(this.buttonReset);
            this.Controls.Add(this.checkBoxFlashOn);
            this.Controls.Add(this.checkBoxFan2On);
            this.Controls.Add(this.checkBoxFan1On);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.comboBoxSerialPort);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.textBoxHistory);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MinimizeBox = false;
            this.Name = "FormKioskTestApp";
            this.Text = "Kios Driver Test App";
            this.Load += new System.EventHandler(this.FormKioskTestApp_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxHistory;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.ComboBox comboBoxSerialPort;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton radioButtonStatusOff;
        private System.Windows.Forms.RadioButton radioButtonStatusRed;
        private System.Windows.Forms.RadioButton radioButtonStatusWhite;
        private System.Windows.Forms.RadioButton radioButtonStatusYellow;
        private System.Windows.Forms.RadioButton radioButtonStatusBlue;
        private System.Windows.Forms.RadioButton radioButtonStatusGreen;
        private System.Windows.Forms.RadioButton radioButtonStatusPurple;
        private System.Windows.Forms.RadioButton radioButtonStatusCyan;
        private System.Windows.Forms.CheckBox checkBoxFan1On;
        private System.Windows.Forms.CheckBox checkBoxFan2On;
        private System.Windows.Forms.CheckBox checkBoxFlashOn;
        private System.Windows.Forms.Button buttonReset;
        private System.Windows.Forms.Button buttonClear;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.Label label1;
    }
}

