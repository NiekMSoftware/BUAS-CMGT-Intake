#include "precomp.h"
#include "Input.h"

// Initialize static members
std::vector<bool> Input::keyStates(350, false);  // 350 is GLFW_KEY_LAST + 1
int Input::cachedKey = 0;

Input::~Input()
{
	keyStates.clear();
}

bool Input::getKey(const int key)
{
	// check for an invalid key
	if (key < 0 || key >= static_cast<int>(keyStates.size())) return false;

	return keyStates[key];
}

bool Input::getKeyDown(const int key)
{
	// check for an invalid key
	if (key < 0 || key >= static_cast<int>(keyStates.size())) return false;

	// only return true on the first press down
	if (keyStates[key] && key != cachedKey)
	{
		cachedKey = key;
		return true;
	}

	return false;
}

float Input::getAxis(const Axis axis)
{
	switch (axis)
	{
		// handle horizontal axis
	case Horizontal:
		if (getKey(GLFW_KEY_A) || getKey(GLFW_KEY_LEFT)) return -1.0f;
		if (getKey(GLFW_KEY_D) || getKey(GLFW_KEY_RIGHT)) return 1.0f;
		break;

		// handle vertical axis
	case Vertical:
		if (getKey(GLFW_KEY_W) || getKey(GLFW_KEY_UP)) return 1.0f;
		if (getKey(GLFW_KEY_S) || getKey(GLFW_KEY_DOWN)) return -1.0f;
		break;
	}

	// return 0 by default
	return 0.0f;
}

void Input::setCallback(GLFWwindow* window)
{
	glfwSetKeyCallback(window, keyCallback);
}

void Input::keyCallback(GLFWwindow*, const int key, int, const int action, int)
{
	// set keys true or false based on the action
	if (action == GLFW_PRESS) keyStates[key] = true;
	else if (action == GLFW_RELEASE)
	{
		keyStates[key] = false;

		// reset the cached key
		if (key == cachedKey)
			cachedKey = 0;
	}
}
