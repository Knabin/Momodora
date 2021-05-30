#include "stdafx.h"
#include "player.h"
#include "stageManager.h"

HRESULT Player::init()
{
	_x = 100;
	_y = WINSIZEY / 2;
	_width = _height = 96;

	_image = IMAGEMANAGER->findImage("idle");
	_effect = IMAGEMANAGER->findImage("effect_charge");
	_isOnGround = false;
	_state = RIGHT_IDLE;

	_probeY = _y + _height / 2;
	_probeYT = _y;
	_probeXL = _x - _width / 2;
	_probeXR = _x + _width / 2;

	_speedX = 8.0f;
	_speedY = 6.0f;
	_gravity = 0;
	_angle = PI / 2;

	_hitbox.set(0, 0, 48, 48);
	_hitbox.setCenterPos(_x, _probeY - _height / 4);
	_attackRc.set(0, 0, 70, 60);

	_canMoveLeft = _canMoveRight =  true;

	{
		_ani_idle = new Animation;
		_ani_idle->init(IMAGEMANAGER->findImage("idle")->getWidth(),
			IMAGEMANAGER->findImage("idle")->getHeight(),
			IMAGEMANAGER->findImage("idle")->getFrameWidth(),
			IMAGEMANAGER->findImage("idle")->getFrameHeight());
		_ani_idle->setDefPlayFrame(false, true);
		_ani_idle->setFPS(1);

		_ani_run = new Animation;
		_ani_run->init(IMAGEMANAGER->findImage("run")->getWidth(),
			IMAGEMANAGER->findImage("run")->getHeight(),
			IMAGEMANAGER->findImage("run")->getFrameWidth(),
			IMAGEMANAGER->findImage("run")->getFrameHeight());
		_ani_run->setDefPlayFrame(false, true);
		_ani_run->setFPS(1);

		_ani_attack = new Animation;
		_ani_attack->init(IMAGEMANAGER->findImage("attack")->getWidth(),
			IMAGEMANAGER->findImage("attack")->getHeight(),
			IMAGEMANAGER->findImage("attack")->getFrameWidth(),
			IMAGEMANAGER->findImage("attack")->getFrameHeight());
		_ani_attack->setDefPlayFrame(false, true);
		_ani_attack->setFPS(1);

		_ani_jump = new Animation;
		_ani_jump->init(IMAGEMANAGER->findImage("jump")->getWidth(),
			IMAGEMANAGER->findImage("jump")->getHeight(),
			IMAGEMANAGER->findImage("jump")->getFrameWidth(),
			IMAGEMANAGER->findImage("jump")->getFrameHeight());
		_ani_jump->setDefPlayFrame(false, false);
		_ani_jump->setFPS(1);

		_ani_throw = new Animation;
		_ani_throw->init(IMAGEMANAGER->findImage("throw")->getWidth(),
			IMAGEMANAGER->findImage("throw")->getHeight(),
			IMAGEMANAGER->findImage("throw")->getFrameWidth(),
			IMAGEMANAGER->findImage("throw")->getFrameHeight());
		_ani_throw->setDefPlayFrame(false, false);
		_ani_throw->setFPS(1);

		_ani_dead = new Animation;
		_ani_dead->init(IMAGEMANAGER->findImage("dead")->getWidth(),
			IMAGEMANAGER->findImage("dead")->getHeight(),
			IMAGEMANAGER->findImage("dead")->getFrameWidth(),
			IMAGEMANAGER->findImage("dead")->getFrameHeight());
		_ani_dead->setDefPlayFrame(false, false);
		_ani_dead->setFPS(1);

		_ani_pray = new Animation;
		_ani_pray->init(IMAGEMANAGER->findImage("pray")->getWidth(),
			IMAGEMANAGER->findImage("pray")->getHeight(),
			IMAGEMANAGER->findImage("pray")->getFrameWidth(),
			IMAGEMANAGER->findImage("pray")->getFrameHeight());
		_ani_pray->setDefPlayFrame(false, false);
		_ani_pray->setFPS(1);

		_ani_effect_charge = new Animation;
		_ani_effect_charge->init(_effect->getWidth(),
			_effect->getHeight(),
			_effect->getFrameWidth(),
			_effect->getFrameHeight());
		_ani_effect_charge->setDefPlayFrame(false, true);
		_ani_effect_charge->setPlayFrame(0, 10, false, true);
		_ani_effect_charge->setFPS(1);
	}

	_ani_idle->setPlayFrame(5, 9, false, false);
	_ani_idle->start();

	_attackCount = _attackCount2 = 0;
	_redAlpha = 0;
	_alpha = 255;
	_isOnceAttack = _charge = _chargeFull = _isAttacked = _isCheckAttack = false;

	_hp = 5;

	return S_OK;
}

