#include "stdafx.h"
#include "imageManager.h"


ImageManager::ImageManager()
{
}


ImageManager::~ImageManager()
{
}

HRESULT ImageManager::init()
{
	return S_OK;
}

void ImageManager::release()
{
	deleteAll();
}

Image* ImageManager::addImage(string strKey, int width, int height)
{
	Image* img = findImage(strKey);

	//만약에 내가 찾으려던 이미지가 이미 있으면 그것을 반환
	if (img) return img;

	img = new Image;

	if (FAILED(img->init(width, height)))
	{
		SAFE_DELETE(img);

		return nullptr;
	}

	//_mImageList.insert(pair<string, image*>(strKey, img));
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

Image * ImageManager::addImage(string strKey, const char * fileName, int width, int height, bool trans, COLORREF transColor)
{
	Image* img = findImage(strKey);

	//만약에 내가 찾으려던 이미지가 이미 있으면 그것을 반환
	if (img) return img;

	img = new Image;

	if (FAILED(img->init(fileName, width, height, trans, transColor)))
	{
		SAFE_DELETE(img);

		return nullptr;
	}

	//_mImageList.insert(pair<string, image*>(strKey, img));
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

Image * ImageManager::addFrameImage(string strKey, const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor)
{
	Image* img = findImage(strKey);

	//만약에 내가 찾으려던 이미지가 이미 있으면 그것을 반환
	if (img) return img;

	img = new Image;

	if (FAILED(img->init(fileName, x, y, width, height, frameX, frameY, trans, transColor)))
	{
		SAFE_DELETE(img);

		return nullptr;
	}

	//_mImageList.insert(pair<string, image*>(strKey, img));
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

Image * ImageManager::addFrameImage(string strKey, const char * fileName, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor)
{
	Image* img = findImage(strKey);

	//만약에 내가 찾으려던 이미지가 이미 있으면 그것을 반환
	if (img) return img;

	img = new Image;

	if (FAILED(img->init(fileName, width, height, frameX, frameY, trans, transColor)))
	{
		SAFE_DELETE(img);

		return nullptr;
	}

	//_mImageList.insert(pair<string, image*>(strKey, img));
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

Image * ImageManager::findImage(string strKey)
{
	mapImageIter key = _mImageList.find(strKey);

	//찾았다면
	if (key != _mImageList.end())
	{
		return key->second;
	}

	return nullptr;
}

BOOL ImageManager::deleteImage(string strKey)
{
	mapImageIter key = _mImageList.find(strKey);

	if (key != _mImageList.end())
	{
		key->second->release();
		SAFE_DELETE(key->second);
		_mImageList.erase(key);

		return true;
	}

	return false;
}

BOOL ImageManager::deleteAll()
{
	mapImageIter iter = _mImageList.begin();

	for (; iter != _mImageList.end();)
	{
		//맵 컨테이너 안에 image*가 있다면
		if (iter->second != NULL)
		{
			iter->second->release();
			SAFE_DELETE(iter->second);
			iter = _mImageList.erase(iter);
		}
		else ++iter;
	}

	_mImageList.clear();

	return true;
}

void ImageManager::render(string strKey, HDC hdc)
{
	Image* img = findImage(strKey);

	if (img) img->render(hdc);
}

void ImageManager::render(string strKey, HDC hdc, int destX, int destY)
{
	Image* img = findImage(strKey);

	if (img) img->render(hdc, destX, destY);
}

void ImageManager::render(string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	Image* img = findImage(strKey);

	if (img) img->render(hdc, destX, destY, sourX, sourY, sourWidth, sourHeight);
}
