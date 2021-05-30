#pragma once
#include "gameNode.h"

class Animation;

class Effect : public GameNode
{
protected:
	int _x;
	int _y;

	Image*		_effectImage;
	Animation*	_effectAnimation;
	BOOL		_isRunning;
	float		_elapsedTime;

public:
	Effect();
	~Effect();

	virtual HRESULT init(Image* effectImage, int frameW, int frameH, int fps, float elapsedTime);
	virtual void release();
	virtual void update();
	virtual void render();

	void startEffect(int x, int y);

	virtual void killEffect();

	BOOL getIsRunning() { return _isRunning; }
};

