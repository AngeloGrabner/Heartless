#pragma once
#include "SDL.h"
#include <unordered_map>
#include <string>
#include <bitset>
#define BUTTON_COUNT 5
class Input
{
public:
	enum Layer
	{
		INTERN,
		UI,
		EDITOR,
		GAME,
		LAYER_LAST // dont use me, im just hier for convenence 
	};
	enum MouseButton
	{
		LMB,
		RMB,
		MMB,
		X1B,
		X2B
	};	
	enum KeyState
	{
		UP = 0,
		DOWN = 1,
		HOLD = 2,
	};
	struct Key
	{
	private: 
		std::bitset<Layer::LAYER_LAST> handled;
	public:
		KeyState ks = UP;
		SDL_Keymod km = SDL_Keymod::KMOD_NONE;
		Key() = default;
		Key(int keystate, int keymod) : ks((KeyState)keystate), km((SDL_Keymod)keymod) {}
		Key(KeyState keystate, SDL_Keymod keymod) : ks(keystate), km(keymod) {}

		bool Up() const { return ks == UP; }
		bool Down() const { return ks == DOWN; }
		bool Hold() const { return ks == HOLD; }
		bool Shift() const { return (km & KMOD_SHIFT) == KMOD_LSHIFT || (km & KMOD_SHIFT) == KMOD_RSHIFT;}
		bool Alt() const { return (km & KMOD_ALT) == KMOD_LALT || (km & KMOD_ALT) == KMOD_RALT; }
		bool Ctrl() const { return (km & KMOD_CTRL) == KMOD_LCTRL || (km & KMOD_CTRL) == KMOD_RCTRL; }
		friend Input;
	};
	using MouseState = KeyState;
	struct Mouse // convenence wrapper 
	{
		MouseState mouseState;
		Mouse(MouseState ms) : mouseState(ms) {};
		bool Down() const { return mouseState == DOWN; }
		bool Up() const { return mouseState == UP; }
		bool Hold() const { return mouseState == HOLD; }
	};
	
private:
	static std::unordered_map<int,Key> sKeys;
	static SDL_Point sPos;
	static SDL_Point sRelativePos;
	static std::pair<MouseState,bool> sButtons[BUTTON_COUNT];
	static std::string sTextInput;
	static bool sKeyboardMode; //true: TextMode, false: KeyMode, false by default
	static SDL_Point sWheel;
public:
	static void Init();
	static void Update();
	static void EventHandler(const SDL_Event* e);
	static Key GetKey(SDL_KeyCode key);
	static SDL_Point GetMousePos();
	static SDL_Point GetRelativeMousePos();
	static Mouse GetMouse(MouseButton mb);
	static const std::string& GetTextChunk(); //returns the keys pressed per frame as a string
	static void Handled(SDL_KeyCode key);
	static void Handled(MouseButton mb);
	static void SetKeyboardMode(bool TextMode); // false means key mode
	static bool GetKeyboardMode(); //true: TextMode, false: KeyMode
	static SDL_Point GetWheel();
};