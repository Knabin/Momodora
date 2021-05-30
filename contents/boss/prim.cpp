#include "stdafx.h"
#include "prim.h"
#include "player/player.h"

HRESULT Prim::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("보스1 시작");

	_ani_start = new Animation;
	_ani_start->init(_image->getWidth(), _image->getHeight(),
		_image->getFrameWidth(), _image->getFrameHeight());
	_ani_start->setFPS(1);
	_ani_start->setDefPlayFrame(false, false);

	_ani_attack = new Animation;
	_ani_attack->init(IMAGEMANAGER->findImage("보스1 공격")->getWidth(), IMAGEMANAGER->findImage("보스1 공격")->getHeight(),
		_image->getFrameWidth(), _image->getFrameHeight());
	_ani_attack->setFPS(1);
	_ani_attack->setDefPlayFrame(false, false);

	_ani_run = new Animation;
	_ani_run->init(IMAGEMANAGER->findImage("보스1 이동")->getWidth(), IMAGEMANAGER->findImage("보스1 이동")->getHeight(),
		_image->getFrameWidth(), _image->getFrameHeight());
	_ani_run->setFPS(1);
	_ani_run->setDefPlayFrame(false, false);

	_ani_angry = new Animation;
	_ani_angry->init(IMAGEMANAGER->findImage("보스1 화남")->getWidth(), IMAGEMANAGER->findImage("보스1 화남")->getHeight(),
		_image->getFrameWidth(), _image->getFrameHeight());
	_ani_angry->setFPS(1);
	_ani_angry->setDefPlayFrame(false, false);

	_rc.set(0, 0, 60, 45);
	_rcAttack.set(0, 0, 140, 60);

	_x = position.x;
	_y = position.y;

	_maxHp = 30;
	_hp = _maxHp;

	_isStart = false;

	return S_OK;
}

void Prim::release()
{
}

void Prim::update()
{
	if (_hp <= 0) return;
	start();
	move();
	checkCollision();
	draw();

	_rc.setCenterPos(_x + _image->getFrameWidth() / 2, _y + _image->getFrameHeight() - 25);
}

void Prim::render()
{
	if (TIMEMANAGER->getDebug())
	{
		_rc.render(getMemDC());
		_rcAttack.render(getMemDC());
	}

	if (_hp <= 0)
	{
		IMAGEMANAGER->findImage("보스1 패배")->render(getMemDC(), _x, _y);
	}
	else
	{
		switch (_direction)
		{
		case ENEMY_DIRECTION::LEFT_IDLE:
		case ENEMY_DIRECTION::RIGHT_IDLE:
			_image->aniRender(getMemDC(), _x, _y, _ani_start);
			break;
		case ENEMY_DIRECTION::LEFT_MOVE:
		case ENEMY_DIRECTION::RIGHT_MOVE:
			_image->aniRender(getMemDC(), _x, _y, _ani_run);
			break;
		case ENEMY_DIRECTION::LEFT_ATTACK:
		case ENEMY_DIRECTION::RIGHT_ATTACK:
			_image->aniRender(getMemDC(), _x, _y, _ani_attack);
			break;
		case ENEMY_DIRECTION::LEFT_ACTION:
		case ENEMY_DIRECTION::RIGHT_ACTION:
			_image->aniRender(getMemDC(), _x, _y, _ani_angry);
			break;
		}
	}
}

void Prim::attack()
{
}

void Prim::checkPlayer()
{
	if (_direction != ENEMY_DIRECTION::LEFT_ATTACK && _direction != ENEMY_DIRECTION::RIGHT_ATTACK && _direction != ENEMY_DIRECTION::LEFT_ACTION && _direction != ENEMY_DIRECTION::RIGHT_ACTION)
	{
		if (getDistance(_player->getX(), 0, _x + _image->getFrameWidth() / 2, 0) <= 50)
		{
			_direction = static_cast<int>(_direction) % 2 == 0 ? ENEMY_DIRECTION::LEFT_ATTACK : ENEMY_DIRECTION::RIGHT_ATTACK;
			_image = IMAGEMANAGER->findImage("보스1 공격");

			if (static_cast<int>(_direction) % 2 == 0)
			{
				_ani_attack->setPlayFrame(0, 19, false);
				_rcAttack.setLeftTopPos(_x + 30, _y + 80);
			}
			else
			{
				_ani_attack->setPlayFrame(20, 39, false);
				_rcAttack.setLeftTopPos(_x + _image->getFrameWidth() / 2 + 40, _y + 80);
			}

			_ani_attack->start();
		}

	}
}

