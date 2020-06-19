#pragma once
#include "gameNode.h"
#define GROUNDMAX 12
#define ENEMYMAX 2
#define LADDERMAX 20
#define FRUITMAX 8
#define NIDDLEMAX 3

enum PLAYERDIRECTION
{
	IDLE,
	LEFT_IDLE,
	RIGHT_IDLE,
	LEFT_MOVE,
	RIGHT_MOVE,
	LEFT_JUMP,
	RIGHT_JUMP,
	LADDER_UP,
	LADDER_DOWN,
	DEAD
};

enum PLAYERIMG
{
	IMG_IDLE,
	IMG_WALK,
	IMG_JUMP,
	IMG_LADDER,
	IMG_DEAD,
	IMG_END
};

enum OBJECTTYPE
{
	OBJECT_FRUIT,
	OBJECT_NIDDLE,
	OBJECT_POT,
	OBJECT_LADDER,
};

struct tagPlayer
{
	POINT_FLOAT point;
	PLAYERDIRECTION direction;

	int width, height;

	float jumpPower;
	float gravity;
	float angle;

	image* img;
};

struct tagEnemy
{
	MYRECT rc;
	image* img;
	int speed;
	int flag;
};

struct tagObject
{
	MYRECT rc;
	image* img;
	OBJECTTYPE type;
	bool isView;
};


struct tagGround
{
	MYRECT rc;
	image* img;
	tagObject* fruit;
	tagObject* niddle;
	tagObject* ladder;
};



class playGround : public gameNode
{
private:
	MYRECT _playGround;
	
	image* _groundImg;
	image* _playerImg[IMG_END];
	image* _ladderImg;
	image* _fruitImg;
	image* _niddleImg;

	tagPlayer _player;
	tagGround _ground[GROUNDMAX];
	tagEnemy _enemy[ENEMYMAX];

	tagObject _ladder[LADDERMAX];
	tagObject _fruit[FRUITMAX];
	tagObject _niddle[NIDDLEMAX];

	int _count, _index;
	int _countEnemy;
	int _countFruit;
	int _countNiddle;
	int _countLadder;

	int _score;

	bool _isClear;
	bool _isGameOver;

	int _loopX1, _loopX2;

public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	int getImgWidth(int index);
	int getImgHeight(int index);

	bool ladderCheck();

	bool isPlayerOnGround();

	void checkCollision();

};

