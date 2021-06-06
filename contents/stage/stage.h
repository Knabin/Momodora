#pragma once
#include "gameNode.h"

enum
{
	STAGE1,
	STAGE2,
	STAGE3,
	END
};

class Player;
class Enemy;

class Stage : public GameNode
{
public:
	Stage(int stageNum, const char* fileName);
	virtual ~Stage();

	virtual HRESULT init() = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	void setPlayerMemoryAddressLink(Player* p) { _player = p; }

	vector<Enemy*>& getEnemyVector() { return _enemies; }
	void setEnemyVector(vector<Enemy*>& vEnemy) { _enemies = vEnemy; }
	void insertIntoEnemyVector(Enemy* e) { _enemies.push_back(e); }

protected:
	Image* _background;
	Image* _pixel;

	Player* _player;

	int			_stageNum;
	const char* _fileName;

	vector<Enemy*>				_enemies;
	vector<Enemy*>::iterator	_iterEnemies;

};

