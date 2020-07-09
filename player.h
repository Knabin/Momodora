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

	// 원거리(차지) 공격
	LEFT_ATTACKC,
	RIGHT_ATTACKC,

	// 올라가기
	LEFT_JUMP,
	RIGHT_JUMP,

	// 떨어지기
	LEFT_FALL,
	RIGHT_FALL,

	// 죽음
	LEFT_DEAD,
	RIGHT_DEAD,
};

struct tagBullet
{
	image* image;
	RECT rc;
	float x, y;
	float fireX, fireY;
	float speed;
	float angle;
	float range;
};

class player : public gameNode
{
private:
	image* _image;
	image* _effect;

	MYRECT _hitbox;
	MYRECT _attackRc;

	float _x, _y;
	float _centerY;
	float _angle;
	float _speedX;
	float _speedY;
	float _gravity;
	int _width;
	int _height;

	int _hp;

	float _probeXL;
	float _probeXR;
	float _probeY;
	float _probeYT;

	bool _isOnGround;
	bool _isCameraShaking;
	bool _isOnceAttack;
	bool _isAttacked;
	bool _isCheckAttack;

	bool _canMoveLeft;
	bool _canMoveRight;

	PLAYERSTATE _state;
	PLAYERSTATE _stateBefore;

	stageManager* _sm;

	animation* _ani_idle;
	animation* _ani_run;
	animation* _ani_attack;
	animation* _ani_jump;
	animation* _ani_throw;
	animation* _ani_dead;
	animation* _ani_charge;

	animation* _ani_effect_charge;

	int _attackCount;
	int _attackCount2;
	int _redAlpha;
	int _attackedCount;

	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;
	bool _charge;
	bool _chargeFull;
	
public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setAnimation(PLAYERSTATE state);

	void fireBullet();
	void fireChargeBullet();
	void moveBullet();
	void renderBullet();
	void removeBullet(int index);


	bool isMovingLeft() 
	{
		if (_state == LEFT_MOVE || ((_state == LEFT_JUMP || _state == LEFT_FALL) && _angle != PI / 2))
			return true;
		return false;
	}

	bool isMovingRight() 
	{
		if (_state == RIGHT_MOVE || ((_state == RIGHT_JUMP || _state == RIGHT_FALL) && _angle != PI / 2))
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

	bool isAttacking()
	{
		if (_state == LEFT_ATTACK || _state == RIGHT_ATTACK) return true;
		return false;
	}

	bool isLeft()
	{
		return _state % 2 == 0 ? true : false;
	}

	void setPointLeftStart();
	void setPointRightStart();

	float getX() { return _x; }
	void setX(float x) { _x = x; }

	float getY() { return _y; }
	void setY(float y) { _y = y; }

	int getHP() { return _hp; }
	void setHP(int hp) { _hp = hp; }

	float getCenterY() { return _centerY; }

	float getProbeXL() { return _probeXL; }
	float getProbeXR() { return _probeXR; }
	float getProbeYT() { return _probeYT; }

	float getProbeY() { return _probeY; }
	void setProbeY(float probeY) { _probeY = probeY; }

	int getWidth() { return _width; }
	int getHeight() { return _height; }

	MYRECT& getHitbox() { return _hitbox; }
	MYRECT& getAttackRc() { return _attackRc; }

	void setGravity(float gravity) { _gravity = gravity; }

	float getSpeedX() { return _speedX; }
	float getSpeedY() { return _speedY; }

	void setAngle(float angle) { _angle = angle; }

	bool getIsCameraShaking() { return _isCameraShaking; }
	void setIsCameraShaking(bool att) { _isCameraShaking = att; }

	bool getIsOnGround() { return _isOnGround; }
	void setIsOnGround(bool isOnGround) 
	{
		_isOnGround = isOnGround; 
		_gravity = 0;
	}

	bool getIsAttacked() { return _isAttacked; }
	void setIsAttacked(bool isAttacked) { _isAttacked = isAttacked; }

	bool getIsCheckAttack() { return _isCheckAttack; }
	void setIsCheckAttack(bool check) { _isCheckAttack = check; }

	bool getCanMoveLeft() { return _canMoveLeft; }
	void setCanMoveLeft(bool canMoveLeft) { _canMoveLeft = canMoveLeft; }

	bool getCanMoveRight() { return _canMoveRight; }
	void setCanMoveRight(bool canMoveRight) { _canMoveRight = canMoveRight; }

	void setStageManagerMemoryAddressLink(stageManager* sm) { _sm = sm; }
};

