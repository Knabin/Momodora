#include "stdafx.h"
#include "commonStage.h"
#include "player.h"
#include "enemy.h"

commonStage::commonStage()
{
}

commonStage::commonStage(int stageNum, const char * stageName)
{
	switch (stageNum)
	{
	case STAGE1:
		_image = IMAGEMANAGER->findImage("¹è°æ");
		_pixel = IMAGEMANAGER->findImage("¹è°æ ÇÈ¼¿");
		_backImg = true;
		break;
	case STAGE2:
		_image = IMAGEMANAGER->findImage("¹è°æ2");
		_pixel = IMAGEMANAGER->findImage("¹è°æ2 ÇÈ¼¿");
		_backImg = true;
		break;
	case STAGE3:
		_image = IMAGEMANAGER->findImage("¹è°æ3");
		_pixel = IMAGEMANAGER->findImage("¹è°æ3 ÇÈ¼¿");
		_backImg = false;
		break;
	}
	_fileName = stageName;
	_stageNum = stageNum;
}

HRESULT commonStage::init()
{
	_vEnemy.clear();
	if (_fileName != nullptr)
		STAGEENEMYMANAGER->parsingEnemyData(_fileName, _vEnemy);

	for (int i = 0; i < _vEnemy.size(); ++i)
		_vEnemy[i]->setPlayerMemoryAddressLink(_player);

	_loopX1 = 0;
	_loopX2 = 0;
	_loopX3 = 0;
	_loopX4 = 0;

	_loopCount = 0;

	switch (_stageNum)
	{
	case STAGE1: case STAGE2:
		CAMERA->setBackWidth(1920);
		if (!SOUNDMANAGER->isPlaySound("ÀÏ¹Ý ¸Ê"))
		{
			SOUNDMANAGER->stopAll("ÀÏ¹Ý ¸Ê");
			SOUNDMANAGER->playBGM("ÀÏ¹Ý ¸Ê", BGMVOLUME);
		}
		break;
	case STAGE3:
		CAMERA->setBackWidth(960);
		SOUNDMANAGER->stopAll("º¸½º Àü ¸Ê");
		if (!SOUNDMANAGER->isPlaySound("º¸½º Àü ¸Ê")) SOUNDMANAGER->playBGM("º¸½º Àü ¸Ê", BGMVOLUME);
		break;
	}

	return S_OK;
}

void commonStage::release()
{
	_vEnemy.clear();
}

void commonStage::update()
{
	if (_backImg)
	{
		++_loopCount;

		if (CAMERA->checkCameraX() && _player->isMovingRight())
		{
			if (_loopCount > 3)
			{
				_loopX4 += 1;
				_loopX3 += 0;
				_loopX2 += 1;
				_loopX1 += 2;
				_loopCount = 0;
			}
		}
		else if (CAMERA->checkCameraX() && _player->isMovingLeft())
		{
			if (_loopCount > 3)
			{
				_loopX4 -= 1;
				_loopX3 -= 0;
				_loopX2 -= 1;
				_loopX1 -= 2;
				_loopCount = 0;
			}
		}
	}

	for (int i = 0; i < _vEnemy.size(); ++i)
		_vEnemy[i]->update();
}

void commonStage::render()
{
	if (_backImg)
	{
		IMAGEMANAGER->findImage("¹é4")->loopRender(getMemDC(), &RectMake(0, 0, WINSIZEX * 2, WINSIZEY), _loopX4, 0);
		IMAGEMANAGER->findImage("¹é3")->loopRender(getMemDC(), &RectMake(0, 0, WINSIZEX * 2, WINSIZEY), _loopX3, 0);
		IMAGEMANAGER->findImage("¹é2")->loopRender(getMemDC(), &RectMake(0, 0, WINSIZEX * 2, WINSIZEY), _loopX2, 0);
		IMAGEMANAGER->findImage("¹é1")->loopRender(getMemDC(), &RectMake(0, 0, WINSIZEX * 2, WINSIZEY), _loopX1, 0);
	}

	_image->render(getMemDC());

	if (TIMEMANAGER->getDebug())
	{
		_pixel->render(getMemDC());
	}

	for (int i = 0; i < _vEnemy.size(); ++i)
		_vEnemy[i]->render();
}
