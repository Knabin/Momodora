#include "stdafx.h"
#include "bossStage.h"
#include "player.h"

HRESULT bossStage::init(int bossNum)
{
	_image = IMAGEMANAGER->findImage("���� ���");
	_pixel = IMAGEMANAGER->findImage("���3 �ȼ�");

	switch (bossNum)
	{
	case BOSS1:
		break;
	case BOSS2:
		break;
	case BOSS3:
		break;
	}

	return S_OK;
}

void bossStage::release()
{
}

void bossStage::update()
{
	CAMERA->setBackWidth(960);
}

void bossStage::render()
{
	_image->render(getMemDC());
}
