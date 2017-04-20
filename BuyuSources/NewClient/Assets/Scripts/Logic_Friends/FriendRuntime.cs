using UnityEngine;
using System.Collections;

class FriendRuntime 
{
    static public FriendRuntime Instance;
    public FriendSysMgr FriendSys;
	
    public static void Init(FriendSysMgr Friendsys)
    {
        Instance = new FriendRuntime();
        Instance.FriendSys = Friendsys;
    }
    public static FriendSysMgr FriendSysMgr
    {
        get
        {
            return Instance.FriendSys;
        }
    }
    public static FriendWndUI  FriendUI
    {
        get
        {
            return FriendSysMgr.FriendWndUI;
        }
    }
    public static EmailWndUI EmailUI
    {
        get
        {
            return FriendSysMgr.EmailWndUI;
        }
    }
    public static PresentWndUI  PresentUI
    {
        get
        {
            return FriendSysMgr.PresentWndUI;
        }
    }
    public static SearchWndUI SearchUI
    {
        get
        {
            return FriendSysMgr.SearchWndUI;
        }
    }
    public static SendMsgWndUI SendMsgUI
    {
        get 
        {
            return FriendSysMgr.SendMsgWndUI;
        }
    }
    public static BlacklistWndUI BlacklistUI
    {
        get
        {
            return FriendSysMgr.BlacklistWndUI;
        }
    }
    public static ApplyCodeWnd ApplyCodeUI
    {
        get
        {
            return FriendSysMgr.ApplyCodeWnd;
        }
    }
    public static void Shutdown()
    {
        Instance = null;
    }
}
