using UnityEngine;
using System.Collections.Generic;
//鱼头顶的卡片
public class DropCardItem
{
    private GameObject m_gocard;
    private CLineMove m_path;
    float m_fShowDelay = 0f;
    bool m_bFly = false;
    ushort m_nGoodid;
    public void Init(GameObject goCard, Vector3 vStart, Vector3 vEnd, ushort nGoodid)
    {
        m_gocard = goCard;
        m_gocard.transform.position = vStart;
        m_nGoodid = nGoodid;
        m_gocard.transform.GetChild(0).GetComponent<Animator>().SetBool(Animator.StringToHash("cardJump"), true);
        m_path = new CLineMove(m_gocard.transform.position, vEnd, 0f, 0.5f);
    }
    public bool Update(float ftime)
    {
        m_fShowDelay += ftime;
        if (m_fShowDelay > 1.5f)
        {
            if (!m_bFly)
            {
                m_bFly = true;
                m_gocard.transform.GetChild(0).GetComponent<Animator>().SetBool(Animator.StringToHash("cardFly"), true);
            }
            m_gocard.transform.position = m_path.Update(ftime);                                 
        }
        else
        {
            m_gocard.transform.position = m_path.Update(0);
        }
        return m_path.IsEnd();
    }
    public ushort CardId()
    {
        return m_nGoodid;
    }
    public void Destroy()
    {
        if (m_gocard)
        {
            GameObject.Destroy(m_gocard);
            m_gocard = null;
        }
    }
}

public class DropItemManager
{
    Object m_objDiamond;
    List<DropCardItem> m_fishcards = new List<DropCardItem>();  //飞行的卡片
    public void Init()
    {
        m_objDiamond = ResManager.Instance.LoadObject("SceneDiamondEffect", "SceneRes/Prefab/UI/Gold/", ResType.SceneRes);
    }
     public void Update(float fTime)
    {     
        for (int i = 0; i < m_fishcards.Count; )
        {
            DropCardItem pCard = m_fishcards[i];
            if (pCard.Update(fTime))
            {
               // SceneRuntime.SceneLogic.LogicUI.GetSkillBar.Guide(pCard.CardId());
                pCard.Destroy();
                Utility.ListRemoveAt(m_fishcards, i);
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
         for (byte i = 0; i < m_fishcards.Count; i++)
         {
             m_fishcards[i].Destroy();
         }
         m_fishcards.Clear();
     }
      public void MoveCard(GameObject goCard, Vector3 vecEnd)
      {
          ushort uId = System.Convert.ToUInt16(goCard.transform.GetChild(0).GetComponent<UISprite>().spriteName);
         // Vector3 vecEnd = Vector3.zero;
         // int nIndex = SceneRuntime.SceneLogic.LogicUI.GetSkillBar.BarItemIndex(uId);
        //  if (nIndex != -1)
          {
             // vecEnd = SceneRuntime.SceneLogic.LogicUI.GetSkillBar.ItemPos((byte)nIndex);
          }

          Vector3 vStart = SceneRuntime.WorldToNGUI(goCard.transform.position);
          goCard.layer = 8;//
          goCard.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
          goCard.transform.localScale = Vector3.one;

          DropCardItem pCard = new DropCardItem();
          m_fishcards.Add(pCard);
          pCard.Init(goCard,vStart,vecEnd, uId);
         
      }

      public void MoveDropCard(Vector3 vecStart, Vector3 vecEnd, ushort uItemid)
      {
          Vector3 offset = new Vector3(Utility.RandFloat(), Utility.RandFloat(), 0);
          vecStart += offset * 0.25f;

          //tagRewardOnce pReward;
          //if (!FishConfig.Instance.m_RewardConfig.RewardMap.TryGetValue(nReward, out pReward))
          //{
          //    return;
          //}
          //for (int i = 0; i < pReward.RewardItemVec.Count; i++)
          {
              //tagItemConfig itemConfig;
              //uint uItemid=pReward.RewardItemVec[i].ItemID;
              //if (!FishConfig.Instance.m_ItemInfo.m_ItemMap.TryGetValue(uItemid, out itemConfig))
              //{
              //    continue;
              //}                       
              //if (itemConfig.ItemTypeID == EItemType.IT_Currey)//特殊处理
              //{                   
              //    GameObject goItem = GameObject.Instantiate(m_objDiamond) as GameObject;
                 
              //    goItem.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);                 
              //    goItem.transform.localScale = Vector3.one;
                  
              //    CardItem pCard = new CardItem();
              //    m_fishcards.Add(pCard);
              //    pCard.Init(goItem, vecStart,vecEnd, (ushort)uItemid, true, true);                  

              //}
             // else//正常处理
              {
                  GameObject goItem = GameObject.Instantiate(FishResManager.Instance.CardObj) as GameObject;
                 
                  goItem.transform.GetChild(0).GetComponent<UISprite>().spriteName = uItemid.ToString();
                  goItem.layer = 8;//
                  goItem.transform.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);
                  goItem.transform.localScale = new Vector3(0.65f, 0.65f, 0.65f);

                  DropCardItem pCard = new DropCardItem();
                  m_fishcards.Add(pCard);
                  pCard.Init(goItem,vecStart, vecEnd, (ushort)uItemid);
              }
                     
          }

      }


}


