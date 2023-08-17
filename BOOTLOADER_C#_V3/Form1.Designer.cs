namespace BOOTLOADER
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            label1 = new Label();
            comPort = new ComboBox();
            btnOpen = new Button();
            btnClose = new Button();
            txtSend = new TextBox();
            btnSend = new Button();
            bindingSource1 = new BindingSource(components);
            bindingSource2 = new BindingSource(components);
            baudrate = new ComboBox();
            fileFlash = new Button();
            pathfile = new TextBox();
            txtReceive = new TextBox();
            backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            btnRefresh = new Button();
            progressFlash = new ProgressBar();
            txtConsole = new TextBox();
            label2 = new Label();
            btnClearConsole = new Button();
            txtChoose = new TextBox();
            btnSendChoose = new Button();
            label3 = new Label();
            label4 = new Label();
            btnConnectDevice = new Button();
            dataGridView = new DataGridView();
            txtCRC = new TextBox();
            btnCRC = new Button();
            txtAddress = new TextBox();
            label5 = new Label();
            btnRead = new Button();
            txtNumber = new TextBox();
            label6 = new Label();
            label7 = new Label();
            txtEraseMemory = new TextBox();
            btnErase = new Button();
            btnJump = new Button();
            label8 = new Label();
            txtAddressWrite = new TextBox();
            label9 = new Label();
            txtNumberWrite = new TextBox();
            label10 = new Label();
            txtData1 = new TextBox();
            txtData2 = new TextBox();
            txtData3 = new TextBox();
            txtData4 = new TextBox();
            label11 = new Label();
            label12 = new Label();
            label13 = new Label();
            label14 = new Label();
            btnWrite = new Button();
            btnClearConsleFlash = new Button();
            ((System.ComponentModel.ISupportInitialize)bindingSource1).BeginInit();
            ((System.ComponentModel.ISupportInitialize)bindingSource2).BeginInit();
            ((System.ComponentModel.ISupportInitialize)dataGridView).BeginInit();
            SuspendLayout();
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(30, 31);
            label1.Name = "label1";
            label1.Size = new Size(35, 15);
            label1.TabIndex = 0;
            label1.Text = "PORT";
            // 
            // comPort
            // 
            comPort.FormattingEnabled = true;
            comPort.Location = new Point(71, 26);
            comPort.Name = "comPort";
            comPort.Size = new Size(67, 23);
            comPort.TabIndex = 1;
            // 
            // btnOpen
            // 
            btnOpen.Location = new Point(297, 24);
            btnOpen.Name = "btnOpen";
            btnOpen.Size = new Size(66, 28);
            btnOpen.TabIndex = 2;
            btnOpen.Text = "Open";
            btnOpen.UseVisualStyleBackColor = true;
            btnOpen.Click += btnOpen_Click;
            // 
            // btnClose
            // 
            btnClose.Location = new Point(369, 23);
            btnClose.Name = "btnClose";
            btnClose.Size = new Size(58, 30);
            btnClose.TabIndex = 3;
            btnClose.Text = "Close";
            btnClose.UseVisualStyleBackColor = true;
            btnClose.Click += btnClose_Click;
            // 
            // txtSend
            // 
            txtSend.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            txtSend.Location = new Point(30, 94);
            txtSend.Multiline = true;
            txtSend.Name = "txtSend";
            txtSend.ScrollBars = ScrollBars.Vertical;
            txtSend.Size = new Size(540, 328);
            txtSend.TabIndex = 4;
            // 
            // btnSend
            // 
            btnSend.Location = new Point(111, 428);
            btnSend.Name = "btnSend";
            btnSend.Size = new Size(89, 30);
            btnSend.TabIndex = 6;
            btnSend.Text = "Flash";
            btnSend.UseVisualStyleBackColor = true;
            btnSend.Click += Send_Click;
            // 
            // baudrate
            // 
            baudrate.FormattingEnabled = true;
            baudrate.Location = new Point(144, 26);
            baudrate.Name = "baudrate";
            baudrate.Size = new Size(68, 23);
            baudrate.TabIndex = 8;
            // 
            // fileFlash
            // 
            fileFlash.Location = new Point(30, 428);
            fileFlash.Name = "fileFlash";
            fileFlash.Size = new Size(75, 30);
            fileFlash.TabIndex = 9;
            fileFlash.Text = "Load";
            fileFlash.UseVisualStyleBackColor = true;
            fileFlash.Click += fileFlash_Click;
            // 
            // pathfile
            // 
            pathfile.Location = new Point(30, 57);
            pathfile.Name = "pathfile";
            pathfile.Size = new Size(540, 23);
            pathfile.TabIndex = 10;
            // 
            // txtReceive
            // 
            txtReceive.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            txtReceive.Location = new Point(30, 492);
            txtReceive.Multiline = true;
            txtReceive.Name = "txtReceive";
            txtReceive.ScrollBars = ScrollBars.Vertical;
            txtReceive.Size = new Size(540, 135);
            txtReceive.TabIndex = 11;
            // 
            // btnRefresh
            // 
            btnRefresh.Location = new Point(218, 23);
            btnRefresh.Name = "btnRefresh";
            btnRefresh.Size = new Size(73, 30);
            btnRefresh.TabIndex = 12;
            btnRefresh.Text = "Refresh";
            btnRefresh.UseVisualStyleBackColor = true;
            btnRefresh.Click += btnRefresh_Click;
            // 
            // progressFlash
            // 
            progressFlash.Location = new Point(30, 464);
            progressFlash.Name = "progressFlash";
            progressFlash.Size = new Size(540, 22);
            progressFlash.TabIndex = 13;
            // 
            // txtConsole
            // 
            txtConsole.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            txtConsole.Location = new Point(586, 86);
            txtConsole.Multiline = true;
            txtConsole.Name = "txtConsole";
            txtConsole.ScrollBars = ScrollBars.Vertical;
            txtConsole.Size = new Size(422, 470);
            txtConsole.TabIndex = 14;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            label2.Location = new Point(683, 21);
            label2.Name = "label2";
            label2.Size = new Size(234, 25);
            label2.TabIndex = 15;
            label2.Text = "Console Update Firmware ";
            // 
            // btnClearConsole
            // 
            btnClearConsole.Location = new Point(916, 562);
            btnClearConsole.Name = "btnClearConsole";
            btnClearConsole.Size = new Size(92, 29);
            btnClearConsole.TabIndex = 16;
            btnClearConsole.Text = "Clear";
            btnClearConsole.UseVisualStyleBackColor = true;
            btnClearConsole.Click += btnClearConsole_Click;
            // 
            // txtChoose
            // 
            txtChoose.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            txtChoose.Location = new Point(586, 594);
            txtChoose.Name = "txtChoose";
            txtChoose.Size = new Size(316, 33);
            txtChoose.TabIndex = 17;
            // 
            // btnSendChoose
            // 
            btnSendChoose.Location = new Point(916, 598);
            btnSendChoose.Name = "btnSendChoose";
            btnSendChoose.Size = new Size(92, 29);
            btnSendChoose.TabIndex = 18;
            btnSendChoose.Text = "Send";
            btnSendChoose.UseVisualStyleBackColor = true;
            btnSendChoose.Click += btnSendChoose_Click;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(586, 569);
            label3.Name = "label3";
            label3.Size = new Size(137, 15);
            label3.TabIndex = 19;
            label3.Text = "Write option you choose";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(707, 60);
            label4.Name = "label4";
            label4.Size = new Size(106, 15);
            label4.TabIndex = 20;
            label4.Text = "Connect to device:";
            // 
            // btnConnectDevice
            // 
            btnConnectDevice.Location = new Point(816, 56);
            btnConnectDevice.Name = "btnConnectDevice";
            btnConnectDevice.Size = new Size(75, 23);
            btnConnectDevice.TabIndex = 21;
            btnConnectDevice.Text = "Connect";
            btnConnectDevice.UseVisualStyleBackColor = true;
            btnConnectDevice.Click += btnConnectDevice_Click;
            // 
            // dataGridView
            // 
            dataGridView.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dataGridView.Location = new Point(1014, 86);
            dataGridView.Name = "dataGridView";
            dataGridView.RowTemplate.Height = 25;
            dataGridView.Size = new Size(460, 192);
            dataGridView.TabIndex = 22;
            // 
            // txtCRC
            // 
            txtCRC.Location = new Point(1024, 605);
            txtCRC.Name = "txtCRC";
            txtCRC.Size = new Size(303, 23);
            txtCRC.TabIndex = 23;
            // 
            // btnCRC
            // 
            btnCRC.Location = new Point(1024, 569);
            btnCRC.Name = "btnCRC";
            btnCRC.Size = new Size(87, 30);
            btnCRC.TabIndex = 24;
            btnCRC.Text = "CRC";
            btnCRC.UseVisualStyleBackColor = true;
            btnCRC.Click += btnCRC_Click;
            // 
            // txtAddress
            // 
            txtAddress.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            txtAddress.Location = new Point(1024, 331);
            txtAddress.Name = "txtAddress";
            txtAddress.Size = new Size(175, 33);
            txtAddress.TabIndex = 25;
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            label5.Location = new Point(1024, 303);
            label5.Name = "label5";
            label5.Size = new Size(169, 25);
            label5.TabIndex = 26;
            label5.Text = "Address Data Read";
            // 
            // btnRead
            // 
            btnRead.Location = new Point(1309, 334);
            btnRead.Name = "btnRead";
            btnRead.Size = new Size(106, 30);
            btnRead.TabIndex = 27;
            btnRead.Text = "Read";
            btnRead.UseVisualStyleBackColor = true;
            btnRead.Click += btnRead_Click;
            // 
            // txtNumber
            // 
            txtNumber.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            txtNumber.Location = new Point(1205, 331);
            txtNumber.Name = "txtNumber";
            txtNumber.Size = new Size(98, 33);
            txtNumber.TabIndex = 28;
            // 
            // label6
            // 
            label6.AutoSize = true;
            label6.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            label6.Location = new Point(1205, 303);
            label6.Name = "label6";
            label6.Size = new Size(81, 25);
            label6.TabIndex = 29;
            label6.Text = "Number";
            // 
            // label7
            // 
            label7.AutoSize = true;
            label7.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            label7.Location = new Point(1024, 447);
            label7.Name = "label7";
            label7.Size = new Size(132, 25);
            label7.TabIndex = 30;
            label7.Text = "Erase Memory";
            // 
            // txtEraseMemory
            // 
            txtEraseMemory.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            txtEraseMemory.Location = new Point(1024, 479);
            txtEraseMemory.Name = "txtEraseMemory";
            txtEraseMemory.Size = new Size(175, 33);
            txtEraseMemory.TabIndex = 31;
            // 
            // btnErase
            // 
            btnErase.Location = new Point(1205, 479);
            btnErase.Name = "btnErase";
            btnErase.Size = new Size(67, 30);
            btnErase.TabIndex = 32;
            btnErase.Text = "Erase";
            btnErase.UseVisualStyleBackColor = true;
            btnErase.Click += btnErase_Click;
            // 
            // btnJump
            // 
            btnJump.Location = new Point(1024, 518);
            btnJump.Name = "btnJump";
            btnJump.Size = new Size(175, 44);
            btnJump.TabIndex = 33;
            btnJump.Text = "Jump to application";
            btnJump.UseVisualStyleBackColor = true;
            btnJump.Click += btnJump_Click;
            // 
            // label8
            // 
            label8.AutoSize = true;
            label8.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            label8.Location = new Point(1024, 374);
            label8.Name = "label8";
            label8.Size = new Size(174, 25);
            label8.TabIndex = 34;
            label8.Text = "Address Data Write";
            // 
            // txtAddressWrite
            // 
            txtAddressWrite.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            txtAddressWrite.Location = new Point(1024, 406);
            txtAddressWrite.Name = "txtAddressWrite";
            txtAddressWrite.Size = new Size(175, 33);
            txtAddressWrite.TabIndex = 35;
            // 
            // label9
            // 
            label9.AutoSize = true;
            label9.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            label9.Location = new Point(1203, 374);
            label9.Name = "label9";
            label9.Size = new Size(81, 25);
            label9.TabIndex = 36;
            label9.Text = "Number";
            // 
            // txtNumberWrite
            // 
            txtNumberWrite.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            txtNumberWrite.Location = new Point(1205, 406);
            txtNumberWrite.Name = "txtNumberWrite";
            txtNumberWrite.Size = new Size(98, 33);
            txtNumberWrite.TabIndex = 37;
            // 
            // label10
            // 
            label10.AutoSize = true;
            label10.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            label10.Location = new Point(1365, 374);
            label10.Name = "label10";
            label10.Size = new Size(51, 25);
            label10.TabIndex = 38;
            label10.Text = "Data";
            // 
            // txtData1
            // 
            txtData1.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            txtData1.Location = new Point(1342, 402);
            txtData1.Name = "txtData1";
            txtData1.Size = new Size(132, 33);
            txtData1.TabIndex = 39;
            // 
            // txtData2
            // 
            txtData2.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            txtData2.Location = new Point(1342, 447);
            txtData2.Name = "txtData2";
            txtData2.Size = new Size(132, 33);
            txtData2.TabIndex = 40;
            // 
            // txtData3
            // 
            txtData3.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            txtData3.Location = new Point(1342, 492);
            txtData3.Name = "txtData3";
            txtData3.Size = new Size(132, 33);
            txtData3.TabIndex = 41;
            // 
            // txtData4
            // 
            txtData4.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            txtData4.Location = new Point(1342, 531);
            txtData4.Name = "txtData4";
            txtData4.Size = new Size(132, 33);
            txtData4.TabIndex = 42;
            // 
            // label11
            // 
            label11.AutoSize = true;
            label11.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            label11.Location = new Point(1318, 407);
            label11.Name = "label11";
            label11.Size = new Size(22, 25);
            label11.TabIndex = 44;
            label11.Text = "1";
            // 
            // label12
            // 
            label12.AutoSize = true;
            label12.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            label12.Location = new Point(1317, 452);
            label12.Name = "label12";
            label12.Size = new Size(22, 25);
            label12.TabIndex = 45;
            label12.Text = "2";
            // 
            // label13
            // 
            label13.AutoSize = true;
            label13.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            label13.Location = new Point(1317, 497);
            label13.Name = "label13";
            label13.Size = new Size(22, 25);
            label13.TabIndex = 46;
            label13.Text = "3";
            // 
            // label14
            // 
            label14.AutoSize = true;
            label14.Font = new Font("Segoe UI", 14.25F, FontStyle.Regular, GraphicsUnit.Point);
            label14.Location = new Point(1317, 535);
            label14.Name = "label14";
            label14.Size = new Size(22, 25);
            label14.TabIndex = 47;
            label14.Text = "4";
            // 
            // btnWrite
            // 
            btnWrite.Location = new Point(1365, 570);
            btnWrite.Name = "btnWrite";
            btnWrite.RightToLeft = RightToLeft.No;
            btnWrite.Size = new Size(106, 30);
            btnWrite.TabIndex = 48;
            btnWrite.Text = "Write";
            btnWrite.UseVisualStyleBackColor = true;
            btnWrite.Click += btnWrite_Click;
            // 
            // btnClearConsleFlash
            // 
            btnClearConsleFlash.Location = new Point(471, 428);
            btnClearConsleFlash.Name = "btnClearConsleFlash";
            btnClearConsleFlash.Size = new Size(85, 30);
            btnClearConsleFlash.TabIndex = 49;
            btnClearConsleFlash.Text = "Clear";
            btnClearConsleFlash.UseVisualStyleBackColor = true;
            btnClearConsleFlash.Click += btnClearConsleFlash_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1503, 639);
            Controls.Add(btnClearConsleFlash);
            Controls.Add(btnWrite);
            Controls.Add(label14);
            Controls.Add(label13);
            Controls.Add(label12);
            Controls.Add(label11);
            Controls.Add(txtData4);
            Controls.Add(txtData3);
            Controls.Add(txtData2);
            Controls.Add(txtData1);
            Controls.Add(label10);
            Controls.Add(txtNumberWrite);
            Controls.Add(label9);
            Controls.Add(txtAddressWrite);
            Controls.Add(label8);
            Controls.Add(btnJump);
            Controls.Add(btnErase);
            Controls.Add(txtEraseMemory);
            Controls.Add(label7);
            Controls.Add(label6);
            Controls.Add(txtNumber);
            Controls.Add(btnRead);
            Controls.Add(label5);
            Controls.Add(txtAddress);
            Controls.Add(btnCRC);
            Controls.Add(txtCRC);
            Controls.Add(dataGridView);
            Controls.Add(btnConnectDevice);
            Controls.Add(label4);
            Controls.Add(label3);
            Controls.Add(btnSendChoose);
            Controls.Add(txtChoose);
            Controls.Add(btnClearConsole);
            Controls.Add(label2);
            Controls.Add(txtConsole);
            Controls.Add(progressFlash);
            Controls.Add(btnRefresh);
            Controls.Add(txtReceive);
            Controls.Add(pathfile);
            Controls.Add(fileFlash);
            Controls.Add(baudrate);
            Controls.Add(btnSend);
            Controls.Add(txtSend);
            Controls.Add(btnClose);
            Controls.Add(btnOpen);
            Controls.Add(comPort);
            Controls.Add(label1);
            Icon = (Icon)resources.GetObject("$this.Icon");
            Name = "Form1";
            Text = "Update Firmware Stm32";
            Load += Form1_Load;
            ((System.ComponentModel.ISupportInitialize)bindingSource1).EndInit();
            ((System.ComponentModel.ISupportInitialize)bindingSource2).EndInit();
            ((System.ComponentModel.ISupportInitialize)dataGridView).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Label label1;
        private ComboBox comPort;
        private Button btnOpen;
        private Button btnClose;
        private TextBox txtSend;
        private Button btnSend;
        private BindingSource bindingSource1;
        private BindingSource bindingSource2;
        private ComboBox baudrate;
        private Button fileFlash;
        private TextBox pathfile;
        private TextBox txtReceive;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private Button btnRefresh;
        private ProgressBar progressFlash;
        private TextBox txtConsole;
        private Label label2;
        private Button btnClearConsole;
        private TextBox txtChoose;
        private Button btnSendChoose;
        private Label label3;
        private Label label4;
        private Button btnConnectDevice;
        private DataGridView dataGridView;
        private TextBox txtCRC;
        private Button btnCRC;
        private TextBox txtAddress;
        private Label label5;
        private Button btnRead;
        private TextBox txtNumber;
        private Label label6;
        private Label label7;
        private TextBox txtEraseMemory;
        private Button btnErase;
        private Button btnJump;
        private Label label8;
        private TextBox txtAddressWrite;
        private Label label9;
        private TextBox txtNumberWrite;
        private Label label10;
        private TextBox txtData1;
        private TextBox txtData2;
        private TextBox txtData3;
        private TextBox txtData4;
        private Label label11;
        private Label label12;
        private Label label13;
        private Label label14;
        private Button btnWrite;
        private Button btnClearConsleFlash;
    }
}