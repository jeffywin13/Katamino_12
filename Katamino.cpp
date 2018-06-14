// Katamino.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "memory.h"
#include "Loop.h"
#include "Tile.h"
#include "Solution.h"

#include "Tree.h"


#define _ROW_ 5 // static value
#define _COL_ 3 // 3 <= _COL_ <= 12

#define _MAX_TILES_COUNT_ 12

void main();
void initTiles(Tile* pT);
int getCountFromChoose(int total , int n);
void getTilesIndex(UINT8 **index , int count , int max , int n);
int getCountOf1(int num , unsigned char* result , int n);

void printBoard(char* board , int row , int col);
void getPositionForTypes(Tile *pT , int N);

void compute(Solution* pS , Tile *pT , UINT8* index , int N);
char* solve(Tile* pT , UINT8* index , UINT8* types , UINT8* positions , int N);

// -- x --
void computeByTree(Tile *pT , unsigned char* index , int N);
void initTree(Tree* pTree , Tile* pT , unsigned char* index , int n);
// -- x --

int _tmain(int argc, _TCHAR* argv[])
{
	main();
	return 0;
}

void main()
{
	printf("**** KATAMINO START ****\n");

	Tile T[_MAX_TILES_COUNT_];
	initTiles(T);

	int N = _COL_; // 因为每个tile都由5个正方形组成，且固定为5行，所以拼多少列，就需要多少块

	// 因为N定了，棋盘的形状就定了
	// 所以就可以计算出12块中每一块的每个形态在棋盘中可能放置的位置
	// 以某种形态的左上角块在棋盘中的位置来表示该形态可能放置的位置
	// 例如：该位置为7 ， 即表示该形态的左上角块放在棋盘中第 7 / N 行， 第 7 % N 列
	getPositionForTypes(T , N);

	// 计算出12选N的可能的组合
	int group = getCountFromChoose(_MAX_TILES_COUNT_ , N);

	// 初始化下标数组
	int i = 0;
	UINT8 **index = new UINT8* [group];
	for(i = 0 ; i < group ; ++i)
		index[i] = new UINT8[N];

	// 给下标数组填数，每一行就是选出的一组N个tile的下标
	getTilesIndex(index , group , _MAX_TILES_COUNT_ , N);

// 	//---------------------------------------
// 	for(i = 0 ; i < group ; ++i)
// 	{
// 		printf("%3d , :  %d , %d , %d , %d\n" , i , index[i][0] , index[i][1] , index[i][2] , index[i][3]);
// 	}
// 	//---------------------------------------

	Solution S = Solution(N);

	// 计算每一组方块是否能拼成？如果能，保存解法
// for(int xx = 0 ; xx < 10 ; ++xx)
// { // this loop is for stress testing
	for(i = 0 ; i < group ; ++i)
	{
// if(i == 84)
// 	N = N;
		Solution localSolution = Solution(N); // 记录当前形态组合下的所有解，并去重
		printf("%3d/%d  ...   " , i , group);
		compute(&localSolution , T , index[i] , N);
		//computeByTree(T , index[i] , N);

		// 将当前形态的所有解，添加到全体解的数组中
		if(localSolution.nodeCount > 0)
		{
			if(S.pLastSolution == NULL)
			{
				S.pSolution = localSolution.pSolution;
				//S.pLastSolution = localSolution.pSolution;
			}
			else
				S.pLastSolution->next = localSolution.pSolution;

			S.pLastSolution = localSolution.pLastSolution;

			S.nodeCount += localSolution.nodeCount;
			localSolution.pSolution = NULL;
		}
	}
//}
	// 销毁下标数组
	for(i = 0 ; i < group ; ++i)
		delete[] index[i];
	delete[] index;

	printf("solutions = %d\n\n" , S.nodeCount);

// 	// print each solutionNode's graphic
// 	i = 0;
// 	strSolutionNode* pStr = S.pSolution;
// 	while(pStr != NULL)
// 	{
// 		printf("(%d)\n" , ++i);
// 		printBoard(pStr->graphic , _ROW_ , N);
// 		pStr = pStr->next;
// 	}
	printf("**** KATAMINO END ****\n");
	S.printToFile("D:\\KataminoSoution_");
}



