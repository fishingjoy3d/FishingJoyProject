using UnityEngine;
using System.Collections.Generic;

//飞行用
class CFlyGift
{
    public CFlyGift(Vector3 vStart, Vector3 vEnd, Object objGift,Object objGifteff, string szspr,byte byDest,bool bBomb)
    {
        m_bBomb = bBomb;
        if (bBomb)
        {
            m_fBombDelay = 1.25f;
        }
        m_goeff = GameObject.Instantiate(objGifteff) as GameObject;
        m_goeff.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_goeff.transform.position = vEnd;
        m_goeff.SetActive(false);

        Transform pChild = m_goeff.transform.GetChild(0).transform;
        Vector3 pos = pChild.localPosition;
        if(byDest==0)
        {           
            pos.x *= -1;
            pChild.localPosition = pos;   
        }
        else if(byDest==1)
        {

        }
        else if(byDest==2)
        {           
            pos.y *= -1;
            pChild.localPosition = pos;                     
        }
        else if(byDest==3)
        {           
            pos.y *= -1;
            pos.x *= -1;
            pChild.localPosition = pos;     
        }
        

        //////
        m_path = new CLineMove(vStart, pChild.position, 0.0f, 0.5f);
        m_goFly = GameObject.Instantiate(objGift) as GameObject;
        m_goFly.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
        m_goFly.transform.GetChild(0).GetComponent<UISprite>().spriteName = szspr;
        m_goFly.transform.position = vStart;

    }
    public bool Update(float fTime)
    {
        if (m_bBomb)
        {
            m_fBombDelay-=fTime;
            if(m_fBombDelay<0)
            {
                m_bBomb = false;
                SceneObjMgr.Instance.PlayBack(BackAnimType.BACK_ANIM_BOMB);
            }

        }
        if (m_goFly.activeSelf)
        {
             m_goFly.transform.position = m_path.Update(fTime);
             //m_goFly.transform.localScale -= Vector3.one*fTime / 0.5f;
             m_goFly.transform.localRotation *= Quaternion.AngleAxis(-360f * fTime, Vector3.forward);
            if(m_path.IsEnd())
            {
                m_goFly.SetActive(false);
                m_goeff.SetActive(true);
            }
        }
        m_fEffTime -= fTime;
        return m_fEffTime < 0;               
    }
    public void Destroy()
    {
        GameObject.Destroy(m_goFly);
        GameObject.Destroy(m_goeff);
    }
    private bool m_bBomb=false;
    private float m_fBombDelay = 0;
    private CLineMove m_path;
    private GameObject m_goFly;
    private GameObject m_goeff;
    private float m_fEffTime=3.5f;
}

class SendGiftEff
{
    private List<CFlyGift> m_ListGift = new List<CFlyGift>();
    private Object m_objFlyGift;
    private List<Object> m_objGiftEff = new List<Object>();     
    public void Init()
    {
        m_objFlyGift = ResManager.Instance.LoadObject("FlyGift", "SceneRes/Prefab/UI/Gift/", ResType.SceneRes);

         for (byte i = 0; i < FishConfig.Instance.m_CharmInfo.CharmMaxItem; ++i)
        {      
            tagCharmOnce pCharm = FishConfig.Instance.m_CharmInfo.CharmIndexMap[i];
            Object pobj = ResManager.Instance.LoadObject(pCharm.ItemID.ToString(), "SceneRes/Prefab/effect/charm/", ResType.SceneRes);
            m_objGiftEff.Add(pobj);
        }
       
    }
    public void ShowGiftEff(byte bySource, byte byDest, byte byGiftIndex)
    {           
        ScenePlayer pPlayer1 = SceneRuntime.Instance.Scene.PlayerMgr.GetPlayer(bySource);
        ScenePlayer pPlayer2 = SceneRuntime.Instance.Scene.PlayerMgr.GetPlayer(byDest);
        if (pPlayer1 != null && pPlayer2 != null && byGiftIndex<m_objGiftEff.Count)
        {
            GlobalAudioMgr.Instance.PlayCharmMusic(Audio.OrdianryMusic.m_Gift0 + byGiftIndex);
            Vector3 v1=new Vector3(pPlayer1.Launcher.LauncherPos.x, pPlayer1.Launcher.LauncherPos.y,0);
            Vector3 v2=new Vector3(pPlayer2.Launcher.LauncherPos.x, pPlayer2.Launcher.LauncherPos.y,0);
            CFlyGift pGift = new CFlyGift(v1, v2, m_objFlyGift, m_objGiftEff[byGiftIndex], FishConfig.Instance.m_CharmInfo.CharmIndexMap[byGiftIndex].ItemID.ToString(), byDest, byGiftIndex == 4);  
            m_ListGift.Add(pGift);
            return;
        }
    }
    public void Update(float fTime)
    {
        for (int i = 0; i < m_ListGift.Count; )
        {
            if (m_ListGift[i].Update(fTime))
            {
                m_ListGift[i].Destroy();
                Utility.ListRemoveAt(m_ListGift, i);
                continue;
            }
            else
            {
                i++;
            }
        }
    }
    public void ShutDown()
    {
        for (int i = 0; i < m_ListGift.Count; i++)
        {
            m_ListGift[i].Destroy();
        }
        m_ListGift.Clear();
    }
}

