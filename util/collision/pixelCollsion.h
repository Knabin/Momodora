#pragma once
#include "gameNode.h"
#include <string>

class StageManager;
class Player;

class pixelCollsion : public GameNode
{
private:
	Image* _pixel;
	string _str[4];

	StageManager* _sm;
	Player* _player;

public:
	pixelCollsion();
	~pixelCollsion();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setStageManagerMemoryAddressLink(StageManager* sm) { _sm = sm; }
	void setPlayerMemoryAddressLink(Player* p) { _player = p; }
};

