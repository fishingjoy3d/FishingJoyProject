using UnityEngine;
using System.Collections.Generic;

public class LauncherEffectMgr 
{
    Object[]    m_GunFireObj = new Object[ConstValue.MAX_LAUNCHER_NUM];
    Object[]    m_GaiEftObj = new Object[ConstValue.MAX_LAUNCHER_NUM];
    Object[]    m_MoveLightObj = new Object[ConstValue.MAX_LAUNCHER_NUM];
    Object      m_ChangeEftObj;
    Object      m_SkillActiveObj;                   //大招激活特效
    Object      m_SkillAvatarObj;                   //头像显示当前播放技能的特效
    Object[]    m_SkillLiveObj = new Object[ConstValue.MAX_LAUNCHER_NUM];       //大招畜力特效
    Object[]    m_SkillMuzzleObj = new Object[ConstValue.MAX_LAUNCHER_NUM];     //大招炮口特效
    Object[]    m_SkillLaserOjb = new Object[ConstValue.MAX_LAUNCHER_NUM];      //大招特效

    GameObject    m_SkillActiveEft = null;
    GameObject    m_SkillLiveEft = null;
    GlobalEffectData    m_XPSkillMuzzleEft = null;

    GameObject[]        m_MoveLightEft = new GameObject[ConstValue.PLAYER_MAX_NUM];
   // List<GameObject>    m_EffectList = new List<GameObject>();



    public void Init()
    {
        for (byte i = 0; i < ConstValue.MAX_LAUNCHER_NUM; ++i)
        {
            m_GunFireObj[i] = ResManager.Instance.LoadObject(string.Format("Fire{0}", i), "SceneRes/Prefab/Effect/Launcher/BulletLauncher/GunBarrelFire/", ResType.SceneRes);
            m_GaiEftObj[i] = ResManager.Instance.LoadObject(string.Format("GaiEffect{0}", i), "SceneRes/Prefab/Effect/Launcher/BulletLauncher/GunBarrelGai/", ResType.SceneRes);
            m_MoveLightObj[i] = ResManager.Instance.LoadObject(string.Format("MoveLight{0}", i), "SceneRes/Prefab/Effect/Launcher/BulletLauncher/GunBarrelMoveLight/",
                ResType.SceneRes);
            m_SkillLiveObj[i] = ResManager.Instance.LoadObject(string.Format("LauncherSkillLive{0}", i), "SceneRes/Prefab/Effect/Launcher/Skill/Live/",
                ResType.SceneRes);
            m_SkillMuzzleObj[i] = ResManager.Instance.LoadObject(string.Format("LauncherSkillMuzzle{0}", i), "SceneRes/Prefab/Effect/Launcher/Skill/Muzzle/",
                ResType.SceneRes);
            m_SkillLaserOjb[i] = ResManager.Instance.LoadObject(string.Format("LauncherSkillMain{0}", i), "SceneRes/Prefab/Effect/Launcher/Skill/Main/",
                ResType.SceneRes);
        }
        m_SkillAvatarObj = ResManager.Instance.LoadObject(string.Format("Skill10"), "SceneRes/Prefab/Effect/Skill/", ResType.SceneRes);
        m_ChangeEftObj = ResManager.Instance.LoadObject("LauncherChange", "SceneRes/Prefab/Effect/Launcher/Change/", ResType.SceneRes);
        m_SkillActiveObj = ResManager.Instance.LoadObject("LauncherSkillActive0", "SceneRes/Prefab/Effect/Launcher/Skill/Active/", ResType.SceneRes);
    }
	public void Update (float delta) 
    {

	}
    //播放炮发射子弹火焰
    public void PlayGunBarrelFire(Transform parent, byte type)
    {
        GameObject go = GameObject.Instantiate(m_GunFireObj[type]) as GameObject;
        go.transform.SetParent(parent, false);
        GlobalEffectData efc = new GlobalEffectData(go, 0, LauncherSetting.LauncherDataList[type].Interval);
        GlobalEffectMgr.Instance.AddEffect(efc);
    }
    //播放炮管子上特效
    public void PlayGaiEffect(Transform parent, byte type)
    {
        if (m_GaiEftObj[type] == null)
            return;
        GameObject go = GameObject.Instantiate(m_GaiEftObj[type]) as GameObject;
        go.transform.SetParent(parent, false);
        GlobalEffectMgr.SetEffectOnUI(go);
       // m_EffectList.Add(go);
    }
    //播放炮上的流光特效
    public void PlayMoveLight(Transform parent, byte type, byte seat)
    {
        m_MoveLightEft[seat] = GameObject.Instantiate(m_MoveLightObj[type]) as GameObject;
        m_MoveLightEft[seat].transform.SetParent(parent, false);
        GlobalEffectMgr.SetEffectOnUI(m_MoveLightEft[seat]);

      //  m_EffectList.Add(m_MoveLightEft[seat]);
    }
    public void SetActiveMoveLight(bool bActive, byte seat)
    {
        if (m_MoveLightEft[seat] == null)
            return;
        m_MoveLightEft[seat].SetActive(bActive);
    }
    //播放切换炮台特效
    public void PlayChangeLauncherEft(Vector2 pos)
    {
        GameObject go = GameObject.Instantiate(m_ChangeEftObj) as GameObject;
        go.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        go.transform.position = new Vector3(pos.x, pos.y, 0);
        GlobalEffectData efc = new GlobalEffectData(go, 0, 0.5f);
        GlobalEffectMgr.Instance.AddEffect(efc);
    }
    //播放玩家头像上的技能
    public void PlayAvatarEft(Transform parent)
    {
        GameObject go = GameObject.Instantiate(m_SkillAvatarObj) as GameObject;
        go.transform.SetParent(parent, false);
        GlobalEffectData efc = new GlobalEffectData(go, 0, 1.5f);
        GlobalEffectMgr.Instance.AddEffect(efc);

    }
    //播放大招激活特效
    public void PlayXPSKillActiveEft(Transform parent, Vector2 pos)
    {
        m_SkillActiveEft = GameObject.Instantiate(m_SkillActiveObj) as GameObject;
        m_SkillActiveEft.transform.SetParent(parent, false);
        m_SkillActiveEft.transform.position = new Vector3(pos.x, pos.y, 0);
        GlobalEffectMgr.SetEffectOnUI(m_SkillActiveEft);
    }
    public void StopXPSkillActiveEft()
    {
        if (m_SkillActiveEft == null)
            return;
        GameObject.Destroy(m_SkillActiveEft);
    }
    //播放大招畜力特效
    public void PlayXPSkillLiveEft(Transform parent, byte type, Vector2 pos)
    {
        m_SkillLiveEft = GameObject.Instantiate(m_SkillLiveObj[type]) as GameObject;
        m_SkillLiveEft.transform.SetParent(parent, false);
        m_SkillLiveEft.transform.position = new Vector3(pos.x, pos.y, 0);
        GlobalEffectMgr.SetEffectOnUI(m_SkillLiveEft);
    }
    public void StopXPSKillLiveEft()
    {
        if (m_SkillLiveEft == null)
            return;
        GameObject.Destroy(m_SkillLiveEft);
    }
    public void PlayXPSkillMuzzleEft(Transform parent, byte type, Vector3 pos, float life)
    {
        GameObject go = GameObject.Instantiate(m_SkillMuzzleObj[type]) as GameObject;
        go.transform.SetParent(parent, false);
        go.transform.position = SceneRuntime.WorldToNGUI(pos);
        m_XPSkillMuzzleEft = new GlobalEffectData(go, 0, life);
        GlobalEffectMgr.Instance.AddEffect(m_XPSkillMuzzleEft);
        GlobalEffectMgr.SetEffectOnUI(go);
        //m_EffectList.Add(go);
    }
    
