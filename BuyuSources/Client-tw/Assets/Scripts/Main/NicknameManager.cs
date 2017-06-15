using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public enum NicknameOptType
{
    NICKNAME_UPLOAD,          //玩家上传头像。
    NICKNAME_ME,              //打开自己的头像
    NICKNAME_SCENE,           //场景中获取玩家头像
    NICKNAME_FRIEND,          //好友列表头像。
    NICKNAME_FIND,            //查找好友头像.
    NICKNAME_BLACK,           //黑名单头像
    NICKNAME_RANK,            //排行榜头像
    NICKNAME_MATCH,           //比赛排行榜头像
    NICKNAME_CARDS,           //名片头像
    NICKNAME_NIUNIU,          //牛牛自己头像
    NICKNAME_NIUNIUHOST,      //牛牛庄家头像
    NICKNAME_NIUNIUVIP,       //牛牛vip头像
    NICKNAME_NIUNIUNORMAL,    //牛牛大众席头像
    NICKNAME_MATCHRANK,       //场景中比赛排行榜头像
    NICKNAME_FORESTDANCE,     //森林舞会玩家头像
    NICKNAME_FORESTDANCEVIP,  //森林舞会vip头像
    NICKNAME_FORESTDANCEHOST, //森林舞会庄家头像
    NICKNAME_FORESTDANCENORMAL,//森林舞会大众席头像
    NICKNAME_CAR_SELF, //自己
    NICKNAME_CAR_HOST,  //庄家
    NICKNAME_CAR_PlAYER_LIST,//大众席
    NICKNAME_GAME_SHARE,//分享
}
public class NicknameManager  :Singleton<NicknameManager>
{
    public void Update(float delta)
    {

    }

    public void SetNickname(UILabel label, string nickname, NicknameOptType opt, object extraData = null)
    {
        bool have = SDKMgr.Instance.SDK.HaveNickname();
        bool optValid = false;
        switch (opt)
        {
            case NicknameOptType.NICKNAME_GAME_SHARE:
            case NicknameOptType.NICKNAME_ME:
            case NicknameOptType.NICKNAME_CAR_SELF:
            case NicknameOptType.NICKNAME_UPLOAD:
                optValid = true;
                break;
        }
        if (have && optValid)
        {
            string userID = SDKMgr.Instance.LoginData.UID;
            AsyncNicknameQuery.Instance.SetAsyncNickname(userID, label);
        }
        else
        {
            label.text = nickname;
        }
    }
}
