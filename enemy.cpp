#include "stdafx.h"
#include "enemy.h"
#include "player.h"


enemy::enemy()
{
}


enemy::~enemy()
{
}

HRESULT enemy::init()
{
	return S_OK;
}

HRESULT enemy::init(MYPOINT position)
{
	return E_NOTIMPL;
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

HRESULT oko::init()
{
	_image = IMAGEMANAGER->findImage("몬스터1");
	_imageRound = IMAGEMANAGER->findImage("몬스터1 프레임");

	_ani_round = new animation;

	return S_OK;
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

HRESULT monkey::init()
{
	_image = IMAGEMANAGER->findImage("몬스터2");

	_rc.set(_x, _y, _x + _image->getWidth(), _y + _image->getHeight());

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
	_ani_attack->setDefPlayFrame(false, true);
	_ani_attack->setFPS(1);
	return S_OK;
}

HRESULT monkey::init(MYPOINT position)
{


	return S_OK;
}

void monkey::release()
{
}

void monkey::update()
{
	if (_ani_run->isPlay())
		_ani_run->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
}

void monkey::render()
{
	_image->aniRender(getMemDC(), 100, 100, _ani_run);
}

void monkey::attack()
{
}

void monkey::checkPlayer()
{
}

void monkey::move()
{
}

void monkey::draw()
{
}

HRESULT bakman::init()
{
	//_image = IMAGEMANAGER->findImage("몬스터3 왼쪽");


	return S_OK;
}

HRESULT bakman::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("몬스터3 공격");
	_bullet = IMAGEMANAGER->findImage("몬스터3 블록");
	_angle = PI * 0.7;
	_gravity = 0.08f;
	_isFire = false;

	_x = position.x;
	_y = position.y;

	_rcCheck.set(0, 0, 600, 300);
	_rcCheck.setCenterPos(_x, _y);

	_ani_attack = new animation;
	_ani_attack->init(IMAGEMANAGER->findImage("몬스터3 공격")->getWidth(),
		IMAGEMANAGER->findImage("몬스터3 공격")->getHeight(),
		IMAGEMANAGER->findImage("몬스터3 공격")->getFrameWidth(),
		IMAGEMANAGER->findImage("몬스터3 공격")->getFrameHeight());
	_ani_attack->setDefPlayFrame(false, true);
	_ani_attack->setFPS(1);
	_ani_attack->setPlayFrame(0, 12, false, true);
	_ani_attack->start();

	return S_OK;
}

void bakman::release()
{
}

void bakman::update()
{
	if (isCollision(_rcCheck, _player->getHitbox()))
	{
		if (!_ani_attack->isPlay()) 
		{
			_image = IMAGEMANAGER->findImage("몬스터3 공격");
			_ani_attack->start();
		}
		_ani_attack->frameUpdate(TIMEMANAGER->getElapsedTime() * 8);
		_count++;
		if (_count >= 45 && !_isFire)
		{
			_isFire = true;
			_bulletX = _x + 20;
			_bulletY = _y;
			_gravity = 0.08f;
		}
	}
	else
	{
		_ani_attack->stop();
		_image = IMAGEMANAGER->findImage("몬스터3 왼쪽");
		_count = 0;
	}

	if (_isFire)
	{
		_bulletX += cosf(_angle) * 7.0f;
		_bulletY += -sinf(_angle) * 7.0f + _gravity;
		_gravity += 0.3f;

		if (_bulletY >= WINSIZEY)
		{
			_count = 23;
			_bulletX = _x + 20;
			_bulletY = _y;
			_gravity = 0.08f;
			_isFire = 0;
		}
	}
}

void bakman::render()
{
	//_rcCheck.render(getMemDC());
	if(_ani_attack->isPlay())
		_image->aniRender(getMemDC(), _x, _y, _ani_attack);
	else 
		_image->render(getMemDC(), _x, _y);
	if (_isFire)
	{
		_bullet->render(getMemDC(), _bulletX, _bulletY);
	}
}

void bakman::attack()
{
}

void bakman::checkPlayer()
{
}

void bakman::move()
{
}

void bakman::draw()
{
}
