#include "stdafx.h"
#include "witch.h"
#include "player/player.h"

HRESULT Witch::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("보스2 왼쪽");
	_imageRight = IMAGEMANAGER->findImage("보스2 오른쪽");
	_imageLeftAttackBack = IMAGEMANAGER->findImage("보스2 왼쪽 불렛2 배경");

	_x = position.x;
	_xRight = WINSIZEX - _x - 100;
	_y = position.y;
	_yRight = _y - 24;

	{
		_leftBullet1[0].Image = _leftBullet1[1].Image = _leftBullet1[2].Image = IMAGEMANAGER->findImage("보스2 왼쪽 불렛");
		_leftBullet1[0].fireX = _leftBullet1[0].x = _leftBullet1[1].fireX = _leftBullet1[1].x = _x + 50;
		_leftBullet1[0].fireY = _leftBullet1[0].y = _leftBullet1[1].fireY = _leftBullet1[1].y = _y;
		_leftBullet1[0].range = _leftBullet1[1].range = 700;
		_leftBullet1[0].isFire = _leftBullet1[1].isFire = false;
		_leftBullet1[0].count = _leftBullet1[1].count = 0.0f;
	}

	for (int i = 0; i < 10; ++i)
	{
		_leftBullet2[i].Image = IMAGEMANAGER->findImage("보스2 왼쪽 불렛2");
		_leftBullet2[i].isFire = false;
		_leftBullet2[i].range = 500;
	}

	{
		_rightBullet1.Image = IMAGEMANAGER->findImage("보스2 오른쪽 불렛");
		_rightBullet1.fireX = _rightBullet1.x = _xRight - 20;
		_rightBullet1.fireY = _rightBullet1.y = _yRight - 20;
		_rightBullet1.range = 1000;
		_rightBullet1.isFire = false;
		_rightBullet1.count = 0;
	}

	{
		_rightBullet2.fireX = _rightBullet2.x = WINSIZEX / 2;
		_rightBullet2.fireY = _rightBullet2.y = WINSIZEY / 2;
		_rightBullet2.isFire = false;
		_rightBullet2.count = 0;
	}

	_rc.set(0, 0, 40, 60);
	_rc.setCenterPos(_x + _image->getFrameWidth() / 2, _y + _image->getFrameHeight() / 2);
	_rcRight.set(0, 0, 40, 60);
	_rcRight.setCenterPos(_xRight + _imageRight->getFrameWidth() / 2, _yRight + _imageRight->getFrameHeight() / 2);

	_ani_idle_left = new Animation;
	_ani_idle_left->init(_image->getWidth(), _image->getHeight(),
		_image->getFrameWidth(), _image->getFrameHeight());
	_ani_idle_left->setFPS(1);
	_ani_idle_left->setDefPlayFrame(false, true);
	_ani_idle_left->setPlayFrame(0, 4, false, true);
	_ani_idle_left->start();

	_ani_attack_left = new Animation;
	_ani_attack_left->init(IMAGEMANAGER->findImage("보스2 왼쪽 공격")->getWidth(),
		IMAGEMANAGER->findImage("보스2 왼쪽 공격")->getHeight(),
		_image->getFrameWidth(), _image->getFrameHeight());
	_ani_attack_left->setFPS(1);
	_ani_attack_left->setDefPlayFrame(false, false);
	_ani_attack_left->setPlayFrame(0, 9, false, false);

	_ani_idle_right = new Animation;
	_ani_idle_right->init(_imageRight->getWidth(), _imageRight->getHeight(),
		_imageRight->getFrameWidth(), _imageRight->getFrameHeight());
	_ani_idle_right->setFPS(1);
	_ani_idle_right->setDefPlayFrame(false, true);
	_ani_idle_right->setPlayFrame(0, 4, false, true);
	_ani_idle_right->start();

	_ani_attack_right = new Animation;
	_ani_attack_right->init(IMAGEMANAGER->findImage("보스2 오른쪽 공격")->getWidth(),
		IMAGEMANAGER->findImage("보스2 오른쪽 공격")->getHeight(),
		_imageRight->getFrameWidth(), _imageRight->getFrameHeight());
	_ani_attack_right->setFPS(1);
	_ani_attack_right->setDefPlayFrame(false, false);
	_ani_attack_right->setPlayFrame(0, 17, false, false);

	_isStart = _attackVer = false;
	_attackCount = _leftAttack2Count = 0;

	_direction = ENEMY_DIRECTION::LEFT_IDLE;

	_maxHp = 30;
	_hp = _maxHp;

	_alpha = 255;

	return S_OK;
}

