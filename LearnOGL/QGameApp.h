#pragma once

//#include "WindowApplication.h"

class QGameApp
{
public:
	void Init();
	virtual void RenderScene();
	virtual void ShutDown();

	virtual void Update(float deltatime);
protected:
	virtual void InitAsset();

	virtual void InitGameplay();
private:
	


};

