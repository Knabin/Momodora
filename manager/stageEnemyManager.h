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
	vector<string> _vFileData;
	vector<string>::iterator _viFileData;

	RECT _rc;

public:
	HRESULT init();
	void release();
	void update();

	void parsingEnemyData(const char * loadFileName, vector<Enemy *>& vEnemy);
	void parsingBossData(const char * lodaFileName, Enemy * boss);
};

