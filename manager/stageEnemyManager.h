#pragma once
#include <vector>
#include <string>

class enemy;

class stageEnemyManager
{
public:
	static stageEnemyManager* getInstance()
	{
		static stageEnemyManager instance;
		return &instance;
	}

private:
	stageEnemyManager();
	~stageEnemyManager();

private:
	vector<string> _vFileData;
	vector<string>::iterator _viFileData;

	RECT _rc;

public:
	HRESULT init();
	void release();
	void update();

	void parsingEnemyData(const char * loadFileName, vector<enemy *>& vEnemy);
	void parsingBossData(const char * lodaFileName, enemy * boss);
};