void Witch::release()
{
}

void Witch::update()
{
	start();
	attack();
	move();
	checkCollision();
}

void Witch::render()
{
	if (TIMEMANAGER->getDebug())
	{
		_rc.render(getMemDC());
		_rcRight.render(getMemDC());
	}
	draw();
}

void Witch::attack()
{
	++_attackCount;

	if (_attackCount > 220)
	{
		switch (_direction)
		{
		case ENEMY_DIRECTION::LEFT_IDLE:
			_image = IMAGEMANAGER->findImage("보스2 왼쪽 공격");
			_ani_idle_left->stop();
			_ani_attack_left->start();
			_direction = ENEMY_DIRECTION::LEFT_ATTACK;
			if (!_attackVer)
			{
				cout << "공격" << endl;
				attackWithLeftBullet(0);
				SOUNDMANAGER->play("witchleft");
			}
			else
			{
				cout << "두 번째 공격" << endl;
				_leftAttack2Count = 0;
				_leftAttackX = _player->getX() - _imageLeftAttackBack->getWidth() / 2;

				for (int i = 0; i < 10; ++i)
				{
					_leftBullet2[i].x = _leftBullet2[i].fireX = _leftAttackX + 40;
					_leftBullet2[i].y = _leftBullet2[i].fireY = 100 + _imageLeftAttackBack->getHeight();
					_leftBullet2[i].angle = PI * 0.5 + RND->getFromFloatTo(-0.5, 0.5);
					_leftBullet2[i].speed = RND->getFromIntTo(4, 8);
				}
			}
			break;
		case ENEMY_DIRECTION::RIGHT_IDLE:
			_imageRight = IMAGEMANAGER->findImage("보스2 오른쪽 공격");
			_ani_idle_right->stop();
			_ani_attack_right->start();
			_direction = ENEMY_DIRECTION::RIGHT_ATTACK;
			if (!_attackVer)
			{
				cout << "공격" << endl;
				_rightBullet1.count = 0;
				_rightBullet1.fireX = _rightBullet1.x = _xRight - 20;
				_rightBullet1.fireY = _rightBullet1.y = _yRight - 20;
				_rightBullet1.isFire = true;
			}
			else
			{
				cout << "두 번째 공격" << endl;

				_rightBullet2.x = _player->getX();
				_rightBullet2.y = _player->getY() - 50;
				_rightBullet2.isFire = true;
			}
			SOUNDMANAGER->play("witchright");
			break;
		case ENEMY_DIRECTION::LEFT_ATTACK:
			if (!_ani_attack_left->isPlay())
			{
				_imageRight = IMAGEMANAGER->findImage("보스2 오른쪽");
				_ani_idle_right->start();

				_direction = ENEMY_DIRECTION::RIGHT_IDLE;
			}
			break;
		case ENEMY_DIRECTION::RIGHT_ATTACK:
			if (!_ani_attack_right->isPlay())
			{
				_image = IMAGEMANAGER->findImage("보스2 왼쪽");
				_ani_idle_left->start();

				_direction = ENEMY_DIRECTION::LEFT_IDLE;
			}
			break;
		}
		_attackCount = 0;
	}
}

void Witch::checkPlayer()
{
}

