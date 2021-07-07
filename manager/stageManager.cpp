#include "stdafx.h"
#include "stageManager.h"
#include "player/player.h"
#include "stage/bossStage.h"
#include "stage/commonStage.h"
#include "enemy/enemy.h"
#include "enemy/oko.h"
#include "enemy/monkey.h"
#include "enemy/bakman.h"
#include "boss/prim.h"
#include "boss/rell.h"
#include "boss/witch.h"

HRESULT StageManager::init()
{
	// TODO: Enemy 추가하는 걸 ENEMYMANAGER로 옮김, signleton으로 변경
	CommonStage * common1 = new CommonStage(0, nullptr);
	common1->init();
	common1->setPlayerMemoryAddressLink(_player);
	_vStageName.push_back("common1");

	CommonStage * common2 = new CommonStage(1, "data/stage/stage1.json");
	common2->init();
	common2->setPlayerMemoryAddressLink(_player);
	_vStageName.push_back("common2");

	CommonStage * common3 = new CommonStage(2, nullptr);
	common3->init();
	common3->setPlayerMemoryAddressLink(_player);
	_vStageName.push_back("common3");


	BossStage * boss1 = new BossStage(0, "data/stage/boss1.json");
	boss1->init();
	boss1->setPlayerMemoryAddressLink(_player);
	_vStageName.push_back("boss1");
	
	BossStage * boss2 = new BossStage(1, "data/stage/boss2.json");
	boss2->init();
	boss2->setPlayerMemoryAddressLink(_player);
	_vStageName.push_back("boss2");

	BossStage * boss3 = new BossStage(2, "data/stage/boss3.json");
	boss3->init();
	boss3->setPlayerMemoryAddressLink(_player);
	_vStageName.push_back("boss3");

	_rc[0].set(0, 0, 75, 96);
	_rc[1].set(0, 0, 75, 96);
	_rc[2].set(0, 0, 75, 96);

	_rc[0].setCenterPos(290, 524);
	_rc[1].setCenterPos(480, 524);
	_rc[2].setCenterPos(670, 524);

	_rc2 = RectMakeCenter(744, 552, 48, 48);

	_rcPray.set(0, 0, 42, 90);
	_rcPray.setLeftTopPos(800, _rc[0].bottom - 90);

	SCENEMANAGER->addScene(_vStageName[0], common1);
	SCENEMANAGER->addScene(_vStageName[1], common2);
	SCENEMANAGER->addScene(_vStageName[2], common3);
	SCENEMANAGER->addScene(_vStageName[3], boss1);
	SCENEMANAGER->addScene(_vStageName[4], boss2);
	SCENEMANAGER->addScene(_vStageName[5], boss3);

	_currentIdx = 0;
	SCENEMANAGER->changeScene(_vStageName[_currentIdx]);


	return S_OK;
}

void StageManager::release()
{
}

void StageManager::update()
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

void StageManager::render()
{
	SCENEMANAGER->render();

	if (_currentIdx == 2 && TIMEMANAGER->getDebug())
	{
		_rc[0].render(getMemDC());
		_rc[1].render(getMemDC());
		_rc[2].render(getMemDC());
		//_rcPray.render(getMemDC());
	}
	if (_currentIdx == 1 && TIMEMANAGER->getDebug())
	{
		Rectangle(getMemDC(), _rc2);
	}

	if (_currentIdx == 2)
	{
		IMAGEMANAGER->findImage("오브젝트")->render(getMemDC(), _rcPray.left, _rcPray.top);
	}

}

void StageManager::moveNextStage()
{
	if (getIsBossStage()) return;
	++_currentIdx;
	SCENEMANAGER->changeScene(_vStageName[_currentIdx]);
}

void StageManager::movePrevStage()
{
	if (_currentIdx == 0) return;
	if (getIsBossStage()) _currentIdx = 2;
	else --_currentIdx;
	SCENEMANAGER->changeScene(_vStageName[_currentIdx]);
}

void StageManager::moveBossStage(int index)
{
	_player->setPointLeftStart();
	_currentIdx = index;
	SCENEMANAGER->changeScene(_vStageName[_currentIdx]);
}
void StageManager::parsingEnemyData(const char * loadFileName, vector<Enemy*>& vEnemy)
{
	_vFileData.clear();
	_vFileData = TXTDATA->txtLoad(loadFileName);

	for (int i = 0; i < _vFileData.size(); i+=3)
	{
		int type = stoi(_vFileData[i], nullptr, 10);
		float x = stof(_vFileData[i + 1], nullptr);
		float y = stof(_vFileData[i + 2], nullptr);
		Enemy * e = nullptr;

		switch (type)
		{
		case 0:
			e = new Oko();
			dynamic_cast<Oko*>(e)->setObjectRect(_rc2);
			break;
		case 1:
			e = new Monkey();
			break;
		case 2:
			e = new bakman();
			break;
		case 3:
			e = new Prim();
			break;
		case 4:
			e = new Witch();
			break;
		case 5:
			e = new Rell();
			break;
		}

		if (e != nullptr)
		{
			e->init({ x,y });
			e->setPlayerMemoryAddressLink(_player);
			vEnemy.push_back(e);
		}
	}
}

void StageManager::checkEnterBossStage()
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

bool StageManager::isAliveBoss()
{
	if (!getIsBossStage()) return false;

	return true;
}

void StageManager::checkPrayObject()
{
	if (_currentIdx != 2) return;

	if (isCollision(_rcPray, _player->getHitbox()))
	{
		_player->setStatePray();
	}
}

int StageManager::getCurrentStageSize()
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
