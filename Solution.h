#include "stdafx.h"

typedef unsigned char UINT8;

typedef struct solutionNode
{
	UINT8* tileIndex;
	UINT8* typeIndex;
	UINT8* posIndex;
	struct solutionNode* next;
	char* graphic;
}strSolutionNode;


class Solution
{
public:
	int cols;
	int rows;
	int nodeCount;
	strSolutionNode* pSolution;
	strSolutionNode* pLastSolution;

	Solution(int inputRows);
	~Solution();
	void addSolution(UINT8* inputIndex , UINT8* inputTypes , UINT8* inputPositions , char* inputGraphic);
	bool isContain(char* inputGraphic);
	void printToFile(char* path);
};