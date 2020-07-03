#pragma once
#include <vector>
#include <list>
#include "gameNode.h"

class player;

class stageManager : public gameNode
{
private:
	player * _player;

	// 0 ~ 2: commonStage, 3 ~ 5: bossStage
	int _currentIdx;

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setPlayerMemoryAddressLink(player *p) { _player = p; }

	int getCurrentIndex() { return _currentIdx; }
	bool getIsBossStage() 
	{
		if (_currentIdx > 2) return true; 
		return false; 
	}
};

