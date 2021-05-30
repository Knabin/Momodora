#pragma once
#include "enemy.h"

class Witch : public Enemy
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