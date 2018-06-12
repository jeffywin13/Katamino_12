#include "stdafx.h"

class Loop
{
public:
	int layer; // 循环的层数
	int* count; // 各层要循环的次数
	Loop(int inputLayer , int* inputCount);
	~Loop();

	void visit(int **outputArray);

private:
	int totalCount; // 循环形成的组合总数
	int* index; // 各层当前循环到的次数，范围为0 ~ count[i] - 1
	
	void next();
};