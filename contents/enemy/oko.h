#pragma once
#include "enemy.h"

class Oko : public Enemy
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