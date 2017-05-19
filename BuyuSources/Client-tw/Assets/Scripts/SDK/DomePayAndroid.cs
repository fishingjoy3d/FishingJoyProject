using UnityEngine;
using System;
using System.Collections.Generic;
/**
 * Pay script class for encapsulating java api in libs\DomePay.jar.
 */
public class DomePayAndroid
{

	// The reflected class of java api of DomePay.jar
	private AndroidJavaClass klass;
	// Use this for initialization
	public AndroidJavaObject curActivity;

	// The instance of pay script.
	private static DomePayAndroid _instance;
	public static DomePayAndroid Instance
	{
		get
		{
			if (_instance == null)
			{
				_instance = new DomePayAndroid();
			}
			return _instance;
		}
	}

	/**
	 * Initialize DomePayAndroid environment.It should be invoked at the beginning of DomePayAndroidDemo.cs
	 */
	public void DomePayCSInit()
	{
		AndroidJavaClass unityPlayer = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
		curActivity = unityPlayer.GetStatic<AndroidJavaObject>("currentActivity");
		klass = new AndroidJavaClass("com.dome.sdk.api.GamePayUnity");
	}

	/**
	 * Initialize pay SDK's instance.It should be invoked at the beginning of the app.
	 * @param main The activity environment for SDK's UI displaying.
	 */
	public void InitializeApp(String appCode, String loginNo, String gameObject, String runtimeScriptMethod)
	{
		curActivity.Call("runOnUiThread", new AndroidJavaRunnable(() =>
		{
			klass.CallStatic("initSDK", curActivity, appCode, loginNo, gameObject, runtimeScriptMethod);
		}));
	}

	/**
	 * Start pay view with special payInfo.
	 * @param payInfo to charge.
	 * @param gameObject Game Object in Unity Games.
	 * @param runtimeScriptMethod The runtime script method which implement the callback of getting pay result.
	 */
	public void pay(String payInfo, String gameObject, String runtimeScriptMethod)
	{
		curActivity.Call("runOnUiThread", new AndroidJavaRunnable(() =>
		{
			klass.CallStatic("pay", curActivity, payInfo, gameObject, runtimeScriptMethod);
		}));
	}

	/**
     * Start SDK's exit UI.
     */
	public void ExitWithUI(String gameObject, String runtimeScriptMethod)
	{
		curActivity.Call("runOnUiThread", new AndroidJavaRunnable(() =>
		{
			klass.CallStatic("exit", curActivity, gameObject, runtimeScriptMethod);
		}));
	}


	/**
     * Release SDK's instance.
     */
	public void Exit()
	{
		curActivity.Call("runOnUiThread", new AndroidJavaRunnable(() =>
		{
			klass.CallStatic("exitApp");
		}));

	}

	/**
	 *  Life cycle
	 */
	public void onResume()
	{
		curActivity.Call("runOnUiThread", new AndroidJavaRunnable(() =>
		{
			klass.CallStatic("onResume");
		}));

	}

	public void onPause()
	{
		curActivity.Call("runOnUiThread", new AndroidJavaRunnable(() =>
		{
			klass.CallStatic("onPause");
		}));

	}

	public void onStop()
	{
		curActivity.Call("runOnUiThread", new AndroidJavaRunnable(() =>
		{
			klass.CallStatic("onStop");
		}));

	}

	public void onDestory()
	{
		curActivity.Call("runOnUiThread", new AndroidJavaRunnable(() =>
		{
			klass.CallStatic("onDestory");
		}));
	}

	/**
	 * Result of pay action.
	 */
	public class PayResult
	{
		/** No pay action */
		public const int NONE = 0;

		/** Pay success */
		public const int SUCCESS = 1;

		/** Pay failed, such as pay info is not ready */
		public const int FAILED = 2;

		/** Pay canceled, such as use cancel to purchase it in pay ui.*/
		public const int CANCELLED = 3;
	}

	/** Result for login action. */
	public class LoginResult
	{
		// No login action.
		public const int NONE = 0;

		// Login success 
		public const int SUCCESS = 1;

		// Login failed 
		public const int FAILED = 2;

		// Login canceled 
		public const int CANCELED = 3;
	}
}