#pragma once

#include "QShader.h"

class IQRenderable
{
protected:
	IQRenderable() = default;

public:
	virtual void Draw(const QShader& shader) = 0;
};

