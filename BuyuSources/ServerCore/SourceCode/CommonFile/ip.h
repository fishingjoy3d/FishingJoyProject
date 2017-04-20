#define STR_BUFF_SIZE 128

//IP查询结果结构体
typedef struct _IpLocation
{
	unsigned int IpStart;
	unsigned int IpEnd;
	char  Country[40];
	char  City[40];
	_IpLocation()
	{
		memset(this,0,sizeof(IpLocation));
	}
} IpLocation;

//IP字符串 -> IP数值
extern unsigned int IpStringToInt(const char * ipStr);

//IP数值 -> IP字符串
extern char * IntToIpString(int ipv);

//查询IP段和所在地
extern IpLocation GetIpLocation(unsigned int ipv);

//根据地址搜索IP段
extern void IpSearch(const char * fnData, const char * fnOut);

//解压数据
extern void DataCompress(const char * fnData, const char * fnOut);

//2. IpLocator.cpp

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")//应该加上该句就可以



//#include "IpLocator.h"

//IP字符串 -> IP数值
unsigned int IpStringToInt(const char * ipStr)
{
	int t[4];
	unsigned int ipv;
	unsigned char * p=(unsigned char *)&ipv;
	sscanf_s(ipStr,"%d.%d.%d.%d",&t[0],&t[1],&t[2],&t[3]);
	p[0] = ConvertIntToBYTE(t[3]); p[1] = ConvertIntToBYTE(t[2]); p[2] = ConvertIntToBYTE(t[1]); p[3] = ConvertIntToBYTE(t[0]);
	return ipv;
}
//IP数值 -> IP字符串
char * IntToIpString(int ipv)
{
	unsigned char * p=(unsigned char *)&ipv;
	static char ipStr[sizeof("xxx.xxx.xxx.xxx")];
	sprintf_s(ipStr,"%d.%d.%d.%d",p[3],p[2],p[1],p[0]);
	return ipStr;
}
// 采用“二分法”搜索索引区, 定位IP索引记录位置
int getIndexOffset(FILE * fp, int fo, int lo, unsigned int ipv)
{
	int mo;    //中间偏移量
	unsigned int mv;    //中间值
	unsigned int fv,lv; //边界值
	unsigned int llv;   //边界末末值
	fseek(fp,fo,SEEK_SET);
	fread(&fv,4,1,fp);
	fseek(fp,lo,SEEK_SET);
	fread(&lv,4,1,fp);
	//临时作它用,末记录体偏移量
	fread(&mo,3,1,fp); mo&=0xffffff;
	fseek(fp,mo,SEEK_SET);
	fread(&llv,sizeof(int),1,fp);
	//printf("%d %d %d %d %d %d %d \n",fo,lo,mo,ipv,fv,lv,llv);
	//边界检测处理
	if(ipv<fv)
		return -1;
	else if(ipv>llv)
		return -1;
	//使用"二分法"确定记录偏移量
	do
	{
		mo=fo+(lo-fo)/7/2*7;
		fseek(fp,mo,SEEK_SET);
		fread(&mv,sizeof(int),1,fp);
		if(ipv>=mv)
			fo=mo;
		else
			lo=mo;
		if(lo-fo==7)
			mo=lo=fo;
	} while(fo!=lo);
	return mo;
}
// 读取IP所在地字符串
char * getString(char * strBuf, FILE * fp)
{
	//byte Tag;
	//int Offset;
	//Tag=fp.ReadByte();
	char tag;
	int so;
	fread(&tag,sizeof(char),1,fp);

	if(tag==0x01)   // 重定向模式1: 城市信息随国家信息定向
	{
		fread(&so,3,1,fp); so&=0xffffff;
		fseek(fp,so,SEEK_SET);
		return getString(strBuf,fp);
	}
	else if(tag==0x02)   // 重定向模式2: 城市信息没有随国家信息定向
	{
		fread(&so,3,1,fp); so&=0xffffff;
		//记下文件当前读位置
		int tmo=ftell(fp);
		fseek(fp,so,SEEK_SET);
		getString(strBuf,fp);
		fseek(fp,tmo,SEEK_SET);
		return strBuf;
	}
	else   // 无重定向: 最简单模式
	{
		fseek(fp,-1,SEEK_CUR);
		//读取字符串
		fread(strBuf,sizeof(char),STR_BUFF_SIZE,fp);
		//修正文件指针
		fseek(fp,strlen(strBuf)+1-STR_BUFF_SIZE,SEEK_CUR);
		return strBuf;
	}
}

