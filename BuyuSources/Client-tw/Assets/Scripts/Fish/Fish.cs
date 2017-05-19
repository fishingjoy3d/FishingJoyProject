using UnityEngine;
using System.Collections.Generic;

public class Fish:IGlobalEffect
{
    public static Vector3 FishInitPos = new Vector3(ConstValue.START_POS, 0, 0);
    public static int DeadHashName = Animator.StringToHash("Dead");
    public static int LaughHashName = Animator.StringToHash("Laugh");
    public static int AttackHashName = Animator.StringToHash("Attack");
    public static int YouYongHashName = Animator.StringToHash("Base Layer.Swim");
    public static int[] ActionHashList = new int[4] {YouYongHashName, DeadHashName, LaughHashName, AttackHashName};
    ushort          m_FishID;
    byte            m_FishType;
    GameObject      m_Model;
    GameObject      m_BoxModel;
    GameObject      m_FishCard;
    Animator        m_Anim;
    Transform       m_ModelTransform;
    Vector3         m_Offset;
    PathController  m_PathCtrl;
    bool            m_Delay;
    float           m_Scaling;
    Quaternion      m_OrgRot;
    Vector3         m_WorldOffset;
    Renderer        m_Renderer;
    List<IFishOpt>  m_OptList;
    Vector3         m_Position;
    bool            m_bCatched;
    object          m_ExtraData;
    byte            m_PackageType;
    byte            m_CatchSeat;
    bool            m_bUpdateScreen;
    bool            m_bUpdateViewPos;
    Vector3         m_ScrPos;
  Vector3         m_ViewPos;
    ushort          m_nRewardDrop;
    float           m_bgsoundDelay=0;           
    public void Init(ushort id, byte type, float scl, float time, float actionSpeed, bool actionUnite, float speed, PathLinearInterpolator interp)
    {
        ResFishData fd = FishResManager.Instance.GetFishData(type);
        if(fd == null)
        {
            Debug.Log("不存在的鱼模型:" + type.ToString());
            return;
        }
        m_CatchSeat     = 0xff;
        m_bCatched      = false;
        m_Delay         = false;
        m_Scaling       = scl;
        m_FishID        = id;
        m_FishType      = type;   
        
        m_PathCtrl = new PathController();
        m_PathCtrl.ResetController(interp, speed, time, fd.ClipLength[(byte)FishClipType.CLIP_CHAOFENG]);

        m_Model = (GameObject)GameObject.Instantiate(FishResManager.Instance.GetFishObj(type));
        m_ModelTransform = m_Model.GetComponent<Transform>();
        m_ModelTransform.position = FishInitPos;
        m_Anim = m_Model.GetComponent<Animator>();
        m_OrgRot = m_ModelTransform.localRotation;
        m_Model.name = m_FishID.ToString();
        SetScaling(scl);
        m_Renderer = m_ModelTransform.GetChild(0).gameObject.GetComponent<Renderer>();
        if (m_Renderer == null)
            m_Renderer = m_ModelTransform.GetChild(1).gameObject.GetComponent<Renderer>();
        m_Anim.speed = actionSpeed;
        if (!actionUnite)
            m_Anim.Play(YouYongHashName, 0, Utility.Range(0.0f, 1.0f));
        if (IsBossFish())
        {
            m_bgsoundDelay = 2;
            GlobalAudioMgr.Instance.StopBgMusic();
            GlobalAudioMgr.Instance.PlayOrdianryMusic(Audio.OrdianryMusic.m_bosscoming);
        }
    }
#if UNITY_EDITOR
    public void SetModelName(string name)
    {
        
    }
#endif
    public void SetDropReward(ushort nReward)
    {
        m_nRewardDrop = nReward;
    }
    public ushort  GetDropReward()
    {
        return m_nRewardDrop;
    }
    public void SetPackage(byte packageType)
    {
        tagChestConfig pChest;
        if (FishConfig.Instance.m_ChestInfo.ChestMap.TryGetValue(packageType, out pChest))
        {
            m_PackageType = packageType;
            float h = FishResManager.Instance.GetFishData(m_FishType).Size.y;
            if (pChest.ImmediateRewardid == 0)//,是宝箱     
            {
                m_BoxModel = (GameObject)GameObject.Instantiate(FishResManager.Instance.BoxObj);
                m_BoxModel.transform.localPosition = new Vector3(0, h * 0.85f, 0);
                h = h / 12 * 25;
                m_BoxModel.transform.localScale = new Vector3(h, h, h);
                m_BoxModel.transform.SetParent(m_Model.transform, false);
            }
            else//是卡片
            {
                //找物品
                tagRewardOnce pReward;
                if (!FishConfig.Instance.m_RewardConfig.RewardMap.TryGetValue(pChest.ImmediateRewardid,out pReward))
                {
                    return;
                }
                if (pReward.RewardItemVec.Count!=1)
                {
                    return;
                }                 
                m_FishCard = (GameObject)GameObject.Instantiate(FishResManager.Instance.CardObj);
                m_FishCard.transform.GetChild(0).GetComponent<UISprite>().spriteName = pReward.RewardItemVec[0].ItemID.ToString();
                if (m_FishType == 9)
                {
                   m_FishCard.transform.localPosition = new Vector3(0, h * 0.85f, 0);                   
                }
                else if (m_FishType == 10)
                {
                    m_FishCard.transform.localPosition = new Vector3(0, h * 1.25f, 0);
                }
                else if (m_FishType == 18)
                {
                     m_FishCard.transform.localPosition = new Vector3(0, h * 1.2f, 0);
                     float fScale = 0.6f;
                    m_FishCard.transform.localScale = new Vector3(fScale, fScale, fScale);
                }
                else if (m_FishType == 20)
                {
                     m_FishCard.transform.localPosition = new Vector3(0, h * 0.85f, 0);
                     float fScale = 0.4f;
                     m_FishCard.transform.localScale = new Vector3(fScale, fScale, fScale);
                }                             
                m_FishCard.transform.SetParent(m_Model.transform.GetChild(0), false);
            }
        }
    }
    public void SetName(string str)
    {
        m_Model.name = str;
    }
    public object ExtraData
    {
        get { return m_ExtraData; }
        set { m_ExtraData = value; }
    }
    public void StopLaugh(bool bEnableEvent)
    {
        if (Controller.IsActiveLaugh)
            m_Anim.SetBool(LaughHashName, false);
        Controller.PathEvent.Reset(0, bEnableEvent);
    }
    public void SetCatched(byte seat)
    {
        m_CatchSeat = seat;
    }
    public bool Catched
    {
        get { return m_CatchSeat != 0xff; }
    }
    public Vector3 WorldOffset
    {
        get { return m_WorldOffset; }
        set { m_WorldOffset = value; }
    }
    public Quaternion OrgRot
    {
        get { return m_OrgRot; }
        set { m_OrgRot = value; }
    }
    public bool ClearOpt()
    {
        if (m_OptList == null)
            return true;
        for (int i = 0; i < m_OptList.Count;++i)
        {
            IFishOpt opt = m_OptList[i];
            FishOptState state = opt.CheckDelay(0);
            if (state == FishOptState.FOS_DELAY || state == FishOptState.FOS_FIRST)
                continue;
            bool bRemove;
            bool bRet = opt.Update(1000, this, out bRemove);
            if (!bRet)
            {
                string str ="Num:" + opt.CatchData.FishList.Count+", ";
                for (int j = 0; j < opt.CatchData.FishList.Count; ++j)
                    str += opt.CatchData.FishList[j].FishType + ",";
                Debug.Log(FishID + ":鱼死亡OPT，又加新动作:" + opt.ToString() +"," + str);
                return false;
            }
        }
        m_OptList.Clear();
        return true;
    }
    public Animator Anim
    {
        get
        {
            return m_Anim;
        }
    }
    public float ActionSpeed
    {
        get
        {
            return m_Anim.speed;
        }
        set
        {
            m_Anim.speed = value;
        }
    }
    public Renderer Render
    {
        get
        {
            return m_Renderer;
        }
    }
    public Transform Transform
    {
        get
        {
            return m_ModelTransform;
        }
    }
    public bool HasOpt
    {
        get
        {
            return m_OptList != null && m_OptList.Count > 0;
        }
    }
    public void AddOpt(IFishOpt opt)
    {
        if (m_OptList == null)
            m_OptList = new List<IFishOpt>();
        m_OptList.Add(opt);
    }
    public bool AddElapsedTime(float delta)
    {
        m_PathCtrl.AddElapsedTime(delta);
        if(Time > 1.0f)
        {
            m_Delay = true;
            m_PathCtrl.Time = 0;
        }
        return true;
    }
    public bool Delay
    {
        get { return m_Delay; }
        set { m_Delay = value; }
    }
    public byte FishType
    {
        get
        {
            return m_FishType;
        }
    }
    public ushort FishID
    {
        get
        {
            return m_FishID;
        }
    }
    public float Time
    {
        get
        {
            return m_PathCtrl.Time;
        }
        set
        {
            m_PathCtrl.Time = value;
        }
    }
    public Quaternion Rotation
    {
        get
        {
            return m_ModelTransform.localRotation;
        }
        set
        {
            m_ModelTransform.localRotation = value;
        }
    }
    public float Speed
    {
        get
        {
            return m_PathCtrl.CurrentSpeed;
        }
        
    }
    public float OrgSpeed
    {
        set
        {
            m_PathCtrl.OrignalSpeed = value;
        }
    }
    public Vector3 Position
    {
        get
        {
            return m_Position;
        }
        set
        {
            m_ModelTransform.localPosition = m_Position = value;
        }
    }

