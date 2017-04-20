using UnityEngine;
using System.Collections.Generic;

struct DestroyFishData
{
    public byte FishType;
}
public class SceneFishMgr
{
    //Fish[] m_FishList = new Fish[FishSetting.FISH_MAX_NUM];
    Dictionary<ushort, Fish> m_FishList = new Dictionary<ushort, Fish>();
    Dictionary<ushort, DestroyFishData> m_DestroyFishList = new Dictionary<ushort, DestroyFishData>();

    Fish[] m_BackFishList = null;
    ushort m_FishNum;
    public void Init()
    {
    }
    
    public void SetFish(Fish fish)
    {
        ushort id = fish.FishID;
        Fish findFish;
        if(m_FishList.TryGetValue(id, out findFish))
        {
            if (findFish.IsDelay)
            {
                findFish.Destroy();
            }
            else
            {
                LogMgr.Log("存在相同的鱼ID1:" + id.ToString() + "time:" + findFish.Time + ", delta:" + SceneMain.Instance.Delta + ", timedelta:" + Time.deltaTime);
                return;
            }
            m_FishList[id] = fish;
        }
        else
            m_FishList.Add(id, fish);
    }
    public void  LaunchFishByAsycScene(NetCmdPack pack)
    {
        NetCmdSyncFish cmdFish = (NetCmdSyncFish)pack.cmd;
        GroupDataList gdl = FishResManager.Instance.GetFishGroup(cmdFish.GroupID);
        float elapsedTime = Utility.TickSpan(pack.tick) + SceneRuntime.NetDelayTime;
        if (gdl.PathGroupData != null)
        {
            FishPathGroupData pathgroup = gdl.PathGroupData;
            PathLinearInterpolator[] interpList = PathManager.Instance.GetPathGroup(pathgroup.PathGroupIndex, SceneRuntime.Inversion);
            if (cmdFish.PathIdx >= interpList.Length)
            {
                LogMgr.Log("路径数量和服务器不一致,路径群:" + cmdFish.PathGroup + ", 索引:" + cmdFish.PathIdx);
            }
            {
                Fish fish = new Fish();
                fish.Init(cmdFish.FishID, pathgroup.FishIndex, pathgroup.FishScaling, cmdFish.FishTime, pathgroup.ActionSpeed, pathgroup.ActionUnite, pathgroup.Speed, interpList[cmdFish.PathIdx]);
#if UNITY_EDITOR
                fish.SetModelName("Fish_PathGroup_" + cmdFish.GroupID);
#endif
                if (fish.Update(elapsedTime))
                {
                    SetFish(fish);
                    fish.Controller.CheckCurrentEvent(cmdFish.IsActiveEvent);
                    fish.Controller.PathEvent.m_CurElapsedTime = cmdFish.ElapsedTime * 0.001f;
                    if(cmdFish.Package != 255)
                        fish.SetPackage(cmdFish.Package);
                    if (cmdFish.DelayType != (byte)FISH_DELAY_TYPE.DELAY_NONE)
                    {
                        float scl;
                        float[] dur = new float[3];
                        Utility.ReductionToFloat(cmdFish.DelayScaling, cmdFish.DelayDuration1, cmdFish.DelayDuration2, cmdFish.DelayDuration3,
                            out scl, dur);
                        float time = cmdFish.DelayCurrentTime * 0.001f;
                        fish.Controller.TimeController.AddSkillTimeScaling(scl, dur, (FISH_DELAY_TYPE)cmdFish.DelayType, time);
                    }
                }
            }
        }
        else
        {
            float fInv = SceneRuntime.Inversion ? -1.0f : 1.0f;
            int pathIndex = cmdFish.PathGroup;
            int gdIdx = cmdFish.PathIdx >> 8;
            int fishIdx = cmdFish.PathIdx & 0xff;

            PathLinearInterpolator pi = PathManager.Instance.GetPath(pathIndex, SceneRuntime.Inversion);
            if(gdIdx >= gdl.GroupDataArray.Length)
            {
                LogMgr.Log("场景鱼同步，索引超出界限1:" + gdIdx);
                return;
            }
            GroupData gd = gdl.GroupDataArray[gdIdx];
            {
                {
                    Fish fish = new Fish();
                    fish.Init(cmdFish.FishID, gd.FishIndex, gd.FishScaling, cmdFish.FishTime, gd.ActionSpeed, gd.ActionUnite, gd.SpeedScaling, pi);
#if UNITY_EDITOR
                    fish.SetModelName("Fish_FishGroup_" + cmdFish.GroupID + "_Path_" + pathIndex);
#endif
                    if (fish.Update(elapsedTime))
                    {
                        if (fishIdx >= gd.PosList.Length)
                        {
                            LogMgr.Log("场景鱼同步，索引超出界限2:" + gdIdx);
                            return;
                        }
                        fish.SetOffset(new Vector3(0, fInv * gd.PosList[fishIdx].y, gd.PosList[fishIdx].z));
                        SetFish(fish);
                        if (cmdFish.Package != 255)
                            fish.SetPackage(cmdFish.Package);
                        fish.Controller.CheckCurrentEvent(cmdFish.IsActiveEvent);
                        fish.Controller.PathEvent.m_CurElapsedTime = cmdFish.ElapsedTime * 0.001f;
                        if (cmdFish.DelayType != (byte)FISH_DELAY_TYPE.DELAY_NONE)
                        {
                            float scl;
                            float[] dur = new float[3];
                            Utility.ReductionToFloat(cmdFish.DelayScaling, cmdFish.DelayDuration1, cmdFish.DelayDuration2, cmdFish.DelayDuration3,
                                out scl, dur);
                            float time = cmdFish.DelayCurrentTime * 0.001f;
                            fish.Controller.TimeController.AddSkillTimeScaling(scl, dur, (FISH_DELAY_TYPE)cmdFish.DelayType, time);
                        }
                    }
                }
            }
        }
    }
    public void LaunchFish(NetCmdPack pack)
    {
        NetCmdFish cmdFish = (NetCmdFish)pack.cmd;
        GroupDataList gdl = FishResManager.Instance.GetFishGroup(cmdFish.GroupID);
        ushort startID = cmdFish.StartID;
        float elapsedTime = Utility.TickSpan(pack.tick) + SceneRuntime.NetDelayTime;
        if (gdl.PathGroupData != null)
        {
            FishPathGroupData pathgroup = gdl.PathGroupData;
            PathLinearInterpolator[] interpList = PathManager.Instance.GetPathGroup(pathgroup.PathGroupIndex, SceneRuntime.Inversion);
            foreach (PathLinearInterpolator interp in interpList)
            {
                Fish fish = new Fish();
                fish.Init(startID, pathgroup.FishIndex, pathgroup.FishScaling, 0, pathgroup.ActionSpeed, pathgroup.ActionUnite, pathgroup.Speed, interp);
#if UNITY_EDITOR
                fish.SetModelName("Fish_PathGroup_" + cmdFish.GroupID);
#endif
                if (fish.AddElapsedTime(elapsedTime))
                {
                    SetFish(fish);
                }
                else
                    fish.Destroy();

                if (++startID >= FishSetting.FISH_MAX_NUM)
                    startID = 0;
            }
        }
        else
        {
            float fInv = SceneRuntime.Inversion ? -1.0f : 1.0f;
            int pathIndex = cmdFish.PathID;
            PathLinearInterpolator pi = PathManager.Instance.GetPath(pathIndex, SceneRuntime.Inversion);
            float startX = gdl.FrontPosition.x;
            foreach (GroupData gd in gdl.GroupDataArray)
            {
                if (gd == null)
                    break;
                if(gd.FishNum > gd.PosList.Length)
                {
                    LogMgr.Log("错误的鱼群路径点:" + gd.FishNum + ", posnum:" + gd.PosList.Length);
                    return;
                }
                for (int i = 0; i < gd.FishNum; ++i)
                {
                    float time = Utility.GetPathTimeByDist(startX, gd.PosList[i].x, pi);
                    Fish fish = new Fish();
                    fish.Init(startID, gd.FishIndex, gd.FishScaling, time, gd.ActionSpeed, gd.ActionUnite, gd.SpeedScaling, pi);
#if UNITY_EDITOR
                    fish.SetModelName("Fish_FishGroup_" + cmdFish.GroupID+"_Path_" + pathIndex);
#endif
                    if (fish.AddElapsedTime(elapsedTime))
                    {
                        fish.SetOffset(new Vector3(0, fInv * gd.PosList[i].y, gd.PosList[i].z));
                        SetFish(fish);
                    }
                    else
                        fish.Destroy();
                    if (++startID == FishSetting.FISH_MAX_NUM)
                        startID = 0;
                }
            }
        }
    }
    void UpdateBackupList(float delta)
    {
        if (m_BackFishList != null)
        {
            int n = 0;
            for (int i = 0; i < m_BackFishList.Length; ++i)
            {
                if (m_BackFishList[i] == null)
                    continue;
                if (!m_BackFishList[i].Update(delta))
                {
                    m_BackFishList[i].Destroy();
                    m_BackFishList[i] = null;
                }
                else
                { 
                    ++n;
                }
            }
            if(n == 0)
                m_BackFishList = null;
        }
    }
    public void Update(float delta)
    {
        m_FishNum = 0;
        List<Fish> fishList = new List<Fish>(m_FishList.Values);
        for (int i = 0; i < fishList.Count; ++i)
        {
            Fish fish = fishList[i];
            if (!fish.Update(delta))
            {
                DestroyFish(fish, fish.Catched);
            }
            else
            {
                ++m_FishNum;
            }
        }
        UpdateBackupList(delta);
        //CheckFishPos();
    }
    public void ClearAllFish()
    {
        foreach (Fish fish in m_FishList.Values)
            fish.Destroy();
        m_FishList.Clear();
    }
    public void BackupFishList()
    {
        if(m_BackFishList != null)
        {
            for (int i = 0; i < m_BackFishList.Length; ++i)
            {
                if (m_BackFishList[i] == null)
                    continue;
                m_BackFishList[i].Destroy();
                m_BackFishList[i] = null;
            }
            m_BackFishList = null;
        }

        m_BackFishList = new Fish[m_FishList.Count];
        int idx = 0;
        foreach(Fish fish in m_FishList.Values)
            m_BackFishList[idx++] = fish;
        m_FishList.Clear();
    }
   
