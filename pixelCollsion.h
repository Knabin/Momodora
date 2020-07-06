#pragma once
#include "gameNode.h"
#include <string>

class stageManager;
class player;

class pixelCollsion : public gameNode
{
private:
	image* _pixel;
	string _str[4];

	stageManager* _sm;
	player* _player;

public:
	pixelCollsion();
	~pixelCollsion();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setStageManagerMemoryAddressLink(stageManager* sm) { _sm = sm; }
	void setPlayerMemoryAddressLink(player* p) { _player = p; }
};

