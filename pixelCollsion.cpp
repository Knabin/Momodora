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

	for (int i = _player->getProbeY() - 20; i <= _player->getProbeY(); i++)
	{
		for (int j = _player->getProbeXL() + 10; j <= _player->getProbeXR() - 10; j++)
		{
			COLORREF color = GetPixel(_pixel->getMemDC(), j, i);

			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if ((r == 0 && g == 255 && b == 255))
			{
				if (!_player->getIsOnGround() && !_player->isJumping())
				{
					_player->setY(i - _player->getHeight() / 2);
					_player->setIsOnGround(true);
					break;
				}
			}
			else
			{
				if (!(_player->isFalling() || _player->isJumping())) _player->setIsOnGround(false);
			}
		}
	}

	for (int i = _player->getProbeYT(); i <= _player->getProbeYT() + 5; ++i)
	{
		for (int j = _player->getProbeXL() + 10; j <= _player->getProbeXR() - 10; j++)
		{
			COLORREF color = GetPixel(_pixel->getMemDC(), j, i);

			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (r == 0 && g == 255 && b == 255)
			{
				_player->setY(i);
				_player->setAnimation(_player->isLeft() ? LEFT_FALL : RIGHT_FALL);
				_player->setGravity(_player->getSpeedY() + 1);
				_player->setAngle(PI / 2);
				break;
			}
		}
	}

	for (int i = _player->getProbeY() - _player->getHeight() / 2; i < _player->getProbeY(); ++i)
	{
		COLORREF color = GetPixel(_pixel->getMemDC(), _player->isLeft() ? _player->getProbeXL() : _player->getProbeXR(), _player->getY() + _player->getWidth() / 4);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if ((r == 0 && g == 255 && b == 255))
		{
			_player->isLeft() ? _player->setCanMoveLeft(false) : _player->setCanMoveRight(false);
			break;
		}
		else if (r == 255 && g == 0 && b == 255)
		{
			if (_sm->getIsBossStage())
			{
				_player->setPointLeftStart();
				_player->isLeft() ? _sm->movePrevStage() : _sm->moveNextStage();
			}
			else
			{
				_player->isLeft() ? _sm->movePrevStage() : _sm->moveNextStage();
				_player->isLeft() ? _player->setPointRightStart() : _player->setPointLeftStart();
			}
			
			break;
		}
		else
		{
			_player->setCanMoveLeft(true);
			_player->setCanMoveRight(true);
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
