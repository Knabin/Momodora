#include "stdafx.h"
#include "effect/effect.h"
#include "animation/animation.h"

Effect::Effect()
	: _effectImage(NULL),
	_effectAnimation(NULL),
	_isRunning(false),
	_x(0), _y(0)
{
}


Effect::~Effect()
{
}

HRESULT Effect::init(Image * effectImage, int frameW, int frameH, int fps, float elapsedTime)
{
	if (!effectImage) return E_FAIL;

	_isRunning = false;
	_effectImage = effectImage;
	_elapsedTime = elapsedTime;

	//이펙트 애니메이션이 없으면 생성해라
	if (!_effectAnimation) _effectAnimation = new Animation;

	_effectAnimation->init(_effectImage->getWidth(), _effectImage->getHeight(),
		frameW, frameH);
	_effectAnimation->setDefPlayFrame(false, false);
	_effectAnimation->setFPS(fps);
	_effectAnimation->stop();

	return S_OK;
}

void Effect::release()
{
	_effectImage = NULL;
	SAFE_DELETE(_effectAnimation);
}

void Effect::update()
{
	//이펙트 애니메이션 실행 변수가 false면 실행하지 마라
	if (!_isRunning) return;

	_effectAnimation->frameUpdate(_elapsedTime);

	if (!_effectAnimation->isPlay()) killEffect();
}

void Effect::render()
{
	if (!_isRunning) return;

	_effectImage->aniRender(getMemDC(), _x, _y, _effectAnimation);
}

void Effect::startEffect(int x, int y)
{
	//이펙트 이미지 또는 이펙트 애니메이션이 없으면 실행하지마라
	if (!_effectImage || !_effectAnimation) return;

	_x = x - (_effectAnimation->getFrameWidth() / 2);
	_y = y - (_effectAnimation->getFrameHeight() / 2);

	_isRunning = true;

	_effectAnimation->start();
}

void Effect::killEffect()
{
	_isRunning = false;
}
