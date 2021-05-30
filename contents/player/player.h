#pragma once
#include "gameNode.h"

class StageManager;

enum PLAYER_STATE
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

	// 기도(피 충전 모션)
	LEFT_PRAY,
	RIGHT_PRAY,
};

struct tagBullet
{
	Image* Image;
	RECT rc;
	float x, y;
	float fireX, fireY;
	float speed;
	float angle;
	float range;
};

class Player : public GameNode
{
private:
	Image* _image;
	Image* _effect;

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

	PLAYER_STATE _state;
	PLAYER_STATE _stateBefore;

	StageManager* _sm;

	Animation* _ani_idle;
	Animation* _ani_run;
	Animation* _ani_attack;
	Animation* _ani_jump;
	Animation* _ani_throw;
	Animation* _ani_dead;
	Animation* _ani_pray;

	Animation* _ani_effect_charge;

	int _attackCount;
	int _attackCount2;
	int _redAlpha;
	int _alpha;
	int _attackedCount;
	int _prayCount;

	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;
	bool _charge;
	bool _chargeFull;
	
public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setAnimation(PLAYER_STATE state);
	void setStatePray();

	void fireBullet();
	void fireChargeBullet();
	void moveBullet();
	void renderBullet();
	void removeBullet(int index);

	bool checkBulletCollision(MYRECT rc);


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
	void setHP(int hp)
	{
		_hp = hp; 
		if (hp <= 0)
		{
			setAnimation(isLeft() ? LEFT_DEAD : RIGHT_DEAD);
		}
	}

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
	void setIsAttacked(bool isAttacked)
	{ 
		if (isAttacked)
		{
			SOUNDMANAGER->play("맞음");
			_x += isLeft() ? 20 : -20;
			_hitbox.setCenterPos(_x, _probeY - _height / 4);
			_alpha = 170;
		}
		_isAttacked = isAttacked;
	}

	bool getIsCheckAttack() { return _isCheckAttack; }
	void setIsCheckAttack(bool check) 
	{
		if (check)
		{
			isLeft() ? EFFECTMANAGER->play("left hit", _attackRc.left, _y) : EFFECTMANAGER->play("right hit", _attackRc.right, _y);
		}
		_isCheckAttack = check; 
	}

	bool getIsDead() { return _hp <= 0 ? true : false; }

	bool getCanMoveLeft() { return _canMoveLeft; }
	void setCanMoveLeft(bool canMoveLeft) { _canMoveLeft = canMoveLeft; }

	bool getCanMoveRight() { return _canMoveRight; }
	void setCanMoveRight(bool canMoveRight) { _canMoveRight = canMoveRight; }

	void setStageManagerMemoryAddressLink(StageManager* sm) { _sm = sm; }
};

