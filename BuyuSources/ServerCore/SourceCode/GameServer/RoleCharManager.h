#pragma once
struct tagRoleCharArray
{
	DWORD		SrcUserID;
	vector<tagRoleCharInfo> Array;
};
class CRoleEx;
class RoleCharManager  //用户聊天管理器  
{
public:
	RoleCharManager();
	virtual ~RoleCharManager();

	bool	OnInit(CRoleEx* pRole);//初始化
	void	OnLoadAllCharInfoByDB(DBO_Cmd_LoadCharInfo* pMsg);//加载全部的聊天消息数据
	//玩家操作
	void    OnLoadCharMapList();//获取全部聊天数据的简易数据
	void    OnLoadAllCharInfoByUserID(DWORD dwSrcUserID);//加载单个玩家的全部的聊天数据
	void	OnAddCharInfo(tagRoleCharInfo& pInfo);//玩家收到一条消息
	void	OnSendCharInfo(tagRoleCharInfo& pInfo);//玩家发送一条消息 

	bool IsLoadDB(){ return m_IsLoadDB; }

	void	OnDelRelation(DWORD dwDestUserID);

	bool GetCharMessageStates();
private:
	CRoleEx*							m_pRole;//玩家
	bool								m_IsLoadDB;
	HashMap<DWORD, tagRoleCharArray>	m_CharMap;//消息列表
};