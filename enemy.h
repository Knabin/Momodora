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

	void pixelCollision();
};

class bakman : public enemy
{
private:
	image* _bullet;
	animation* _ani_attack;
	animation* _ani_attack2;

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
	animation* _ani_start;
	animation* _ani_run;
	animation* _ani_angry;
	animation* _ani_attack;

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

	void start();
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

	int _attackCount;

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

	void start();
};

class rell : public enemy
{
	float _xLarge, _yLarge;	// 3페이즈용

	enum class RELLSTATE
	{
		LEFT_IDLE,
		RIGHT_IDLE,

		LEFT_MOVE,	// 1, 텔레포트
		RIGHT_MOVE,

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

	void start();
};