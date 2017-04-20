using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class HallLoginUI_BaseWind 
{
    string                      m_path = "HallRes/Prefab/UI/";
    GameObject                  m_WindObj;
    Transform                   m_BaseTranF;
    HallLogicUIStatue.Hall_STATE m_WindID = new HallLogicUIStatue.Hall_STATE();
    Object                      m_Obj;
   

    public void Init()
    { }
    public Object Initobj(string objName)
    {
        m_Obj = ResManager.Instance.LoadObject(objName, Path, ResType.HallRes);
        return m_Obj;
    }
    public void Init_GameObj(bool bShow = true)
    {
        SetParent(((GameObject)GameObject.Instantiate(m_Obj)).transform, bShow);
    }
    public void SetParent(Transform objtr, bool bShow = true)
    {
        objtr.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        if (objtr.localScale.x != 1) objtr.localScale = new Vector3(1, 1, 1);
        m_BaseTranF = objtr;
        m_WindObj = m_BaseTranF.gameObject;
        if (m_WindObj.activeSelf!=bShow) m_WindObj.SetActive(bShow);
    }
    public void ShutDown()
    {
        //if (HallRunTimeInfo.Instance_UI != null)
        //   HallRunTimeInfo.Login_UI.ClearHallWindTouchPool(m_WindID.m_state);
        if (m_WindObj != null)
        {

            GameObject.Destroy(m_WindObj);
        }
        m_WindObj = null;
        m_BaseTranF = null;    
    }
    public void Update(float dTime) 
    { }
    public GameObject GridInstantiateObj(UIGrid grid, GameObject obj)
    {
        GameObject obj__ = GameObject.Instantiate(obj) as GameObject;
        if (!obj__.activeSelf)
            obj__.SetActive(true);
        grid.AddChild(obj__.transform);
        if (obj__.transform.localScale.x != 1)
            obj__.transform.localScale = new Vector3(1, 1, 1);
        return obj__;
    }
    public void CleanScrollView(UIGrid grid)
    {

        List<Transform> gridChid = grid.GetChildList();
        for (int i = 0; i < gridChid.Count; ++i)
        {
            GameObject.Destroy(gridChid[i].gameObject);
        }
        grid.transform.DetachChildren();

    }
    public void ChangeBtnName(UIButton btn , string name)
    {
        btn.normalSprite = name;
        btn.hoverSprite = name;
        btn.pressedSprite = name;
        btn.disabledSprite = name;
    }
    public UIEventListener GetBtnLister(GameObject BtnObj ,bool bAddTouchPool = true)
    {
        return UIEventListener.Get(BtnObj);
    }
    public void PlayBtnMusic()
    {
        GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_BtnMusic);
    }
    public Transform BaseTranF
    {
        get { return m_BaseTranF; }
        set { m_BaseTranF = value; }
    }
    public string Path
    {
        get { return m_path; }
        set { m_path = value; }
    }
    public struct WindBtn
    {
        public GameObject   m_BtnObj;
        public UIButton     m_Btn;
        public byte         m_Tag;
        public bool         m_IsChecked;
    } 
    public struct WindTwreen
    {
        public TweenAlpha m_alpha;
        public TweenPosition m_Pos;
    }
    public struct ViewInf
    {
        public UIGrid m_grid;
        public UIGrid m_grid0;
        public GameObject m_BaseChild;
    }
    public GameObject WindObj
    {
        get { return m_WindObj; }
        set { m_WindObj = value; }
    }
    public Object Obj
    {
        get { return m_Obj; }
        set { m_Obj = value; }
    }
    public HallLogicUIStatue.Hall_STATE WindID
    {
        get { return m_WindID; }
        set { m_WindID = value; }
    }
}
