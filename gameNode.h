#pragma once
#include "image.h"
#include "camera.h"

class gameNode
{
private:
	image* _backBuffer;
	camera* _camera;

	void setBackBuffer();
	void setCamera();

public:
	gameNode();
	virtual ~gameNode();

	//HRESULT�� ����ũ�μ���Ʈ ���� ����� ��ȯ�� �ε�
	//S_OK, E_FAIL, SUCCDED ������ ���â�� 
	//����� �ʱ�ȭ�� �ߵƴ��� �ƴ����� �����ش�
	virtual HRESULT init();			//�ʱ�ȭ ���� �Լ�
	virtual void release();			//�޸� ���� ����
	virtual void update();			//���� ����
	virtual void render(HDC hdc);	//�׸��� ����

	//����� ������
	image* getBackBuffer() { return _backBuffer; }
	camera* getCamera() { return _camera; }

	LRESULT MainProc(HWND, UINT, WPARAM, LPARAM);
};

