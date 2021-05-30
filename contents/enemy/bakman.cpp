#include "stdafx.h"
#include "bakman.h"
#include "player/player.h"

HRESULT bakman::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("몬스터3 공격");
	_bullet = IMAGEMANAGER->findImage("몬스터3 블록");

	_isFire = false;

	_direction = ENEMY_DIRECTION::LEFT_IDLE;

	_x = position.x;
	_y = position.y;

	_width = _image->getFrameWidth();
	_height = _image->getFrameHeight();

	_rc.set(0, 0, 50, 60);
	_rc.setCenterPos(_x + _width / 2, _y + _height / 2 + 20);

	_rcBullet.set(0, 0, _bullet->getWidth(), _bullet->getHeight());

	_ani_attack = new Animation;
	_ani_attack->init(IMAGEMANAGER->findImage("몬스터3 공격")->getWidth(),
		IMAGEMANAGER->findImage("몬스터3 공격")->getHeight(),
		IMAGEMANAGER->findImage("몬스터3 공격")->getFrameWidth(),
		IMAGEMANAGER->findImage("몬스터3 공격")->getFrameHeight());
	_ani_attack->setDefPlayFrame(false, false);
	_ani_attack->setFPS(1);

	_ani_attack2 = new Animation;
	_ani_attack2->init(IMAGEMANAGER->findImage("몬스터3 공격2")->getWidth(),
		IMAGEMANAGER->findImage("몬스터3 공격2")->getHeight(),
		IMAGEMANAGER->findImage("몬스터3 공격2")->getFrameWidth(),
		IMAGEMANAGER->findImage("몬스터3 공격2")->getFrameHeight());
	_ani_attack->setDefPlayFrame(false, false);
	_ani_attack2->setFPS(1);

	_hp = 3;

	return S_OK;
}

void bakman::release()
{
	_ani_attack->stop();
	SAFE_DELETE(_ani_attack);
}

void bakman::update()
{
	move();
	if (_hp <= 0) return;
	checkPlayer();
	attack();
	checkCollision();

	if (_ani_attack->isPlay())
		_ani_attack->frameUpdate(TIMEMANAGER->getElapsedTime() * 5);

	if (_ani_attack2->isPlay())
		_ani_attack2->frameUpdate(TIMEMANAGER->getElapsedTime() * 5);

}

void bakman::render()
{
	draw();
}

void bakman::attack()
{

	if ((_direction == ENEMY_DIRECTION::LEFT_ATTACK ||
		_direction == ENEMY_DIRECTION::RIGHT_ATTACK) && !_ani_attack->isPlay() && !_isFire)
	{
		_image = IMAGEMANAGER->findImage("몬스터3 공격2");
		if (_direction == ENEMY_DIRECTION::LEFT_ATTACK)
		{
			_direction = ENEMY_DIRECTION::LEFT_ACTION;
			_ani_attack2->setPlayFrame(0, 5, false, false);
			SOUNDMANAGER->play("bak1", 1.0f);

			_bulletX = _x + 20;
			_bulletY = _y;
			_angle = PI * 0.7;
		}
		else
		{
			_direction = ENEMY_DIRECTION::RIGHT_ACTION;
			_ani_attack2->setPlayFrame(6, 11, false, false);
			SOUNDMANAGER->play("bak1", 1.0f);

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
	if (getDistance(_x, _y, _player->getX(), _player->getY()) <= 250)
	{
		if (_direction != ENEMY_DIRECTION::LEFT_ATTACK &&
			_direction != ENEMY_DIRECTION::RIGHT_ATTACK)
		{
			_image = IMAGEMANAGER->findImage("몬스터3 공격");
			if (_player->getX() < _rc.right - _rc.getWidth() / 2)
			{
				_direction = ENEMY_DIRECTION::LEFT_ATTACK;
				_ani_attack->setPlayFrame(0, 5, false, false);
			}
			else
			{
				_direction = ENEMY_DIRECTION::RIGHT_ATTACK;
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
			_direction = ENEMY_DIRECTION::LEFT_IDLE;
			_image = IMAGEMANAGER->findImage("몬스터3 왼쪽");
		}
		else
		{
			_direction = ENEMY_DIRECTION::RIGHT_IDLE;
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

		_rcBullet.setLeftTopPos(_bulletX, _bulletY);
	}
}

void bakman::draw()
{
	if (_hp <= 0) return;
	if (TIMEMANAGER->getDebug())
	{
		_rc.render(getMemDC());
	}

	if (_isFire)
	{
		if (TIMEMANAGER->getDebug()) _rcBullet.render(getMemDC());
		_bullet->render(getMemDC(), _bulletX, _bulletY);
	}

	switch (_direction)
	{
	case ENEMY_DIRECTION::LEFT_IDLE:
	case ENEMY_DIRECTION::RIGHT_IDLE:
		_image->render(getMemDC(), _x, _y);
		break;
	case ENEMY_DIRECTION::LEFT_ATTACK:
	case ENEMY_DIRECTION::RIGHT_ATTACK:
		_image->aniRender(getMemDC(), _x, _y, _ani_attack);
		break;
	case ENEMY_DIRECTION::LEFT_ACTION:
	case ENEMY_DIRECTION::RIGHT_ACTION:
		_image->aniRender(getMemDC(), _x, _y, _ani_attack2);
		break;
	}


}

void bakman::checkCollision()
{
	// 플레이어를 공격
	// 본체 충돌
	// 던지는거
	if (!_player->getIsAttacked())
	{
		if (isCollision(_player->getHitbox(), _rc) || isCollision(_player->getHitbox(), _rcBullet))
		{
			_player->setIsAttacked(true);
			_player->setHP(_player->getHP() - 1);
		}
	}

	// 공격당함
	if (_player->isAttacking() && !_player->getIsCheckAttack() && isCollision(_player->getAttackRc(), _rc))
	{
		_player->setIsCheckAttack(true);
		_hp -= 1;
		cout << "공격, " << _hp << endl;
	}

	if (_player->checkBulletCollision(_rc))
	{
		_hp -= 1;
		cout << "원거리 공격, " << _hp << endl;
	}
}

