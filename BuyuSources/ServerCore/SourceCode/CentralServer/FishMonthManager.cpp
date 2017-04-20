#include "Stdafx.h"
#include "FishServer.h"
#include "FishMonthManager.h"
#include <algorithm>
#include "..\CommonFile\DBLogManager.h"
extern void SendLogDB(NetCmd* pCmd);
#define RankFilePath "xml\\"
bool SortByMonthSocre(tagRoleMonthCenterInfo* obj1, tagRoleMonthCenterInfo* obj2)//排序处理
{
	if (!obj1 || !obj2)
		return false;
	return (obj1->MonthInfo.dwMonthScore*obj1->VipAddScore) > (obj2->MonthInfo.dwMonthScore*obj2->VipAddScore);//需要添加积分的比例
}
FishMonth::FishMonth()
{
	m_MonthID = 0;
	m_IsInSignUp = false;
	m_IsStart = false;
	m_NowMonthTimeID = 0;
	//m_MonthRankSum = (RandUInt() % 30) + 20;
}
FishMonth::~FishMonth()
{
	OnDestroy();
}
bool FishMonth::OnInit(BYTE MontID)
{
	HashMap<BYTE, tagMonthConfig>::iterator Iter = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(MontID);
	if (Iter == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return false;
	}
	//m_FishMonthConfig = &Iter->second;
	m_MonthID = MontID;
	m_RoleVec.clear();
	m_MonthRoleMap.clear();
	m_IsInSignUp = false;
	m_IsStart = false;
	return true;
}
void FishMonth::OnLoadRobotInfo()
{
	if (!g_FishServer.GetMonthManager().RobotIsFinish())
		return;
	if (!m_IsInSignUp && !m_IsStart)//非报名阶段不加载机器人
		return;
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagMonthRobot>::iterator IterRobot = IterConfig->second.m_RobotMap.begin();
	for (; IterRobot != IterConfig->second.m_RobotMap.end(); ++IterRobot)
	{
		float fValue = 1.0f;
		if (IterRobot->second.SumRate != 0)
			fValue = (RandUInt() % (IterRobot->second.SumRate * 2) + (100 - IterRobot->second.SumRate)) / 100.0f;
		DWORD RobotSum = static_cast<DWORD>(fValue* IterRobot->second.RobotSum);
		for (DWORD i = 0; i < RobotSum; ++i)
		{
			RobotUserInfo * pInfo = g_FishServer.GetRobotManager().GetNonUseRobot();
			if (!pInfo)
			{
				continue;
			}
			//DWORD Level = IterRobot->first;

			tagRoleMonthCenterInfo* pRole = new tagRoleMonthCenterInfo;
			if (!pRole)
			{
				ASSERT(false);
				return;
			}
			pRole->MonthInfo.bMonthID = m_MonthID;
			pRole->MonthInfo.dwIndex = m_RoleVec.size();
			pRole->MonthInfo.dwMonthGlobel = IterConfig->second.InitMonthGlobel;//报名的时候就初始化好积分
			pRole->MonthInfo.dwMonthScore = 0;
			pRole->MonthInfo.dwUserID = pInfo->dwUserID;
			pRole->MonthInfo.wUserAddGlobelNum = 0;
			TCHARCopy(pRole->NickName, CountArray(pRole->NickName), pInfo->NickName, _tcslen(pInfo->NickName));//玩家名称用于记录xml文件使用的
			pRole->FaceID = pInfo->dwFaceID;//头像ID
			pRole->VipAddScore = g_FishServer.GetFishConfig().GetVipConfig().AddMonthScoreRate(pInfo->VipLevel);//比赛积分的提升
			pRole->MonthInfo.SkillSum = 0;
			m_RoleVec.push_back(pRole);
			m_MonthRoleMap.insert(std::map<DWORD, tagRoleMonthCenterInfo*>::value_type(pRole->MonthInfo.dwUserID, pRole));

			tagRobotInfo pRobotInfo;
			pRobotInfo.dwUserID = pInfo->dwUserID;
			pRobotInfo.Level = IterRobot->first;
			pRobotInfo.SkillSum = 0;

			//设置当前未机器人 记录下
			m_RobotMap.insert(HashMap<DWORD, tagRobotInfo>::value_type(pInfo->dwUserID, pRobotInfo));
		}
	}
	OnRoleSumChange();
}
void FishMonth::OnLoadRobotFinish()
{
	OnLoadRobotInfo();//比赛加载机器人
}
void FishMonth::SendMonthStatesToGameServer(BYTE GameConfigID, BYTE MonthStates)
{
	//将比赛的状态发送到GameServer去
	CG_Cmd_MonthStates msg;
	SetMsgInfo(msg, GetMsgType(Main_Month, CG_MonthStates), sizeof(CG_Cmd_MonthStates));
	msg.MonthID = m_MonthID;
	msg.MonthStates = MonthStates;

	if (GameConfigID == 0)
	{
		g_FishServer.GetCenterManager().SendNetCmdToAllGameServer(&msg);
	}
	else
	{
		g_FishServer.GetCenterManager().SendNetCmdToGameServer(GameConfigID, &msg);
	}
}
void FishMonth::OnUpdateRobotInfo()
{
	//30s更新一次 
	//便利全部的机器人 添加机器人的积分
	if (!IsInStart())
		return;
	if (m_RobotMap.empty())
		return;
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRobotInfo>::iterator Iter = m_RobotMap.begin();
	for (; Iter != m_RobotMap.end(); ++Iter)
	{
		HashMap<DWORD, tagRoleMonthCenterInfo*>::iterator IterFind = m_MonthRoleMap.find(Iter->first);
		if (IterFind == m_MonthRoleMap.end())
			continue;
		HashMap<DWORD, tagMonthRobot>::iterator IterRobot = IterConfig->second.m_RobotMap.find(Iter->second.Level);
		if (IterRobot == IterConfig->second.m_RobotMap.end())
			continue;
		if (IterFind->second->MonthInfo.dwMonthScore >= IterRobot->second.MaxScore)
			continue;

		float fValue = 1.0f;
		if (IterRobot->second.Rate != 0)
			fValue = (RandUInt() % (IterRobot->second.Rate * 2) + (100 - IterRobot->second.Rate)) / 100.0f;
		DWORD AddScore = static_cast<DWORD>(IterRobot->second.AddScore * fValue);
		IterFind->second->MonthInfo.dwMonthScore += AddScore;

		//机器人 除了 普通使用积分 并且 有可能 在每次更新的时候 使用技能 
		if (Iter->second.SkillSum < IterRobot->second.SkillSum)
		{
			if (RandUInt() % 100 <= IterRobot->second.SkillUseRate)
			{
				fValue = 1.0f;
				if (IterRobot->second.SkillScoreRate != 0)
					fValue = (RandUInt() % (IterRobot->second.SkillScoreRate * 2) + (100 - IterRobot->second.SkillScoreRate)) / 100.0f;
				DWORD AddScore = static_cast<DWORD>(IterRobot->second.SkillScore * fValue);
				IterFind->second->MonthInfo.dwMonthScore += AddScore;
				IterFind->second->MonthInfo.SkillSum += 1;//技能次数添加1
				Iter->second.SkillSum += 1;

				//g_FishServer.ShowInfoToWin("机器人使用技能 添加积分:%u", AddScore);
			}
		}
	}
}
void FishMonth::OnDestroyRobotInfo()
{
	//在比赛结束的时候 我们将机器人全部归还
	if (m_RobotMap.empty())
		return;
	HashMap<DWORD, tagRobotInfo>::iterator Iter = m_RobotMap.begin();
	for (; Iter != m_RobotMap.end(); ++Iter)
	{
		g_FishServer.GetRobotManager().OnNonUseRobot(Iter->first);//归还机器人
	}
}
void FishMonth::OnDestroy()
{
	//对数据进行销毁
	if (m_RoleVec.empty())
		return;
	std::vector<tagRoleMonthCenterInfo*>::iterator Iter = m_RoleVec.begin();
	for (; Iter != m_RoleVec.end(); ++Iter)
	{
		if (!(*Iter))
			continue;
		delete (*Iter);
	}
	m_RoleVec.clear();
	m_MonthRoleMap.clear();
}
BYTE FishMonth::GetMonthID()
{
	return m_MonthID;
}
void FishMonth::SaveFileToFtp(const char* FilePath,const char* FileName)//将当前比赛的排行数据上传到FTP去
{
	if (!FileName || !FilePath)
	{
		ASSERT(false);
		return;
	}
	if (strlen(FileName) == 0 || strlen(FilePath) == 0)
	{
		ASSERT(false);
		return;
	}
	FILE* pFile = NULL;
	errno_t Error = fopen_s(&pFile, FilePath, "rb+");
	if (Error !=0 || !pFile)
	{
		ASSERT(false);
		return;
	}
	//获取文件的长度 
	fseek(pFile, 0, SEEK_END);
	long FileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	DWORD PageSize = sizeof(CF_Cmd_UpLoadFile)+(FileSize - 1)* sizeof(BYTE);
	CF_Cmd_UpLoadFile* msg = (CF_Cmd_UpLoadFile*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->SetCmdType(GetMsgType(Main_Ftp, CF_UpLoadFile));
	fread_s(msg->Array, FileSize, 1, FileSize, pFile);
	msg->FileSum = FileSize;
	fclose(pFile);
	strncpy_s(msg->FileName, CountArray(msg->FileName), FileName, strlen(FileName));
	
	//函数设置完毕
	std::vector<CF_Cmd_UpLoadFile*> pVec;
	SqlitMsg(msg, PageSize, FileSize, false, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<CF_Cmd_UpLoadFile*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			g_FishServer.SendNetCmdToFtp(*Iter);
			free(*Iter);
		}
		pVec.clear();
	}
}
void FishMonth::UpdateByMin()//比赛的更新 应该和客户端保持一致 就是 0秒的时候更新 不是按分钟走
{
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		//比赛不存在了 可能是配置文件重载了 我们关闭比赛
		OnLeaveSignUp();//结束报名阶段
		OnLeaveStart();//结束比赛阶段
		ASSERT(false);
		return;
	}

	//阶段进行更新处理 很多阶段 
	//可能是同一分钟进行处理的 我们想要判断下
	//需要一个可以唯一标记的ID 用于标记比赛的阶段
	DWORD MonthTimeID = IterConfig->second.GetMonthTimeID();
	if (m_NowMonthTimeID != 0)
	{
		//当前在比赛中 我们判断比赛是否需要结束
		if (m_NowMonthTimeID != MonthTimeID)
		{
			//当前阶段已经结束了
			//让当前阶段结束
			OnLeaveSignUp();//结束报名阶段
			OnLeaveStart();//结束比赛阶段
			m_NowMonthTimeID = MonthTimeID;
		}
		else
		{
			//还在当前阶段 我们判断下
			BYTE TimeID = (BYTE)(MonthTimeID >> 16);
			HashMap<BYTE, tagMonthTime>::iterator Iter = IterConfig->second.m_MonthTime.find(TimeID);
			if (Iter == IterConfig->second.m_MonthTime.end())
			{
				ASSERT(false);
				//时间不存在 立刻结束比赛
				OnLeaveSignUp();
				OnLeaveStart();
			}
			else
			{
				if (!Iter->second.IsInSignTime())
					OnLeaveSignUp();
				if (!Iter->second.IsInStarTime())
					OnLeaveStart();
			}
		}
	}
	else
		m_NowMonthTimeID = MonthTimeID;
	//旧的比赛 已经处理了 开始处理新的比赛
	if (m_NowMonthTimeID != 0)
	{
		//新的阶段  判断 是否开启比赛了 或者 开始报名了
		BYTE TimeID = (BYTE)(MonthTimeID >> 16);
		HashMap<BYTE, tagMonthTime>::iterator Iter = IterConfig->second.m_MonthTime.find(TimeID);
		if (Iter == IterConfig->second.m_MonthTime.end())
		{
			ASSERT(false);
			//时间不存在 立刻结束比赛
			OnLeaveSignUp();
			OnLeaveStart();
		}
		else
		{
			if (Iter->second.IsInSignTime())
				OnJoinSignUp();
			if (Iter->second.IsInStarTime())
				OnJoinStart();
		}
	}
}
BYTE FishMonth::IsInSignUp()
{
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return 0;
	}
	//判断当前时间是否在比赛阶段 进行判断
	HashMap<BYTE, tagMonthTime>::iterator Iter = IterConfig->second.m_MonthTime.begin();
	for (; Iter != IterConfig->second.m_MonthTime.end(); ++Iter)
	{
		if (Iter->second.IsInSignTime())
		{
			return Iter->first;//以ID为标记
		}
	}
	return 0;
}
BYTE FishMonth::IsInStart()
{
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return 0;
	}
	HashMap<BYTE, tagMonthTime>::iterator Iter = IterConfig->second.m_MonthTime.begin();
	for (; Iter != IterConfig->second.m_MonthTime.end(); ++Iter)
	{
		if (Iter->second.IsInStarTime())
		{
			return Iter->first;//以ID为标记
		}
	}
	return 0;
}
void FishMonth::OnJoinSignUp()
{
	//当比赛进入到报名阶段的时候 我们开始做一些准备工作
	if (m_IsInSignUp)
		return;
	if (!m_RoleVec.empty())
	{
		ASSERT(false);
	}
	m_IsInSignUp = true;

	OnLoadRobotInfo();//加载机器人

	SendMonthStatesToGameServer(0,1);
	/*SYSTEMTIME pTime;
	GetLocalTime(&pTime);
	char IpStr[128] = { 0 };
	sprintf_s(IpStr, sizeof(IpStr), "比赛ID:%d  TimeID:%d  进入报名阶段  %d:%d \n", m_FishMonthConfig->MonthID,(m_NowMonthTimeID >> 16), pTime.wHour, pTime.wMinute);
	std::cout << IpStr;*/
}
void FishMonth::OnLeaveSignUp()
{
	if (!m_IsInSignUp)
		return;
	//离开报名的阶段的时候
	m_IsInSignUp = false;
	SendMonthStatesToGameServer(0,2);
	/*SYSTEMTIME pTime;
	GetLocalTime(&pTime);
	char IpStr[128] = { 0 };
	sprintf_s(IpStr, sizeof(IpStr), "比赛ID:%d  TimeID:%d  离开报名阶段  %d:%d \n", m_FishMonthConfig->MonthID, (m_NowMonthTimeID >> 16), pTime.wHour, pTime.wMinute);
	std::cout << IpStr;*/
}
void FishMonth::OnJoinStart()//比赛进入正式阶段
{
	if (m_IsStart)
		return;
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return;
	}

	//判断报名的人数 我们进行设置
	m_IsStart = true;
	SendMonthStatesToGameServer(0,4);
	/*SYSTEMTIME pTime;
	GetLocalTime(&pTime);
	char IpStr[128] = { 0 };
	sprintf_s(IpStr, sizeof(IpStr), "比赛ID:%d  TimeID:%d  进入比赛阶段  %d:%d \n", m_FishMonthConfig->MonthID, (m_NowMonthTimeID >> 16), pTime.wHour, pTime.wMinute);
	std::cout << IpStr;*/

	if (m_RoleVec.empty())
		return;//当前比赛无人参加 无须处理
	std::vector<tagRoleMonthCenterInfo*>::iterator Iter = m_RoleVec.begin();
	for (int i = 0; Iter != m_RoleVec.end(); ++Iter, ++i)
	{
		if (!(*Iter))
			continue;
		//设置当前玩家的一些基本的数据
		(*Iter)->MonthInfo.dwMonthGlobel = IterConfig->second.InitMonthGlobel;
		(*Iter)->MonthInfo.dwMonthScore = 0;
		(*Iter)->MonthInfo.wUserAddGlobelNum = 0;
		(*Iter)->MonthInfo.dwIndex = i;//设置好排名
	}
}
void FishMonth::OnSaveMonthXml(SYSTEMTIME& pTime, std::vector<tagRoleMonthCenterInfo*>& pVec)
{
	//当一轮比赛结束后 我们想要写进XMl文件 Month_ID_Year_Month_Day_Hour_Min.xml
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return;
	}
	if (pVec.empty())
	{
		//无需要排行的数据 无须生成文件
		return;
	}
	BYTE RankSum = IterConfig->second.RankSum;
	char FileName[32] = { 0 };
	sprintf_s(FileName, CountArray(FileName), "Month_%d_%d_%d_%d_%d_%d.xml",m_MonthID, pTime.wYear, pTime.wMonth, pTime.wDay, pTime.wHour, pTime.wMinute);

	tinyxml2::XMLDocument xmlDoc;
	XMLDeclaration* Declaration = xmlDoc.NewDeclaration("xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"");
	if (!Declaration)
	{
		ASSERT(false);
		return;
	}
	xmlDoc.InsertEndChild(Declaration);
	tinyxml2::XMLElement* elmRank = xmlDoc.NewElement("FishMonth");
	if (!elmRank)
	{
		ASSERT(false);
		return;
	}
	elmRank->SetAttribute("MonthID", m_MonthID);
	std::vector<tagRoleMonthCenterInfo*>::iterator Iter = pVec.begin();
	for (int i = 1; Iter != pVec.end() && i <= RankSum; ++Iter)
	{
		if (!(*Iter))
			continue;
		if ((*Iter)->MonthInfo.dwMonthScore == 0)//积分为0不进入排名
			continue;
		tinyxml2::XMLElement* elmRook = xmlDoc.NewElement("RoleInfo");
		if (!elmRook)
			continue;
		elmRook->SetAttribute("Index", i);
		elmRook->SetAttribute("UserID", (unsigned int)(*Iter)->MonthInfo.dwUserID);
		elmRook->SetAttribute("FaceID", (unsigned int)(*Iter)->FaceID);
		char * UNickName = UnicodeToutf8((char*)(*Iter)->NickName);
		elmRook->SetAttribute("NickName", UNickName);
		free(UNickName);
		elmRook->SetAttribute("Socre", (unsigned int)(*Iter)->MonthInfo.dwMonthScore);
		elmRook->SetAttribute("VipSocre", (unsigned int)((*Iter)->MonthInfo.dwMonthScore * (*Iter)->VipAddScore - (*Iter)->MonthInfo.dwMonthScore));
		elmRank->InsertEndChild(elmRook);
		++i;
	}
	xmlDoc.InsertEndChild(elmRank);

	string FilePath = RankFilePath;
	FilePath += FileName;
	xmlDoc.SaveFile(FilePath.c_str());

	SaveFileToFtp(FilePath.c_str(),FileName);
}
void FishMonth::OnLeaveStart()
{
	if (!m_IsStart)
		return;
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return;
	}
	//根据列表里的玩家 我们发送奖励到玩家的身上去
	m_IsStart = false;
	if (m_RoleVec.empty())
		return;
	//将当前比赛的记录保存起来 待处理收尾操作
	m_LastMonthInfo.OnAddInfo(m_NowMonthTimeID,m_RoleVec);//将当前的放入到集合里面去

	//发送命令 到全部的GameServer 让全部的GameServer上传数据
	CG_Cmd_GetUserMonthInfo msg;
	SetMsgInfo(msg, GetMsgType(Main_Month, CG_GetUserMonthInfo), sizeof(CG_Cmd_GetUserMonthInfo));
	msg.MonthID = m_MonthID;
	g_FishServer.GetCenterManager().SendNetCmdToAllGameServer(&msg);

	m_RoleVec.clear();//清空集合 并且删除对象
	m_MonthRoleMap.clear();//一场比赛结束后 清空数据
	OnRoleSumChange();//人数归0

	OnDestroyRobotInfo();

	SendMonthStatesToGameServer(0,8);
}
void FishMonth::OnChangeRolePoint(DWORD dwUserID, DWORD Point)
{
	if (!m_IsStart)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRoleMonthCenterInfo*>::iterator IterFind = m_MonthRoleMap.find(dwUserID);
	if (IterFind == m_MonthRoleMap.end() || !IterFind->second)
	{
		ASSERT(false);
		return;
	}
	std::vector<tagRoleMonthCenterInfo*>::iterator Iter = m_RoleVec.begin() + IterFind->second->MonthInfo.dwIndex;
	if (Iter == m_RoleVec.end() || !(*Iter) || (*Iter)->MonthInfo.dwUserID != dwUserID)
	{
		ASSERT(false);
		return;
	}
	//直接修改玩家的积分
	(*Iter)->MonthInfo.dwMonthScore = Point;
}
void FishMonth::OnChangeRoleGlobel(DWORD dwUserID, DWORD Globel)
{
	//修改玩家的金币
	HashMap<DWORD, tagRoleMonthCenterInfo*>::iterator Iter = m_MonthRoleMap.find(dwUserID);
	if (Iter != m_MonthRoleMap.end() && Iter->second)
	{
		Iter->second->MonthInfo.dwMonthGlobel = Globel;
	}
	else
	{
		ASSERT(false);
	}
}
void FishMonth::OnChangeRoleInfo(DWORD dwUserID, DWORD Point, DWORD Globel, DWORD SkillSum)
{
	HashMap<DWORD, tagRoleMonthCenterInfo*>::iterator Iter = m_MonthRoleMap.find(dwUserID);
	if (Iter != m_MonthRoleMap.end() && Iter->second)
	{
		Iter->second->MonthInfo.dwMonthGlobel = Globel;
		Iter->second->MonthInfo.dwMonthScore = Point;
		Iter->second->MonthInfo.SkillSum = SkillSum;
	}
	else
	{
		ASSERT(false);
	}
}
void FishMonth::OnAddMonthGlobelNum(DWORD dwUserID)
{
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRoleMonthCenterInfo*>::iterator Iter = m_MonthRoleMap.find(dwUserID);
	if (Iter != m_MonthRoleMap.end() && Iter->second)
	{
		Iter->second->MonthInfo.wUserAddGlobelNum += 1;
		//续币后 添加玩家的金币 乐币的扣除 有GameServer进行
		HashMap<BYTE, tagMonthConvert>::iterator IterFind = IterConfig->second.m_ConvertMap.find(Iter->second->MonthInfo.wUserAddGlobelNum);
		if (IterFind != IterConfig->second.m_ConvertMap.end())
		{
			Iter->second->MonthInfo.dwMonthGlobel += IterFind->second.AddMonthGlobel;
		}
		else
		{
			ASSERT(false);
		}
	}
	else
	{
		ASSERT(false);
	}
}
void FishMonth::OnLoadAllGameUserMonthInfo()
{
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return;
	}
	//一个GameServer 已经将全部的积分上传了 
	if (m_LastMonthInfo.OnAddGameServerFinish())
	{
		/*char IpStr[128] = { 0 };
		sprintf_s(IpStr, sizeof(IpStr), "比赛ID:%d  TimeID:%d  离开比赛阶段  %d:%d \n", m_FishMonthConfig->MonthID, (m_LastMonthInfo.m_NowMonthTimeID >> 16), m_LastMonthInfo.pTime.wHour, m_LastMonthInfo.pTime.wMinute);
		std::cout << IpStr;*/

		if (m_LastMonthInfo.Lastm_RoleVec.empty())
			return;
		//对vector 进行排序 必须进行排序
		sort(m_LastMonthInfo.Lastm_RoleVec.begin(), m_LastMonthInfo.Lastm_RoleVec.end(), SortByMonthSocre);//最终排名次 玩家排序
		OnSaveMonthXml(m_LastMonthInfo.pTime,m_LastMonthInfo.Lastm_RoleVec);
		std::vector<tagRoleMonthCenterInfo*>::iterator Iter = m_LastMonthInfo.Lastm_RoleVec.begin();
		for (int i = 0; Iter != m_LastMonthInfo.Lastm_RoleVec.end(); ++Iter)
		{
			//发送命令到GameServer 让玩家离开桌子
			if (!(*Iter))
				continue;


			if ((*Iter)->MonthInfo.dwMonthScore == 0)
			{
				//玩家积分为 0  我们进行处理
				g_DBLogManager.LogMonthInfoToDB((*Iter)->MonthInfo.dwUserID, (*Iter)->MonthInfo.bMonthID, (*Iter)->MonthInfo.dwMonthScore, 0xffffffff, (*Iter)->MonthInfo.wUserAddGlobelNum, (*Iter)->MonthInfo.SkillSum, 0, SendLogDB);

				CG_Cmd_UserMonthEnd msgEnd;
				SetMsgInfo(msgEnd, GetMsgType(Main_Month, CG_UserMonthEnd), sizeof(CG_Cmd_UserMonthEnd));
				msgEnd.dwUserID = (*Iter)->MonthInfo.dwUserID;
				msgEnd.MonthID = m_MonthID;
				msgEnd.MonthIndex = 0xffffffff;//最后一名
				msgEnd.MonthScores = 0;
				msgEnd.VipScores = 0;
				CenterRole* pRole = g_FishServer.GetRoleManager().QueryCenterUser(msgEnd.dwUserID);
				if (pRole)
					pRole->SendDataToGameServer(&msgEnd);
				continue;
			}
			CG_Cmd_UserMonthEnd msgEnd;
			SetMsgInfo(msgEnd,GetMsgType(Main_Month, CG_UserMonthEnd), sizeof(CG_Cmd_UserMonthEnd));
			msgEnd.dwUserID = (*Iter)->MonthInfo.dwUserID;
			msgEnd.MonthID = m_MonthID;
			msgEnd.MonthIndex = i + 1;//名次
			msgEnd.MonthScores = (*Iter)->MonthInfo.dwMonthScore;
			msgEnd.VipScores = static_cast<DWORD>(((*Iter)->VipAddScore - 1.0f) * (*Iter)->MonthInfo.dwMonthScore);
			CenterRole* pRole = g_FishServer.GetRoleManager().QueryCenterUser(msgEnd.dwUserID);
			if (pRole)
				pRole->SendDataToGameServer(&msgEnd);
			else
			{
				//因为玩家不在线 我们想要处理下 玩家获得名次的时候 的情况 
				//1.玩家获得比奖励 玩家获得比赛的金牌奖励等 数据记录情况 DBR命令
				DBR_Cmd_ChangeGameDataByMonth msg;
				SetMsgInfo(msg, DBR_ChangeGameDataByMonth, sizeof(DBR_Cmd_ChangeGameDataByMonth));
				msg.dwUserID = msgEnd.dwUserID;
				msg.MonthIndex = ConvertIntToBYTE(i + 1);
				g_FishServer.SendNetCmdToDB(&msg);
			}

			//给指定玩家发送系统邮件 找到玩家名次对应的奖励
			HashMap<WORD, tagMonthReward>::iterator IterReward = IterConfig->second.m_MonthReward.find(ConvertDWORDToWORD(i + 1));
			if (IterReward == IterConfig->second.m_MonthReward.end())
			{
				g_DBLogManager.LogMonthInfoToDB((*Iter)->MonthInfo.dwUserID, (*Iter)->MonthInfo.bMonthID, msgEnd.MonthScores + msgEnd.VipScores, msgEnd.MonthIndex, (*Iter)->MonthInfo.wUserAddGlobelNum, (*Iter)->MonthInfo.SkillSum, 0, SendLogDB);
				continue;
			}
			//发送邮件奖励 系统邮件直接携带RewardID 进行处理
			tagRoleMail	MailInfo;
			MailInfo.bIsRead = false;
			//比赛的内容需要特殊的处理 我们想要一个 特殊的转义字符串 客户端 和 服务器通用的 
			_sntprintf_s(MailInfo.Context, CountArray(MailInfo.Context), TEXT("恭喜您获得{MonthName:MonthID=%d}元话费赛 的第%d名 得到以下奖励"), m_MonthID, i + 1);
			MailInfo.RewardID = IterReward->second.RewardID;
			MailInfo.RewardSum = 1;
			MailInfo.MailID = 0;
			MailInfo.SendTimeLog = time(NULL);
			MailInfo.SrcFaceID = 0;
			TCHARCopy(MailInfo.SrcNickName, CountArray(MailInfo.SrcNickName), TEXT(""), 0);
			MailInfo.SrcUserID = 0;//系统发送
			MailInfo.bIsExistsReward = (MailInfo.RewardID != 0 && MailInfo.RewardSum!=0);
			DBR_Cmd_AddUserMail msg;
			SetMsgInfo(msg,DBR_AddUserMail, sizeof(DBR_Cmd_AddUserMail));
			msg.dwDestUserID = (*Iter)->MonthInfo.dwUserID;
			msg.MailInfo = MailInfo;
			g_FishServer.SendNetCmdToDB(&msg);	

			g_DBLogManager.LogMonthInfoToDB((*Iter)->MonthInfo.dwUserID, (*Iter)->MonthInfo.bMonthID, msgEnd.MonthScores + msgEnd.VipScores, msgEnd.MonthIndex, (*Iter)->MonthInfo.wUserAddGlobelNum, (*Iter)->MonthInfo.SkillSum, IterReward->second.RewardID, SendLogDB);

			++i;
		}
		m_LastMonthInfo.OnDestroy();
	}
}
void FishMonth::SignUp(DWORD dwUserID)
{
	//1.判断是否为报名时间
	if (!m_IsInSignUp)
	{
		CG_Cmd_SignUpMonth msg;
		SetMsgInfo(msg,GetMsgType(Main_Month, CG_SignUpMonth), sizeof(CG_Cmd_SignUpMonth));
		msg.dwUserID = dwUserID;
		msg.MonthID = m_MonthID;
		msg.Result = false;
		CenterRole* pRole = g_FishServer.GetRoleManager().QueryCenterUser(dwUserID);
		if (pRole)
			pRole->SendDataToGameServer(&msg);
		else
		{
			ASSERT(false);
		}
		return;
	}
	//判断玩家是否已经报名了
	HashMap<DWORD, tagRoleMonthCenterInfo*>::iterator IterFind = m_MonthRoleMap.find(dwUserID);
	if (IterFind != m_MonthRoleMap.end() && IterFind->second)
	{
		CG_Cmd_SignUpMonth msg; 
		SetMsgInfo(msg,GetMsgType(Main_Month, CG_SignUpMonth), sizeof(CG_Cmd_SignUpMonth));
		msg.dwUserID = dwUserID;
		msg.MonthID = m_MonthID;
		msg.Result = false;
		CenterRole* pRole = g_FishServer.GetRoleManager().QueryCenterUser(dwUserID);
		if (pRole)
			pRole->SendDataToGameServer(&msg);
		else
		{
			ASSERT(false);
		}
		return;
	}
	CG_Cmd_SignUpMonth msg;  //发送命令到客户端 去询问玩家是否可以有足够的门票加入比赛
	SetMsgInfo(msg,GetMsgType(Main_Month, CG_SignUpMonth), sizeof(CG_Cmd_SignUpMonth));
	msg.dwUserID = dwUserID;
	msg.MonthID = m_MonthID;
	msg.Result = true;
	CenterRole* pRole = g_FishServer.GetRoleManager().QueryCenterUser(dwUserID);
	if (pRole)
		pRole->SendDataToGameServer(&msg);
	else
	{
		ASSERT(false);
	}
}
void FishMonth::SignUpResult(GC_Cmd_SignUpMonthResult* pMsg)
{
	//GameServer已经扣除玩家的比赛数据了
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return;
	}
	if (!pMsg || !m_IsInSignUp || m_MonthID != pMsg->MonthID)
	{
		ASSERT(false);
		return;
	}
	CenterRole* pUser = g_FishServer.GetRoleManager().QueryCenterUser(pMsg->dwUserID);
	if (!pUser)
	{
		ASSERT(false);
		return;
	}
	HashMap<DWORD, tagRoleMonthCenterInfo*>::iterator IterFind = m_MonthRoleMap.find(pMsg->dwUserID);
	if (IterFind != m_MonthRoleMap.end())
	{
		//ASSERT(false);
		return;
	}
	//玩家可以进行报名了 
	tagRoleMonthCenterInfo* pRole = new tagRoleMonthCenterInfo;
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	pRole->MonthInfo.bMonthID = m_MonthID;
	pRole->MonthInfo.dwIndex = m_RoleVec.size();
	pRole->MonthInfo.dwMonthGlobel = IterConfig->second.InitMonthGlobel;//报名的时候就初始化好积分
	pRole->MonthInfo.dwMonthScore = 0;
	pRole->MonthInfo.dwUserID = pMsg->dwUserID;
	pRole->MonthInfo.wUserAddGlobelNum = 0;
	TCHARCopy(pRole->NickName, CountArray(pRole->NickName), pUser->GetRoleInfo().NickName, _tcslen(pUser->GetRoleInfo().NickName));//玩家名称用于记录xml文件使用的
	pRole->FaceID = pUser->GetRoleInfo().dwFaceID;//头像ID
	pRole->VipAddScore = g_FishServer.GetFishConfig().GetVipConfig().AddMonthScoreRate(pUser->GetRoleInfo().VipLevel);//比赛积分的提升
	pRole->MonthInfo.SkillSum = 0;
	m_RoleVec.push_back(pRole);
	m_MonthRoleMap.insert(std::map<DWORD, tagRoleMonthCenterInfo*>::value_type(pRole->MonthInfo.dwUserID, pRole));

	CG_Cmd_SignUpMonthSucess msg; //告诉GameServer 玩家报名比赛成功了
	SetMsgInfo(msg,GetMsgType(Main_Month, CG_SignUpMonthSucess), sizeof(CG_Cmd_SignUpMonthSucess));
	msg.dwUserID = pMsg->dwUserID;
	msg.MonthID = pRole->MonthInfo.bMonthID;
	pUser->SendDataToGameServer(&msg);

	OnRoleSumChange();
}
void FishMonth::ResetUserMonthInfo(DWORD dwUserID)
{
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return;
	}
	CenterRole* pUser = g_FishServer.GetRoleManager().QueryCenterUser(dwUserID);
	if (!pUser)
	{
		ASSERT(false);
		return;
	}

	CG_Cmd_ResetMonthInfo msg;
	SetMsgInfo(msg, GetMsgType(Main_Month, CG_ResetMonthInfo), sizeof(CG_Cmd_ResetMonthInfo));
	msg.dwUserID = dwUserID;
	msg.MonthID = m_MonthID;
	msg.Result = false;
	//比赛是玩家已经报名 并且 已经开始比赛了
	if (!m_IsStart)
	{
		ASSERT(false);
		pUser->SendDataToGameServer(&msg);
		return;
	}
	HashMap<DWORD, tagRoleMonthCenterInfo*>::iterator Iter = m_MonthRoleMap.find(dwUserID);
	if (Iter == m_MonthRoleMap.end() || !Iter->second)
	{
		ASSERT(false);
		pUser->SendDataToGameServer(&msg);
		return;
	}
	Iter->second->MonthInfo.dwMonthScore = 0;
	Iter->second->MonthInfo.dwMonthGlobel = IterConfig->second.InitMonthGlobel;
	Iter->second->MonthInfo.wUserAddGlobelNum = 0;

	msg.Result = true;
	pUser->SendDataToGameServer(&msg);
}
bool FishMonth::IsCanJoinMonth(DWORD dwUserID, tagRoleMonthInfo& MonthInfo)
{
	if (!m_IsStart)
		return false;
	HashMap<DWORD, tagRoleMonthCenterInfo*>::iterator Iter = m_MonthRoleMap.find(dwUserID);
	if (Iter == m_MonthRoleMap.end() || !Iter->second)//未报名 无法处理
		return false;
	MonthInfo = Iter->second->MonthInfo;
	return true;
}
void FishMonth::JoinMonth(DWORD dwUserID,WORD TableID)
{
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return;
	}
	//玩家需要进入比赛
	CenterRole* pUser = g_FishServer.GetRoleManager().QueryCenterUser(dwUserID);
	if (!pUser)
	{
		ASSERT(false);
		return;
	}
	if (pUser->IsRobot())
	{
		//玩家为机器人 无须报名 直接就可以进入比赛 
		HashMap<DWORD, tagRoleMonthCenterInfo*>::iterator IterFind = m_MonthRoleMap.find(pUser->GetRoleID());
		if (IterFind == m_MonthRoleMap.end())
		{
			tagRoleMonthCenterInfo* pRole = new tagRoleMonthCenterInfo;
			if (!pRole)
			{
				ASSERT(false);
				return;
			}
			pRole->MonthInfo.bMonthID = m_MonthID;
			pRole->MonthInfo.dwIndex = m_RoleVec.size();
			pRole->MonthInfo.dwMonthGlobel = IterConfig->second.InitMonthGlobel;//报名的时候就初始化好积分
			pRole->MonthInfo.dwMonthScore = 0;
			pRole->MonthInfo.dwUserID = dwUserID;
			pRole->MonthInfo.wUserAddGlobelNum = 0;
			TCHARCopy(pRole->NickName, CountArray(pRole->NickName), pUser->GetRoleInfo().NickName, _tcslen(pUser->GetRoleInfo().NickName));//玩家名称用于记录xml文件使用的
			pRole->FaceID = pUser->GetRoleInfo().dwFaceID;//头像ID
			pRole->VipAddScore = g_FishServer.GetFishConfig().GetVipConfig().AddMonthScoreRate(pUser->GetRoleInfo().VipLevel);
			pRole->MonthInfo.SkillSum = 0;
			m_RoleVec.push_back(pRole);
			m_MonthRoleMap.insert(std::map<DWORD, tagRoleMonthCenterInfo*>::value_type(pRole->MonthInfo.dwUserID, pRole));
			//数据插入完毕 我们可以让玩家进入比赛了
			OnRoleSumChange();//比赛人数变化了
			
			CG_Cmd_JoinMonth msg;
			SetMsgInfo(msg, GetMsgType(Main_Month, CG_JoinMonth), sizeof(CG_Cmd_JoinMonth));
			msg.Result = true;
			msg.MonthInfo = pRole->MonthInfo;
			msg.TableID = TableID;
			pUser->SendDataToGameServer(&msg);
		}
		else
		{
			CG_Cmd_JoinMonth msg;
			SetMsgInfo(msg, GetMsgType(Main_Month, CG_JoinMonth), sizeof(CG_Cmd_JoinMonth));
			msg.Result = true;
			msg.MonthInfo = IterFind->second->MonthInfo;
			msg.TableID = TableID;
			pUser->SendDataToGameServer(&msg);
		}
		return;
	}
	if (!m_IsStart)
	{
		CG_Cmd_JoinMonth msg; 
		SetMsgInfo(msg,GetMsgType(Main_Month, CG_JoinMonth), sizeof(CG_Cmd_JoinMonth));
		msg.Result = false;
		msg.MonthInfo.dwUserID = dwUserID;
		msg.TableID = TableID;
		pUser->SendDataToGameServer(&msg);
		return;
	}
	HashMap<DWORD, tagRoleMonthCenterInfo*>::iterator Iter = m_MonthRoleMap.find(dwUserID);
	if (Iter == m_MonthRoleMap.end() || !Iter->second)//未报名 无法处理
	{
		CG_Cmd_JoinMonth msg; 
		SetMsgInfo(msg,GetMsgType(Main_Month, CG_JoinMonth), sizeof(CG_Cmd_JoinMonth));
		msg.Result = false;
		msg.MonthInfo.dwUserID = dwUserID;
		msg.TableID = TableID;
		pUser->SendDataToGameServer(&msg);
		return;
	}
	//已经报名成功了 并且当前已经处于比赛了
	CG_Cmd_JoinMonth msg; 
	SetMsgInfo(msg,GetMsgType(Main_Month, CG_JoinMonth), sizeof(CG_Cmd_JoinMonth));
	msg.Result = true;
	msg.MonthInfo = Iter->second->MonthInfo;
	msg.TableID = TableID;
	pUser->SendDataToGameServer(&msg);
}
void FishMonth::UpdateMonthIndex()
{
	//对vector进行排序
	if (m_RoleVec.empty() || !m_IsStart)//只有在比赛中 并且人数大于0才进行更新
		return;
	sort(m_RoleVec.begin(), m_RoleVec.end(), SortByMonthSocre);
	//排序完毕后便利 从大到小
	DWORD UpperMonthSocre = 0;
	std::vector<tagRoleMonthCenterInfo*>::iterator Iter = m_RoleVec.begin();
	for (size_t i = 0; Iter != m_RoleVec.end(); ++Iter,++i)//便利全部的玩家 必须使用vector  保证 比赛的排名 HashMap因为不保证顺序 
	{
		if (!(*Iter))
			continue;
		if ((*Iter)->MonthInfo.dwIndex != i)
		{
			//玩家的排名改变了 我们修改玩家的排名
			(*Iter)->MonthInfo.dwIndex = i;
			CG_Cmd_UserChangeIndex msgSrc; 
			SetMsgInfo(msgSrc,GetMsgType(Main_Month, CG_UserChangeIndex), sizeof(CG_Cmd_UserChangeIndex));
			msgSrc.dwUserID = (*Iter)->MonthInfo.dwUserID;
			msgSrc.dwIndex = i;
			msgSrc.dwUpperSocre = UpperMonthSocre;
			CenterRole* pRole = g_FishServer.GetRoleManager().QueryCenterUser(msgSrc.dwUserID);
			if (pRole)
				pRole->SendDataToGameServer(&msgSrc);
		}
		UpperMonthSocre = (*Iter)->MonthInfo.dwMonthScore;
	}

	//排序完毕后 我们生成一个列表发送到GameServer去 并且 转发到客户端去
	CG_Cmd_MonthList msg;
	SetMsgInfo(msg, GetMsgType(Main_Month, CG_MonthList), sizeof(CG_Cmd_MonthList));
	msg.MonthID = GetMonthID();
	ZeroMemory(&msg.MonthInfo, sizeof(tagMonthRoleInfo)*MAX_MonthList_Length);
	for (BYTE i = 0; i < MAX_MonthList_Length && i<m_RoleVec.size(); ++i)
	{
		msg.MonthInfo[i].dwUserID = m_RoleVec[i]->MonthInfo.dwUserID;
		msg.MonthInfo[i].FaceID = m_RoleVec[i]->FaceID;
		msg.MonthInfo[i].Score = m_RoleVec[i]->MonthInfo.dwMonthScore;
		msg.MonthInfo[i].VipScore = static_cast<DWORD>(msg.MonthInfo[i].Score * m_RoleVec[i]->VipAddScore - msg.MonthInfo[i].Score);
		msg.MonthInfo[i].Index = m_RoleVec[i]->MonthInfo.dwIndex;
		TCHARCopy(msg.MonthInfo[i].NickName, CountArray(msg.MonthInfo[i].NickName), m_RoleVec[i]->NickName, _tcslen(m_RoleVec[i]->NickName));//玩家名称用于记录xml文件使用的
	}
	g_FishServer.GetCenterManager().SendNetCmdToAllGameServer(&msg);//将命令发送到全部的GameServer去
}
void FishMonth::OnRoleSumChange()
{
	HashMap<BYTE, tagMonthConfig>::iterator IterConfig = g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.find(m_MonthID);
	if (IterConfig == g_FishServer.GetFishConfig().GetMonthConfig().m_MonthMap.end())
	{
		ASSERT(false);
		return;
	}
	CG_Cmd_SendMonthRoleSum msg;
	SetMsgInfo(msg, GetMsgType(Main_Month, CG_SendMonthRoleSum), sizeof(CG_Cmd_SendMonthRoleSum));
	msg.Sum = 1;
	msg.Array[0].MonthID = IterConfig->second.MonthID;
	msg.Array[0].MonthUserSum = GetSigupSum();
	msg.States = MsgEnd;
	g_FishServer.GetCenterManager().SendNetCmdToAllGameServer(&msg);
}
//----------------------------------------------------------------------------------------------------------------------Manager
FishMonthManager::FishMonthManager()
{
	m_FishMonthMap.clear();
	m_RobotIsFinish = false;
}
FishMonthManager::~FishMonthManager()
{
	if (!m_FishMonthMap.empty())
	{
		HashMap<BYTE, FishMonth*>::iterator Iter = m_FishMonthMap.begin();
		for (; Iter != m_FishMonthMap.end(); ++Iter)
		{
			delete Iter->second;
		}
		m_FishMonthMap.clear();
	}
}
bool FishMonthManager::OnInit()
{
	//将所有的比赛创建出来
	tagMonthMap& pMap = g_FishServer.GetFishConfig().GetMonthConfig();
	HashMap<BYTE, tagMonthConfig>::iterator Iter = pMap.m_MonthMap.begin();
	for (; Iter != pMap.m_MonthMap.end(); ++Iter)
	{
		FishMonth* pMonth = new FishMonth();
		if (!pMonth)
		{
			ASSERT(false);
			continue;
		}
		if (!pMonth->OnInit(Iter->first))//将所有配置好的比赛创建出来
		{
			delete pMonth;
			ASSERT(false);
			return false;
		}
		m_FishMonthMap.insert(HashMap<BYTE, FishMonth*>::value_type(pMonth->GetMonthID(), pMonth));
	}
	//OnSendMonthDataToGame(0);//初始化的时候同步比赛数据
	return true;
}
void FishMonthManager::OnLoadRobotFinish()
{
	m_RobotIsFinish = true;
	HashMap<BYTE, FishMonth*>::iterator Iter = m_FishMonthMap.begin();
	for (; Iter != m_FishMonthMap.end(); ++Iter)
	{
		Iter->second->OnLoadRobotFinish();
	}
}
void FishMonthManager::Update(DWORD dwTimer)
{
	UpdateByMin(dwTimer);//一分钟更新一次 用于控制比赛的开启 和结束
	UpdateRobot(dwTimer);//30s 更新一次 更新机器人的数据
	UpdateMonthIndex(dwTimer);//10S 更新一次 用于控制所有比赛的玩家的积分排名
	//UpdateMonthRoleSunToGameServer(dwTimer);//将参加比赛的人数发送到GameServer去 10S一次
}
void FishMonthManager::UpdateByMin(DWORD dwTimer)//不能只是分钟 必须是每一分钟的0秒的时候(0-59) 
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	static BYTE LogUpdateMin = 0xff;//上次更新记录的分钟数
	if (time.wSecond == 0 && (LogUpdateMin == 0xff || time.wMinute != LogUpdateMin))
	{
		LogUpdateMin = (BYTE)time.wMinute;//记录上次更新
		if (m_FishMonthMap.empty())
			return;
		HashMap<BYTE, FishMonth*>::iterator Iter = m_FishMonthMap.begin();
		for (; Iter != m_FishMonthMap.end(); ++Iter)
		{
			Iter->second->UpdateByMin();
		}
	}
	else if (LogUpdateMin == 0xff)
	{
		//为0xff 的时候  是初始化更新 直接处理
		LogUpdateMin = (BYTE)time.wMinute;//记录上次更新
		if (m_FishMonthMap.empty())
			return;
		HashMap<BYTE, FishMonth*>::iterator Iter = m_FishMonthMap.begin();
		for (; Iter != m_FishMonthMap.end(); ++Iter)
		{
			Iter->second->UpdateByMin();
		}
	}
}
void FishMonthManager::UpdateMonthIndex(DWORD dwTimer)//更新各个比赛的排名
{
	static DWORD MonthUpdateIndex = 0;
	if (dwTimer - MonthUpdateIndex < 30000)
		return;
	MonthUpdateIndex = dwTimer;
	if (m_FishMonthMap.empty())
		return;
	HashMap<BYTE, FishMonth*>::iterator Iter = m_FishMonthMap.begin();
	for (; Iter != m_FishMonthMap.end(); ++Iter)
	{
		if (!Iter->second)
			continue;
		Iter->second->UpdateMonthIndex();
	}
}
void FishMonthManager::UpdateRobot(DWORD dwTimer)
{
	static DWORD MonthUpdateRobot = 0;
	if (dwTimer - MonthUpdateRobot < 30000)
		return;
	MonthUpdateRobot = dwTimer;
	if (m_FishMonthMap.empty())
		return;
	HashMap<BYTE, FishMonth*>::iterator Iter = m_FishMonthMap.begin();
	for (; Iter != m_FishMonthMap.end(); ++Iter)
	{
		if (!Iter->second)
			continue;
		Iter->second->OnUpdateRobotInfo();
	}
}
//void FishMonthManager::UpdateMonthRoleSunToGameServer(DWORD dwTimer)
//{
//	static DWORD MonthUpdateRoleSum = 0;
//	if (dwTimer - MonthUpdateRoleSum < 10000)//每10秒 将全部的比赛人数同步给玩家 begin ,,finish
//		return;
//	MonthUpdateRoleSum = dwTimer;
//	//将比赛发送给全部的GameServer 去
//	//变长命令 分包发送等数据
//	NetCmd pCmd;
//	SetMsgInfo(pCmd,GetMsgType(Main_Month, CG_SendMonthRoleSumBegin), sizeof(NetCmd));
//	g_FishServer.GetCenterManager().SendNetCmdToAllGameServer(&pCmd);
//	WORD wSendSize = 0;
//	CG_Cmd_SendMonthRoleSum * msg = (CG_Cmd_SendMonthRoleSum*)malloc(SOCKET_TCP_PACKET);
//	DWORD PageSize = sizeof(CG_Cmd_SendMonthRoleSum)-sizeof(tagMonthRoleSum);
//	CheckMsgSize(PageSize);
//	int DelIndex = 0;
//	int i = 0;
//
//	msg->SetCmdType(GetMsgType(Main_Month, CG_SendMonthRoleSum));
//	msg->SetCmdSize((WORD)PageSize);
//
//	HashMap<BYTE, FishMonth*>::iterator Iter = m_FishMonthMap.begin();
//	for (; Iter != m_FishMonthMap.end(); ++Iter)
//	{
//		if (!Iter->second->MonthIsNeedSendPlayerSum())//在比赛或者 报名中 同步玩家人数
//			continue;
//
//		if (PageSize + sizeof(tagMonthRoleSum) >= SOCKET_TCP_PACKET)
//		{
//			//当前包已经满了 分包发送
//			g_FishServer.GetCenterManager().SendNetCmdToAllGameServer(msg);
//			PageSize = sizeof(CG_Cmd_SendMonthRoleSum)-sizeof(tagMonthRoleSum);
//			DelIndex = i;
//			free(msg);
//			msg = (CG_Cmd_SendMonthRoleSum*)malloc(SOCKET_TCP_PACKET);
//			msg->SetCmdType(GetMsgType(Main_Month, CG_SendMonthRoleSum));
//			msg->SetCmdSize((WORD)PageSize);
//		}
//		msg->MonthSum = i - DelIndex + 1;
//		msg->MonthInfo[i - DelIndex].MonthID = Iter->second->GetMonthID();
//		msg->MonthInfo[i - DelIndex].MonthUserSum = Iter->second->GetSigupSum();
//		PageSize += sizeof(tagItemInfo);
//		msg->SetCmdSize((WORD)PageSize);
//		++i;
//	}
//	if (PageSize >= sizeof(CG_Cmd_SendMonthRoleSum))
//	{
//		g_FishServer.GetCenterManager().SendNetCmdToAllGameServer(msg);
//		free(msg);
//	}
//	else if (msg)
//	{
//		free(msg);
//	}
//
//	NetCmd pCmdFinish;
//	SetMsgInfo(pCmdFinish,GetMsgType(Main_Month, CG_SendMonthRoleSumFinish), sizeof(NetCmd));
//	g_FishServer.GetCenterManager().SendNetCmdToAllGameServer(&pCmdFinish);
//}
FishMonth* FishMonthManager::QueryMonth(BYTE MonthID)
{
	HashMap<BYTE, FishMonth*>::iterator Iter = m_FishMonthMap.find(MonthID);
	if (Iter == m_FishMonthMap.end() || !Iter->second)
		return NULL;
	else
		return Iter->second;
}
void FishMonthManager::SendRoleSignUpInfoToGameServer(DWORD dwUserID)
{
	CenterRole* pRole = g_FishServer.GetRoleManager().QueryCenterUser(dwUserID);
	if (!pRole)
	{
		ASSERT(false);
		return;
	}
	DWORD PageSize = sizeof(CG_Cmd_LoadRoleSignUp)+(m_FishMonthMap.size() - 1)*sizeof(BYTE);
	CG_Cmd_LoadRoleSignUp* msg = (CG_Cmd_LoadRoleSignUp*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->dwUserID = dwUserID;
	msg->SetCmdType(GetMsgType(Main_Month, CG_LoadRoleSignUp));
	HashMap<BYTE, FishMonth*>::iterator Iter = m_FishMonthMap.begin();
	WORD SignUpMonthSum = 0;
	for (; Iter != m_FishMonthMap.end(); ++Iter)
	{
		if (!Iter->second)
			continue;
		if (Iter->second->RoleIsInMonth(dwUserID))
		{
			msg->Array[SignUpMonthSum] = Iter->second->GetMonthID();
			++SignUpMonthSum;
		}
	}
	PageSize = sizeof(CG_Cmd_LoadRoleSignUp)+(SignUpMonthSum - 1)*sizeof(BYTE);

	std::vector<CG_Cmd_LoadRoleSignUp*> pVec;
	SqlitMsg(msg, PageSize, SignUpMonthSum,false, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<CG_Cmd_LoadRoleSignUp*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			pRole->SendDataToGameServer(*Iter);
			free(*Iter);
		}
		pVec.clear();
	}
}
void FishMonthManager::ResetUserMonthInfo(DWORD dwUserID, BYTE MonthID)
{
	//重置指定比赛数据 
	FishMonth* pMonth = QueryMonth(MonthID);
	if (!pMonth)
	{
		ASSERT(false);
		return;
	}
	pMonth->ResetUserMonthInfo(dwUserID);
}
void FishMonthManager::OnGameRsg(BYTE GameID)
{
	HashMap<BYTE, FishMonth*>::iterator Iter = m_FishMonthMap.begin();
	for (; Iter != m_FishMonthMap.end(); ++Iter)
	{
		BYTE States = 0;
		if (Iter->second->IsInSignUp())
			States |= 1;
		else
			States |= 2;

		if (Iter->second->IsInStart())
			States |= 4;
		else
			States |= 8;

		Iter->second->SendMonthStatesToGameServer(GameID, States);
	}
}
void FishMonthManager::OnSendMonthDataToGame(BYTE GameID)
{
	//将比赛的数据同步到GameServer去
	//GameSocketID ==0 的时候 表示是全部的GameServer 否则指的是单独的一个GameServer为
	//0.比赛人数发送给GameServer 并且在更新的时候 进行更新 
	//1.组织命令
	DWORD PageSize = sizeof(CG_Cmd_SendMonthRoleSum)+sizeof(tagMonthRoleSum)* (m_FishMonthMap.size() - 1);
	CG_Cmd_SendMonthRoleSum * msg = (CG_Cmd_SendMonthRoleSum*)malloc(PageSize);
	if (!msg)
	{
		ASSERT(false);
		return;
	}
	msg->SetCmdType(GetMsgType(Main_Month, CG_SendMonthRoleSum));
	HashMap<BYTE, FishMonth*>::iterator Iter = m_FishMonthMap.begin();
	for (WORD i = 0; Iter != m_FishMonthMap.end(); ++Iter, ++i)
	{
		if (!Iter->second)
			continue;
		msg->Array[i].MonthID = Iter->first;
		msg->Array[i].MonthUserSum = Iter->second->GetSigupSum();//报名的人数 当前阶段的人数
	}

	std::vector<CG_Cmd_SendMonthRoleSum*> pVec;
	SqlitMsg(msg, PageSize, m_FishMonthMap.size(),false, pVec);
	free(msg);
	if (!pVec.empty())
	{
		std::vector<CG_Cmd_SendMonthRoleSum*>::iterator Iter = pVec.begin();
		for (; Iter != pVec.end(); ++Iter)
		{
			if (GameID == 0)
			{
				g_FishServer.GetCenterManager().SendNetCmdToAllGameServer(*Iter);
			}
			else
			{
				g_FishServer.GetCenterManager().SendNetCmdToGameServer(GameID, *Iter);
			}
			free(*Iter);
		}
		pVec.clear();
	}
}
//void FishMonthManager::OnLogonToMonth(ServerClientData* pClient,DWORD dwUserID, BYTE MonthID)
//{
//	FishMonth* pMonth = QueryMonth(MonthID);
//	tagRoleMonthInfo MonthInfo;
//	if (!pMonth || !pMonth->IsCanJoinMonth(dwUserID, MonthInfo))
//	{
//		ASSERT(false);
//		CG_Cmd_LogonToMonthTable msg;
//		SetMsgInfo(msg, GetMsgType(Main_Month, CG_LogonToMonthTable), sizeof(CG_Cmd_LogonToMonthTable));
//		msg.dwUserID = dwUserID;
//		msg.MonthID = MonthID;
//		msg.Result = false;
//		g_FishServer.SendNetCmdToClient(pClient, &msg);
//		return;
//	}
//	//玩家可以加入比赛
//	CG_Cmd_LogonToMonthTable msg;
//	SetMsgInfo(msg, GetMsgType(Main_Month, CG_LogonToMonthTable), sizeof(CG_Cmd_LogonToMonthTable));
//	msg.dwUserID = dwUserID;
//	msg.MonthID = MonthID;
//	msg.Result = true;
//	msg.MonthInfo = MonthInfo;
//	g_FishServer.SendNetCmdToClient(pClient, &msg);
//	return;
//}


bool LastMonthRewardInfo::OnAddGameServerFinish()
{
	if (Lastm_RoleVec.empty())
	{
		ASSERT(false);
		return false;
	}
	LoadSum++;
	if (LoadSum == g_FishServer.GetCenterManager().GetGameServerSum())
		return true;
	else
		return false;
}
