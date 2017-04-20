using UnityEngine;
using System.Collections.Generic;

class CatchLithingData
{
    public Fish MainFish;
    public List<Fish> SubFish = new List<Fish>();
}
class LightingMoveData
{
    public Transform Trans;
    public Vector3 StartPos;
    public Vector3 EndPos;
    public float Delta;
}
class CatchLithingList
{
    public float DelayTime = 0;
    public Dictionary<ushort, CatchLithingData> LithingList = new Dictionary<ushort, CatchLithingData>();
    public void AddMainFish(Fish mainFish)
    {
        if(LithingList.ContainsKey(mainFish.FishID) == false)
        {
            CatchLithingData cbd = new CatchLithingData();
            cbd.MainFish = mainFish;
            LithingList.Add(mainFish.FishID, cbd);
        }
    }
    public void AddSubFish(Fish mainFish, Fish subFish)
    {
        CatchLithingData cbd;
        if(LithingList.TryGetValue(mainFish.FishID, out cbd))
        {
            cbd.SubFish.Add(subFish);
        }
        else
        {
            cbd = new CatchLithingData();
            cbd.MainFish = mainFish;
            cbd.SubFish.Add(subFish);
            LithingList.Add(mainFish.FishID, cbd);
        }
    }
}
public class SceneSkillMgr
{
   
