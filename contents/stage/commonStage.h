#pragma once
#include "gameNode.h"

enum
{
	STAGE1,
	STAGE2,
	STAGE3
};

class Player;
class Enemy;

class CommonStage : public GameNode
{
private:
	vector<Enemy *> _vEnemy;
	vector<Enemy *>::iterator _viEnemy;

	const char * _fileName;

	Image* _image;
	Image* _pixel;

	bool _backImg;

	int _loopCount;

	int _loopX1;
	int _loopX2;
	int _loopX3;
	int _loopX4;

	Player* _player;

	int _stageNum;


public:
	CommonStage();
	CommonStage(int stageNum, const char * fileName);

	// STAGE1 = 0, STAGE2 = 1, STAGE3 = 2
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setPlayerMemoryAddressLink(Player *p) { _player = p; }

	vector<Enemy *>& getEnemyVector() { return _vEnemy; }
	void setEnemyVector(vector<Enemy *>& vEnemy) { _vEnemy = vEnemy; }
	void insertIntoEnemyVector(Enemy * e) { _vEnemy.push_back(e); }
};

