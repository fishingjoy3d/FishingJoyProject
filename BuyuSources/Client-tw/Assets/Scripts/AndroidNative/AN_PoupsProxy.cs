using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AN_PoupsProxy{

    private const string CLASS_NAME = "com.androidnative.popups.PopUpsManager";
    private static AndroidJavaClass androidNativeClass;

    public static void CallActivityFunction(string methodName, params object[] args)
    {
#if UNITY_ANDROID
        if (Application.platform != RuntimePlatform.Android)
        {
            return;
        }

        try
        {
            if (androidNativeClass == null)
            {
                androidNativeClass = new AndroidJavaClass(CLASS_NAME);
            }
            androidNativeClass.CallStatic(methodName, args);
        }
        catch (System.Exception ex)
        {
            Debug.LogWarning(ex.Message);
        }
#endif
    }

    public static void ShowToast(string str)
    {
        CallActivityFunction("showToast", str);
    }
}
