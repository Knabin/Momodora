#include "stdafx.h"
#include "playGround.h"


playGround::playGround()
{
}


playGround::~playGround()
{
}

//초기화 함수
HRESULT playGround::init()
{
	gameNode::init(true);
	{
		IMAGEMANAGER->addImage("백1", "image/back1.bmp", 960, 723, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("백2", "image/back2.bmp", 960, 723, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("백3", "image/back3.bmp", 960, 723, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("백4", "image/back4.bmp", 960, 723, false, RGB(255, 0, 255));

		IMAGEMANAGER->addImage("redDC", "image/redDC.bmp", 600, 600, false, NULL);

		IMAGEMANAGER->addImage("배경", "image/background.bmp", 1920, 720, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("배경 픽셀", "image/background_pixel.bmp", 1920, 720, false, RGB(0, 0, 0));
		IMAGEMANAGER->addImage("배경2", "image/background2.bmp", 1920, 720, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("배경2 픽셀", "image/background2_pixel.bmp", 1920, 720, false, RGB(0, 0, 0));
		IMAGEMANAGER->addImage("배경3", "image/background3.bmp", 960, 720, false, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("배경3 픽셀", "image/background3_pixel.bmp", 960, 720, false, RGB(0, 0, 0));
		IMAGEMANAGER->addImage("배경3 픽셀 보스", "image/background3_pixel_b.bmp", 960, 720, false, RGB(0, 0, 0));
		IMAGEMANAGER->addImage("까만 배경", "image/black.bmp", WINSIZEX, WINSIZEY, false, RGB(0, 0, 0));

		IMAGEMANAGER->addImage("오브젝트", "image/object_pray.bmp", 42, 90, true, RGB(255, 0, 255));

		IMAGEMANAGER->addImage("보스 배경", "image/background4.bmp", 960, 720, false, RGB(0, 0, 0));

		IMAGEMANAGER->addImage("게임오버", "image/gameover.bmp", 750, 133, true, RGB(255, 0, 255));

		IMAGEMANAGER->addImage("목숨", "image/heart.bmp", 30, 23, true, RGB(255, 0, 255));

		IMAGEMANAGER->addFrameImage("idle", "image/momo_idle.bmp", 480, 192, 5, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("attack", "image/momo_attack.bmp", 384, 192, 4, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("attack2", "image/momo_attack2.bmp", 384, 192, 4, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("run", "image/momo_run.bmp", 768, 192, 8, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("jump", "image/momo_jump.bmp", 1056, 192, 11, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("throw", "image/momo_throw.bmp", 288, 192, 3, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("dead", "image/momo_dead.bmp", 1536, 192, 16, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("pray", "image/momo_pray.bmp", 384, 192, 4, 2, true, RGB(255, 0, 255));

		IMAGEMANAGER->addFrameImage("effect_charge", "image/momo_effect_charge.bmp", 1056, 100, 11, 1, true, RGB(255, 0, 255));

		IMAGEMANAGER->addImage("부적", "image/momo_bullet.bmp", 35, 15, false, RGB(0, 0, 0));
		IMAGEMANAGER->addFrameImage("보스 블록", "image/block_boss.bmp", 1152, 144, 24, 1, true, RGB(255, 0, 255));

		IMAGEMANAGER->addImage("몬스터1", "image/oko.bmp", 39, 39, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("몬스터1 프레임", "image/oglop.bmp", 1536, 96, 16, 1, true, RGB(255, 0, 255));

		IMAGEMANAGER->addFrameImage("몬스터2", "image/monkey_move.bmp", 1152, 192, 6, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("몬스터2 공격", "image/monkey_attack.bmp", 2112, 192, 11, 2, true, RGB(255, 0, 255));

		IMAGEMANAGER->addImage("몬스터3 왼쪽", "image/bakman_left.bmp", 132, 96, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("몬스터3 오른쪽", "image/bakman_right.bmp", 132, 96, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("몬스터3 공격", "image/bakman_attack.bmp", 924, 192, 7, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("몬스터3 공격2", "image/bakman_attack2.bmp", 792, 192, 6, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("몬스터3 블록", "image/bakfall.bmp", 45, 45, true, RGB(255, 0, 255));

		IMAGEMANAGER->addFrameImage("보스1 시작", "image/prim_startled.bmp", 4200, 144, 10, 1, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("보스1 이동", "image/prim_move.bmp", 3360, 288, 8, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("보스1 화남", "image/prim_angry.bmp", 6300, 288, 15, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("보스1 공격", "image/prim_attack.bmp", 8400, 288, 20, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("보스1 패배", "image/prim_defeated.bmp", 420, 144, true, RGB(255, 0, 255));

		IMAGEMANAGER->addFrameImage("보스2 왼쪽", "image/witchd_idle.bmp", 480, 96, 5, 1, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("보스2 왼쪽 공격", "image/witchd_attack.bmp", 960, 96, 10, 1, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("보스2 오른쪽", "image/witchl_idle.bmp", 720, 144, 5, 1, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("보스2 오른쪽 공격", "image/witchl_magic.bmp", 2592, 144, 18, 1, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("보스2 왼쪽 불렛", "image/witchd_bullet2.bmp", 18, 18, true, RGB(0, 255, 255));
		IMAGEMANAGER->addImage("보스2 왼쪽 불렛2", "image/witchd_bullet.bmp", 33, 33, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("보스2 왼쪽 불렛2 배경", "image/witchd_bullet_back.bmp", 150, 480, false, RGB(0, 0, 0));
		IMAGEMANAGER->addImage("보스2 오른쪽 불렛", "image/witchl_bullet.bmp", 40, 40, true, RGB(255, 0, 255));

		IMAGEMANAGER->addFrameImage("보스3-1 기본", "image/rell1_idle.bmp", 480, 192, 5, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("보스3-1 땅", "image/rell1_ground.bmp", 1152, 192, 12, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("보스3-1 공격", "image/rell1_fire.bmp", 960, 192, 10, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("보스3-1 기도", "image/rell1_pray.bmp", 480, 96, 5, 1, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("보스3-1 기도 불꽃", "image/rell1_pray_effect3.bmp", 6000, 600, 10, 1, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("보스3-1 땅 왼쪽", "image/rell1_sword_left.bmp", 96, 192, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("보스3-1 땅 오른쪽", "image/rell1_sword_right.bmp", 96, 192, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("보스3 불렛", "image/rell_bullet.bmp", 30, 32, true, RGB(255, 0, 255));

		IMAGEMANAGER->addFrameImage("보스3-2", "image/rell2_appear.bmp", 576, 96, 5, 1, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("보스3-2 눈꽃", "image/rell_snowflake.bmp", 255, 255, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("보스3-2 눈꽃2", "image/rell_snowflake3.bmp", 510, 255, 2, 1, true, RGB(255, 0, 255));

		IMAGEMANAGER->addFrameImage("보스3-3 기본", "image/rell3_idle.bmp", 1800, 480, 5, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("보스3-3 땅", "image/rell3_ground.bmp", 4320, 480, 12, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addFrameImage("보스3-3 공격", "image/rell3_attack.bmp", 2520, 480, 7, 2, true, RGB(255, 0, 255));
		IMAGEMANAGER->addImage("보스3-3 땅 공격", "image/rell3_sword.bmp", 288, 288, true, RGB(255, 0, 255));

		IMAGEMANAGER->addImage("보스3 죽음", "image/rell_defeated.bmp", 96, 96, true, RGB(255, 0, 255));
	}

	{
		SOUNDMANAGER->addSound("일반 맵", "sound/stage1.ogg", true, true);
		SOUNDMANAGER->addSound("보스 전 맵", "sound/stage2.ogg", true, true);
		SOUNDMANAGER->addSound("보스1 맵", "sound/battle1.ogg", true, true);
		SOUNDMANAGER->addSound("보스2 맵", "sound/battle2.ogg", true, true);
		SOUNDMANAGER->addSound("보스3 맵", "sound/battle3.ogg", true, true);

		SOUNDMANAGER->addSound("점프", "sound/jump.wav", false, false);
		SOUNDMANAGER->addSound("공격", "sound/leaf.wav", false, false);
		SOUNDMANAGER->addSound("죽음", "sound/death.wav", false, false);
		SOUNDMANAGER->addSound("맞음", "sound/hurt.wav", false, false);
		SOUNDMANAGER->addSound("pray", "sound/fairy.wav", false, false);

		SOUNDMANAGER->addSound("monkey", "sound/monkey_atk.wav", false, false);
		SOUNDMANAGER->addSound("bak1", "sound/bakman.wav", false, false);

		SOUNDMANAGER->addSound("witchleft", "sound/spell2.wav", false, false);
		SOUNDMANAGER->addSound("witchright", "sound/spell1.wav", false, false);

		SOUNDMANAGER->addSound("rellball", "sound/ghost.wav", false, false);
		SOUNDMANAGER->addSound("rellground", "sound/rellball.wav", false, false);
		SOUNDMANAGER->addSound("rellpray", "sound/blowfish.wav", false, true);
		SOUNDMANAGER->addSound("rellexp", "sound/explosion2.wav", false, false);
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

	_background = IMAGEMANAGER->findImage("배경");
	_backgroundPixel = IMAGEMANAGER->findImage("배경 픽셀");

	_shakeCount = 0;
	_isShaking = _isGameOver = false;

	EFFECTMANAGER->addEffect("left", "image/momo_effect_left.bmp", 768, 120, 192, 120, 1, 1.0f, 4);
	EFFECTMANAGER->addEffect("left2", "image/momo_effect_left2.bmp", 768, 120, 192, 120, 1, 1.0f, 4);
	EFFECTMANAGER->addEffect("right", "image/momo_effect_right.bmp", 768, 120, 192, 120, 1, 1.0f, 4);
	EFFECTMANAGER->addEffect("right2", "image/momo_effect_right2.bmp", 768, 120, 192, 120, 1, 1.0f, 4);
	EFFECTMANAGER->addEffect("left hit", "image/momo_effect_hit_left.bmp", 672, 64, 96, 64, 1, 1.0f, 2);
	EFFECTMANAGER->addEffect("left hit2", "image/momo_effect_hit_left2.bmp", 192, 32, 32, 32, 1, 1.0f, 3);
	EFFECTMANAGER->addEffect("right hit", "image/momo_effect_hit_right.bmp", 672, 64, 96, 64, 1, 1.0f, 2);
	EFFECTMANAGER->addEffect("right hit2", "image/momo_effect_hit_right2.bmp", 192, 32, 32, 32, 1, 1.0f, 3);
	EFFECTMANAGER->addEffect("pray", "image/momo_effect_pray.bmp", 264, 66, 66, 66, 1, 0.5f, 2);

	EFFECTMANAGER->addEffect("보스2", "image/witchl_bullet2_effect.bmp", 1800, 300, 300, 300, 1, 0.8f, 2);
	
	EFFECTMANAGER->addEffect("보스1 기도 전", "image/rell1_pray_effect.bmp", 100, 21, 25, 21, 1, 0.2f, 4);

	// ==========================================
	// ## 카메라 중점 초기화 ##
	// ==========================================
	CAMERA->setPosition(_player->getX(), _player->getY());

	return S_OK;
}

//메모리 해제
void playGround::release()
{
	SAFE_DELETE(_player);
	SAFE_DELETE(_sm);
	SAFE_DELETE(_pixel);
}

//연산
void playGround::update()
{
	gameNode::update();

	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		TIMEMANAGER->setDebug();
	}

	_player->update();
	if (!_isGameOver) 
	{
		_pixel->update();
		_sm->update();

		if (_player->getIsCameraShaking())
		{
			CAMERA->setIsShaking(true);
			++_shakeCount;
			if (_shakeCount > 10)
			{
				_shakeCount = 0;
				CAMERA->setIsShaking(false);
				_player->setIsCameraShaking(false);
			}
		}

		EFFECTMANAGER->update();

		if (_player->getIsDead())
		{
			_isGameOver = true;
		}
	}

	// ==========================================
	// ## 카메라 중점 업데이트 ##
	// ==========================================
	if (CAMERA->getIsShaking()) CAMERA->cameraShake();
	else CAMERA->setPosition(_player->getX(), _player->getY());	
}

//그리기 전용
void playGround::render()
{	
	PatBlt(getMemDC(), 0, 0, getMemDCWidth(), getMemDCHeight(), WHITENESS);
	//=================================================
	if (!_isGameOver)
	{
		_sm->render();
		EFFECTMANAGER->render();
		//TIMEMANAGER->render(getMemDC());

		for (int i = 0; i < _player->getHP(); i++)
		{
			IMAGEMANAGER->findImage("목숨")->render(getMemDC(), CAMERA->getLeft() + 35 * (i + 1), 30);
		}
	}
	else
	{
		IMAGEMANAGER->findImage("까만 배경")->render(getMemDC(), CAMERA->getLeft(), 0);
		if (!SOUNDMANAGER->isPlaySound("죽음")) IMAGEMANAGER->findImage("게임오버")->render(getMemDC(), CAMERA->getLeft() + (WINSIZEX - IMAGEMANAGER->findImage("게임오버")->getWidth()) /2, 100);
	}
	_player->render();

	//=============================================
	_backBuffer->render(getHDC(), 0 + CAMERA->getShakeNumber(), 0,
		CAMERA->getLeft(), CAMERA->getTop(),
		CAMERA->getWidth(), CAMERA->getHeight());
}
