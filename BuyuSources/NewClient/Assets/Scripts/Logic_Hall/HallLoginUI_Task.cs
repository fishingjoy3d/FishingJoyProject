using UnityEngine;
using System.Collections.Generic;


public class HallLoginUI_Task : HallLoginUI_BaseWind
{
    ViewInf             m_Scroll = new ViewInf();
    UIPanel             m_UIPanel;
    Transform           m_ScrollTrans;
    List<TaskItem>      m_TaskList = new List<TaskItem>();
    Texture2D[]         m_TaskBgTex = new Texture2D[2];

    public void Init(GameObject go)
    {
        WindObj = go;
        BaseTranF = go.transform;

        for (byte i = 0; i < 2; ++i )
            m_TaskBgTex[i] = (Texture2D)ResManager.Instance.LoadObject(string.Format("Hall_Information_Task{0}", i), 
                "HallRes/Image/UI/", ResType.HallRes, typeof(Texture2D));

        m_Scroll.m_grid = BaseTranF.GetChild(0).GetChild(1).GetComponent<UIGrid>();
        m_Scroll.m_BaseChild = BaseTranF.GetChild(0).GetChild(0).gameObject;
        m_UIPanel = BaseTranF.GetChild(0).GetComponent<UIPanel>();
        m_ScrollTrans = BaseTranF.GetChild(0);
    }
    void InitTaskItem(RoleTaskBase value)
    {
        TaskItem item = new TaskItem();
        item.Init(m_Scroll.m_BaseChild, false);
        tagTaskStrInfo taskStrInfo = FishConfig.Instance.m_TaskInfo.m_TaskStr[value.pConfig.TaskID];
        string taskName = taskStrInfo.TaskName;
        string taskDes = taskStrInfo.TaskDec;
        uint curTaskValue = PlayerRole.Instance.TaskManager.GetTask(value.pConfig.TaskID).pTaskInfo.TaskValue;
        uint TaskValueMax = value.pConfig.TaskEventInfo.FinishValue;
        tagRewardOnce pOnce = FishConfig.Instance.GetRewardOnceByID(value.pConfig.RewardID);
        item.SetTaskDate(pOnce, value.pConfig.TaskID, taskDes, taskName, taskStrInfo.TaskIcon);
        item.UpdateTaskProgress(curTaskValue, TaskValueMax, value.m_TaskEventIsFinish);
        if (value.m_TaskEventIsFinish)
            item.UpdateTaskBg(m_TaskBgTex[1]);
        else
            item.UpdateTaskBg(m_TaskBgTex[0]);
        m_TaskList.Add(item);
    }
    static int CompareItemBySate(TaskItem item1, TaskItem item2)
    {
        if (item1.bFinish && !item2.bFinish)
            return -1;
        else if (!item1.bFinish && item2.bFinish)
            return 1;
        else
            return 0;

    }
    public void UpdateTaskDate()
    {
        if (WindObj.activeSelf != true)
            return;

        if (PlayerRole.Instance.TaskManager.GetTaskArray() == null)
            return;

        ClearGrid();
        m_TaskList.Clear();
       // m_ScrollTrans.localPosition = new Vector3(0, 110, 0);
       // m_UIPanel.clipOffset = new Vector2(0, -120);
        foreach (KeyValuePair<byte, RoleTaskBase> map in PlayerRole.Instance.TaskManager.GetTaskArray())
        {
            InitTaskItem(map.Value);
        }
        m_TaskList.Sort(CompareItemBySate);
        for (int i = 0; i < m_TaskList.Count; ++i)
        {
            m_Scroll.m_grid.AddChild(m_TaskList[i].m_BaseTrans);
            m_TaskList[i].ResetLocalScale();
        }
    }
    public void UpdateTaskProgress(byte taskID)
    {
        if (WindObj.activeSelf != true)
            return;
        if (PlayerRole.Instance.TaskManager.GetTaskArray() == null)
            return;
        for (byte i = 0; i < m_TaskList.Count; )
        {
            if (m_TaskList[i].TaskID == taskID)
            {
                if (PlayerRole.Instance.TaskManager.GetTaskArray().ContainsKey(taskID) == false)    //领取奖励后删除任务
                {
                    //显示完成任务的奖励
                    if (int256Function.GetBitStates(PlayerRole.Instance.RoleInfo.RoleMe.GetTaskStates(), taskID))
                    {
                        GlobalHallUIMgr.Instance.ShowTipBox_0(TipBox_0.TipBox_0_ZiTi.Mission, 2f, FishConfig.Instance.m_TaskInfo.m_TaskMap[taskID].RewardID);
                    }
                    TaskItem item = m_TaskList[i];
                    m_Scroll.m_grid.RemoveChild(item.m_BaseTrans);
                    item.ShutDown();
                    Utility.ListRemoveAt(m_TaskList, i);
                }
                else
                {
                    RoleTaskBase pBase = PlayerRole.Instance.TaskManager.GetTaskArray()[taskID];
                    uint curTaskValue = PlayerRole.Instance.TaskManager.GetTask(taskID).pTaskInfo.TaskValue;
                    uint TaskValueMax = pBase.pConfig.TaskEventInfo.FinishValue;
                    if (pBase.m_TaskEventIsFinish)
                        m_TaskList[i].UpdateTaskBg(m_TaskBgTex[1]);
                    else
                        m_TaskList[i].UpdateTaskBg(m_TaskBgTex[0]);

                    m_TaskList[i].UpdateTaskProgress(curTaskValue, TaskValueMax, pBase.m_TaskEventIsFinish);
                }

                break;
            }
            else
            {
                i++;
                //新增一个任务
                if ( (i == m_TaskList.Count) && (PlayerRole.Instance.TaskManager.GetTaskArray().ContainsKey(taskID)) )
                {
                  // InitTaskItem(PlayerRole.Instance.TaskManager.GetTaskArray()[taskID]);
                    UpdateTaskDate();
                }
            }
        }
        
    }
    
    void ClearGrid()
    {
        List<Transform> gridChid = m_Scroll.m_grid.GetChildList();

        foreach (Transform tr in gridChid)
        {
            GameObject.Destroy(tr.gameObject);
        }
        m_Scroll.m_grid.transform.DetachChildren();
    }
    public void ShutDown()
    {
        for (byte i = 0; i < 2; ++i )
            ResManager.Instance.UnloadObject(m_TaskBgTex[i]);
    }


}
