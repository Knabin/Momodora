#include "stdafx.h"
#include "playGround.h"

playGround::playGround()
{
}


playGround::~playGround()
{
}

//초기화 함수
HRESULT playGround::init()
{
	gameNode::init();

	_countFruit = 0;
	_fruitImg = new image;
	_fruitImg->init("image/obj_fruit.bmp", 32, 32, true, RGB(255, 0, 255));

	for (int i = 0; i < FRUITMAX; i++)
	{
		_fruit[i].img = _fruitImg;
		_fruit[i].type = OBJECT_FRUIT;
		_fruit[i].isView = false;
	}

	_countNiddle = 0;
	_niddleImg = new image;
	_niddleImg->init("image/obj_niddle.bmp", 20, 20, true, RGB(255, 0, 255));

	for (int i = 0; i < NIDDLEMAX; i++)
	{
		_niddle[i].img = _niddleImg;
		_niddle[i].type = OBJECT_NIDDLE;
		_niddle[i].isView = false;
	}

	_playGround.set(0, 0, WINSIZEX - 60, WINSIZEY - 20);
	_groundImg = new image;
	_groundImg->init("image/floor.bmp", 230, 25, true, RGB(255, 0, 255));

	// ground 및 과일 초기화
	for (int i = 0; i < GROUNDMAX; i++)
	{
		int r = RND->getFromIntTo(1, 10);
		
		if ((i / 3) % 2 == 1)
		{
			_ground[i].rc.set(200 * (i % 3) + 70, 80 + 110 * (i / 3), 200*(i%3) + 230, 80 + 110 * (i / 3) + 25);
		}
		else
		{
			_ground[i].rc.set(260 * (i % 3), 80 + 110 * (i / 3), 260 * (i % 3) + 210, 80 + 110 * (i / 3) + 25);
		}

		// 확률에 따라서 0~FRUITMAX개의 과일 생성
		if (r >= 6 && _countFruit < FRUITMAX)
		{
			_ground[i].fruit = &_fruit[_countFruit];

			_fruit[_countFruit].rc.left = r >= 8 ? _ground[i].rc.left + 50 : _ground[i].rc.right - 50;
			_fruit[_countFruit].rc.bottom = _ground[i].rc.top;
			_fruit[_countFruit].rc.right = _fruit[_countFruit].rc.left + 32;
			_fruit[_countFruit].rc.top = _fruit[_countFruit].rc.bottom - 32;
			_fruit[_countFruit].isView = true;

			_countFruit++;
		}
		else
		{
			_ground[i].fruit = nullptr;
		}

		_ground[i].img = _groundImg;
	}
	
	_countLadder = 0;
	_ladderImg = new image;
	_ladderImg->init("image/obj_ladder.bmp", 55, 110, true, RGB(255, 0, 255));

	// 사다리 초기화
	for (int i = 0; i < LADDERMAX; i++)
	{
		if (_ladder[i].isView) continue;

		int r = RND->getFromIntTo(0, 3);
		int n = RND->getFromIntTo(1, 4);
		// 시작하는 ground 또는 끝나는 ground에 이미 사다리가 있을 경우
		if (_ground[r + 3 * _countLadder].ladder != nullptr || 
			(_countLadder >= 3 && _ground[r + 3 * _countLadder].ladder != nullptr) ||
			(_countLadder > 0 && _ground[r + 3 * (_countLadder-1)].ladder != nullptr)) continue;
		int x = r >= 2 ? _ground[r+3* _countLadder].rc.left + 40 : _ground[r + 3 * _countLadder].rc.left + 40 * n;
		int y = _ground[r + 3 * _countLadder].rc.top;

		_ladder[i].img = _ladderImg;
		_ladder[i].rc.set(x, y, x + 55, y + 110);
		_ladder[i].isView = true;

		_ground[r + 3 * _countLadder].ladder = &_ladder[i];
		if (_countLadder < 2) _ground[r + 3 * (_countLadder + 1)].ladder = &_ladder[i];

		if (++_countLadder >= 4) break;
	}

	// 장애물 초기화, 설정한 확률에 따라서 0 ~ NIDDLEMAX개가 생성됨
	for (int i = 0; i < GROUNDMAX; i++)
	{
		int n = RND->getFromIntTo(1, 10);
		int r = RND->getFromIntTo(3, 5);
		if (n >= 7 && _countNiddle < NIDDLEMAX)
		{
			_niddle[_countNiddle].rc.left = n > 8 ? _ground[i].rc.left + 10 * n : _ground[i].rc.right - 10 * n;
			_niddle[_countNiddle].rc.bottom = _ground[i].rc.top;
			_niddle[_countNiddle].rc.right = _niddle[_countNiddle].rc.left + 20;
			_niddle[_countNiddle].rc.top = _niddle[_countNiddle].rc.bottom - 20;

			// 생성하려는 ground에 사다리가 있다면
			if (_ground[i].ladder != nullptr)
			{
				// 사다리를 기준으로 좌표를 설정하여 겹치는 경우가 적게 처리함
				_niddle[_countNiddle].rc.left =
					(_ground[i].rc.left + _ground[i].rc.getWidth() / 2) <= (_ground[i].ladder->rc.left + _ground[i].ladder->rc.getWidth() / 2) ?
					_ground[i].ladder->rc.left - r * 5 : _ground[i].ladder->rc.right + r * 5;
				_niddle[_countNiddle].rc.right = _niddle[_countNiddle].rc.left + 20;
			}
			_niddle[_countNiddle].isView = true;

			_ground[i].niddle = &_niddle[_countNiddle];
			_countNiddle++;
		}
	}

	// 적 초기화, 시작 X 좌표의 기준이 되는 ground는 랜덤하게 설정함
	for (int i = 0; i < ENEMYMAX; i++)
	{
		int wr = RND->getInt(3);
		int sr = RND->getFromIntTo(1, 3);
		_enemy[i].rc.set(_ground[wr].rc.left, _ground[i * 6].rc.top - 24, _ground[wr].rc.left + 24, _ground[i * 6].rc.top);
		_enemy[i].speed = sr;
		_enemy[i].img = new image;
		_enemy[i].img->init("image/monster1.bmp", 48, 48, 2, 2, true, RGB(255, 0, 255));
		_enemy[i].img->setX(_enemy[i].rc.left);
		_enemy[i].img->setY(_enemy[i].rc.top);
		_enemy[i].img->setFrameX(0);
		_enemy[i].img->setFrameY(0);
		_enemy[i].flag = 1;
	}

	_player.angle = PI / 2;
	_player.jumpPower = 4.2f;
	_player.gravity = 0.2f;
	_player.width = 32;
	_player.height = 32;
	_player.point.x = _playGround.right + 10;
	_player.point.y = _playGround.bottom - _player.height;

	for (int i = 0; i < IMG_END; i++)
	{
		_playerImg[i] = new image;
		char str[128];
		sprintf_s(str, "image/player_%d.bmp", i + 1);
		_playerImg[i]->init(str, getImgWidth(i), getImgHeight(i), getImgWidth(i) / 32, getImgHeight(i) / 32, true, RGB(255, 0, 255));
	}

	_player.img = _playerImg[IMG_IDLE];
	_player.img->setX(_player.point.x);
	_player.img->setY(_player.point.y);
	_player.img->setFrameX(0);
	_player.img->setFrameY(0);

	_count = _index = _countEnemy = _isGameOver = _score = _isClear = 0;

	return S_OK;
}

