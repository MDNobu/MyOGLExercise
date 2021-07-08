#pragma once

//#include "WindowApplication.h"

class QGameApp
{
public:
	void Init();
	virtual void RenderScene();
	virtual void ShutDown();
protected:
	virtual void InitAsset();
private:
	void InitPipeline();


};

