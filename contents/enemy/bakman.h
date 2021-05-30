#pragma once
#include "enemy.h"

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

