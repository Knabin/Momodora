#pragma once
#include "gameNode.h"

class player;

enum ENEMYDIRECTION
{
	IDLE,
	MOVE
};

class enemy : public gameNode
{
protected:
	image* _image;
	float _x;
	float _y;
	bool _isAlive;

	MYRECT _rc;			// 충돌 처리용
	MYRECT _rcCheck;	// 플레이어 체크용

	player* _player;

	ENEMYDIRECTION _direction;

public:
	enemy();
	~enemy();

	virtual HRESULT init();
	virtual HRESULT init(MYPOINT position);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void attack();
	virtual void checkPlayer();
	virtual void move();
	virtual void draw();

	void enemyDirection(int num) { _direction = (ENEMYDIRECTION)num; }
	inline MYRECT getRect() { return _rc; }

	void setPlayerMemoryAddressLink(player * p) { _player = p; }
};

class oko : public enemy
{
private:
	image* _imageRound;
	animation* _ani_round;
	MYRECT _objectRc;

public:
	HRESULT init();
	HRESULT init(MYPOINT position);
	void release();
	void update();
	void render();

	virtual void attack();
	virtual void checkPlayer();
	virtual void move();
	virtual void draw();

	void setObjectRect(MYRECT& rc) { _objectRc = rc; }
};

class monkey : public enemy
{
private:
	animation* _ani_run;
	animation* _ani_attack;

public:
	HRESULT init();
	HRESULT init(MYPOINT position);
	void release();
	void update();
	void render();

	virtual void attack();
	virtual void checkPlayer();
	virtual void move();
	virtual void draw();
};

class bakman : public enemy
{
private:
	image* _bullet;
	animation* _ani_attack;

	float _x, _y;
	float _bulletX, _bulletY;
	float _angle;
	float _gravity;
	int _count;

	bool _isFire;

public:
	HRESULT init();
	HRESULT init(MYPOINT position);
	void release();
	void update();
	void render();

	virtual void attack();
	virtual void checkPlayer();
	virtual void move();
	virtual void draw();
};