#pragma once
#include "gameNode.h"
#include <iostream>
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
using namespace std;

class playGround : public gameNode
{
private:
	image* _background;
	image* _backgroundPixel;

	class stageManager* _sm;
	class pixelCollsion* _pixel;
	class player* _player;

	bool _isShaking;
	bool _isGameOver;
	int _shakeCount;

public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
};

