using UnityEngine;
using System.Collections.Generic;
using System.Runtime.InteropServices;
public enum ResolutionLevel
{
    RESOLUTION_HIGH,
    RESOLUTION_MID,
    RESOLUTION_LOW,
    RESOLUTION_ORIGNAL,
}
public class Resolution
{
    public const float DEFAULT_ASPECT = 1.777777777f;
    static int      m_nScreenWidth;
    static int      m_nScreenHeight;
    static float    m_nOrgAspect;
    static bool     m_bIsHD;

    public static int GetScreenWidth()
    {
        return m_nScreenWidth;
    }
    public static int GetScreenHeight()
    {
        return m_nScreenHeight;
    }

    public static bool IsHD
    {
        get
        {
            return m_bIsHD;
        }
    }
    public static  void SwitchFullscreen()
    {
        if(Screen.width == m_nScreenWidth)
        {
            Screen.SetResolution(1280, 720, false);
        }
        else
        {
            Screen.SetResolution(m_nScreenWidth, m_nScreenHeight, true);
        }
    }
    public static void ResetViewport(bool b16_9)
    {
        if (b16_9)
        {
            Camera.main.aspect = DEFAULT_ASPECT;
            //计算当前aspect和1.777之间的比例
            float defaultHeight = m_nScreenWidth / DEFAULT_ASPECT;
            float factor = 1.0f - defaultHeight / m_nScreenHeight;
            if (factor > 0.0001f)
            {
                float a = factor;
                Camera.main.rect = new Rect(0, a * 0.5f, 1.0f, 1.0f - a);
                
            }
        }
        else
        {
            Camera.main.rect = new Rect(0, 0, 1, 1);
            Camera.main.aspect = m_nOrgAspect;
        }
    }
    public static void CheckResolution()
    {
        Camera.main.aspect = DEFAULT_ASPECT;
        ResetViewport(true);
    }
#if (UNITY_STANDALONE_WIN)
    [DllImport("user32.dll")]
    public static extern int GetSystemMetrics(int nIndex);
#endif
    public static void GlobalInit()
    {

#if (UNITY_STANDALONE_WIN)
        //U3D在win下有BUG，分辨率更改之后，重启游戏会保存之前的分辨率
        m_nScreenWidth = GetSystemMetrics(0);
        m_nScreenHeight = GetSystemMetrics(1);
#else
        m_nScreenWidth = Screen.currentResolution.width;
        m_nScreenHeight = Screen.currentResolution.height;
#endif
        m_bIsHD = m_nScreenWidth > 1280;
        Screen.SetResolution(m_nScreenWidth, m_nScreenHeight, true);
       
        m_nOrgAspect = (float)m_nScreenWidth / m_nScreenHeight;
        Camera.main.aspect = DEFAULT_ASPECT;
        ResetViewport(true);
        return;
    }
}
