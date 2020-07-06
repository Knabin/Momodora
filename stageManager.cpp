#include "stdafx.h"
#include "stageManager.h"
#include "player.h"
#include "bossStage.h"
#include "commonStage.h"
#include "enemy.h"

HRESULT stageManager::init()
{
	commonStage * common1 = new commonStage();
	common1->init(0);
	common1->setPlayerMemoryAddressLink(_player);
	_vStageName.push_back("common1");

	commonStage * common2 = new commonStage();
	common2->init(1);
	common2->setPlayerMemoryAddressLink(_player);
	_vStageName.push_back("common2");

	commonStage * common3 = new commonStage();
	common3->init(2);
	common3->setPlayerMemoryAddressLink(_player);
	_vStageName.push_back("common3");


	bossStage * boss1 = new bossStage();
	boss1->init(0);
	boss1->setPlayerMemoryAddressLink(_player);
	_vStageName.push_back("boss1");
	
	bossStage * boss2 = new bossStage();
	boss2->init(1);
	boss2->setPlayerMemoryAddressLink(_player);
	_vStageName.push_back("boss2");

	bossStage * boss3 = new bossStage();
	boss1->init(2);
	boss1->setPlayerMemoryAddressLink(_player);
	_vStageName.push_back("boss3");

	_rc[0].set(0, 0, 75, 96);
	_rc[1].set(0, 0, 75, 96);
	_rc[2].set(0, 0, 75, 96);

	_rc[0].setCenterPos(290, 524);
	_rc[1].setCenterPos(480, 524);
	_rc[2].setCenterPos(670, 524);

	_rc2.set(0, 0, 48, 48);
	_rc2.setCenterPos(744, 552);

	enemy* e = new oko;
	e->init({ _rc2.left, _rc2.top });
	dynamic_cast<oko*>(e)->setObjectRect(_rc2);
	common2->insertIntoEnemyVector(e);

	enemy* m = new monkey;
	m->init();
	common2->insertIntoEnemyVector(m);

	enemy* b = new bakman;
	b->init({ 470, 428 });
	b->setPlayerMemoryAddressLink(_player);
	common2->insertIntoEnemyVector(b);

	enemy* b2 = new bakman;
	b2->init({ 1000, 480 });
	b2->setPlayerMemoryAddressLink(_player);
	common2->insertIntoEnemyVector(b2);

	SCENEMANAGER->addScene(_vStageName[0], common1);
	SCENEMANAGER->addScene(_vStageName[1], common2);
	SCENEMANAGER->addScene(_vStageName[2], common3);
	SCENEMANAGER->addScene(_vStageName[3], boss2);
	SCENEMANAGER->addScene(_vStageName[4], boss2);
	SCENEMANAGER->addScene(_vStageName[5], boss2);

	_currentIdx = 0;
	SCENEMANAGER->changeScene(_vStageName[_currentIdx]);


	return S_OK;
}

void stageManager::release()
{
}

void stageManager::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_F1))
	{
		SCENEMANAGER->changeScene(_vStageName[0]);
		_currentIdx = 0;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F2))
	{
		SCENEMANAGER->changeScene(_vStageName[1]);
		_currentIdx = 1;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F3))
	{
		SCENEMANAGER->changeScene(_vStageName[2]);
		_currentIdx = 2;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F4))
	{
		SCENEMANAGER->changeScene(_vStageName[3]);
		_currentIdx = 3;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F5))
	{
		SCENEMANAGER->changeScene(_vStageName[4]);
		_currentIdx = 4;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F6))
	{
		SCENEMANAGER->changeScene(_vStageName[5]);
		_currentIdx = 5;
	}

	SCENEMANAGER->update();	
}

void stageManager::render()
{
	SCENEMANAGER->render();

	if (_currentIdx == 2 && DEBUG)
	{
		_rc[0].render(getMemDC());
		_rc[1].render(getMemDC());
		_rc[2].render(getMemDC());
	}
	if (_currentIdx == 1 && DEBUG)
	{
		_rc2.render(getMemDC());
	}

	cout << _currentIdx << endl;
}

void stageManager::moveNextStage()
{
	if (getIsBossStage()) return;
	++_currentIdx;
	SCENEMANAGER->changeScene(_vStageName[_currentIdx]);
}

void stageManager::movePrevStage()
{
	if (_currentIdx == 0) return;
	if (getIsBossStage()) _currentIdx = 2;
	else --_currentIdx;
	SCENEMANAGER->changeScene(_vStageName[_currentIdx]);
}

void stageManager::moveBossStage(int index)
{
	_player->setPointLeftStart();
	_currentIdx = index;
	SCENEMANAGER->changeScene(_vStageName[_currentIdx]);
}

void stageManager::checkEnterBossStage()
{
	if (_currentIdx != 2) return;
	
	if(isCollision(_rc[0], _player->getHitbox()))
	{
		moveBossStage(3);
	}
	if (isCollision(_rc[1], _player->getHitbox()))
	{
		moveBossStage(4);
	}
	if (isCollision(_rc[2], _player->getHitbox()))
	{
		moveBossStage(5);
	}
}

bool stageManager::isAliveBoss()
{
	if (!getIsBossStage()) return false;

	//TODO: 보스 enemy 가져와서 살아 잇는지 체크 필요
	return true;
}

int stageManager::getCurrentStageSize()
{
	switch (_currentIdx)
	{
	case 0:
	case 1:
		return 1920;
	case 2:
	case 3:
	case 4:
	case 5:
		return 960;
	}
}
