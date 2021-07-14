#pragma once
#include "QGameApp.h"
#include "QModel.h"
#include "QGameApp.h"
#include <memory>
#include "QShader.h"
#include "QCamera.h"

class QHelloModel :
    public QGameApp
{
public:
	QHelloModel();

public:
	virtual void RenderScene() override;


	virtual void ShutDown() override;


	virtual void Update(float deltatime) override;

protected:
	virtual void InitAsset() override;


	virtual void InitGameplay() override;

private:
	std::unique_ptr<QShader> m_Shader;
	std::unique_ptr<QModel> m_Model;

};

