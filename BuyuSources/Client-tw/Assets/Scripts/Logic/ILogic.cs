using UnityEngine;
using System.Collections.Generic;

//游戏中的逻辑接口
//1.启动时的更新逻辑
//2.初始化的逻辑
//3.登陆的逻辑
//4.注册的逻辑
//5.大厅的逻辑
//6.个人信息的逻辑
//7.游戏场景中的逻辑
public enum LogicType
{
    LOGIC_UPDATE = 0,
    LOGIC_INIT,
    LOGIC_LOGON,
    LOGIC_HALL,
    LOGIC_SCENE,

    LOGIC_MAX
}
public interface ILogic
{
    //初始化
    bool Init(ILogicUI logicUI, object obj);
    //每帧更新
    void Update(float delta);
    //释放资源
    void Shutdown();
    
}

//逻辑表示界面
public interface ILogicUI
{
    //初始化
    bool Init(ILogic logic, object obj);
    //GUI调用
    void OnGUI(float delta);
    //每帧调用
    void Update(float delta);
    //资源释放
    void Shutdown();                
}
