using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using Facebook.Unity;

public class QueryNickname
{
    public string userID;
    public string nickname;
    public UILabel label;
}


public class AsyncNicknameQuery : MonoBehaviour
{
    private static AsyncNicknameQuery _instance = null;
    public static AsyncNicknameQuery GetInstance() { return Instance; }
    public static AsyncNicknameQuery Instance
    {
        get
        {
            if (_instance == null)
            {
                GameObject obj = new GameObject("AsyncNameQuery");
                _instance = obj.AddComponent<AsyncNicknameQuery>();
                DontDestroyOnLoad(obj);
                _instance.Init();
            }
            return _instance;
        }
    }

    private Dictionary<string, string> mNicknameDic = new Dictionary<string, string>();
    private List<QueryNickname> mQueryList = new List<QueryNickname>();

    public bool Init()
    {
        if (!Directory.Exists(Application.persistentDataPath + "/ImageCache/"))
        {
            Directory.CreateDirectory(Application.persistentDataPath + "/ImageCache/");
        }
        return true;
    }

    public void SetAsyncImage(string url, UITexture image)
    {
        //判断是否是第一次加载这张图片
        if (!File.Exists(path + url.GetHashCode()))
        {
            //如果之前不存在缓存文件
            StartCoroutine(DownloadImage(url, image));
        }
        else
        {
            StartCoroutine(LoadLocalImage(url, image));
        }
    }

    IEnumerator DownloadImage(string url, UITexture image)
    {
        Debug.Log("downloading new image:" + path + url.GetHashCode());//url转换HD5作为名字
        WWW www = new WWW(url);
        yield return www;

        Texture2D tex2d = www.texture;
        //将图片保存至缓存路径
        byte[] pngData = tex2d.EncodeToPNG();                         //将材质压缩成byte流
        File.WriteAllBytes(path + url.GetHashCode(), pngData);        //然后保存到本地

        image.mainTexture = tex2d;
    }

    IEnumerator LoadLocalImage(string url, UITexture image)
    {
        string filePath = "file:///" + path + url.GetHashCode();

        Debug.Log("getting local image:" + filePath);
        WWW www = new WWW(filePath);
        yield return www;

        Texture2D texture = www.texture;

        image.mainTexture = texture;
    }

    public string path
    {
        get
        {
            return RuntimeInfo.GetLocalDirectory() + "/names/";
        }
    }

    public void SetAsyncNickname(string userID, UILabel label)
    {
        string query = "?ids=";
        query += userID + ",";
        query = query.Remove(query.Length - 1);
        //FB.API(query, HttpMethod.GET, GetUserNameCallback);

        if (mNicknameDic.ContainsKey(userID))
        {
            label.text = mNicknameDic[userID];
        }
        else
        {
            QueryNickname qun = new QueryNickname();
            qun.userID = userID;
            qun.label = label;
            mQueryList.Add(qun);
        }
    }

    private void GetUserNameCallback(IGraphResult result)
    {
        if (result == null)
        {
            AN_PoupsProxy.ShowToast("GetUserName null");
        }

        // Some platforms return the empty string instead of null.
        if (!string.IsNullOrEmpty(result.Error))
        {
            AN_PoupsProxy.ShowToast("GetUserName Error " + result.Error);
        }
        else if (result.Cancelled)
        {
            AN_PoupsProxy.ShowToast("GetUserName Cancelled");
        }
        else if (result.ResultList != null && result.ResultList.Count > 0)
        {
            AN_PoupsProxy.ShowToast("GetUserName Success " + result.ResultList.Count + " " + result.ResultList[0].ToString());
        }
        else
        {
            AN_PoupsProxy.ShowToast("GetUserName Success ResultList IsNullOrEmpty");
        }
    }
}
