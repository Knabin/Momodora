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

struct tagBulletEnemy
{
	image* image;
	float x, y;
	float fireX, fireY;
	float angle;
	float speed;
	float range;
	bool isFire;
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
	
	int _hp;
	int _maxHp;

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

	virtual void checkCollision();

	virtual int getHP() { return _hp; }
	virtual int getMaxHP() { return _maxHp; }

	void enemyDirection(int num) { _direction = (ENEMYDIRECTION)num; }
	inline MYRECT& getRect() { return _rc; }

	void setPlayerMemoryAddressLink(player * p) { _player = p; }
};

class oko : public enemy
{
private:
	image* _imageRound;
	animation* _ani_round;
	RECT _objectRc;

	bool _isOnceAttacked;

public:
	HRESULT init(MYPOINT position);
	void release();
	void update();
	void render();

	virtual void attack();
	virtual void checkPlayer();
	virtual void move();
	virtual void draw();

	virtual void checkCollision();

	//void setObjectRect(MYRECT& rc) { _objectRc = rc; }
	void setObjectRect(RECT& rc) { _objectRc = rc; }
};

class monkey : public enemy
{
private:
	animation* _ani_run;
	animation* _ani_attack;

	MYRECT _rcHit;
	MYRECT _rcAttack;

	float _probeY;
	float _probeXL;
	float _probeXR;

	bool _isOnGround;
	bool _canMoveLeft;
	bool _canMoveRight;

public:
	HRESULT init(MYPOINT position);
	void release();
	void update();
	void render();

	virtual void attack();
	virtual void checkPlayer();
	virtual void move();
	virtual void draw();

	virtual void checkCollision();
	void pixelCollision();
};

class bakman : public enemy
{
private:
	image* _bullet;
	animation* _ani_attack;
	animation* _ani_attack2;

	MYRECT _rcBullet;

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

	virtual void checkCollision();
};

// ===================================================
// ## 보스 몬스터 클래스 ##

class prim : public enemy
{
	animation* _ani_start;
	animation* _ani_run;
	animation* _ani_angry;
	animation* _ani_attack;

	MYRECT _rcAttack;

	bool _isStart;

public:
	HRESULT init(MYPOINT position);
	void release();
	void update();
	void render();

	virtual void attack();
	virtual void checkPlayer();
	virtual void move();
	virtual void draw();

	virtual void checkCollision();
	void start();
};

class witch : public enemy
{
	image* _imageRight;

	tagBulletEnemy _leftBullet1;
	tagBulletEnemy _leftBullet2;
	tagBulletEnemy _rightBullet1;
	tagBulletEnemy _rightBullet2;

	animation* _ani_idle_left;
	animation* _ani_idle_right;
	animation* _ani_attack_left;
	animation* _ani_attack_right;

	MYRECT _rcRight;

	float _xRight, _yRight;

	int _attackCount;
	int _alpha;

	bool _isStart;
	bool _attackVer;

public:
	HRESULT init(MYPOINT position);
	void release();
	void update();
	void render();

	virtual void attack();
	virtual void checkPlayer();
	virtual void move();
	virtual void draw();

	virtual void checkCollision();
	void start();
};

class rell : public enemy
{
	float _xLarge, _yLarge;	// 3페이즈용

	enum class RELLSTATE
	{
		LEFT_IDLE,
		RIGHT_IDLE,

		LEFT_GROUND,
		RIGHT_GROUND,

		LEFT_ATTACK,
		RIGHT_ATTACK,

		PRAY,	// 1
		AIR,	// 2, 떠 있는 상태
		DEAD,
	};

	enum class RELLMODE
	{
		MODE1,
		MODE2,
		MODE3,
	};
	
	image* _image;

	image* _bullet;
	image* _snow;
	image* _snow2;

	// 1페이즈
	animation* _ani_idle1;
	animation* _ani_ground1;
	animation* _ani_attack1;
	animation* _ani_pray;

	//2
	animation* _ani_attack2;

	//3
	animation* _ani_idle3;
	animation* _ani_ground3;
	animation* _ani_attack3;

	RELLSTATE _state;
	RELLMODE _mode;

	bool _isStart;
public:
	HRESULT init(MYPOINT position);
	void release();
	void update();
	void render();

	virtual void attack();
	virtual void checkPlayer();
	virtual void move();
	virtual void draw();

	virtual void checkCollision();
	void start();
};