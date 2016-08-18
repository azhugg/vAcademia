using System;
using System.Collections;
using System.Text;

namespace VisualWorldEditor
{
    partial class HistoryStorage
    {
        private int m_capacity; // ������������ ����� �������
        private int m_historyLength; // ������� ����� �������
        private int m_historyCurrent; // ������� ��������� � �������
        private HistoryElement[] m_actionsArray; // ��������� �������

        public HistoryStorage(int capacity)
        {
            m_capacity = capacity;
            m_historyLength = 0;
            m_historyCurrent = -1;
            m_actionsArray = new HistoryElement[m_capacity];
        }

        public bool CheckDuplicateName(String newName) // ��������� ���� �� � ������� ������ ��� � ����� ��������������� ������
        {
             lock (this)
            {
                for (int i=0; i< m_historyLength; i++)
                {
                    if (m_actionsArray[i].cmdId == Commander.CMD_RENAME)
                    {
                        if (m_actionsArray[i].cmdParams[0] == newName)
                            return true;
                    }
                }

            }
            return false;
        }


        public bool IsAlreadyRenamed(String newName) // ��������� ���� �� � ������� ��������������� ������ �� �� 
        {
            lock (this)
            {
                for (int i = 0; i < m_historyLength; i++)
                {
                    if (m_actionsArray[i].cmdId == Commander.CMD_RENAME)
                    {
                        if (m_actionsArray[i].objNames[0] == newName)
                            return true;
                    }
                }

            }
            return false;
        }

        public void AddAction(HistoryElement newAction) // �������� �������� � �������
        {
            lock (this)
            {
                if (m_historyCurrent == m_capacity - 1)
                {
                    // ���������� ������� ������� ��� ������������
                    m_capacity += m_capacity;
                    HistoryElement[] increaseHistory = new HistoryElement[m_capacity];
                    Array.Copy(m_actionsArray, increaseHistory, m_actionsArray.Length);
                    m_actionsArray = increaseHistory;
                }

                m_actionsArray[m_historyCurrent+1] = newAction;
                m_historyCurrent++;
                m_historyLength = m_historyCurrent + 1;

                // ��������� ������� ����������� ������
                HistoryAllowedEventArgs undoEvargs = new HistoryAllowedEventArgs(true);
                OnUndoAllowed(undoEvargs);
                // ��������� ������� ��������� �������
                HistoryAllowedEventArgs redoEvargs = new HistoryAllowedEventArgs(false);
                OnRedoAllowed(redoEvargs);
            }
        }

        // �������������� �����
        private void ShiftHistory() // �������� �������, ���� ������ �����
        {
            for (int actIndex = 0; actIndex < m_capacity - 1; actIndex++ )
            {
                m_actionsArray[actIndex] = m_actionsArray[actIndex + 1];
            }
        }

        public int GetHistoryPosition() // ������� ��������� � �������
        {
            return m_historyCurrent;
        }
        
        public int GetHistoryLength() // ����� �������� �������
        {
            return m_historyLength;
        }

        public HistoryElement GetHistoryElement(int index) // ������� �������
        {
            if(index < 0 || index > m_historyLength)
            {
                return null;
            }

            return m_actionsArray[index];
        }

        public bool isRedo() // �������� �� ������
        {
            return (m_historyCurrent < m_historyLength - 1);         
        }

        public HistoryElement Redo() // ������ �������� �� �������
        {
            if(!isRedo())
            {
                return null;
            }
            m_historyCurrent++;

            HistoryAllowedEventArgs historyEvargs = null;
            // ��������� ������� ����������� �������
            if (isRedo())
            {
                historyEvargs = new HistoryAllowedEventArgs(true);
                OnRedoAllowed(historyEvargs);
            }
            else
            {
                historyEvargs = new HistoryAllowedEventArgs(false);
                OnRedoAllowed(historyEvargs);
            }

            // ��������� ������� ����������� ������
            historyEvargs = new HistoryAllowedEventArgs(true);
            OnUndoAllowed(historyEvargs); 

            return m_actionsArray[m_historyCurrent];
        }

        public bool isUndo() // �������� �� �����
        {
            return (m_historyCurrent > -1);  
        }

        public HistoryElement Undo() // ����� ��������
        {
            if(!isUndo())
            {
                return null;
            }
            m_historyCurrent--;

            HistoryAllowedEventArgs historyEvargs = null;
            // ��������� ������� ����������� ������
            if(isUndo())
            {
                historyEvargs = new HistoryAllowedEventArgs(true);
                OnUndoAllowed(historyEvargs);
            }
            else
            {
                historyEvargs = new HistoryAllowedEventArgs(false);
                OnUndoAllowed(historyEvargs);
            }

            // ��������� ������� ����������� �������
            historyEvargs = new HistoryAllowedEventArgs(true);
            OnRedoAllowed(historyEvargs);            

            return m_actionsArray[m_historyCurrent+1];
        }

        public void Clear() // ������� ������� (�������� ��������� ���)
        {
            m_historyCurrent = -1;
            m_historyLength = 0;

            HistoryAllowedEventArgs historyEvargs = new HistoryAllowedEventArgs(false);
            OnUndoAllowed(historyEvargs);  
            OnRedoAllowed(historyEvargs);    
        }
    }
}
