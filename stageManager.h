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
	vector<string> _vFileData;

	MYRECT _rc[3];
	RECT _rc2;
	MYRECT _rcPray;

	/* 
	stageManager에서 해야 할 것

	인덱스 2번째 스테이지에서 rect로 충돌 체크(해당 방으로 이동)
	enemyManager 쪽으로 각 stage 안에 있는 enemy vector 넘겨서 enemy 받아오기
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

	void parsingEnemyData(const char * loadFileName, vector<enemy*>& vEnemy);

	void checkEnterBossStage();
	bool isAliveBoss();

	void checkPrayObject();

	int getCurrentStageSize();
	int getCurrentIndex() { return _currentIdx; }

	//vector<enemy *>& getCurrentStageEnemyVector { _v}

	bool getIsBossStage() 
	{
		if (_currentIdx > 2) return true; 
		return false; 
	}

};