void compute(Solution* pS , Tile *pT , UINT8* index , int N)
{
	char* board = new char[_ROW_ * N];
	memset(board , _SPACE_ , _ROW_ * N *sizeof(char));

	//-----------------------------------------
	// 	printBoard(board , _ROW_ , N);
	// 	for(int i = 0 ; i < N ; ++i)
	// 		pT[index[i]].display();
	//-----------------------------------------

	// 利用组中每块的每种形态(选1)的每种pos(选1)，进行穷举！
	int i = 0 , j = 0;
	UINT8* nodes = new UINT8[N];
	int typeArrayCount = 1; // 所有形态的组合数
	//nodes[i]表示这组积木中第i块积木所拥有的形态数
	for(i = 0 ; i < N ; ++i)
	{
		nodes[i] = pT[index[i]].typeCount;
		typeArrayCount *= nodes[i];
	}

	// 求出所有形态的组合 , typeArrayCount行 x N列 的数组
	UINT8** typeArray = new UINT8*[typeArrayCount];
	for(i = 0 ; i < typeArrayCount ; ++i)
		typeArray[i] = new UINT8[N];

	Loop typeLoop = Loop(N , nodes);
	typeLoop.visit(typeArray);

printf(" , %d types\n" , typeArrayCount);

	// 循环每一形态组合
	for(i = 0 ; i < typeArrayCount ; ++i)
	{	
// debug: 输出当前选中的形态组合
// for(int j = 0 ; j < N ; ++j)
// {
// 	displayType(pT[index[j]].types[typeArray[i][j]] , pT[index[j]].m);
// }
// printf("--------------\n");

		// 求出当前形态组合中，所有摆放位置的组合

		UINT8* pos = new UINT8[N];
		int posArrayCount = 1;
		
		for(j = 0 ; j < N ; ++j)
		{
			pos[j] = pT[index[j]].types[typeArray[i][j]].posCount;
			posArrayCount *= pos[j];
		}

		// 有可能某块的某种形态，在当前棋盘上，根本放不下(posCount == 0)
		// 例如：N=4时，大横条就放不下。
		if(posArrayCount == 0)
		{
			delete[] pos;
			continue;
		}

		UINT8** posArray = new UINT8*[posArrayCount];
		for(j = 0 ; j < posArrayCount ; ++j)
			posArray[j] = new UINT8[N];


		Loop posLoop = Loop(N , pos);
		posLoop.visit(posArray);

		// 判断当前的形态组合，放在当前的位置组合，是否恰好拼满
		// 如果拼满，就是一组可行解
		for(j = 0 ; j < posArrayCount ; ++j)
		{			
			char* resultGraphic = solve(pT , index , typeArray[i] , posArray[j] , N);
			if(NULL != resultGraphic)
			{
				if(false == pS->isContain(resultGraphic))
					pS->addSolution(index , typeArray[i] , posArray[j] , resultGraphic);
			}
		}

		// release
		for(j = 0 ; j < posArrayCount ; ++j)
			delete[] posArray[j];
		delete[] posArray;

		delete[] pos;
		
	}

//	printf(" , solution's count = %d\n" , pS->nodeCount);


	// release memory
	for(i = 0 ; i < typeArrayCount ; ++i)
		delete[] typeArray[i];
	delete[] typeArray;

	delete[] nodes;

	delete[] board;
	return;
}



char* solve(Tile* pT , UINT8* index , UINT8* types , UINT8* positions , int N)
{
	char* board = new char[_ROW_ * N];
	memset(board , _SPACE_ , _ROW_ * N *sizeof(char));

	for(int i = 0 ; i < N ; ++i)
	{
		bool flag = pT[index[i]].putIntoBoard(board , _SPACE_ , N , _ROW_ , types[i] , positions[i]);
		if(flag == false)
		{
			delete[] board;
			return NULL;
		}
	}

	return board;
}