void Player::release()
{
}

void Player::update()
{
	if(KEYMANAGER->isOnceKeyDown('Q'))
	{
		_hp = 5;
	}
	if (_hp > 0)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			if (_state != LEFT_MOVE && !isJumping() && !isFalling() && !isAttacking())
			{
				setAnimation(LEFT_MOVE);
			}
		}

		if (KEYMANAGER->isStayKeyDown(VK_LEFT) && _x - _hitbox.getWidth() / 2 >= 0)
		{
			if (isJumping())
			{
				_angle = PI / 5 * 3;
				setAnimation(LEFT_JUMP);
			}
			else if (_state != LEFT_IDLE && _isOnGround && !isFalling() && !isAttacking())
			{
				_stateBefore = _state;
				_state = LEFT_MOVE;
				if (!_ani_run->isPlay())
				{
					_image = IMAGEMANAGER->findImage("run");
					_ani_run->setPlayFrame(0, 7, false, true);
					_ani_run->start();
				}
			}
			if (_state == LEFT_MOVE && _ani_run->isPlay() && _canMoveLeft) _x -= 4;
		}

		if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
		{
			if (_state != LEFT_IDLE)
			{
				if (_isOnGround)
				{
					SOUNDMANAGER->stop("이동");
					setAnimation(LEFT_IDLE);
				}
				else if (!(isFalling() || isJumping() || isAttacking()))
				{
					setAnimation(LEFT_FALL);
					_stateBefore = LEFT_IDLE;
					_angle = PI / 5 * 3;
					_gravity = _speedY + 0.2f;
				}
			}
			if (isJumping() || isFalling() || isAttacking())
			{
				_stateBefore = LEFT_IDLE;
			}
		}

		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			if (_state != RIGHT_MOVE && !isJumping() && !isFalling() && !isAttacking())
			{
				setAnimation(RIGHT_MOVE);
			}
		}

		if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _x + _hitbox.getWidth() / 2 <= _sm->getCurrentStageSize())
		{
			if (isJumping())
			{
				_angle = PI / 5 * 2;
				setAnimation(RIGHT_JUMP);
			}
			else if (_state != RIGHT_IDLE && _isOnGround && !isFalling() && !isAttacking())
			{
				_stateBefore = _state;
				_state = RIGHT_MOVE;
				if (!_ani_run->isPlay())
				{
					_image = IMAGEMANAGER->findImage("run");
					_ani_run->setPlayFrame(8, 15, false, true);
					_ani_run->start();
				}
			}
			if (_state == RIGHT_MOVE && _ani_run->isPlay() && _canMoveRight) _x += 4;
		}

		if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
		{
			if (_state != RIGHT_IDLE)
			{
				if (_isOnGround)
				{
					_ani_run->stop();
					_stateBefore = _state;
					_state = RIGHT_IDLE;
					_image = IMAGEMANAGER->findImage("idle");
					_ani_idle->setPlayFrame(5, 9, false, true);
					_ani_idle->start();
				}
				else if (!(isFalling() || isJumping() || isAttacking()))
				{
					_ani_run->stop();
					setAnimation(RIGHT_FALL);
					_angle = PI / 5 * 2;
					_gravity = _speedY + 0.2f;
				}
			}

			if (isJumping() || isFalling() || isAttacking())
			{
				_stateBefore = RIGHT_IDLE;
			}
		}

		if (KEYMANAGER->isOnceKeyDown('A'))
		{
			if (!_canMoveRight) _x -= 2;
			else if (!_canMoveLeft) _x += 2;

			if ((isMovingLeft() || _state == LEFT_IDLE) && !isJumping() && !isFalling() && _isOnGround)
			{
				SOUNDMANAGER->play("점프", 0.4f);
				_stateBefore = _state;
				setAnimation(LEFT_JUMP);
				_gravity = 0;
				_isOnGround = false;
				_angle = PI / 2;
			}
			else if ((isMovingRight() || _state == RIGHT_IDLE) && !isJumping() && !isFalling() && _isOnGround)
			{
				SOUNDMANAGER->play("점프", 0.4f);
				_stateBefore = _state;
				setAnimation(RIGHT_JUMP);
				_gravity = 0;
				_isOnGround = false;
				_angle = PI / 2;
			}
		}

		if (KEYMANAGER->isOnceKeyDown('S'))
		{
			_isCheckAttack = false;
			if (_state != LEFT_ATTACK && _state != RIGHT_ATTACK)
			{
				if (isMovingLeft() || _state == LEFT_IDLE || _state == LEFT_JUMP)
				{
					setAnimation(LEFT_ATTACK);
					_attackRc.setLeftTopPos(_hitbox.left - _hitbox.getWidth() - 10, _hitbox.top - 10);
				}
				else
				{
					setAnimation(RIGHT_ATTACK);
					_attackRc.setLeftTopPos(_hitbox.right - 10, _hitbox.top - 10);
				}
				_isOnceAttack = !_isOnceAttack;
			}
		}

		if (KEYMANAGER->isStayKeyDown('S'))
		{
			++_attackCount;

			if (_attackCount >= 30 && !_charge)
			{
				_charge = true;
				cout << "charge on" << endl;
				_ani_effect_charge->start();
			}

			if (_charge)
			{
				++_attackCount2;
				if (_chargeFull && _redAlpha <= 250) _redAlpha += 5;
				if (_redAlpha <= 252) _redAlpha += 3;

				if (_attackCount2 >= 100 && !_chargeFull)
				{
					_chargeFull = true;
					_ani_effect_charge->stop();
					_redAlpha = 0;
					cout << "charge full on" << endl;
				}
			}

			if (_ani_effect_charge->isPlay())
			{
				_ani_effect_charge->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
			}
		}

		if (KEYMANAGER->isOnceKeyUp('S'))
		{
			if (_chargeFull)
			{
				// 차지 공격
				setAnimation(isLeft() ? LEFT_ATTACKC : RIGHT_ATTACKC);
				cout << "차지 공격" << endl;
				fireChargeBullet();
			}
			else if (_charge)
			{
				// 원거리 공격
				setAnimation(isLeft() ? LEFT_ATTACKC : RIGHT_ATTACKC);
				cout << "원거리 공격" << endl;
				fireBullet();
			}

			_charge = false;
			_chargeFull = false;
			_attackCount = 0;
			_attackCount2 = 0;
			_redAlpha = 0;
			_ani_effect_charge->stop();
		}

		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			_sm->checkEnterBossStage();
			_sm->checkPrayObject();
		}

		if (_isAttacked)
		{
			if (_attackedCount == 0)
			{
				_isCameraShaking = true;
			}

			_attackedCount++;

			if (_attackedCount > 150)
			{
				_isAttacked = false;
				_attackedCount = 0;
				_alpha = 255;
			}
		}
		if ((isJumping() || isFalling()) && !_isOnGround)
		{
			if (isJumping() && !(_angle >= PI / 2 && _canMoveLeft || _angle <= PI / 2 && _canMoveRight))
			{
				setAnimation(isLeft() ? LEFT_FALL : RIGHT_FALL);
			}
			else if (isFalling() && (!_canMoveLeft || !_canMoveRight))
			{
				_y += 3;
			}
			else
			{
				if (_x - _hitbox.getWidth() / 2 >= 0 && _x + _hitbox.getWidth() / 2 <= _sm->getCurrentStageSize())
					_x += cosf(_angle) * _speedX;
				_y += -sinf(_angle) * _speedY + _gravity;
				_gravity += 0.15f;
			}
		}

		if (!(isJumping() || isFalling()) && !_isOnGround)
		{
			_y += 3;
		}

		_hitbox.setCenterPos(_x, _probeY - _height / 4);
		_centerY = _hitbox.right - _hitbox.getWidth() / 2;

		_probeXL = _hitbox.left;
		_probeXR = _hitbox.right;
		_probeY = _y + _height / 2;
		_probeYT = _y;

		moveBullet();
	}

	switch (_state)
	{
	case LEFT_IDLE: case RIGHT_IDLE:
		_ani_idle->frameUpdate(TIMEMANAGER->getElapsedTime() * 5);
		break;
	case LEFT_MOVE: case RIGHT_MOVE:
		_ani_run->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		break;
	case LEFT_ATTACK: case RIGHT_ATTACK:
		_ani_attack->frameUpdate(TIMEMANAGER->getElapsedTime() * 20);
		if (!_ani_attack->isPlay())
		{
			if (_state == LEFT_ATTACK) setAnimation(LEFT_IDLE);
			else setAnimation(RIGHT_IDLE);
		}
		break;
	case LEFT_ATTACKC: case RIGHT_ATTACKC:
		_ani_throw->frameUpdate(TIMEMANAGER->getElapsedTime() * 15);
		if (!_ani_throw->isPlay())
		{
			if (_state == LEFT_ATTACKC) setAnimation(LEFT_IDLE);
			else setAnimation(RIGHT_IDLE);
		}
		break;
	case LEFT_JUMP: case RIGHT_JUMP:
		_ani_jump->frameUpdate(TIMEMANAGER->getElapsedTime() * 2);

		if (_gravity > _speedY)
		{
			_image = IMAGEMANAGER->findImage("jump");
			_state == LEFT_JUMP ? _ani_jump->setPlayFrame(2, 10, false, false) : _ani_jump->setPlayFrame(13, 21, false, false);
			_state = _state == LEFT_JUMP ? LEFT_FALL : RIGHT_FALL;
		}
		break;
	case LEFT_FALL: case RIGHT_FALL:
		if (_isOnGround)
		{
			_state = _stateBefore;
			setAnimation(_state);
		}
		_ani_jump->frameUpdate(TIMEMANAGER->getElapsedTime() * 1);

		break;

	case LEFT_DEAD: case RIGHT_DEAD:
		_ani_dead->frameUpdate(TIMEMANAGER->getElapsedTime() * 13);
		break;

	case LEFT_PRAY: case RIGHT_PRAY:
		_ani_pray->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		_prayCount++;
		if (_prayCount >= 150)
		{
			if (_hp < 5)
			{
				SOUNDMANAGER->play("pray");
				EFFECTMANAGER->play("pray", _x, _y + 30);
				++_hp;
			}
			_prayCount = 0;
		}
		break;
	}

	
}

