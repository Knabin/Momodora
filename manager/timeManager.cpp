#include "stdafx.h"
#include "timeManager.h"


TimeManager::TimeManager()
	: _timer(NULL)
{
}


TimeManager::~TimeManager()
{
}

HRESULT TimeManager::init()
{
	_timer = new Timer;
	_timer->init();

	return S_OK;
}

void TimeManager::release()
{
	if (_timer != NULL) SAFE_DELETE(_timer);
}

void TimeManager::update(float lockFPS)
{
	if (_timer != NULL)
	{
		_timer->tick(lockFPS);
	}

}

void TimeManager::render(HDC hdc)
{
	char str[256];
	string frameRate;

	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);


#ifdef _DEBUG
	{
		sprintf_s(str, "framePerSecond(FPS) : %d", _timer->getFrameRate());
		TextOut(hdc, CAMERA->getLeft(), 0, str, strlen(str));

		sprintf_s(str, "worldTime : %f", _timer->getWorldTime());
		TextOut(hdc, CAMERA->getLeft(), 20, str, strlen(str));

		sprintf_s(str, "elapsedTime : %f", _timer->getElapsedTime());
		TextOut(hdc, CAMERA->getLeft(), 40, str, strlen(str));

	}
#else
	{
		sprintf_s(str, "framePerSecond(FPS) : %d", _timer->getFrameRate());
		TextOut(hdc, CAMERA->getLeft(), 0, str, strlen(str));
	}
#endif

}
