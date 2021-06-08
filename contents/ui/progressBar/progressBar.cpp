#include "stdafx.h"
#include "progressBar.h"

ProgressBar::ProgressBar(float x, float y, int width, int height)
	: _x(x), _y(y), _rcProgress(RectMake(x, y, width, height))
	, _progressBarTop(IMAGEMANAGER->addImage("frontBar", "resources/image/boss_health.bmp", width, height, true, RGB(255, 0, 255)))
	, _progressBarBottom(IMAGEMANAGER->addImage("backBar", "resources/image/boss_health_back.bmp", width, height, false, RGB(255, 0, 255)))
{
	init();
}


ProgressBar::~ProgressBar()
{
}

HRESULT ProgressBar::init()
{
	_width = _progressBarBottom->getWidth();

	return S_OK;
}

void ProgressBar::release()
{
}

void ProgressBar::update()
{
	_rcProgress = RectMakeCenter(_x, _y,
		_progressBarTop->getWidth(),
		_progressBarBottom->getHeight());
}

void ProgressBar::render()
{
	IMAGEMANAGER->render("backBar", getMemDC(),
		_rcProgress.left + _progressBarBottom->getWidth() / 2,
		_y + _progressBarBottom->getHeight() / 2,
		0, 0,
		_progressBarBottom->getWidth(), _progressBarBottom->getHeight());

	IMAGEMANAGER->render("frontBar", getMemDC(),
		_rcProgress.left + _progressBarBottom->getWidth() / 2,
		_y + _progressBarBottom->getHeight() / 2,
		0, 0,
		_width, _progressBarBottom->getHeight());

}

void ProgressBar::setGauge(float currentGauge, float maxGauge)
{
	_width = (currentGauge / maxGauge) * _progressBarBottom->getWidth();
}
