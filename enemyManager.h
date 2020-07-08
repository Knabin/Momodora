#pragma once
#include "gameNode.h"
#include <vector>


class enemyManager : public gameNode
{
private:


private:
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

};
