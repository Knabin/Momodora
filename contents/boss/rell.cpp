#include "stdafx.h"
#include "rell.h"
#include "player/player.h"

HRESULT Rell::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("보스3-1 기본");
	_fire = IMAGEMANAGER->findImage("보스3-1 기도 불꽃");
	_snow = IMAGEMANAGER->findImage("보스3-2 눈꽃");
	_snow2 = IMAGEMANAGER->findImage("보스3-2 눈꽃2");

	{
		_ani_idle1 = new Animation;
		_ani_idle1->init(_image->getWidth(), _image->getHeight(),
			_image->getFrameWidth(), _image->getFrameHeight());
		_ani_idle1->setFPS(1);
		_ani_idle1->setDefPlayFrame(false, true);
		_ani_idle1->setPlayFrame(0, 4, false, true);
		_ani_idle1->start();

		_ani_ground1 = new Animation;
		_ani_ground1->init(IMAGEMANAGER->findImage("보스3-1 땅")->getWidth(),
			IMAGEMANAGER->findImage("보스3-1 땅")->getHeight(),
			_image->getFrameWidth(), _image->getFrameHeight());
		_ani_ground1->setFPS(1);
		_ani_ground1->setDefPlayFrame(false, false);

		_ani_attack1 = new Animation;
		_ani_attack1->init(IMAGEMANAGER->findImage("보스3-1 공격")->getWidth(),
			IMAGEMANAGER->findImage("보스3-1 공격")->getHeight(),
			_image->getFrameWidth(), _image->getFrameHeight());
		_ani_attack1->setFPS(1);
		_ani_attack1->setDefPlayFrame(false, false);

		_ani_pray = new Animation;
		_ani_pray->init(IMAGEMANAGER->findImage("보스3-1 공격")->getWidth(),
			IMAGEMANAGER->findImage("보스3-1 공격")->getHeight(),
			_image->getFrameWidth(), _image->getFrameHeight());
		_ani_pray->setFPS(1);
		_ani_pray->setDefPlayFrame(false, true);


		_ani_attack2 = new Animation;
		_ani_attack2->init(IMAGEMANAGER->findImage("보스3-2")->getWidth(),
			IMAGEMANAGER->findImage("보스3-2")->getHeight(),
			_image->getFrameWidth(), _image->getFrameHeight());
		_ani_attack2->setFPS(1);
		_ani_attack2->setDefPlayFrame(false, false);


		_ani_idle3 = new Animation;
		_ani_idle3->init(IMAGEMANAGER->findImage("보스3-3 기본")->getWidth(),
			IMAGEMANAGER->findImage("보스3-3 기본")->getHeight(),
			IMAGEMANAGER->findImage("보스3-3 기본")->getFrameWidth(),
			IMAGEMANAGER->findImage("보스3-3 기본")->getFrameHeight());
		_ani_idle3->setFPS(1);
		_ani_idle3->setDefPlayFrame(false, true);

		_ani_ground3 = new Animation;
		_ani_ground3->init(IMAGEMANAGER->findImage("보스3-3 땅")->getWidth(),
			IMAGEMANAGER->findImage("보스3-3 땅")->getHeight(),
			IMAGEMANAGER->findImage("보스3-3 땅")->getFrameWidth(),
			IMAGEMANAGER->findImage("보스3-3 땅")->getFrameHeight());
		_ani_ground3->setFPS(1);
		_ani_ground3->setDefPlayFrame(false, false);

		_ani_attack3 = new Animation;
		_ani_attack3->init(IMAGEMANAGER->findImage("보스3-3 공격")->getWidth(),
			IMAGEMANAGER->findImage("보스3-3 공격")->getHeight(),
			IMAGEMANAGER->findImage("보스3-3 공격")->getFrameWidth(),
			IMAGEMANAGER->findImage("보스3-3 공격")->getFrameHeight());
		_ani_attack3->setFPS(1);
		_ani_attack3->setDefPlayFrame(false, false);

		_ani_effect_fire = new Animation;
		_ani_effect_fire->init(_fire->getWidth(),
			_fire->getHeight(),
			_fire->getFrameWidth(),
			_fire->getFrameHeight());
		_ani_effect_fire->setFPS(1);
		_ani_effect_fire->setDefPlayFrame(false, false);

		_ani_snow = new Animation;
		_ani_snow->init(_snow2->getWidth(),
			_snow2->getHeight(),
			_snow2->getFrameWidth(),
			_snow2->getFrameHeight());
		_ani_snow->setFPS(1);
		_ani_snow->setDefPlayFrame(false, true);
		_ani_snow->setPlayFrame(0, 1, false, true);
	}

	_state = RELL_STATE::LEFT_IDLE;
	_mode = RELL_MODE::MODE1;

	_x = position.x;
	_y = position.y;
	_yUp = _y - 160;

	_rc.set(0, 0, 50, 55);
	_rc.setCenterPos(_x + _image->getFrameWidth() / 2, _y + _image->getFrameHeight() / 2 + 20);

	_attackVer = false;
	_canPray = true;

	_attackCount = _prayCount = 0;

	_maxHp = 60;
	_hp = _maxHp;

	{
		for (int i = 0; i < 3; ++i)
		{
			_bullet1[i].Image = IMAGEMANAGER->findImage("보스3 불렛");
			_bullet1[i].fireX = _bullet1[i].x = _x + _image->getWidth() / 2;
			_bullet1[i].fireY = _bullet1[i].y = _y + _image->getHeight() / 2;
			_bullet1[i].angle = PI * 0.5f;
			_bullet1[i].speed = 7.0f;
			_bullet1[i].range = 700;
		}

		for (int i = 0; i < 20; ++i)
		{
			_bullet2[i].Image = IMAGEMANAGER->findImage("보스3 불렛");
			_bullet2[i].fireX = _bullet2[i].x = _x + _image->getWidth() / 2;
			_bullet2[i].fireY = _bullet2[i].y = _y + _image->getHeight() / 2;
			_bullet2[i].angle = PI * 0.5f;
			_bullet2[i].speed = 1.0f;
			_bullet2[i].range = 150;
			_bullet2[i].count = 0;
		}

		for (int i = 0; i < 3; ++i)
		{
			_ground[i].Image = IMAGEMANAGER->findImage("보스3-1 땅");
			_ground[i].x = _x + _image->getWidth() / 2;
			_ground[i].y = _y + _image->getHeight() / 2;
			_ground[i].rc.set(0, 0, 48, 10);
		}
	}

	return S_OK;
}

