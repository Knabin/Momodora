#pragma once
#include "gameNode.h"

class stageManager;

enum PLAYERSTATE
{
	// IDLE
	LEFT_IDLE,
	RIGHT_IDLE,

	// 이동
	LEFT_MOVE,
	RIGHT_MOVE,
	
	// 근거리 공격
	LEFT_ATTACK,
	RIGHT_ATTACK,

	// 원거리 공격(Bullet)
	LEFT_ATTACKB,
	RIGHT_ATTACKB,

	// 원거리 차지 공격(Charge)
	LEFT_ATTACKC,
	RIGHT_ATTACKC,

	// 점프
	LEFT_JUMP,
	RIGHT_JUMP,

	// 찬영이 떨어트리기
	LEFT_FALL,
	RIGHT_FALL,

	// 찬영이 죽는다
	LEFT_DEAD,
	RIGHT_DEAD,
};

class player : public gameNode
{
private:
	image* _image;

	MYRECT _hitbox;

	float _x, _y;
	float _centerY;
	float _angle;
	float _speed;
	float _gravity;
	int _width;
	int _height;

	float _probeXL;
	float _probeXR;
	float _probeY;

	bool _isOnGround;
	bool _isAttacking;
	bool _isOnceAttack;
	bool _canMoveLeft;
	bool _canMoveRight;

	PLAYERSTATE _state;
	PLAYERSTATE _stateBefore;

	stageManager* _sm;

	animation* _ani_idle;
	animation* _ani_run;
	animation* _ani_attack;
	animation* _ani_jump;
	animation* _ani_dead;

	int _attackCount;
	int _attackCount2;
	
public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setAnimation(PLAYERSTATE state);

	bool isMoveLeft() 
	{
		if (_state == LEFT_MOVE || _state == LEFT_JUMP || _state == LEFT_FALL)
			return true;
		return false;
	}

	bool isMoveRight() 
	{
		if (_state == RIGHT_MOVE || _state == RIGHT_JUMP || _state == RIGHT_FALL)
			return true;
		return false;
	}

	bool isJumping()
	{
		if (_state == LEFT_JUMP || _state == RIGHT_JUMP) return true;
		return false;
	}

	bool isFalling()
	{
		if (_state == LEFT_FALL || _state == RIGHT_FALL) return true;
		return false;
	}

	bool isLeft()
	{
		return _state % 2 == 0 ? true : false;
	}

	float getX() { return _x; }
	void setX(float x) { _x = x; }

	float getY() { return _y; }
	void setY(float y) { _y = y; }

	float getCenterY() { return _centerY; }

	float getProbeXL() { return _probeXL; }
	float getProbeXR() { return _probeXR; }

	float getProbeY() { return _probeY; }
	void setProbeY(float probeY) { _probeY = probeY; }

	float getWidth() { return _width; }

	float getHeight() { return _height; }

	bool getIsAttacking() { return _isAttacking; }
	void setIsAttacking(bool att) { _isAttacking = att; }

	bool getIsOnGround() { return _isOnGround; }
	void setIsOnGround(bool isOnGround) 
	{
		_isOnGround = isOnGround; 
		_gravity = 0;
	}

	void setCanMoveLeft(bool canMoveLeft) { _canMoveLeft = canMoveLeft; }
	void setCanMoveRight(bool canMoveRight) { _canMoveRight = canMoveRight; }

	void setStageManagerMemoryAddressLink(stageManager* sm) { _sm = sm; }
};

