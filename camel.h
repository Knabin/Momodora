#pragma once
#include "gameNode.h"
#include "effect.h"

class camel : public gameNode
{
private:
	image* _camel;

	animation* _ani1;	//����Ʈ �ִϸ��̼� ��
	animation* _ani2;	//�迭�� ����ϴ� �ִϸ��̼� ��
	animation* _ani3;	//�����ݺ� ��� �ִϸ��̼� ��

	effect* _effect;

public:
	camel();
	~camel();

	HRESULT init();
	void release();
	void update();
	void render();
};

