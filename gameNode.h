#pragma once
#include "image.h"

class gameNode
{
private:
	image* _backBuffer;
	void setBackBuffer();

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

	LRESULT MainProc(HWND, UINT, WPARAM, LPARAM);
};

