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

	

	_pgBar.init(WINSIZEX / 2 - 343, WINSIZEY - 100, 687, 39);

	return S_OK;
}

void bossStage::release()
{
}

void bossStage::update()
{
	CAMERA->setBackWidth(960);
	_pgBar.update();
}

void bossStage::render()
{
	_image->render(getMemDC());
	_pgBar.render();
}