void initTiles(Tile* pT)
{
	// A
	char squaresA[9] = {_MARK_ , _MARK_ , _SPACE_ , _SPACE_ , _MARK_ , _MARK_ , _SPACE_ , _SPACE_ , _MARK_};
	pT[0].init(3 , 3 , squaresA , 'A' , Color(142 , 88 , 196));
	//pT[0].display();

	// B
	char squaresB[6] = {_MARK_ , _MARK_ , _MARK_ , _SPACE_ , _MARK_ , _MARK_};
	pT[1].init(3 , 2 , squaresB , 'B' , Color(22 , 160 , 236));
	//pT[1].display();

	// C
	char squaresC[9] = {_SPACE_ , _MARK_ , _MARK_ , _SPACE_ , _MARK_ , _SPACE_ , _MARK_ , _MARK_ , _SPACE_};
	pT[2].init(3 , 3 , squaresC , 'C' , Color(135 , 135 , 135));
	//pT[2].display();

	// D
	char squaresD[6] = {_MARK_ , _MARK_ , _MARK_ , _MARK_ , _SPACE_ , _MARK_};
	pT[3].init(3 , 2 , squaresD , 'D' , Color(227 , 227 , 31));
	//pT[3].display();

	// E
	char squaresE[8] = {_SPACE_ , _SPACE_ , _MARK_ , _MARK_ , _MARK_ , _MARK_ , _MARK_ , _SPACE_};
	pT[4].init(4 , 2 , squaresE , 'E' , Color(26 , 26 , 26));
	//pT[4].display();

	// F
	char squaresF[8] = {_MARK_ , _SPACE_ , _SPACE_ , _SPACE_ , _MARK_ , _MARK_ , _MARK_ , _MARK_};
	pT[5].init(4 , 2 , squaresF , 'F' , Color(219 , 115 , 17));
	//pT[5].display();

	// G
	char squaresG[9] = {_MARK_ , _MARK_ , _MARK_ , _SPACE_ , _MARK_ , _SPACE_ ,  _SPACE_ , _MARK_ , _SPACE_};
	pT[6].init(3 , 3 , squaresG , 'G' , Color(50 , 155 , 60));
	//pT[6].display();

	// H
	char squaresH[8] = {_MARK_ , _MARK_ , _MARK_ , _MARK_ , _SPACE_ ,  _MARK_ , _SPACE_ , _SPACE_};
	pT[7].init(4 , 2 , squaresH , 'H' , Color(110 , 75 , 35));
	//pT[7].display()

	// I
	char squaresI[9] = {_SPACE_ ,  _MARK_ , _SPACE_ , _MARK_ , _MARK_ , _MARK_ , _SPACE_ ,  _MARK_ , _SPACE_};
	pT[8].init(3 , 3 , squaresI , 'I' , Color(200 , 45 , 30));
	//pT[8].display()

	// J
	char squaresJ[9] = {_SPACE_ ,  _MARK_ , _MARK_ , _MARK_ , _MARK_ , _SPACE_ , _SPACE_ ,  _MARK_ , _SPACE_};
	pT[9].init(3 , 3 , squaresJ , 'J' , Color(85 , 205 , 30));
	//pT[9].display()

	// K
	char squaresK[9] = {_MARK_ ,  _MARK_ , _MARK_ , _MARK_ , _SPACE_ , _SPACE_ , _MARK_ ,  _SPACE_ , _SPACE_};
	pT[10].init(3 , 3 , squaresK , 'K' , Color(40 , 40 , 135));
	//pT[10].display()

	// L
	char squaresL[5] = {_MARK_ ,  _MARK_ , _MARK_ , _MARK_ , _MARK_};
	pT[11].init(5 , 1 , squaresL , 'L' , Color(235 , 130 , 160));
	//pT[11].display()
}




int getCountFromChoose(int total , int n)
{
	int i = 0 , j = 0 , a = 1 , b = 1;
	for(i = 1 , j = total ; i <= n ; ++i , --j)
	{
		a *= i;
		b *= j;
	}

	return (b / a);
}


void getTilesIndex(UINT8 **index , int count , int max , int n)
{
	int i = 0 ; 
	int limit = 1;
	for(int i = 0 ; i < max ; ++i)
		limit *= 2;

	int cnt = 0;
	for(int i = 0 ; i < limit ; ++i)
	{ // from 0 to (2 ^ max - 1)
		if (n == getCountOf1(i , index[cnt] , n))
			++cnt;
	}
}

