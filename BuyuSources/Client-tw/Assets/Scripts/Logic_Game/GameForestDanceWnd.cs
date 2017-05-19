using UnityEngine;
using System.Collections;
using System.Collections.Generic;
class ColorFill
{
    public byte byIndex;
    public Material FillMaterial;
}
public class GameForestDanceWnd : BaseWnd
{

    Object m_Obj;
    Object[] m_MaterialObj;

    Renderer[] pointObj;
    Material[] material;

    Transform m_pointTrans;
    public Transform m_AnimalTrans;

    float m_StopAngle2 = 0;
    float m_StopSpeed2 = 0;

    float rotateCount2 = 0;
    float angle = 0.0f;
    float speed = 0;
    int index;
    bool isRotate = false;
    bool isStop = false;
    public bool isResult = false;
    float axis = 0;

    float m_StopAngle = 0;
    float m_StopSpeed = 0;

    bool isPointRotate = false;
    bool isPointStop = false;
    public bool isPointResult = false;
    float pointRotateCount;
    float pointAngle = 0;
    float pointSpeed = 0;
    float pointAxis;
    int pointIndex;
    float rotateCount = 0;


    public Animator cameraAnim;
    public Animator areaAnim;
    public Animator[] animalAnim = new Animator[24];

    List<ColorFill> ColorFillList = new List<ColorFill>();
    float filldelay = 0;
    

    public void InitObj()
    {
        m_Obj = ResManager.Instance.LoadObject("ForestDanceParty", "GameRes/ForestDance/", ResType.GameRes);
        m_MaterialObj = new Object[3];
        m_MaterialObj[0] = ResManager.Instance.LoadObject("zhishideng_r", "GameRes/ForestDance/Textures/Materials/", ResType.GameRes);
        m_MaterialObj[1] = ResManager.Instance.LoadObject("zhishideng_y", "GameRes/ForestDance/Textures/Materials/", ResType.GameRes);
        m_MaterialObj[2] = ResManager.Instance.LoadObject("zhishideng_g", "GameRes/ForestDance/Textures/Materials/", ResType.GameRes);
        
    }


    public void Init()
    {
        //SceneObjMgr.Instance.SceneCamera.SetActive(false);
        //SceneObjMgr.Instance.SceneBackground.SetActive(false);

        m_BaseWndObject = (GameObject)GameObject.Instantiate(m_Obj);
        Camera cc = m_BaseWndObject.transform.GetChild(2).GetComponent<Camera>();
        cc.rect = Camera.main.rect;
        cc.aspect = Camera.main.aspect;
        m_BaseTrans = m_BaseWndObject.transform;
        m_AnimalTrans = m_BaseTrans.GetChild(0).GetChild(1);
        m_pointTrans = m_BaseTrans.GetChild(1);
        cameraAnim = m_BaseTrans.GetChild(2).GetComponent<Animator>();
        areaAnim = m_BaseTrans.GetChild(1).GetChild(2).GetComponent<Animator>();
        for (byte i = 0; i < 24; i++)
        {
            animalAnim[i] = m_AnimalTrans.GetChild(i).GetChild(1).GetComponent<Animator>();
        }
        material = new Material[3];
        material[0] = (Material)GameObject.Instantiate(m_MaterialObj[0]);
        material[1] = (Material)GameObject.Instantiate(m_MaterialObj[1]);
        material[2] = (Material)GameObject.Instantiate(m_MaterialObj[2]);

        InitPoint();

        isRotate = false;
        isStop = false;
        isResult = false;
        isPointRotate = false;    
        isPointStop = false;
        isPointResult = false;

        //areaAnim.
        //areaAnim.SetBool("isAreaClose", true);
        //areaAnim.SetBool("isAreaOpen", false);
        //cameraAnim.SetBool("isCameraBack", true);
        //cameraAnim.SetBool("isCameraFront", false);
    }

    void InitPoint()
    {
        pointObj = new Renderer[24];
        for (byte i = 0; i < 24; i++)
        {
            pointObj[i] = m_BaseTrans.GetChild(0).GetChild(0).GetChild(i).GetChild(0).GetComponent<Renderer>();
        }
    }

