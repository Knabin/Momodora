#pragma once
#include <vector>
#include <string>

class Enemy;

class StageEnemyManager
{
public:
	static StageEnemyManager* getInstance()
	{
		static StageEnemyManager instance;
		return &instance;
	}

private:
	StageEnemyManager();
	~StageEnemyManager();

private:
	vector<Utils::st_data_enemy> _vFileData;
	vector<Utils::st_data_enemy> ::iterator _viFileData;

	RECT _rc;

public:
	HRESULT init();
	void release();
	void update();

	void parsingEnemyData(const char * loadFileName, vector<Enemy *>& vEnemy);
	void parsingBossData(const char * loadFileName, Enemy * boss);
};

