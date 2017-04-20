#include "StdAfx.h"
#include "XmlReader.h"
#define MAX_NAME_LENGTH  128
WHXml::WHXml()
{
	m_XmlDoc = NULL;
	m_ActionNode.clear();
}
WHXml::~WHXml()
{
	if (m_XmlDoc)
	{
		m_XmlDoc->Clear();
		delete m_XmlDoc;
	}

	if (!m_ActionNode.empty())
	{
		std::list<WHXmlNode*>::iterator Iter = m_ActionNode.begin();
		for (; Iter != m_ActionNode.end(); ++Iter)
		{
			free(*Iter);
		}
		m_ActionNode.clear();
	}
}
bool WHXml::LoadXMLFilePath(const TCHAR * FilePath)
{
	//加载指定路径的XML文件
	/*if (FilePath == NULL || strlen(FilePath) == 0)
	return false;*/
	//无法读取Uniocd 的文件 
	//Anis 可以读取
	if (FilePath == NULL)
		return false;
	if (_tcsclen(FilePath) > MAX_NAME_LENGTH)
		return false;
	UINT Count = 0;
	char *nFileName = WCharToChar(FilePath, Count);
	m_XmlDoc = new tinyxml2::XMLDocument;
	if (!m_XmlDoc)
	{
		free(nFileName);
		return false;
	}
	tinyxml2::XMLError pError = m_XmlDoc->LoadFile(nFileName);
	free(nFileName);
	return (pError == tinyxml2::XML_SUCCESS);//读取指定的XML文件
}
WHXmlNode* WHXml::GetChildNodeByName(const TCHAR* NodeName)
{
	//根据跟节点的名称获取节点
	if (NodeName == NULL)
		return false;
	if (_tcsclen(NodeName) > MAX_NAME_LENGTH)
		return false;
	UINT Count = 0;
	char *nNodeName = WCharToChar(NodeName, Count);

	tinyxml2::XMLElement* node = m_XmlDoc->FirstChildElement(nNodeName);//获取指定名称的节点信息
	if (!node)
	{
		free(nNodeName);
		return NULL;
	}
	WHXmlNode*  pNode = (WHXmlNode*)malloc(sizeof(WHXmlNode));
	pNode->SetXmlNode(node, this);
	m_ActionNode.push_back(pNode);//这个是无须遍历的 直接使用链表就可以了
	free(nNodeName);
	return pNode;
}
void WHXml::ActionXmlNode(WHXmlNode* pNode)
{
	if (pNode == NULL)
		return;
	m_ActionNode.push_back(pNode);
}
WHXmlNode::WHXmlNode()
{

}
WHXmlNode::~WHXmlNode()
{

}
void WHXmlNode::SetXmlNode(tinyxml2::XMLElement* nNode, WHXml* nXml)
{
	m_XmlNode = nNode;
	m_pXml = nXml;
}
WHXmlNode* WHXmlNode::GetNextSignelNode()//同一级别下的同名的节点
{
	//获取下一个子节点的信息
	if (m_XmlNode == NULL || m_pXml == NULL)
		return NULL;
	tinyxml2::XMLElement* nNode = m_XmlNode->NextSiblingElement();//获取下一个节点
	if (nNode == NULL)
		return NULL;
	while (nNode && strcmp(nNode->Value(), m_XmlNode->Value()) != 0)
	{
		nNode = nNode->NextSiblingElement();
	}
	if (!nNode)
		return NULL;
	//节点的名称
	//获取相同名称的节点

	WHXmlNode*  pNode = (WHXmlNode*)malloc(sizeof(WHXmlNode));
	pNode->SetXmlNode(nNode, m_pXml);
	m_pXml->ActionXmlNode(pNode);
	return pNode;
}
WHXmlNode* WHXmlNode::GetChildNodeByName(const TCHAR* NodeName)
{
	//根据名称获取子节点的信息
	if (m_XmlNode == NULL || m_pXml == NULL)
		return NULL;
	if (NodeName == NULL)
		return NULL;
	if (_tcsclen(NodeName) > MAX_NAME_LENGTH)
		return NULL;

	UINT Count = 0;
	char *nNodeName = WCharToChar(NodeName, Count);

	tinyxml2::XMLElement* nNode = m_XmlNode->FirstChildElement(nNodeName);
	if (nNode == NULL)
	{
		free(nNodeName);
		return NULL;
	}
	WHXmlNode*  pNode = (WHXmlNode*)malloc(sizeof(WHXmlNode));
	pNode->SetXmlNode(nNode, m_pXml);
	m_pXml->ActionXmlNode(pNode);
	free(nNodeName);
	return pNode;
}
bool WHXmlNode::GetAttribute(const TCHAR* AttributeName, __int64& nInt64Value)
{
	if (m_XmlNode == NULL || m_pXml == NULL)
		return false;
	if (AttributeName == NULL)
		return false;
	if (_tcsclen(AttributeName) > MAX_NAME_LENGTH)
		return false;

	UINT Count = 0;
	char *nAttributeName = WCharToChar(AttributeName, Count);

	const char* pDestSrc = m_XmlNode->Attribute(nAttributeName);
	if (pDestSrc == null)
	{
		free(nAttributeName);
		return false;
	}

	nInt64Value = strtoll(pDestSrc,null,10);

	free(nAttributeName);
	return true;
}
bool WHXmlNode::GetAttribute(const TCHAR* AttributeName, __int32& nInt32Value)
{
	if (m_XmlNode == NULL || m_pXml == NULL)
		return false;
	if (AttributeName == NULL)
		return false;
	if (_tcsclen(AttributeName) > MAX_NAME_LENGTH)
		return false;

	UINT Count = 0;
	char *nAttributeName = WCharToChar(AttributeName, Count);

	const char* pDestSrc = m_XmlNode->Attribute(nAttributeName);
	if (pDestSrc == null)
	{
		free(nAttributeName);
		return false;
	}
	nInt32Value = strtol(pDestSrc,null,10);
	free(nAttributeName);
	return true;
}
bool WHXmlNode::GetAttribute(const TCHAR* AttributeName, __int16& nInt16Value)
{
	if (m_XmlNode == NULL || m_pXml == NULL)
		return false;
	if (AttributeName == NULL)
		return false;
	if (_tcsclen(AttributeName) > MAX_NAME_LENGTH)
		return false;

	UINT Count = 0;
	char *nAttributeName = WCharToChar(AttributeName, Count);

	const char* pDestSrc = m_XmlNode->Attribute(nAttributeName);
	if (pDestSrc == null)
	{
		free(nAttributeName);
		return false;
	}
	nInt16Value = static_cast<__int16>(strtol(pDestSrc,null,10));

	free(nAttributeName);

	return true;
}
bool WHXmlNode::GetAttribute(const TCHAR* AttributeName, __int8& nInt8Value)
{
	if (m_XmlNode == NULL || m_pXml == NULL)
		return false;
	if (AttributeName == NULL)
		return false;
	if (_tcsclen(AttributeName) > MAX_NAME_LENGTH)
		return false;
	UINT Count = 0;
	char *nAttributeName = WCharToChar(AttributeName, Count);

	const char* pDestSrc = m_XmlNode->Attribute(nAttributeName);
	if (pDestSrc == null)
	{
		free(nAttributeName);
		return false;
	}
	nInt8Value = static_cast<__int8>(strtol(pDestSrc,null,10));

	free(nAttributeName);
	return true;
}
bool WHXmlNode::GetAttribute(const TCHAR* AttributeName, DWORD& nDWORDValue)
{
	if (m_XmlNode == NULL || m_pXml == NULL)
		return false;
	if (AttributeName == NULL)
		return false;
	if (_tcsclen(AttributeName) > MAX_NAME_LENGTH)
		return false;

	UINT Count = 0;
	char *nAttributeName = WCharToChar(AttributeName, Count);

	const char* pDestSrc = m_XmlNode->Attribute(nAttributeName);
	if (pDestSrc == null)
	{
		free(nAttributeName);
		return false;
	}
	nDWORDValue = strtoul(pDestSrc, null, 10);
	free(nAttributeName);
	return true;
}
bool WHXmlNode::GetAttribute(const TCHAR* AttributeName, unsigned __int64& nUInt64Value)
{
	if (m_XmlNode == NULL || m_pXml == NULL)
		return false;
	if (AttributeName == NULL)
		return false;
	if (_tcsclen(AttributeName) > MAX_NAME_LENGTH)
		return false;

	UINT Count = 0;
	char *nAttributeName = WCharToChar(AttributeName, Count);

	const char* pDestSrc = m_XmlNode->Attribute(nAttributeName);
	if (pDestSrc == null)
	{
		free(nAttributeName);
		return false;
	}
	nUInt64Value = strtoull(pDestSrc, null, 10);
	free(nAttributeName);
	return true;
}
bool WHXmlNode::GetAttribute(const TCHAR* AttributeName, unsigned __int32& nUInt32Value)
{
	if (m_XmlNode == NULL || m_pXml == NULL)
		return false;
	if (AttributeName == NULL)
		return false;
	if (_tcsclen(AttributeName) > MAX_NAME_LENGTH)
		return false;

	UINT Count = 0;
	char *nAttributeName = WCharToChar(AttributeName, Count);

	const char* pDestSrc = m_XmlNode->Attribute(nAttributeName);
	if (pDestSrc == null)
	{
		free(nAttributeName);
		return false;
	}
	nUInt32Value = strtoul(pDestSrc, null, 10);
	free(nAttributeName);
	return true;
}
bool WHXmlNode::GetAttribute(const TCHAR* AttributeName, unsigned __int16& nUInt16Value)
{
	if (m_XmlNode == NULL || m_pXml == NULL)
		return false;
	if (AttributeName == NULL)
		return false;
	if (_tcsclen(AttributeName) > MAX_NAME_LENGTH)
		return false;

	UINT Count = 0;
	char *nAttributeName = WCharToChar(AttributeName, Count);

	const char* pDestSrc = m_XmlNode->Attribute(nAttributeName);
	if (pDestSrc == null)
	{
		free(nAttributeName);
		return false;
	}
	nUInt16Value = static_cast<unsigned __int16>(strtoul(pDestSrc, null, 10));
	free(nAttributeName);
	return true;
}
bool WHXmlNode::GetAttribute(const TCHAR* AttributeName, unsigned __int8& nUInt8Value)
{
	if (m_XmlNode == NULL || m_pXml == NULL)
		return false;
	if (AttributeName == NULL)
		return false;
	if (_tcsclen(AttributeName) > MAX_NAME_LENGTH)
		return false;

	UINT Count = 0;
	char *nAttributeName = WCharToChar(AttributeName, Count);

	const char* pDestSrc = m_XmlNode->Attribute(nAttributeName);
	if (pDestSrc == null)
	{
		free(nAttributeName);
		return false;
	}
	nUInt8Value = static_cast<unsigned __int8>(strtoul(pDestSrc, null, 10));
	free(nAttributeName);
	return true;
}
bool WHXmlNode::GetAttribute(const TCHAR* AttributeName, double& ndoubleValue)
{
	if (m_XmlNode == NULL || m_pXml == NULL)
		return false;
	if (AttributeName == NULL)
		return false;
	if (_tcsclen(AttributeName) > MAX_NAME_LENGTH)
		return false;
	UINT Count = 0;
	char *nAttributeName = WCharToChar(AttributeName, Count);

	const char* pDestSrc = m_XmlNode->Attribute(nAttributeName);
	if (pDestSrc == null)
	{
		free(nAttributeName);
		return false;
	}
	ndoubleValue = strtof(pDestSrc,null);
	free(nAttributeName);
	return true;
}
bool WHXmlNode::GetAttribute(const TCHAR* AttributeName, float& nfloatValue)
{
	if (m_XmlNode == NULL || m_pXml == NULL)
		return false;
	if (AttributeName == NULL)
		return false;
	if (_tcsclen(AttributeName) > MAX_NAME_LENGTH)
		return false;
	UINT Count = 0;
	char *nAttributeName = WCharToChar(AttributeName, Count);

	const char* pDestSrc = m_XmlNode->Attribute(nAttributeName);
	if (pDestSrc == null)
	{
		free(nAttributeName);
		return false;
	}
	nfloatValue = static_cast<float>(strtof(pDestSrc,null));
	free(nAttributeName);
	return true;
}
//bool WHXmlNode::GetAttribute(const TCHAR* AttributeName, char& nCharValue)
//{
//	//读取一个 char
//	if (m_XmlNode == NULL || m_pXml == NULL)
//		return false;
//	if (AttributeName == NULL)
//		return false;
//	if (_tcsclen(AttributeName) > MAX_NAME_LENGTH)
//		return false;
//	char nAttributeName[MAX_NAME_LENGTH] = { 0 };
//	WHGlobalFunction::TCHATToChar(AttributeName, nAttributeName);
//
//	nCharValue = m_XmlNode->Attribute(nAttributeName)[0];//读取第一个字符
//}
bool WHXmlNode::GetAttribute(const TCHAR* AttributeName, TCHAR* nStrValue, size_t Size)
{
	//读取一个字符串 转为TCHAR类型
	if (m_XmlNode == NULL || m_pXml == NULL)
		return false;
	if (AttributeName == NULL)
		return false;
	if (_tcsclen(AttributeName) > MAX_NAME_LENGTH)
		return false;

	UINT Count = 0;
	char *nAttributeName = WCharToChar(AttributeName, Count);

	const char* nStr = m_XmlNode->Attribute(nAttributeName);//一个C串
	free(nAttributeName);
	if (strlen(nStr) > Size)
		return false;

	TCHAR* pChar = CharToWChar(nStr, Count);
	TCHARCopy(nStrValue, Size, pChar, Count);
	free(pChar);
	return true;
}