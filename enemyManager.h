#pragma once
#include "gameNode.h"
#include <vector>


class enemyManager : public gameNode
{
private:


private:
	/*
	enemyManager���� �ؾ� �� ��
	stageManager�� vector ��û�ϰ� �浹 ó�� ����
	*/

public:
	enemyManager();
	~enemyManager();

	HRESULT init();
	void release();
	void update();
	void render();

};