void Witch::move()
{
	switch (_direction)
	{
	case ENEMY_DIRECTION::LEFT_IDLE:
	case ENEMY_DIRECTION::RIGHT_IDLE:
		_ani_idle_left->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		_ani_idle_right->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		break;

	case ENEMY_DIRECTION::LEFT_ATTACK:
	case ENEMY_DIRECTION::RIGHT_ATTACK:
		_ani_attack_left->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		_ani_attack_right->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		break;
	}

	if (_leftBullet1[0].isFire)
	{
		_leftBullet1[0].count += 0.01f;
		_leftBullet1[0].x = calculateBezierPoint(_leftBullet1[0].count, _pts[0], _pts[1], _pts[2], _pts[3]).x;
		_leftBullet1[0].y = calculateBezierPoint(_leftBullet1[0].count, _pts[0], _pts[1], _pts[2], _pts[3]).y;
		_leftBullet1[0].rc = { _leftBullet1[0].x, _leftBullet1[0].y, _leftBullet1[0].x + _leftBullet1[0].Image->getWidth(), _leftBullet1[0].y + _leftBullet1[0].Image->getHeight() };
		if (_leftBullet1[0].range <= getDistance(_leftBullet1[0].fireX, _leftBullet1[0].fireY, _leftBullet1[0].x, _leftBullet1[0].y))
		{
			_leftBullet1[0].isFire = false;
			_leftBullet1[0].count = 0;
		}

		if (_leftBullet1[0].count >= 0.4f && !_leftBullet1[1].isFire)
		{
			attackWithLeftBullet(1);
			SOUNDMANAGER->play("witchleft");
		}
	}

	if (_leftBullet1[1].isFire)
	{
		_leftBullet1[1].count += 0.005f;
		_leftBullet1[1].x = calculateBezierPoint(_leftBullet1[1].count, _pts[4], _pts[5], _pts[6], _pts[7]).x;
		_leftBullet1[1].y = calculateBezierPoint(_leftBullet1[1].count, _pts[4], _pts[5], _pts[6], _pts[7]).y;
		_leftBullet1[1].rc = { _leftBullet1[1].x, _leftBullet1[1].y, _leftBullet1[1].x + _leftBullet1[1].Image->getWidth(), _leftBullet1[1].y + _leftBullet1[1].Image->getHeight() };
		if (_leftBullet1[1].range <= getDistance(_leftBullet1[1].fireX, _leftBullet1[1].fireY, _leftBullet1[1].x, _leftBullet1[1].y))
		{
			_leftBullet1[1].isFire = false;
			_leftBullet1[1].count = 0;
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		if (_leftBullet2[i].isFire)
		{
			_leftBullet2[i].x += cosf(_leftBullet2[i].angle) * _leftBullet2[i].speed;
			_leftBullet2[i].y += -sinf(_leftBullet2[i].angle) * _leftBullet2[i].speed;
			_leftBullet2[i].rc = { _leftBullet2[i].x, _leftBullet2[i].y, _leftBullet2[i].x + _leftBullet2[i].Image->getWidth(), _leftBullet2[i].y + _leftBullet2[i].Image->getHeight() };
			if (getDistance(_leftBullet2[i].x, _leftBullet2[i].y, _leftBullet2[i].fireX, _leftBullet2[i].fireY)
				>= _leftBullet2[i].range)
			{
				_leftBullet2[i].isFire = false;
			}
		}
	}

	if (_rightBullet1.isFire)
	{
		++_rightBullet1.count;
		if (_rightBullet1.count == 50)
		{
			_rightBullet1.angle = getAngle(_rightBullet1.fireX, _rightBullet1.fireY, _player->getX(), _player->getY());
		}
		if (_rightBullet1.count > 80)
		{
			_rightBullet1.x += cosf(_rightBullet1.angle) * 13.0f;
			_rightBullet1.y += -sinf(_rightBullet1.angle) * 13.0f;
			_rightBullet1.rc = { _rightBullet1.x, _rightBullet1.y, _rightBullet1.x + _rightBullet1.Image->getWidth(), _rightBullet1.y + _rightBullet1.Image->getHeight() };
			if (getDistance(_rightBullet1.x, _rightBullet1.y, _rightBullet1.fireX, _rightBullet1.fireY)
				>= _rightBullet1.range)
			{
				_rightBullet1.isFire = false;
				_attackVer = !_attackVer;
			}
		}
	}

	if (_rightBullet2.isFire)
	{
		++_rightBullet2.count;
		if (++_rightBullet2.count > 230)
		{
			EFFECTMANAGER->play("보스2", _rightBullet2.x + 20, _rightBullet2.y + 20);
			_canCheckRight2 = true;
			_rightBullet2.isFire = false;
			SOUNDMANAGER->play("rellexp", 0.5f);
		}
	}

	if (_canCheckRight2)
	{
		++_rightBullet2.count;

		if (_rightBullet2.count > 300)
		{
			_rightBullet2.count = 0;
			_canCheckRight2 = false;
			_attackVer = !_attackVer;
		}
	}
}

void Witch::draw()
{
	if (!_isStart)
	{
		_image->aniRender(getMemDC(), _x, _y, _ani_idle_left);
		_imageRight->aniRender(getMemDC(), _xRight, _yRight, _ani_idle_right);
	}
	else {
		if (_hp <= 0)
		{
			if (_alpha > 0) _alpha -= 5;
			switch (_direction)
			{
			case ENEMY_DIRECTION::LEFT_IDLE:
			case ENEMY_DIRECTION::LEFT_ATTACK:
				if (_ani_idle_left->isPlay()) _ani_idle_left->stop();
				if (_ani_attack_left->isPlay()) _ani_attack_left->stop();
				_image = IMAGEMANAGER->findImage("보스2 왼쪽");
				_image->alphaFrameRender(getMemDC(), _x, _y, 0, 0, _alpha);
				break;
			case ENEMY_DIRECTION::RIGHT_IDLE:
			case ENEMY_DIRECTION::RIGHT_ATTACK:
				if (_ani_idle_right->isPlay()) _ani_idle_right->stop();
				if (_ani_attack_right->isPlay()) _ani_attack_right->stop();
				_imageRight = IMAGEMANAGER->findImage("보스2 오른쪽");
				_imageRight->alphaFrameRender(getMemDC(), _xRight, _yRight, 0, 0, _alpha);
				break;
			}
		}
		else
		{
			switch (_direction)
			{
			case ENEMY_DIRECTION::LEFT_IDLE:
				_image->aniRender(getMemDC(), _x, _y, _ani_idle_left);
				break;
			case ENEMY_DIRECTION::RIGHT_IDLE:
				_imageRight->aniRender(getMemDC(), _xRight, _yRight, _ani_idle_right);
				break;
			case ENEMY_DIRECTION::LEFT_ATTACK:
				_image->aniRender(getMemDC(), _x, _y, _ani_attack_left);
				if (_attackVer)
				{
					++_leftAttack2Count;
					if (_leftAttack2Count > 100)
					{
						SOUNDMANAGER->play("witchleft");
						for (int i = 0; i < 10; ++i)
						{
							_leftBullet2[i].isFire = true;
						}
						_leftAttack2Count = -100;
					}
					else
						_imageLeftAttackBack->alphaRender(getMemDC(), _leftAttackX, 95, 122);
				}
				break;
			case ENEMY_DIRECTION::RIGHT_ATTACK:
				_imageRight->aniRender(getMemDC(), _xRight, _yRight, _ani_attack_right);
				break;
			}
		}
		for (int i = 0; i < 2; ++i)
		{
			if (_leftBullet1[i].isFire)
			{
				if (TIMEMANAGER->getDebug()) _leftBullet1[i].rc.render(getMemDC());
				_leftBullet1[i].Image->render(getMemDC(), _leftBullet1[i].x, _leftBullet1[i].y);
			}
		}

		for (int i = 0; i < 10; ++i)
		{
			if (_leftBullet2[i].isFire)
			{
				if (TIMEMANAGER->getDebug()) _leftBullet2[i].rc.render(getMemDC());
				_leftBullet2[i].Image->render(getMemDC(), _leftBullet2[i].x, _leftBullet2[i].y);
			}
		}

		if (_rightBullet1.isFire)
		{
			_rightBullet1.Image->render(getMemDC(), _rightBullet1.x - _rightBullet1.Image->getWidth() / 2,
				_rightBullet1.y - _rightBullet1.Image->getHeight() / 2);
		}

		if (_rightBullet2.isFire)
		{
			if (TIMEMANAGER->getDebug()) _rightBullet2.rc.render(getMemDC());
			EllipseMake(getMemDC(), _rightBullet2.x, _rightBullet2.y, 30, 30);
		}
	}
}

void Witch::checkCollision()
{
	if (!_player->getIsAttacked())
	{
		if (static_cast<int>(_direction) % 2 == 0)
		{
			// 왼쪽
			if (isCollision(_player->getHitbox(), _rc))
			{
				_player->setIsAttacked(true);
				_player->setHP(_player->getHP() - 1);
			}

			if (!_attackVer)
			{
				for (int i = 0; i < 2; ++i)
				{
					if (!_leftBullet1[i].isFire) continue;
					if (isCollision(_player->getHitbox(), _leftBullet1[i].rc))
					{
						_player->setIsAttacked(true);
						_player->setHP(_player->getHP() - 1);
						break;
					}
				}
			}
			else
			{
				for (int i = 0; i < 10; ++i)
				{
					if (!_leftBullet2[i].isFire) continue;
					if (isCollision(_player->getHitbox(), _leftBullet2[i].rc))
					{
						_player->setIsAttacked(true);
						_player->setHP(_player->getHP() - 1);
						break;
					}
				}
			}
		}
		else
		{
			// 오른쪽
			if (isCollision(_player->getHitbox(), _rcRight))
			{
				_player->setIsAttacked(true);
				_player->setHP(_player->getHP() - 1);
			}

			if (!_attackVer)
			{
				if (_rightBullet1.isFire && isCollision(_player->getHitbox(), _rightBullet1.rc))
				{
					_player->setIsAttacked(true);
					_player->setHP(_player->getHP() - 1);
				}
			}
			else
			{
				if (_canCheckRight2 && isCollision(MYCIRCLE(_rightBullet2.x + 20, _rightBullet2.y + 20, 100), _player->getHitbox()))
				{
					_player->setIsAttacked(true);
					_player->setHP(_player->getHP() - 1);
				}
			}
		}
	}

	if (_player->isAttacking() && !_player->getIsCheckAttack())
	{
		if (static_cast<int>(_direction) % 2 == 0)
		{
			if (isCollision(_player->getAttackRc(), _rc))
			{
				_player->setIsCheckAttack(true);
				_hp -= 2;
			}
		}
		else
		{
			if (isCollision(_player->getAttackRc(), _rcRight))
			{
				_player->setIsCheckAttack(true);
				_hp -= 2;
			}
		}
	}
	if (static_cast<int>(_direction) % 2 == 0)
	{
		if (_player->checkBulletCollision(_rc))
		{
			_hp -= 1;
		}
	}
	else
	{
		if (_player->checkBulletCollision(_rcRight))
		{
			_hp -= 1;
		}
	}

}

void Witch::start()
{
	if (!_isStart)
	{
		_attackCount++;
		if (_attackCount >= 150)
		{
			_isStart = true;
			_attackCount = 0;
		}
	}
}

void Witch::attackWithLeftBullet(int i)
{
	_leftBullet1[i].x = _leftBullet1[i].fireX;
	_leftBullet1[i].y = _leftBullet1[i].fireY;
	_pts[0 + i * 4] = { static_cast<long>(_x), static_cast<long>(_y) };
	_pts[1 + i * 4] = { static_cast<long>((_player->getX() + _x) * 0.3), static_cast<long>(_y + 300) };
	_pts[2 + i * 4] = { static_cast<long>((_player->getX() + _x) * 0.7), static_cast<long>(_player->getY() - 300) };
	_pts[3 + i * 4] = { static_cast<long>(_player->getX()), static_cast<long>(_player->getY()) };
	_leftBullet1[i].isFire = true;
}
