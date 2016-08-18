using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Windows.Forms;

namespace VisualWorldEditor
{
    class ServerPath
    {
        public ServerPath()
        {

        }

        public static String geometryDir = "/www/vu/root/files/vu2008-02-20/geometry/";

        public static String GetServerPath(String url)
        {
            byte serverNumber = 0;
            return GetServerPath(url, ref serverNumber);
        }

        public static String GetServerPath(String url, ref byte serverNumber)
        {
            String serverDiskPath = "";
            EditorSettings settings = new EditorSettings();
            switch (url)
            {
                case "95.169.190.11":
                    {
                        MessageBox.Show("�������� ���� �� �������� � ��������-��������!", "������", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        Environment.Exit(0);
                        break;
                    }
                case "192.168.0.2":
                    {
                        serverDiskPath = "//Fserver/WebServersCD/home/virtuniver.loc";
                        settings.ServerPath = "";
                        settings.Save();
                        serverNumber = 2;
                        break;
                    }
                case "192.168.0.78":
                    {
                        serverDiskPath = "//Tera/WebServersCD/home/virtuniver.loc";
                        settings.ServerPath = "";
                        settings.Save();
                        serverNumber = 78;
                        break;
                    }
                case "www.virtuniver.loc":
                    {
                        // � ����������� �� ip ������� ����
                        IPHostEntry hostEntry = Dns.GetHostEntry(url);
                        IPAddress[] ipList = hostEntry.AddressList;
                        foreach (IPAddress ip in ipList)
                        {
                            byte[] ipBytes = ip.GetAddressBytes();
                            if (ipBytes[3] == 2)
                            {
                                serverDiskPath = "//Fserver/WebServersCD/home/virtuniver.loc";
                                settings.ServerPath = "";
                                settings.Save();
                                serverNumber = ipBytes[3];
                                break;
                            }
                            else if (ipBytes[3] == 78)
                            {
                                serverDiskPath = "//Tera/WebServersCD/home/virtuniver.loc";
                                settings.ServerPath = "";
                                settings.Save();
                                serverNumber = ipBytes[3];
                                break;
                            }                            
                        }
                        break;
                    }
                case "2":
                    {
                        serverDiskPath = "//Fserver/WebServersCD/home/virtuniver.loc";
                        serverNumber = 2;
                        break;
                    }
                case "78":
                    {
                        serverDiskPath = "//Tera/WebServersCD/home/virtuniver.loc";
                        serverNumber = 78;
                        break;
                    }
                case "0":
                    {
                        FolderBrowserDialog serverPathBrowser = new FolderBrowserDialog();
                        serverPathBrowser.Description = "�������� ����� ����� ��� �����������:";
                        serverPathBrowser.RootFolder = Environment.SpecialFolder.MyComputer;
                        serverPathBrowser.ShowNewFolderButton = false;

                        MessageBox.Show("�������� ����� � �������� (��������: " + settings.MysqlHost + ").", "����������� ��������", MessageBoxButtons.OK, MessageBoxIcon.Information);

                        DialogResult dlgResult = serverPathBrowser.ShowDialog();

                        if (dlgResult == DialogResult.OK)
                        {
                            serverDiskPath = serverPathBrowser.SelectedPath;
                            serverNumber = 0;
                        }

                        break;
                    }
                default:
                    {
                        // ������� ���� �� �������
                        if (settings.ServerPath == "")
                        {
                            FolderBrowserDialog serverPathBrowser = new FolderBrowserDialog();
                            serverPathBrowser.Description = "�������� ����� �����:";
                            serverPathBrowser.RootFolder = Environment.SpecialFolder.MyComputer;
                            serverPathBrowser.ShowNewFolderButton = false;

                            MessageBox.Show("�������� ����� �� ������� � ������ ����� (��������: " + settings.MysqlHost + ").", "���������", MessageBoxButtons.OK, MessageBoxIcon.Information);

                            DialogResult dlgResult = serverPathBrowser.ShowDialog();

                            if (dlgResult == DialogResult.OK)
                            {
                                settings.ServerPath = serverPathBrowser.SelectedPath;
                                settings.Save();
                            }

                            serverDiskPath = settings.ServerPath;
                        }
                        else
                        {
                            serverDiskPath = settings.ServerPath;
                        }
                        break;
                    }
            }
            return serverDiskPath;
        }
    }
}
