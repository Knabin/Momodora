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

void enemy::checkCollision()
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

	_isOnceAttacked = false;

	_hp = 2;
	return S_OK;
}

void oko::release()
{
	_ani_round->stop();
	SAFE_DELETE(_ani_round);
}

void oko::update()
{
	if (_hp <= 0) return;
	move();
	draw();
	checkCollision();

	if(_ani_round->isPlay())
		_ani_round->frameUpdate(TIMEMANAGER->getElapsedTime() * 30);
}

void oko::render()
{
	if (_hp <= 0) return;
	if(DEBUG) _rc.render(getMemDC());
	_imageRound->aniRender(getMemDC(), _rc.left - _rc.getWidth() + 10, _rc.top - _rc.getHeight() + 10, _ani_round);
	if(!_isOnceAttacked) _image->render(getMemDC(), _rc.left, _rc.top);
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

void oko::checkCollision()
{
	// 플레이어를 공격함
	// oko는 본체 충돌처리만 진행
	if (!_player->getIsAttacked() && isCollision(_player->getHitbox(), _rc))
	{
		cout << "충돌" << endl;
		_player->setIsAttacked(true);
		_player->setHP(_player->getHP() - 1);
	}

	// 플레이어에게 공격당함
	// 플레이어 공격 범위에 들어오면 하나 까이고

	if (_player->isAttacking() && !_player->getIsCheckAttack() && isCollision(_player->getAttackRc(), _rc))
	{
		_player->setIsCheckAttack(true);
		_hp -= 1;
		cout << "공격, " << _hp << endl;
		if (_hp == 1)
		{
			_isOnceAttacked = true;
		}
	}

	if (_player->checkBulletCollision(_rc))
	{
		_hp -= 1;
		cout << "원거리 공격, " << _hp << endl;
		if (_hp == 1)
		{
			_isOnceAttacked = true;
		}
	}
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

	_rcHit.set(0, 0, 50, 50);

	_rcCheck.set(0, 0, 500, 250);
	_rcCheck.setCenterPos(_x + _width / 2, _y + _height / 2);

	_rcAttack.set(0, 0, 20, 40);

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

	_hp = 3;

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
	case ENEMYDIRECTION::LEFT_IDLE:
	case ENEMYDIRECTION::RIGHT_IDLE:
	case ENEMYDIRECTION::LEFT_MOVE:
	case ENEMYDIRECTION::RIGHT_MOVE:
		if (_ani_run->isPlay()) _ani_run->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		break;
	case ENEMYDIRECTION::LEFT_ATTACK:
	case ENEMYDIRECTION::RIGHT_ATTACK:
		if (_ani_attack->isPlay()) _ani_attack->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		break;
	}
		
}

