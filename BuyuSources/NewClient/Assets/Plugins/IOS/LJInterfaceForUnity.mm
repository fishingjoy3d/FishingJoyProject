//
//  LJInterfaceForUnity.m
//  LJInterfaceForUnity
//
//  Created by longfeiwang on 4/7/15.
//  Copyright (c) 2015 LJSDK. All rights reserved.
//
#import <Foundation/Foundation.h>
#import "LJCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void UnitySendMessage(const char* obj, const char* method, const char* msg);
    
#ifdef __cplusplus
}
#endif

/***************************************************常量********************************************************/

static const int SUCCESS = 112501;
static const int FAILURE = 112500;
static const int LOGOUT  = 160105;
/***************************************************常量********************************************************/
/***************************************************工具方法*****************************************************/
//convert NSString to const char*
const char* nStr2CStr(NSString* nStr) {
    return [nStr cStringUsingEncoding:NSUTF8StringEncoding];
}

//convert const char* to NSString
NSString* cStr2NStr(const char* cstr) {
    return [[NSString alloc] initWithCString:cstr encoding:NSUTF8StringEncoding];
}

//send message to unity
void sendMsg2Unity(const char* gameObject,const char* callbackMethod,const char* jsonData) {
    NSLog(@"gameObject:%@",cStr2NStr(gameObject));
    NSLog(@"callbackMethod:%@",cStr2NStr(callbackMethod));
    UnitySendMessage(gameObject, callbackMethod, jsonData);
}

//convert int to NSString
NSString* i2NStr(int i) {
    return [[NSString alloc] initWithFormat:@"%d",i];
}

// By default mono string marshaler creates .Net string for returned UTF-8 C string
// and calls free for returned value, thus returned strings should be allocated on heap
char* MakeStringCopy (const char* string)
{
    if (string == NULL)
        return NULL;
    
    char* res = (char*)malloc(strlen(string) + 1);
    strcpy(res, string);
    return res;
}

//create json const char*
const char* createJSONResult (int code,NSDictionary* data) {
    NSMutableDictionary *jsonDict = [[NSMutableDictionary alloc]init];
    [jsonDict setValue:[NSNumber numberWithInteger:code] forKey:@"resultCode"];
    [jsonDict setValue:data forKey:@"data"];
    
    NSError* err;
    NSData* jsonData = [NSJSONSerialization dataWithJSONObject:jsonDict options:NSJSONWritingPrettyPrinted error:&err];
    NSString* jsonStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    
    NSLog(@"jsonResult:%@",jsonStr);
    return nStr2CStr(jsonStr);
}

/************************************************工具方法*****************************************************/


/************************************************登录回调*****************************************************/
@interface UnityUserCallback : NSObject<UserCallback>
{
    NSString* mGameObject;
    NSString* mCallbackMethod;
}
-(id)initWithGameObject:(NSString*)gameObject andCallbackMethod:(NSString*)callbackMethod;
@end

@implementation UnityUserCallback

-(id)initWithGameObject:(NSString*)gameObject andCallbackMethod:(NSString*)callbackMethod
{
    if (self == [self init]) {
        mGameObject = gameObject;
        mCallbackMethod = callbackMethod;
    }
    return self;
}

-(void)onLoginFailedWithReason:(NSString *)reason CustomParams:(NSString *)customParams {
    NSLog(@"login result:%@",reason);
    NSDictionary* jsonDict = [NSDictionary dictionaryWithObjectsAndKeys:reason,@"detail",
                              customParams,@"customParams",
                              nil];
    sendMsg2Unity(nStr2CStr(mGameObject),nStr2CStr(mCallbackMethod),createJSONResult(FAILURE,jsonDict));
}

