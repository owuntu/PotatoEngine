#pragma once

struct GLFWwindow;

namespace PotatoEngine
{
class Game
{
public:
	bool Init();
	int Run();
	void Reset();
private:
	GLFWwindow* m_window = nullptr;
};
} // PotatoEngine
