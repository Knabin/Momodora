#include "stdafx.h"
#include "bossStage.h"
#include "player.h"
#include "enemy.h"

HRESULT bossStage::init(int bossNum)
{

	_image = IMAGEMANAGER->findImage("보스 배경");

	_block = IMAGEMANAGER->findImage("보스 블록");

	_ani_block = new animation;
	_ani_block->init(_block->getWidth(),
		_block->getHeight(),
		_block->getFrameWidth(),
		_block->getFrameHeight());
	_ani_block->setFPS(1);
	_ani_block->setDefPlayFrame(false, false);
	
	switch (bossNum)
	{
	case BOSS1:
		break;
	case BOSS2:
		break;
	case BOSS3:
		break;
	}

	_rc.set(0, 144, 48, 428);
	_rcCol.set(0, 0, 50, 150);
	_rcCol.setLeftTopPos(0, _rc.bottom);

	_pgBar.init(WINSIZEX / 2 - 343, WINSIZEY - 100, 687, 39);

	_isStart = _isDead = false;
	


	return S_OK;
}

void bossStage::release()
{
	_isStart = false;
	_isDead = false;
}

void bossStage::update()
{
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && !_isStart)
	{
		_isStart = true;
		_ani_block->setPlayFrame(0, 23, false);
		_ani_block->start();
		CAMERA->setBackWidth(960);
	}

	if (_ani_block->isPlay())
	{
		_ani_block->frameUpdate(TIMEMANAGER->getElapsedTime() * 30);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		_isStart = false;
		_isDead = true;
	}

	if (checkPointInRect(_rcCol, _player->getProbeXL(), _player->getY()) && _isStart && !_isDead)
	{
		_player->setCanMoveLeft(false);
	}

	if (_isStart)
	{
		_boss->update();
	}
	_pgBar.update();
}

void bossStage::render()
{
	_image->render(getMemDC());
	
	if (_isStart)
	{
		_pgBar.render();
		_block->aniRender(getMemDC(), _rc.left, _rc.bottom, _ani_block);
	}

	_boss->render();
}
