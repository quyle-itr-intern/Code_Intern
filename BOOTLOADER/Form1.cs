using System;
using System.Data;
using System.Diagnostics;
using System.Diagnostics.Metrics;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Security.Policy;
using System.Text;
using System.Threading;
using static System.Net.Mime.MediaTypeNames;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ProgressBar;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ToolTip;
using System.Xml.Linq;
using System.Windows.Forms;

namespace BOOTLOADER
{
    public partial class Form1 : Form
    {
        SerialPort port = new SerialPort();
        char Data_Receive;
        string pathfilehex = new string("");

        public Form1()
        {
            this.MinimumSize = new Size(600, 680);
            this.MaximumSize = new Size(600, 680);
            InitializeComponent();
        }

        public static byte getHex(byte Copy_uint8_tAsci)
        {
            byte Result = 0;
            /*0 ... 9*/
            if ((Copy_uint8_tAsci >= 48) && (Copy_uint8_tAsci <= 57))
                Result = (byte)((byte)Copy_uint8_tAsci - 48);
            /*A ... F*/
            else if ((Copy_uint8_tAsci >= 65) && (Copy_uint8_tAsci <= 70))
                Result = (byte)((byte)Copy_uint8_tAsci - 48);
            return Result;
        }

        public static uint ReadSizeFlash(string pathfile)
        {
            UInt16 CC;
            UInt32 u32SizeDataFirmware = 0;

            string arr;
            string path = pathfile;
            if (File.Exists(path))
            {
                using (StreamReader reader = new StreamReader(new FileStream(path, FileMode.Open), new UTF8Encoding())) // do anything you want, e.g. read it
                {
                    try
                    {
                        arr = "";
                        while ((arr = reader.ReadLine()) != null)
                        {
                            switch (getHex((byte)arr[8]))
                            {
                                case 4: /* Extended Linear Address Record: used to identify the extended linear address  */
                                    break;
                                case 5: /* Start Linear Address Record: the address where the program starts to run      */
                                    break;
                                case 0: /* Data Rrecord: used to record data, most records of HEX files are data records */
                                    CC = (ushort)((ushort)(getHex((byte)arr[1]) << 4) | getHex((byte)arr[2]));
                                    u32SizeDataFirmware += CC;
                                    break;
                                default:
                                    break;
                            }
                        }
                        reader.Close();
                    }
                    catch (IOException)
                    {

                    }
                }
            }
            else
            {
                string title = "File";
                MessageBox.Show("File not found from path !!!", title);
            }
            return u32SizeDataFirmware;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string[] ports = SerialPort.GetPortNames();
            comPort.Items.AddRange(ports);
            comPort.SelectedIndex = 0;
            btnClose.Enabled = true;

            baudrate.Items.Add("4800");
            baudrate.Items.Add("9600");
            baudrate.Items.Add("14400");
            baudrate.Items.Add("19200");
            baudrate.Items.Add("38400");
            baudrate.Items.Add("56000");
            baudrate.Items.Add("57600");
            baudrate.Items.Add("115200");
            baudrate.SelectedIndex = 7;
        }

