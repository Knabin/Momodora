﻿#include "stdafx.h"
#include "stage/commonStage.h"
#include "player/player.h"
#include "enemy/enemy.h"

CommonStage::CommonStage()
{
}

CommonStage::CommonStage(int stageNum, const char * stageName)
{
	switch (stageNum)
	{
	case STAGE1:
		_image = IMAGEMANAGER->findImage("배경");
		_pixel = IMAGEMANAGER->findImage("배경 픽셀");
		_backImg = true;
		break;
	case STAGE2:
		_image = IMAGEMANAGER->findImage("배경2");
		_pixel = IMAGEMANAGER->findImage("배경2 픽셀");
		_backImg = true;
		break;
	case STAGE3:
		_image = IMAGEMANAGER->findImage("배경3");
		_pixel = IMAGEMANAGER->findImage("배경3 픽셀");
		_backImg = false;
		break;
	}
	_fileName = stageName;
	_stageNum = stageNum;
}

HRESULT CommonStage::init()
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
		if (!SOUNDMANAGER->isPlaySound("일반 맵"))
		{
			SOUNDMANAGER->stopAll("일반 맵");
			SOUNDMANAGER->playBGM("일반 맵", BGMVOLUME);
		}
		break;
	case STAGE3:
		CAMERA->setBackWidth(960);
		SOUNDMANAGER->stopAll("보스 전 맵");
		if (!SOUNDMANAGER->isPlaySound("보스 전 맵")) SOUNDMANAGER->playBGM("보스 전 맵", BGMVOLUME);
		break;
	}

	return S_OK;
}

void CommonStage::release()
{
	for (int i = 0; i < _vEnemy.size(); ++i)
	{
		_vEnemy[i]->release();
		SAFE_DELETE(_vEnemy[i]);
	}
	_vEnemy.clear();
}

void CommonStage::update()
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

void CommonStage::render()
{
	if (_backImg)
	{
		auto four = RectMake(0, 0, WINSIZEX * 2, WINSIZEY);
		auto three = RectMake(0, 0, WINSIZEX * 2, WINSIZEY);
		auto two = RectMake(0, 0, WINSIZEX * 2, WINSIZEY);
		auto one = RectMake(0, 0, WINSIZEX * 2, WINSIZEY);

		IMAGEMANAGER->findImage("백4")->loopRender(getMemDC(), &four, _loopX4, 0);
		IMAGEMANAGER->findImage("백3")->loopRender(getMemDC(), &three, _loopX3, 0);
		IMAGEMANAGER->findImage("백2")->loopRender(getMemDC(), &two, _loopX2, 0);
		IMAGEMANAGER->findImage("백1")->loopRender(getMemDC(), &one, _loopX1, 0);
	}

	_image->render(getMemDC());

	if (TIMEMANAGER->getDebug())
	{
		_pixel->render(getMemDC());
	}

	for (int i = 0; i < _vEnemy.size(); ++i)
		_vEnemy[i]->render();
}