void monkey::render()
{
	//_rc.render(getMemDC());
	_rcHit.render(getMemDC());
	_rcAttack.render(getMemDC());
	if (_hp <= 0) return;
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
		if (_player->getX() < _x + 50)
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
		if (_player->getHitbox().right < _x)
		{
			if (_direction != ENEMYDIRECTION::LEFT_MOVE)
			{
				_direction = ENEMYDIRECTION::LEFT_MOVE;
				_image = IMAGEMANAGER->findImage("몬스터2");
				_ani_run->setPlayFrame(0, 5, false, true);
				_ani_run->start();
			}
		}
		else if (_player->getHitbox().left > _x + _width)
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
		_ani_attack->stop();
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

void monkey::checkCollision()
{
	// 플레이어를 공격함
	// monkey 본체 충돌처리
	// monkey 공격 rect와 충돌처리
	if (!_player->getIsAttacked())
	{
		if (isCollision(_player->getHitbox(), _rcAttack) || isCollision(_player->getHitbox(), _rcHit))
		{
			cout << "충돌" << endl;
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
		cout << "공격, " << _hp << endl;
	}

	if (_player->checkBulletCollision(_rc))
	{
		_hp -= 1;
		cout << "원거리 공격, " << _hp << endl;
	}
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
	for ( ; i<= dest; ++i)
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

	_rc.set(0, 0, 50, 60);
	_rc.setCenterPos(_x + _width / 2, _y + _height / 2 + 20);

	_rcCheck.set(0, 0, 600, 300);
	_rcCheck.setCenterPos(_x + _width / 2, _y + _height / 2);

	_rcBullet.set(0, 0, _bullet->getWidth(), _bullet->getHeight());

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

		_rcBullet.setLeftTopPos(_bulletX, _bulletY);
	}
}

void bakman::draw()
{
	if (_isFire)
	{
		_rcBullet.render(getMemDC());
		_bullet->render(getMemDC(), _bulletX, _bulletY);
	}

	if (_hp <= 0) return;
	_rc.render(getMemDC());
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
			cout << "충돌" << endl;
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


// ===============================================================

HRESULT prim::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("보스1 시작");
	
	_ani_start = new animation;
	_ani_start->init(_image->getWidth(), _image->getHeight(),
		_image->getFrameWidth(), _image->getFrameHeight());
	_ani_start->setFPS(1);
	_ani_start->setDefPlayFrame(false, false);

	_ani_attack = new animation;
	_ani_attack->init(IMAGEMANAGER->findImage("보스1 공격")->getWidth(), IMAGEMANAGER->findImage("보스1 공격")->getHeight(),
		_image->getFrameWidth(), _image->getFrameHeight());
	_ani_attack->setFPS(1);
	_ani_attack->setDefPlayFrame(false, false);

	_ani_run = new animation;
	_ani_run->init(IMAGEMANAGER->findImage("보스1 이동")->getWidth(), IMAGEMANAGER->findImage("보스1 이동")->getHeight(),
		_image->getFrameWidth(), _image->getFrameHeight());
	_ani_run->setFPS(1);
	_ani_run->setDefPlayFrame(false, false);

	_ani_angry = new animation;
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

void prim::release()
{
}

void prim::update()
{
	if (_hp <= 0) return;
	start();
	move();
	checkCollision();
	draw();

	_rc.setCenterPos(_x + _image->getFrameWidth() / 2, _y + _image->getFrameHeight() - 25);
}

void prim::render()
{
	_rc.render(getMemDC());
	_rcAttack.render(getMemDC());

	if (_hp <= 0)
	{
		IMAGEMANAGER->findImage("보스1 패배")->render(getMemDC(), _x, _y);
	}
	else
	{
		switch (_direction)
		{
		case ENEMYDIRECTION::LEFT_IDLE:
		case ENEMYDIRECTION::RIGHT_IDLE:
			_image->aniRender(getMemDC(), _x, _y, _ani_start);
			break;
		case ENEMYDIRECTION::LEFT_MOVE:
		case ENEMYDIRECTION::RIGHT_MOVE:
			_image->aniRender(getMemDC(), _x, _y, _ani_run);
			break;
		case ENEMYDIRECTION::LEFT_ATTACK:
		case ENEMYDIRECTION::RIGHT_ATTACK:
			_image->aniRender(getMemDC(), _x, _y, _ani_attack);
			break;
		case ENEMYDIRECTION::LEFT_ACTION:
		case ENEMYDIRECTION::RIGHT_ACTION:
			_image->aniRender(getMemDC(), _x, _y, _ani_angry);
			break;
		}
	}
}

void prim::attack()
{
}

void prim::checkPlayer()
{
	if (_direction != ENEMYDIRECTION::LEFT_ATTACK && _direction != ENEMYDIRECTION::RIGHT_ATTACK && _direction != ENEMYDIRECTION::LEFT_ACTION && _direction != ENEMYDIRECTION::RIGHT_ACTION)
	{
		if (getDistance(_player->getX(), 0, _x + _image->getFrameWidth() / 2, 0) <= 50)
		{
			_direction = static_cast<int>(_direction) % 2 == 0 ? ENEMYDIRECTION::LEFT_ATTACK : ENEMYDIRECTION::RIGHT_ATTACK;
			_image = IMAGEMANAGER->findImage("보스1 공격");

			if (static_cast<int>(_direction) % 2 == 0)
			{
				_ani_attack->setPlayFrame(0, 19, false);
				_rcAttack.setLeftTopPos(_x + 30, _y + 80);
			}
			else
			{
				_ani_attack->setPlayFrame(20, 39, false);
				_rcAttack.setLeftTopPos(_x + _image->getFrameWidth() / 2 + 40 , _y + 80);
			}

			_ani_attack->start();
		}

	}
}

void prim::move()
{
	if (_direction == ENEMYDIRECTION::LEFT_MOVE || _direction == ENEMYDIRECTION::RIGHT_MOVE)
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

void prim::draw()
{
	switch (_direction)
	{
	case ENEMYDIRECTION::LEFT_IDLE:
	case ENEMYDIRECTION::RIGHT_IDLE:
		_ani_start->frameUpdate(TIMEMANAGER->getElapsedTime() * 0.5);

		if (!_ani_start->isPlay())
		{
			_image = IMAGEMANAGER->findImage("보스1 이동");
			if (_player->getX() < _x + _image->getFrameWidth() / 2)
			{
				_direction = ENEMYDIRECTION::LEFT_MOVE;
				_ani_run->setPlayFrame(0, 7, true);
			}
			else
			{
				_direction = ENEMYDIRECTION::RIGHT_MOVE;
				_ani_run->setPlayFrame(8, 15, true);
			}
			_ani_run->start();
		}
		break;
	case ENEMYDIRECTION::LEFT_MOVE:
	case ENEMYDIRECTION::RIGHT_MOVE:
		_ani_run->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		break;
	case ENEMYDIRECTION::LEFT_ATTACK:
	case ENEMYDIRECTION::RIGHT_ATTACK:
		_ani_attack->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);

		if (!_ani_attack->isPlay())
		{
			_image = IMAGEMANAGER->findImage("보스1 화남");
			if (static_cast<int>(_direction) % 2 == 0) 
			{
				_direction = ENEMYDIRECTION::LEFT_ACTION;
				_ani_angry->setPlayFrame(0, 14, false);
			}
			else
			{
				_direction = ENEMYDIRECTION::RIGHT_ACTION;
				_ani_angry->setPlayFrame(15, 29, false);
			}
			_ani_angry->start();
		}
		break;
	case ENEMYDIRECTION::LEFT_ACTION:
	case ENEMYDIRECTION::RIGHT_ACTION:
		_ani_angry->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);

		if (!_ani_angry->isPlay())
		{
			_image = IMAGEMANAGER->findImage("보스1 이동");
			if (_player->getX() < _x + _image->getFrameWidth() / 2)
			{
				_direction = ENEMYDIRECTION::LEFT_MOVE;
				_ani_run->setPlayFrame(0, 7, true);
			}
			else
			{
				_direction = ENEMYDIRECTION::RIGHT_MOVE;
				_ani_run->setPlayFrame(8, 15, true);
			}
			_ani_run->start();
		}
		break;
	}
}

void prim::checkCollision()
{
	if (!_player->getIsAttacked())
	{
		if (isCollision(_player->getHitbox(), _rc) || 
			(isCollision(_player->getHitbox(), _rcAttack) && (_direction == ENEMYDIRECTION::LEFT_ATTACK ||
				_direction == ENEMYDIRECTION::RIGHT_ATTACK)))
		{
			cout << "충돌" << endl;
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

void prim::start()
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

HRESULT witch::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("보스2 왼쪽");
	_imageRight = IMAGEMANAGER->findImage("보스2 오른쪽");
	_imageLeftAttackBack = IMAGEMANAGER->findImage("보스2 왼쪽 불렛2 배경");

	_x = position.x;
	_xRight = WINSIZEX - _x - 100;
	_y = position.y;
	_yRight = _y - 24;
	
	{
		_leftBullet1[0].image = _leftBullet1[1].image = _leftBullet1[2].image = IMAGEMANAGER->findImage("보스2 왼쪽 불렛");
		_leftBullet1[0].fireX = _leftBullet1[0].x = _leftBullet1[1].fireX = _leftBullet1[1].x = _x + 50;
		_leftBullet1[0].fireY = _leftBullet1[0].y = _leftBullet1[1].fireY = _leftBullet1[1].y = _y;
		_leftBullet1[0].range = _leftBullet1[1].range = 700;
		_leftBullet1[0].isFire = _leftBullet1[1].isFire = false;
		_leftBullet1[0].count = _leftBullet1[1].count = 0.0f;
	}

	for (int i = 0; i < 10; ++i)
	{
		_leftBullet2[i].image = IMAGEMANAGER->findImage("보스2 왼쪽 불렛2");
		_leftBullet2[i].isFire = false;
		_leftBullet2[i].range = 500;
	}
	
	{
		_rightBullet1.image = IMAGEMANAGER->findImage("보스2 오른쪽 불렛");
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

	_ani_idle_left = new animation;
	_ani_idle_left->init(_image->getWidth(), _image->getHeight(),
		_image->getFrameWidth(), _image->getFrameHeight());
	_ani_idle_left->setFPS(1);
	_ani_idle_left->setDefPlayFrame(false, true);
	_ani_idle_left->setPlayFrame(0, 4, false, true);
	_ani_idle_left->start();

	_ani_attack_left = new animation;
	_ani_attack_left->init(IMAGEMANAGER->findImage("보스2 왼쪽 공격")->getWidth(),
		IMAGEMANAGER->findImage("보스2 왼쪽 공격")->getHeight(),
		_image->getFrameWidth(), _image->getFrameHeight());
	_ani_attack_left->setFPS(1);
	_ani_attack_left->setDefPlayFrame(false, false);
	_ani_attack_left->setPlayFrame(0, 9, false, false);

	_ani_idle_right = new animation;
	_ani_idle_right->init(_imageRight->getWidth(), _imageRight->getHeight(),
		_imageRight->getFrameWidth(), _imageRight->getFrameHeight());
	_ani_idle_right->setFPS(1);
	_ani_idle_right->setDefPlayFrame(false, true);
	_ani_idle_right->setPlayFrame(0, 4, false, true);
	_ani_idle_right->start();

	_ani_attack_right = new animation;
	_ani_attack_right->init(IMAGEMANAGER->findImage("보스2 오른쪽 공격")->getWidth(),
		IMAGEMANAGER->findImage("보스2 오른쪽 공격")->getHeight(),
		_imageRight->getFrameWidth(), _imageRight->getFrameHeight());
	_ani_attack_right->setFPS(1);
	_ani_attack_right->setDefPlayFrame(false, false);
	_ani_attack_right->setPlayFrame(0, 17, false, false);

	_isStart = _attackVer = false;
	_attackCount = _leftAttack2Count = 0;

	_direction = ENEMYDIRECTION::LEFT_IDLE;

	_maxHp = 30;
	_hp = _maxHp;

	_alpha = 255;

	return S_OK;
}

void witch::release()
{
}

void witch::update()
{
	start();
	attack();
	move();
	checkCollision();
}

void witch::render()
{
	_rc.render(getMemDC());
	_rcRight.render(getMemDC());
	draw();
}

void witch::attack()
{
	++_attackCount;

	if (_attackCount > 220)
	{
		switch (_direction)
		{
		case ENEMYDIRECTION::LEFT_IDLE:
			_image = IMAGEMANAGER->findImage("보스2 왼쪽 공격");
			_ani_idle_left->stop();
			_ani_attack_left->start();
			_direction = ENEMYDIRECTION::LEFT_ATTACK;
			if (!_attackVer)
			{
				cout << "공격" << endl;
				attackWithLeftBullet(0);
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
		case ENEMYDIRECTION::RIGHT_IDLE:
			_imageRight = IMAGEMANAGER->findImage("보스2 오른쪽 공격");
			_ani_idle_right->stop();
			_ani_attack_right->start();
			_direction = ENEMYDIRECTION::RIGHT_ATTACK;
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
			break;
		case ENEMYDIRECTION::LEFT_ATTACK:
			if (!_ani_attack_left->isPlay())
			{
				_imageRight = IMAGEMANAGER->findImage("보스2 오른쪽");
				_ani_idle_right->start();

				_direction = ENEMYDIRECTION::RIGHT_IDLE;
			}
			break;
		case ENEMYDIRECTION::RIGHT_ATTACK:
			if (!_ani_attack_right->isPlay())
			{
				_image = IMAGEMANAGER->findImage("보스2 왼쪽");
				_ani_idle_left->start();

				_direction = ENEMYDIRECTION::LEFT_IDLE;
			}
			break;
		}
		_attackCount = 0;
	}
}

void witch::checkPlayer()
{
}

void witch::move()
{
	switch (_direction)
	{
	case ENEMYDIRECTION::LEFT_IDLE:
	case ENEMYDIRECTION::RIGHT_IDLE:
		_ani_idle_left->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		_ani_idle_right->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		break;

	case ENEMYDIRECTION::LEFT_ATTACK:
	case ENEMYDIRECTION::RIGHT_ATTACK:
		_ani_attack_left->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		_ani_attack_right->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
		break;
	}

	if (_leftBullet1[0].isFire)
	{
		_leftBullet1[0].count += 0.01f;
		_leftBullet1[0].x = CalculateBezierPoint(_leftBullet1[0].count, _pts[0], _pts[1], _pts[2], _pts[3]).x;
		_leftBullet1[0].y = CalculateBezierPoint(_leftBullet1[0].count, _pts[0], _pts[1], _pts[2], _pts[3]).y;
		_leftBullet1[0].rc = { _leftBullet1[0].x, _leftBullet1[0].y, _leftBullet1[0].x + _leftBullet1[0].image->getWidth(), _leftBullet1[0].y + _leftBullet1[0].image->getHeight() };
		if (_leftBullet1[0].range <= getDistance(_leftBullet1[0].fireX, _leftBullet1[0].fireY, _leftBullet1[0].x, _leftBullet1[0].y))
		{
			_leftBullet1[0].isFire = false;
			_leftBullet1[0].count = 0;
		}

		if (_leftBullet1[0].count >= 0.4f && !_leftBullet1[1].isFire)
		{
			attackWithLeftBullet(1);
		}
	}
	
	if (_leftBullet1[1].isFire)
	{
		_leftBullet1[1].count += 0.005f;
		_leftBullet1[1].x = CalculateBezierPoint(_leftBullet1[1].count, _pts[4], _pts[5], _pts[6], _pts[7]).x;
		_leftBullet1[1].y = CalculateBezierPoint(_leftBullet1[1].count, _pts[4], _pts[5], _pts[6], _pts[7]).y;
		_leftBullet1[1].rc = { _leftBullet1[1].x, _leftBullet1[1].y, _leftBullet1[1].x + _leftBullet1[1].image->getWidth(), _leftBullet1[1].y + _leftBullet1[1].image->getHeight() };
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
			_leftBullet2[i].rc = { _leftBullet2[i].x, _leftBullet2[i].y, _leftBullet2[i].x + _leftBullet2[i].image->getWidth(), _leftBullet2[i].y + _leftBullet2[i].image->getHeight() };
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
			_rightBullet1.rc = { _rightBullet1.x, _rightBullet1.y, _rightBullet1.x + _rightBullet1.image->getWidth(), _rightBullet1.y + _rightBullet1.image->getHeight() };
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

void witch::draw()
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
			case ENEMYDIRECTION::LEFT_IDLE:
			case ENEMYDIRECTION::LEFT_ATTACK:
				if (_ani_idle_left->isPlay()) _ani_idle_left->stop();
				if (_ani_attack_left->isPlay()) _ani_attack_left->stop();
				_image = IMAGEMANAGER->findImage("보스2 왼쪽");
				_image->alphaFrameRender(getMemDC(), _x, _y, 0, 0, _alpha);
				break;
			case ENEMYDIRECTION::RIGHT_IDLE:
			case ENEMYDIRECTION::RIGHT_ATTACK:
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
			case ENEMYDIRECTION::LEFT_IDLE:
				_image->aniRender(getMemDC(), _x, _y, _ani_idle_left);
				break;
			case ENEMYDIRECTION::RIGHT_IDLE:
				_imageRight->aniRender(getMemDC(), _xRight, _yRight, _ani_idle_right);
				break;
			case ENEMYDIRECTION::LEFT_ATTACK:
				_image->aniRender(getMemDC(), _x, _y, _ani_attack_left);
				if (_attackVer)
				{
					++_leftAttack2Count;
					if (_leftAttack2Count > 100)
					{
						for (int i = 0; i < 10; ++i)
						{
							_leftBullet2[i].isFire = true;
						}
					}
					else
						_imageLeftAttackBack->alphaRender(getMemDC(), _leftAttackX, 95, 122);
				}
				break;
			case ENEMYDIRECTION::RIGHT_ATTACK:
				_imageRight->aniRender(getMemDC(), _xRight, _yRight, _ani_attack_right);
				break;
			}
		}
		for (int i = 0; i < 2; ++i)
		{
			if (_leftBullet1[i].isFire)
				_leftBullet1[i].image->render(getMemDC(), _leftBullet1[i].x, _leftBullet1[i].y);
		}

		for (int i = 0; i < 10; ++i)
		{
			if (_leftBullet2[i].isFire)
				_leftBullet2[i].image->render(getMemDC(), _leftBullet2[i].x, _leftBullet2[i].y);
		}

		if (_rightBullet1.isFire)
			_rightBullet1.image->render(getMemDC(), _rightBullet1.x - _rightBullet1.image->getWidth()/ 2, 
				_rightBullet1.y - _rightBullet1.image->getHeight() / 2);

		if (_rightBullet2.isFire)
			EllipseMake(getMemDC(), _rightBullet2.x, _rightBullet2.y, 30, 30);
	}
}

void witch::checkCollision()
{
	if (!_player->getIsAttacked())
	{
		if (static_cast<int>(_direction) % 2 == 0)
		{
			// 왼쪽
			if (isCollision(_player->getHitbox(), _rc))
			{
				cout << "충돌" << endl;
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
						cout << "충돌" << endl;
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
						cout << "충돌" << endl;
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
				cout << "충돌" << endl;
				_player->setIsAttacked(true);
				_player->setHP(_player->getHP() - 1);
			}

			if (!_attackVer)
			{
				if (_rightBullet1.isFire && isCollision(_player->getHitbox(), _rightBullet1.rc))
				{
					cout << "충돌" << endl;
					_player->setIsAttacked(true);
					_player->setHP(_player->getHP() - 1);
				}
			}
			else
			{
				if (_canCheckRight2 && isCollision(MYCIRCLE(_rightBullet2.x + 20, _rightBullet2.y + 20, 100), _player->getHitbox()))
				{
					cout << "충돌" << endl;
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
				cout << "공격, " << _hp << endl;
			}
		}
		else
		{
			if (isCollision(_player->getAttackRc(), _rcRight))
			{
				_player->setIsCheckAttack(true);
				_hp -= 2;
				cout << "공격, " << _hp << endl;
			}
		}
	}
	if (static_cast<int>(_direction) % 2 == 0)
	{
		if (_player->checkBulletCollision(_rc))
		{
			_hp -= 1;
			cout << "원거리 공격, " << _hp << endl;
		}
	}
	else
	{
		if (_player->checkBulletCollision(_rcRight))
		{
			_hp -= 1;
			cout << "원거리 공격, " << _hp << endl;
		}
	}

}

void witch::start()
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

void witch::attackWithLeftBullet(int i)
{
	_leftBullet1[i].x = _leftBullet1[i].fireX;
	_leftBullet1[i].y = _leftBullet1[i].fireY;
	_pts[0 + i*4] = { static_cast<long>(_x), static_cast<long>(_y) };
	_pts[1 + i*4] = { static_cast<long>((_player->getX() + _x) * 0.3), static_cast<long>(_y + 300) };
	_pts[2 + i*4] = { static_cast<long>((_player->getX() + _x) * 0.7), static_cast<long>(_player->getY() - 300) };
	_pts[3 + i*4] = { static_cast<long>(_player->getX()), static_cast<long>(_player->getY()) };
	_leftBullet1[i].isFire = true;
}


HRESULT rell::init(MYPOINT position)
{
	_image = IMAGEMANAGER->findImage("보스3-1 기본");
	_fire = IMAGEMANAGER->findImage("보스3-1 기도 불꽃");
	_snow = IMAGEMANAGER->findImage("보스3-2 눈꽃");
	_snow2 = IMAGEMANAGER->findImage("보스3-2 눈꽃2");

	{
		_ani_idle1 = new animation;
		_ani_idle1->init(_image->getWidth(), _image->getHeight(),
			_image->getFrameWidth(), _image->getFrameHeight());
		_ani_idle1->setFPS(1);
		_ani_idle1->setDefPlayFrame(false, true);
		_ani_idle1->setPlayFrame(0, 4, false, true);
		_ani_idle1->start();

		_ani_ground1 = new animation;
		_ani_ground1->init(IMAGEMANAGER->findImage("보스3-1 땅")->getWidth(), 
			IMAGEMANAGER->findImage("보스3-1 땅")->getHeight(),
			_image->getFrameWidth(), _image->getFrameHeight());
		_ani_ground1->setFPS(1);
		_ani_ground1->setDefPlayFrame(false, false);

		_ani_attack1 = new animation;
		_ani_attack1->init(IMAGEMANAGER->findImage("보스3-1 공격")->getWidth(),
			IMAGEMANAGER->findImage("보스3-1 공격")->getHeight(),
			_image->getFrameWidth(), _image->getFrameHeight());
		_ani_attack1->setFPS(1);
		_ani_attack1->setDefPlayFrame(false, false);

		_ani_pray = new animation;
		_ani_pray->init(IMAGEMANAGER->findImage("보스3-1 공격")->getWidth(),
			IMAGEMANAGER->findImage("보스3-1 공격")->getHeight(),
			_image->getFrameWidth(), _image->getFrameHeight());
		_ani_pray->setFPS(1);
		_ani_pray->setDefPlayFrame(false, true);


		_ani_attack2 = new animation;
		_ani_attack2->init(IMAGEMANAGER->findImage("보스3-2")->getWidth(),
			IMAGEMANAGER->findImage("보스3-2")->getHeight(),
			_image->getFrameWidth(), _image->getFrameHeight());
		_ani_attack2->setFPS(1);
		_ani_attack2->setDefPlayFrame(false, false);


		_ani_idle3 = new animation;
		_ani_idle3->init(IMAGEMANAGER->findImage("보스3-3 기본")->getWidth(),
			IMAGEMANAGER->findImage("보스3-3 기본")->getHeight(),
			IMAGEMANAGER->findImage("보스3-3 기본")->getFrameWidth(),
			IMAGEMANAGER->findImage("보스3-3 기본")->getFrameHeight());
		_ani_idle3->setFPS(1);
		_ani_idle3->setDefPlayFrame(false, true);

		_ani_ground3 = new animation;
		_ani_ground3->init(IMAGEMANAGER->findImage("보스3-3 땅")->getWidth(),
			IMAGEMANAGER->findImage("보스3-3 땅")->getHeight(),
			IMAGEMANAGER->findImage("보스3-3 땅")->getFrameWidth(),
			IMAGEMANAGER->findImage("보스3-3 땅")->getFrameHeight());
		_ani_ground3->setFPS(1);
		_ani_ground3->setDefPlayFrame(false, false);

		_ani_attack3 = new animation;
		_ani_attack3->init(IMAGEMANAGER->findImage("보스3-3 공격")->getWidth(),
			IMAGEMANAGER->findImage("보스3-3 공격")->getHeight(),
			IMAGEMANAGER->findImage("보스3-3 공격")->getFrameWidth(),
			IMAGEMANAGER->findImage("보스3-3 공격")->getFrameHeight());
		_ani_attack3->setFPS(1);
		_ani_attack3->setDefPlayFrame(false, false);

		_ani_effect_fire = new animation;
		_ani_effect_fire->init(_fire->getWidth(),
			_fire->getHeight(),
			_fire->getFrameWidth(),
			_fire->getFrameHeight());
		_ani_effect_fire->setFPS(1);
		_ani_effect_fire->setDefPlayFrame(false, false);

		_ani_snow = new animation;
		_ani_snow->init(_snow2->getWidth(),
			_snow2->getHeight(),
			_snow2->getFrameWidth(),
			_snow2->getFrameHeight());
		_ani_snow->setFPS(1);
		_ani_snow->setDefPlayFrame(false, true);
		_ani_snow->setPlayFrame(0, 1, false, true);
	}

	_state = RELLSTATE::LEFT_IDLE;
	_mode = RELLMODE::MODE1;

	_x = position.x;
	_y = position.y;
	_yUp = _y - 160;

	_rc.set(0, 0, 50, 55);
	_rc.setCenterPos(_x + _image->getFrameWidth() / 2, _y + _image->getFrameHeight() / 2 + 20);

	_rcCheck.set(0, 0, 400, 400);
	_rcCheck.setCenterPos(_x + _image->getFrameWidth() / 2, _y + _image->getFrameHeight() / 2 - 150);

	_rcCheckAttack.set(0, 0, 200, 100);
	_rcCheckAttack.setCenterPos(_x + _image->getFrameWidth() / 2, _y + _image->getFrameHeight() / 2);

	_attackVer = false;
	_canPray = true;

	_attackCount = _prayCount = 0;

	_maxHp = 60;
	_hp = _maxHp;

	{
		for (int i = 0; i < 3; ++i)
		{
			_bullet1[i].image = IMAGEMANAGER->findImage("보스3 불렛");
			_bullet1[i].fireX = _bullet1[i].x = _x + _image->getWidth() / 2;
			_bullet1[i].fireY = _bullet1[i].y = _y + _image->getHeight() / 2;
			_bullet1[i].angle = PI * 0.5f;
			_bullet1[i].speed = 7.0f;
			_bullet1[i].range = 700;
		}

		for (int i = 0; i < 20; ++i)
		{
			_bullet2[i].image = IMAGEMANAGER->findImage("보스3 불렛");
			_bullet2[i].fireX = _bullet2[i].x = _x + _image->getWidth() / 2;
			_bullet2[i].fireY = _bullet2[i].y = _y + _image->getHeight() / 2;
			_bullet2[i].angle = PI * 0.5f;
			_bullet2[i].speed = 1.0f;
			_bullet2[i].range = 150;
			_bullet2[i].count = 0;
		}

		for (int i = 0; i < 3; ++i)
		{
			_ground[i].image = IMAGEMANAGER->findImage("보스3-1 땅");
			_ground[i].x = _x + _image->getWidth() / 2;
			_ground[i].y = _y + _image->getHeight() / 2;
			_ground[i].rc.set(0, 0, 48, 10);
		}
	}

	return S_OK;
}

void rell::release()
{
}

void rell::update()
{
	move();
	attack();
	checkCollision();

	cout << "상태: " << static_cast<int>(_state) << endl;
}

void rell::render()
{
	_rc.render(getMemDC());
	_ground[0].rc.render(getMemDC());
	draw();
}

void rell::attack()
{
	if (_mode == RELLMODE::MODE1)
	{
		++_attackCount;
		if (_attackCount > 120)
		{
			switch (_state)
			{
			case rell::RELLSTATE::LEFT_IDLE:
			case rell::RELLSTATE::RIGHT_IDLE:
				if (_canPray && _hp <= _maxHp * 0.5)
				{
					_image = IMAGEMANAGER->findImage("보스3-1 기도");
					_prayCount = 0;
					_rcPray.set(0, 0, 300, 400);
					_rcPray.setCenterPos(_x + _image->getFrameWidth() / 2, _y + _image->getFrameHeight() - 200);
					_state = RELLSTATE::PRAY;
					_ani_pray->setPlayFrame(0, 4, false, true);
					_ani_pray->start();
					break;
				}
				if (!_attackVer)
				{
					_image = IMAGEMANAGER->findImage("보스3-1 공격");
					if (changeDirectionToLeft())
					{
						_state = RELLSTATE::LEFT_ATTACK;
						_ani_attack1->setPlayFrame(0, 9);
					}
					else
					{
						_state = RELLSTATE::RIGHT_ATTACK;
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
				}
				else
				{
					// 땅에서 소환
					_image = IMAGEMANAGER->findImage("보스3-1 땅");
					if (changeDirectionToLeft())
					{
						_state = RELLSTATE::LEFT_GROUND;
						_ground[0].image = IMAGEMANAGER->findImage("보스3-1 땅 왼쪽");
						_ani_ground1->setPlayFrame(0, 11);
					}
					else
					{
						_state = RELLSTATE::RIGHT_GROUND;
						_ground[0].image = IMAGEMANAGER->findImage("보스3-1 땅 오른쪽");
						_ani_ground1->setPlayFrame(12, 21);
					}
					_ani_ground1->start();

					_ground[0].x = _player->getX();
					_ground[0].rc.setCenterPos(_ground[0].x, _ground[0].y);
				}
				_ani_idle1->stop();
				break;
			case rell::RELLSTATE::LEFT_GROUND:
			case rell::RELLSTATE::RIGHT_GROUND:
				if (!_ani_ground1->isPlay())
				{
					_image = IMAGEMANAGER->findImage("보스3-1 기본");
					if (changeDirectionToLeft())
					{
						_state = RELLSTATE::LEFT_IDLE;
						_ani_idle1->setPlayFrame(0, 4);
					}
					else
					{
						_state = RELLSTATE::RIGHT_IDLE;
						_ani_idle1->setDefPlayFrame(5, 7);
					}
					_ani_idle1->start();
					_attackVer = !_attackVer;
				}
				break;
			case rell::RELLSTATE::LEFT_ATTACK:
			case rell::RELLSTATE::RIGHT_ATTACK:
				if (!_ani_attack1->isPlay())
				{
					_image = IMAGEMANAGER->findImage("보스3-1 기본");
					if (changeDirectionToLeft())
					{
						_state = RELLSTATE::LEFT_IDLE;
						_ani_idle1->setPlayFrame(0, 4);
					}
					else
					{
						_state = RELLSTATE::RIGHT_IDLE;
						_ani_idle1->setDefPlayFrame(5, 7);
					}
					_ani_idle1->start();
					_attackVer = !_attackVer;
				}
				break;
			case rell::RELLSTATE::PRAY:
				++_prayCount;
				_hp += 7;
				if (_prayCount > 3)
				{
					_image = IMAGEMANAGER->findImage("보스3-1 기본");
					if (changeDirectionToLeft())
					{
						_state = RELLSTATE::LEFT_IDLE;
						_ani_idle1->setPlayFrame(0, 4);
					}
					else
					{
						_state = RELLSTATE::RIGHT_IDLE;
						_ani_idle1->setDefPlayFrame(5, 7);
					}
					_ani_effect_fire->setPlayFrame(0, 9, false, false);
					_ani_effect_fire->start();
					_ani_idle1->start();
					_ani_pray->stop();
					_prayCount = 0;
					_canPray = false;
				}
				break;
			}

			_attackCount = 0;
		}
	}
	else if (_mode == RELLMODE::MODE2)
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
			case rell::RELLSTATE::LEFT_IDLE:
			case rell::RELLSTATE::RIGHT_IDLE:
				checkPlayer();
				break;
			case rell::RELLSTATE::LEFT_GROUND:
			case rell::RELLSTATE::RIGHT_GROUND:
				if (!_ani_ground3->isPlay())
				{
					_image = IMAGEMANAGER->findImage("보스3-3 기본");
					if (changeDirectionToLeft())
					{
						_state = RELLSTATE::LEFT_IDLE;
						_ani_idle3->setPlayFrame(0, 4, false, true);
					}
					else
					{
						_state = RELLSTATE::RIGHT_IDLE;
						_ani_idle3->setPlayFrame(5, 9, false, true);
					}
					_ani_idle3->start();
				}
				break;
			case rell::RELLSTATE::LEFT_ATTACK:
			case rell::RELLSTATE::RIGHT_ATTACK:
				if (!_ani_attack3->isPlay())
				{
					_image = IMAGEMANAGER->findImage("보스3-3 기본");
					if (changeDirectionToLeft())
					{
						_state = RELLSTATE::LEFT_IDLE;
						_ani_idle3->setPlayFrame(0, 4, false, true);
					}
					else
					{
						_state = RELLSTATE::RIGHT_IDLE;
						_ani_idle3->setPlayFrame(5, 9, false, true);
					}
					_ani_idle3->start();
				}
				break;
			case rell::RELLSTATE::DEAD:
				break;
			}
			_attackCount = 0;
		}
	}
}

void rell::checkPlayer()
{
	if (isCollision(_player->getHitbox(), _rcCheckAttack))
	{
		_image = IMAGEMANAGER->findImage("보스3-3 공격");
		// 공격 범위
		if (changeDirectionToLeft())
		{
			// 왼쪽으로
			_state = RELLSTATE::LEFT_ATTACK;
			_ani_attack3->setPlayFrame(0, 6);
		}
		else
		{
			// 오른쪽으로
			_state = RELLSTATE::RIGHT_ATTACK;
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
			_state = RELLSTATE::LEFT_GROUND;
			_ani_ground3->setPlayFrame(0, 11);
		}
		else
		{
			// 오른쪽으로
			_state = RELLSTATE::RIGHT_GROUND;
			_ani_ground3->setPlayFrame(12, 23);
		}
		_ani_ground3->start();
		_groundCount = 0;
	}
}

void rell::move()
{
	if (_mode == RELLMODE::MODE1)
	{
		switch (_state)
		{
		case rell::RELLSTATE::LEFT_IDLE:
		case rell::RELLSTATE::RIGHT_IDLE:
			_ani_idle1->frameUpdate(TIMEMANAGER->getElapsedTime() * 5);
			break;
		case rell::RELLSTATE::LEFT_GROUND:
		case rell::RELLSTATE::RIGHT_GROUND:
			_ani_ground1->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);

			++_ground[0].count;
			if (_ground[0].count >= 50)
				_ground[0].isFire = true;
			break;
		case rell::RELLSTATE::LEFT_ATTACK:
		case rell::RELLSTATE::RIGHT_ATTACK:
			_ani_attack1->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
			break;
		case rell::RELLSTATE::PRAY:
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
			_bullet1[i].rc = { _bullet1[i].x, _bullet1[i].y, _bullet1[i].x + _bullet1[i].image->getWidth(), _bullet1[i].y + _bullet1[i].image->getHeight() };
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
	else if (_mode == RELLMODE::MODE2)
	{
		switch (_state)
		{
		case rell::RELLSTATE::AIR:
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
			
			_bullet2[i].rc = { _bullet2[i].x, _bullet2[i].y, _bullet2[i].x + _bullet2[i].image->getWidth(), _bullet2[i].y + _bullet2[i].image->getHeight() };
		}
	}
	else
	{
		switch (_state)
		{
		case rell::RELLSTATE::LEFT_IDLE:
		case rell::RELLSTATE::RIGHT_IDLE:
			_ani_idle3->frameUpdate(TIMEMANAGER->getElapsedTime() * 5);
			break;
		case rell::RELLSTATE::LEFT_GROUND:
		case rell::RELLSTATE::RIGHT_GROUND:
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
		case rell::RELLSTATE::LEFT_ATTACK:
		case rell::RELLSTATE::RIGHT_ATTACK:
			_ani_attack3->frameUpdate(TIMEMANAGER->getElapsedTime() * 15);
			break;
		case rell::RELLSTATE::DEAD:
			break;
		}
	}	
}

void rell::draw()
{
	if (_mode == RELLMODE::MODE1)
	{
		switch (_state)
		{
		case rell::RELLSTATE::LEFT_IDLE:
		case rell::RELLSTATE::RIGHT_IDLE:
			_image->aniRender(getMemDC(), _x, _y, _ani_idle1);
			break;
		case rell::RELLSTATE::LEFT_GROUND:
		case rell::RELLSTATE::RIGHT_GROUND:
			_image->aniRender(getMemDC(), _x, _y, _ani_ground1);
			break;
		case rell::RELLSTATE::LEFT_ATTACK:
		case rell::RELLSTATE::RIGHT_ATTACK:
			_image->aniRender(getMemDC(), _x, _y, _ani_attack1);
			break;
		case rell::RELLSTATE::PRAY:
			_rcPray.render(getMemDC());
			_image->aniRender(getMemDC(), _x, _y, _ani_pray);
			break;
		}

		if (_ani_effect_fire->isPlay()) 
			_fire->aniRender(getMemDC(), _rcPray.left - 160, _rcPray.top - 130, _ani_effect_fire);

		for (int i = 0; i < 3; ++i)
		{
			if (!_bullet1[i].isFire) continue;
			_bullet1[i].rc.render(getMemDC());
			_bullet1[i].image->alphaRender(getMemDC(), _bullet1[i].x, _bullet1[i].y, 180);
		}

		if (_ground[0].isFire)
			_ground[0].image->render(getMemDC(), _ground[0].x - _ground[0].image->getWidth() / 2, _ground[0].y - _ground[0].image->getHeight());
	}
	else if (_mode == RELLMODE::MODE2)
	{
		switch (_state)
		{
		case rell::RELLSTATE::AIR:
			_snow2->aniAlphaRender(getMemDC(), _x - _snow2->getFrameWidth() / 2 + 48, _yUp - _snow2->getFrameHeight() / 2 + 50, _ani_snow, 100);
			_snow->render(getMemDC(), _x - _snow->getWidth() / 2 + 48, _yUp - _snow->getHeight() /2 + 50);
			_image->aniRender(getMemDC(), _x, _yUp, _ani_attack2);
			break;
		}

		for (int i = 0; i < 8; ++i)
		{
			if (!_bullet2[i].isFire) continue;
			_bullet2[i].rc.render(getMemDC());
			_bullet2[i].image->alphaRender(getMemDC(), _bullet2[i].x, _bullet2[i].y, 100);
		}
	}
	else
	{
		_rcCheckAttack.render(getMemDC());
		_rc.render(getMemDC());

		switch (_state)
		{
		case rell::RELLSTATE::LEFT_IDLE:
		case rell::RELLSTATE::RIGHT_IDLE:
			_image->aniRender(getMemDC(), _xLarge, _yLarge, _ani_idle3);
			break;
		case rell::RELLSTATE::LEFT_GROUND:
		case rell::RELLSTATE::RIGHT_GROUND:
			_image->aniRender(getMemDC(), _xLarge, _yLarge, _ani_ground3);
			break;
		case rell::RELLSTATE::LEFT_ATTACK:
		case rell::RELLSTATE::RIGHT_ATTACK:
			_image->aniRender(getMemDC(), _xLarge, _yLarge, _ani_attack3);
			break;
		case rell::RELLSTATE::DEAD:
			_image->render(getMemDC(), _x, _y);
			break;
		}

		for (int i = 0; i < 3; ++i)
		{
			if (!_ground[i].isFire) continue;
			_ground[i].image->render(getMemDC(), _ground[i].x - _ground[i].image->getWidth() / 2, _ground[i].y - _ground[i].image->getHeight());
			_ground[i].rc.render(getMemDC());
		}
	}
}

void rell::checkCollision()
{
	if (!_player->getIsAttacked())
	{
		if (isCollision(_player->getHitbox(), _rc))
		{
			cout << "충돌" << endl;
			_player->setIsAttacked(true);
			_player->setHP(_player->getHP() - 1);
		}

		if (_mode == RELLMODE::MODE1)
		{
			if (!_attackVer)
			{
				for (int i = 0; i < 3; ++i)
				{
					if (!_bullet1[i].isFire) continue;
					if (isCollision(_player->getHitbox(), _bullet1[i].rc))
					{
						cout << "충돌" << endl;
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
					cout << "충돌" << endl;
					_player->setIsAttacked(true);
					_player->setHP(_player->getHP() - 1);
				}
			}
			if (_ani_effect_fire->isPlay() && isCollision(_player->getHitbox(), _rcPray))
			{
				cout << "충돌" << endl;
				_player->setIsAttacked(true);
				_player->setHP(_player->getHP() - 2);
			}
		}
		else if (_mode == RELLMODE::MODE2)
		{
			for (int i = 0; i < 8; ++i)
			{
				if (!_bullet2[i].isFire) continue;
				if (isCollision(_player->getHitbox(), _bullet2[i].rc))
				{
					cout << "충돌" << endl;
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
					cout << "충돌" << endl;
					_player->setIsAttacked(true);
					_player->setHP(_player->getHP() - 1);
					break;
				}
			}

			if (isCollision(_player->getHitbox(), _rcCheck))
			{
				if (_state == RELLSTATE::LEFT_ATTACK)
				{
					if (changeDirectionToLeft())
					{
						cout << "충돌" << endl;
						_player->setIsAttacked(true);
						_player->setHP(_player->getHP() - 1);
					}
				}
				else if (_state == RELLSTATE::RIGHT_ATTACK)
				{
					if (!changeDirectionToLeft())
					{
						cout << "충돌" << endl;
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

bool rell::changeDirectionToLeft()
{
	// 만약 _rc(보스 충돌용).left보다 플레이어 히트박스 right가 작다면 <- 이쪽 방향
	if (_player->getHitbox().right < _rc.left)
	{
		return true;
	}
	return false;
}

void rell::changeMode()
{
	if (_hp <= 0)
	{
		switch (_mode)
		{
		case RELLMODE::MODE1:
			_mode = RELLMODE::MODE2;
			_state = RELLSTATE::AIR;
			_image = IMAGEMANAGER->findImage("보스3-2");
			_ani_attack2->setPlayFrame(0, 4, false, true);
			_ani_attack2->start();
			_ani_snow->start();

			_yUp = _y - 160;

			_rc.setCenterPos(_x + _image->getFrameWidth() / 2, _yUp + _image->getFrameHeight() / 2 + 20);

			for (int i = 0; i < 8; ++i)
			{
				_bullet2[i].isFire = false;
				_bullet2[i].x = _bullet2[i].fireX = _x + _image->getFrameWidth() / 2 - _bullet2[i].image->getWidth() / 2;
				_bullet2[i].y = _bullet2[i].fireY = _yUp + _bullet2[i].image->getHeight() / 2 + 20;
				_bullet2[i].angle = PI2 * (0.125 * i);
			}
			_maxHp = 20;
			_hp = 20;
			break;
		case RELLMODE::MODE2:
			_mode = RELLMODE::MODE3;
			_state = RELLSTATE::LEFT_IDLE;
			_image = IMAGEMANAGER->findImage("보스3-3 기본");
			_ani_idle3->setPlayFrame(0, 4, false, true);
			_ani_idle3->start();

			_xLarge = _x - _image->getFrameWidth() / 2 + IMAGEMANAGER->findImage("보스3-1 기본")->getFrameWidth() / 2;
			_yLarge = _y - _image->getFrameHeight() / 2 + IMAGEMANAGER->findImage("보스3-1 기본")->getFrameHeight() / 2;

			_rc.setCenterPos(_x + IMAGEMANAGER->findImage("보스3-1 기본")->getFrameWidth() / 2, _y + IMAGEMANAGER->findImage("보스3-1 기본")->getFrameHeight() / 2 + 20);
			_rcCheck.setCenterPos(_x + IMAGEMANAGER->findImage("보스3-1 기본")->getFrameWidth() / 2, _y + IMAGEMANAGER->findImage("보스3-1 기본")->getFrameHeight() / 2 + 20);
			_rcCheckAttack.setCenterPos(_x + IMAGEMANAGER->findImage("보스3-1 기본")->getFrameWidth() / 2, _y + IMAGEMANAGER->findImage("보스3-1 기본")->getFrameHeight() / 2 + 20);

			_maxHp = 40;
			_hp = 40;
			_attackCount = 0;

			for (int i = 0; i < 3; ++i)
			{
				_ground[i].image = IMAGEMANAGER->findImage("보스3-3 땅 공격");
				_ground[i].x = _xLarge + _image->getWidth() / 2;
				_ground[i].y = _yLarge + _image->getHeight() / 2 - 80;
				_ground[i].count = 15 * (i + 1);
				_ground[i].rc.set(0, 0, 48, 10);
			}
			break;
		case RELLMODE::MODE3:
			_state = RELLSTATE::DEAD;
			_image = IMAGEMANAGER->findImage("보스3 죽음");
			break;
		}
	}
}
