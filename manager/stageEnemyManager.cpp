#include "stdafx.h"
#include "stageEnemyManager.h"
#include "enemy/enemy.h"

StageEnemyManager::StageEnemyManager()
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
	_vFileData = TXTDATA->txtLoad(loadFileName);

	for (int i = 0; i < _vFileData.size(); i += 3)
	{
		int type = stoi(_vFileData[i], nullptr, 10);
		float x = stof(_vFileData[i + 1], nullptr);
		float y = stof(_vFileData[i + 2], nullptr);
		Enemy * e = nullptr;

		switch (type)
		{
		case 0:
			e = new oko;
			dynamic_cast<oko*>(e)->setObjectRect(_rc);
			break;
		case 1:
			e = new monkey;
			break;
		case 2:
			e = new bakman;
			break;
		case 3:
			e = new prim;
			break;
		case 4:
			e = new witch;
			break;
		case 5:
			e = new rell;
			break;
		}

		if (e != nullptr)
		{
			e->init({ x,y });
			vEnemy.push_back(e);
		}
	}
}

void StageEnemyManager::parsingBossData(const char * lodaFileName, Enemy * boss)
{
	_vFileData.clear();
	_vFileData = TXTDATA->txtLoad(lodaFileName);

	if (_vFileData.size() <= 0) return;

	int type = stoi(_vFileData[0], nullptr, 10);
	float x = stof(_vFileData[1], nullptr);
	float y = stof(_vFileData[2], nullptr);

	switch (type)
	{
	case 0:
		boss = new oko;
		dynamic_cast<oko*>(boss)->setObjectRect(_rc);
		break;
	case 1:
		boss = new monkey;
		break;
	case 2:
		boss = new bakman;
		break;
	case 3:
		boss = new prim;
		break;
	case 4:
		boss = new witch;
		break;
	case 5:
		boss = new rell;
		break;
	}

	if (boss != nullptr)
	{
		boss->init({ x,y });
	}
}
