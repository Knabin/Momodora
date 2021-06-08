#pragma once
#include "stage.h"

class Player;
class Enemy;

class CommonStage : public Stage
{
public:
	CommonStage(int stageNum, const char * fileName);
	~CommonStage() override final;

	virtual HRESULT init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

private:
	bool _backImg;

	int _loopCount;

	int _loopX1;
	int _loopX2;
	int _loopX3;
	int _loopX4;
};

