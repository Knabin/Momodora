#pragma once

#include "stage.h"
#include "ui/progressBar/progressBar.h"

class Player;
class Enemy;

class BossStage : public Stage
{
public:
	BossStage(int bossNum, const char * fileName);
	virtual ~BossStage() override final;

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

private:
	Image* _block;
	Animation* _ani_block;

	MYRECT _blockRc;
	MYRECT _blockColRc;

	unique_ptr<ProgressBar> _pgBar;

	bool _isStart;
	bool _isDead;
};