void Player::render()
{
	if(isAttacking() && !getIsCheckAttack() && TIMEMANAGER->getDebug()) _attackRc.render(getMemDC());
	if (TIMEMANAGER->getDebug())
	{
		_hitbox.render(getMemDC());
		Rectangle(getMemDC(), _probeXL + 10, _probeYT, _probeXR - 10, _probeYT + 10);
		RectangleMakeCenter(getMemDC(), _probeXL, _y + _height / 4, 5, 5);
		RectangleMakeCenter(getMemDC(), _probeXR, _y + _height / 4, 5, 5);
	}

	if (_charge)
	{
		switch (_state)
		{
		case LEFT_IDLE: case RIGHT_IDLE:
			_image->aniRedRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_idle, _redAlpha);
			break;
		case LEFT_MOVE: case RIGHT_MOVE:
			_image->aniRedRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_run, _redAlpha);
			break;
		case LEFT_ATTACK: case RIGHT_ATTACK:
			_image->aniRedRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_attack, _redAlpha);
			break;
		case LEFT_JUMP: case RIGHT_JUMP: case LEFT_FALL:  case RIGHT_FALL:
			_image->aniRedRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_jump, _redAlpha);
			break;
		case LEFT_DEAD: case RIGHT_DEAD:
			_image->aniRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_dead);
			if (!_ani_dead->isPlay()) SOUNDMANAGER->stop("죽음");
			break;
		case LEFT_PRAY: case RIGHT_PRAY:
			_image->aniRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_pray);
			break;
		}
	}
	
	else {
		switch (_state)
		{
		case LEFT_IDLE: case RIGHT_IDLE:
			if (_alpha == 255) _image->aniRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_idle);
			else _image->aniAlphaRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_idle, _alpha);

			break;
		case LEFT_MOVE: case RIGHT_MOVE:
			if (_alpha == 255) _image->aniRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_run);
			else _image->aniAlphaRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_run, _alpha);
			break;
		case LEFT_ATTACK: case RIGHT_ATTACK:
			if (_alpha == 255) _image->aniRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_attack);
			else _image->aniAlphaRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_attack, _alpha);
			break;
		case LEFT_ATTACKC: case RIGHT_ATTACKC:
			if (_alpha == 255) _image->aniRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_throw);
			else _image->aniAlphaRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_throw, _alpha);
			break;
		case LEFT_JUMP: case RIGHT_JUMP: case LEFT_FALL:  case RIGHT_FALL:
			if (_alpha == 255) _image->aniRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_jump);
			else _image->aniAlphaRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_jump, _alpha);
			break;
		case LEFT_DEAD: case RIGHT_DEAD:
			_image->aniRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_dead);
			if (!_ani_dead->isPlay()) SOUNDMANAGER->stop("죽음");
			break;
		case LEFT_PRAY: case RIGHT_PRAY:
			_image->aniRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_pray);
			break;
		}
	}

	if (_charge && _ani_effect_charge->isPlay())
	{
		_effect->aniRender(getMemDC(), _x - _width / 2, _y - _height / 2 + 20, _ani_effect_charge);
	}

	renderBullet();

}

