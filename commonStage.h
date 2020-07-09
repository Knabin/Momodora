#pragma once
#include "gameNode.h"

enum
{
	STAGE1,
	STAGE2,
	STAGE3
};

class player;
class enemy;

class commonStage : public gameNode
{
private:
	vector<enemy *> _vEnemy;
	vector<enemy *>::iterator _viEnemy;

	const char * _fileName;

	image* _image;
	image* _pixel;

	bool _backImg;

	int _loopCount;

	int _loopX1;
	int _loopX2;
	int _loopX3;
	int _loopX4;

	player* _player;

	int _stageNum;


public:
	commonStage();
	commonStage(int stageNum, const char * fileName);

	// STAGE1 = 0, STAGE2 = 1, STAGE3 = 2
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setPlayerMemoryAddressLink(player *p) { _player = p; }

	vector<enemy *>& getEnemyVector() { return _vEnemy; }
	void setEnemyVector(vector<enemy *>& vEnemy) { _vEnemy = vEnemy; }
	void insertIntoEnemyVector(enemy * e) { _vEnemy.push_back(e); }
};

