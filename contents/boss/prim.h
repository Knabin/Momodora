#pragma once
#include "enemy.h"

class Prim : public Enemy
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