    public void SetBegin(uint result)
    {
        int color = (int)result >> 8;
        int kind = (byte)result;

        m_pointTrans.Rotate(new Vector3(0, color * 15, 0));
        m_AnimalTrans.Rotate(new Vector3(0, (24 + color - kind) % 24 * 15, 0));
    }
    private Material DatetoMaterial(byte byDate)
    {
        if (byDate < 4)
        {
            return material[0];
        }
        else if (byDate < 8)
        {
            return material[1];
        }
        else
        {
            return material[2];
        }         
    }
    public void SetTableAreaDate(byte[] areaData, bool bShowAnimation)
    {
        ColorFill pNewItem = new ColorFill();
        pNewItem.byIndex = 0;
        pNewItem.FillMaterial = DatetoMaterial(areaData[0]);
        ColorFillList.Add(pNewItem);

        for (byte i = 1; i < areaData.Length; i++)
        {
            pNewItem = new ColorFill();
            pNewItem.byIndex = i;
            pNewItem.FillMaterial = DatetoMaterial(areaData[areaData.Length - i]);
            ColorFillList.Add(pNewItem);
        }

        if (!bShowAnimation)
        {
            for (int i = 0; i < ColorFillList.Count; i++)
            {
                ColorFill pItem = ColorFillList[i];
                pointObj[pItem.byIndex].material = pItem.FillMaterial;
            }
            ColorFillList.Clear();
        }
    }

    public void BeginRotate(uint result)
    {
        int color = (int)result >> 8;
        int kind = (byte)result;

        pointAngle = -m_pointTrans.localRotation.eulerAngles.y % 360;
        pointIndex = 24 - color;
        pointSpeed = 0;
        isPointRotate = true;

        angle = m_AnimalTrans.localRotation.eulerAngles.y % 360;
        angle -= 360;
        index = (24 + color - kind) % 24;
        speed = 0;
        isRotate = true;

    }

    public void Update(float deltaTime)
    {
        if(ColorFillList.Count!=0)
        {
            filldelay -= deltaTime;
            if(filldelay<0)
            {
                filldelay = 0.05f;
                ColorFill pItem = ColorFillList[0];
                pointObj[pItem.byIndex].material = pItem.FillMaterial;
                Utility.ListRemoveAt(ColorFillList, 0);
            }                    
        }
        if (isPointRotate)
        {
            m_pointTrans.localRotation = Quaternion.AngleAxis(pointAngle, Vector3.down);
            pointAngle += deltaTime * pointSpeed;
            if (pointAngle < 360)
            {
                pointSpeed += deltaTime * 80;
            }
            else if (pointAngle >= 720)
            {
                pointAxis = pointSpeed * pointSpeed / (30 * pointIndex + 1440);
                pointAngle = 720;
                m_StopSpeed = pointSpeed;
                m_StopAngle = pointAngle;
                rotateCount = 0;
                isPointRotate = false;
                isPointStop = true;
            }
        }
        if (isPointStop)
        {
            rotateCount += deltaTime;
            pointSpeed -= pointAxis * deltaTime;
            if (rotateCount >= m_StopSpeed / pointAxis)
            {
                rotateCount = m_StopSpeed / pointAxis;
                isPointResult = true;
                isPointStop = false;
            }
            m_StopAngle = pointAngle + m_StopSpeed * rotateCount - pointAxis * (rotateCount * rotateCount) / 2f;
            m_pointTrans.localRotation = Quaternion.AngleAxis(m_StopAngle, Vector3.down);
        }

        if (isRotate)
        {
            m_AnimalTrans.localRotation = Quaternion.AngleAxis(angle, Vector3.up);
            angle += deltaTime * speed;
            if (angle < 360)
            {
                if (speed < 220)
                {
                    speed += deltaTime * 90;
                }
            }
            else if (angle >= 720)
            {
                axis = speed * speed / (30 * index + 1440);
                angle = 720;
                m_StopSpeed2 = speed;
                m_StopAngle2 = angle;
                rotateCount2 = 0;
                isRotate = false;
                isStop = true;
            }
        }
        if (isStop)
        {
            rotateCount2 += deltaTime;
            speed -= axis * deltaTime;
            if (rotateCount2 >= m_StopSpeed2 / axis)
            {
                rotateCount2 = m_StopSpeed2 / axis;
                isResult = true;
                isStop = false;
            }
            m_StopAngle2 = angle + m_StopSpeed2 * rotateCount2 - axis * (rotateCount2 * rotateCount2) / 2f;
            m_AnimalTrans.localRotation = Quaternion.AngleAxis(m_StopAngle2, Vector3.up);
        }

        if (isResult && isPointResult)
        {
            areaAnim.SetBool("isAreaClose", false);
            areaAnim.SetBool("isAreaOpen", true);
            cameraAnim.SetBool("isCameraBack", false);
            cameraAnim.SetBool("isCameraFront", true);
        }
    }

    public void ShutDown()
    {
        if (m_BaseWndObject != null)
        {
            WndManager.Instance.Pop(m_BaseWndObject);
            GameObject.Destroy(m_BaseWndObject);
            m_BaseWndObject = null;
        }
    }
}
