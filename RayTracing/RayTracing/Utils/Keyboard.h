#pragma once
#include "CustomKeyCodes.h"

class Keyboard {
public:
	bool m_currentStates[KeyCodes::K_CODE_MAX]{ false };

	inline void updateKeystate(const KeyCodes key, bool isPressed) { m_currentStates[key] = isPressed; }
	
	inline bool isKeyDown(const KeyCodes key) const { return m_currentStates[key]; }
};
