using UnityEngine;
using System.Collections;

public class GameLogicMgr : BaseWnd 
{
    Object m_obj;
    GameNiuniuUI m_GameNiuniuUI = new GameNiuniuUI();
    GameForestDanceUI m_GameForestDanceUI = new GameForestDanceUI();
    GameCarUI m_GameCarUI = new GameCarUI();

    public void Init()
    {
        if (m_obj != null)
            return;
        m_obj = ResManager.Instance.LoadObject("RelaxEnterWnd", "HallRes/Prefab/UI/", ResType.HallRes);
    }

    public void ShowGameEnterWnd()
    {
        m_BaseWndObject = (GameObject)GameObject.Instantiate(m_obj);
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        UIEventListener.Get(m_BaseTrans.GetChild(0).gameObject).onClick = EventShowNiuniuWnd;
        UIEventListener.Get(m_BaseTrans.GetChild(1).gameObject).onClick = EventShowCarWnd;
        UIEventListener.Get(m_BaseTrans.GetChild(2).gameObject).onClick = EventShowForestDanceWnd;
        UIEventListener.Get(m_BaseTrans.GetChild(4).gameObject).onClick = EventCloseGameWnd;
        WndManager.Instance.Push(m_BaseWndObject);
    }

    void EventShowNiuniuWnd(GameObject go)
    {
        PlayerRole.Instance.RoleMiNiGame.m_NiuNiuGame.OnJoinRoom();
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }

