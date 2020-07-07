#pragma once
#include "gameNode.h"

class player;

enum class ENEMYDIRECTION
{
	LEFT_IDLE,
	RIGHT_IDLE,

	LEFT_MOVE,
	RIGHT_MOVE,

	LEFT_ATTACK,
	RIGHT_ATTACK,

	// prim
	LEFT_ACTION,
	RIGHT_ACTION,
};

class enemy : public gameNode
{
protected:
	image* _image;
	float _x;
	float _y;
	int _width;
	int _height;
	bool _isAlive;

	MYRECT _rc;			// 충돌 처리용
	MYRECT _rcCheck;	// 플레이어 체크용

	player* _player;

	ENEMYDIRECTION _direction;

public:
	enemy();
	~enemy();

	virtual HRESULT init(MYPOINT position);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void attack();
	virtual void checkPlayer();
	virtual void move();
	virtual void draw();

	void enemyDirection(int num) { _direction = (ENEMYDIRECTION)num; }
	inline MYRECT& getRect() { return _rc; }

	void setPlayerMemoryAddressLink(player * p) { _player = p; }
};

class oko : public enemy
{
private:
	image* _imageRound;
	animation* _ani_round;
	MYRECT _objectRc;

public:
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

	MYRECT _rcAttack;

	float _probeY;
	float _probeXL;
	float _probeXR;

	bool _isOnGround;

public:
	HRESULT init(MYPOINT position);
	void release();
	void update();
	void render();

	virtual void attack();
	virtual void checkPlayer();
	virtual void move();
	virtual void draw();

	void pixelCollision();
};

class bakman : public enemy
{
private:
	image* _bullet;
	animation* _ani_attack;

	float _bulletX, _bulletY;
	float _angle;
	float _gravity;
	int _count;

	bool _isFire;

public:
	HRESULT init(MYPOINT position);
	void release();
	void update();
	void render();

	virtual void attack();
	virtual void checkPlayer();
	virtual void move();
	virtual void draw();
};

// ===================================================
// ## 보스 몬스터 클래스 ##

class prim : public enemy
{
	animation* _ani_idle;
	animation* _ani_run;
	animation* _ani_angry;
	animation* _ani_attack;

public:
	HRESULT init(MYPOINT position);
	void release();
	void update();
	void render();

	virtual void attack();
	virtual void checkPlayer();
	virtual void move();
	virtual void draw();
};

class witch : public enemy
{
	image* _imageRight;
	image* _bulletLeft;
	image* _bulletRight;

	animation* _ani_idle_left;
	animation* _ani_idle_right;
	animation* _ani_attack_left;
	animation* _ani_attack_right;

	float _xRight, _yRight;

public:
	HRESULT init(MYPOINT position);
	void release();
	void update();
	void render();

	virtual void attack();
	virtual void checkPlayer();
	virtual void move();
	virtual void draw();
};

class rell : public enemy
{
	enum RELLSTATE
	{

	};

	image* _bullet;
	image* _snow;
	image* _snow2;

	animation* _ani_appear;
	animation* _ani_idle;
	animation* _ani_idle2;
	animation* _ani_attack;
	animation* _ani_ground;
	animation* _ani_fire;

public:
	HRESULT init(MYPOINT position);
	void release();
	void update();
	void render();

	virtual void attack();
	virtual void checkPlayer();
	virtual void move();
	virtual void draw();
};