#pragma once
#include "enemy.h"

class Monkey : public Enemy
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