#include "stdafx.h"
#include "pixelCollsion.h"
#include "player.h"
#include "stageManager.h"


pixelCollsion::pixelCollsion()
{
}


pixelCollsion::~pixelCollsion()
{
}

HRESULT pixelCollsion::init()
{

	_str[0] = "¹è°æ ÇÈ¼¿";
	_str[1] = "¹è°æ2 ÇÈ¼¿";
	_str[2] = "¹è°æ3 ÇÈ¼¿";


	return S_OK;
}

void pixelCollsion::release()
{
}

void pixelCollsion::update()
{
	if (_sm->getIsBossStage()) _pixel = IMAGEMANAGER->findImage(_str[2]);
	else _pixel = IMAGEMANAGER->findImage(_str[_sm->getCurrentIndex()]);

	{
		COLORREF color = GetPixel(_pixel->getMemDC(), _player->getX(), _player->getProbeY());

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if ((r == 0 && g == 255 && b == 255) && !_player->getIsOnGround())
		{
			cout << " ¤Ð¤Ð¤Ð " << endl;
			_player->setY(_player->getProbeY() - _player->getHeight() / 2);
			_player->setIsOnGround(true);
		}
	}

	{
		COLORREF color = GetPixel(_pixel->getMemDC(), _player->isLeft() ? _player->getProbeXL() : _player->getProbeXR(), _player->getY() + _player->getWidth() / 4);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if ((r == 0 && g == 255 && b == 255))
		{
			_player->isLeft() ? _player->setCanMoveLeft(false) : _player->setCanMoveRight(false);
		}
		else
		{
			_player->isLeft() ? _player->setCanMoveLeft(true) : _player->setCanMoveRight(true);
		}
	}

	/*{
		COLORREF color = GetPixel(_pixel->getMemDC(), _player->getX(), _player->getY());

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if ((r == 255 && g == 0 && b == 255))
		{
			if (_x > WINSIZEX / 2)
			{
				if (_sm->moveNextStage()) _x = 100;
			}
			else
				if (_sm->movePrevStage()) _x = _sm->getCurrentWidth() - 100;
		}
	}*/
}

void pixelCollsion::render()
{
	
}
