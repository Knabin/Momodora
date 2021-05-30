#include "stdafx.h"
#include "monkey.h"
#include "player/player.h"

HRESULT Monkey::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("몬스터2");

	_x = position.x;
	_y = position.y;
	_width = _image->getFrameWidth();
	_height = _image->getFrameHeight();

	_probeXL = _x + 70;
	_probeXR = _x + _width - 70;
	_probeY = _y + _height / 2;

	_direction = ENEMY_DIRECTION::LEFT_IDLE;

	_rc.set(0, 0, 70, 100);
	_rc.setCenterPos(_x + _width / 2, _y + _height / 2);

	_rcHit.set(0, 0, 50, 50);

	_rcAttack.set(0, 0, 20, 40);

	_ani_run = new Animation;
	_ani_run->init(_image->getWidth(),
		_image->getHeight(),
		_image->getFrameWidth(),
		_image->getFrameHeight());
	_ani_run->setDefPlayFrame(false, true);
	_ani_run->setFPS(1);
	_ani_run->setPlayFrame(0, 5, false, true);
	_ani_run->start();

	_ani_attack = new Animation;
	_ani_attack->init(IMAGEMANAGER->findImage("몬스터2 공격")->getWidth(),
		IMAGEMANAGER->findImage("몬스터2 공격")->getHeight(),
		IMAGEMANAGER->findImage("몬스터2 공격")->getFrameWidth(),
		IMAGEMANAGER->findImage("몬스터2 공격")->getFrameHeight());
	_ani_attack->setDefPlayFrame(false, false);
	_ani_attack->setFPS(1);
	_ani_attack->setPlayFrame(0, 10, false, false);

	_isOnGround = false;
	_canMoveLeft = _canMoveRight = true;

	_hp = 3;

	return S_OK;
}

void Monkey::release()
{
	_ani_run->stop();
	_ani_attack->stop();
	SAFE_DELETE(_ani_run);
	SAFE_DELETE(_ani_attack);
}

void Monkey::update()
{
	if (_hp <= 0) return;
	if (_player->getX() <= _x + _width / 2)
	{
		_rcAttack.setLeftTopPos(_x + 60, _y + 60);
	}
	else
	{
		_rcAttack.setLeftTopPos(_x + _width / 2 + 20, _y + 60);
	}
	_rcHit.setCenterPos(_x + _width / 2, _y + _height / 2 + 25);
	checkPlayer();
	pixelCollision();
	move();
	checkCollision();


	switch (_direction)
	{
	case ENEMY_DIRECTION::LEFT_IDLE:
	case ENEMY_DIRECTION::RIGHT_IDLE:
	case ENEMY_DIRECTION::LEFT_MOVE:
	case ENEMY_DIRECTION::RIGHT_MOVE:
		if (_ani_run->isPlay()) _ani_run->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		break;
	case ENEMY_DIRECTION::LEFT_ATTACK:
	case ENEMY_DIRECTION::RIGHT_ATTACK:
		if (_ani_attack->isPlay()) _ani_attack->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		break;
	}

}

void Monkey::render()
{
	draw();
}

void Monkey::attack()
{
}

