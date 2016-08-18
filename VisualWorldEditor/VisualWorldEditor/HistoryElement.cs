using System;
using System.Collections.Generic;
using System.Text;

namespace VisualWorldEditor
{
    class HistoryElement
    {
        public byte cmdId; // id �������
        public String[] objNames; // ����� ��������
        public Object cmdParam; // ������ ���������
        public String[] cmdParams; // ��������� ���������

        public HistoryElement(byte _cmdId, String[] _objNames)
        {
            cmdId = _cmdId;
            objNames = _objNames;
            cmdParam = 0;
            cmdParams = null;
        }

        public HistoryElement(byte _cmdId, String[] _objNames, Object _cmdParam)
        {
            cmdId = _cmdId;
            objNames = _objNames;
            cmdParam = _cmdParam;
            cmdParams = null;
        }

        public HistoryElement(byte _cmdId, String[] _objNames, String[] _cmdParams)
        {
            cmdId = _cmdId;
            objNames = _objNames;
            cmdParam = 0;
            cmdParams = _cmdParams;
        }

        public HistoryElement(byte _cmdId, String[] _objNames, Object _cmdParam, String[] _cmdParams)
        {
            cmdId = _cmdId;
            objNames = _objNames;
            cmdParam = _cmdParam;
            cmdParams = _cmdParams;
        }
    }
}
