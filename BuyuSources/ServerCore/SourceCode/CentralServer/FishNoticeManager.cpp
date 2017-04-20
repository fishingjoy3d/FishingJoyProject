#include "Stdafx.h"
#include "FishNoticeManager.h"
#include "FishServer.h"
FishNoticeManager::FishNoticeManager()
{

}
FishNoticeManager::~FishNoticeManager()
{

}
void FishNoticeManager::OnReLoadFishNotcieConfig()
{
	//重新加载全部的公告数据
	g_FishServer.GetFishConfig().LoadFishNoticeConfig(TEXT("FishNoticeConfig.xml"));
}
void FishNoticeManager::OnUpdateByMin(DWORD dwTimer)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	static BYTE LogNoticeUpdateMin = 0xff;//上次更新记录的分钟数
	if (time.wSecond == 0 && (LogNoticeUpdateMin == 0xff || time.wMinute != LogNoticeUpdateMin))
	{
		LogNoticeUpdateMin = (BYTE)time.wMinute;//记录上次更新
		OnUpdateByZeroSec();
	}
}
void FishNoticeManager::OnUpdateByZeroSec()
{
	//每分钟0秒进行更新	
	HashMap<DWORD, tagNotice>& pMap = g_FishServer.GetFishConfig().GetAllNoticeInfo();
	if (pMap.empty())
	{
		return;
	}
	time_t NowTime = time(null);
	HashMap<DWORD, tagNotice>::iterator Iter = pMap.begin();
	for (; Iter != pMap.end(); ++Iter)
	{
		//处理每一个公告
		OnHandleOnceNotice(Iter->second, NowTime);
	}
}
void FishNoticeManager::OnHandleOnceNotice(tagNotice& pNotcie,time_t NowTime)
{
	//处理当前公告
	if (pNotcie.BeginTime > NowTime || (pNotcie.BeginTime + (pNotcie.StepNum - 1) *pNotcie.StepMin*60) < NowTime)
		return;
	DWORD DiffSec = static_cast<DWORD>(NowTime - pNotcie.BeginTime);
	//处理公告的时候 因为外部保证是一分钟一次的 我们可以在内部按分钟进行结算
	DWORD ChangeSec = (DiffSec % (pNotcie.StepMin * 60));
	if (ChangeSec < 60)
	{
		g_FishServer.SendMessageByType(pNotcie.MessageInfo, _tcslen(pNotcie.MessageInfo), MT_Center, pNotcie.MessageInfoColor, pNotcie.OnceStepNum, pNotcie.OnceStepSec, 0);
	}
}