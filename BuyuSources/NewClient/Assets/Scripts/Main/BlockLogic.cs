using UnityEngine;
using System.Collections.Generic;

public class BlockData
{
    public string   Msg;
    public ushort   Code;
    public bool     Close;
    public bool     Block;
    public bool     URL;
}
public class BlockLogic:Singleton<BlockLogic>
{
    LinkedList<BlockData> m_BlockList = new LinkedList<BlockData>();
    bool m_bBlock = false;
    bool m_bClose = false;
    public bool Update(float delta)
    {
        if (m_bBlock)
            return false;
        if(m_BlockList.Count > 0)
        {
            BlockData bd = m_BlockList.First.Value;
            m_BlockList.RemoveFirst();
            if(bd.URL)
            {
                NativeInterface.Block_DownNewClientVersion(bd.Msg);
            }
            else
            {
                NativeInterface.Block_ShowMsg(bd.Msg, bd.Code, bd.Close);
            }
            m_bClose = bd.Close;
            m_bBlock = bd.Block;
        }
        return !m_bBlock;
    }
    public void HandleMsg()
    {
        if(m_bClose)
            SceneMain.Instance.Shutdown();
        m_bBlock = false;
    }
    public void AddBlock(string msg, ushort code, bool bclose, bool bblock, bool url)
    {
        BlockData bd = new BlockData();
        bd.Msg = msg;
        bd.Code = code;
        bd.Close = bclose;
        bd.Block = bblock;
        bd.URL = url;
        m_BlockList.AddLast(bd);
    }
}

