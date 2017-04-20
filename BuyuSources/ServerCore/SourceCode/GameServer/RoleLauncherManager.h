//玩家炮台控制 也涉及到倍率的控制
//玩家炮台 与物品相关 也与VIP相关 也与月卡相关
#pragma once
class CRoleEx;
class CConfig;
class RoleLauncherManager
{
public:
	RoleLauncherManager();
	virtual ~RoleLauncherManager();
	void			OnInit(CRoleEx* pRole);//当初始化的时候 也就是全部物品已经读取成功后 我们将炮台的数据重新设置下
	void			OnAddItem(DWORD ItemID);//当获得物品的时候
	void			OnDelItem(DWORD ItemID);//当失去物品的时候
	bool			IsCanUserLauncherByID(BYTE LauncherID);//是否可以使用指定的炮台 每次开炮都需要进行判断的

	void			OnVipLevelChange(BYTE OldVipLevel,BYTE NewVipLevel);//当VIP变化的时候
	//void			OnMonthCardChange(BYTE OldMonthCardID, BYTE NewMonthCardID);//月卡发生变化的时候

	bool			IsCanUseLauncherAllTime(BYTE LauncherID);//判断当前炮是否为永久的
private:
	CRoleEx*		m_pRole;
	CConfig*		m_pConfig;
	DWORD			m_LauncherStates;//炮台的状态控制
	HashMap<DWORD, BYTE> m_ItemToLauncherMap;
};