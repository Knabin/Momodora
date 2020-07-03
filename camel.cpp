#include "stdafx.h"
#include "camel.h"


camel::camel()
{
}


camel::~camel()
{
}

HRESULT camel::init()
{
	_camel = IMAGEMANAGER->addFrameImage("³«Å¸", "camel.bmp", 300, 267, 4, 3, true, RGB(255, 0, 255));

	//µğÆúÆ®
	_ani1 = new animation;
	_ani1->init(_camel->getWidth(), _camel->getHeight(),
		_camel->getFrameWidth(), _camel->getFrameHeight());
	_ani1->setDefPlayFrame(true, true);
	_ani1->setFPS(1);

	//¹è¿­ Àç»ı
	int arrAni[] = { 3, 5, 1, 7, 3, 4 };

	_ani2 = new animation;
	_ani2->init(_camel->getWidth(), _camel->getHeight(),
		_camel->getFrameWidth(), _camel->getFrameHeight());
	_ani2->setPlayFrame(arrAni, 6, false);
	_ani2->setFPS(10);

	//±¸°£ Àç»ı
	_ani3 = new animation;
	_ani3->init(_camel->getWidth(), _camel->getHeight(),
		_camel->getFrameWidth(), _camel->getFrameHeight());
	_ani3->setPlayFrame(2, 8, false, true);
	_ani3->setFPS(30);

	IMAGEMANAGER->addImage("Æø¹ß", "explosion.bmp", 832, 62, true, RGB(255, 0, 255));

	_effect = new effect;
	_effect->init(IMAGEMANAGER->findImage("Æø¹ß"), 32, 62, 1.0f, 1.0f);

	EFFECTMANAGER->addEffect("explosion", "explosion.bmp", 832, 62, 32, 62, 1.0f, 0.5f, 300);

	return S_OK;
}

void camel::release()
{
}

void camel::update()
{
	if (KEYMANAGER->isOnceKeyDown('Q')) _ani1->start();
	if (KEYMANAGER->isOnceKeyDown('W')) _ani2->start();
	if (KEYMANAGER->isOnceKeyDown('E')) _ani3->start();

	_ani1->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
	_ani2->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
	_ani3->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		_effect->startEffect(_ptMouse.x, _ptMouse.y);
	}

	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{
		EFFECTMANAGER->play("explosion", _ptMouse.x, _ptMouse.y);
	}

	EFFECTMANAGER->play("explosion", RND->getFromIntTo(200, 400), RND->getFromIntTo(200, 400));

	EFFECTMANAGER->update();

	_effect->update();

}

void camel::render()
{
	_camel->aniRender(getMemDC(), 100, 100, _ani1);
	_camel->aniRender(getMemDC(), 300, 100, _ani2);
	_camel->aniRender(getMemDC(), 500, 100, _ani3);

	_effect->render();

	EFFECTMANAGER->render();

}