    public void DestroyFish(Fish fish, bool catched)
    {
        DestroyFishData dd;
        dd.FishType = fish.FishType;
        m_DestroyFishList[fish.FishID] = dd;
        m_FishList.Remove(fish.FishID);
        fish.Destroy();
    }
    public Fish FindFishByID(ushort fishid)
    {
        Fish fish = null;
        m_FishList.TryGetValue(fishid, out fish);
        return fish;
    }
    public CatchFishData FindFish(ushort fishid)
    {
        CatchFishData cfd;
        if(m_FishList.TryGetValue(fishid, out cfd.FishObj))
        {
            cfd.FishType = cfd.FishObj.FishType;
        }
        else
        {
            DestroyFishData dfd;
            if (m_DestroyFishList.TryGetValue(fishid, out dfd))
            {
                cfd.FishType = dfd.FishType;
               // Debug.Log("鱼已经被销毁了。");
            }
            else
            {
                LogMgr.Log("未找到鱼ID:" + fishid);
                cfd.FishType = ConstValue.INVALID_FISH_TYPE;
            }
        }
        return cfd;
    }
    public void Shutdown()
    {
        if (m_BackFishList != null)
        {
            for (int i = 0; i < m_BackFishList.Length; ++i)
            {
                if (m_BackFishList[i] == null)
                    continue;
                m_BackFishList[i].Destroy();
                m_BackFishList[i] = null;
            }
        }
        foreach(Fish f in m_FishList.Values)
            f.Destroy();
        m_FishList.Clear();
    }
    public void SetFishPackage(NetCmdPack pack)
    {
        NetCmdFishPackage p = (NetCmdFishPackage)pack.cmd;
        CatchFishData cfd = FindFish(p.FishID);
        if(cfd.FishObj != null)
        {
            cfd.FishObj.SetPackage(p.PackageType);
        }
    }
    public Fish[] BackFishList
    {
        get { return m_BackFishList; }
    }
    public ushort FishNum
    {
        get { return m_FishNum; }
    }
    
