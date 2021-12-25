#pragma once

struct GLFWwindow;

namespace PotatoEngine
{
class Camera;

class Game
{
public:
	bool Init();
	int Run();
	void Reset();

private:
	void Update();
	void ProcessInput();
	void ProcessKeyboardPress(int key);

	float m_deltaTime = 0.f;
	float m_lastFrameTime = 0.f;

	GLFWwindow* m_window = nullptr;
	Camera* m_pMainCamera = nullptr;
};
} // PotatoEngine
