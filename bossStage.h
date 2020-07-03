#pragma once
#include "gameNode.h"

class player;

enum
{
	BOSS1,
	BOSS2,
	BOSS3
};

class bossStage : public gameNode
{
private:
	image* _image;
	image* _pixel;

	player* _player;

public:
	virtual HRESULT init(int bossNum);
	virtual void release();
	virtual void update();
	virtual void render();

	void setPlayerMemoryAddressLink(player *p) { _player = p; }
};
