#pragma once
class camera
{
	typedef struct tagCameraInfo
	{
		HDC hMemDC;
		HBITMAP hBit;
		HBITMAP hOBit;

		float x;
		float y;

		int backWidth;
		int backHeight;

		int width;	//보여질 영역 가로 크기
		int height;	//보여질 영역 세로 크기


		tagCameraInfo()
		{
			hMemDC			= NULL;
			hBit			= NULL;
			hOBit			= NULL;
			x				= 0.0f;
			y				= 0.0f;
			width			= WINSIZEX;
			height			= WINSIZEY;
			backWidth		= WINSIZEX;
			backHeight		= WINSIZEY;

		}
	} CAMERA_INFO, *LPCAMERA_INFO;

private:
	LPCAMERA_INFO _cameraInfo;

public:
	camera();
	~camera();
 
	HRESULT init(int width, int height, int backWidth, int backHeight);

	void release();

	inline HDC getMemDC() { return _cameraInfo->hMemDC; }

	bool checkCameraX();
	bool checkCameraY();

	void render(HDC hdc);

	inline void setX(int x) { _cameraInfo->x = x; }
	inline float getX() { return _cameraInfo->x; }

	inline void setY(int y) { _cameraInfo->y = y; }
	inline float getY() { return _cameraInfo->y; }

	inline void setWidth(int width) { _cameraInfo->width = width; }
	inline int getWidth() { return _cameraInfo->width; }

	inline void setHeight(int height) { _cameraInfo->height = height; }
	inline int getHeight() { return _cameraInfo->height; }

	inline int getLeft() { 
		if (!checkCameraX()) return (_cameraInfo->x <= _cameraInfo->width) ? 0 : _cameraInfo->backWidth - _cameraInfo->width;
		return _cameraInfo->x - _cameraInfo->width / 2; }
	inline int getTop() { 
		if (!checkCameraY()) return (_cameraInfo->y <= _cameraInfo->height) ? 0 : _cameraInfo->backHeight - _cameraInfo->height;
		return _cameraInfo->y - _cameraInfo->height / 2; }
};

