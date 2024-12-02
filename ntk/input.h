#pragma once

enum Axis {
	Horizontal,
	Vertical
};

class Input
{
public:
	static bool Key(int key);
	static bool KeyDown(int key);
	static float GetAxis(Axis axis);

	static void SetCallback(GLFWwindow* window);

private:
	~Input() { keyStates.clear(); }
	static void KeyCallback(GLFWwindow*, int key, int, int action, int);

private:
	static std::vector<bool> keyStates;
	static int cachedKey;
};