void Rell::release()
{
}

void Rell::update()
{
	move();
	attack();
	checkCollision();

	cout << "상태: " << static_cast<int>(_state) << endl;
}

void Rell::render()
{
	if (TIMEMANAGER->getDebug())
	{
		_rc.render(getMemDC());
		_ground[0].rc.render(getMemDC());
	}
	draw();
}

void Rell::attack()
{
	if (_mode == RELL_MODE::MODE1)
	{
		++_attackCount;
		if (_attackCount > 120)
		{
			switch (_state)
			{
			case Rell::RELL_STATE::LEFT_IDLE:
			case Rell::RELL_STATE::RIGHT_IDLE:
				if (_canPray && _hp <= _maxHp * 0.5)
				{
					_image = IMAGEMANAGER->findImage("보스3-1 기도");
					_prayCount = 0;
					_rcPray.set(0, 0, 300, 400);
					_rcPray.setCenterPos(_x + _image->getFrameWidth() / 2, _y + _image->getFrameHeight() - 200);
					_state = RELL_STATE::PRAY;
					_ani_pray->setPlayFrame(0, 4, false, true);
					_ani_pray->start();
					SOUNDMANAGER->play("rellpray", 0.8f);
					break;
				}
				if (!_attackVer)
				{
					_image = IMAGEMANAGER->findImage("보스3-1 공격");
					if (changeDirectionToLeft())
					{
						_state = RELL_STATE::LEFT_ATTACK;
						_ani_attack1->setPlayFrame(0, 9);
					}
					else
					{
						_state = RELL_STATE::RIGHT_ATTACK;
						_ani_attack1->setPlayFrame(10, 19);
					}
					_ani_attack1->start();

					for (int i = 0; i < 3; ++i)
					{
						_bullet1[i].x = _bullet1[i].fireX = _x;
						_bullet1[i].y = _bullet1[i].fireY = _y;
						_bullet1[i].angle = getAngle(_x, _y, _player->getX(), _player->getY()) + 0.06f * (1 - i);
						_bullet1[i].isFire = true;
						_bullet1[i].count = 0;
					}
					SOUNDMANAGER->play("rellball", 1.0f);
				}
				else
				{
					// 땅에서 소환
					_image = IMAGEMANAGER->findImage("보스3-1 땅");
					if (changeDirectionToLeft())
					{
						_state = RELL_STATE::LEFT_GROUND;
						_ground[0].Image = IMAGEMANAGER->findImage("보스3-1 땅 왼쪽");
						_ani_ground1->setPlayFrame(0, 11);
					}
					else
					{
						_state = RELL_STATE::RIGHT_GROUND;
						_ground[0].Image = IMAGEMANAGER->findImage("보스3-1 땅 오른쪽");
						_ani_ground1->setPlayFrame(12, 21);
					}
					_ani_ground1->start();

					_ground[0].x = _player->getX();
					_ground[0].rc.setCenterPos(_ground[0].x, _ground[0].y);
				}
				_ani_idle1->stop();
				break;
			case Rell::RELL_STATE::LEFT_GROUND:
			case Rell::RELL_STATE::RIGHT_GROUND:
				if (!_ani_ground1->isPlay())
				{
					_image = IMAGEMANAGER->findImage("보스3-1 기본");
					if (changeDirectionToLeft())
					{
						_state = RELL_STATE::LEFT_IDLE;
						_ani_idle1->setPlayFrame(0, 4);
					}
					else
					{
						_state = RELL_STATE::RIGHT_IDLE;
						_ani_idle1->setDefPlayFrame(5, 7);
					}
					_ani_idle1->start();
					_attackVer = !_attackVer;
				}
				break;
			case Rell::RELL_STATE::LEFT_ATTACK:
			case Rell::RELL_STATE::RIGHT_ATTACK:
				if (!_ani_attack1->isPlay())
				{
					_image = IMAGEMANAGER->findImage("보스3-1 기본");
					if (changeDirectionToLeft())
					{
						_state = RELL_STATE::LEFT_IDLE;
						_ani_idle1->setPlayFrame(0, 4);
					}
					else
					{
						_state = RELL_STATE::RIGHT_IDLE;
						_ani_idle1->setDefPlayFrame(5, 7);
					}
					_ani_idle1->start();
					_attackVer = !_attackVer;
				}
				break;
			case Rell::RELL_STATE::PRAY:
				++_prayCount;
				_hp += 7;
				if (_prayCount > 3)
				{
					SOUNDMANAGER->stop("rellpray");
					_image = IMAGEMANAGER->findImage("보스3-1 기본");
					if (changeDirectionToLeft())
					{
						_state = RELL_STATE::LEFT_IDLE;
						_ani_idle1->setPlayFrame(0, 4);
					}
					else
					{
						_state = RELL_STATE::RIGHT_IDLE;
						_ani_idle1->setDefPlayFrame(5, 7);
					}
					_ani_effect_fire->setPlayFrame(0, 9, false, false);
					_ani_effect_fire->start();
					_ani_idle1->start();
					_ani_pray->stop();
					_prayCount = 0;
					_canPray = false;
					SOUNDMANAGER->play("rellexp");
				}
				break;
			}

			_attackCount = 0;
		}
	}
	else if (_mode == RELL_MODE::MODE2)
	{
		_attackCount++;
		if (_attackCount >= 70)
		{
			_attackVer = !_attackVer;
			_attackCount = 0;
		}

		for (int i = 0; i < 8; ++i)
		{
			if (_attackVer)
			{
				if (i % 2 == 0) continue;
				if (_bullet2[i].isFire) continue;
				_bullet2[i].x = _bullet2[i].fireX;
				_bullet2[i].y = _bullet2[i].fireY;
				_bullet2[i].count = 0;
				_bullet2[i].isFire = true;
			}
			else
			{
				if (i % 2 != 0) continue;
				if (_bullet2[i].isFire) continue;
				_bullet2[i].x = _bullet2[i].fireX;
				_bullet2[i].y = _bullet2[i].fireY;
				_bullet2[i].count = 0;
				_bullet2[i].isFire = true;
			}
		}
	}
	else
	{
		_attackCount++;
		if (_attackCount >= 90)
		{
			switch (_state)
			{
			case Rell::RELL_STATE::LEFT_IDLE:
			case Rell::RELL_STATE::RIGHT_IDLE:
				checkPlayer();
				break;
			case Rell::RELL_STATE::LEFT_GROUND:
			case Rell::RELL_STATE::RIGHT_GROUND:
				if (!_ani_ground3->isPlay())
				{
					_image = IMAGEMANAGER->findImage("보스3-3 기본");
					if (changeDirectionToLeft())
					{
						_state = RELL_STATE::LEFT_IDLE;
						_ani_idle3->setPlayFrame(0, 4, false, true);
					}
					else
					{
						_state = RELL_STATE::RIGHT_IDLE;
						_ani_idle3->setPlayFrame(5, 9, false, true);
					}
					_ani_idle3->start();
				}
				break;
			case Rell::RELL_STATE::LEFT_ATTACK:
			case Rell::RELL_STATE::RIGHT_ATTACK:
				if (!_ani_attack3->isPlay())
				{
					_image = IMAGEMANAGER->findImage("보스3-3 기본");
					if (changeDirectionToLeft())
					{
						_state = RELL_STATE::LEFT_IDLE;
						_ani_idle3->setPlayFrame(0, 4, false, true);
					}
					else
					{
						_state = RELL_STATE::RIGHT_IDLE;
						_ani_idle3->setPlayFrame(5, 9, false, true);
					}
					_ani_idle3->start();
				}
				break;
			case Rell::RELL_STATE::DEAD:
				break;
			}
			_attackCount = 0;
		}
	}
}

