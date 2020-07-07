#pragma once
#include "progressBar.h"

enum
{
	BOSS1,
	BOSS2,
	BOSS3
};

class player;
class enemy;

class bossStage : public gameNode
{
private:
	image* _image;
	image* _pixel;

	RECT _rc;

	progressBar _pgBar;

	player* _player;
	enemy* _boss;

public:
	virtual HRESULT init(int bossNum);
	virtual void release();
	virtual void update();
	virtual void render();

	void setPlayerMemoryAddressLink(player *p) { _player = p; }

	enemy* getBossPointer() { return _boss; }
	void setBossPointer(enemy* boss) { _boss = boss; }
};

