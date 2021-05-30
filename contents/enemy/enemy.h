﻿#pragma once
#include "gameNode.h"

class Player;

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
	Image* Image;

	MYRECT rc;
	float x, y;
	float fireX, fireY;
	float angle;
	float speed;
	float range;
	float count;

	bool isFire;
};

class Enemy : public GameNode
{
protected:
	Image* _image;
	float _x;
	float _y;
	int _width;
	int _height;
	bool _isAlive;
	
	int _hp;
	int _maxHp;

	MYRECT _rc;			// 충돌 처리용
	//MYRECT _rcCheck;	// 플레이어 체크용

	Player* _player;

	ENEMYDIRECTION _direction;

public:
	virtual HRESULT init(MYPOINT position) = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	virtual void attack() = 0;
	virtual void checkPlayer() = 0;
	virtual void move() = 0;
	virtual void draw() = 0;

	virtual void checkCollision() = 0;

	virtual int getHP() { return _hp; }
	virtual int getMaxHP() { return _maxHp; }

	void enemyDirection(int num) { _direction = (ENEMYDIRECTION)num; }
	inline MYRECT& getRect() { return _rc; }

	void setPlayerMemoryAddressLink(Player * p) { _player = p; }
};

class oko : public Enemy
{
private:
	Image* _imageRound;
	Animation* _ani_round;
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

	void setObjectRect(RECT& rc) { _objectRc = rc; }
};

class monkey : public Enemy
{
private:
	Animation* _ani_run;
	Animation* _ani_attack;

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

class bakman : public Enemy
{
private:
	Image* _bullet;
	Animation* _ani_attack;
	Animation* _ani_attack2;

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

class prim : public Enemy
{
	Animation* _ani_start;
	Animation* _ani_run;
	Animation* _ani_angry;
	Animation* _ani_attack;

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

class witch : public Enemy
{
	Image* _imageRight;
	Image* _imageLeftAttackBack;

	tagBulletEnemy _leftBullet1[3];
	tagBulletEnemy _leftBullet2[10];
	tagBulletEnemy _rightBullet1;
	tagBulletEnemy _rightBullet2;

	Animation* _ani_idle_left;
	Animation* _ani_idle_right;
	Animation* _ani_attack_left;
	Animation* _ani_attack_right;

	MYRECT _rcRight;
	MYRECT _check;

	POINT _pts[8];

	float _xRight, _yRight;

	int _attackCount;
	int _leftAttack2Count;
	int _alpha;

	float _leftAttackX;

	bool _isStart;
	bool _attackVer;
	bool _canCheckRight2;

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

	const POINT& calculateBezierPoint(float time, POINT startPoint, POINT curPoint1, POINT curPoint2, POINT endPoint)
	{
		// 베지어 3차 곡선 공식
		// [x,y] = (1–t)^3 * P0 + 3(1–t)^2 * t * P1 + 3(1–t)t^2 * P2 + t^3 * P3

		float a1 = 1 - time;		// (1-t)
		float a2 = a1 * a1;			// (1-t)^2
		float a3 = a2 * a1;			// (1-t)^3

		float t2 = time * time;		// t^2
		float t3 = t2 * time;		// t^3

		POINT p = { startPoint.x * a3, startPoint.y * a3 };
		p.x += 3 * a2 * time * curPoint1.x;
		p.y += 3 * a2 * time * curPoint1.y;
		p.x += 3 * a1 * t2 * curPoint2.x;
		p.y += 3 * a1 * t2 * curPoint2.y;
		p.x += t3 * endPoint.x;
		p.y += t3 * endPoint.y;

		return p;
	}

	void attackWithLeftBullet(int index);
};

class rell : public Enemy
{
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
	
	Image* _image;

	Image* _bullet;
	Image* _fire;
	Image* _snow;
	Image* _snow2;

	// 1페이즈
	Animation* _ani_idle1;
	Animation* _ani_ground1;
	Animation* _ani_attack1;
	Animation* _ani_pray;
	Animation* _ani_effect_fire;

	//2
	Animation* _ani_attack2;
	Animation* _ani_snow;

	//3
	Animation* _ani_idle3;
	Animation* _ani_ground3;
	Animation* _ani_attack3;

	RELLSTATE _state;
	RELLMODE _mode;

	tagBulletEnemy _bullet1[3];
	tagBulletEnemy _bullet2[16];
	tagBulletEnemy _ground[3];

	MYRECT _rcPray;

	bool _isStart;
	bool _attackVer;
	bool _shootCount;
	bool _canPray;

	int _attackCount;
	int _prayCount;
	int _groundCount;

	float _yUp;	// 2페이즈용
	float _xLarge, _yLarge;	// 3페이즈용

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
	bool changeDirectionToLeft();
	void start();
	void changeMode();
};