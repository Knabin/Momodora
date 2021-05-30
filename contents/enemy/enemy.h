#pragma once
#include "gameNode.h"

class Player;

enum class ENEMY_DIRECTION
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

	ENEMY_DIRECTION _direction;

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

	void enemyDirection(int num) { _direction = (ENEMY_DIRECTION)num; }
	inline MYRECT& getRect() { return _rc; }

	void setPlayerMemoryAddressLink(Player * p) { _player = p; }
};