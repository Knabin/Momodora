#include "stdafx.h"
#include "enemy.h"
#include "player.h"


enemy::enemy()
{
}


enemy::~enemy()
{
}

HRESULT enemy::init(MYPOINT position)
{
	return S_OK;
}

void enemy::release()
{
}

void enemy::update()
{
}

void enemy::render()
{
	draw();
}

void enemy::attack()
{
}

void enemy::checkPlayer()
{
}

void enemy::move()
{

}

void enemy::draw()
{

}

HRESULT oko::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("몬스터1");
	_imageRound = IMAGEMANAGER->findImage("몬스터1 프레임");

	_x = position.x - _image->getWidth();
	_y = position.y - _image->getHeight();

	_rc.set(_x, _y, _x + _image->getWidth(), _y + _image->getHeight());

	_ani_round = new animation;
	_ani_round->init(_imageRound->getWidth(),
		_imageRound->getHeight(),
		_imageRound->getFrameWidth(),
		_imageRound->getFrameHeight());
	_ani_round->setDefPlayFrame(false, true);
	_ani_round->setPlayFrame(0, 15, false, true);
	_ani_round->setFPS(1);
	_ani_round->start();
	return S_OK;
}

void oko::release()
{
	_ani_round->stop();
	SAFE_DELETE(_ani_round);
}

void oko::update()
{
	move();
	draw();

	if(_ani_round->isPlay())
		_ani_round->frameUpdate(TIMEMANAGER->getElapsedTime() * 30);
}

void oko::render()
{
	if (DEBUG)_objectRc.render(getMemDC());
	if(DEBUG) _rc.render(getMemDC());
	_imageRound->aniRender(getMemDC(), _rc.left - _rc.getWidth() + 10, _rc.top - _rc.getHeight() + 10, _ani_round);
	_image->render(getMemDC(), _rc.left, _rc.top);
}

void oko::attack()
{
}

void oko::checkPlayer()
{
}

void oko::move()
{
	
	if (_rc.bottom <= _objectRc.top && _rc.left < _objectRc.right)
	{
		_rc.move(4, 0);
		if (_rc.left > _objectRc.right) _rc.setLeftTopPos(_objectRc.right, _rc.top);
	}
	else if (_rc.top < _objectRc.bottom && _rc.left >= _objectRc.right)
	{
		_rc.move(0, 4);
		if (_rc.top > _objectRc.bottom) _rc.setLeftTopPos(_objectRc.right, _objectRc.bottom);
	}
	else if (_rc.top >= _objectRc.bottom && _rc.right > _objectRc.left)
	{
		_rc.move(-4, 0);
		if (_rc.right < _objectRc.left) _rc.setLeftTopPos(_objectRc.left - _rc.getWidth(), _objectRc.bottom);
	}
	else
	{
		_rc.move(0, -4);
		if (_rc.bottom < _objectRc.top) _rc.setLeftTopPos(_objectRc.left - _rc.getWidth(), _objectRc.top - _rc.getHeight());
	}
	_x = _rc.right - _rc.getWidth() / 2;
	_y = _rc.bottom - _rc.getHeight() / 2;
}

void oko::draw()
{
}

HRESULT monkey::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("몬스터2");

	_x = position.x;
	_y = position.y;
	_width = _image->getFrameWidth();
	_height = _image->getFrameHeight();

	_probeXL = _x + 70;
	_probeXR = _x + _width - 70;
	_probeY = _y + _height / 2;

	_direction = ENEMYDIRECTION::LEFT_IDLE;

	_rc.set(0, 0, 70, 100);
	_rc.setCenterPos(_x + _width / 2 , _y + _height / 2);

	_rcCheck.set(0, 0, 500, 250);
	_rcCheck.setCenterPos(_x + _width / 2, _y + _height / 2);

	_rcAttack.set(0, 0, 30, 80);

	_ani_run = new animation;
	_ani_run->init(_image->getWidth(),
		_image->getHeight(),
		_image->getFrameWidth(),
		_image->getFrameHeight());
	_ani_run->setDefPlayFrame(false, true);
	_ani_run->setFPS(1);
	_ani_run->setPlayFrame(0, 5, false, true);
	_ani_run->start();

	_ani_attack = new animation;
	_ani_attack->init(IMAGEMANAGER->findImage("몬스터2 공격")->getWidth(),
		IMAGEMANAGER->findImage("몬스터2 공격")->getHeight(),
		IMAGEMANAGER->findImage("몬스터2 공격")->getFrameWidth(),
		IMAGEMANAGER->findImage("몬스터2 공격")->getFrameHeight());
	_ani_attack->setDefPlayFrame(false, false);
	_ani_attack->setFPS(1);
	_ani_attack->setPlayFrame(0, 10, false, false);

	_isOnGround = false;
	_canMoveLeft = _canMoveRight = true;

	return S_OK;
}

