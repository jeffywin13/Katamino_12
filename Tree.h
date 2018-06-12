#include "stdafx.h"

typedef struct node
{
	struct node* father;
	struct node* child;
	struct node* next;
	int nodeIndex;
} strNode;


class Tree
{
public:
	strNode root;
	int layers;
	int leafCount;
	int* nodes; // node count for each layer

	Tree(int n , int* array);
	~Tree();
	void init(int count , int index);
	void grow();
	void visit(int **result);
	void destroy();

private:
	void visit(int **result , strNode* pNode);
	void destroy(strNode* pNode);
	int visitCount;
	void grow(strNode* pRoot , int layerIndex);
	//void destroy(strNode* pNode , int index);
};