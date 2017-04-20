//服务器端公告管理
#pragma once
class FishNoticeManager
{
public:
	FishNoticeManager();
	virtual ~FishNoticeManager();

	void OnReLoadFishNotcieConfig();//重新加载配置文件
	void OnUpdateByMin(DWORD dwTimer);
private:
	void OnUpdateByZeroSec();//更新  每分钟的0秒进行更新处理
	void OnHandleOnceNotice(tagNotice& pNotcie, time_t NowTime);
};