    public void PlayLaserEft(Transform parent, byte type, byte clientSeat, Vector2 dir, Vector3 pos, float angle)
    {
        GameObject go = GameObject.Instantiate(m_SkillLaserOjb[type]) as GameObject;
        if (type == 3)
        {
            Vector3 direction = new Vector3(dir.x, dir.y, 0);
            pos.z += 2;
            pos += -direction * 5;
            if (clientSeat > 1)
               angle = 180 - angle;
            go.transform.localRotation = Quaternion.Euler(0, 0, angle);
            go.transform.localPosition = pos;

        }
        else
        {
            go.transform.SetParent(parent, false);
            go.transform.localPosition = new Vector3(0, 0, 0);
        }
        GlobalEffectData eft = new GlobalEffectData(go, 0, 1.3f);
        GlobalEffectMgr.Instance.AddEffect(eft);

    }
    public void RemoveAtEffect()
    {
        //for (int i = 0; i < m_EffectList.Count; )
        //{
        //    if (m_EffectList[i] == null)
        //        Utility.ListRemoveAt(m_EffectList, i);
        //    else
        //        ++i;
        //}
    }
    public void ShutDown()
    {
        for (byte i = 0; i < ConstValue.MAX_LAUNCHER_NUM; ++i)
        {
            ResManager.Instance.UnloadObject(m_GunFireObj[i]);
            ResManager.Instance.UnloadObject(m_GaiEftObj[i]);
            ResManager.Instance.UnloadObject(m_MoveLightObj[i]);
            ResManager.Instance.UnloadObject(m_SkillLiveObj[i]);
            ResManager.Instance.UnloadObject(m_SkillMuzzleObj[i]);
            ResManager.Instance.UnloadObject(m_SkillLaserOjb[i]);
        }
        ResManager.Instance.UnloadObject(m_SkillActiveObj);
        ResManager.Instance.UnloadObject(m_ChangeEftObj);
    }

}
