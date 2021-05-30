#include "stdafx.h"
#include "oko.h"
#include "player/player.h"

HRESULT Oko::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("몬스터1");
	_imageRound = IMAGEMANAGER->findImage("몬스터1 프레임");

	_x = position.x - _image->getWidth();
	_y = position.y - _image->getHeight();

	_rc.set(_x, _y, _x + _image->getWidth(), _y + _image->getHeight());

	_ani_round = new Animation;
	_ani_round->init(_imageRound->getWidth(),
		_imageRound->getHeight(),
		_imageRound->getFrameWidth(),
		_imageRound->getFrameHeight());
	_ani_round->setDefPlayFrame(false, true);
	_ani_round->setPlayFrame(0, 15, false, true);
	_ani_round->setFPS(1);
	_ani_round->start();

	_isOnceAttacked = false;

	_hp = 2;
	return S_OK;
}

void Oko::release()
{
	_ani_round->stop();
	SAFE_DELETE(_ani_round);
}

void Oko::update()
{
	if (_hp <= 0) return;
	move();
	draw();
	checkCollision();

	if (_ani_round->isPlay())
		_ani_round->frameUpdate(TIMEMANAGER->getElapsedTime() * 30);
}

void Oko::render()
{
	if (_hp <= 0) return;
	if (TIMEMANAGER->getDebug()) _rc.render(getMemDC());
	_imageRound->aniRender(getMemDC(), _rc.left - _rc.getWidth() + 10, _rc.top - _rc.getHeight() + 10, _ani_round);
	if (!_isOnceAttacked) _image->render(getMemDC(), _rc.left, _rc.top);
}

void Oko::attack()
{
}

void Oko::checkPlayer()
{
}

void Oko::move()
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

void Oko::draw()
{
}

void Oko::checkCollision()
{
	// 플레이어를 공격함
	// oko는 본체 충돌처리만 진행
	if (!_player->getIsAttacked() && isCollision(_player->getHitbox(), _rc))
	{
		_player->setIsAttacked(true);
		_player->setHP(_player->getHP() - 1);
	}

	// 플레이어에게 공격당함
	// 플레이어 공격 범위에 들어오면 하나 까이고

	if (_player->isAttacking() && !_player->getIsCheckAttack() && isCollision(_player->getAttackRc(), _rc))
	{
		_player->setIsCheckAttack(true);
		_hp -= 1;
		if (_hp == 1) _isOnceAttacked = true;
	}

	if (_player->checkBulletCollision(_rc))
	{
		_hp -= 1;
		cout << "원거리 공격, " << _hp << endl;
		if (_hp == 1)	_isOnceAttacked = true;
	}
}