void Player::setAnimation(PLAYERSTATE state)
{
	_ani_idle->stop();
	_ani_run->stop();
	_ani_attack->stop();
	_ani_jump->stop();
	_ani_throw->stop();
	_ani_pray->stop();

	_state = state;

	switch (state)
	{
	case LEFT_IDLE:
		_ani_idle->setPlayFrame(0, 4, false, true);
		_ani_idle->start();
		_image = IMAGEMANAGER->findImage("idle");
		break;
	case RIGHT_IDLE:
		_ani_idle->setPlayFrame(5, 9, false, true);
		_ani_idle->start();
		_image = IMAGEMANAGER->findImage("idle");
		break;
	case LEFT_MOVE:
		_ani_run->setPlayFrame(0, 7, false, true);
		_ani_run->start();
		_image = IMAGEMANAGER->findImage("run");
		break;
	case RIGHT_MOVE:
		_ani_run->setPlayFrame(8, 15, false, true);
		_ani_run->start();
		_image = IMAGEMANAGER->findImage("run");
		break;
	case LEFT_ATTACK:
		_ani_attack->setPlayFrame(0, 3, false, false);
		if (_isOnceAttack)
		{
			_image = IMAGEMANAGER->findImage("attack2");
			EFFECTMANAGER->play("left2", _x, _y + 10);
		}
		else
		{
			_image = IMAGEMANAGER->findImage("attack");
			EFFECTMANAGER->play("left", _x, _y + 10);
		}
		SOUNDMANAGER->play("공격", 0.8f);
		_ani_attack->start();
		break;
	case RIGHT_ATTACK:
		_ani_attack->setPlayFrame(4, 7, false, false);
		if (_isOnceAttack)
		{
			_image = IMAGEMANAGER->findImage("attack2");
			EFFECTMANAGER->play("right2", _x, _y + 10);
		}
		else
		{
			_image = IMAGEMANAGER->findImage("attack");
			EFFECTMANAGER->play("right", _x, _y + 10);
		}
		SOUNDMANAGER->play("공격", 0.8f);
		_ani_attack->start();
		break;
	case LEFT_ATTACKC:
		_ani_throw->setPlayFrame(0, 2, false, false);
		_ani_throw->start();
		_image = IMAGEMANAGER->findImage("throw");
		break;
	case RIGHT_ATTACKC:
		_ani_throw->setPlayFrame(3, 5, false, false);
		_ani_throw->start();
		_image = IMAGEMANAGER->findImage("throw");
		break;
	case LEFT_JUMP:
		_ani_jump->setPlayFrame(0, 1, false, true);
		_ani_jump->start();
		_image = IMAGEMANAGER->findImage("jump");
		break;
	case RIGHT_JUMP:
		_ani_jump->setPlayFrame(11, 12, false, true);
		_ani_jump->start();
		_image = IMAGEMANAGER->findImage("jump");
		break;
	case LEFT_FALL:
		_ani_jump->setPlayFrame(2, 10, false, false);
		_ani_jump->start();
		_image = IMAGEMANAGER->findImage("jump");
		break;
	case RIGHT_FALL:
		_ani_jump->setPlayFrame(13, 21, false, false);
		_ani_jump->start();
		_image = IMAGEMANAGER->findImage("jump");
		break;
	case LEFT_DEAD:
		_ani_dead->setPlayFrame(0, 15, false, false);
		_ani_dead->start();
		_image = IMAGEMANAGER->findImage("dead");
		SOUNDMANAGER->stopAll("");
		SOUNDMANAGER->play("죽음", 0.7f);
		break;
	case RIGHT_DEAD:
		_ani_dead->setPlayFrame(16, 31, false, false);
		_ani_dead->start();
		_image = IMAGEMANAGER->findImage("dead");
		SOUNDMANAGER->stopAll("");
		SOUNDMANAGER->play("죽음", 0.7f);
		break;
	case LEFT_PRAY:
		_ani_pray->setPlayFrame(0, 3, false, false);
		_ani_pray->start();
		_image = IMAGEMANAGER->findImage("pray");
		break;
	case RIGHT_PRAY:
		_ani_pray->setPlayFrame(4, 7, false, false);
		_ani_pray->start();
		_image = IMAGEMANAGER->findImage("pray");
		break;
	}
}

