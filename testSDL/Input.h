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
		UI_15, // ui 
		UI_14,
		UI_13,
		UI_12,
		UI_11,
		UI_10,
		UI_9,
		UI_8,
		UI_7,
		UI_6,
		UI_5,
		UI_4,
		UI_3,
		UI_2,
		UI_1,
		UI, // use the LAYER_UI macro from Ui.h instead
		EDITOR,
		GAME,
		LAYER_COUNT // dont use me, im just hier for convenence 
	};
	struct Key;
	struct Mouse;
	
	enum MouseButton;
	enum KeyState;

	using MouseState = KeyState;

private:
	using KeyMap = std::unordered_map<int, std::pair<Key, std::bitset<Layer::LAYER_COUNT>>>;
	using MouseT = std::pair<MouseState, std::bitset<Layer::LAYER_COUNT>>;

	static KeyMap sKeys;
	static SDL_Point sPos; 
	static SDL_Point sRelativePos;
	static MouseT sButtons[BUTTON_COUNT];
	static std::string sTextInput;
	static bool sKeyboardMode; //true: TextMode, false: KeyMode, false by default
	static SDL_Point sWheel;
public:
	static void Init();
	static void Update();
	static void EventHandler(const SDL_Event* e);
	static Key GetKey(SDL_KeyCode key,Layer layer);
	static SDL_Point GetMousePos();
	static SDL_Point GetRelativeMousePos();
	static Mouse GetMouse(MouseButton mb, Input::Layer layer);
	static const std::string& GetTextChunk(); //returns the keys pressed per frame as a string
	static void Handled(SDL_KeyCode key, Input::Layer layer);
	static void Handled(MouseButton mb, Input::Layer layer);
	static void SetKeyboardMode(bool TextMode); // false means key mode
	static bool GetKeyboardMode(); //true: TextMode, false: KeyMode
	static SDL_Point GetWheel();


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
		KeyState ks = UP;
		SDL_Keymod km = SDL_Keymod::KMOD_NONE;

		Key() = default;
		Key(int keystate, int keymod) : ks((KeyState)keystate), km((SDL_Keymod)keymod) {}
		Key(KeyState keystate, SDL_Keymod keymod) : ks(keystate), km(keymod) {}

		bool Up() const { return ks == UP; }
		bool DownOrHold() const { return ks == DOWN || ks == HOLD; }
		bool Down() const { return ks == DOWN; }
		bool Hold() const { return ks == HOLD; }
		bool Shift() const { return (km & KMOD_SHIFT) == KMOD_LSHIFT || (km & KMOD_SHIFT) == KMOD_RSHIFT; }
		bool Alt() const { return (km & KMOD_ALT) == KMOD_LALT || (km & KMOD_ALT) == KMOD_RALT; }
		bool Ctrl() const { return (km & KMOD_CTRL) == KMOD_LCTRL || (km & KMOD_CTRL) == KMOD_RCTRL; }
	};

	struct Mouse 
	{
		MouseState mouseState = UP;

		Mouse() = default;
		Mouse(MouseState ms) : mouseState(ms) {};

		bool Down() const { return mouseState == DOWN; }
		bool Up() const { return mouseState == UP; }
		bool Hold() const { return mouseState == HOLD; }
		bool DownOrHold() const { return mouseState == DOWN || mouseState == HOLD;}
	};
};