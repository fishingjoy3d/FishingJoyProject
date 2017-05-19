using UnityEngine;
using System.Collections;

public class VipMgrLogic 
{

    VIPLogicUI m_VipLogicUI = new VIPLogicUI();
    MonthCardLogicUI m_MonthCardLogicUI = new MonthCardLogicUI();

    public void Init()
    {
        m_VipLogicUI.InitObj();
        m_MonthCardLogicUI.InitObj();
    }
    public void ShowVipWnd()
    {
        m_VipLogicUI.ShowVipWnd();
    }
    public void ShowVipDetailWnd()
    {
        m_VipLogicUI.ShowVipDetail();
    }
    public void ShowMonthCardWnd()
    {
        m_MonthCardLogicUI.ShowMonthCardWnd();
    }
    public void UpdateMonthCardData()
    {

    }
    public void ShutDown()
    {
        m_MonthCardLogicUI.ShutDown();
        m_VipLogicUI.ShutDown();
    }

    public VIPLogicUI VipLogicUI
    {
        get { return m_VipLogicUI; }
    }
    public MonthCardLogicUI MonthCardLogicUI
    {
        get { return m_MonthCardLogicUI; }
    }


}
