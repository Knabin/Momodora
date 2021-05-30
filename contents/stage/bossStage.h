#pragma once
#include "ui/progressBar/progressBar.h"

enum
{
	BOSS1,
	BOSS2,
	BOSS3
};

class Player;
class Enemy;

class BossStage : public GameNode
{
private:
	Image* _image;

	Image* _block;
	Animation* _ani_block;

	MYRECT _rc;
	MYRECT _rcCol;

	ProgressBar _pgBar;

	Player* _player;
	
	int _bossNum;

	const char * _fileName;
	vector<Enemy*> _vBoss;

	bool _isStart;
	bool _isDead;

public:
	BossStage(int bossNum, const char * fileName);
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setPlayerMemoryAddressLink(Player *p) { _player = p; }

	/*Enemy* getBossPointer() { return _boss; }
	void setBossPointer(Enemy* boss) { _boss = boss; }*/
};

