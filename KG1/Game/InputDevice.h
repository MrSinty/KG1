#pragma once

#include "../Support/DXInclude.h"
#include "Game.h"

#include <unordered_map>

class InputDevice
{
    std::unordered_map<WPARAM, bool> keyStates;
    int mouseX = 0, mouseY = 0;

public:
    void OnKeyDown(WPARAM key);
    void OnKeyUp(WPARAM key);
    void OnMouseMove(int x, int y);

    bool IsKeyDown(WPARAM key) const;

    inline std::pair<int, int> GetMousePosition() const { return { mouseX, mouseY }; }

public:
    InputDevice(const InputDevice&) = delete;
    InputDevice& operator=(const InputDevice&) = delete;

    inline static InputDevice& Get()
    {
        static InputDevice instance;
        return instance;
    }

private:
    InputDevice() = default;
};

