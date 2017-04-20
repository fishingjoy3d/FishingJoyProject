using System;
using System.Collections.Generic;

public class SDK
{
    public const uint RESULT_OK         = 112501;    //成功的代码
    public const uint RESULT_FAILED     = 112500;    //失败的代码
    public const uint CHANNEL_HAS_EXIT  = 112108;    //渠道有退出界面。
    public const uint LOGOUT            = 160105;    //登出
    public const string ENTER_SERVER    = "enterServer";
    public const string CREATE_ROLE     = "createRole";
    public const string LEVEL_UP        = "levelUp";
}
public enum LoginState
{
    LOGIN_NONE,
    LOGIN_BEGIN,
    LOGIN_FAILED,
    LOGIN_OK,
}
public class SDKLoginResult
{
    public LoginState Result; //是否登录成功
    public string   UID;
    public string   ChannelLabel;
    public string   ChannelUid;
    public string   ProductCode;
    public string   ChannelCode;
    public string   Token;
    public string   UserName;
    public string   CustomParams;
}
public interface ISDKCallback
{
    void InitCallback(bool bResult);
    /*
    登录回调 
    userName 用户名，渠道提供，username存在为空或者相同的情况，请勿使用 username作为用户标识
    customParams 为登陆接口（login）传入的自定义参数，此处原样返回
    channelCode 渠道编码，用于标识不同的渠道，可在开发者后台查看，平台提供
    uid 统一Userid，针对多渠道均不会存在userid冲突问题，可作为唯一标识用 户标志，平台提供
    token 渠道令牌，标识用户登陆状态是否有效，渠道提供
    productCode 产品代码，由开发者后台生成，平台提供
    channelUid 渠道用户id，渠道提供
    channelLabel 渠道标识，游戏开发者可根据此字段区分渠道，渠道对应关系请参看文档《如何区分渠道》
     */
    void LoginCallback(SDKLoginResult result);
    void PayCallback(bool bResult);
    void LogoutCallback(string customParams);
    void ExitCallback(bool showExitUI);
}
public interface ISDKInterface
{
    void GlobalInit();
    //初始化
    void Init();
    //登录
    void Login(string customparms); 
    //登出
    /*
    customparms 为自定义参数，在登出结果回调中会原样返回。
    接口说明：

    登出操作用于用户切换帐号等事件处理，有如下不同处理：
    a.游戏中存在登出或者切换帐号的按钮，则可在点击按钮时进行登出接口调用，在登出回调中进行重新登录等操作。
    b.游戏中不存在登出或者切换帐号的按钮时，建议修改游戏添加登出或切换帐号按钮， 若实在无法添加，可在退出游戏前调用登出接口，这种情况下会存在部分渠道会审核不通过的情况，需游戏方与渠道去进行沟通。
     */
    void Logout(string customparms);
    //支付
    /*
    注意：支付收到成功的回调后，请到游戏服务器查询订单状态再进行道具发放，请勿直接做支付成功的UI提示。支付收到失败的回调后，可以进行明确的充值失败提示。
     
    amount 定额支付总金额，单位为人民币分
    itemName 游戏币名称，如金币、钻石等
    count 购买商品数量，如100钻石，传入100；10魔法石，传入10
    chargePointName 计费点名称，没有时可传空
    customParams 游戏开发者自定义字段，会与支付结果一起通知到游戏服务器，游戏服务器可通过该字段判断交易的详细内容（金额、角色等）
    callBackUrl 支付结果通知地址，支付完成后我方后台会向该地址发送支付通知
     */
    void Pay(int amount, string itemName, int count, string chargePointName, string customParams, int itemID);
    //退出游戏
    /*
    在游戏退出前调用退出接口，会有如下不同处理：
    a.渠道存在退出界面，如91、360等，会收到resultCode为LJSDK.CHANNEL_EXIT的回调，此时游戏方只需在回调中进行退出游戏操作即可，无需再弹退出界面；
    b.渠道不存在退出界面，如百度移动游戏等，会收到resultCode为LJSDK.GAME_EXIT的回调，此时游戏方需在回调中弹出自己的游戏退出确认界面，否则会出现渠道审核不通过情况；
     */
    void Exit();

    //用户信息扩展接口
    /*
    参数说明：
    id 当前情景，目前支持 enterServer，levelUp，createRole
    roleId 当前登录的玩家角色ID，必须为数字，若无，传入userid
    roleName 当前登录的玩家角色名，不能为空，不能为null，若无，传入”游戏名称+username”，，如”刀塔传奇风吹来的鱼”
    roleLevel 当前登录的玩家角色等级，必须为数字，若无，传入1
    zoneId 当前登录的游戏区服ID，必须为数字，若无，传入1
    zoneName 当前登录的游戏区服名称，不能为空，不能为null，若无，传入游戏名称+”1区”，如”刀塔传奇1区”
    balance 当前用户游戏币余额，必须为数字，若无，传入0
    vip 当前用户VIP等级，必须为数字，若无，传入1
    partyName 当前用户所属帮派，不能为空，不能为null，若无，传入”无帮派”

    接口说明：
    上传数据接口需在三处调用，分别为进入服务器、玩家升级、玩家创建用户角色，同一方法，通过id来区分，有如下三点需注意：
    a.游戏中该接口必须调用三次，传入不同的id（LJSDK.enterServer，LJSDK.createRole,LJSDK.levelUp），以区分不同的上传数据；
    b.若游戏中无对应接口功能，如游戏中无需创建角色，则可根据自身情况在合适位置进行调用，如登录成功后；
    c.在上传游戏数据时，若存在无对应字段值的情况，可传入默认值，具体参见参数说明；
    d.游戏中动态变化的值不能写死，如有多个服务器，则zoneId不能传入固定值，部分渠道会进行数据验证，若写死会导致无法通过上线审核。
     */
    void SetExtraData(string id, string roleId, string roleName, int roleLevel, int zoneId, string zoneName, int balance, int vip, string partyName);

    //获取渠道的名称
    string GetChannelLabel();
    string GetChannelName();
    //提示玩家登陆的UI
    void ShowAndroidToast(string info);
    //释放资源
    void ReleaseResource();
}
