#pragma once

/**
 * Static input class, using GLFW.
 */
class Input
{
public:
	enum Axis
	{
		Horizontal,
		Vertical
	};

	static bool getKey(int key);
	static bool getKeyDown(int key);
	static float getAxis(Axis axis);

	static void setCallback(GLFWwindow* window);

private:
	~Input();
	static void keyCallback(GLFWwindow*, int key, int, int action, int);

	static std::vector<bool> keyStates;
	static int cachedKey;
};
