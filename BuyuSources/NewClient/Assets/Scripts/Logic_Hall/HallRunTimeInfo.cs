using UnityEngine;
using System.Collections;

public class HallRunTimeInfo 
{
    static public HallRunTimeInfo Instance;
    static public HallRunTimeInfo Instance_UI;
    public HallLogic m_login;
    public HallLogicUI m_loginUi;
    public bool m_bInitCompletion = false;
    public static void Init(HallLogic login )
    {
        Instance = new HallRunTimeInfo();
        Instance.m_login = login;
    }
    public static void SetHallInfoBeginInit()
    {
        Instance_UI.m_bInitCompletion = false;
    }
    public static void SetHallInfoInitCompletion()
    {
        Instance_UI.m_bInitCompletion = true;
    }
    public static bool IsHallInfoInitCompletion()
    {
        return Instance_UI.m_bInitCompletion;
    }
    public static void Init_UI(HallLogicUI loginui)
    {
        Instance_UI = new HallRunTimeInfo();
        Instance_UI.m_loginUi = loginui;
    }
    public void ShutDown(byte index)
    {
        if (index == 0)
            Instance = null;
        else
            Instance_UI = null;
    }
    public static HallLogic Login_
    {
        get { return Instance.m_login; }
    }
    public static HallLogicUI Login_UI
    {
        get { return Instance_UI.m_loginUi; }
    }
}
