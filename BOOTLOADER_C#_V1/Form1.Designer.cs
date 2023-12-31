﻿namespace BOOTLOADER
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
            ((System.ComponentModel.ISupportInitialize)bindingSource1).BeginInit();
            ((System.ComponentModel.ISupportInitialize)bindingSource2).BeginInit();
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
            txtConsole.Location = new Point(586, 57);
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
            btnClearConsole.Location = new Point(916, 533);
            btnClearConsole.Name = "btnClearConsole";
            btnClearConsole.Size = new Size(92, 29);
            btnClearConsole.TabIndex = 16;
            btnClearConsole.Text = "Clear";
            btnClearConsole.UseVisualStyleBackColor = true;
            btnClearConsole.Click += btnClearConsole_Click;
            // 
            // txtChoose
            // 
            txtChoose.Location = new Point(586, 568);
            txtChoose.Name = "txtChoose";
            txtChoose.Size = new Size(422, 23);
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
            label3.Location = new Point(586, 547);
            label3.Name = "label3";
            label3.Size = new Size(137, 15);
            label3.TabIndex = 19;
            label3.Text = "Write option you choose";
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1036, 639);
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
    }
}