void Rell::checkPlayer()
{
	if (getDistance(_x, _y, _player->getX(), _player->getY()) <= 200)
	{
		_image = IMAGEMANAGER->findImage("보스3-3 공격");
		// 공격 범위
		if (changeDirectionToLeft())
		{
			// 왼쪽으로
			_state = RELL_STATE::LEFT_ATTACK;
			_ani_attack3->setPlayFrame(0, 6);
		}
		else
		{
			// 오른쪽으로
			_state = RELL_STATE::RIGHT_ATTACK;
			_ani_attack3->setPlayFrame(7, 11);
		}
		_ani_attack3->start();
	}
	else
	{
		_image = IMAGEMANAGER->findImage("보스3-3 땅");
		// 땅 공격
		if (changeDirectionToLeft())
		{
			// 왼쪽으로
			_state = RELL_STATE::LEFT_GROUND;
			_ani_ground3->setPlayFrame(0, 11);
		}
		else
		{
			// 오른쪽으로
			_state = RELL_STATE::RIGHT_GROUND;
			_ani_ground3->setPlayFrame(12, 23);
		}
		_ani_ground3->start();
		_groundCount = 0;
	}
}

void Rell::move()
{
	if (_mode == RELL_MODE::MODE1)
	{
		switch (_state)
		{
		case Rell::RELL_STATE::LEFT_IDLE:
		case Rell::RELL_STATE::RIGHT_IDLE:
			_ani_idle1->frameUpdate(TIMEMANAGER->getElapsedTime() * 5);
			break;
		case Rell::RELL_STATE::LEFT_GROUND:
		case Rell::RELL_STATE::RIGHT_GROUND:
			_ani_ground1->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);

			++_ground[0].count;
			if (_ground[0].count >= 50 && !_ground[0].isFire)
			{
				_ground[0].isFire = true;
				SOUNDMANAGER->play("rellground", 1.0f);
			}
			break;
		case Rell::RELL_STATE::LEFT_ATTACK:
		case Rell::RELL_STATE::RIGHT_ATTACK:
			_ani_attack1->frameUpdate(TIMEMANAGER->getElapsedTime() * 15);
			break;
		case Rell::RELL_STATE::PRAY:
			_ani_pray->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
			EFFECTMANAGER->play("보스1 기도 전", _x + _image->getFrameWidth() / 2, _y + _image->getFrameHeight() - 10);
			break;
		}

		if (_ani_effect_fire->isPlay()) _ani_effect_fire->frameUpdate(TIMEMANAGER->getElapsedTime() * 20);

		for (int i = 0; i < 3; ++i)
		{
			if (!_bullet1[i].isFire) continue;
			++_bullet1[i].count;
			_bullet1[i].x += cosf(_bullet1[i].angle) * _bullet1[i].speed;
			_bullet1[i].y += -sinf(_bullet1[i].angle) * _bullet1[i].speed;
			_bullet1[i].rc = { _bullet1[i].x, _bullet1[i].y, _bullet1[i].x + _bullet1[i].Image->getWidth(), _bullet1[i].y + _bullet1[i].Image->getHeight() };
			if (getDistance(_bullet1[i].x, _bullet1[i].y, _bullet1[i].fireX, _bullet1[i].fireY) >= _bullet1[i].range)
			{
				_bullet1[i].isFire = false;
			}
			if (_bullet1[i].count < 10) break;
		}

		if (_ground[0].isFire && ++_ground[0].count >= 200)
		{
			_ground[0].count = 0;
			_ground[0].isFire = false;
		}
	}
	else if (_mode == RELL_MODE::MODE2)
	{
		switch (_state)
		{
		case Rell::RELL_STATE::AIR:
			_ani_attack2->frameUpdate(TIMEMANAGER->getElapsedTime() * 5);
			_ani_snow->frameUpdate(TIMEMANAGER->getElapsedTime() * 5);
			break;
		}

		for (int i = 0; i < 8; ++i)
		{
			if (!_bullet2[i].isFire) continue;
			_bullet2[i].x += cosf(_bullet2[i].angle) * _bullet2[i].speed;
			_bullet2[i].y += -sinf(_bullet2[i].angle) * _bullet2[i].speed;

			if (getDistance(_bullet2[i].x, _bullet2[i].y, _bullet2[i].fireX, _bullet2[i].fireY) >= _bullet2[i].range)
			{
				_bullet2[i].isFire = false;
				_bullet2[i].x = _bullet2[i].fireX;
				_bullet2[i].y = _bullet2[i].fireY;
			}

			_bullet2[i].rc = { _bullet2[i].x, _bullet2[i].y, _bullet2[i].x + _bullet2[i].Image->getWidth(), _bullet2[i].y + _bullet2[i].Image->getHeight() };
		}
	}
	else
	{
		switch (_state)
		{
		case Rell::RELL_STATE::LEFT_IDLE:
		case Rell::RELL_STATE::RIGHT_IDLE:
			_ani_idle3->frameUpdate(TIMEMANAGER->getElapsedTime() * 5);
			break;
		case Rell::RELL_STATE::LEFT_GROUND:
		case Rell::RELL_STATE::RIGHT_GROUND:
			_ani_ground3->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
			_groundCount++;

			for (int i = 0; i < 3; ++i)
			{
				if (_ground[i].isFire) continue;
				if (_groundCount >= _ground[i].count)
				{
					_ground[i].x = _player->getX();
					_ground[i].rc.setCenterPos(_ground[i].x, _ground[i].y);
					_ground[i].isFire = true;
				}
			}

			if (_groundCount >= 90)
			{
				for (int i = 0; i < 3; ++i)
				{
					_ground[i].isFire = false;
				}
				_groundCount = 0;
			}

			break;
		case Rell::RELL_STATE::LEFT_ATTACK:
		case Rell::RELL_STATE::RIGHT_ATTACK:
			_ani_attack3->frameUpdate(TIMEMANAGER->getElapsedTime() * 15);
			break;
		case Rell::RELL_STATE::DEAD:
			break;
		}
	}
}

