#include "stdafx.h"
#include "Tree.h"

Tree::Tree(int n , int* array)
{	
	layers = n;
	leafCount = 1;
	nodes = new int[n];
	for(int i = 0 ; i < n ; ++i)
	{
		nodes[i] = array[i];
		leafCount *= array[i];
	}

	root.father = NULL;
	root.child = NULL;
	root.next = NULL;
	root.nodeIndex = -1;

	grow();

	return;
}


void Tree::grow()
{
	int i = 0;
	strNode* p = &root;
//	while(i < layers)
//	{
		grow(p , i);
//	}

	return;
}


void Tree::grow(strNode* pRoot , int layerIndex)
{
	int count = nodes[layerIndex]; // 要创建的新节点的个数
	strNode *p = pRoot->child , *q = p;
	int i = 0;
	for(i = 0 ; i < count ; ++i)
	{
		p = new strNode;
		p->father = pRoot;
		p->child = NULL;
		p->next = NULL;

		if(i > 0)
			q->next = p;
		else
			pRoot->child = p;

		q = p;
	}

	if(layerIndex < layers - 1)
	{	// 非叶子结点
		p = pRoot->child;
		while(p->next != NULL)
		{
			grow(p , layerIndex + 1);
			p = p->next;
		}
	}

	
	return;
}

Tree::~Tree()
{
	if(nodes != NULL)
		delete[] nodes;
}



// void Tree::grow()
// {
// 	int i = 0 , j = 0 , k = 0;
// 	strNode* p = NULL;
// 	strNode** currentLeaves = NULL;
// 	strNode** newLeaves = NULL;
// 	int currentLeafCount = 1;
// 	int newLeafCount = 0;
// 
// 	currentLeaves = new strNode*[1];
// 	currentLeaves[0] = &root;
// 
// 	for(i = 0 ; i < layers ; ++i)
// 	{
// 		// 		p = &root;
// 		// 		for(j = 0 ; j < i ; ++j)
// 		// 			p = p->child;
// 		newLeafCount = 1;
// 		for(j = 0 ; j < i + 1 ; ++j)
// 			newLeafCount *= nodes[j];
// 
// 		if(newLeaves != NULL)
// 			delete[] newLeaves;
// 		newLeaves = new strNode*[newLeafCount];
// 
// 		int c = 0;
// 
// 		// 记录所有新增叶子节点
// 
// 		for(j = 0 ; j < currentLeafCount ; ++j)
// 		{
// 			p = currentLeaves[j];
// 			strNode* tmpRoot = p;
// 			tmpRoot->child = new strNode[nodes[i]];
// 			for(k = 0 ; k < nodes[i] ; ++k)
// 			{
// 				tmpRoot->child[k].child = NULL;
// 				tmpRoot->child[k].father = tmpRoot;
// 				tmpRoot->child[k].nodeIndex = k;
// 				tmpRoot->child[k].next = (k == nodes[i] - 1) ? NULL : &(tmpRoot->child[k + 1]);
// 				newLeaves[c++] = &(tmpRoot->child[k]);
// 			}
// 		}
// 
// 		delete[] currentLeaves;
// 		currentLeaves = newLeaves;
// 		currentLeafCount = newLeafCount;
// 		newLeaves = NULL;
// 	}
// 
// 	delete[] currentLeaves;
// 
// 	return;
// }