void monkey::release()
{
	_ani_run->stop();
	_ani_attack->stop();
	SAFE_DELETE(_ani_run);
	SAFE_DELETE(_ani_attack);
}

void monkey::update()
{
	if (_player->getX() <= _x + _width / 2)
	{
		_rcAttack.setLeftTopPos(_x + 40, _y + 20);
	}
	else
	{
		_rcAttack.setLeftTopPos(_x + _width / 2 + 30, _y + 20);
	}
	checkPlayer();
	pixelCollision();
	move();

	switch (_direction)
	{
	case ENEMYDIRECTION::LEFT_IDLE:
		break;
	case ENEMYDIRECTION::RIGHT_IDLE:
		break;
	case ENEMYDIRECTION::LEFT_MOVE:
		break;
	case ENEMYDIRECTION::RIGHT_MOVE:
		break;
	case ENEMYDIRECTION::LEFT_ATTACK:
		if(_ani_attack->isPlay()) _ani_attack->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		break;
	case ENEMYDIRECTION::RIGHT_ATTACK:
		if (_ani_attack->isPlay()) _ani_attack->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		break;
	}

	if (_ani_run->isPlay())
		_ani_run->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
}

void monkey::render()
{
	//_rc.render(getMemDC());
	//_rcAttack.render(getMemDC());
	switch (_direction)
	{
	case ENEMYDIRECTION::LEFT_IDLE:
	case ENEMYDIRECTION::RIGHT_IDLE:
	case ENEMYDIRECTION::LEFT_MOVE:
	case ENEMYDIRECTION::RIGHT_MOVE:
		_image->aniRender(getMemDC(), _x, _y, _ani_run);
		break;
	case ENEMYDIRECTION::LEFT_ATTACK:
	case ENEMYDIRECTION::RIGHT_ATTACK:
		_image->aniRender(getMemDC(), _x, _y, _ani_attack);
		break;
	}
}

void monkey::attack()
{
}

void monkey::checkPlayer()
{
	if (isCollision(_rcAttack, _player->getHitbox()))
	{
		_ani_run->stop();
		if (_player->getX() < _x + _width / 2)
		{
			if (_direction != ENEMYDIRECTION::LEFT_ATTACK)
			{
				_direction = ENEMYDIRECTION::LEFT_ATTACK;
				_image = IMAGEMANAGER->findImage("몬스터2 공격");
				_ani_attack->setPlayFrame(0, 10, false, true);
				_ani_attack->start();
			}
			if (_direction == ENEMYDIRECTION::LEFT_ATTACK)
			{
				attack();
			}
		}
		else
		{
			if (_direction != ENEMYDIRECTION::RIGHT_ATTACK)
			{
				_direction = ENEMYDIRECTION::RIGHT_ATTACK;
				_image = IMAGEMANAGER->findImage("몬스터2 공격");
				_ani_attack->setPlayFrame(11, 21, false, true);
				_ani_attack->start();
			}
			if (_direction == ENEMYDIRECTION::RIGHT_ATTACK)
			{
				attack();
			}
		}
	}
	else if (isCollision(_rcCheck, _player->getHitbox()))
	{
	if (_player->getHitbox().left < _x + _width / 2)
		{
			if (_direction != ENEMYDIRECTION::LEFT_MOVE)
			{
				_direction = ENEMYDIRECTION::LEFT_MOVE;
				_image = IMAGEMANAGER->findImage("몬스터2");
				_ani_run->setPlayFrame(0, 5, false, true);
				_ani_run->start();
			}
		}
		else if (_player->getHitbox().right > _x - _width / 2)
		{
			if (_direction != ENEMYDIRECTION::RIGHT_MOVE)
			{
				_direction = ENEMYDIRECTION::RIGHT_MOVE;
				_image = IMAGEMANAGER->findImage("몬스터2");
				_ani_run->setPlayFrame(6, 11, false, true);
				_ani_run->start();
			}
		}
	}
	else
	{
		_direction = (_direction == ENEMYDIRECTION::LEFT_MOVE || _direction == ENEMYDIRECTION::LEFT_ATTACK || _direction == ENEMYDIRECTION::LEFT_IDLE) ? ENEMYDIRECTION::LEFT_IDLE : ENEMYDIRECTION::RIGHT_IDLE;
	}
}

