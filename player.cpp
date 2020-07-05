#include "stdafx.h"
#include "player.h"
#include "stageManager.h"

HRESULT player::init()
{
	_x = 100;
	_y = WINSIZEY / 2;
	_width = _height = 96;

	_image = IMAGEMANAGER->findImage("idle");
	_isOnGround = false;
	_state = RIGHT_IDLE;

	_probeY = _y + _height / 2;
	_probeXL = _x - _width / 2;
	_probeXR = _x + _width / 2;

	_speed = 5.0f;
	_gravity = 0;
	_angle = PI / 2;

	_hitbox.set(0, 0, 48, 48);
	_hitbox.setCenterPos(_x, _probeY - _height / 4);

	_canMoveLeft = _canMoveRight =  true;

	{
		_ani_idle = new animation;
		_ani_idle->init(IMAGEMANAGER->findImage("idle")->getWidth(),
			IMAGEMANAGER->findImage("idle")->getHeight(),
			IMAGEMANAGER->findImage("idle")->getFrameWidth(),
			IMAGEMANAGER->findImage("idle")->getFrameHeight());
		_ani_idle->setDefPlayFrame(false, true);
		_ani_idle->setFPS(1);

		_ani_run = new animation;
		_ani_run->init(IMAGEMANAGER->findImage("run")->getWidth(),
			IMAGEMANAGER->findImage("run")->getHeight(),
			IMAGEMANAGER->findImage("run")->getFrameWidth(),
			IMAGEMANAGER->findImage("run")->getFrameHeight());
		_ani_run->setDefPlayFrame(false, true);
		_ani_run->setFPS(1);

		_ani_attack = new animation;
		_ani_attack->init(IMAGEMANAGER->findImage("attack")->getWidth(),
			IMAGEMANAGER->findImage("attack")->getHeight(),
			IMAGEMANAGER->findImage("attack")->getFrameWidth(),
			IMAGEMANAGER->findImage("attack")->getFrameHeight());
		_ani_attack->setDefPlayFrame(false, true);
		_ani_attack->setFPS(1);

		_ani_jump = new animation;
		_ani_jump->init(IMAGEMANAGER->findImage("jump")->getWidth(),
			IMAGEMANAGER->findImage("jump")->getHeight(),
			IMAGEMANAGER->findImage("jump")->getFrameWidth(),
			IMAGEMANAGER->findImage("jump")->getFrameHeight());
		_ani_jump->setDefPlayFrame(false, false);
		_ani_jump->setFPS(1);
	}

	_ani_idle->setPlayFrame(5, 9, false, true);
	_ani_idle->start();

	_isOnceAttack = false;

	_attackCount = _attackCount2 = 0;

	return S_OK;
}

void player::release()
{
}

void player::update()
{

	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		if (_state != LEFT_MOVE && !isJumping() && !isFalling())
		{
			_stateBefore = _state;
			_ani_idle->stop();
			_state = LEFT_MOVE;
			_image = IMAGEMANAGER->findImage("run");
			_ani_run->setPlayFrame(0, 7, false, true);
			_ani_run->start();
		}
	}

	if (KEYMANAGER->isStayKeyDown(VK_LEFT) && _x - _hitbox.getWidth() / 2 >= 0)
	{
		if (isJumping())
		{
			_angle = PI / 5 * 3;
			setAnimation(LEFT_JUMP);
		}
		else if (_state != LEFT_IDLE && _isOnGround && !isFalling())
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
		if (_state == LEFT_MOVE && _ani_run->isPlay() && _canMoveLeft) _x -= 3;
	}

	if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
	{
		if (isJumping() || isFalling())
		{
			_stateBefore = LEFT_IDLE;
		}
		if (_state != LEFT_IDLE && _isOnGround)
		{
			_ani_run->stop();
			_stateBefore = _state;
			_state = LEFT_IDLE;
			_image = IMAGEMANAGER->findImage("idle");
			_ani_idle->setPlayFrame(0, 4, false, true);
			_ani_idle->start();
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		if (_state != RIGHT_MOVE && !isJumping() && !isFalling())
		{
			_ani_idle->stop();
			_stateBefore = _state;
			_state = RIGHT_MOVE;
			_image = IMAGEMANAGER->findImage("run");
			_ani_run->setPlayFrame(8, 15, false, true);
			_ani_run->start();
		}
	}

	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _x + _hitbox.getWidth() / 2) //<= _sm->getCurrentWidth())
	{
		if (isJumping()) 
		{ 
			_angle = PI / 5*2; 
			setAnimation(RIGHT_JUMP);
		}
		else if (_state != RIGHT_IDLE && _isOnGround && !isFalling())
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
		if (_state == RIGHT_MOVE && _ani_run->isPlay() && _canMoveRight) _x += 3;
	}

	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
	{
		if (isJumping() || isFalling())
		{
			_stateBefore = RIGHT_IDLE;
		}
		if (_state != RIGHT_IDLE && _isOnGround)
		{
			_ani_run->stop();
			_stateBefore = _state;
			_state = RIGHT_IDLE;
			_image = IMAGEMANAGER->findImage("idle");
			_ani_idle->setPlayFrame(5, 9, false, true);
			_ani_idle->start();
		}
	}

	if (KEYMANAGER->isOnceKeyDown('A'))
	{
		if (!_canMoveRight) _x -= 2;
		else if (!_canMoveLeft) _x += 2;

		if ((isMoveLeft() || _state == LEFT_IDLE) && !isJumping() && !isFalling() && _isOnGround)
		{
			_stateBefore = _state;
			setAnimation(LEFT_JUMP);
			_gravity = 0;
			_isOnGround = false;
			_angle = PI / 2;
		}
		else if ((isMoveRight() || _state == RIGHT_IDLE) && !isJumping() && !isFalling() && _isOnGround)
		{
			_stateBefore = _state;
			setAnimation(RIGHT_JUMP);
			_gravity = 0;
			_isOnGround = false;
			_angle = PI / 2;
		}
	}

	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		if (_state != LEFT_ATTACK && _state != RIGHT_ATTACK)
		{
			if (isMoveLeft() || _state == LEFT_IDLE)
			{
				_stateBefore = _state;
				_state = LEFT_ATTACK;
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

			}
			else
			{
				_stateBefore = _state;
				_state = RIGHT_ATTACK;
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

			}
			_ani_attack->start();

			_isOnceAttack = !_isOnceAttack;
		}
		_isAttacking = true;
	}

	if (KEYMANAGER->isStayKeyDown('S'))
	{
		if (_isAttacking)
		{

		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		//image->alphaRedFrameRender(getMemDC(), _x - _width / 2, _y - _height / 2, 0, 0, 100);
		//IMAGEMANAGER->findImage("idle")->alphaRedRender(getMemDC(), 122);
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
			if (_state == LEFT_ATTACK)
			{
				_stateBefore = _state;
				_state = LEFT_IDLE;
				_image = IMAGEMANAGER->findImage("idle");
				_ani_idle->setPlayFrame(0, 4, false, true);
				_ani_idle->start();
			}
			else
			{
				_stateBefore = _state;
				_state = RIGHT_IDLE;
				_image = IMAGEMANAGER->findImage("idle");
				_ani_idle->setPlayFrame(5, 9, false, true);
				_ani_idle->start();
			}
		}
		break;
	case LEFT_JUMP: case RIGHT_JUMP:
		_ani_jump->frameUpdate(TIMEMANAGER->getElapsedTime() * 2);

		if (_gravity > _speed)
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
	}

	if ((isJumping() || isFalling()) && !_isOnGround)
	{
		_x += cosf(_angle) * _speed;
		_y += -sinf(_angle) * _speed + _gravity;
		_gravity += 0.09f;
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

	cout << "모모 상태: " << _state << " 이전: " << _stateBefore << endl;
}