    //获取场景中价值最大的鱼
    public ushort GetFishBySortGold(Vector3 scrStartPos, Vector3 viewStartPos)
    {
        uint gold = 0;
        ushort fishID = 0;
        float dist = float.MaxValue;
        Dictionary<ushort, Fish>.ValueCollection fishlist = m_FishList.Values;
        foreach (Fish fish in fishlist)
        {
            if(fish.IsDelay || fish.Catched || fish.FishType >= FishSetting.FishDataList.Length || FishSetting.FishDataList[fish.FishType] == null)
                continue;
            if (Utility.IsInScreen(fish.ScreenPos) == false || fish.Position.z < ConstValue.NEAR_Z || fish.Position.z > ConstValue.FAR_Z)
                continue;
            if (!Utility.CheckLauncherAngle(fish, scrStartPos, viewStartPos))
                continue;
            uint fishGold = FishSetting.FishDataList[fish.FishType].Gold;
            if (fishGold > gold)
            {
                gold = fishGold;
                fishID = fish.FishID;
            }
            else if(fishGold == gold)
            {
                float curDist = (fish.ScreenPos - scrStartPos).sqrMagnitude;
                if(curDist < dist)
                {
                    dist = curDist;
                    fishID = fish.FishID;
                }
            }
        }
        return fishID;
    }
    public bool GetFishScreenPos(ushort fishID, out Vector2 ScreenPos)
    {
        Fish fish = FindFishByID(fishID);

        if (fish == null)
        {
            ScreenPos = Vector2.zero;
            return false;
        }
        ScreenPos.x = fish.ScreenPos.x;
        ScreenPos.y = fish.ScreenPos.y;
        return true;
    }
    //获取鱼的ID，0表示没有找到，通过屏幕点
    public ushort GetFishIDByScreenPoint(Vector3 scrPos)
    {
        Ray ray = Camera.main.ScreenPointToRay(scrPos);
        RaycastHit hit;
        //射线检测，相关检测信息保存到RaycastHit 结构中  
        if (Physics.Raycast(ray, out hit, float.MaxValue, 1 << LayerMask.NameToLayer("FishLayer")))
        {
            return ushort.Parse(hit.collider.gameObject.name);
        }  
        return 0;
    }
    int SendInterval = 0;
    public void CheckFishPos()
    {
        if (m_FishNum == 0 || ++SendInterval < 120)
            return;
        SendInterval = 0;

        List<CheckFishPos> list = new List<CheckFishPos>();
        foreach(Fish fish in m_FishList.Values)
        {
            if (fish.Delay)
                continue;
            CheckFishPos ncf = new CheckFishPos();
            ncf.FishID = fish.FishID;
            ncf.Time = fish.Time;
            ncf.Pos = Utility.ToNetCmdVec3(fish.Position);
            ncf.Rot = Utility.ToNetCmdVec4(fish.Rotation);
            ncf.Speed = fish.Speed;
            list.Add(ncf);

            if (list.Count > 50)
            {
                NetCmdCheckFishPos cmd = new NetCmdCheckFishPos();
                cmd.Fish = list.ToArray();
                cmd.FishNum = (ushort)cmd.Fish.Length;
                cmd.SetCmdType(NetCmdType.CMD_CHECK_FISH_POS);
                SceneRuntime.Send<NetCmdCheckFishPos>(cmd);
                list.Clear();
            }
        }
        {
            NetCmdCheckFishPos cmd = new NetCmdCheckFishPos();
            cmd.Fish = list.ToArray();
            cmd.FishNum = (ushort)cmd.Fish.Length;
            cmd.SetCmdType(NetCmdType.CMD_CHECK_FISH_POS);
            SceneRuntime.Send<NetCmdCheckFishPos>(cmd);
        }
    }
   
}
