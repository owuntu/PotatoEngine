#pragma once

#include <memory>
struct GLFWwindow;

namespace PotatoEngine
{
class Camera;

class Game
{
public:
	virtual bool Init();
	virtual int Run();
	virtual void Reset();

	int ScreenWidth() const;
	int ScreenHeight() const;

	virtual ~Game();
protected:
	virtual void Update();
	virtual void Render();
	
	virtual void ProcessInput();
	void ProcessKeyboardPress(int key);
	void MouseCallBack(GLFWwindow* window, double xPos, double yPos);

	float m_deltaTime = 0.f;
	float m_lastFrameTime = 0.f;

	GLFWwindow* m_window = nullptr;
	std::shared_ptr<Camera> m_pMainCamera = nullptr;


	float m_mouseLastX;
	float m_mouseLastY;
};
} // PotatoEngine