void player::render()
{
	if (DEBUG)
	{
		_hitbox.render(getMemDC());
		//RectangleMakeCenter(getMemDC(), _x, _y, 100, 100);
		Rectangle(getMemDC(), _probeXL + 10, _probeY - _height / 2, _probeXR - 10, _probeY);
		RectangleMakeCenter(getMemDC(), _probeXL, _y + _height / 4, 5, 5);
		RectangleMakeCenter(getMemDC(), _probeXR, _y + _height / 4, 5, 5);
	}

	switch (_state)
	{
	case LEFT_IDLE: case RIGHT_IDLE:
		_image->aniRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_idle);
		//_image->aniFrameRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_idle, RGB(255, 0, 255));
		break;
	case LEFT_MOVE: case RIGHT_MOVE:
		_image->aniRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_run);
		break;
	case LEFT_ATTACK: case RIGHT_ATTACK:
		_image->aniRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_attack);
		break;
	case LEFT_JUMP: case RIGHT_JUMP: case LEFT_FALL:  case RIGHT_FALL:
		_image->aniRender(getMemDC(), _x - _width / 2, _y - _height / 2, _ani_jump);
		break;
	}

	IMAGEMANAGER->findImage("run")->alphaRedFrameRender(getMemDC(), 0, 0, 0, 1, 122);
	//IMAGEMANAGER->findImage("run")->alphaRedFrameRender(getMemDC(), 300, 0, 1, 1, 122);
	//IMAGEMANAGER->findImage("idle")->alphaRedRender(getMemDC(), 122);
}

void player::setAnimation(PLAYERSTATE state)
{
	_ani_idle->stop();
	_ani_run->stop();
	_ani_attack->stop();
	_ani_jump->stop();

	switch (state)
	{
	case LEFT_IDLE:
		_state = LEFT_IDLE;
		_ani_idle->setPlayFrame(0, 4, false, true);
		_ani_idle->start();
		_image = IMAGEMANAGER->findImage("idle");
		break;
	case RIGHT_IDLE:
		_state = RIGHT_IDLE;
		_ani_idle->setPlayFrame(5, 9, false, true);
		_ani_idle->start();
		_image = IMAGEMANAGER->findImage("idle");
		break;
	case LEFT_MOVE:
		_state = LEFT_MOVE;
		_ani_run->setPlayFrame(0, 7, false, true);
		_ani_run->start();
		_image = IMAGEMANAGER->findImage("run");
		break;
	case RIGHT_MOVE:
		_state = RIGHT_MOVE;
		_ani_run->setPlayFrame(8, 15, false, true);
		_ani_run->start();
		_image = IMAGEMANAGER->findImage("run");
		break;
	case LEFT_ATTACK:
		break;
	case RIGHT_ATTACK:
		break;
	case LEFT_ATTACKB:
		break;
	case RIGHT_ATTACKB:
		break;
	case LEFT_ATTACKC:
		break;
	case RIGHT_ATTACKC:
		break;
	case LEFT_JUMP:
		_state = LEFT_JUMP;
		_ani_jump->setPlayFrame(0, 1, false, true);
		_ani_jump->start();
		_image = IMAGEMANAGER->findImage("jump");
		break;
	case RIGHT_JUMP:
		_state = RIGHT_JUMP;
		_ani_jump->setPlayFrame(11, 12, false, true);
		_ani_jump->start();
		_image = IMAGEMANAGER->findImage("jump");
		break;
	case LEFT_FALL:
		break;
	case RIGHT_FALL:
		break;
	case LEFT_DEAD:
		break;
	case RIGHT_DEAD:
		break;
	}
}
