#include "stdafx.h"
#include "bossStage.h"
#include "player.h"
#include "enemy.h"

HRESULT bossStage::init(int bossNum)
{

	_image = IMAGEMANAGER->findImage("보스 배경");
	_pixel = IMAGEMANAGER->findImage("배경3 픽셀 보스");

	_block = IMAGEMANAGER->findImage("보스 블록");

	_ani_block = new animation;
	_ani_block->init(_block->getWidth(),
		_block->getHeight(),
		_block->getFrameWidth(),
		_block->getFrameHeight());
	
	switch (bossNum)
	{
	case BOSS1:
		break;
	case BOSS2:
		break;
	case BOSS3:
		break;
	}

	_rc = RectMake(0, 428, 48, 144);
	

	_pgBar.init(WINSIZEX / 2 - 343, WINSIZEY - 100, 687, 39);

	return S_OK;
}

void bossStage::release()
{
}

void bossStage::update()
{
	CAMERA->setBackWidth(960);
	_pgBar.update();

	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) && !_isStart)
	{
		_isStart = true;
	}

	if (KEYMANAGER->isToggleKey(VK_TAB))
	{

	}

	_boss->update();
}

void bossStage::render()
{
	_image->render(getMemDC());
	
		_pgBar.render();
		IMAGEMANAGER->findImage("보스 블록")->render(getMemDC(), _rc.left, _rc.top);
	if (_isStart)
	{
	}

	_boss->render();
}
