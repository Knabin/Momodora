#pragma once
#include "util/image/image.h"
#include <map>

class ImageManager
{
public:
	static ImageManager* getInstance()
	{
		static ImageManager instance;
		return &instance;
	}

private:
	ImageManager();
	~ImageManager();

private:
	//      map<first, second>
	typedef map<string, Image*>				mapImageList;
	typedef map<string, Image*>::iterator	mapImageIter;

private:
	mapImageList _mImageList;
	   
public:
	HRESULT init();
	void release();

	Image* addImage(string strKey, int width, int height);
	Image* addImage(string strKey, const char* fileName, int width, int height, bool trans, COLORREF transColor);

	Image* addFrameImage(string strKey, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor);
	Image* addFrameImage(string strKey, const char* fileName, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor);

	//이미지 찾는 함수 (중요)
	Image* findImage(string strKey);

	BOOL deleteImage(string strKey);
	
	BOOL deleteAll();

	void render(string strKey, HDC hdc);
	void render(string strKey, HDC hdc, int destX, int destY);
	void render(string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);

	//frameRender는 여러분이 만들어보세요~

};

