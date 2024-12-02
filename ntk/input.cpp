#include "precomp.h"
#include "input.h"

// Initialize static members
std::vector<bool> Input::keyStates(350, false);  // 350 is GLFW_KEY_LAST + 1
int Input::cachedKey = 0;

// --------------------------------------------
// Retrieves the key that is being held down.
// --------------------------------------------
bool Input::Key(int key)
{
    // check for an invalid key
    if (key < 0 || key >= static_cast<int>(keyStates.size())) return false;

    return keyStates[key];
}

// --------------------------------------------
// Retrieves the key that got pressed down just once
// --------------------------------------------
bool Input::KeyDown(int key)
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

// --------------------------------------------
// Returns the input axis, either Horizontal or Vertical in a float value from -1 to 1.
// --------------------------------------------
float Input::GetAxis(Axis axis)
{
    switch (axis)
    {
        // handle horizontal axis
    case Horizontal:
        if (Key(GLFW_KEY_A) || Key(GLFW_KEY_LEFT)) return -1.0f;
        if (Key(GLFW_KEY_D) || Key(GLFW_KEY_RIGHT)) return 1.0f;
        break;

        // handle vertical axis
    case Vertical:
        if (Key(GLFW_KEY_W) || Key(GLFW_KEY_UP)) return -1.0f;
        if (Key(GLFW_KEY_S) || Key(GLFW_KEY_DOWN)) return 1.0f;
        break;
    }

    // return 0 by default
    return 0.0f;
}

// --------------------------------------------
// Set's the key callback of the GLFW library.
// --------------------------------------------
void Input::SetCallback(GLFWwindow* window)
{
	glfwSetKeyCallback(window, KeyCallback);
}

// --------------------------------------------
// Registers the key callback to the corresponding GLFW events.
// -------------------------------------------
void Input::KeyCallback(GLFWwindow*, int key, int, int action, int)
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