#include "stdafx.h"
#include "bossStage.h"
#include "player.h"
#include "enemy.h"

bossStage::bossStage(int bossNum, const char * fileName)
{
	_image = IMAGEMANAGER->findImage("º¸½º ¹è°æ");
	_block = IMAGEMANAGER->findImage("º¸½º ºí·Ï");

	_ani_block = new animation;
	_ani_block->init(_block->getWidth(),
		_block->getHeight(),
		_block->getFrameWidth(),
		_block->getFrameHeight());
	_ani_block->setFPS(1);
	_ani_block->setDefPlayFrame(false, false);

	_fileName = fileName;
	_bossNum = bossNum;
}

HRESULT bossStage::init()
{
	_rc.set(0, 144, 48, 428);
	_rcCol.set(0, 0, 50, 150);
	_rcCol.setLeftTopPos(0, _rc.bottom);

	_pgBar.init(WINSIZEX / 2 - 343, WINSIZEY - 100, 687, 39);
	_vBoss.clear();
	if(_fileName != nullptr) STAGEENEMYMANAGER->parsingEnemyData(_fileName, _vBoss);
	
	for(int i = 0; i < _vBoss.size(); ++i)
		_vBoss[i]->setPlayerMemoryAddressLink(_player);

	_isStart = _isDead = false;

	switch (_bossNum)
	{
	case BOSS1:
		
		if (!SOUNDMANAGER->isPlaySound("º¸½º1 ¸Ê"))
		{
			SOUNDMANAGER->stopAll("º¸½º1 ¸Ê");
			SOUNDMANAGER->playBGM("º¸½º1 ¸Ê", BGMVOLUME);
		}
		break;
	case BOSS2:
		if (!SOUNDMANAGER->isPlaySound("º¸½º2 ¸Ê"))
		{
			SOUNDMANAGER->stopAll("º¸½º2 ¸Ê");
			SOUNDMANAGER->playBGM("º¸½º2 ¸Ê", BGMVOLUME);
		}
		break;
	case BOSS3:
		if (!SOUNDMANAGER->isPlaySound("º¸½º3 ¸Ê"))
		{
			SOUNDMANAGER->stopAll("º¸½º3 ¸Ê");
			SOUNDMANAGER->playBGM("º¸½º3 ¸Ê", BGMVOLUME);
		}
		break;
	}

	return S_OK;
}



void bossStage::release()
{
	SAFE_DELETE(_vBoss[0]);
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

	if (KEYMANAGER->isOnceKeyDown(VK_LSHIFT))
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
		for (int i = 0; i < _vBoss.size(); ++i)
			_vBoss[i]->update();
	}

	if (_vBoss[0]->getHP() <= 0) _isDead = true;

	_pgBar.setGauge(_vBoss[0]->getHP(), _vBoss[0]->getMaxHP());
	_pgBar.update();
}

void bossStage::render()
{
	_image->render(getMemDC());
	
	if (_isStart && !_isDead)
	{
		_pgBar.render();
		_block->aniRender(getMemDC(), _rc.left, _rc.bottom, _ani_block);
	}

	for (int i = 0; i < _vBoss.size(); ++i)
		_vBoss[i]->render();
}