void monkey::move()
{
	if (_direction == ENEMYDIRECTION::LEFT_MOVE && _canMoveLeft)
	{
		_x -= 3;
	}
	if (_direction == ENEMYDIRECTION::RIGHT_MOVE && _canMoveRight)
	{
		_x += 3;
	}

	if (!_isOnGround)
	{
		_y += 5;
	}

	_probeXL = _x + 70;
	_probeXR = _x + _width - 70;
	_probeY = _y + _height / 2;

	_rc.setCenterPos(_x + _width / 2, _y + _height / 2);
	_rcCheck.setCenterPos(_x + _width / 2, _y + _height / 2);

}

void monkey::draw()
{
}

void monkey::pixelCollision()
{
	int i = 0;
	int dest = 0;

	switch (_direction)	
	{
	case ENEMYDIRECTION::LEFT_IDLE:
	case ENEMYDIRECTION::LEFT_ATTACK:
	case ENEMYDIRECTION::LEFT_MOVE:
		i = _rc.left + 15;
		dest = _x + _width / 2;
		break;
	case ENEMYDIRECTION::RIGHT_IDLE:
	case ENEMYDIRECTION::RIGHT_MOVE:
	case ENEMYDIRECTION::RIGHT_ATTACK:
		i = _x + _width / 2;
		dest = _rc.right - 15;
		break;

	}

	//for (int i = _rc.left; i <= _rc.right; ++i)
	for(; i<= dest; ++i)
	{
		for (int j = _rc.bottom - 5; j < _rc.bottom; ++j)
		{
			COLORREF color = GetPixel(IMAGEMANAGER->findImage("배경2 픽셀")->getMemDC(), i, j);

			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (!_isOnGround && r == 0 && g == 255 && b == 255)
			{
				_y = j - _height;
				_isOnGround = true;
				break;
			}
			else
			{
				_isOnGround = false;
			}
		}
	}

	if (_direction == ENEMYDIRECTION::LEFT_MOVE)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("배경2 픽셀")->getMemDC(), _rc.left, _rc.bottom - 5);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 0 && g == 255 && b == 255)
		{
			//_x = _rc.left;
			_canMoveLeft = false;
		}
		else _canMoveLeft = true;
	}
	else if (_direction == ENEMYDIRECTION::RIGHT_MOVE)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("배경2 픽셀")->getMemDC(), _rc.right, _rc.bottom - 50);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 0 && g == 255 && b == 255)
		{
			//_x = _rc.right - 70;
			_canMoveRight = false;
		}
		else _canMoveRight = true;
	}
}

