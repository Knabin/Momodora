#pragma once
#include "gameNode.h"

class player;

enum
{
	STAGE1,
	STAGE2,
	STAGE3
};

class commonStage : public gameNode
{
private:
	image* _image;
	image* _pixel;

	bool _backImg;

	int _loopCount;

	int _loopX1;
	int _loopX2;
	int _loopX3;
	int _loopX4;

	player* _player;

	int _stageNum;


public:
	// STAGE1 = 0, STAGE2 = 1, STAGE3 = 2
	virtual HRESULT init(int stageNum);
	virtual void release();
	virtual void update();
	virtual void render();

	void setPlayerMemoryAddressLink(player *p) { _player = p; }
};
