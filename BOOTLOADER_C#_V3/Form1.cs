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

        /* Table of CRC values for high–order byte */
        byte[] auch_crc_high = new byte[]
        {
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
            0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
            0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
            0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
            0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
            0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
        };

        /* Table of CRC values for low–order byte */
        byte[] auch_crc_low = new byte[]
        {
            0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA,
            0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15,
            0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35,
            0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA,
            0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
            0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
            0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7,
            0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
            0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E,
            0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40
       };
        byte idle = 0;
        byte Erase = 0;
        static byte Reader = 0;
        static byte Writer = 0;
        byte choose = 0;
        SerialPort port = new SerialPort();
        char Data_Receive;
        string pathfilehex = new string("");
        string RxString;
        byte flag_update = 0;
        DataTable dt = new DataTable();

        String START = "55";
        String UPDATE = "04";
        UInt16 delay_ms = 1;

        public Form1()
        {
            this.MinimumSize = new Size(1500, 680);
            this.MaximumSize = new Size(1500, 680);
            InitializeComponent();

            dt.Columns.Add("Address", typeof(string));
            dt.Columns.Add("0", typeof(string));
            dt.Columns.Add("4", typeof(string));
            dt.Columns.Add("8", typeof(string));
            dt.Columns.Add("C", typeof(string));

            dt.Rows.Add(new object[] { "0x08008000", "", "", "", "" });
            dt.Rows.Add(new object[] { "0x08008010", "", "", "", "" });
            dt.Rows.Add(new object[] { "0x08008020", "", "", "", "" });
            dt.Rows.Add(new object[] { "0x08008030", "", "", "", "" });
            dt.Rows.Add(new object[] { "0x08008040", "", "", "", "" });

            dataGridView.DataSource = dt;

            dataGridView.Columns[0].MinimumWidth = 92;
            dataGridView.Columns[1].MinimumWidth = 92;
            dataGridView.Columns[2].MinimumWidth = 92;
            dataGridView.Columns[3].MinimumWidth = 92;
            dataGridView.Columns[4].MinimumWidth = 92;

            dataGridView.Columns[0].SortMode = DataGridViewColumnSortMode.NotSortable;
            dataGridView.Columns[1].SortMode = DataGridViewColumnSortMode.NotSortable;
            dataGridView.Columns[2].SortMode = DataGridViewColumnSortMode.NotSortable;
            dataGridView.Columns[3].SortMode = DataGridViewColumnSortMode.NotSortable;

            dataGridView.AutoGenerateColumns = false;

            dataGridView.RowHeadersVisible = false;

            dataGridView.Rows[1].HeaderCell.Value = "0x0800800";

            dataGridView.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            dataGridView.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells; // Adjust column sizes automatically
            dataGridView.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCells; // Adjust row heights automatically
        }
        public UInt16 crc(String buffer, UInt16 buffer_length)
        {
            byte crc_high = 0xFF; /* high byte of CRC initialized */
            byte crc_low = 0xFF; /* low byte of CRC initialized */
            UInt16 index;           /* will index into CRC lookup table */
            UInt16 index_buffer = 0;

            while (buffer_length-- != 0) /* pass through message buffer */
            {
                index = (ushort)(crc_low ^ buffer[index_buffer]); /* calculate the CRC */
                index_buffer++;
                crc_low = (byte)(crc_high ^ auch_crc_high[index]);
                crc_high = auch_crc_low[index];
            }
            return (ushort)((crc_high << 8) | (crc_low));
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
            UInt16 CountOut = 0;
            UInt16 TIME_OUT = 200;
            if (port.IsOpen)
            {
                btnSend.Enabled = false;
                if (txtSend.Lines.Length > 0)
                {
                    TextReader read = new System.IO.StringReader(txtSend.Text);
                    int rows = txtSend.Lines.Length;
                    UInt32 sizeCode = ReadSizeFlash(pathfilehex);
                    flag_update = 1;

                    port.Write("U");
                    await Task.Delay(500);

                    string tx = START + UPDATE + "Update";
                    string crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + crc_string);

                    CountOut = 0;
                    Data_Receive = ' ';
                    while (Data_Receive != 'R')
                    {
                        await Task.Delay(delay_ms);
                        CountOut++;
                        if (CountOut == TIME_OUT)
                        {
                            port.Write("C");
                            string title = "Load Flash";
                            MessageBox.Show("STM32 Bootloader Not Response Update !!! ", title);
                            goto end;
                        }
                    }

                    tx = START + UPDATE + "Size" + sizeCode + ":";
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + crc_string);

                    txtReceive.Clear();
                    txtReceive.Text = "Size firmware update: " + sizeCode + " Bytes\r\n";
                    txtReceive.Text += "Check size flash memory update firmware\r\n";

                    CountOut = 0;
                    Data_Receive = ' ';
                    while (Data_Receive != 'R')
                    {
                        await Task.Delay(delay_ms);
                        CountOut++;
                        if (CountOut == TIME_OUT)
                        {
                            port.Write("C");
                            string title = "Load Flash";
                            MessageBox.Show("STM32 Bootloader Not Response Size !!! ", title);
                            goto end;
                        }
                        if (Data_Receive == 'N')
                        {
                            port.Write("C");
                            string title = "Load Flash";
                            MessageBox.Show("STM32 Bootloader Not Response Over Size !!! ", title);
                            goto end;
                        }
                    }
                    txtReceive.Text += "Size flash memory ok for update firmware\r\n";

                    tx = START + UPDATE + "Begin";
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + crc_string);

                    Data_Receive = ' ';
                    CountOut = 0;
                    while (Data_Receive != 'R')
                    {
                        await Task.Delay(delay_ms);
                        CountOut++;
                        if (CountOut == TIME_OUT)
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
                        data_line = read.ReadLine();

                        tx = START + UPDATE + data_line;
                        UInt16 check = crc(tx, (ushort)tx.Length);
                        if (check < 0x00FF)
                        {
                            crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                            port.Write(tx + "00" + crc_string);
                        }
                        else if (check < 0x0FFF)
                        {
                            crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                            port.Write(tx + "0" + crc_string);
                        }
                        else
                        {
                            crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                            port.Write(tx + crc_string);
                        }

                        Data_Receive = ' ';
                        CountOut = 0;
                        while (Data_Receive != 'R')
                        {
                            await Task.Delay(delay_ms);
                            CountOut++;
                            if (Data_Receive == 'E')
                            {
                                Data_Receive = ' ';
                                CountOut = 0;
                                tx = START + UPDATE + data_line;

                                check = crc(tx, (ushort)tx.Length);
                                if (check < 0x0FFF)
                                {
                                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                                    port.Write(tx + "0" + crc_string);
                                }
                                else
                                {
                                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                                    port.Write(tx + crc_string);
                                }
                            }
                            if (Data_Receive == 'C')
                            {
                                progressFlash.Value = 0;
                                MessageBox.Show("STM32 Bootloader Cancel Update Firmware !!! ", "Load Flash");
                                goto end;
                            }
                            if (CountOut == TIME_OUT)
                            {
                                progressFlash.Value = 0;
                                string title = "Load Flash";
                                MessageBox.Show("STM32 Bootloader Not Response in Process !!! ", title);
                                goto end;
                            }
                        }
                    }

                    tx = START + UPDATE + "End";
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + crc_string);

                    CountOut = 0;
                    Data_Receive = ' ';
                    while (Data_Receive != 'R')
                    {
                        await Task.Delay(delay_ms);
                        CountOut++;
                        if (CountOut == TIME_OUT)
                        {
                            MessageBox.Show("STM32 Bootloader Not Response End !!! ", "Load Flash");
                        }
                    }
                    progressFlash.PerformStep();
                    Thread.Sleep(500);

                    tx = START + "03" + "08008000";
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + crc_string);
                    progressFlash.Value = 0;

                    txtReceive.Text += "Flash firmware to memory successfully !!!\r\n";
                    MessageBox.Show("STM32 Bootloader Success !!! ", "Load Flash");
                end:;
                    btnSend.Enabled = true;
                    flag_update = 0;
                }
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
            RxString = sp.ReadExisting();
            if (RxString == null)
                return;
            if (flag_update == 1)
            {
                Data_Receive = (char)RxString[0];
            }
            Debug.Print("Data Received:");
            Debug.Print(RxString);
            if (flag_update != 1)
            {
                this.Invoke(new EventHandler(showConsoleText));
            }
        }
        private void showConsoleText(object sender, EventArgs e)
        {
            txtConsole.AppendText(RxString);
        }
        private void btnClose_Click(object sender, EventArgs e)
        {
            if (port.IsOpen)
            {
                port.Close();
                btnOpen.Enabled = true;
                btnClose.Enabled = false;
            }
            else
            {
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
        private void btnClearConsole_Click(object sender, EventArgs e)
        {
            txtConsole.Clear();
        }
        private void btnSendChoose_Click(object sender, EventArgs e)
        {
            if (port.IsOpen && (btnConnectDevice.Text == "Disconnect"))
            {
                if (idle == 0)
                {
                    if (txtChoose.Text == null)
                        return;
                    choose = (byte)int.Parse(txtChoose.Text);
                }
                btnSendChoose.Enabled = false;

                switch (choose)
                {
                    case 1:
                        {
                            if (Reader == 0)
                            {
                                idle = 1;
                                txtConsole.AppendText("Address Read: ");
                                Reader++;
                            }
                            else
                            {
                                txtConsole.AppendText(txtChoose.Text + "\r\n");
                                port.Write("F" + choose + txtChoose.Text);
                                Reader = 0;
                                idle = 0;
                            }
                            break;
                        }
                    case 2:
                        {
                            if (Writer == 0)
                            {
                                idle = 1;
                                txtConsole.AppendText("Address Write: ");
                                Writer++;
                            }
                            else
                            {
                                txtConsole.AppendText(txtChoose.Text + "\r\n");
                                port.Write("F" + choose + txtChoose.Text);
                                Writer = 0;
                                idle = 0;
                            }
                            break;
                        }
                    case 3:
                        {
                            port.Write("F" + txtChoose.Text);
                            btnConnectDevice.Text = "Connect";
                            break;
                        }
                    case 4:
                        {
                            port.Write("F" + txtChoose.Text);
                            break;
                        }
                    case 5:
                        {
                            port.Write("F" + txtChoose.Text);
                            break;
                        }
                    case 6:
                        {
                            if (Erase == 0)
                            {
                                idle = 1;
                                txtConsole.AppendText("Erase sector: ");
                                Erase++;
                            }
                            else
                            {
                                txtConsole.AppendText(txtChoose.Text + "\r\n");
                                port.Write("F" + choose + txtChoose.Text);
                                Erase = 0;
                                idle = 0;
                            }
                            break;
                        }
                    default:
                        {
                            MessageBox.Show("No valid number !!!", "Communication");
                            choose = 0;
                            break;
                        }
                }
                btnSendChoose.Enabled = true;
                txtChoose.Clear();
            }
            else
            {
                if (btnConnectDevice.Text != "Connect")
                    MessageBox.Show("Please connect COMx port to write !!!", "COM");
                else
                    MessageBox.Show("Please connect to device !!!", "Communication");
            }
        }
        private async void btnConnectDevice_Click(object sender, EventArgs e)
        {
            if (port.IsOpen)
            {
                if (btnConnectDevice.Text == "Connect")
                {
                    btnConnectDevice.Text = "Disconnect";
                    port.Write("C");
                    await Task.Delay(100);
                    port.Write("U");
                }
                else
                {
                    btnConnectDevice.Text = "Connect";
                }
            }
            else
            {
                MessageBox.Show("Please connect COMx port to write !!!", "COM");
            }
        }
        private void btnCRC_Click(object sender, EventArgs e)
        {
            string crc_string = txtCRC.Text;
            txtConsole.Text = crc(crc_string, (ushort)crc_string.Length).ToString("X");
        }
        private void btnRead_Click(object sender, EventArgs e)
        {
            if (port.IsOpen)
            {
                string tx = START + "00" + txtAddress.Text + txtNumber.Text;
                string crc_string;
                UInt16 check = crc(tx, (ushort)tx.Length);
                if (check < 0x00FF)
                {
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + "00" + crc_string);
                }
                else if (check < 0x0FFF)
                {
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + "0" + crc_string);
                }
                else
                {
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + crc_string);
                }
            }
            else
            {
                MessageBox.Show("Please select COMx port to flash !!!", "COM");
            }
        }
        private void btnErase_Click(object sender, EventArgs e)
        {
            if (port.IsOpen)
            {
                string tx = START + "02" + txtEraseMemory.Text;
                string crc_string;
                UInt16 check = crc(tx, (ushort)tx.Length);
                if (check < 0x00FF)
                {
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + "00" + crc_string);
                }
                else if (check < 0x0FFF)
                {
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + "0" + crc_string);
                }
                else
                {
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + crc_string);
                }
            }
            else
            {
                MessageBox.Show("Please select COMx port to flash !!!", "COM");
            }
        }
        private void btnJump_Click(object sender, EventArgs e)
        {
            if (port.IsOpen)
            {
                string tx = START + "03" + "08008000";
                string crc_string;
                UInt16 check = crc(tx, (ushort)tx.Length);
                if (check < 0x00FF)
                {
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + "00" + crc_string);
                }
                else if (check < 0x0FFF)
                {
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + "0" + crc_string);
                }
                else
                {
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + crc_string);
                }
            }
            else
            {
                MessageBox.Show("Please select COMx port to flash !!!", "COM");
            }
        }
        private void btnWrite_Click(object sender, EventArgs e)
        {
            if (port.IsOpen)
            {
                string tx = START + "01" + txtAddressWrite.Text + txtNumberWrite.Text;

                if (txtData1.Text != null)
                {
                    char[] nameArray = txtData1.Text.ToCharArray();
                    Array.Reverse(nameArray);
                    string reverse = new string(nameArray);
                    tx += reverse;
                }
                if (txtData2.Text != null)
                    tx += txtData2.Text;
                if (txtData3.Text != null)
                    tx += txtData3.Text;
                if (txtData4.Text != null)
                    tx += txtData4.Text;

                string crc_string;
                UInt16 check = crc(tx, (ushort)tx.Length);
                if (check < 0x00FF)
                {
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + "00" + crc_string);
                }
                else if (check < 0x0FFF)
                {
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + "0" + crc_string);
                }
                else
                {
                    crc_string = crc(tx, (ushort)tx.Length).ToString("X");
                    port.Write(tx + crc_string);
                }
            }
            else
            {
                MessageBox.Show("Please select COMx port to flash !!!", "COM");
            }
        }
        private void btnClearConsleFlash_Click(object sender, EventArgs e)
        {
            txtSend.Clear();
        }
    }
}