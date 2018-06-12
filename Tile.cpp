#include "stdafx.h"
#include "Tile.h"





Tile::Tile()
{
	w = 0;
	h = 0;
	c = Color();
	shape = NULL;
	m = ' ';
	typeCount = 0;
	types = NULL;
}

void Tile::init(UINT8 width , UINT8 height , char* squares , char mark , Color color)
{
	UINT8 s = checkParam(width , height , squares);
	if(s == 0)
		return;
	else
	{
		//squaresCount = s;
		w = width;
		h = height;
		c = color;
		m = mark;
		shape = new UINT8[w * h];
		for(int i = 0 ; i < w * h ; ++i)
			shape[i] = squares[i] == _SPACE_ ? 0 : 1;
	}

	initTypes();

	// show for debug
// 	printf("typeCount for %c is : %d\n\n" , m , typeCount);
// 	for(int k = 0 ; k < typeCount ; ++k)
// 		displayType(types[k] , m);
}

void Tile::initTypes()
{
	strType t[8];
	int size = w * h;
	int memSize = size * sizeof(UINT8);
	// 0->3 , rotate unti-clockwise 90 each time; 4 is flip left and right by 0 ; same as 5&1 , 6&2 , 7&3

	t[0].w = w;
	t[0].h = h;
	t[0].shape = new UINT8[size];
	memcpy(t[0].shape , shape , memSize);
	//displayType(t[0] , m);

	// 1
	t[1].w = h;
	t[1].h = w;
	t[1].shape = new UINT8[size];
	rotateUntiClockwise90(t[1].shape , t[0]);
	//displayType(t[1] , m);

	// 2
	t[2].w = w;
	t[2].h = h;
	t[2].shape = new UINT8[size];
	rotateUntiClockwise90(t[2].shape , t[1]);
	//displayType(t[2] , m);

	// 3
	t[3].w = h;
	t[3].h = w;
	t[3].shape = new UINT8[size];
	rotateUntiClockwise90(t[3].shape , t[2]);
	//displayType(t[3] , m);

	// 4
	t[4] = t[0];
	t[4].shape = new UINT8[size];
	memcpy(t[4].shape , t[0].shape , memSize);
	flipLeftRight(t[4]);
	//displayType(t[4] , m);

	// 5
	t[5] = t[1];
	t[5].shape = new UINT8[size];
	memcpy(t[5].shape , t[1].shape , memSize);
	flipLeftRight(t[5]);
	//displayType(t[5] , m);

	// 6
	t[6] = t[2];
	t[6].shape = new UINT8[size];
	memcpy(t[6].shape , t[2].shape , memSize);
	flipLeftRight(t[6]);
	//displayType(t[6] , m);

	// 7
	t[7] = t[3];
	t[7].shape = new UINT8[size];
	memcpy(t[7].shape , t[3].shape , memSize);
	flipLeftRight(t[7]);
	//displayType(t[7] , m);

	typeCount = deleteDup(t , 8);
	types = new strType[typeCount];
	memcpy(types , t , sizeof(strType) * typeCount);
}

int Tile::deleteDup(strType* t , int count)
{
	int ret = count;
	strType tmp;
	for(int i = 1 ; i < ret ; ++i)
	{
		for(int j = 0 ; j < i ; ++j)
		{
			if(shapeEqual(t[i] , t[j]))
			{	// duplicate , swap with "last"
				if(i == ret - 1)
					return i;

				tmp = t[i];
				t[i] = t[ret - 1];
				t[ret - 1] = tmp;
				--ret;
				j = -1; // restart from j = 0  , after ++j
			}
		}
	}

	return ret;
}

bool shapeEqual(strType t1 , strType t2)
{
	if(t1.w == t2.w && t1.h == t2.h)
	{
		for(int i = 0 ; i < t1.w * t1.h ; ++i)
		{
			if(t1.shape[i] != t2.shape[i])
				return false;
		}
		return true;
	}
	else
		return false;
}

void rotateUntiClockwise90(UINT8* dst , strType src)
{
	int w = src.h;
	int h = src.w;
	for(int i = 0 ; i < h ; ++i)
	{
		for(int j = 0 ; j < w ; ++j)
		{
			dst[i * w + j] = src.shape[j * src.w + (src.w - 1) - i];
		}
	}
}

void flipLeftRight(strType type)
{
	UINT8 tmp;
	for(int i = 0 ; i < type.h ; ++i)
	{
		for(int j = 0 ; j < type.w / 2 ; ++j)
		{
			tmp = type.shape[i * type.w + j];
			type.shape[i * type.w + j] = type.shape[i * type.w + type.w - 1 -j];
			type.shape[i * type.w + type.w - 1 -j] = tmp;
		}
	}
}

void displayType(strType t , UINT8 m)
{
	printf("\n");
	for(int i = 0 ; i < t.h ; ++i)
	{
		for(int j = 0 ; j < t.w ; ++j)
			printf("%2c" , t.shape[i * t.w + j] == 0 ? '.' : m);
		printf("\n");
	}
}

Tile::~Tile()
{
	if(shape != NULL)
		delete[] shape;

	if(types != NULL)
	{
		for(int i = 0 ; i < typeCount ; ++i)
		{
			if(types[i].shape != NULL)
				delete[] types[i].shape;
		}
		delete[] types;
	}
}


void Tile::display()
{
	for(int i = 0 ; i < h ; ++i)
	{
		for(int j = 0 ; j < w ; ++j)
			printf("%2c" , shape[i * w + j] == 0 ? '.' : m);
		printf("\n");
	}
	printf("\n");
}

UINT8 Tile::checkParam(UINT8 width , UINT8 height , char squares[])
{
	unsigned int a = width * height;
	if(a == 0 || a > MAX_GRID_COUNT)
		return 0;

	UINT8 count = 0;
	for(int i = 0 ; i < MAX_GRID_COUNT ; ++i)
	{
		if(squares[i] != _SPACE_)
			++count;
	}

	return count;
}

bool Tile::putIntoBoard(char* board , char spaceChar , int width , int height , int typeIndex , int positionIndex)
{
	strType type = types[typeIndex];
	UINT8* shape = type.shape;
	int pos = type.pos[positionIndex];
	int smallHeight = type.h;
	int smallWidth = type.w;
	int startH = pos / width;
	int startW = pos % width;
	for(int i = 0 ; i < smallHeight ; ++i)
	{
		for(int j = 0 ; j < smallWidth ; ++j)
		{
			char* pInBoard = board + ((i + startH) * width + (j + startW));
			UINT8* pType = shape + (i * smallWidth + j);
			if(*pInBoard != spaceChar)
			{ // 当前位置已被占
				if(*pType != 0)
					return false;
			}
			else
			{ // 当前位置空着
				if(*pType == 1)
					*pInBoard = m;
			}
		}
	}

	return true;
}
