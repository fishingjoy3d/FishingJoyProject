using UnityEngine;
using System.Collections.Generic;
using System.IO;

public class FishResManager:Singleton<FishResManager>
{
    List<ResFishData>   m_FishList      = new List<ResFishData>();
    List<GroupDataList> m_FishGroupList = new List<GroupDataList>();
    List<Object> m_FishObjList          = new List<Object>();
    Object m_BoxObj;
    Object m_CardObj;
    byte m_ValidCount;
    byte m_ValidMaxCount;
    public void AddGroupData(GroupDataList gdl)
    {
        m_FishGroupList.Add(gdl);
    }
    public void AddFishData(ResFishData fd)
    {
        m_FishList.Add(fd);
    }
    public byte ValidFishCount
    {
        get
        {
            return m_ValidCount;
        }
    }
    public byte ValidMaxCount
    {
        get
        {
            return m_ValidMaxCount;
        }
    }
    System.Collections.IEnumerator InitProcedure(object objobj)
    {
        m_BoxObj = ResManager.Instance.LoadObject("Box0", "FishRes/Prefab/", ResType.FishRes, false);
        m_CardObj = ResManager.Instance.LoadObject("FishCard", "FishRes/Prefab/", ResType.FishRes, false);

        for (byte i = 0; i < 30; ++i)
        {
            string fishid = "Fish" + i.ToString();
            Object obj = ResManager.Instance.LoadObject(fishid, "FishRes/Prefab/", ResType.FishRes, false);
            if (obj == null)
            {
                m_FishObjList.Add(null);
                continue;
            }
            ++m_ValidCount;
            m_ValidMaxCount = i;
            m_FishObjList.Add(obj);
        }
        InitLogic.EndInit();
        yield break;
    }
    public bool Init()
    {
        //InitProcedure(null);
        InitLogic.StartInit();
        SceneMain.Instance.StartInnerCoroutine(InitProcedure(null));
        return true;
    }
    public int GetGroupCount()
    {
        return m_FishGroupList.Count;
    }
    public GroupDataList GetFishGroup(ushort groupID)
    {
        if (m_FishGroupList.Count <= groupID || m_FishGroupList[groupID] == null)
        {
            return null;
        }
        return m_FishGroupList[groupID];
    }
    public ResFishData GetFishData(byte idx)
    {
        if (m_FishList.Count <= idx || m_FishList[idx] == null)
        {
            return null;
        }
        return m_FishList[idx];
    }
    public Object GetFishObj(byte idx)
    {
        if (m_FishObjList.Count <= idx)
            return null;
        return m_FishObjList[idx];
    }
    public int FishObjCount
    {
        get
        {
            return m_FishObjList.Count;
        }
    }
    public Object BoxObj
    {
        get { return m_BoxObj; }
        set { m_BoxObj = value; }
    }
    public Object CardObj
    {
        get { return m_CardObj; }
    }
}

