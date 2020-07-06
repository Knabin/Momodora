#include "stdafx.h"
#include "playGround.h"


playGround::playGround()
{
}


playGround::~playGround()
{
}

//�ʱ�ȭ �Լ�
HRESULT playGround::init()
{
	gameNode::init(true);
	{
		IMAGEMANAGER->addImage("��1", "image/back1.bmp", 960, 723, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("��2", "image/back2.bmp", 960, 723, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("��3", "image/back3.bmp", 960, 723, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("��4", "image/back4.bmp", 960, 723, false, RGB(255, 0, 255));

		IMAGEMANAGER->addImage("redDC", "image/redDC.bmp", 500, 500, false, NULL);

		IMAGEMANAGER->addImage("���", "image/background.bmp", 1920, 720, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("��� �ȼ�", "image/background_pixel.bmp", 1920, 720, false, RGB(0, 0, 0));
		IMAGEMANAGER->addImage("���2", "image/background2.bmp", 1920, 720, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("���2 �ȼ�", "image/background2_pixel.bmp", 1920, 720, false, RGB(0, 0, 0));
		IMAGEMANAGER->addImage("���3", "image/background3.bmp", 960, 720, false, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("���3 �ȼ�", "image/background3_pixel.bmp", 960, 720, false, RGB(0, 0, 0));
		IMAGEMANAGER->addImage("���3 �ȼ� ����", "image/background3_pixel_b.bmp", 960, 720, false, RGB(0, 0, 0));

		IMAGEMANAGER->addImage("���� ���", "image/background4.bmp", 960, 720, false, RGB(0, 0, 0));

		IMAGEMANAGER->addFrameImage("idle", "image/momo_idle.bmp", 480, 192, 5, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("attack", "image/momo_attack.bmp", 384, 192, 4, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("attack2", "image/momo_attack2.bmp", 384, 192, 4, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("run", "image/momo_run.bmp", 768, 192, 8, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("jump", "image/momo_jump.bmp", 1056, 192, 11, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("throw", "image/momo_throw.bmp", 288, 192, 3, 2, true, RGB(255, 0, 255));

		IMAGEMANAGER->addFrameImage("effect_charge", "image/momo_effect_charge.bmp", 1056, 100, 11, 1, true, RGB(255, 0, 255));

		IMAGEMANAGER->addImage("����", "image/momo_bullet.bmp", 35, 15, false, RGB(0, 0, 0));
		IMAGEMANAGER->addImage("���� ���", "image/block_boss.bmp", 48, 144, false, RGB(0, 0, 0));

		IMAGEMANAGER->addImage("����1", "image/oko.bmp", 39, 39, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("����1 ������", "image/oglop.bmp", 1536, 96, 16, 1, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("����2", "image/monkey_move.bmp", 576, 192, 6, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("����2 ����", "image/monkey_attack.bmp", 2112, 192, 11, 2, true, RGB(255, 0, 255));
	}

	_player = new player;
	_sm = new stageManager;

	_player->setStageManagerMemoryAddressLink(_sm);
	_sm->setPlayerMemoryAddressLink(_player);

	_player->init();
	_sm->init();



	_pixel = new pixelCollsion;
	_pixel->init();



	_pixel->setStageManagerMemoryAddressLink(_sm);
	_pixel->setPlayerMemoryAddressLink(_player);

	_background = IMAGEMANAGER->findImage("���");
	_backgroundPixel = IMAGEMANAGER->findImage("��� �ȼ�");

	//_vTest.push_back("�׽�Ʈ1");
	//_vTest.push_back("�׽�Ʈ2");
	////TXTDATA->txtSave("test.data", _vTest);
	
	vector<string> _vTest = TXTDATA->txtLoad("test.data");
	cout << _vTest[0] << endl;
	cout << _vTest[1] << endl;

	_shakeCount = 0;
	_isShaking = false;

	EFFECTMANAGER->addEffect("left", "image/momo_effect_left.bmp", 768, 120, 192, 120, 1, 1.0f, 4);
	EFFECTMANAGER->addEffect("left2", "image/momo_effect_left2.bmp", 768, 120, 192, 120, 1, 1.0f, 4);
	EFFECTMANAGER->addEffect("right", "image/momo_effect_right.bmp", 768, 120, 192, 120, 1, 1.0f, 4);
	EFFECTMANAGER->addEffect("right2", "image/momo_effect_right2.bmp", 768, 120, 192, 120, 1, 1.0f, 4);
	

	// ==========================================
	// ## ī�޶� ���� �ʱ�ȭ ##
	// ==========================================
	CAMERA->setPosition(_player->getX(), _player->getY());

	return S_OK;
}

//�޸� ����
void playGround::release()
{
	
}

//����
void playGround::update()
{
	gameNode::update();

	_player->update();
	_pixel->update();
	_sm->update();

	if (_player->getIsCameraShaking())
	{
		CAMERA->setIsShaking(true);
		++_shakeCount;
		if (_shakeCount > 5)
		{
			_shakeCount = 0;
			CAMERA->setIsShaking(false);
			_player->setIsCameraShaking(false);
		}
	}

	EFFECTMANAGER->update();

	// ==========================================
	// ## ī�޶� ���� ������Ʈ ##
	// ==========================================
	if (CAMERA->getIsShaking()) CAMERA->cameraShake();
	else CAMERA->setPosition(_player->getX(), _player->getY());	
}

//�׸��� ����
void playGround::render()
{	
	PatBlt(getMemDC(), 0, 0, getMemDCWidth(), getMemDCHeight(), WHITENESS);
	//PatBlt(CAMERA->getMemDC(), 0, 0, getMemDCWidth(), getMemDCHeight(), WHITENESS);
	//=================================================
	
	_sm->render();
	//IMAGEMANAGER->findImage("���2 �ȼ�")->render(getMemDC());
	_player->render();
	EFFECTMANAGER->render();
	TIMEMANAGER->render(getMemDC());

	//=============================================
	_backBuffer->render(getHDC(), 0, 0,
		CAMERA->getLeft(), CAMERA->getTop(),
		CAMERA->getWidth(), CAMERA->getHeight());
	//CAMERA->render(getHDC());
}
