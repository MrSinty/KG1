#include "InputDevice.h"
#include <iostream>

void InputDevice::OnKeyDown(WPARAM key)
{
    keyStates[key] = true;
    std::cout << "Key Down: " << key << std::endl;
}

void InputDevice::OnKeyUp(WPARAM key)
{
    keyStates[key] = false;
    std::cout << "Key Up: " << key << std::endl;
}

bool InputDevice::IsKeyDown(WPARAM key) const
{
    auto it = keyStates.find(key);
    return (it != keyStates.end()) ? it->second : false;
}

void InputDevice::OnMouseMove(int x, int y)
{
    mouseX = x;
    mouseY = y;
    //std::cout << "Mouse Move: X=" << x << " Y=" << y << std::endl;
}
