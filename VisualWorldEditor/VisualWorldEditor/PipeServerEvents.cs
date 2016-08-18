using System;
using System.Collections.Generic;
using System.Text;

namespace VisualWorldEditor
{
    public delegate void CommandAddedEventHandler(Object sender, CommandAddedEventArgs args);

    // ���� ��������� �������
    public class CommandAddedEventArgs
    {
        private DataReader cmdData;

        public CommandAddedEventArgs(DataReader _cmdData)
        {
            cmdData = _cmdData;
        }

        public DataReader CmdData
        {
            get { return (cmdData); }
        }
    };

    partial class PipeServer
    {
        public event CommandAddedEventHandler CommandAdded; // ������� ��������� ����� �������

        protected virtual void OnCommandAdded(CommandAddedEventArgs args)
        {
            if (CommandAdded != null)
            {
                CommandAdded(this, args);
            }
        }


    }
}