int getCountOf1(int num , unsigned char* result , int n)
{	//	计算num的二进制表示中，数字1的个数
	int ret = 0 , i = 0;
	unsigned char r[_MAX_TILES_COUNT_];
	for(i = 0 ; i < _MAX_TILES_COUNT_ ; ++i)
	{
		if(num & 1)
		{
			r[ret] = (unsigned char)i;
			++ret;
		}
		num = num >> 1;
	}

	if(ret == n)
		memcpy(result , r , sizeof(unsigned char) * n);

	return ret;
}






void printBoard(char* board , int row , int col)
{
	printf("\n");
	for(int i = 0 ; i < row ; ++i)
	{
		for(int j = 0 ; j < col ; ++j)
		{
			char c = board[i * col + j];
			if(c == _SPACE_)
				printf(" .");
			else
				printf("%2c" , c);
		}
		printf("\n");
	}
	printf("\n");
}


void getPositionForTypes(Tile *pT , int N)
{
	strType *p = NULL;
	for(int i = 0 ; i < _MAX_TILES_COUNT_ ; ++i)
	{
		for(int j = 0 ; j < pT[i].typeCount ; ++j)
		{
			UINT8 w = pT[i].types[j].w;
			UINT8 h = pT[i].types[j].h;
			p = &(pT[i].types[j]);
			p->posCount = 0;

			if(p->w > N || p->h > _ROW_)
				continue;
			
			for(int k = 0 ; k < _ROW_ * N ; ++k)
			{
				if((k % N + p->w <= N) && (k / N + p->h <= _ROW_))
				{
					(p->pos)[p->posCount] = k;
					++(p->posCount);
				}
			}
			//----------------------------------------
			displayType(*p , 'X');
			printf("%d positions:\n" , p->posCount);
			for(int x = 0 ; x < p->posCount ; ++x)
				printf("%d , " , p->pos[x]);
			printf("\n\n\n");
			//----------------------------------------
		}
	}

	return;
}
















void computeByTree(Tile *pT , unsigned char* index , int N)
{
	char* board = new char[_ROW_ * N];
	memset(board , _SPACE_ , _ROW_ * N *sizeof(char));

	//-----------------------------------------
	// 	printBoard(board , _ROW_ , N);
	// 	for(int i = 0 ; i < N ; ++i)
	// 		pT[index[i]].display();
	//-----------------------------------------

	// 利用组每块每种形态的pos，进行穷举！
	int i = 0 , j = 0;
	int* nodes = new int[N];
	for(i = 0 ; i < N ; ++i)
		nodes[i] = pT[index[i]].typeCount;

	Tree typesTree = Tree(N , nodes); // 创建一棵树
	typesTree.destroy();
	if(0){
		// 树的根节点到每一个叶子节点的路径，都代表一种types的组合
		// 通过遍历这棵树，得到所有的备选形态组合
		int **typeArray = new int*[typesTree.leafCount];
		for(i = 0 ; i < typesTree.leafCount ; ++i)
			typeArray[i] = new int[N];

		typesTree.visit(typeArray);
		typesTree.destroy();

		// 	for(i = 0 ; i < typesTree.leafCount ; ++i)
		// 	{
		// 		int *position = new int[N];
		// 		for(j = 0 ; j < N ; ++j)
		// 			position[j] = pT[index[j]].types[typeArray[i][j]].posCount;
		// 
		// 		Tree positionsTree = Tree(N ,position);
		// 		// 获得当前type下，所有备选的position的组合
		// 		int **positionArray = new int*[positionsTree.leafCount];
		// 		for(j = 0 ; j < positionsTree.leafCount ; ++j)
		// 			positionArray[j] = new int[N];
		// 
		// 		positionsTree.visit(positionArray);
		// 		positionsTree.destroy();
		// 
		// 		// 判断特定type，特定position，是否能拼满
		// 
		// 
		// 		delete[] position;
		// 	}


		// release memory
		for(i = 0 ; i < typesTree.leafCount ; ++i)
			delete[] typeArray[i];
		delete[] typeArray;
	}

	delete[] nodes;

	delete[] board;
	return;
}


void initTree(Tree* pTree , Tile* pT , unsigned char* index , int n)
{
	for(int i = 0 ; i < n ; ++i)
	{
		int idx = index[i];
		int nodeCount = pT[i].typeCount;
		pTree->init(idx , nodeCount);
	}
}