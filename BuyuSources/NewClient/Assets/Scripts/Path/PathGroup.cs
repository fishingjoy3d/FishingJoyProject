using UnityEngine;
using System.Collections.Generic;

//路径群和鱼群是相似的概念，区别是：
//1.鱼群是多种/多条鱼对游走在一条路径上。
//2.路径群是其中的每条路径都游走一条相同的鱼类。
public class PathGroup
{
    public bool m_RenderPath        = true;
    public bool m_EnablePath        = true;
    public int m_PathCount = 5;
    public int m_PathRadius = 20;
    public int m_OffsetRadius = 10;
    public bool EnablePath
    {
        get { return m_EnablePath; }
        set { m_EnablePath = value; }
    }
    
    public void UpdatePath()
    {
        return;
    }
	void Reset()
	{
	}
	
	void Update () {
	}
}