-(void)onLoginSuccessWithXMUser:(XMUser *)xmUser CustomParams:(NSString *)customParams {
    NSLog(@"login result:success");
    NSDictionary* jsonDict = [NSDictionary dictionaryWithObjectsAndKeys:xmUser.userName,@"userName",
                              xmUser.userId,@"uid",
                              [XMUtil getChannelCode],@"channelCode",
                              [XMUtil getProductCode],@"productCode",
                              xmUser.token,@"token",
                              xmUser.channelUserId,@"channelUid",
                              customParams,@"customParams",
                              [XMUtil readPlistWithKey:@"CHANNEL_LABEL"],@"channelLabel",
                              nil];
    sendMsg2Unity(nStr2CStr(mGameObject),nStr2CStr(mCallbackMethod),createJSONResult(SUCCESS, jsonDict));
}

-(void)onLogout:(NSString *)customParams {
    NSLog(@"logout");
    if (customParams == NULL || customParams == nil) {
        customParams = @"";
    }
    
    NSDictionary* jsonDict = [NSDictionary dictionaryWithObjectsAndKeys:@"logout",@"detail",
                              customParams,@"customParams",
                              nil];
    sendMsg2Unity(nStr2CStr(mGameObject),nStr2CStr(mCallbackMethod),createJSONResult(LOGOUT, jsonDict));
}

@end
/************************************************登录回调·完*****************************************************/

/************************************************初始化回调*****************************************************/
@interface UnityInitCallback : NSObject<InitCallback>
{
    NSString* mGameObject;
    NSString* mCallbackMethod;
}
-(id)initWithGameObject:(NSString*)gameObject andCallbackMethod:(NSString*)callbackMethod;

@end

@implementation UnityInitCallback

-(id)initWithGameObject:(NSString*)gameObject andCallbackMethod:(NSString*)callbackMethod
{
    if (self == [self init]) {
        mGameObject = gameObject;
        mCallbackMethod = callbackMethod;
    }
    return self;
}

-(void)onInitSuccess:(NSString *)successInfo
{
    NSLog(@"Init result:%@",successInfo);
    NSDictionary* jsonDict = [NSDictionary dictionaryWithObjectsAndKeys:successInfo,@"detail",
                              nil];
    sendMsg2Unity(nStr2CStr(mGameObject),nStr2CStr(mCallbackMethod),createJSONResult(SUCCESS, jsonDict));
}

-(void)onInitFailed:(NSString *)failInfo
{
    NSLog(@"Init result:%@",failInfo);
    NSDictionary* jsonDict = [NSDictionary dictionaryWithObjectsAndKeys:failInfo,@"detail",
                              nil];
    sendMsg2Unity(nStr2CStr(mGameObject),nStr2CStr(mCallbackMethod),createJSONResult(FAILURE, jsonDict));
}

@end
/************************************************初始化回调·完*****************************************************/

/************************************************支付回调*****************************************************/
@interface UnityPayCallback : NSObject<PayCallback>
{
    NSString* mGameObject;
    NSString* mCallbackMethod;
}
-(id)initWithGameObject:(NSString*)gameObject andCallbackMethod:(NSString*)callbackMethod;
@end

@implementation UnityPayCallback

-(id)initWithGameObject:(NSString*)gameObject andCallbackMethod:(NSString*)callbackMethod
{
    if (self == [self init]) {
        mGameObject = gameObject;
        mCallbackMethod = callbackMethod;
    }
    return self;
}

-(void)onPaySuccess:(NSString *)successInfo
{
    NSLog(@"pay result:%@",successInfo);
    NSDictionary* jsonDict = [NSDictionary dictionaryWithObjectsAndKeys:successInfo,@"detail",
                              nil];
    sendMsg2Unity(nStr2CStr(mGameObject),nStr2CStr(mCallbackMethod),createJSONResult(SUCCESS, jsonDict));
}

-(void)onPayFailed:(NSString *)failInfo
{
    NSLog(@"pay result:%@",failInfo);
    NSDictionary* jsonDict = [NSDictionary dictionaryWithObjectsAndKeys:failInfo,@"detail",
                              nil];
    sendMsg2Unity(nStr2CStr(mGameObject),nStr2CStr(mCallbackMethod),createJSONResult(FAILURE, jsonDict));
}

