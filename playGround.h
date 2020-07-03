#pragma once
#include "gameNode.h"

#include "pixelCollsion.h"

#include "player.h"
#include "progressBar.h"

#include "stageManager.h"
#include "bossStage.h"
#include "commonStage.h"

#include <iostream>
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
using namespace std;

struct tagObject
{
	MYRECT rc;
};

class playGround : public gameNode
{
private:
	image* _background;
	image* _backgroundPixel;

	stageManager* _sm;

	pixelCollsion* _pixel;

	player* _player;

	vector<string> _vTest;

	progressBar _pgBar;

	int _playerCount;

	bool _isShaking;
	int _shakeCount;



public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
};