//查询IP段和所在地
IpLocation GetIpLocation(unsigned int ipv)
{
	char szPath[MAX_PATH]={0};
	GetModuleFileNameA(NULL, szPath, sizeof(szPath));
	PathRemoveFileSpecA(szPath);
	PathAppendA(szPath,("qqwry.dat"));
	FILE * fp = null;
	errno_t Error= fopen_s(&fp, szPath, "rb");
	if (Error!= 0  || !fp)
	{
		throw "打开数据文件失败";
	}
	int fo,lo;   //首末索引偏移量
	fread(&fo,sizeof(int),1,fp);
	fread(&lo,sizeof(int),1,fp);
	//获取索引记录偏移量
	int rcOffset=getIndexOffset(fp,fo,lo,ipv);
	fseek(fp,rcOffset,SEEK_SET);
	IpLocation ipl;
	if(rcOffset>=0)
	{
		fseek(fp,rcOffset,SEEK_SET);
		//读取开头IP值
		fread(&ipl.IpStart,sizeof(int),1,fp);
		//转到记录体
		int ro;   //记录体偏移量
		fread(&ro,3,1,fp); ro&=0xffffff;
		fseek(fp,ro,SEEK_SET);
		//读取结尾IP值
		fread(&ipl.IpEnd,sizeof(int),1,fp);
		char strBuf[STR_BUFF_SIZE];
		getString(strBuf,fp);
		//ipl.Country=new char[strlen(strBuf+1)];
		if(strlen(ipl.Country)+strlen(strBuf)<40)
		{
			strcpy_s(ipl.Country, CountArray(ipl.Country),strBuf);
		}
		getString(strBuf,fp);
		//ipl.City=new char[strlen(strBuf+1)];
		if(strlen(ipl.City)+strlen(strBuf)<40)
		{
			strcpy_s(ipl.City, CountArray(ipl.City), strBuf);
		}
	}
	else
	{
		//没找到
		ipl.IpStart=0;
		ipl.IpEnd=0;
		strcpy_s(ipl.Country, CountArray(ipl.Country), "未知国家");
		strcpy_s(ipl.City, CountArray(ipl.City), "未知地址");

		//ipl.Country="未知国家";
		//ipl.City="未知地址";
	}
	fclose(fp);
	return ipl;
}

//解压数据
void DataCompress(const char * fnData, const char * fnOut)
{
	FILE * fp = null;
	errno_t Error = fopen_s(&fp, fnData, "rb");
	if (Error != 0 || !fp)
	{
		throw "打开数据文件失败";
	}
	FILE * fpo = null;
	Error = fopen_s(&fp, fnOut, "w");
	//如果没有打开文件则输出到屏幕
	if (Error != 0 || !fpo)
		fpo=stdout;
	int fo,lo,mo,ro;
	unsigned int ipStart,ipEnd;
	char country[STR_BUFF_SIZE];
	char city[STR_BUFF_SIZE];
	char sStart[sizeof("xxx.xxx.xxx.xxx")];
	char sEnd[sizeof("xxx.xxx.xxx.xxx")];
	fread(&fo,sizeof(int),1,fp);
	fread(&lo,sizeof(int),1,fp);
	int rCount=0;   //记录计数
	for(mo=fo;mo<=lo;mo+=7)
	{
		fseek(fp,mo,SEEK_SET);
		fread(&ipStart,sizeof(int),1,fp);
		fread(&ro,3,1,fp); ro&=0xffffff;
		fseek(fp,ro,SEEK_SET);
		fread(&ipEnd,sizeof(int),1,fp);
		getString(country,fp);
		getString(city,fp);
		//将IP值转化为字符串
		strcpy_s(sStart, CountArray(sStart),IntToIpString(ipStart));
		strcpy_s(sEnd, CountArray(sEnd), IntToIpString(ipEnd));
		fprintf(fpo,"%s - %s %s %s\n",sStart,sEnd,country,city);
		rCount++;
	}
	fprintf(fpo,"\n数据库记录总数: %d\n",rCount);
	fclose(fp);
	if(fpo!=stdout)
		fclose(fpo);
}

void IpLocating(const char * ipStr,char *result)
{
	unsigned int ipv=IpStringToInt(ipStr);
	IpLocation ipl;
	ipl=GetIpLocation(ipv);
	char ipStart[sizeof("xxx.xxx.xxx.xxx")];
	strcpy_s(ipStart, CountArray(ipStart),IntToIpString(ipl.IpStart));
	char ipEnd[sizeof("xxx.xxx.xxx.xxx")];
	strcpy_s(ipEnd, CountArray(ipEnd), IntToIpString(ipl.IpEnd));

	if(strlen(result)+strlen(ipl.Country)<40)
	{
		strcpy_s(result, CountArray(result), ipl.Country);
	}
	if(strlen(result)+strlen(ipl.City)<40)
	{
		strcpy_s(result, CountArray(result), ipl.City);
	}
	//printf("IP地址段: %s - %s\n",ipStart,ipEnd);
	//printf("IP所在地: %s %s\n",ipl.Country,ipl.City);
}
void IpLocating(DWORD ipv, char *result,DWORD ArrayLength)
{
	IpLocation ipl;
	ipl = GetIpLocation(ipv);
	char ipStart[sizeof("xxx.xxx.xxx.xxx")];
	strcpy_s(ipStart, CountArray(ipStart), IntToIpString(ipl.IpStart));
	char ipEnd[sizeof("xxx.xxx.xxx.xxx")];
	strcpy_s(ipEnd, CountArray(ipEnd), IntToIpString(ipl.IpEnd));

	if (strlen(result) + strlen(ipl.Country)<MAX_ADDRESS_LENGTH_IP)
	{
		strncpy_s(result, ArrayLength, ipl.Country, CountArray(ipl.Country));
	}
	if (strlen(result) + strlen(ipl.City)<MAX_ADDRESS_LENGTH_IP)
	{
		strncat_s(result, ArrayLength, ipl.City, CountArray(ipl.City));
	}
}