// void Tree::grow()
// {
// 	int i = 0 , j = 0 , k = 0;
// 	strNode* p = NULL;
// 	strNode** currentLeaves = NULL;
// 	strNode** newLeaves = NULL;
// 	int currentLeafCount = 1;
// 	int newLeafCount = 0;
// 
// 	currentLeaves = new strNode*[1];
// 	currentLeaves[0] = &root;
// 
// 	for(i = 0 ; i < layers ; ++i)
// 	{
// 		newLeafCount = 1;
// 		for(j = 0 ; j < i + 1 ; ++j)
// 			newLeafCount *= nodes[j];
// 
// 		newLeaves = new strNode*[newLeafCount];
// 
// 		int c = 0;
// 
// 		// 记录所有新增叶子节点
// 
// 		for(j = 0 ; j < currentLeafCount ; ++j)
// 		{
// 			p = currentLeaves[j];
// 			strNode* tmpRoot = p;
// 			tmpRoot->child = new strNode[nodes[i]];
// 			for(k = 0 ; k < nodes[i] ; ++k)
// 			{
// 				tmpRoot->child[k].child = NULL;
// 				tmpRoot->child[k].father = tmpRoot;
// 				tmpRoot->child[k].nodeIndex = k;
// 				tmpRoot->child[k].next = (k == nodes[i] - 1) ? NULL : &(tmpRoot->child[k + 1]);
// 				newLeaves[c++] = &(tmpRoot->child[k]);
// 			}
// 		}
// 
// 		delete[] currentLeaves;
// 		currentLeaves = new strNode*[newLeafCount];
// 		memcpy(currentLeaves , newLeaves , sizeof(strNode*) * newLeafCount);
// 		delete[] newLeaves;
// 		newLeaves = NULL;
// 		currentLeafCount = newLeafCount;
// 	}
// 
// 	delete[] currentLeaves;
// 
// 	return;
// }



void Tree::destroy()
{
	destroy(&root);
}

void Tree::destroy(strNode* pNode)
{
	strNode* p = pNode->child , *q = NULL;

	if(p == NULL)
		delete pNode;
	else
	{
		while(p != NULL)
		{
			q = p->next;
			destroy(p);
			p = q;
		}
	}

	return;
}



// void Tree::destroy()
// {
// //	strNode* p = NULL;
// //	while(root.child != NULL)
// 	if(root.child != NULL)
// 	{
// // 		p = root.child;
// // 		root.child = p->next;
// 		destroy(root.child);
// 	}
// 	layers = 0;
// //	delete[] nodes;
// 	return;
// }



// void Tree::destroy(strNode* pNode)
// {
// 	if(pNode->child != NULL)
// 	{
// 		strNode* p = NULL;
// 		while(pNode->child != NULL)
// 		{
// 			p = pNode->child;
// 			pNode->child = p->next;
// 			destroy(p);
// 		}
// 		//destroy(pNode->child);
// 	}
// 	else
// 	{
// 		if(pNode == root.child)
// 			pNode = pNode;
// 
// 		if(pNode->father != NULL)
// 		{
// 			pNode->father->child = NULL;//pNode->next;
// 			delete pNode;
// 		}
// 	}
// 
// 	return;
// }


// void Tree::destroy(strNode* pNode)
// {
// 	if(pNode->child != NULL)
// 		destroy(pNode->child);
// 	else
// 	{
// 		if(pNode->father != NULL)
// 		{
// 			pNode->father->child = NULL;//pNode->next;
// 			delete pNode;
// 		}
// 	}
// 
// 	return;
// }


void Tree::visit(int **result)
{
	visitCount = 0;
	visit(result , &root);
	return;
}

//int cc = 0;

void Tree::visit(int **result , strNode* pNode)
{
	if(pNode->child != NULL)
	{
		strNode* p = pNode->child;
		while(p)
		{
			visit(result , p);
			p = p->next;
		}
	}
	else
	{
		strNode* r = pNode;
		int n = layers - 1;
		while(r->father != NULL)
		{
			//printf("%3d" , r->nodeIndex);
			result[visitCount][n--] = r->nodeIndex;
			r = r->father;
		}
		++visitCount;
//		printf("\n");

// 		printf("cc = %d\n" , cc);
// 		++cc;
	}
	return;
}


void Tree::init(int count , int index)
{
// 	strNode* p = root;
// 	for(int i = 0 ; i < root.layer ; ++i)
// 	{
// 		p = p->child;
// 	}
}
