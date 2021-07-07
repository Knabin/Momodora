#include "stdafx.h"
#include "stageEnemyManager.h"
#include "enemy/enemy.h"
#include "enemy/oko.h"
#include "enemy/monkey.h"
#include "enemy/bakman.h"
#include "boss/prim.h"
#include "boss/rell.h"
#include "boss/witch.h"

StageEnemyManager::StageEnemyManager()
	: _rc()
{
}

StageEnemyManager::~StageEnemyManager()
{
}

HRESULT StageEnemyManager::init()
{
	_rc = RectMakeCenter(744, 552, 48, 48);
	
	return S_OK;
}

void StageEnemyManager::release()
{
}

void StageEnemyManager::update()
{
}

void StageEnemyManager::parsingEnemyData(const char * loadFileName, vector<Enemy*>& vEnemy)
{
	_vFileData.clear();
	JSON->dataLoad(loadFileName, _vFileData);

	for (int i = 0; i < _vFileData.size(); ++i)
	{
		int type = _vFileData[i].type;
		float x = _vFileData[i].x;
		float y = _vFileData[i].y;

		Enemy * e = nullptr;

		switch (type)
		{
		case 0:
			e = new Oko;
			dynamic_cast<Oko*>(e)->setObjectRect(_rc);
			break;
		case 1:
			e = new Monkey;
			break;
		case 2:
			e = new bakman;
			break;
		case 3:
			e = new Prim;
			break;
		case 4:
			e = new Witch;
			break;
		case 5:
			e = new Rell;
			break;
		}

		if (e != nullptr)
		{
			e->init({ x,y });
			vEnemy.push_back(e);
		}
	}
}

void StageEnemyManager::parsingBossData(const char * loadFileName, Enemy * boss)
{
	_vFileData.clear();
	JSON->dataLoad(loadFileName, _vFileData);

	if (_vFileData.size() <= 0) return;

	int type = _vFileData[0].type;
	float x = _vFileData[0].x;
	float y = _vFileData[0].y;

	switch (type)
	{
	case 0:
		boss = new Oko;
		dynamic_cast<Oko*>(boss)->setObjectRect(_rc);
		break;
	case 1:
		boss = new Monkey;
		break;
	case 2:
		boss = new bakman;
		break;
	case 3:
		boss = new Prim;
		break;
	case 4:
		boss = new Witch;
		break;
	case 5:
		boss = new Rell;
		break;
	}

	if (boss != nullptr)
	{
		boss->init({ x,y });
	}
}