void Player::setStatePray()
{
	_prayCount = 0;
	setAnimation(isLeft() ? LEFT_PRAY : RIGHT_PRAY);
}

void Player::fireBullet()
{
	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(bullet));

	bullet.Image = IMAGEMANAGER->findImage("부적");
	bullet.speed = 7.0f;
	bullet.x = bullet.fireX = _x + (isLeft() ? -20 : 20);
	bullet.y = bullet.fireY = _y + 25;
	bullet.angle = isLeft() ? PI : 0;
	bullet.range = 150;

	bullet.rc = RectMakeCenter(bullet.x, bullet.y,
		bullet.Image->getWidth(),
		bullet.Image->getHeight());

	_vBullet.push_back(bullet);
}

void Player::fireChargeBullet()
{
	for (int i = 0; i < 3; ++i)
	{
		tagBullet bullet;
		ZeroMemory(&bullet, sizeof(bullet));

		bullet.Image = IMAGEMANAGER->findImage("부적");
		bullet.speed = 7.0f;
		bullet.x = bullet.fireX = _x + (isLeft() ? -20 : 20);
		bullet.y = bullet.fireY = _y + 25;
		bullet.angle = (isLeft() ? PI : 0) + (1 - i) * 0.5f;
		bullet.range = 150;

		bullet.rc = RectMakeCenter(bullet.x, bullet.y,
			bullet.Image->getWidth(),
			bullet.Image->getHeight());

		_vBullet.push_back(bullet);
	}
}

