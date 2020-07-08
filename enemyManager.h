#pragma once
#include "gameNode.h"
#include <vector>
#include "enemy.h"

class stageManager;

class enemyManager : public gameNode
{
private:
	vector<enemy*>				_vEnemy;
	vector<enemy *>::iterator	_viEnemy;

	enemy* _boss;

	stageManager* _sm;
	/*
	enemyManager에서 해야 할 것
	stageManager로 vector 요청하고 충돌 처리 진행
	*/

public:
	enemyManager();
	~enemyManager();

	HRESULT init();
	void release();
	void update();
	void render();

	void setStageManagerMemoryAddressLink(stageManager* sm) { _sm = sm; }

	void setEnemyVector(vector<enemy *>& vEnemy) { _vEnemy = vEnemy; }
	void setBoss(enemy * e) { _boss = e; }
};