@end
/************************************************支付回调·完*****************************************************/


/***************************************************LJSDK unity接口**************************************************/

UnityUserCallback* userCallback;
UnityPayCallback* payCallback;
UnityInitCallback* initCallback;

extern "C"
{
    void _init(const char* gameObject,const char* callbackMethod)
    {
        initCallback = [[UnityInitCallback alloc] initWithGameObject:cStr2NStr(gameObject) andCallbackMethod:cStr2NStr(callbackMethod)];
        [[AgentManager instance] initWithCallback:initCallback];
    }
    
    void _setUserCallback(const char* gameObject,const char* callbackMethod)
    {
        userCallback = [[UnityUserCallback alloc] initWithGameObject:cStr2NStr(gameObject) andCallbackMethod:cStr2NStr(callbackMethod)];
    }
    
    void _login(const char* params)
    {
        NSLog(@"login");
        if (userCallback == nil) {
            [XMUtil toast:@"Call setUserCallback please"];
        } else {
            [[AgentManager instance] loginWithCustomInfo:cStr2NStr(params) Delegate:userCallback];
        }
    }
    
    void _pay(const char* gameObject,const char* callbackMethod,int amount,const char* itemName,int count,const char* chargePointName,const char* customParams,const char* callbackUrl)
    {
        NSLog(@"pay");
        NSString *myitemName = [[NSString alloc] initWithUTF8String:itemName];
        NSString *newItem = @"金币";
        if([myitemName isEqualToString:@"金币"])
            newItem = @"金币";
        else if([myitemName isEqualToString:@"钻石"])
            newItem = @"钻石";
        else if([myitemName isEqualToString:@"月卡"])
            newItem = @"月卡";

        payCallback = [[UnityPayCallback alloc] initWithGameObject:cStr2NStr(gameObject) andCallbackMethod:cStr2NStr(callbackMethod)];
        [[AgentManager instance] payWithAmount:amount itemName:newItem count:count customParams:cStr2NStr(customParams) CallbackUrl:cStr2NStr(callbackUrl) Delegate:payCallback];
    }
    
    void _setExtData(const char* _id,const char* roleId,const char* roleName,int roleLevel,int zoneId,const char* zoneName,int balance,int vip,const char* partyName)
    {
        NSLog(@"setExtData");
        NSDictionary* jsonDict = [NSDictionary dictionaryWithObjectsAndKeys:cStr2NStr(_id),@"_id",
                                  cStr2NStr(roleId),@"roleId",
                                  cStr2NStr(roleName),@"roleName",
                                  i2NStr(roleLevel),@"roleLevel",
                                  i2NStr(zoneId),@"zoneId",
                                  cStr2NStr(zoneName),@"zoneName",
                                  i2NStr(balance),@"balance",
                                  i2NStr(vip),@"vip",
                                  cStr2NStr(partyName),@"partyName",
                                  nil];
        NSError* err;
        NSData* jsonData = [NSJSONSerialization dataWithJSONObject:jsonDict options:NSJSONWritingPrettyPrinted error:&err];
        NSString* jsonStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
        [[AgentManager instance] setExtRoleData:jsonStr];
    }
    
    void _logout(const char* params)
    {
        NSLog(@"logout");
        if (userCallback == nil) {
            [XMUtil toast:@"Call setUserCallback please"];
        } else {
            [[AgentManager instance] logoutWithCustomInfo:cStr2NStr(params) Delegate:userCallback];
        }
    }
    
    const char* _getChannelLabel() {
        return MakeStringCopy([[XMUtil getChannelLabel] UTF8String]);
    }
    
    void _showIOSDialog(const char* title,const char* content) {
        [XMUtil showMessage:[[NSString alloc] initWithUTF8String:title] withTitle:[[NSString alloc] initWithUTF8String:content] forTime:1.5f];
    }
}
