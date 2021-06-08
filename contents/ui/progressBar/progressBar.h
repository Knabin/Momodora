#pragma once
#include "gameNode.h"

class ProgressBar : public GameNode
{
private:
	RECT _rcProgress;
	float _x, _y;

	float _width;	//100%

	Image* _progressBarTop;
	Image* _progressBarBottom;

public:
	ProgressBar(float x, float y, int width, int height);
	~ProgressBar();

	HRESULT init();
	void release();
	void update();
	void render();

	void setGauge(float currentGauge, float maxGauge);

	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
};

