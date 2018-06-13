#include "stdafx.h"

typedef unsigned char UINT8;

class Loop
{
public:
	int layer; // 循环的层数
	UINT8* count; // 各层要循环的次数
	Loop(int inputLayer , UINT8* inputCount);
	~Loop();

	void visit(UINT8 **outputArray);

private:
	int totalCount; // 循环形成的组合总数
	UINT8* index; // 各层当前循环到的次数，范围为0 ~ count[i] - 1
	
	void next();
};