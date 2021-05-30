#pragma once
#include "util/timer/timer.h"

class TimeManager
{
public:
	static TimeManager* getInstance()
	{
		static TimeManager instance;
		return &instance;
	}

private:
	TimeManager();
	~TimeManager();

private:
	Timer* _timer;
	bool _debug;

public:
	HRESULT init();
	void release();
	void update(float lockFPS = 0.0f);
	void render(HDC hdc);

	bool getDebug() { return _debug; }
	void setDebug() { _debug = !_debug; }

	inline float getElapsedTime() const { return _timer->getElapsedTime(); }
	inline float getWorldTime() const { return _timer->getWorldTime(); }

};

