using System;
using System.Collections.Generic;
public class LogicRuntimeData
{
    public ILogic       LogicObj;
    public ILogicUI     LogicUIObj;
    public ResType      LogicRes;
    public LogicType    Type;
}
public class LogicFlowManager
{
    interface ILogicFactoryBase
    {
        ILogic CreateLogic();
    }
    interface ILogicUIFactoryBase
    {
        ILogicUI CreateLogicUI();
    }
    struct LogicMapping
    {
        public ResType              LogicResType;
        public ILogicFactoryBase    LogicInterface;
        public ILogicUIFactoryBase  LogicUIInterface;
    }
    class LogicFactory<T> : ILogicFactoryBase where T : ILogic, new()
    {
        public ILogic CreateLogic()
        {
            return new T();
        }
    }
    class LogicUIFactory<T> : ILogicUIFactoryBase where T : ILogicUI, new()
    {
        public ILogicUI CreateLogicUI()
        {
            return new T();
        }
    }
    static LogicMapping NewMapping<LOGIC, UI>(LogicType logicType, ResType res)
        where LOGIC :ILogic, new()
        where UI    :ILogicUI, new()
    {
        LogicMapping lm     = new LogicMapping();
        lm.LogicResType     = res;
        lm.LogicInterface   = new LogicFactory<LOGIC>();
        lm.LogicUIInterface = new LogicUIFactory<UI>();
        return lm;
    }

    static LogicMapping[] ms_LogicMapping = new LogicMapping[(int)LogicType.LOGIC_MAX]
    {
        //按枚举顺序，在此注册逻辑
        NewMapping<UpdateLogic, UpdateLogicUI>(LogicType.LOGIC_UPDATE,  ResType.MAX),
        NewMapping<InitLogic,   InitLogicUI>(LogicType.LOGIC_INIT,      ResType.GlobalRes),
        NewMapping<LogonLogic,  LogonLogicUI>(LogicType.LOGIC_LOGON,    ResType.LogonRes),
        NewMapping<HallLogic,   HallLogicUI>(LogicType.LOGIC_HALL,      ResType.HallRes),
        NewMapping<SceneLogic,  SceneLogicUI>(LogicType.LOGIC_SCENE,    ResType.SceneRes),
    };
    
    public static LogicRuntimeData GetBackLogic(LogicType lt)
    {
        if(lt > LogicType.LOGIC_LOGON)
        {
            return GetLogic(--lt);
        }
        else
        {
            LogMgr.Log("GetBackLogic failed.");
            return null;
        }
    }
    public static LogicRuntimeData GetForwardLogic(LogicType lt)
    {
        if (lt < LogicType.LOGIC_MAX - 1)
        {
            return GetLogic(++lt);
        }
        else
        {
            LogMgr.Log("GetForwardLogic failed.");
            return null;
        }
    }
    public static LogicRuntimeData GetLogic(LogicType lt)
    {
        LogicRuntimeData ld = new LogicRuntimeData();
        LogicMapping lm = ms_LogicMapping[(int)lt];
        ld.LogicObj = lm.LogicInterface.CreateLogic();
        ld.LogicUIObj = lm.LogicUIInterface.CreateLogicUI();
        ld.LogicRes = lm.LogicResType;
        ld.Type = lt;
        return ld;
    }
}