void Prim::move()
{
	if (_direction == ENEMY_DIRECTION::LEFT_MOVE || _direction == ENEMY_DIRECTION::RIGHT_MOVE)
	{
		if (_player->getX() < _x + _image->getFrameWidth() / 2)
		{
			_x -= 6;
		}
		else
		{
			_x += 6;
		}
	}
}

void Prim::draw()
{
	switch (_direction)
	{
	case ENEMY_DIRECTION::LEFT_IDLE:
	case ENEMY_DIRECTION::RIGHT_IDLE:
		_ani_start->frameUpdate(TIMEMANAGER->getElapsedTime() * 0.5);

		if (!_ani_start->isPlay())
		{
			_image = IMAGEMANAGER->findImage("보스1 이동");
			if (_player->getX() < _x + _image->getFrameWidth() / 2)
			{
				_direction = ENEMY_DIRECTION::LEFT_MOVE;
				_ani_run->setPlayFrame(0, 7, true);
			}
			else
			{
				_direction = ENEMY_DIRECTION::RIGHT_MOVE;
				_ani_run->setPlayFrame(8, 15, true);
			}
			_ani_run->start();
		}
		break;
	case ENEMY_DIRECTION::LEFT_MOVE:
	case ENEMY_DIRECTION::RIGHT_MOVE:
		_ani_run->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		break;
	case ENEMY_DIRECTION::LEFT_ATTACK:
	case ENEMY_DIRECTION::RIGHT_ATTACK:
		_ani_attack->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);

		if (!_ani_attack->isPlay())
		{
			_image = IMAGEMANAGER->findImage("보스1 화남");
			if (static_cast<int>(_direction) % 2 == 0)
			{
				_direction = ENEMY_DIRECTION::LEFT_ACTION;
				_ani_angry->setPlayFrame(0, 14, false);
			}
			else
			{
				_direction = ENEMY_DIRECTION::RIGHT_ACTION;
				_ani_angry->setPlayFrame(15, 29, false);
			}
			_ani_angry->start();
		}
		break;
	case ENEMY_DIRECTION::LEFT_ACTION:
	case ENEMY_DIRECTION::RIGHT_ACTION:
		_ani_angry->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);

		if (!_ani_angry->isPlay())
		{
			_image = IMAGEMANAGER->findImage("보스1 이동");
			if (_player->getX() < _x + _image->getFrameWidth() / 2)
			{
				_direction = ENEMY_DIRECTION::LEFT_MOVE;
				_ani_run->setPlayFrame(0, 7, true);
			}
			else
			{
				_direction = ENEMY_DIRECTION::RIGHT_MOVE;
				_ani_run->setPlayFrame(8, 15, true);
			}
			_ani_run->start();
		}
		break;
	}
}

void Prim::checkCollision()
{
	if (!_player->getIsAttacked())
	{
		if (isCollision(_player->getHitbox(), _rc) ||
			(isCollision(_player->getHitbox(), _rcAttack) && (_direction == ENEMY_DIRECTION::LEFT_ATTACK ||
				_direction == ENEMY_DIRECTION::RIGHT_ATTACK)))
		{
			_player->setIsAttacked(true);
			_player->setHP(_player->getHP() - 1);
		}
	}

	if (_player->isAttacking() && !_player->getIsCheckAttack() && isCollision(_player->getAttackRc(), _rc))
	{
		_player->setIsCheckAttack(true);
		_hp -= 2;
		cout << "공격, " << _hp << endl;
	}

	if (_player->checkBulletCollision(_rc))
	{
		_hp -= 1;
		cout << "원거리 공격, " << _hp << endl;
	}
}

void Prim::start()
{
	if (!_isStart)
	{
		_ani_start->setPlayFrame(0, 9, false);
		_ani_start->start();
		_isStart = true;
	}
	else
	{

		if (_ani_start->isPlay())
		{
			_ani_start->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		}
		else
		{
			checkPlayer();
		}
	}
}