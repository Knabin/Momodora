#pragma once
#include "gameNode.h"
#include <iostream>
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
using namespace std;

class PlayGround : public GameNode
{
private:
	Image* _background;
	Image* _backgroundPixel;

	class StageManager* _sm;
	class pixelCollsion* _pixel;
	class Player* _player;

	bool _isShaking;
	bool _isGameOver;
	int _shakeCount;

public:
	PlayGround();
	~PlayGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
};

