#include "stdafx.h"
#include "commonStage.h"
#include "player.h"

HRESULT commonStage::init(int stageNum)
{
	switch (stageNum)
	{
	case STAGE1:
		_image = IMAGEMANAGER->findImage("���");
		_pixel = IMAGEMANAGER->findImage("��� �ȼ�");
		_backImg = true;
		break;
	case STAGE2:
		_image = IMAGEMANAGER->findImage("���2");
		_pixel = IMAGEMANAGER->findImage("���2 �ȼ�");
		_backImg = true;
		break;
	case STAGE3:
		_image = IMAGEMANAGER->findImage("���3");
		_pixel = IMAGEMANAGER->findImage("���3 �ȼ�");
		_backImg = false;
		break;
	}

	_stageNum = stageNum;


	_loopX1 = 0;
	_loopX2 = 0;
	_loopX3 = 0;
	_loopX4 = 0;

	_loopCount = 0;

	return S_OK;
}

void commonStage::release()
{
}

void commonStage::update()
{
	switch (_stageNum)
	{
	case STAGE1:
		CAMERA->setBackWidth(1920);
		break;
	case STAGE2:
		CAMERA->setBackWidth(1920);
		break;
	case STAGE3:
		CAMERA->setBackWidth(960);
		break;
	}

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
}

void commonStage::render()
{
	if (_backImg)
	{
		IMAGEMANAGER->findImage("��4")->loopRender(getMemDC(), &RectMake(0, 0, WINSIZEX * 2, WINSIZEY), _loopX4, 0);
		IMAGEMANAGER->findImage("��3")->loopRender(getMemDC(), &RectMake(0, 0, WINSIZEX * 2, WINSIZEY), _loopX3, 0);
		IMAGEMANAGER->findImage("��2")->loopRender(getMemDC(), &RectMake(0, 0, WINSIZEX * 2, WINSIZEY), _loopX2, 0);
		IMAGEMANAGER->findImage("��1")->loopRender(getMemDC(), &RectMake(0, 0, WINSIZEX * 2, WINSIZEY), _loopX1, 0);
	}

	_image->render(getMemDC());
}