void Rell::draw()
{
	if (_mode == RELL_MODE::MODE1)
	{
		switch (_state)
		{
		case Rell::RELL_STATE::LEFT_IDLE:
		case Rell::RELL_STATE::RIGHT_IDLE:
			_image->aniRender(getMemDC(), _x, _y, _ani_idle1);
			break;
		case Rell::RELL_STATE::LEFT_GROUND:
		case Rell::RELL_STATE::RIGHT_GROUND:
			_image->aniRender(getMemDC(), _x, _y, _ani_ground1);
			break;
		case Rell::RELL_STATE::LEFT_ATTACK:
		case Rell::RELL_STATE::RIGHT_ATTACK:
			_image->aniRender(getMemDC(), _x, _y, _ani_attack1);
			break;
		case Rell::RELL_STATE::PRAY:
			if (TIMEMANAGER->getDebug()) _rcPray.render(getMemDC());
			_image->aniRender(getMemDC(), _x, _y, _ani_pray);
			break;
		}

		if (_ani_effect_fire->isPlay())
			_fire->aniRender(getMemDC(), _rcPray.left - 160, _rcPray.top - 130, _ani_effect_fire);

		for (int i = 0; i < 3; ++i)
		{
			if (!_bullet1[i].isFire) continue;
			if (TIMEMANAGER->getDebug()) _bullet1[i].rc.render(getMemDC());
			_bullet1[i].Image->alphaRender(getMemDC(), _bullet1[i].x, _bullet1[i].y, 180);
		}

		if (_ground[0].isFire)
		{
			_ground[0].Image->render(getMemDC(), _ground[0].x - _ground[0].Image->getWidth() / 2, _ground[0].y - _ground[0].Image->getHeight());
			if (TIMEMANAGER->getDebug()) _ground[0].rc.render(getMemDC());
		}
	}
	else if (_mode == RELL_MODE::MODE2)
	{
		switch (_state)
		{
		case Rell::RELL_STATE::AIR:
			_snow2->aniAlphaRender(getMemDC(), _x - _snow2->getFrameWidth() / 2 + 48, _yUp - _snow2->getFrameHeight() / 2 + 50, _ani_snow, 100);
			_snow->render(getMemDC(), _x - _snow->getWidth() / 2 + 48, _yUp - _snow->getHeight() / 2 + 50);
			_image->aniRender(getMemDC(), _x, _yUp, _ani_attack2);
			break;
		}

		for (int i = 0; i < 8; ++i)
		{
			if (!_bullet2[i].isFire) continue;
			if (TIMEMANAGER->getDebug()) _bullet2[i].rc.render(getMemDC());
			_bullet2[i].Image->alphaRender(getMemDC(), _bullet2[i].x, _bullet2[i].y, 150);
		}
	}
	else
	{
		if (TIMEMANAGER->getDebug())
		{
			_rc.render(getMemDC());
		}

		switch (_state)
		{
		case Rell::RELL_STATE::LEFT_IDLE:
		case Rell::RELL_STATE::RIGHT_IDLE:
			_image->aniRender(getMemDC(), _xLarge, _yLarge, _ani_idle3);
			break;
		case Rell::RELL_STATE::LEFT_GROUND:
		case Rell::RELL_STATE::RIGHT_GROUND:
			_image->aniRender(getMemDC(), _xLarge, _yLarge, _ani_ground3);
			break;
		case Rell::RELL_STATE::LEFT_ATTACK:
		case Rell::RELL_STATE::RIGHT_ATTACK:
			_image->aniRender(getMemDC(), _xLarge, _yLarge, _ani_attack3);
			break;
		case Rell::RELL_STATE::DEAD:
			_image->render(getMemDC(), _x, _y);
			break;
		}

		for (int i = 0; i < 3; ++i)
		{
			if (!_ground[i].isFire) continue;
			_ground[i].Image->render(getMemDC(), _ground[i].x - _ground[i].Image->getWidth() / 2, _ground[i].y - _ground[i].Image->getHeight());
			if (TIMEMANAGER->getDebug()) _ground[i].rc.render(getMemDC());
		}
	}
}

