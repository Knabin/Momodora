#include "stdafx.h"
#include "stage/bossStage.h"
#include "player/player.h"
#include "enemy/enemy.h"

BossStage::BossStage(int bossNum, const char * fileName)
	: Stage(bossNum, fileName)
	, _isStart(false)
	, _isDead(false)
	, _blockRc(0, 0, 0, 0)
	, _blockColRc(0, 0, 0, 0)
{
	init();
}

BossStage::~BossStage()
{
	SAFE_DELETE(_ani_block);
}

HRESULT BossStage::init()
{
	_background = IMAGEMANAGER->findImage("보스 배경");
	_block = IMAGEMANAGER->findImage("보스 블록");

	_ani_block = new Animation;
	_ani_block->init(_block->getWidth(), _block->getHeight(),
					 _block->getFrameWidth(), _block->getFrameHeight());
	_ani_block->setFPS(1);
	_ani_block->setDefPlayFrame(false, false);

	_blockRc.set(0, 144, 48, 428);
	_blockColRc.set(0, 0, 50, 150);
	_blockColRc.setLeftTopPos(0, _blockRc.bottom);

	_pgBar = make_unique<ProgressBar>(new ProgressBar(WINSIZEX / 2 - 343, WINSIZEY - 100, 687, 39));
	_enemies.clear();

	if(_fileName != nullptr) STAGEENEMYMANAGER->parsingEnemyData(_fileName, _enemies);
	
	for(int i = 0; i < _enemies.size(); ++i)
		_enemies[i]->setPlayerMemoryAddressLink(_player);

	_isStart = _isDead = false;

	switch (_stageNum)
	{
	case STAGE1:
		
		if (!SOUNDMANAGER->isPlaySound("보스1 맵"))
		{
			SOUNDMANAGER->stopAll("보스1 맵");
			SOUNDMANAGER->playBGM("보스1 맵", BGMVOLUME);
		}
		break;
	case STAGE2:
		if (!SOUNDMANAGER->isPlaySound("보스2 맵"))
		{
			SOUNDMANAGER->stopAll("보스2 맵");
			SOUNDMANAGER->playBGM("보스2 맵", BGMVOLUME);
		}
		break;
	case STAGE3:
		if (!SOUNDMANAGER->isPlaySound("보스3 맵"))
		{
			SOUNDMANAGER->stopAll("보스3 맵");
			SOUNDMANAGER->playBGM("보스3 맵", BGMVOLUME);
		}
		break;
	}

	return S_OK;
}

void BossStage::release()
{
	for (int i = 0; i < _enemies.size(); ++i)
	{
		_enemies[i]->release();
		SAFE_DELETE(_enemies[i]);
	}
}

void BossStage::update()
{
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && !_isStart)
	{
		_isStart = true;
		_ani_block->setPlayFrame(0, 23, false);
		_ani_block->start();
		CAMERA->setBackWidth(960);
	}

	if (_ani_block->isPlay())
	{
		_ani_block->frameUpdate(TIMEMANAGER->getElapsedTime() * 30);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_LSHIFT))
	{
		_isStart = false;
		_isDead = true;
	}

	if (checkPointInRect(_blockColRc, _player->getProbeXL(), _player->getY()) && _isStart && !_isDead)
	{
		_player->setCanMoveLeft(false);
	}

	if (_isStart)
	{
		bool checkDead = true;
		for (int i = 0; i < _enemies.size(); ++i)
		{
			_enemies[i]->update();

			if (_enemies[i]->getHP() > 0)
			{
				checkDead = false;
			}
		}
		_isDead = checkDead;
	}

	if (_enemies.size() != 0)
	{
		_pgBar->setGauge(_enemies[0]->getHP(), _enemies[0]->getMaxHP());
		_pgBar->update();
	}
}

void BossStage::render()
{
	_background->render(getMemDC());
	
	if (_isStart && !_isDead)
	{
		_pgBar->render();
		_block->aniRender(getMemDC(), _blockRc.left, _blockRc.bottom, _ani_block);
	}

	for (int i = 0; i < _enemies.size(); ++i)
		_enemies[i]->render();
}
