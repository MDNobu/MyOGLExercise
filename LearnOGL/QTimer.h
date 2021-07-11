#pragma once
class QTimer
{

public:
	static QTimer& GetInstance();
private:
	QTimer() {}

public:

	void Update();
	float GetDeltaTime();

private:
	float m_LastFrame = 0.0f;

	float m_DeltatTime = 0.0f;
};