void Rell::checkCollision()
{
	if (!_player->getIsAttacked())
	{
		if (isCollision(_player->getHitbox(), _rc))
		{
			_player->setIsAttacked(true);
			_player->setHP(_player->getHP() - 1);
		}

		if (_mode == RELL_MODE::MODE1)
		{
			if (!_attackVer)
			{
				for (int i = 0; i < 3; ++i)
				{
					if (!_bullet1[i].isFire) continue;
					if (isCollision(_player->getHitbox(), _bullet1[i].rc))
					{
						_player->setIsAttacked(true);
						_player->setHP(_player->getHP() - 1);
						break;
					}
				}
			}
			else
			{
				if (_ground[0].isFire && isCollision(_player->getHitbox(), _ground[0].rc))
				{
					_player->setIsAttacked(true);
					_player->setHP(_player->getHP() - 1);
				}
			}
			if (_ani_effect_fire->isPlay() && isCollision(_player->getHitbox(), _rcPray))
			{
				_player->setIsAttacked(true);
				_player->setHP(_player->getHP() - 2);
			}
		}
		else if (_mode == RELL_MODE::MODE2)
		{
			for (int i = 0; i < 8; ++i)
			{
				if (!_bullet2[i].isFire) continue;
				if (isCollision(_player->getHitbox(), _bullet2[i].rc))
				{
					_player->setIsAttacked(true);
					_player->setHP(_player->getHP() - 1);
					break;
				}
			}
		}
		else
		{
			for (int i = 0; i < 3; ++i)
			{
				if (!_ground[i].isFire) continue;
				if (isCollision(_player->getHitbox(), _ground[i].rc))
				{
					_player->setIsAttacked(true);
					_player->setHP(_player->getHP() - 1);
					break;
				}
			}

			if (getDistance(_x, _y, _player->getX(), _player->getY()) <= 400)
			{
				if (_state == RELL_STATE::LEFT_ATTACK)
				{
					if (changeDirectionToLeft())
					{
						_player->setIsAttacked(true);
						_player->setHP(_player->getHP() - 1);
					}
				}
				else if (_state == RELL_STATE::RIGHT_ATTACK)
				{
					if (!changeDirectionToLeft())
					{
						_player->setIsAttacked(true);
						_player->setHP(_player->getHP() - 1);
					}
				}
			}
		}
	}

	if (_player->isAttacking() && !_player->getIsCheckAttack())
	{
		if (isCollision(_player->getAttackRc(), _rc))
		{
			_player->setIsCheckAttack(true);
			_hp -= 2;
			cout << "공격, " << _hp << endl;
		}
	}

	if (_player->checkBulletCollision(_rc))
	{
		_hp -= 1;
		cout << "원거리 공격, " << _hp << endl;
	}

	changeMode();
}