    Vector3[,] m_Pos = new Vector3[3, 5];
    List<LightingMoveData> m_MoveList = new List<LightingMoveData>();
    List<CatchLithingList> m_LightinData = new List<CatchLithingList>();
    void RecordSkill(SkillType index,byte byseat)
    {
        SceneRuntime.LogicUI.GetSkillBar.RecordUsed(index, byseat);
    }
    public void Init()
    {
        InitTornadoPos();
    }
    void InitTornadoPos()
    {
        Vector2[,] Posssion = new Vector2[3, 5];
        Vector3[] pos = new Vector3[3];
        float radius = 40.0f;

        pos[0] = new Vector3(-13, -62.0f, 1132);
        pos[1] = new Vector3(-173.0f, -102.0f, 900);
        pos[2] = new Vector3(128.0f, -98.0f, 948);

        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                m_Pos[j, k].x = pos[j].x + radius * Posssion[j, k].x;
                m_Pos[j, k].y = pos[j].y + radius * Posssion[j, k].y;
                m_Pos[j, k].z = pos[j].z;
            }
        }
    }
    public void GetTornadoPos(Vector3[] pos)
    {
        pos[0] = m_Pos[0, Utility.Range(0, 5)];
        pos[1] = m_Pos[1, Utility.Range(0, 5)];
        pos[2] = m_Pos[2, Utility.Range(0, 5)];
    }
    public void Update(float delta)
    {
        for(int i = 0; i < m_MoveList.Count; )
        {
            LightingMoveData lmd = m_MoveList[i];
            lmd.Delta += delta;
            float t = Mathf.Min(lmd.Delta / 0.3f, 1.0f);
            lmd.Trans.localPosition = Vector3.Lerp(lmd.StartPos, lmd.EndPos, t);
            if (t >= 1.0f)
            {
                Utility.ListRemoveAt(m_MoveList, i);
                continue;
            }
            else
                ++i;
        }
        for(int i = 0; i < m_LightinData.Count;)
        {
            CatchLithingList cll = m_LightinData[i];
            cll.DelayTime -= delta;
            if (cll.DelayTime <= 0)
            {
                ProcessLightingData(cll);
                Utility.ListRemoveAt(m_LightinData, i);
            }
            else
                ++i;
        }
    }
    public void Shutdown()
    {

    }
    public void UseSkillTornado(NetCmdPack pack)
    {
        NetCmdSkillTornado cmd = (NetCmdSkillTornado)pack.cmd;
        bool bNotDelay = Utility.GetTickCount() - pack.tick < ConstValue.FISH_OVER_TIME;
        byte clientSeat = SceneRuntime.ServerToClientSeat(cmd.Seat);
        if (SceneRuntime.PlayerMgr.GetPlayer(clientSeat) == null)
            return;
        CatchedData cd  = new CatchedData();
        cd.CatchType    = (byte)CatchedType.CATCHED_SKILL;
        cd.SubType      = (byte)SkillType.SKILL_TORNADO;
        cd.FishList     = new List<CatchFishData>();
        cd.ClientSeat   = clientSeat;
        cd.RateIndex    = SceneRuntime.PlayerMgr.GetPlayer(cd.ClientSeat).RateIndex;

        RecordSkill(SkillType.SKILL_TORNADO, clientSeat);
        if(bNotDelay == false)
        {
            ProcessDelayOver(cmd.fishID, cd);
            return;
        }

        Vector3[] pos = new Vector3[3];
        SceneRuntime.PlayerMgr.PlayCD(SkillSetting.SkillDataList[(byte)SkillType.SKILL_TORNADO].CDTime);
        SceneRuntime.SceneLogic.LogicUI.PlayCD(SkillSetting.SkillDataList[(byte)SkillType.SKILL_TORNADO].CDTime, SkillType.SKILL_TORNADO);
        GetTornadoPos(pos);
        GameObject effect = SceneRuntime.EffectMgr.GetSkillEffect(1);
        effect.transform.localPosition = new Vector3(0, 0, 500);
        GlobalEffectData gfd = new GlobalEffectData(effect, 0, 5.0f);
        GlobalEffectMgr.Instance.AddEffect(gfd);
        SceneObjMgr.Instance.PlayBack(BackAnimType.BACK_ANIM_JF);

        for (int i = 0; i < 3; ++i)
        {
            effect = SceneRuntime.EffectMgr.GetSkillEffect(0);
            Vector3 p = pos[i];
            p.y -= 50;
            effect.transform.localPosition = pos[i];
            gfd = new GlobalEffectData(effect, 0, 5.0f);
            GlobalEffectMgr.Instance.AddEffect(gfd);
        }
        SceneRuntime.PlayerMgr.PlaySkillAvatarEft(clientSeat);
        if(cmd.fishID != null)
        for (int i = 0; i < cmd.fishID.Length; ++i)
        {
            CatchFishData cfd = SceneRuntime.FishMgr.FindFish(cmd.fishID[i].FishID);
            if (cfd.FishObj != null)
            {
                Fish fish = cfd.FishObj;
                fish.StopLaugh(false);
                float d1 = new Vector2(fish.Position.x - pos[0].x, fish.Position.y - pos[0].y).sqrMagnitude;
                float d2 = new Vector2(fish.Position.x - pos[1].x, fish.Position.y - pos[1].y).sqrMagnitude;
                float d3 = new Vector2(fish.Position.x - pos[2].x, fish.Position.y - pos[2].y).sqrMagnitude;
                int posidx = (d1 < d2) ? (d1 < d3 ? 0 : 2) : (d2 < d3 ? 1 : 2);

                float transRot = -Utility.Range(1000, 2000);
                float pathRot = -Utility.Range(3000, 5000);
                float speed = Utility.Range(4, 8);
                float transTime = Utility.Range(0.3f, 0.6f);
                int idx = Utility.Range(0, PathManager.Instance.LongJuanFeng.Length);

                float delay = Utility.LerpFloat(FishSetting.FishDeadTime.LongJuanFeng_Dead_Time1, FishSetting.FishDeadTime.LongJuanFeng_Dead_Time2, Utility.Range(0.0f, 1.0f));
                ReductionData rd = new ReductionData();
                rd.Speed = 0;
                rd.Duration1 = delay;
                FishOptReduction f = new FishOptReduction(0.0f, rd);
                FishOptPath fop = new FishOptPath(PathManager.Instance.LongJuanFeng[idx], transRot, pathRot, speed, transTime, pos[posidx] + new Vector3(0, 50, 0));
                fop.SetDeadData(delay, false, cd);
                fish.ClearOpt();
                fish.AddOpt(fop);
                fish.SetCatched(clientSeat);
                cd.FishList.Add(cfd);

                fish.SetDropReward(cmd.fishID[i].nReward);
            }
            else if (cfd.IsValidFishType())
            {
               // SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
                byte multiple = (byte)SkillSetting.SkillDataList[(byte)SkillType.SKILL_TORNADO].multiple;
                SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, multiple) );

                cd.FishList.Add(cfd);
            }
        } 
        SceneRuntime.ComputeGoldNum(cd);
        SceneRuntime.SceneLogic.CatchFish(cd);
        //SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, cd.GoldNum);
        //if (clientSeat == SceneRuntime.MyClientSeat)
        //{
        //    if (cd.GoldNum != cmd.GoldNum)
        //    {
        //        LogMgr.Log("龙卷风:金币不相等:" + cd.GoldNum + "," + cmd.GoldNum);
        //    }
        //    else if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() != cmd.TotalNum)
        //    {
        //        LogMgr.Log("龙卷风:总金币不相等:" + (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel()) + "," + cmd.TotalNum);
        //    }
        //}
    }
    public void UseSkillLighting(NetCmdPack pack)
    {
        NetCmdSkillLighting cmd = (NetCmdSkillLighting)pack.cmd;
        bool bNotDelay = Utility.GetTickCount() - pack.tick < ConstValue.FISH_OVER_TIME;
        byte clientSeat = SceneRuntime.ServerToClientSeat(cmd.Seat);
        if (SceneRuntime.PlayerMgr.GetPlayer(clientSeat) == null)
            return;
        CatchedData cd = new CatchedData();
        cd.CatchType = (byte)CatchedType.CATCHED_SKILL;
        cd.SubType = (byte)SkillType.SKILL_LIGHTING;
        cd.FishList = new List<CatchFishData>();
        cd.ClientSeat = clientSeat;
        cd.RateIndex = SceneRuntime.PlayerMgr.GetPlayer(cd.ClientSeat).RateIndex;

        RecordSkill(SkillType.SKILL_LIGHTING, clientSeat);
        if (bNotDelay == false)
        {
            ProcessDelayOver(cmd.FishID, cd);
            return;
        }

        SceneRuntime.PlayerMgr.PlayCD(SkillSetting.SkillDataList[(byte)SkillType.SKILL_LIGHTING].CDTime);
        SceneRuntime.SceneLogic.LogicUI.PlayCD(SkillSetting.SkillDataList[(byte)SkillType.SKILL_LIGHTING].CDTime, SkillType.SKILL_LIGHTING);
        GameObject effect = SceneRuntime.EffectMgr.GetSkillEffect(5);
        effect.transform.localPosition = new Vector3(0, 21.7f, 150);
        GlobalEffectData gfd = new GlobalEffectData(effect, 0, 3.0f);
        GlobalEffectMgr.Instance.AddEffect(gfd);
        SceneObjMgr.Instance.PlayBack(BackAnimType.BACK_ANIM_SD);
        SceneRuntime.PlayerMgr.PlaySkillAvatarEft(clientSeat);
        CatchLithingList cll = null;
        float lightDuration2 = 1.5f;

        if(cmd.FishID != null)
        for (int i = 0; i < cmd.FishID.Length; ++i)
        {
            NetFishDeadTime dt = cmd.FishID[i];
            CatchFishData cfd = SceneRuntime.FishMgr.FindFish(dt.FishID);
            if (cfd.FishObj != null)
            {
                Fish fish = cfd.FishObj;
                fish.StopLaugh(false);
                if (dt.LightingFishID != 0)
                {
                    if (cll == null)
                        cll = new CatchLithingList();
                    CatchFishData cfd2 = SceneRuntime.FishMgr.FindFish(dt.LightingFishID);
                    if (cfd2.FishObj != null)
                        cll.AddSubFish(cfd2.FishObj, fish);
                    lightDuration2 = 2.0f;
                }
                else
                    lightDuration2 = 1.5f;
                float t = dt.DeadTime * ConstValue.INV255;
                ReductionData rd = new ReductionData();
                rd.Speed = 0;
                rd.Duration1 = Utility.LerpFloat(FishSetting.FishDeadTime.ShanDian_Dead_Time1, FishSetting.FishDeadTime.ShanDian_Dead_Time2, t);
                rd.Duration2 = lightDuration2;
                rd.Duration3 = 0;

                BlendData bd = new BlendData();
                bd.EffectTex = SceneRuntime.EffectMgr.GetLightingTex(fish.FishType);
                bd.Blend_Type = (byte)BlendType.BLEND_LERP_TEX;
                bd.Duration1 = 0.25f;
                bd.Duration2 = 2.0f;
                bd.Factor = 1.0f;
                fish.ClearOpt();

                FishOptReduction fr = new FishOptReduction(0.0f, rd, rd.Duration1, bd);
                fr.DeadType = FishDeadType.DEAD_IMMEDIATE;
                fr.CatchData = cd;
                fish.SetCatched(clientSeat);
                fish.AddOpt(fr);
                fish.SetDropReward(dt.nReward);

                effect = SceneRuntime.EffectMgr.GetSkillEffect(4);
                gfd = new GlobalEffectData(fish, effect, rd.Duration1, lightDuration2, GlobalEffectPosConverter.LightingPosConvert);
                GlobalEffectMgr.Instance.AddEffect(gfd);
                cd.FishList.Add(cfd);

                
            }
            else if (cfd.IsValidFishType())
            {
                byte multiple = (byte)SkillSetting.SkillDataList[(byte)SkillType.SKILL_TORNADO].multiple;
                SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, multiple));
                //SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
                cd.FishList.Add(cfd);
            }
        }
        SceneRuntime.ComputeGoldNum(cd);
        SceneRuntime.SceneLogic.CatchFish(cd);
        if (cll != null)
            ProcessLightingData(cll);
        //SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, cd.GoldNum);
        //if (clientSeat == SceneRuntime.MyClientSeat)
        //{
        //    if (cd.GoldNum != cmd.GoldNum)
        //    {
        //        LogMgr.Log("闪电:金币不相等:" + cd.GoldNum + "," + cmd.GoldNum);
        //    }
        //    else if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() != cmd.TotalNum)
        //    {
        //        LogMgr.Log("闪电:总金币不相等:" + (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel()) + "," + cmd.TotalNum);
        //    }
        //}
    }
    public void UseSkillFreeze(NetCmdPack pack)
    {
        bool bNotDelay = Utility.GetTickCount() - pack.tick < ConstValue.FISH_OVER_TIME;
        NetCmdSkillFreeze cmd = (NetCmdSkillFreeze)pack.cmd;
        byte clientSeat = SceneRuntime.ServerToClientSeat(cmd.Seat);
        if (SceneRuntime.PlayerMgr.GetPlayer(clientSeat) == null)
            return;
        

        CatchedData cd = new CatchedData();
        cd.CatchType = (byte)CatchedType.CATCHED_SKILL;
        cd.SubType = (byte)SkillType.SKILL_FREEZE;
        cd.FishList = new List<CatchFishData>();
        cd.ClientSeat = clientSeat;
        cd.RateIndex = SceneRuntime.PlayerMgr.GetPlayer(cd.ClientSeat).RateIndex;

        RecordSkill(SkillType.SKILL_FREEZE, clientSeat);
        if (bNotDelay == false)
        {
            ProcessDelayOverFreeze(cmd.FishID, cd);
            return;
        }
        CatchLithingList cll = null;
        GameObject effect = SceneRuntime.EffectMgr.GetSkillEffect(2);
        effect.transform.localPosition = new Vector3(-17, -18.9f, 100);
        GlobalEffectData gfd = new GlobalEffectData(effect, 0, 5.0f);
        SceneRuntime.PlayerMgr.PlayCD(SkillSetting.SkillDataList[(byte)SkillType.SKILL_FREEZE].CDTime);
        SceneRuntime.SceneLogic.LogicUI.PlayCD(SkillSetting.SkillDataList[(byte)SkillType.SKILL_FREEZE].CDTime, SkillType.SKILL_FREEZE);
        GlobalEffectMgr.Instance.AddEffect(gfd);
        SceneObjMgr.Instance.PlayBack(BackAnimType.BACK_ANIM_BD);
        SceneRuntime.PlayerMgr.PlaySkillAvatarEft(clientSeat);

        ReductionData reduction = SkillSetting.SkillDataList[(int)SkillType.SKILL_FREEZE].Reduction;

        BlendData bd = new BlendData();
        bd.BaseTex = SceneRuntime.EffectMgr.SkillFreezeBaseTex;
        bd.EffectTex = SceneRuntime.EffectMgr.SkillFreezeEffectTex;
        bd.Blend_Type = (byte)BlendType.BLEND_ADD_TEX;
        bd.Duration1 = 0;
        bd.Duration2 = 2.0f;
        bd.Factor = 0.5f;

        BlendData bd2 = new BlendData();
        bd2.EffectTex = SceneRuntime.EffectMgr.SkillFreezeBaseTex;
        bd2.Blend_Type = (byte)BlendType.BLEND_ADD_TEX;
        bd2.Duration1 = reduction.Duration1;
        bd2.Duration2 = reduction.Duration2;
        bd2.Duration3 = reduction.Duration3;
        bd2.Factor = 0.8f;
        if (cmd.FishID != null)
        for (int i = 0; i < cmd.FishID.Length; ++i)
        {
            NetFishDeadTime dt = cmd.FishID[i];
            CatchFishData cfd = SceneRuntime.FishMgr.FindFish(dt.FishID);
            bool dead = dt.DeadTime != 0;
            if (cfd.FishObj != null)
            {
                Fish fish = cfd.FishObj;
                float t = dt.DeadTime * ConstValue.INV255;
                FishOptReduction fr;
                if (dead)
                {
                    if (dt.LightingFishID != 0)
                    {
                        if (cll == null)
                            cll = new CatchLithingList();
                        CatchFishData cfd2 = SceneRuntime.FishMgr.FindFish(dt.LightingFishID);
                        if (cfd2.FishObj != null)
                            cll.AddSubFish(cfd2.FishObj, fish);
                    }
                   
                    fish.Controller.PathEvent.Reset(0, false);
                    ReductionData rd = new ReductionData();
                    rd.Speed = FishSetting.FishDeadTime.BingDong_Speed;
                    rd.Duration2 = Utility.LerpFloat(FishSetting.FishDeadTime.BingDong_Dead_Time1, FishSetting.FishDeadTime.BingDong_Dead_Time2, t);
                    fr = new FishOptReduction(rd.Speed, rd, 0, bd);
                    fr.DeadType = FishDeadType.DEAD_IMMEDIATE;
                    fr.CatchData = cd;

                    effect = SceneRuntime.EffectMgr.GetSkillEffect(3);
                    Vector3 pos = fish.Position;
                    pos.z -= 20;
                    effect.transform.localPosition = pos;
                    gfd = new GlobalEffectData(effect, rd.Duration2, 1.0f);
                    gfd.Scaling = fish.Scaling * Mathf.Max(1.0f, (pos.z / 1000)) * 0.2f;
                    fish.SetCatched(clientSeat);
                    GlobalEffectMgr.Instance.AddEffect(gfd);
                    fish.ClearOpt();
                    fish.AddOpt(fr);
                    cd.FishList.Add(cfd);
                    fish.SetDropReward(dt.nReward);
                }
                else
                {
                    fr = new FishOptReduction(reduction.Speed, reduction, 0, bd2);
                    fish.ClearOpt();
                    fish.AddOpt(fr);
                }
            }
            else if(dead && cfd.IsValidFishType())
            {
               // SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
                byte multiple = (byte)SkillSetting.SkillDataList[(byte)SkillType.SKILL_FREEZE].multiple;
                SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, multiple));

                cd.FishList.Add(cfd);
            }
        }
        SceneRuntime.ComputeGoldNum(cd);
        SceneRuntime.SceneLogic.CatchFish(cd);
        if (cll != null)
            ProcessLightingData(cll);
        //SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, cd.GoldNum);
        //if (clientSeat == SceneRuntime.MyClientSeat)
        //{
        //    if (cd.GoldNum != cmd.GoldNum)
        //    {
        //        LogMgr.Log("冰冻:金币不相等:" + cd.GoldNum + "," + cmd.GoldNum);
        //    }
        //    else if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() != cmd.TotalNum)
        //    {
        //        LogMgr.Log("冰冻:总金币不相等:" + (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel()) + "," + cmd.TotalNum);
        //    }
        //}
    }
    public void UseSkillLock(NetCmdPack pack)
    {
        NetCmdSkillLock cmd = (NetCmdSkillLock)pack.cmd;
        //客户端可以使用锁定技能
        //...
        byte clientSeat = SceneRuntime.ServerToClientSeat(cmd.Seat);
        if (clientSeat == SceneRuntime.PlayerMgr.MyClientSeat)
        {
            SceneRuntime.LogicUI.PlayCD(SkillSetting.SkillDataList[(byte)SkillType.SKILL_LOCK].CDTime, SkillType.SKILL_LOCK);
            SceneRuntime.PlayerMgr.MySelf.Launcher.OnClickAutoShotCancel(null);
            SceneRuntime.PlayerMgr.SetLocked(true);
        }
        else
        {
            SceneRuntime.PlayerMgr.ShowOtherUserLocked(clientSeat);
        }

    }
    public void UseSkillDisaster(NetCmdPack pack)
    {
        NetCmdSkillDisaster cmd = (NetCmdSkillDisaster)pack.cmd;
        bool bNotDelay = Utility.GetTickCount() - pack.tick < ConstValue.FISH_OVER_TIME;
        byte clientSeat = SceneRuntime.ServerToClientSeat(cmd.Seat);
        if (SceneRuntime.PlayerMgr.GetPlayer(clientSeat) == null)
            return;

        CatchedData cd = new CatchedData();
        cd.CatchType = (byte)CatchedType.CATCHED_SKILL;
        cd.SubType = (byte)SkillType.SKILL_DISASTER;
        cd.FishList = new List<CatchFishData>();
        cd.ClientSeat = clientSeat;
        cd.RateIndex = SceneRuntime.PlayerMgr.GetPlayer(cd.ClientSeat).RateIndex;

        RecordSkill(SkillType.SKILL_DISASTER, clientSeat);
        if(bNotDelay == false)
        {
            ProcessDelayOver(cmd.FishID, cd);
            return;
        }
        CatchLithingList cll = null;
        SceneRuntime.PlayerMgr.PlayCD(SkillSetting.SkillDataList[(byte)SkillType.SKILL_DISASTER].CDTime);
        SceneRuntime.SceneLogic.LogicUI.PlayCD(SkillSetting.SkillDataList[(byte)SkillType.SKILL_DISASTER].CDTime, SkillType.SKILL_DISASTER);
        GameObject effect = SceneRuntime.EffectMgr.GetSkillEffect(6);
        GlobalEffectData gfd = new GlobalEffectData(effect, 0, 5.0f);
        GlobalEffectMgr.Instance.AddEffect(gfd);
        SceneObjMgr.Instance.PlayBack(BackAnimType.BACK_ANIM_TZ);
        SceneRuntime.PlayerMgr.PlaySkillAvatarEft(clientSeat);

        BlendData bd = new BlendData();
        bd.BaseTex = SceneRuntime.EffectMgr.SkillDisasterTex;
        bd.EffectTex = bd.BaseTex;
        bd.Blend_Type = (byte)BlendType.BLEND_ADD_TEX;
        bd.Duration1 = 1.0f;
        bd.Duration2 = 2.0f;
        bd.Factor = 3.0f;

        FishOptPath pt = new FishOptPath(PathManager.Instance.DouDongPath, 0, 0, 1, 0, Vector3.zero);
        if (cmd.FishID != null)
        for (int i = 0; i < cmd.FishID.Length; ++i)
        {
            NetFishDeadTime dt = cmd.FishID[i];
            CatchFishData cfd = SceneRuntime.FishMgr.FindFish(dt.FishID);
            if (cfd.FishObj != null)
            {
                Fish fish = cfd.FishObj;
                fish.StopLaugh(false);
                float t = dt.DeadTime * ConstValue.INV255;
                ReductionData rd = new ReductionData();
                
                float dt1 = Utility.LerpFloat(FishSetting.FishDeadTime.TianZai_Dead_Time1, FishSetting.FishDeadTime.TianZai_Dead_Time1, t);
                float dt2 = Utility.LerpFloat(FishSetting.FishDeadTime.TianZai_Stay_Time1, FishSetting.FishDeadTime.TianZai_Stay_Time2, t);
                rd.Duration1 = dt1 + FishSetting.FishDeadTime.TianZai_DouDong_Time;

                if (dt.LightingFishID != 0)
                {
                    if (cll == null)
                        cll = new CatchLithingList();
                    
                    CatchFishData cfd2 = SceneRuntime.FishMgr.FindFish(dt.LightingFishID);
                    if (cfd2.FishObj != null)
                    {
                        cll.AddSubFish(cfd2.FishObj, fish);
                    }
                }

                rd.Duration2 = dt2;
                rd.Speed = FishSetting.FishDeadTime.TianZai_Speed;
                fish.ClearOpt();
                FishOptOffset foo = new FishOptOffset(FishSetting.FishDeadTime.TianZai_DouDong_Time, PathManager.Instance.DouDongPath);
                foo.Delay = dt1;
                fish.AddOpt(foo);

                FishOptReduction fr = new FishOptReduction(0, rd, rd.Duration1, bd);
                fr.DeadType = FishDeadType.DEAD_ACTION;
                fr.CatchData = cd;
                fish.SetCatched(clientSeat);
                fish.AddOpt(fr);
                fish.SetDropReward(dt.nReward);

                cd.FishList.Add(cfd);
            }
            else if(cfd.IsValidFishType())
            {
               // SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
                byte multiple = (byte)SkillSetting.SkillDataList[(byte)SkillType.SKILL_DISASTER].multiple;
                SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, multiple));
                cd.FishList.Add(cfd);
            }
        }
        SceneRuntime.ComputeGoldNum(cd);
        SceneRuntime.SceneLogic.CatchFish(cd);
        if(cll != null)
        {
            cll.DelayTime = 1.0f;
            m_LightinData.Add(cll);
            //ProcessLightingData(cll);
        }
        //SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, cd.GoldNum);
        //if (clientSeat == SceneRuntime.MyClientSeat)
        //{
        //    if (cd.GoldNum != cmd.GoldNum)
        //    {
        //        LogMgr.Log("天灾:金币不相等:" + cd.GoldNum + "," + cmd.GoldNum);
        //    }
        //    else if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() != cmd.TotalNum)
        //    {
        //        LogMgr.Log("天灾:总金币不相等:" + (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel()) + "," + cmd.TotalNum);
        //    }
        //}
    }
    public void FishCatched(NetCmdPack pack)
    {
        NetCmdCatched cmd = (NetCmdCatched)pack.cmd;
        byte clientSeat;
        byte id;
        SceneRuntime.BuuletIDToSeat(cmd.BulletID, out clientSeat, out id);
        CatchBulletData cbd = SceneRuntime.BulletMgr.GetBullet(clientSeat, id);
        if (cbd.IsValid() == false)
            return;
        if (SceneRuntime.PlayerMgr.GetPlayer(clientSeat) == null)
            return;
        bool bNotDelay = Utility.GetTickCount() - pack.tick < ConstValue.FISH_OVER_TIME;
        CatchedData cd = new CatchedData();
        cd.CatchType = (byte)CatchedType.CATCHED_BULLET;
        cd.ClientSeat = clientSeat;
        cd.FishList = new List<CatchFishData>();
        ushort lockfishid = 0;
        if (cbd.BulletObj != null)
        {
            if(cbd.BulletObj.LauncherType == 4)
                lockfishid = cbd.BulletObj.LockedFishID;
            cd.SubType = cbd.BulletObj.LauncherType;
            cd.RateIndex = cbd.BulletObj.RateIndex;
        }
        else
        {
            cd.SubType = cbd.LauncherType;
            cd.RateIndex = cbd.RateIndex;
        }

        if(bNotDelay == false)
        {
            ProcessDelayOver(cmd.Fishs, cd);
            return;
        }

        CatchLithingList cll = null;
        BlendData bd = new BlendData(Color.red, 0.6f, 0, 0, 0.4f);
        if(cmd.Fishs != null)
        for (int i = 0; i < cmd.Fishs.Length; ++i)
        {
            NetFishCatched nfc = cmd.Fishs[i];
            CatchFishData cfd = SceneRuntime.FishMgr.FindFish(nfc.FishID);
            if(cfd.IsValidFishType() == false)
                continue;
            Fish fish = cfd.FishObj;
            if (fish != null && fish.FishID == lockfishid)
            {
                cbd.BulletObj.ClearLockFishID();//只有穿透子弹才需要清空锁定，其他子弹发生碰撞就销毁了。
            }
            switch ((FishCatchEventType)nfc.CatchEvent)
            {
                case FishCatchEventType.CATCH_EVENT_EFFECT:
                    if (fish == null || fish.Catched)
                        continue;

                    if (fish.HasOpt == false)
                    {
                        FishOptReduction ff = new FishOptReduction(1.0f, new ReductionData(1.0f, 0, 0, 0.5f), 0, bd);
                        fish.AddOpt(ff);
                    }
                    break;
                case FishCatchEventType.CATCH_EVENT_ATTACK:
                    if (fish == null || fish.Catched)
                        continue;
                    FishOptReduction ff1 = new FishOptReduction(0.5f, new ReductionData(0.5f, 0, 0, 0.5f), 0, bd);
                    fish.AddOpt(ff1);
                    if (FishResManager.Instance.GetFishData(fish.FishType).ClipLength[(int)FishClipType.CLIP_GONGJI] != 0)
                    {
                        FishOptAction foa = new FishOptAction(FishClipType.CLIP_GONGJI, 1.5f);
                        fish.AddOpt(foa);
                    }
                    break;
                case FishCatchEventType.CATCH_EVENT_CATCHED:
                    if (fish != null)
                    {
                        FishOptReduction deadRed = new FishOptReduction(1.0f, new ReductionData(0, 0, 1.0f, 0), 0, bd);
                        FishOptAction fod = new FishOptAction(FishClipType.CLIP_SIWANG, 1.0f);
                        fod.SetDeadData(0, false, cd);
                        fish.ClearOpt();
                        fish.AddOpt(fod);
                        fish.AddOpt(deadRed);
                        fish.SetCatched(clientSeat);

                        fish.SetDropReward(nfc.nReward);
                    }
                    else
                    {
                        SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
                    }
                    cd.FishList.Add(cfd);
                    break;
                case FishCatchEventType.CATCH_EVENT_CATCHED_LIGHTING_MAIN:
                    if (fish != null)
                    {
                        FishOptReduction deadRed = new FishOptReduction(1.0f, new ReductionData(0, 0, 2.0f, 0), 0, null);
                        FishOptAction fod = new FishOptAction(FishClipType.CLIP_SIWANG, 1.0f);
                        fod.SetDeadData(1, false, cd);
                        fish.ClearOpt();
                        fish.AddOpt(fod);
                        fish.AddOpt(deadRed);
                        fish.SetCatched(clientSeat);
                        fish.SetDropReward(nfc.nReward);
                        if (cll == null)
                            cll = new CatchLithingList();
                        cll.AddMainFish(fish);
                    }
                    else
                    {
                        SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
                    }
                    cd.FishList.Add(cfd);
                    break;
                case FishCatchEventType.CATCH_EVENT_CATCHED_LIGHTING:
                    if (fish != null)
                    {
                        FishOptReduction deadRed = new FishOptReduction(1.0f, new ReductionData(0, 0, 2.0f, 0), 0, null);
                        FishOptAction fod = new FishOptAction(FishClipType.CLIP_SIWANG, 1.0f);
                        fod.SetDeadData(1, false, cd);
                        fish.ClearOpt();
                        fish.AddOpt(fod);
                        fish.AddOpt(deadRed);
                        fish.SetCatched(clientSeat);
                        fish.SetDropReward(nfc.nReward);

                        CatchFishData cfd2 = SceneRuntime.FishMgr.FindFish(nfc.LightingFishID);
                        if (cfd2.IsValidFishType())
                        {
                            if (cll == null)
                                cll = new CatchLithingList();
                            cll.AddSubFish(cfd2.FishObj, fish);
                        }
                    }
                    else
                    {
                        SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
                    }
                    cd.FishList.Add(cfd);
                    break;
            }
        }
        if (cd.FishList.Count > 0)
        {
            SceneRuntime.ComputeGoldNum(cd);
            SceneRuntime.SceneLogic.CatchFish(cd);
        }

        SceneRuntime.PlayerMgr.FishCatch(clientSeat, cmd.Combo);
        if (cbd.BulletObj != null)
        {
            cbd.BulletObj.GoldNum += cd.GoldNum;
            SceneRuntime.EffectMgr.PlayFishNet(cbd.BulletObj.Position, cd.SubType);
        }
        if (cd.SubType != 4)
            SceneRuntime.BulletMgr.RemoveBullet(clientSeat, id);

        if (cll != null)
            ProcessLightingData(cll);
        //SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, cd.GoldNum);
        //if (clientSeat == SceneRuntime.MyClientSeat)
        //{
        //    if (cd.GoldNum != cmd.GoldNum)
        //    {
        //        LogMgr.Log("子弹:金币不相等:" + cd.GoldNum + "," + cmd.GoldNum);
        //    }
        //    else if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() != cmd.TotalNum)
        //    {
        //        LogMgr.Log("子弹:总金币不相等:" + (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel()) + "," + cmd.TotalNum);
        //    }
        //}
    }
    public void FishCatchedByFreeze(NetCmdPack pack)
    {
        NetCmdBulletReduction cmd = (NetCmdBulletReduction)pack.cmd;
        byte clientSeat;
        byte id;
        SceneRuntime.BuuletIDToSeat(cmd.BulletID, out clientSeat, out id);
        CatchBulletData cbd = SceneRuntime.BulletMgr.GetBullet(clientSeat, id);
        if (cbd.IsValid() == false)
            return;
        if (SceneRuntime.PlayerMgr.GetPlayer(clientSeat) == null)
            return;
        CatchedData cd = new CatchedData();
        cd.CatchType = (byte)CatchedType.CATCHED_BULLET;
        cd.ClientSeat = clientSeat;
        cd.FishList = new List<CatchFishData>();
        if (cbd.BulletObj != null)
        {
            cd.SubType = cbd.BulletObj.LauncherType;
            cd.RateIndex = cbd.BulletObj.RateIndex;
        }
        else
        {
            cd.SubType = cbd.LauncherType;
            cd.RateIndex = cbd.RateIndex;
        }

        bool bNotDelay = Utility.GetTickCount() - pack.tick < ConstValue.FISH_OVER_TIME;
        if(bNotDelay == false)
        {
            ProcessDelayOver(cmd.FishID, cd);
            return;
        }
        CatchLithingList cll = null;
        BlendData bd = new BlendData();
        BlendData bd2 = new BlendData();
        ReductionData reduction = BulletSetting.FreezeBulletReduction;

        bd.BaseTex = SceneRuntime.EffectMgr.SkillFreezeBaseTex;
        bd.EffectTex = SceneRuntime.EffectMgr.SkillFreezeEffectTex;
        bd.Blend_Type = (byte)BlendType.BLEND_ADD_TEX;
        bd.Duration1 = 0;
        bd.Duration2 = 2.0f;
        bd.Factor = 0.5f;

        bd2.EffectTex = SceneRuntime.EffectMgr.SkillFreezeBaseTex;
        bd2.Blend_Type = (byte)BlendType.BLEND_ADD_TEX;
        bd2.Duration1 = reduction.Duration1;
        bd2.Duration2 = reduction.Duration2;
        bd2.Duration3 = reduction.Duration3;
        bd2.Factor = 1.0f;

        if (cmd.FishID != null)
        for (int i = 0; i < cmd.FishID.Length; ++i)
        {
            NetFishCatched nfc = cmd.FishID[i];
            CatchFishData cfd = SceneRuntime.FishMgr.FindFish(nfc.FishID);
            if (cfd.IsValidFishType() == false)
                continue;
            Fish fish = cfd.FishObj;
            FishOptReduction fr;
            switch ((FishCatchEventType)nfc.CatchEvent)
            {
                case FishCatchEventType.CATCH_EVENT_EFFECT:
                    continue;
                case FishCatchEventType.CATCH_EVENT_ATTACK:
                    if (fish == null || fish.Catched)
                        continue;
                    fr = new FishOptReduction(reduction.Speed, reduction, 0, bd2);
                    fish.AddOpt(fr);
                    break;
                case FishCatchEventType.CATCH_EVENT_CATCHED_LIGHTING:
                    if (fish != null)
                    {
                        FishOptReduction deadRed = new FishOptReduction(1.0f, new ReductionData(0, 0, 2.0f, 0), 0, null);
                        FishOptAction fod = new FishOptAction(FishClipType.CLIP_SIWANG, 1.0f);
                        fod.SetDeadData(1, false, cd);
                        fish.ClearOpt();
                        fish.AddOpt(fod);
                        fish.AddOpt(deadRed);
                        fish.SetCatched(clientSeat);
                        fish.SetDropReward(nfc.nReward);

                        CatchFishData cfd2 = SceneRuntime.FishMgr.FindFish(nfc.LightingFishID);
                        if (cfd2.IsValidFishType())
                        {
                            if (cll == null)
                                cll = new CatchLithingList();
                            cll.AddSubFish(cfd2.FishObj, fish);
                        }
                    }
                    else
                    {
                        SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
                    }
                    cd.FishList.Add(cfd);
                    break;
                case FishCatchEventType.CATCH_EVENT_CATCHED:
                    if (fish != null)
                    {
                        ReductionData rd = new ReductionData();
                        rd.Duration2 = Utility.LerpFloat(FishSetting.FishDeadTime.Bullet_BingDong_Dead_Time1, FishSetting.FishDeadTime.Bullet_BingDong_Dead_Time2, Utility.Range(0.0f, 1.0f));
                        rd.Speed = 0;
                        fr = new FishOptReduction(rd.Speed, rd, 0, bd);
                        fr.DeadType = FishDeadType.DEAD_IMMEDIATE;
                        fr.CatchData = cd;
                        fish.ClearOpt();
                        fish.SetCatched(clientSeat);
                        fish.AddOpt(fr);
                        fish.SetDropReward(nfc.nReward);

                        GameObject effect = SceneRuntime.EffectMgr.GetSkillEffect(3);
                        Vector3 pos = fish.Position;
                        pos.z -= 20;
                        effect.transform.localPosition = pos;
                        GlobalEffectData gfd = new GlobalEffectData(effect, rd.Duration2, 1.0f);
                        gfd.Scaling = SceneRuntime.ComputeEffectScaling(fish, 0.2f);
                        GlobalEffectMgr.Instance.AddEffect(gfd);
                    }
                    else
                    {
                        SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
                    }
                    cd.FishList.Add(cfd);
                    break;
                case FishCatchEventType.CATCH_EVENT_CATCHED_LIGHTING_MAIN:
                    if (fish != null)
                    {
                        ReductionData rd = new ReductionData();
                        rd.Duration2 = 1;// Utility.LerpFloat(FishSetting.FishDeadTime.Bullet_BingDong_Dead_Time1, FishSetting.FishDeadTime.Bullet_BingDong_Dead_Time2, Utility.Range(0.0f, 1.0f));
                        rd.Speed = 0;
                        fr = new FishOptReduction(rd.Speed, rd, 0, bd);
                        fr.DeadType = FishDeadType.DEAD_IMMEDIATE;
                        fr.CatchData = cd;
                        fish.ClearOpt();
                        fish.SetCatched(clientSeat);
                        fish.AddOpt(fr);
                        fish.SetDropReward(nfc.nReward);

                        GameObject effect = SceneRuntime.EffectMgr.GetSkillEffect(3);
                        Vector3 pos = fish.Position;
                        pos.z -= 20;
                        effect.transform.localPosition = pos;
                        GlobalEffectData gfd = new GlobalEffectData(effect, rd.Duration2, 1.0f);
                        gfd.Scaling = SceneRuntime.ComputeEffectScaling(fish, 0.2f);
                        GlobalEffectMgr.Instance.AddEffect(gfd);

                        if (cll == null)
                            cll = new CatchLithingList();
                        cll.AddMainFish(fish);
                    }
                    else
                    {
                        SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
                    }
                    cd.FishList.Add(cfd);
                    break;
            }
        }
        if (cd.FishList.Count > 0)
        {
            SceneRuntime.ComputeGoldNum(cd);
            SceneRuntime.SceneLogic.CatchFish(cd);
        }
        SceneRuntime.PlayerMgr.FishCatch(clientSeat, cmd.Combo);
        if (cbd.BulletObj != null)
        {
            cbd.BulletObj.GoldNum += cd.GoldNum;
            SceneRuntime.EffectMgr.PlayFishNet(cbd.BulletObj.Position, cd.SubType);
        }
        SceneRuntime.BulletMgr.RemoveBullet(clientSeat, id);
        if (cll != null)
            ProcessLightingData(cll);
        //SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, cd.GoldNum);
        //if (clientSeat == SceneRuntime.MyClientSeat)
        //{
        //    if (cd.GoldNum != cmd.GoldNum)
        //    {
        //        LogMgr.Log("冰子弹:金币不相等:" + cd.GoldNum + "," + cmd.GoldNum);
        //    }
        //    else if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() != cmd.TotalNum)
        //    {
        //        LogMgr.Log("冰子弹:总金币不相等:" + (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel()) + "," + cmd.TotalNum);
        //    }
        //}
    }
    public void LaunchLaser(NetCmdPack pack)
    {
        NetCmdSkillLaserResponse cmd = (NetCmdSkillLaserResponse)pack.cmd;
        byte clientSeat = SceneRuntime.ServerToClientSeat(cmd.Seat);
        bool bNotDelay = Utility.GetTickCount() - pack.tick < ConstValue.FISH_OVER_TIME;
        if (SceneRuntime.PlayerMgr.GetPlayer(clientSeat) == null)
            return;
        CatchedData cd = new CatchedData();
        cd.CatchType = (byte)CatchedType.CATCHED_LASER;
        cd.ClientSeat = clientSeat;
        cd.SubType = cmd.LaserType;
        cd.FishList = new List<CatchFishData>();
        cd.RateIndex = SceneRuntime.PlayerMgr.GetPlayer(clientSeat).RateIndex;

        if (bNotDelay == false)
        {
            ProcessDelayOver(cmd.FishID, cd);
            return;
        }
        CatchLithingList cll = null;
        SceneRuntime.PlayerMgr.LaunchLaser(clientSeat, cmd.LaserType, cmd.Degree);  
        SceneObjMgr.Instance.PlayBack(BackAnimType.BACK_ANIM_JG);
        SceneRuntime.PlayerMgr.PlayCD(LauncherSetting.LauncherDataList[cmd.LaserType].LaserCDTime);
        SceneRuntime.SceneLogic.LogicUI.PlayCD(LauncherSetting.LauncherDataList[cmd.LaserType].LaserCDTime, SkillType.SKILL_FREEZE);

        if (cmd.FishID != null)
        {
            
            if (cmd.LaserType == 3)
            {
                BlendData bd = new BlendData();
                bd.BaseTex = SceneRuntime.EffectMgr.SkillFreezeBaseTex;
                bd.EffectTex = SceneRuntime.EffectMgr.SkillFreezeEffectTex;
                bd.Blend_Type = (byte)BlendType.BLEND_ADD_TEX;
                bd.Duration1 = 0;
                bd.Duration2 = 2.0f;
                bd.Factor = 0.5f;
                for (int i = 0; i < cmd.FishID.Length; ++i)
                {
                    NetFishDeadTime nfc = cmd.FishID[i];
                    CatchFishData cfd = SceneRuntime.FishMgr.FindFish(nfc.FishID);
                    if (cfd.IsValidFishType() == false)
                        continue;
                    Fish fish = cfd.FishObj;
                    if (fish != null)
                    {
                        if (nfc.LightingFishID != 0)
                        {
                            if (cll == null)
                                cll = new CatchLithingList();
                            CatchFishData cfd2 = SceneRuntime.FishMgr.FindFish(nfc.LightingFishID);
                            if (cfd2.FishObj != null)
                                cll.AddSubFish(cfd2.FishObj, fish);
                        }
                        fish.ClearOpt();
                        fish.StopLaugh(false);
                        float t = nfc.DeadTime * ConstValue.INV255;
                        FishOptReduction fr;
                        ReductionData rd = new ReductionData();
                        rd.Speed = FishSetting.FishDeadTime.JiGuang_BingDong_Speed;
                        rd.Duration2 = Utility.LerpFloat(FishSetting.FishDeadTime.JiGuang_Dead_Time1, FishSetting.FishDeadTime.JiGuang_Dead_Time2, t);
                        fr = new FishOptReduction(rd.Speed, rd, 0, bd);
                        fr.DeadType = FishDeadType.DEAD_IMMEDIATE;
                        fr.CatchData = cd;
                        fish.SetCatched(clientSeat);
                        fish.AddOpt(fr);
                        fish.SetDropReward(nfc.nReward);

                        GameObject effect = SceneRuntime.EffectMgr.GetSkillEffect(3);
                        Vector3 pos = fish.Position;
                        pos.z -= 20;
                        effect.transform.localPosition = pos;
                        GlobalEffectData gfd = new GlobalEffectData(effect, rd.Duration2, 1.0f);
                        gfd.Scaling = SceneRuntime.ComputeEffectScaling(fish, 0.2f);
                        GlobalEffectMgr.Instance.AddEffect(gfd);
                    }
                    else
                    {
                        SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
                    }
                    cd.FishList.Add(cfd);
                }
            }
            else
            {
                BlendData bd = new BlendData();
                bd.BaseTex = null;
                bd.EffectTex = SceneRuntime.EffectMgr.SkillDissolveTex;
                bd.Blend_Type = (byte)BlendType.BLEND_DISSOLVE_TEX;
                bd.Duration1 = 1.2f;
                bd.Duration2 = 0.0f;
                bd.Factor = 0.7f;
                for (int i = 0; i < cmd.FishID.Length; ++i)
                {
                    NetFishDeadTime nfc = cmd.FishID[i];
                    CatchFishData cfd = SceneRuntime.FishMgr.FindFish(nfc.FishID);
                    if (cfd.IsValidFishType() == false)
                        continue;
                    Fish fish = cfd.FishObj;
                    if (fish != null)
                    {
                        if (nfc.LightingFishID != 0)
                        {
                            if (cll == null)
                                cll = new CatchLithingList();
                            CatchFishData cfd2 = SceneRuntime.FishMgr.FindFish(nfc.LightingFishID);
                            if (cfd2.FishObj != null)
                                cll.AddSubFish(cfd2.FishObj, fish);
                        }
                        fish.ClearOpt();
                        fish.StopLaugh(false);
                        float t = nfc.DeadTime * ConstValue.INV255;
                        ReductionData rd = new ReductionData();
                        rd.Speed = FishSetting.FishDeadTime.JiGuang_Speed;
                        rd.Duration1 = FishSetting.FishDeadTime.JiGuang_Stay_Time;
                        rd.Duration2 = Utility.LerpFloat(FishSetting.FishDeadTime.JiGuang_Dead_Time1, FishSetting.FishDeadTime.JiGuang_Dead_Time2, t);
                        FishOptReduction ff = new FishOptReduction(0.25f, rd, 0.0f, bd);
                        ff.CatchData = cd;
                        ff.DeadType = FishDeadType.DEAD_IMMEDIATE;
                        fish.SetCatched(clientSeat);
                        fish.AddOpt(ff);
                        fish.SetDropReward(nfc.nReward);
                        if (cmd.LaserType < SceneRuntime.EffectMgr.LaserSubEffectCount)
                        {
                            GameObject go = SceneRuntime.EffectMgr.GetLaserSubEffect(cmd.LaserType);
                            GlobalEffectData gef = new GlobalEffectData(fish, go, 0, 1.0f, GlobalEffectPosConverter.LaserPosConvert);
                            gef.SetScaling(SceneRuntime.ComputeEffectScaling(fish, 0.2f));
                            GlobalEffectMgr.Instance.AddEffect(gef);
                        }
                    }
                    else
                    {
                        SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
                    }
                    cd.FishList.Add(cfd);
                }
            }
        }
        SceneRuntime.ComputeGoldNum(cd);
        SceneRuntime.SceneLogic.CatchFish(cd);
        if(cll != null)
        {
            ProcessLightingData(cll);
        }
        //SceneRuntime.PlayerMgr.UpdatePlayerGold(clientSeat, cd.GoldNum);
        //if (clientSeat == SceneRuntime.MyClientSeat)
        //{
        //    if (cd.GoldNum != cmd.GoldNum)
        //    {
        //        LogMgr.Log("大招:金币不相等:" + cd.GoldNum + "," + cmd.GoldNum);
        //    }
        //    else if (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel() != cmd.TotalNum)
        //    {
        //        LogMgr.Log("大招:总金币不相等:" + (PlayerRole.Instance.RoleInfo.RoleMe.GetGlobel()) + "," + cmd.TotalNum);
        //    }
        //}
    }
    public void Reduction(NetCmdPack pack)
    {
        //延迟过大，不做处理。
        if(Utility.GetTickCount() - pack.tick >= ConstValue.FISH_OVER_TIME)
            return;
        NetCmdReduction red = (NetCmdReduction)pack.cmd;
        ReductionData rd = LauncherSetting.LauncherDataList[red.LaserType].LaserReduction;
        if(red.FishID != null)
        for(int i = 0; i < red.FishID.Length; ++i)
        {
            CatchFishData cfd = SceneRuntime.FishMgr.FindFish(red.FishID[i]);
            if (cfd.FishObj == null)
                continue;
            FishOptReduction ff = new FishOptReduction(rd.Speed, rd);
            cfd.FishObj.ClearOpt();
            cfd.FishObj.AddOpt(ff);
        }
    }
    //清场动画
    public void ClearScene(NetCmdPack pack)
    {
        byte clearType =  ((NetCmdClearScene)pack.cmd).ClearType;
        bool leftToRight = clearType == 0;
        bool bNotDelay = Utility.GetTickCount() - pack.tick < ConstValue.FISH_OVER_TIME;
        if (bNotDelay == false || clearType == 2)
        {
            SceneRuntime.FishMgr.ClearAllFish();
            return;
        }
        SceneRuntime.SceneLogic.SetClearScene();
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_ClearFish);
        ushort id = leftToRight ? (ushort)8 : (ushort)7;
        GameObject effect = SceneRuntime.EffectMgr.GetSkillEffect(id);
        GlobalEffectData gfd = new GlobalEffectData(effect, 0, 4.0f);
        GlobalEffectMgr.Instance.AddEffect(gfd);
        if (leftToRight)
            PathManager.Instance.BoLang.SetWorldMatrix(Matrix4x4.TRS(Vector3.zero, Quaternion.AngleAxis(0, Vector3.up), Vector3.one));
        else
            PathManager.Instance.BoLang.SetWorldMatrix(Matrix4x4.TRS(Vector3.zero, Quaternion.AngleAxis(180, Vector3.up), Vector3.one));
        SceneRuntime.FishMgr.BackupFishList();

        if(SceneRuntime.FishMgr.BackFishList != null)
        foreach(Fish fish in SceneRuntime.FishMgr.BackFishList)
        {
            if (fish.Delay || fish.Catched)
                continue;

            GameObject effectobj = SceneRuntime.EffectMgr.GetSkillEffect(9);
            float z = SceneRuntime.ComputeZScaling(fish, 1.0f);
            float scl = z * fish.Scaling * 1.0f;
            effectobj.transform.localScale = new Vector3(scl, scl, scl);
            GlobalEffectData gfdf = new GlobalEffectData(fish, effectobj, 0, 4.0f, null, scl);
            float y = FishResManager.Instance.GetFishData(fish.FishType).Size.y;
            gfdf.Offset = new Vector3(0, y * 0.65f * z * fish.Scaling, 0);
            GlobalEffectMgr.Instance.AddEffect(gfdf);
            fish.ExtraData = gfdf;
            FishOptTimer fot = new FishOptTimer(gfd, PathManager.Instance.BoLang, leftToRight);
            fish.ClearOpt();
            fish.AddOpt(fot);
        }
    }
    public void ProcessDelayOverFreeze(NetFishDeadTime[] fishList, CatchedData cd)
    {
        if (fishList == null)
            return;
        for (int i = 0; i < fishList.Length; ++i)
        {
            NetFishDeadTime dt = fishList[i];
            CatchFishData cfd = SceneRuntime.FishMgr.FindFish(dt.FishID);
            if (cfd.IsValidFishType() == false)
                continue;

            bool dead = dt.DeadTime != 0;
            if (dead)
            {
                SceneRuntime.PlayerMgr.UpdatePlayerGold(cd.ClientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
                cd.FishList.Add(cfd);
                if(cfd.FishObj != null)
                    SceneRuntime.FishMgr.DestroyFish(cfd.FishObj, false);
            }
        }
        SceneRuntime.ComputeGoldNum(cd);
        SceneRuntime.SceneLogic.CatchFish(cd);
    }
    public void ProcessDelayOver(NetFishDeadTime[] fishList, CatchedData cd)
    {
        if (fishList == null)
            return;
        for(int i = 0; i < fishList.Length; ++i)
        {
            NetFishDeadTime dt = fishList[i];
            CatchFishData cfd = SceneRuntime.FishMgr.FindFish(dt.FishID);
            if (cfd.IsValidFishType() == false)
                continue;
            SceneRuntime.PlayerMgr.UpdatePlayerGold(cd.ClientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
            cd.FishList.Add(cfd);
            if(cfd.FishObj != null)
                SceneRuntime.FishMgr.DestroyFish(cfd.FishObj, false);
        }
        SceneRuntime.ComputeGoldNum(cd);
        SceneRuntime.SceneLogic.CatchFish(cd);
    }
    public void ProcessDelayOver(NetFishCatched[] fishList, CatchedData cd)
    {
        if (fishList == null)
            return;
        for (int i = 0; i < fishList.Length; ++i)
        {
            NetFishCatched dt = fishList[i];
            if (dt.CatchEvent != (byte)FishCatchEventType.CATCH_EVENT_CATCHED)
                continue;
            CatchFishData cfd = SceneRuntime.FishMgr.FindFish(dt.FishID);
            if (cfd.IsValidFishType() == false)
                continue;
            SceneRuntime.PlayerMgr.UpdatePlayerGold(cd.ClientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
            cd.FishList.Add(cfd);
            if (cfd.FishObj != null)
                SceneRuntime.FishMgr.DestroyFish(cfd.FishObj, false);
        }
        SceneRuntime.ComputeGoldNum(cd);
        SceneRuntime.SceneLogic.CatchFish(cd);
    }
    public void ProcessDelayOver(ushort [] fishList, CatchedData cd)
    {
        if (fishList == null)
            return;
        for (int i = 0; i < fishList.Length; ++i)
        {
            CatchFishData cfd = SceneRuntime.FishMgr.FindFish(fishList[i]);
            if (cfd.IsValidFishType() == false)
                continue;
            SceneRuntime.PlayerMgr.UpdatePlayerGold(cd.ClientSeat, SceneRuntime.GetFishGold(cfd.FishType, cd.RateIndex));
            cd.FishList.Add(cfd);
            if (cfd.FishObj != null)
                SceneRuntime.FishMgr.DestroyFish(cfd.FishObj, false);
        }
        SceneRuntime.ComputeGoldNum(cd);
        SceneRuntime.SceneLogic.CatchFish(cd);
    }
    static Vector3 ScreenToCenterPoint(Vector3 pos)
    {
        Vector3 ScrSize = new Vector3(Screen.width >> 1, Screen.height >> 1, 0);
        Vector3 newPos = new Vector3();
        newPos.x = (pos.x - ScrSize.x) / ScrSize.x * (1024 >> 1);
        newPos.y = (pos.y - ScrSize.y) / ScrSize.y * (576 >> 1);
        newPos.z = 0;
        return newPos;
    }
    void ProcessLightingData(CatchLithingList cll)
    {
        foreach(CatchLithingData cld in cll.LithingList.Values)
        {
            ProcessLightingFishMain(cld.MainFish);
            ProcessLightingFish(cld.SubFish, cld.MainFish);
        }
    }
    public void ProcessLightingFishMain(Fish fish)
    {
        Vector3 p1 = fish.ScreenPos;
        Vector3 pv1 = ScreenToCenterPoint(p1);

        GameObject gov1 = SceneRuntime.EffectMgr.GetLightingFishEffect(false);
        GlobalEffectData efc = new GlobalEffectData(gov1, 0, 2.0f);
        GlobalEffectMgr.Instance.AddEffect(efc);
        gov1.transform.localPosition = pv1;
        gov1.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
    }
    public void ProcessLightingFish(List<Fish> fishList , Fish fishLighting)
    {
        GlobalEffectData efc = null;
        Vector3 p1 = fishLighting.ScreenPos;
        Vector3 pv1 = ScreenToCenterPoint(p1);
        foreach(Fish f in fishList)
        {
            Vector3 p2 = f.ScreenPos;
            Vector3 pv2 = ScreenToCenterPoint(p2);
            Vector3 dir = (pv2 - pv1);
            float dist = dir.magnitude;
            dir /= dist;
            float dot = Vector3.Dot(Vector3.right, dir);
            float angle = Mathf.Acos(Mathf.Clamp(dot, -1.0f, 1.0f)) * Mathf.Rad2Deg;
            if (dir.y < 0)
                angle = -angle;

            GameObject gov2 = SceneRuntime.EffectMgr.GetLightingFishEffect(true);
            GameObject gov3 = SceneRuntime.EffectMgr.GetLightingFishEffect(false);
            
            efc = new GlobalEffectData(gov2, 0, 2.0f);
            GlobalEffectMgr.Instance.AddEffect(efc);
            efc = new GlobalEffectData(gov3, 0, 2.0f);
            GlobalEffectMgr.Instance.AddEffect(efc);

            gov2.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
            gov3.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);

            LightingMoveData lmd = new LightingMoveData();
            lmd.Delta = 0;
            lmd.Trans = gov3.transform;
            lmd.StartPos = pv1;
            lmd.EndPos = pv2;
            m_MoveList.Add(lmd);
            Vector3 ss = gov2.transform.localScale;
            ss.x = dist;
            gov2.transform.localPosition = pv1;
            gov3.transform.localPosition = pv1;
            gov2.transform.localScale = ss;
            gov2.transform.localRotation = Quaternion.AngleAxis(angle, Vector3.forward);
        }
    }
}
