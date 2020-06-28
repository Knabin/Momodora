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
	HDC hdc = GetDC(_hWnd);

	_cameraInfo = new CAMERA_INFO;
	_cameraInfo->hMemDC = CreateCompatibleDC(hdc);
	_cameraInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, backWidth, backHeight);
	_cameraInfo->hOBit = (HBITMAP)SelectObject(_cameraInfo->hMemDC, _cameraInfo->hBit);
	_cameraInfo->width = width;
	_cameraInfo->height = height;
	_cameraInfo->backWidth = backWidth;
	_cameraInfo->backHeight = backHeight;

	if (_cameraInfo == NULL)
	{
		release();
		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);
	return S_OK;
}

void camera::release()
{
	if (_cameraInfo)
	{
		SelectObject(_cameraInfo->hMemDC, _cameraInfo->hOBit);
		DeleteObject(_cameraInfo->hBit);
		DeleteDC(_cameraInfo->hMemDC);

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

void camera::render(HDC hdc)
{
	BitBlt(hdc,
		0,
		0,
		_cameraInfo->width,
		_cameraInfo->height,
		_cameraInfo->hMemDC,
		getLeft(), getTop(),
		SRCCOPY);
}
