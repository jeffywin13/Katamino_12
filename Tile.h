#ifndef _TILE_H_ 
#include "stdafx.h"

#define MAX_GRID_COUNT	16
#define MAX_POSITION 60 // 12 x 5
#define _SPACE_ ' '
#define _MARK_ 'X'


typedef unsigned char UINT8;



class Color
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;

	Color()
	{
		r = 0;
		g = 0;
		b = 0;
	}

	Color(unsigned char red , unsigned char green , unsigned char blue)
	{
		r = red;
		g = green;
		b = blue;
	}
};


typedef struct type
{
	int w;
	int h;
	unsigned char* shape;
	int posCount;
	int pos[MAX_POSITION];
} strType;



void rotateUntiClockwise90(unsigned char* dst , strType src);
void flipLeftRight(strType type);
void displayType(strType t , unsigned char m);
bool shapeEqual(strType t1 , strType t2);


class Tile
{
public:
	int w;
	int h;
	Color c;
	char m;
	unsigned char* shape;
	int typeCount;
	strType* types;

//public:
// 	unsigned char squaresCount;
// 	unsigned char isAxialSymmetry;
// 	unsigned char isCentreSymmetry;


	Tile();
	~Tile();
	void init(unsigned char width , unsigned char height , char* squares , char mark , Color color);
	void initTypes();
	int deleteDup(strType* t , int count);
	void display();
	unsigned char checkParam(unsigned char width , unsigned char height , char squares[]);
	bool putIntoBoard(char* board , char spaceChar , int width , int height , int typeIndex , int positionIndex);
};

#endif