        private async void Send_Click(object sender, EventArgs e)
        {
            UInt16 TimerOut = 100;
            UInt16 CountOut = 0;
            if (port.IsOpen)
            {
                btnSend.Enabled = false;
                if (txtSend.Lines.Length > 0)
                {
                    TextReader read = new System.IO.StringReader(txtSend.Text);
                    int rows = txtSend.Lines.Length;

                    UInt32 sizeCode = ReadSizeFlash(pathfilehex);

                    CountOut = 0;
                    Data_Receive = ' ';
                    while (Data_Receive != 'R')
                    {
                        port.Write("U");
                        await Task.Delay(500);
                        CountOut++;
                        if (CountOut == 5)
                        {
                            string title = "Load Flash";
                            MessageBox.Show("STM32 Bootloader Not Response Update !!! ", title);
                            goto end;
                        }
                    }

                    CountOut = 0;
                    Data_Receive = ' ';
                    port.Write("S" + sizeCode + "\n");
                    txtReceive.Clear();

                    txtReceive.Text += "Size firmware update: " + sizeCode + " Bytes\r\n";
                    txtReceive.Text += "Check size flash memory update firmware\r\n";
                    while (Data_Receive != 'R')
                    {
                        Thread.Sleep(1);
                        CountOut++;
                        if ((CountOut == 3000))
                        {
                            string title = "Load Flash";
                            MessageBox.Show("STM32 Bootloader Not Response Size !!! " + Data_Receive, title);
                            goto end;
                        }
                    }
                    txtReceive.Text += "Size flash memory ok for update firmware\r\n";

                    Data_Receive = ' ';
                    CountOut = 0;
                    while (Data_Receive != 'R')
                    {
                        port.Write("X");
                        await Task.Delay(1000);
                        CountOut++;
                        if( CountOut == 5)
                        {
                            MessageBox.Show("STM32 Bootloader Start Fail !!! ", "Load Flash");
                            goto end;
                        }    
                    }

                    progressFlash.Maximum = rows;
                    progressFlash.Step = 1;
                    progressFlash.Value = 0;
                    string data_line;
                    for (int r = 1; r < rows; r++)
                    {
                        progressFlash.PerformStep();
                        Data_Receive = ' ';
                        data_line = read.ReadLine();
                        port.Write(data_line);
                        //port.Write("Y");

                        TimerOut = 2000;
                        while ((Data_Receive != 'R') && (TimerOut > 0))
                        {
                            await Task.Delay(1);
                            TimerOut--;
                            if (Data_Receive == 'E')
                            {
                                Data_Receive = ' ';
                                TimerOut = 2000;
                                port.Write(data_line);
                            }
                        }
                        if (TimerOut == 0)
                        {
                            string title = "Load Flash";
                            MessageBox.Show("STM32 Bootloader Not Response in Process !!! " + pathfilehex, title);
                            goto end;
                        }
                        Data_Receive = ' ';
                    }
                    CountOut = 0;
                    Data_Receive = ' ';
                    port.Write("Z");
                    while (Data_Receive != 'R')
                    {
                        await Task.Delay(10);
                        CountOut++;
                        if (CountOut == 100)
                        {
                            MessageBox.Show("STM32 Bootloader Not Response End !!! ", "Load Flash");
                        }
                    }
                    progressFlash.PerformStep();
                    await Task.Delay(550);
                    txtReceive.Text += "Flash firmware to memory successfully !!!\r\n";
                    MessageBox.Show("STM32 Bootloader Success !!! ", "Load Flash");
                }
            end:;
                btnSend.Enabled = true;
            }
            else
            {
                string title = "COM";
                MessageBox.Show("Please select COMx port to flash !!!", title);
            }
        }
        private void btnOpen_Click(object sender, EventArgs e)
        {
            btnOpen.Enabled = false;
            btnClose.Enabled = true;
            try
            {
                port.BaudRate = int.Parse(baudrate.Items[baudrate.SelectedIndex].ToString());
                port.DataBits = 8;
                port.StopBits = StopBits.One;
                port.Parity = Parity.None;
                port.PortName = comPort.Items[comPort.SelectedIndex].ToString();
                port.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
                port.Open();
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                btnClose.Enabled = false;
                btnOpen.Enabled = true;
            }
        }

        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            string temp = sp.ReadExisting();
            if (temp == null)
                return;
            Data_Receive = (char)temp[0];
            Debug.Print("Data Received:");
            Debug.Print(Data_Receive.ToString());
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            if (port.IsOpen)
            {
                port.Close();
                btnOpen.Enabled = true;
                btnClose.Enabled = false;
            }
        }

        private void fileFlash_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Filter = "Text files | *.hex"; // file types, that will be allowed to upload
            dialog.Multiselect = false; // allow/deny user to upload more than one file at a time
            if (dialog.ShowDialog() == DialogResult.OK) // if user clicked OK
            {
                String path = dialog.FileName; // get name of file
                pathfilehex = path;
                using (StreamReader reader = new StreamReader(new FileStream(path, FileMode.Open), new UTF8Encoding())) // do anything you want, e.g. read it
                {
                    try
                    {
                        string text = reader.ReadToEnd();
                        txtSend.Text = text;
                        pathfile.Text = path;
                        reader.Close();
                    }
                    catch (IOException)
                    {

                    }
                }
            }
        }
        private void btnRefresh_Click(object sender, EventArgs e)
        {
            comPort.Items.Clear();
            string[] ports = SerialPort.GetPortNames();
            comPort.Items.AddRange(ports);
        }
    }
}