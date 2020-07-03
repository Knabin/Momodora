#pragma once
#include "gameNode.h"
#include "effect.h"

class camel : public gameNode
{
private:
	image* _camel;

	animation* _ani1;	//디폴트 애니메이션 용
	animation* _ani2;	//배열로 재생하는 애니메이션 용
	animation* _ani3;	//구간반복 재생 애니메이션 용

	effect* _effect;

public:
	camel();
	~camel();

	HRESULT init();
	void release();
	void update();
	void render();
};

