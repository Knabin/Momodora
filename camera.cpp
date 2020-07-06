#include "stdafx.h"
#include "camera.h"

camera::camera()
	: _cameraInfo(NULL)
{
}

camera::~camera()
{
}


HRESULT camera::init(int width, int height, int backWidth, int backHeight)
{
	if (_cameraInfo != NULL) release();

	_cameraInfo = new CAMERA_INFO;
	_cameraInfo->width = width;
	_cameraInfo->height = height;
	_cameraInfo->backWidth = backWidth;
	_cameraInfo->backHeight = backHeight;

	_isShaking = false;

	if (_cameraInfo == NULL)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}

void camera::release()
{
	if (_cameraInfo)
	{
		SAFE_DELETE(_cameraInfo);
	}
}

bool camera::checkCameraX()
{
	if (_cameraInfo->x - _cameraInfo->width / 2 <= 0 ||
		_cameraInfo->x + _cameraInfo->width / 2 >= _cameraInfo->backWidth)
		return false;
	return true;
}

bool camera::checkCameraY()
{
	if (_cameraInfo->y - _cameraInfo->height / 2 <= 0 ||
		_cameraInfo->y + _cameraInfo->height / 2 >= _cameraInfo->backHeight)
		return false;
	return true;
}

void camera::cameraShake()
{
	_flag *= -1;
}