//메모리 해제
void playGround::release()
{
	SAFE_DELETE(_fruitImg);
	SAFE_DELETE(_niddleImg);
	SAFE_DELETE(_groundImg);
	for (int i = 0; i < ENEMYMAX; i++)
		SAFE_DELETE(_enemy[i].img);
	for (int i = 0; i < IMG_END; i++)
		SAFE_DELETE(_playerImg[i]);
	
}

//연산
void playGround::update()
{
	gameNode::update();

	if (!_isGameOver && !_isClear)
	{
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			if (isPlayerOnGround())
			{
				_player.direction = RIGHT_MOVE;
				_player.img = _playerImg[IMG_WALK];
				_player.img->setFrameY(1);
				_player.point.x += 3;
				_player.img->setX(_player.point.x);
				_player.img->setY(_player.point.y);
			}
		}
		if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
		{
			if (_player.direction == RIGHT_MOVE)
			{
				_player.direction = RIGHT_IDLE;
				_player.img->setFrameX(0);
			}
		}
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			if (isPlayerOnGround())
			{
				_player.direction = LEFT_MOVE;
				_player.img = _playerImg[IMG_WALK];
				_player.img->setFrameY(0);
				_player.point.x -= 3;
				_player.img->setX(_player.point.x);
				_player.img->setY(_player.point.y);
			}
		}
		if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
		{
			if (_player.direction == LEFT_MOVE)
			{
				_player.direction = LEFT_IDLE;
				_player.img->setFrameX(1);
			}
		}
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			if (isPlayerOnGround())
			{
				bool isLeft = _player.direction == LEFT_MOVE || _player.direction == LEFT_IDLE;
				_player.direction = (isLeft ? LEFT_JUMP : RIGHT_JUMP);
				_player.angle = PI / 3 * (isLeft ? 2 : 1);
				_player.img = _playerImg[IMG_WALK];
				_player.img->setFrameY(isLeft ? 0 : 1);
				_player.jumpPower = 4.6f;
				_player.gravity = 0.1f;
			}
		}

		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			if (ladderCheck() && _player.direction != DEAD)
			{
				_player.direction = LADDER_UP;
				_player.img = _playerImg[IMG_LADDER];
				_player.img->setFrameX(0);
			}
		}

		if (KEYMANAGER->isStayKeyDown(VK_UP))
		{
			if (_player.direction == LADDER_UP)
			{
				_player.point.y -= 3;
				MYRECT t;
				t.set(_player.point.x, _player.point.y, _player.point.x + _player.width, _player.point.y + _player.height + 3);

				for (int i = 0; i < LADDERMAX; i++)
				{
					if (!_ladder[i].isView) continue;
					if (TTYONE_UTIL::isCollision(_ladder[i].rc, t) && _player.point.y + _player.height <= _ladder[i].rc.top)
					{
						_player.point.y = _ladder[i].rc.top - _player.height;
						_player.direction = IDLE;
						_player.img = _playerImg[IMG_IDLE];
						_player.img->setFrameX(0);
						break;
					}
				}
			}
			else if(_player.direction != DEAD)
			{
				_player.direction = IDLE;
				_player.img = _playerImg[IMG_IDLE];
				_player.img->setFrameX(0);
			}
		}

		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			if (ladderCheck() && _player.direction != DEAD) {
				_player.direction = LADDER_DOWN;
				_player.img = _playerImg[IMG_LADDER];
				_player.img->setFrameX(0);
			}
		}

		if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		{
			if (_player.direction == LADDER_DOWN)
			{
				_player.point.y += 3;
				MYRECT t;
				t.set(_player.point.x, _player.point.y, _player.point.x + _player.width + 3, _player.point.y + _player.height - 3);

				for (int i = 0; i < LADDERMAX; i++)
				{
					if (!_ladder[i].isView) continue;
					if (TTYONE_UTIL::isCollision(_ladder[i].rc, t) && _player.point.y + _player.height >= _ladder[i].rc.bottom)
					{
						_player.point.y = _ladder[i].rc.bottom - _player.height;
						_player.direction = IDLE;
						_player.img = _playerImg[IMG_IDLE];
						_player.img->setFrameX(0);
						break;
					}
				}
			}
			else if(_player.direction != DEAD)
			{
				_player.direction = IDLE;
				_player.img = _playerImg[IMG_IDLE];
				_player.img->setFrameX(0);
			}
		}

		if (_player.direction == LEFT_JUMP)
		{
			_player.point.x += cosf(_player.angle) * _player.jumpPower;
			_player.point.y += -sinf(_player.angle) * _player.jumpPower + _player.gravity;
			_player.gravity += 0.20f;

			if (_player.point.y + _player.height >= _playGround.bottom)
			{
				_player.point.y = _playGround.bottom - _player.height;
				_player.direction = LEFT_IDLE;
			}
			for (int i = 0; i < GROUNDMAX; i++)
			{
				MYRECT t;
				t.set(_player.point.x, _player.point.y, _player.point.x + _player.width, _player.point.y + _player.height);
				if (TTYONE_UTIL::isCollisionReaction(_ground[i].rc, t))
				{
					_player.point.y = _ground[i].rc.top - _player.height;
					_player.direction = LEFT_IDLE;
					break;
				}
			}
		}

		if (_player.direction == RIGHT_JUMP)
		{
			_player.point.x += cosf(_player.angle) * _player.jumpPower;
			_player.point.y += -sinf(_player.angle) * _player.jumpPower + _player.gravity;
			_player.gravity += 0.21f;

			if (_player.point.y + _player.height >= _playGround.bottom)
			{
				_player.point.y = _playGround.bottom - _player.height;
				_player.direction = RIGHT_IDLE;
			}
			for (int i = 0; i < GROUNDMAX; i++)
			{
				MYRECT t;
				t.set(_player.point.x, _player.point.y, _player.point.x + _player.width, _player.point.y + _player.height);
				if (TTYONE_UTIL::isCollisionReaction(_ground[i].rc, t))
				{
					_player.point.y = _ground[i].rc.top - _player.height;
					_player.direction = RIGHT_IDLE;
					break;
				}
			}
		}

		if (_player.direction == DEAD)
		{
			_player.point.y += 5;

			if (_player.point.y + _player.height >= _playGround.bottom)
			{
				_player.point.y = _playGround.bottom - _player.height;
				_isGameOver = true;
			}
		}

		_player.img->setX(_player.point.x);
		_player.img->setY(_player.point.y);

		_count++;

		if (_count % 20 == 0)
		{
			if (_player.direction == IDLE || _player.direction == RIGHT_MOVE || _player.direction == LEFT_MOVE)
			{
				if (_index > 0) _index = -1;
				_index++;
				_player.img->setFrameX(_index);
			}
			if (_player.direction == LADDER_UP && _player.direction == LADDER_DOWN || _player.direction == DEAD)
			{
				if (_index > 1) _index = -1;
				_index++;
				_player.img->setFrameX(_index);

			}
			_count = 0;
		}

		// 적 이동 처리
		for (int i = 0; i < ENEMYMAX; i++)
		{
			_enemy[i].rc.move(_enemy[i].speed * _enemy[i].flag, 0);
			_enemy[i].img->setX(_enemy[i].rc.left);
			_enemy[i].img->setY(_enemy[i].rc.right);

			if (_enemy[i].rc.right >= _playGround.right || _enemy[i].rc.left <= _playGround.left)
			{
				_enemy[i].img->setFrameY(_enemy[i].img->getFrameY() + _enemy[i].flag);
				_enemy[i].flag *= -1;
			}
		}

		checkCollision();

		_player.img->setX(_player.point.x);
		_player.img->setY(_player.point.y);
	}
}