void Player::moveBullet()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); )
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->Image->getWidth(), _viBullet->Image->getHeight());

		if (_viBullet->range < getDistance(_viBullet->x, _viBullet->y,
			_viBullet->fireX, _viBullet->fireY))
		{
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}

void Player::renderBullet()
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		Rectangle(getMemDC(), _viBullet->rc);
		_viBullet->Image->render(getMemDC(),
			_viBullet->rc.left, _viBullet->rc.top);
	}
}

void Player::removeBullet(int index)
{
	_vBullet.erase(_vBullet.begin() + index);
}

bool Player::checkBulletCollision(MYRECT rc)
{
	RECT rcR = RectMake(rc.left, rc.top, rc.getWidth(), rc.getHeight());
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		if (isCollision(_vBullet[i].rc, rcR))
		{
			isLeft() ? EFFECTMANAGER->play("left hit2", _vBullet[i].rc.left, _vBullet[i].rc.top) : EFFECTMANAGER->play("right hit2", _vBullet[i].rc.right, _vBullet[i].rc.top);
			removeBullet(i);
			return true;
		}
	}
	return false;
}

void Player::setPointLeftStart()
{
	_x = 80;
}

void Player::setPointRightStart()
{
	_x = _sm->getCurrentStageSize() - 80;
}
