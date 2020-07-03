#pragma once
#include "gameNode.h"
#include <vector>


class enemyManager : public gameNode
{
private:


private:


public:
	enemyManager();
	~enemyManager();

	HRESULT init();
	void release();
	void update();
	void render();

};
