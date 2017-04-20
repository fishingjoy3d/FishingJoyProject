//用于在程序崩溃的时候 收集用于调试的dump数据
#pragma once
class Dump  //一个记录程序崩溃的类
{
public:
	Dump();
	void OnInit();
	virtual ~Dump();
};