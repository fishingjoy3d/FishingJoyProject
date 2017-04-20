using UnityEngine;
using System.Collections.Generic;

public class SceneWonderful : BaseWnd
{
    UILabel         m_SocreLabel;
    TweenScale      m_TweenScale = null;
    Object          m_EftObj;
    GlobalEffectData m_GlobalEffect;
    float           m_LifeTime;
    bool            m_bDelay = false;
    float           m_Delay;
    float           m_DelayInterval;
    int             m_Scroe;
    int             m_Index = 0;
    bool            m_bNumAinm = false;


    public void Init()
    {
        Object obj = ResManager.Instance.LoadObject("Wonderful_UI", "SceneRes/Prefab/UI/Wonderful/", ResType.SceneRes);
        m_BaseWndObject = GameObject.Instantiate(obj) as GameObject;
        m_BaseTrans = m_BaseWndObject.transform;
        m_BaseTrans.SetParent(SceneObjMgr.Instance.UIPanelTransform, false);

        //加载特效
        m_EftObj = ResManager.Instance.LoadObject("UIEf_Chests2", "SceneRes/Prefab/Effect/Chest/", ResType.SceneRes);
        

        m_SocreLabel = m_BaseTrans.GetChild(0).GetComponent<UILabel>();
        m_TweenScale = m_BaseWndObject.GetComponent<TweenScale>();

        ResManager.Instance.UnloadObject(obj);
        //ResManager.Instance.UnloadObject(obj1);
    }
    public void ShowWonderfulUI(CatchedData cd)
    {
        if (m_BaseWndObject.activeSelf == true)
        {
            if (m_GlobalEffect.IsEnd == false)
                m_GlobalEffect.Life = 0;
            DestorySelf();
        }
        float lifetime = 3.8f;
        float efttime = 0.1f;
        float delaytime = 0.1f;
        if (cd.CatchType == 2 )
        {
            switch (cd.SubType)
            {
                case (byte)SkillType.SKILL_FREEZE:
                    {
                        lifetime = 3.8f;
                        efttime = 0.2f;
                        delaytime = 0.2f;
                    }
                    break;
                case (byte)SkillType.SKILL_LIGHTING:
                    {
                        lifetime = 3.8f;
                        efttime = 2.0f;
                        delaytime = 2.0f;
                    }
                    break;
                case (byte)SkillType.SKILL_TORNADO:
                    {
                        lifetime = 3.8f;
                        efttime = 2.2f;
                        delaytime = 2.2f;
                    }
                    break;
            }
        }
        m_LifeTime = lifetime;
        m_Scroe = cd.GoldNum;
        m_BaseWndObject.SetActive(true);
        m_SocreLabel.text = "0";
        
        //播放特效
       GameObject Eft = GameObject.Instantiate(m_EftObj) as GameObject;
       m_GlobalEffect = new GlobalEffectData(Eft, efttime, 4.5f);
       GlobalEffectMgr.Instance.AddEffect(m_GlobalEffect);
       GlobalEffectMgr.SetEffectOnUI(Eft);

       m_TweenScale.delay = delaytime;
       m_bDelay = true;
       m_DelayInterval = delaytime;
       m_bNumAinm = true;

       m_TweenScale.ResetToBeginning();
       m_TweenScale.PlayForward();
    }
    public bool UpDataUIScroe(float deltaTime)
    {
        if (m_BaseWndObject.activeSelf != true)
            return false;

        if (m_bDelay)
        {
            m_Delay += deltaTime;
            if (m_Delay >= m_DelayInterval)
            {
                m_bDelay = false;
                m_Delay = 0;
            }
            return false;
        }

        if (m_bNumAinm)
        {
            byte rate = 1;
            int AddNum = 1;
            if (m_Scroe <= 1000)
                AddNum = 10;
            else if (m_Scroe > 1000 && m_Scroe <= 2500)
                AddNum = 25;
            else if (m_Scroe > 2500 && m_Scroe < 5000)
                AddNum = 50;
            else if (m_Scroe > 5000 && m_Scroe <= 10000)
                AddNum = 250;
            else if (m_Scroe > 10000 && m_Scroe <= 50000)
                AddNum = 500;
            else if (m_Scroe > 50000 && m_Scroe <= 100000)
                AddNum = 2500;
            else if (m_Scroe > 100000 && m_Scroe <= 500000)
                AddNum = 5000;
            else
                AddNum = 10000;

            if (m_SocreLabel != null && ++m_Index > rate)
            {
                m_Index = 0;
                int numScroe = 0;
                string str = m_SocreLabel.text;
                int.TryParse(str, out numScroe);
                numScroe += AddNum;
                if (numScroe >= m_Scroe)
                {
                    m_bNumAinm = false;
                    numScroe = m_Scroe;
                }
                m_SocreLabel.text = numScroe.ToString();

            }
        }
        m_LifeTime -= deltaTime;
        if (m_LifeTime < 0)
        {
            m_LifeTime = 0;
            DestorySelf();
            return false;
        }
        else
            return true;
    }
    public void DestorySelf()
    {
        m_BaseWndObject.SetActive(false);
    }
    public void ShutDown()
    {
        if (m_BaseWndObject != null)
            GameObject.Destroy(m_BaseWndObject);
        ResManager.Instance.UnloadObject(m_EftObj);
    }
	
}
