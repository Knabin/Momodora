#pragma once
#include <vector>
#include <string>
#include "gameNode.h"

class player;
class enemy;

class stageManager : public gameNode
{
private:
	player * _player;

	// 0 ~ 2: commonStage, 3 ~ 5: bossStage
	int _currentIdx;
	vector<string> _vStageName;

	MYRECT _rc[3];
	MYRECT _rc2;

	/* 
	stageManager���� �ؾ� �� ��

	�ε��� 2��° ������������ rect�� �浹 üũ(�ش� ������ �̵�)
	enemyManager ������ �� stage �ȿ� �ִ� enemy vector �Ѱܼ� enemy �޾ƿ���
	*/
	

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void moveNextStage();
	void movePrevStage();
	void moveBossStage(int index);

	void setPlayerMemoryAddressLink(player *p) { _player = p; }

	void checkEnterBossStage();
	bool isAliveBoss();


	int getCurrentStageSize();
	int getCurrentIndex() { return _currentIdx; }

	bool getIsBossStage() 
	{
		if (_currentIdx > 2) return true; 
		return false; 
	}

};