//그리기 전용
void playGround::render(HDC hdc)
{
	//백버퍼의 DC영역을 가지고 온다 
	HDC backDC = this->getBackBuffer()->getMemDC();
	PatBlt(backDC, 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
	//=================================================

	for (int i = 0; i < GROUNDMAX; i++)
		_ground[i].img->render(backDC, _ground[i].rc.left, _ground[i].rc.top, _groundImg->getX(), _groundImg->getY(), _ground[i].rc.getWidth(), _ground[i].rc.getHeight());

	for (int i = 0; i < LADDERMAX; i++)
		if(_ladder[i].isView) _ladder[i].img->render(backDC, _ladder[i].rc.left, _ladder[i].rc.top);

	for (int i = 0; i < FRUITMAX; i++)
		if (_fruit[i].isView) _fruit[i].img->render(backDC, _fruit[i].rc.left, _fruit[i].rc.top);

	for (int i = 0; i < NIDDLEMAX; i++)
		if (_niddle[i].isView) _niddle[i].img->render(backDC, _niddle[i].rc.left, _niddle[i].rc.top);

	char str[128];
	sprintf_s(str, "fruit: %d개, score: %d", _countFruit, _score);
	TextOut(backDC, WINSIZEX / 2, WINSIZEY / 2, str, strlen(str));

	for (int i = 0; i < ENEMYMAX; i++)
		_enemy[i].img->frameRender(backDC, _enemy[i].rc.left, _enemy[i].rc.top, _enemy[i].img->getFrameX(), _enemy[i].img->getFrameY());

	_player.img->frameRender(backDC, _player.img->getX(), _player.img->getY(), _player.img->getFrameX(), _player.img->getFrameY());

	
	if (_isClear)
		TextOut(backDC, WINSIZEX / 2 - 50, WINSIZEY / 2 - 100, "레벨 클리어!", strlen("레벨 클리어!"));
	if (_isGameOver)
		TextOut(backDC, WINSIZEX / 2 - 50, WINSIZEY / 2 - 100, "게임 오버!", strlen("게임 오버!"));
	//=============================================
	this->getBackBuffer()->render(hdc, 0, 0);
}

int playGround::getImgWidth(int index)
{
	int i = 32;
	switch(index)
	{
	case IMG_IDLE:
	case IMG_WALK:
	case IMG_DEAD:
		i *= 2;
		break;
	case IMG_JUMP:
		i *= 4;
		break;
	case IMG_LADDER:
		i *= 3;
		break;
	}
	return i;
}

int playGround::getImgHeight(int index)
{
	int i = 32;
	switch (index)
	{
	case IMG_IDLE:
	case IMG_LADDER:
		break;
	case IMG_WALK:
	case IMG_DEAD:
	case IMG_JUMP:
		i *= 2;
		break;
	}
	return i;
}

bool playGround::ladderCheck()
{
	for (int i = 0; i < LADDERMAX; i++)
	{
		if (_player.point.x + _player.width / 2 >= _ladder[i].rc.left && _player.point.x + _player.width / 2 <= _ladder[i].rc.right &&
			_player.point.y + _player.height >= _ladder[i].rc.top && _player.point.y + _player.height <= _ladder[i].rc.bottom)
		{
			return true;
		}
	}
	return false;
}

bool playGround::isPlayerOnGround()
{
	if (_player.direction == IDLE ||
		_player.direction == LEFT_IDLE ||
		_player.direction == RIGHT_IDLE ||
		_player.direction == LEFT_MOVE ||
		_player.direction == RIGHT_MOVE) return true;
	return false;
}

void playGround::checkCollision()
{
	MYRECT p;
	p.set(_player.point.x, _player.point.y, _player.point.x + _player.width, _player.point.y + _player.height);

	// 빈 공간 충돌 처리
	for (int i = 0; i < GROUNDMAX; i++)
	{
		MYRECT checkT;
		checkT.set(_ground[i].rc.left - 30, _ground[i].rc.top - 30, _ground[i].rc.right + 30, _ground[i].rc.bottom);

		if (_player.direction == LEFT_JUMP || _player.direction == RIGHT_JUMP || _player.direction == LADDER_UP || _player.direction == LADDER_DOWN) break;

		if (TTYONE_UTIL::isCollision(checkT, p) && (_player.point.x > _ground[i].rc.right || _player.point.x + _player.width < _ground[i].rc.left))
		{
			_player.direction = DEAD;
			_player.img = _playerImg[IMG_DEAD];
			_player.img->setFrameX(0);
		}
	}

	// 적 충돌 처리
	for (int i = 0; i < ENEMYMAX; i++)
	{
		if (TTYONE_UTIL::isCollision(_enemy[i].rc, p))
		{
			_player.direction = DEAD;
			_player.img = _playerImg[IMG_DEAD];
			_player.img->setFrameX(0);
		}
	}

	// 과일 충돌 처리
	for (int i = 0; i <= _countFruit; i++)
	{
		if (_fruit[i].isView && TTYONE_UTIL::isCollision(_fruit[i].rc, p))
		{
			_fruit[i].isView = false;
			_score += 100;
			--_countFruit;
			if (_countFruit <= 0)
				_isClear = true;
		}
	}

	// 장애물 충돌 처리
	for (int i = 0; i <= _countNiddle; i++)
	{
		if (_niddle[i].isView && TTYONE_UTIL::isCollision(_niddle[i].rc, p))
		{
			_player.direction = DEAD;
			_player.img = _playerImg[IMG_DEAD];
			_player.img->setFrameX(0);
		}
	}
}
