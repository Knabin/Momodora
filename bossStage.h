#pragma once
#include "progressBar.h"

enum
{
	BOSS1,
	BOSS2,
	BOSS3
};

class player;
class enemy;

class bossStage : public gameNode
{
private:
	image* _image;

	image* _block;
	animation* _ani_block;

	MYRECT _rc;
	MYRECT _rcCol;

	progressBar _pgBar;

	player* _player;
	
	int _bossNum;

	const char * _fileName;
	vector<enemy*> _vBoss;

	bool _isStart;
	bool _isDead;

public:
	bossStage(int bossNum, const char * fileName);
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setPlayerMemoryAddressLink(player *p) { _player = p; }

	/*enemy* getBossPointer() { return _boss; }
	void setBossPointer(enemy* boss) { _boss = boss; }*/
};

