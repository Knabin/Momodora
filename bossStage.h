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

	image* _block;
	animation* _ani_block;

	MYRECT _rc;
	MYRECT _rcCol;

	progressBar _pgBar;

	player* _player;
	enemy* _boss;

	bool _isStart;
	// TODO: isDead를 boss 클래스에서 받아오기
	bool _isDead;

public:
	virtual HRESULT init(int bossNum);
	virtual void release();
	virtual void update();
	virtual void render();

	void setPlayerMemoryAddressLink(player *p) { _player = p; }

	enemy* getBossPointer() { return _boss; }
	void setBossPointer(enemy* boss) { _boss = boss; }
};

