#include "stdafx.h"
#include "bossStage.h"
#include "player.h"

HRESULT bossStage::init(int bossNum)
{

	_image = IMAGEMANAGER->findImage("���� ���");
	_pixel = IMAGEMANAGER->findImage("���3 �ȼ� ����");
	
	switch (bossNum)
	{
	case BOSS1:
		break;
	case BOSS2:
		break;
	case BOSS3:
		break;
	}

	_rc = RectMake(0, 428, 48, 144);
	

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

	if (KEYMANAGER->isToggleKey(VK_TAB))
	{

	}
}

void bossStage::render()
{
	_image->render(getMemDC());
	_pgBar.render();
	IMAGEMANAGER->findImage("���� ���")->render(getMemDC(), _rc.left, _rc.top);
}