HRESULT bakman::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("몬스터3 공격");
	_bullet = IMAGEMANAGER->findImage("몬스터3 블록");

	_isFire = false;

	_direction = ENEMYDIRECTION::LEFT_IDLE;

	_x = position.x;
	_y = position.y;

	_width = _image->getFrameWidth();
	_height = _image->getFrameHeight();

	_rc.set(0, 0, 70, 80);
	_rc.setCenterPos(_x + _width / 2, _y + _height / 2);

	_rcCheck.set(0, 0, 600, 300);
	_rcCheck.setCenterPos(_x + _width / 2, _y + _height / 2);

	_ani_attack = new animation;
	_ani_attack->init(IMAGEMANAGER->findImage("몬스터3 공격")->getWidth(),
		IMAGEMANAGER->findImage("몬스터3 공격")->getHeight(),
		IMAGEMANAGER->findImage("몬스터3 공격")->getFrameWidth(),
		IMAGEMANAGER->findImage("몬스터3 공격")->getFrameHeight());
	_ani_attack->setDefPlayFrame(false, false);
	_ani_attack->setFPS(1);

	_ani_attack2 = new animation;
	_ani_attack2->init(IMAGEMANAGER->findImage("몬스터3 공격2")->getWidth(),
		IMAGEMANAGER->findImage("몬스터3 공격2")->getHeight(),
		IMAGEMANAGER->findImage("몬스터3 공격2")->getFrameWidth(),
		IMAGEMANAGER->findImage("몬스터3 공격2")->getFrameHeight());
	_ani_attack->setDefPlayFrame(false, false);
	_ani_attack2->setFPS(1);

	return S_OK;
}

void bakman::release()
{
	_ani_attack->stop();
	SAFE_DELETE(_ani_attack);
}

void bakman::update()
{
	checkPlayer();
	attack();

	if (_ani_attack->isPlay())
		_ani_attack->frameUpdate(TIMEMANAGER->getElapsedTime() * 5);

	if (_ani_attack2->isPlay())
		_ani_attack2->frameUpdate(TIMEMANAGER->getElapsedTime() * 5);

	move();
}

void bakman::render()
{
	draw();
}

void bakman::attack()
{

	if ((_direction == ENEMYDIRECTION::LEFT_ATTACK ||
		_direction == ENEMYDIRECTION::RIGHT_ATTACK) && !_ani_attack->isPlay() && !_isFire)
	{
		_image = IMAGEMANAGER->findImage("몬스터3 공격2");
		if (_direction == ENEMYDIRECTION::LEFT_ATTACK)
		{
			_direction = ENEMYDIRECTION::LEFT_ACTION;
			_ani_attack2->setPlayFrame(0, 5, false, false);

			_bulletX = _x + 20;
			_bulletY = _y;
			_angle = PI * 0.7;
		}
		else
		{
			_direction = ENEMYDIRECTION::RIGHT_ACTION;
			_ani_attack2->setPlayFrame(6, 11, false, false);

			_bulletX = _x + 20;
			_bulletY = _y;
			_angle = PI * 0.3;
		}
		_ani_attack2->start();


		_gravity = 0.08f;
		_isFire = true;
	}
}

void bakman::checkPlayer()
{
	if (isCollision(_rcCheck, _player->getHitbox()))
	{
		if (_direction != ENEMYDIRECTION::LEFT_ATTACK &&
			_direction != ENEMYDIRECTION::RIGHT_ATTACK)
		{
			_image = IMAGEMANAGER->findImage("몬스터3 공격");
			if (_player->getX() < _rc.right - _rc.getWidth() / 2)
			{
				_direction = ENEMYDIRECTION::LEFT_ATTACK;
				_ani_attack->setPlayFrame(0, 5, false, false);
			}
			else
			{
				_direction = ENEMYDIRECTION::RIGHT_ATTACK;
				_ani_attack->setPlayFrame(7, 13, false, false);
			}
			_ani_attack->start();
		}
	}
	else
	{
		_ani_attack->stop();
		_ani_attack2->stop();

		if (_player->getX() < _rc.right - _rc.getWidth() / 2)
		{
			_direction = ENEMYDIRECTION::LEFT_IDLE;
			_image = IMAGEMANAGER->findImage("몬스터3 왼쪽");
		}
		else
		{
			_direction = ENEMYDIRECTION::RIGHT_IDLE;
			_image = IMAGEMANAGER->findImage("몬스터3 오른쪽");
		}
	}
}

