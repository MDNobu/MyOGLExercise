#pragma once

//#include "WindowApplication.h"


class QGameApp
{
public:
	void Init(int width, int height);
	virtual void RenderScene();
	virtual void ShutDown();

	virtual void Update(float deltatime);
protected:
	virtual void InitAsset();

	virtual void InitGameplay();

	//void 
protected:
	int m_Width;
	int m_Height;

private:
	


};