bool Rell::changeDirectionToLeft()
{
	// 만약 _rc(보스 충돌용).left보다 플레이어 히트박스 right가 작다면 <- 이쪽 방향
	if (_player->getHitbox().right < _rc.left)
	{
		return true;
	}
	return false;
}

void Rell::changeMode()
{
	if (_hp <= 0)
	{
		switch (_mode)
		{
		case RELL_MODE::MODE1:
			_mode = RELL_MODE::MODE2;
			_state = RELL_STATE::AIR;
			_image = IMAGEMANAGER->findImage("보스3-2");
			_ani_attack2->setPlayFrame(0, 4, false, true);
			_ani_attack2->start();
			_ani_snow->start();

			_yUp = _y - 160;

			_rc.setCenterPos(_x + _image->getFrameWidth() / 2, _yUp + _image->getFrameHeight() / 2 + 20);

			for (int i = 0; i < 8; ++i)
			{
				_bullet2[i].isFire = false;
				_bullet2[i].x = _bullet2[i].fireX = _x + _image->getFrameWidth() / 2 - _bullet2[i].Image->getWidth() / 2;
				_bullet2[i].y = _bullet2[i].fireY = _yUp + _bullet2[i].Image->getHeight() / 2 + 20;
				_bullet2[i].angle = PI2 * (0.125 * i);
			}
			_maxHp = 20;
			_hp = 20;
			break;
		case RELL_MODE::MODE2:
			_mode = RELL_MODE::MODE3;
			_state = RELL_STATE::LEFT_IDLE;
			_image = IMAGEMANAGER->findImage("보스3-3 기본");
			_ani_idle3->setPlayFrame(0, 4, false, true);
			_ani_idle3->start();

			_xLarge = _x - _image->getFrameWidth() / 2 + IMAGEMANAGER->findImage("보스3-1 기본")->getFrameWidth() / 2;
			_yLarge = _y - _image->getFrameHeight() / 2 + IMAGEMANAGER->findImage("보스3-1 기본")->getFrameHeight() / 2;

			_rc.setCenterPos(_x + IMAGEMANAGER->findImage("보스3-1 기본")->getFrameWidth() / 2, _y + IMAGEMANAGER->findImage("보스3-1 기본")->getFrameHeight() / 2 + 20);

			_maxHp = 40;
			_hp = 40;
			_attackCount = 0;

			for (int i = 0; i < 3; ++i)
			{
				_ground[i].Image = IMAGEMANAGER->findImage("보스3-3 땅 공격");
				_ground[i].x = _xLarge + _image->getWidth() / 2;
				_ground[i].y = _yLarge + _image->getHeight() / 2 - 80;
				_ground[i].count = 15 * (i + 1);
				_ground[i].rc.set(0, 0, 48, 10);
			}
			break;
		case RELL_MODE::MODE3:
			_state = RELL_STATE::DEAD;
			_image = IMAGEMANAGER->findImage("보스3 죽음");
			break;
		}
	}
}