void Monkey::checkPlayer()
{
	if (isCollision(_rcAttack, _player->getHitbox()))
	{
		_ani_run->stop();
		if (_player->getX() < _x + 50)
		{
			if (_direction != ENEMY_DIRECTION::LEFT_ATTACK)
			{
				_direction = ENEMY_DIRECTION::LEFT_ATTACK;
				_image = IMAGEMANAGER->findImage("몬스터2 공격");
				_ani_attack->setPlayFrame(0, 10, false, true);
				_ani_attack->start();
				SOUNDMANAGER->play("monkey", 1.0f);
			}
			if (_direction == ENEMY_DIRECTION::LEFT_ATTACK)
			{
				attack();
			}
		}
		else
		{
			if (_direction != ENEMY_DIRECTION::RIGHT_ATTACK)
			{
				_direction = ENEMY_DIRECTION::RIGHT_ATTACK;
				_image = IMAGEMANAGER->findImage("몬스터2 공격");
				_ani_attack->setPlayFrame(11, 21, false, true);
				_ani_attack->start();
				SOUNDMANAGER->play("monkey", 1.0f);
			}
			if (_direction == ENEMY_DIRECTION::RIGHT_ATTACK)
			{
				attack();
			}
		}
	}
	else if (getDistance(_x, _y, _player->getX(), _player->getY()) <= 200)
	{
		if (_player->getHitbox().right < _x)
		{
			if (_direction != ENEMY_DIRECTION::LEFT_MOVE)
			{
				_direction = ENEMY_DIRECTION::LEFT_MOVE;
				_image = IMAGEMANAGER->findImage("몬스터2");
				_ani_run->setPlayFrame(0, 5, false, true);
				_ani_run->start();
			}
		}
		else if (_player->getHitbox().left > _x + _width)
		{
			if (_direction != ENEMY_DIRECTION::RIGHT_MOVE)
			{
				_direction = ENEMY_DIRECTION::RIGHT_MOVE;
				_image = IMAGEMANAGER->findImage("몬스터2");
				_ani_run->setPlayFrame(6, 11, false, true);
				_ani_run->start();
			}
		}
	}
	else
	{
		_ani_attack->stop();
		_direction = (_direction == ENEMY_DIRECTION::LEFT_MOVE || _direction == ENEMY_DIRECTION::LEFT_ATTACK || _direction == ENEMY_DIRECTION::LEFT_IDLE) ? ENEMY_DIRECTION::LEFT_IDLE : ENEMY_DIRECTION::RIGHT_IDLE;
	}
}

void Monkey::move()
{
	if (_direction == ENEMY_DIRECTION::LEFT_MOVE && _canMoveLeft)
	{
		_x -= 3;
	}
	if (_direction == ENEMY_DIRECTION::RIGHT_MOVE && _canMoveRight)
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
}

void Monkey::draw()
{
	if (_hp <= 0) return;
	if (TIMEMANAGER->getDebug())
	{
		_rcHit.render(getMemDC());
		_rcAttack.render(getMemDC());
	}
	switch (_direction)
	{
	case ENEMY_DIRECTION::LEFT_IDLE:
	case ENEMY_DIRECTION::RIGHT_IDLE:
	case ENEMY_DIRECTION::LEFT_MOVE:
	case ENEMY_DIRECTION::RIGHT_MOVE:
		_image->aniRender(getMemDC(), _x, _y, _ani_run);
		break;
	case ENEMY_DIRECTION::LEFT_ATTACK:
	case ENEMY_DIRECTION::RIGHT_ATTACK:
		_image->aniRender(getMemDC(), _x, _y, _ani_attack);
		break;
	}
}

void Monkey::checkCollision()
{
	// 플레이어를 공격함
	// monkey 본체 충돌처리
	// monkey 공격 rect와 충돌처리
	if (!_player->getIsAttacked())
	{
		if (isCollision(_player->getHitbox(), _rcAttack) || isCollision(_player->getHitbox(), _rcHit))
		{
			_player->setIsAttacked(true);
			_player->setHP(_player->getHP() - 1);
		}
	}

	// 플레이어에게 공격당함
	// 플레이어 공격 범위에 들어오면 하나 까임
	if (_player->isAttacking() && !_player->getIsCheckAttack() && isCollision(_player->getAttackRc(), _rc))
	{
		_player->setIsCheckAttack(true);
		_hp -= 1;
	}

	if (_player->checkBulletCollision(_rc))
	{
		_hp -= 1;
	}
}


void Monkey::pixelCollision()
{
	int i = 0;
	int dest = 0;

	switch (_direction)
	{
	case ENEMY_DIRECTION::LEFT_IDLE:
	case ENEMY_DIRECTION::LEFT_ATTACK:
	case ENEMY_DIRECTION::LEFT_MOVE:
		i = _rc.left + 15;
		dest = _x + _width / 2;
		break;
	case ENEMY_DIRECTION::RIGHT_IDLE:
	case ENEMY_DIRECTION::RIGHT_MOVE:
	case ENEMY_DIRECTION::RIGHT_ATTACK:
		i = _x + _width / 2;
		dest = _rc.right - 15;
		break;

	}

	//for (int i = _rc.left; i <= _rc.right; ++i)
	for (; i <= dest; ++i)
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

	if (_direction == ENEMY_DIRECTION::LEFT_MOVE)
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
	else if (_direction == ENEMY_DIRECTION::RIGHT_MOVE)
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