    public PathController Controller
    {
        get { return m_PathCtrl; }
        set { m_PathCtrl = value; }
    }
    public void SetOffset(Vector3 offset)
    {
        m_Offset = offset;
    }
    void SetScaling(float scl)
    {
        m_ModelTransform.localScale *= scl;
        if (m_FishType == 0)
        {
            ParticleSystem[] ps = m_Model.GetComponentsInChildren<ParticleSystem>();
            for (int i = 0; i < ps.Length; ++i)
            {
                ps[i].startSize *= scl;
                ps[i].Clear();
            }
        }
    }
    public float Scaling
    {
        get
        {
            return m_Scaling;
        }
    }
    public bool OptUpdate(float delta)
    {
        if (m_OptList == null)
            return true;

        bool bRemove;
        for (int i = 0; i < m_OptList.Count; )
        {
            IFishOpt opt = m_OptList[i];
            FishOptState state = opt.CheckDelay(delta);
            if (state == FishOptState.FOS_DELAY)
            {
                ++i;
                continue;
            }
            if (state == FishOptState.FOS_FIRST)
                opt.Init(this);

            bool bRet = opt.Update(delta, this, out bRemove);
            if (bRet == false)
                return false;

            if (bRemove)
            {
                Utility.ListRemoveAt(m_OptList, i);
            }
            else
                ++i;
        }
        return true;
    }
    public bool IsActionEvent
    {
        get
        {
            return m_PathCtrl.IsActiveEvent;
        }
    }
    public bool IsDelay
    {
        get
        {
            return m_Delay;
        }
    }
    public bool HasPackage
    {
        get
        {
            return m_BoxModel != null;
        }
    }
    //测试使用
    public bool TestUpdate(float delta)
    {
        bool bRet = m_PathCtrl.Update(delta);
        if (m_PathCtrl.Time < 0.0f)
            return true;
        if (m_PathCtrl.Time >= 1.0f)
            return false;

        if (bRet)
        {
            m_ModelTransform.position = m_Position = m_PathCtrl.Interpolator.m_WorldMatrix.MultiplyPoint(m_PathCtrl.m_Position + m_Offset) + WorldOffset;
            m_ModelTransform.rotation = m_PathCtrl.Interpolator.m_WorldRotation * m_PathCtrl.m_Rotation * m_OrgRot;
        }
        if (m_PathCtrl.CtrlEvent == ControllerEvent.EVENT_ACTIVE_LAUGH)
        {
            m_PathCtrl.ResetCtrlEvent();
            m_Anim.SetBool(LaughHashName, true);
        }
        else if (m_PathCtrl.CtrlEvent == ControllerEvent.EVENT_DEACTIVE_LAUGH)
        {
            m_PathCtrl.ResetCtrlEvent();
            m_Anim.SetBool(LaughHashName, false);
        }
        return true;
    }
    public bool Update(float delta)
    {
        if (IsBossFish()&&m_bgsoundDelay>0)
        {
            m_bgsoundDelay -= delta;
            if(m_bgsoundDelay<0)
            {                                           
                GlobalAudioMgr.Instance.PlayerBGMusic(Audio.EffectBGType.EffectBGSound3+m_FishID%2);
            }
        }
        if(m_Delay)
        {
            Time += delta;
            return Time < 0.5f;
        }
        if (OptUpdate(delta) == false)
            return false;

        bool bRet = m_PathCtrl.Update(delta);
        if (m_PathCtrl.Time < 0.0f)
            return true;
        if (m_PathCtrl.Time >= 1.0f)
        {
            //自然死亡，延迟删除
            m_Delay = true;
            Time = 0;
            return true;
        }

        if (bRet)
        {
            m_bUpdateScreen = false;
            m_bUpdateViewPos = false;
            m_ModelTransform.position = m_Position = m_PathCtrl.Interpolator.m_WorldMatrix.MultiplyPoint(m_PathCtrl.m_Position + m_Offset) + WorldOffset;
            m_ModelTransform.rotation = m_PathCtrl.Interpolator.m_WorldRotation * m_PathCtrl.m_Rotation * m_OrgRot;
        }
        if(m_PathCtrl.CtrlEvent == ControllerEvent.EVENT_ACTIVE_LAUGH)
        {
            m_PathCtrl.ResetCtrlEvent();
            m_Anim.SetBool(LaughHashName, true);
        }
        else if(m_PathCtrl.CtrlEvent == ControllerEvent.EVENT_DEACTIVE_LAUGH)
        {
            m_PathCtrl.ResetCtrlEvent();
            m_Anim.SetBool(LaughHashName, false);
        }

        if (m_BoxModel!=null)
        {
            m_BoxModel.transform.localRotation *= Quaternion.AngleAxis(180 * delta, Vector3.forward);
        }
        return true;
    }
    public void OnActiveBox()
    {
        SceneRuntime.SceneLogic.ChestMgr.MoveBox(SceneRuntime.WorldToNGUI(m_BoxModel.transform.position));      
    }
    public Vector3 ScreenPos
    {
        get
        {
            if (m_bUpdateScreen == false)
            {
                m_bUpdateScreen = true;
                m_ScrPos = Camera.main.WorldToScreenPoint(m_Position);
                m_ScrPos.z = 0;
            }
            return m_ScrPos;
        }
    }
    public Vector3 ViewPos
    {
        get
        {
            if (m_bUpdateViewPos == false)
            {
                m_bUpdateViewPos = true;
                m_ViewPos = Camera.main.ScreenToViewportPoint(ScreenPos);
                m_ViewPos.z = 0;
            }
            return m_ViewPos;
        }
    }
    public void Destroy()
    {
        if(IsBossFish())
        {
            GlobalAudioMgr.Instance.PlayerBGMusic(Audio.EffectBGType.EffectBGSound1);
        }
        if (m_BoxModel != null)
        {
            //触发宝箱事件。
            if(m_CatchSeat == SceneRuntime.PlayerMgr.MyClientSeat)
                OnActiveBox();
            GameObject.Destroy(m_BoxModel);
            m_BoxModel = null;
        }

        if (m_FishCard != null)
        {
            if (m_CatchSeat == SceneRuntime.PlayerMgr.MyClientSeat)
            {               
               // SceneRuntime.SceneLogic.LogicUI.GetSkillBar.JudgeGuide();
                Vector2 vecPos = SceneRuntime.SceneLogic.PlayerMgr.MySelf.Launcher.LauncherPos;
                SceneRuntime.SceneLogic.LogicUI.MoveCard(m_FishCard, new Vector3(vecPos.x, vecPos.y,0));               
            }
            else
            {
                GameObject.Destroy(m_FishCard);
            }            
            m_FishCard = null;
        }
        GameObject.Destroy(m_Model);
        m_Model = null;
        m_ModelTransform = null;
        m_Anim = null;
        m_PathCtrl = null;
    }
    public bool IsBossFish()
    {
        return m_FishType == 1 || m_FishType == 3 || m_FishType == 19;
    }
}
