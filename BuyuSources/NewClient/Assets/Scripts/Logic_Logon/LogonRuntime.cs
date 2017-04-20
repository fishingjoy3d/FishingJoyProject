using UnityEngine;
using System.Collections.Generic;

public class LogonRuntime 
{
    static public LogonRuntime Instance;
    public LogonLogic Logon;

    public static void Init(LogonLogic logic)
    {
        Instance = new LogonRuntime();

        Instance.Logon = logic;
    }
    public static void Shutdown()
    {
        Instance = null;
    }
    public static LogonLogic LogonLogic
    {
        get
        {
            if (Instance != null)
                return Instance.Logon;
            else
                return null;
        }
    }
    public static LogonLogicUI LogonLogicUI
    {
        get
        {
            return Instance.Logon.LogicUI;
        }
    }
}