    void EventShowCarWnd(GameObject go)
    {

        //if (m_BaseWndObject != null)
        //{
        //    WndManager.Instance.Pop(m_BaseWndObject);
        //    GameObject.Destroy(m_BaseWndObject);
        //    m_BaseWndObject = null;
        //}
        //m_GameCarUI.ShowWnd();

        PlayerRole.Instance.RoleMiNiGame.m_CarGame.OnJoinRoom();
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }        
    }

    void EventShowForestDanceWnd(GameObject go)
    {
        PlayerRole.Instance.RoleMiNiGame.m_DialGame.OnJoinRoom();
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }

    void EventCloseGameWnd(GameObject go)
    {
        ShutDown();
    }

    public void Update(float deltaTime)
    {
        m_GameForestDanceUI.Update(deltaTime);
        m_GameNiuniuUI.Update(deltaTime);
        m_GameCarUI.Update(deltaTime);
    }
    public void ShowBackToMain()
    {
        m_GameForestDanceUI.BackToMain();
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
        m_GameNiuniuUI.ShutDown();
        m_GameForestDanceUI.ShutDown();
        m_GameCarUI.ShutDown();
    }


    //牛牛
    public void ShowNiuniuWnd()
    {
        m_GameNiuniuUI.InitObj();
        m_GameNiuniuUI.ShowNiuniuWnd();
        GlobalHallUIMgr.Instance.GameShare.EnterGame();
    }

    public void SetPlayerHeader(Texture2D tex)
    {
        m_GameNiuniuUI.SetPlayerHeader(tex);
    }

    public void GetNiuniuHostline(WriteBankerList list)
    {
        m_GameNiuniuUI.GetHostLine(list);
    }

    public void UpdateNiuniuHost(BankerUserInfo info)
    {
        m_GameNiuniuUI.UpdateHost(info);
    }

    public void NiuniuStopWeager()
    {
        m_GameNiuniuUI.StopWeager();
    }

    public void NiuniuGetNormalPeople(uint page, uint sum, NiuNiuRoleInfo[] array)
    {
        m_GameNiuniuUI.GetNormalPeople(page, sum, array);
    }

    public void NiuniuVipWeager(byte seat, uint addGlobal, byte index, uint allGlobal)
    {
        m_GameNiuniuUI.VipWeager(seat, addGlobal, index, allGlobal);
    }

    public void NiuniuVipGlobalChange(uint id, byte seat, uint globalSum)
    {
        m_GameNiuniuUI.VipGlobalChange(id, seat, globalSum);
    }

    public void NiuniuSnatchHost()
    {
        m_GameNiuniuUI.SnatchHost();
    }

    public void NiuniuVipSeatChange(bool result, VipSeatList list)
    {
        m_GameNiuniuUI.VipSeatChange(result, list);
    }

    public void NiuniuSetVipHead(byte id, Texture2D tex)
    {
        m_GameNiuniuUI.SetVipHead(id, tex);
    }

    public void NiuniuSetNormalHead(Texture2D tex, uint id, byte index)
    {
        m_GameNiuniuUI.SetNormalHead(tex, id, index);
    }

    public void NiuniuBeLeaveVip(NiuNiuRoleInfo info)
    {
        m_GameNiuniuUI.BeLeaveVip(info);
    }

    public void SetNiuniuHostHeader(Texture2D tex)
    {
        m_GameNiuniuUI.SetHostHeader(tex);
    }
    public bool GetGameNiuNiuWnd()
    {
        if (m_GameNiuniuUI.m_BaseTrans == null)
            return false;
        return true;
    }
    public void GetNiuNiuTableState(NiuNiuGameLog log, VipSeatList list, BankerUserInfo TableBankerInfo, WriteBankerList TableWriteBankerList, 
        NiuNiuTableStates state, byte time, byte[] card, ushort[] result, ulong[] globalNum)
    {
        m_GameNiuniuUI.GetTableState(log, list, TableBankerInfo, TableWriteBankerList, state, time, card, result, globalNum);
    }

    public void UpdateNiuniuDate(ulong[] date)
    {
        m_GameNiuniuUI.UpdateTotalMoney(date);
    }

    public void UpdateNiuniuMyWeagerNum()
    {
        m_GameNiuniuUI.UpdateMyWeagerNum();
    }
    public void GetNiuniuResult(BankerUserInfo tableBankInfo, VipSeatList oldVipInfo, VipSeatList newVipInfo, NiuNiuGameLog log, int state, TableBrandResult result)
    {
        m_GameNiuniuUI.GetResult(tableBankInfo ,oldVipInfo, newVipInfo ,log, state, result);
    }
    public void SetNiuniuWeagerBtn() 
    {
        m_GameNiuniuUI.SetWeagerBtn();
    }

    public void SetNiuniuMyGold()
    {
        m_GameNiuniuUI.SetMyGoldNum();
    }

    //车
    public void ShowCarWnd()
    {
        m_GameCarUI.ShowWnd();
        GlobalHallUIMgr.Instance.GameShare.EnterGame();
    }
    public bool GetCarLogicMgr()
    {
       return m_GameCarUI.IsAcitve();
    }

    public void EnterGameCar(byte TableUpdateSec,
     CarTableStates TableStates,
     CarTableUpdateInfo TableGlobelInfo,
     CarBankerUserInfo TableBankerInfo,
     CarWriteBankerList TableWriteBankerList,
  CarVipSeatList TableVipSeatInfo,
  CarGameLog TableGameLog,
  byte TableResult)
    {
        m_GameCarUI.EnterGameCar( TableUpdateSec,
      TableStates,
      TableGlobelInfo,
      TableBankerInfo,
      TableWriteBankerList,
   TableVipSeatInfo,
   TableGameLog,
   TableResult);
    }
    public void SetCarAvetarSelf(Texture2D tex)
    {
        m_GameCarUI.SetCarAvetarSelf(tex);
    }
    public void SetCarAvetarHost(Texture2D tex)
    {
        m_GameCarUI.SetCarAvetarHost(tex);
    }
    public void SetCarAvetarPlayList(Texture2D tex, uint id, byte data)
    {
        m_GameCarUI.SetCarAvetarPlayList(tex,id,data);
    }
    public void SetCarBetBtn()
    {
        m_GameCarUI.SetCarBetBtn();
    }
     public void SetCarPlayerGold()
    {
        m_GameCarUI.SetCarPlayerGold();
    }
     public void UpdateCarMyWeagerNum()
    {
        m_GameCarUI.UpdateCarMyWeagerNum();
    }
     public void UpdateCarDate(ulong[] date)
     {
         m_GameCarUI.UpdateCarDate(date);
     }
     public void GetCarHostline(CarWriteBankerList list)
     {
         m_GameCarUI.GetCarHostline(list);
     }
     public void CarGetNormalPeople(uint page, uint sum, CarRoleInfo[] array)
     {
         m_GameCarUI.CarGetNormalPeople(page, sum, array);
     }
     public void CarStopWeager()
     {
         m_GameCarUI.CarStopWeager();
     }
     public void UpdateCarHost(CarBankerUserInfo info)
     {
         m_GameCarUI.UpdateCarHost(info);
     }
     public void CarStatusChange(CarTableStates TableStates,
  CarTableUpdateInfo TableGlobelInfo,
  CarVipSeatList OldVipSeatInfo,
  CarVipSeatList TableVipSeatInfo,
  CarWriteBankerList TableWriteBankerList,
  CarBankerUserInfo TableBankerUserInfo,
  CarGameLog TableGameLog,
  byte TableResult)
     {
         m_GameCarUI.CarStatusChange(
         TableStates,
      TableGlobelInfo,
      OldVipSeatInfo,
      TableVipSeatInfo,
      TableWriteBankerList,
      TableBankerUserInfo,
      TableGameLog,
      TableResult);
     }
     public void CarSnatchHost()
     {
         m_GameCarUI.CarSnatchHost();
     }

    //森林舞会
    public bool GetGameForestDanceWnd()
    {
        if (m_GameForestDanceUI.m_BaseTrans == null)
            return false;
        return true;
    }

    public void ShowForestDanceWnd()
    {
        m_GameForestDanceUI.InitObj();
        m_GameForestDanceUI.ShowWnd();
        GlobalHallUIMgr.Instance.GameShare.EnterGame();
    }

    public void ForestDanceSetPlayerHead(Texture2D tex)
    {
        m_GameForestDanceUI.SetPlayerHead(tex);
    }

    public void ForestDanceGetTableState(byte tableUpdateSec, DialTableStates TableStates, DialTableUpdateInfo TableGlobelInfo,
        DialTableAreaData AreaData, DialBankerUserInfo TableBankerInfo, DialWriteBankerList TableWriteBankerList,
        DialVipSeatList TableVipSeatInfo, DialGameLog TableGameLog, uint tableResult)
    {
        m_GameForestDanceUI.GetTableState(tableUpdateSec, TableStates, TableGlobelInfo,
        AreaData, TableBankerInfo, TableWriteBankerList,
        TableVipSeatInfo, TableGameLog, tableResult);
    }

    public void ForestDanceStateChange(DialTableStates TableStates, DialTableUpdateInfo TableGlobelInfo,
        DialTableAreaData AreaData, DialVipSeatList OldVipSeatInfo, DialVipSeatList TableVipSeatInfo,
        DialWriteBankerList TableWriteBankerList, DialBankerUserInfo TableBankerUserInfo,
        DialGameLog TableGameLog, uint tableResult)
    {
        m_GameForestDanceUI.StateChange(TableStates, TableGlobelInfo,
        AreaData, OldVipSeatInfo, TableVipSeatInfo,
        TableWriteBankerList, TableBankerUserInfo,
        TableGameLog, tableResult);
    }

    public void ForestDanceUpdateTotalWeager(DialTableUpdateInfo TableGlobelInfo, DialTableStates TableStates)
    {
        m_GameForestDanceUI.UpdateTotalWeager(TableGlobelInfo, TableStates);
    }

    public void ForestDanceUpdateMyWeager(uint[] TableGlobelInfo)
    {
        m_GameForestDanceUI.UpdateMyWeager(TableGlobelInfo);
    }

    public void ForestDanceVipChange(bool Result, DialVipSeatList TableVipList)
    {
        m_GameForestDanceUI.VipSeatChange(Result, TableVipList);
    }

    public void ForestDanceSetVipHead(byte id, Texture2D tex)
    {
        m_GameForestDanceUI.SetVipHead(id, tex);
    }

    public void ForestDanceLeaveVip(DialRoleInfo info)
    {
        m_GameForestDanceUI.BeLeaveVip(info);
    }

    public void ForestDanceGetHostLine(DialWriteBankerList list)
    {
        m_GameForestDanceUI.GetHostLine(list);
    }

    public void ForestDanceUpdateHost(DialBankerUserInfo info)
    {
        m_GameForestDanceUI.UpdateHost(info);
    }

    public void ForestDanceSetHostHead(Texture2D tex)
    {
        m_GameForestDanceUI.SetHostHeader(tex);
    }

    public void ForestDanceGetNormal(uint Page, uint TotalRoleSum, DialRoleInfo[] Array)
    {
        m_GameForestDanceUI.GetNormalPeople(Page, TotalRoleSum, Array);
    }

    public void ForestDanceSetNormalHead(Texture2D tex, uint id, byte data)
    {
        m_GameForestDanceUI.SetNormalHead(tex, id, data);
    }

    public void ForestDanceSetMyGold()
    {
        m_GameForestDanceUI.SetPlayerGold();
    }

    public void ForestDanceSetWeagerBtn()
    {
        m_GameForestDanceUI.SetWeagerBtn();
    }    
}
