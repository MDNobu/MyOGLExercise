#pragma once
#include "QCamera.h"
class QFPSCamera :
    public QCamera
{
public:
	virtual void Walk(float delta) override;


	virtual void Strafe(float delta) override;

};

