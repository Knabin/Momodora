#pragma once
#include "gameNode.h"
#include <vector>


class enemyManager : public gameNode
{
private:


private:
	/*
	enemyManager���� �ؾ� �� ��
	���� �о� �;� ��(Ÿ��(common, boss), Ÿ�� �ε���(0~2, 0~4), ���� x��, ���� y��, ��Ʈ�ڽ� width, ��Ʈ�ڽ� height)
	stageManager�� ���� vector�� enemy �����ؼ� �ֱ�(�̶� ���� parsing�ؾ� ��!)
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
