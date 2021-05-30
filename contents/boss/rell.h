#pragma once
#include "enemy.h"

class Rell : public Enemy
{
	enum class RELL_STATE
	{
		LEFT_IDLE,
		RIGHT_IDLE,

		LEFT_GROUND,
		RIGHT_GROUND,

		LEFT_ATTACK,
		RIGHT_ATTACK,

		PRAY,	// 1
		AIR,	// 2, 떠 있는 상태
		DEAD,
	};

	enum class RELL_MODE
	{
		MODE1,
		MODE2,
		MODE3,
	};

	Image* _image;

	Image* _bullet;
	Image* _fire;
	Image* _snow;
	Image* _snow2;

	// 1페이즈
	Animation* _ani_idle1;
	Animation* _ani_ground1;
	Animation* _ani_attack1;
	Animation* _ani_pray;
	Animation* _ani_effect_fire;

	//2
	Animation* _ani_attack2;
	Animation* _ani_snow;

	//3
	Animation* _ani_idle3;
	Animation* _ani_ground3;
	Animation* _ani_attack3;

	RELL_STATE _state;
	RELL_MODE _mode;

	tagBulletEnemy _bullet1[3];
	tagBulletEnemy _bullet2[16];
	tagBulletEnemy _ground[3];

	MYRECT _rcPray;

	bool _isStart;
	bool _attackVer;
	bool _shootCount;
	bool _canPray;

	int _attackCount;
	int _prayCount;
	int _groundCount;

	float _yUp;	// 2페이즈용
	float _xLarge, _yLarge;	// 3페이즈용

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
	bool changeDirectionToLeft();
	void start();
	void changeMode();
};