void bakman::move()
{
	if (_isFire)
	{
		_bulletX += cosf(_angle) * 7.0f;
		_bulletY += -sinf(_angle) * 7.0f + _gravity;
		_gravity += 0.3f;

		if (_bulletY >= WINSIZEY)
		{
			_bulletX = _x + 20;
			_bulletY = _y;
			_gravity = 0.08f;
			_isFire = false;
		}
	}
}

void bakman::draw()
{
	switch (_direction)
	{
	case ENEMYDIRECTION::LEFT_IDLE:
	case ENEMYDIRECTION::RIGHT_IDLE:
		_image->render(getMemDC(), _x, _y);
		break;
	case ENEMYDIRECTION::LEFT_ATTACK:
	case ENEMYDIRECTION::RIGHT_ATTACK:
		_image->aniRender(getMemDC(), _x, _y, _ani_attack);
		break;
	case ENEMYDIRECTION::LEFT_ACTION:
	case ENEMYDIRECTION::RIGHT_ACTION:
		_image->aniRender(getMemDC(), _x, _y, _ani_attack2);
		break;
	}

	if (_isFire)
	{
		_bullet->render(getMemDC(), _bulletX, _bulletY);
	}
}


HRESULT prim::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("보스1 왼쪽");
	_x = position.x;
	_y = position.y;

	return S_OK;
}

void prim::release()
{
}

void prim::update()
{
}

void prim::render()
{
	_image->render(getMemDC(), _x, _y);
}

void prim::attack()
{
}

void prim::checkPlayer()
{
}

void prim::move()
{
}

void prim::draw()
{
}

HRESULT witch::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("보스2 왼쪽");
	_imageRight = IMAGEMANAGER->findImage("보스2 오른쪽");
	_x = position.x;
	_y = _yRight = position.y;
	_xRight = _x + 100;

	_ani_idle_left = new animation;
	_ani_idle_left->init(_image->getWidth(), _image->getHeight(),
		_image->getFrameWidth(), _image->getFrameHeight());
	_ani_idle_left->setFPS(1);
	_ani_idle_left->setDefPlayFrame(false, true);
	_ani_idle_left->setPlayFrame(0, 4, false, true);
	_ani_idle_left->start();

	_ani_idle_right = new animation;
	_ani_idle_right->init(_imageRight->getWidth(), _imageRight->getHeight(),
		_imageRight->getFrameWidth(), _imageRight->getFrameHeight());
	_ani_idle_right->setFPS(1);
	_ani_idle_right->setDefPlayFrame(false, true);
	_ani_idle_right->setPlayFrame(0, 4, false, true);
	_ani_idle_right->start();

	return S_OK;
}

void witch::release()
{
}

void witch::update()
{
	_ani_idle_left->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
	_ani_idle_right->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
}

void witch::render()
{
	_image->aniRender(getMemDC(), _x, _y, _ani_idle_left);
	_imageRight->aniRender(getMemDC(), _xRight, _yRight, _ani_idle_right);
}

void witch::attack()
{
}

void witch::checkPlayer()
{
}

void witch::move()
{
}

void witch::draw()
{
}


HRESULT rell::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("보스3 등장");
	_ani_appear = new animation;
	_ani_appear->init(_image->getWidth(), _image->getHeight(),
		_image->getFrameWidth(), _image->getFrameHeight());
	_ani_appear->setFPS(1);
	_ani_appear->setDefPlayFrame(0, 1);
	_ani_appear->setPlayFrame(0, 5, false, true);
	_ani_appear->start();

	_x = position.x;
	_y = position.y;

	return S_OK;
}

void rell::release()
{
}

void rell::update()
{
	_ani_appear->frameUpdate(TIMEMANAGER->getElapsedTime() * 8);
}

void rell::render()
{
//	_image->render(getMemDC(), _x, _y);
	_image->aniRender(getMemDC(), _x, _y, _ani_appear);
}

void rell::attack()
{
}

void rell::checkPlayer()
{
}

void rell::move()
{
}

void rell::draw()
{
}
