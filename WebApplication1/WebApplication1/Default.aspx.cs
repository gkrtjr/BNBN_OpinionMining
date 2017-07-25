using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Text;

namespace WebApplication1
{
    public partial class _Default : Page
    {
        const int PORT = 4977;
        const string IP = "127.0.0.1";
            //"175.214.164.29";

        NetworkStream NS = null;
        StreamReader SR = null;
        StreamWriter SW = null;
        static TcpClient client = null;

        protected void Page_Load(object sender, EventArgs e)
        {
            TextBox1.Attributes["onkeypress"] = "if (event.keyCode==13){" + Page.GetPostBackEventReference(Button2) + "; return false;}";
            TextBox1.Focus();
        }

        protected void Button2_Click(object sender, EventArgs e)
        {
            try
            {
                client = new TcpClient();
                client.Connect(IP, PORT);

                NS = client.GetStream(); // 소켓에서 메시지를 가져오는 스트림

                SR = new StreamReader(NS, Encoding.GetEncoding("euc-kr")); // Get message
                SW = new StreamWriter(NS, Encoding.GetEncoding("euc-kr")); // Send message

                string textBoxStr = TextBox1.Text;
                bool isTextBoxEmpty = (textBoxStr.Length == 0 || textBoxStr.Equals('\r') || textBoxStr.Equals('\n'));
                if (!isTextBoxEmpty)
                {
                    string SendMessage = textBoxStr;
                    string GetMessage = string.Empty;

                    SW.WriteLine(SendMessage);
                    SW.Flush();

                    string resultStr = String.Empty;

                    while (!GetMessage.Equals("$end"))
                    {
                        GetMessage = SR.ReadLine();
                        if (GetMessage.Equals("$end")) break;
                        resultStr += GetMessage;
                        //Console.WriteLine("Get:" + GetMessage);
                    }
                    BulletedList1.Items.Add(resultStr);
                    TextBox1.Text = "";

                }
            }
            catch { }


        }

        protected void RadioButtonList1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }

}