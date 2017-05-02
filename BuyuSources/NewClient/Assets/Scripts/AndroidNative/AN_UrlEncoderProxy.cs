using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AN_UrlEncoderProxy
{
    private const string CLASS_NAME = "java.net.URLEncoder";
    private static AndroidJavaClass _androidNativeClass;

    private static AndroidJavaClass androidNativeClass
    {
        get
        {
#if UNITY_ANDROID
            if (Application.platform != RuntimePlatform.Android)
            {
                return null;
            }
            try
            {
                if (_androidNativeClass == null)
                {
                    _androidNativeClass = new AndroidJavaClass(CLASS_NAME);
                }
            }
            catch (System.Exception ex)
            {
                Debug.LogWarning(ex.Message);
            }
            return _androidNativeClass;
#else
            return null;
#endif
        }
    }

    public static string Encode(string str)
    {
        return androidNativeClass.CallStatic<string>("encode", str, "UTF_8");
    }
}