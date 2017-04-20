#pragma once
#include <list>
#include "stdafx.h"
#include "tinyxml2.h"
using namespace tinyxml2;
class WHXml
{
	friend class WHXmlNode;
public:
	WHXml();
	virtual ~WHXml();
	virtual bool LoadXMLFilePath(const TCHAR * FilePath);//读取指定路径的XML文件 必须使用TCHAR
	virtual WHXmlNode* GetChildNodeByName(const TCHAR* NodeName);//获取跟节点 根据名称
private:
	void ActionXmlNode(WHXmlNode* pNode);
private:
	//属性
	tinyxml2::XMLDocument*					m_XmlDoc;//XML 文档对象
	std::list<WHXmlNode*>					m_ActionNode;//已经激活了的节点
};
class WHXmlNode
{
	friend class WHXml;
public:
	WHXmlNode();
	virtual ~WHXmlNode();

	WHXmlNode* GetNextSignelNode();//获取下一个相同名称的节点
	WHXmlNode* GetChildNodeByName(const TCHAR* NodeName);//获取子节点 根据名称

	//读取属性的函数
	//int8 int16 int32 int64
	bool			GetAttribute(const TCHAR* AttributeName, __int64& nInt64Value);
	bool			GetAttribute(const TCHAR* AttributeName, __int32& nInt32Value);
	bool			GetAttribute(const TCHAR* AttributeName, __int16& nInt16Value);
	bool			GetAttribute(const TCHAR* AttributeName, __int8& nInt8Value);
	//uint8 uint16 uint32 uint64
	bool			GetAttribute(const TCHAR* AttributeName, unsigned __int64& nUInt64Value);
	bool			GetAttribute(const TCHAR* AttributeName, unsigned __int32& nUInt32Value);
	bool			GetAttribute(const TCHAR* AttributeName, unsigned __int16& nUInt16Value);
	bool			GetAttribute(const TCHAR* AttributeName, unsigned __int8& nUInt8Value);
	bool			GetAttribute(const TCHAR* AttributeName, DWORD& nDWORDValue);
	//float double
	bool			GetAttribute(const TCHAR* AttributeName, double& ndoubleValue);
	bool			GetAttribute(const TCHAR* AttributeName, float&  nfloatValue);
	//char          CHAR 就是Int8
	//bool			GetAttribute(const TCHAR* AttributeName, char& nCharValue);
	//TCHAR[]
	bool			GetAttribute(const TCHAR* AttributeName, TCHAR* nStrValue, size_t Size);
private:
	void SetXmlNode(tinyxml2::XMLElement* nNode, WHXml* nXml);
private:
	tinyxml2::XMLElement*						m_XmlNode;//节点
	WHXml*										m_pXml;//XML文档
};