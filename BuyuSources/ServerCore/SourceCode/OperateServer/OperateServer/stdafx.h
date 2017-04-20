
#pragma once
// TODO:  在此处引用程序需要的其他头文件
#include "targetver.h"
#define CLIENT_CMD_SIZE 10240
#define MAX_SEND_CMD_COUNT 10240
#define USE_ACCEPT_RECV_DATA 1
#include "Common.h"


//运营服务器的错误编码 和客户端 一一对应

enum RoleOperateError //运营服务器的错误枚举
{
	//实名验证
	ROE_RealNameVerification_Sucess = 1,//实名验证成功
	ROE_RealNameVerification_IDENTITY_IsBeUse,//身份证被使用了
	ROE_RealNameVerification_IDENTITY_Error,//身份证格式错误
	ROE_RealNameVerification_IDENTITY_NotName,//身份证与实际名称不符合
	ROE_RealNameVerification_TimeOut,//实名验证超时
	ROE_RealNameVerification_IsExists,//实名验证重复提交

	//手机绑定
	ROE_PhoneVerificationNum_Sucess = 50,//获取验证码成功
	ROE_PhoneVerificationNum_PhoneError,//电话号码错误
	ROE_PhoneVerificationNum_PhoneBeUse,//电话号码被使用了
	ROE_PhoneVerificationNum_IsExists,//重复提交
	ROE_PhoneVerificationNum_WriteSend,//提交绑定手机成功 等待短信发送
	ROE_BindPhone_Sucess,//绑定手机成功
	ROE_BindPhone_BindNumError,//验证码错误
	ROE_BindPhone_BindTimeOut,//验证码超时
	ROE_BindPhone_SendSmsInfoError,//发送验证码错误

	//邮箱绑定
	ROE_BindMail_Sucess = 100,//绑定邮箱成功
	ROE_BindMail_IsBeUse,//邮箱呗使用了 无法绑定
	ROE_BindPhone_MailError,//邮箱错误无法绑定

	//购买实体物品
	ROE_BuyEntityItem_Sucess = 150,
	ROE_BuyEntityItem_Failed,

	//话费充值
	ROE_PhonePay_Sucess = 200,
	ROE_PhonePay_System,//内部错误
	ROE_PhonePay_HttpError,//外部错误
};