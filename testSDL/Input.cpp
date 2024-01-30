#include "Input.h"
#include <stdexcept>
#include <iostream>

std::unordered_map<int, Input::Key> Input::sKeys;
SDL_Point Input::sPos = { 0,0 };
std::pair<Input::MouseState, bool>  Input::sButtons[];
SDL_Point Input::sRelativePos;
std::string Input::sTextInput;
bool Input::sKeyboardMode = false;
SDL_Point Input::sWheel = { 0,0 };

void Input::Init()
{
	for (int i = 0; i < BUTTON_COUNT; i++)
	{
		sButtons[i].first = UP;
		sButtons[i].second = false;
	}
	//commen keys inited for use 
	sKeys['\r'] = {UP,KMOD_NONE};
	sKeys['\x1b'] = {UP,KMOD_NONE};
	sKeys['\t'] = { UP,KMOD_NONE };
	for (char i = ' '; i <= '@'; i++)
	{
		sKeys[i] = { UP,KMOD_NONE };
	}
	for (char i = '['; i <= '~'; i++)
	{
		sKeys[i] = { UP,KMOD_NONE };
	}
}

void Input::Update()
{
	for (auto& k : sKeys) // clears down keys
	{
		if (k.second.ks == DOWN)
			k.second.ks = HOLD;
		k.second.handled = false;
	}
	for (int i = 0; i < 5; i++)
	{
		if (sButtons[i].first == DOWN)
			sButtons[i].first = HOLD;
		sButtons[i].second = false;
	}
	sRelativePos = { 0,0 };	
	sWheel = { 0,0 };
	sTextInput.clear();
}

void Input::EventHandler(const SDL_Event* e)
{
	switch (e->type)
	{
	//case SDL_TEXTEDITING:
	//	//not sure how it works
	//	break;

	case SDL_TEXTINPUT: 
		if (sKeyboardMode)
		{
			auto text = e->text.text;
			for (int i = 0; i < SDL_TEXTINPUTEVENT_TEXT_SIZE; i++)
			{
				if (text[i] == 0)
				{
					break;
				}
				if (text[i] <= -1) // strip non ascii chars, (char)128 == -1 see: https://stackoverflow.com/questions/2980253/how-to-convert-utf-8-to-ascii-in-c
				{
					continue;
				}
				sTextInput.push_back(text[i]);
			}
		}
		break;
	case SDL_KEYDOWN:
		if (!sKeyboardMode)
			sKeys[e->key.keysym.sym] = { DOWN,e->key.keysym.mod };
		else if (e->key.keysym.sym == SDLK_RETURN)
			sTextInput.push_back('\n');
		else if (e->key.keysym.sym == SDLK_BACKSPACE)
			sTextInput.push_back(8);
		break;
	case SDL_KEYUP:
		if (!sKeyboardMode)
			sKeys[e->key.keysym.sym] = { UP, SDL_Keymod::KMOD_NONE };
		break;
	case SDL_MOUSEMOTION:
		sPos.x = e->motion.x;
		sPos.y = e->motion.y;

		sRelativePos.x = e->motion.xrel;
		sRelativePos.y = e->motion.yrel;
		break;
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		sPos.x = e->button.x;
		sPos.y = e->button.y;
		{
			MouseState ms = UP;
			if (e->button.state == SDL_PRESSED)
			{
				ms = (MouseState)(DOWN);
			}
			else if (e->button.state == SDL_RELEASED)
			{
				ms = UP;
			}

			switch (e->button.button)
			{
			case SDL_BUTTON_LEFT:
				sButtons[LMB].first = ms;
				break;
			case SDL_BUTTON_MIDDLE:
				sButtons[MMB].first = ms;
				break;
			case SDL_BUTTON_RIGHT:
				sButtons[RMB].first = ms;
				break;
			case SDL_BUTTON_X1:
				sButtons[X1B].first = ms;
				break;
			case SDL_BUTTON_X2:
				sButtons[X2B].first = ms;
				break;
			}
		}
		break;
	case SDL_MOUSEWHEEL:
		sWheel.x += e->wheel.x;
		sWheel.y += e->wheel.y;
		break;
	default:
		break;
	}
}

Input::Key Input::GetKey(SDL_KeyCode key)
{
	try
	{
		return sKeys.at(key);
	}
	catch (std::out_of_range& e)
	{
		return Input::Key(UP, SDL_Keymod::KMOD_NONE);
	}
}

SDL_Point Input::GetMousePos()
{
	return sPos;
}

SDL_Point Input::GetRelativeMousePos()
{
	return sRelativePos;
}

Input::Mouse Input::GetMouse(MouseButton mb)
{
	return Mouse(!sButtons[mb].second ? sButtons[mb].first : MouseState::UP);
}

const std::string& Input::GetTextChunk()
{
	return sTextInput;
}

void Input::Handled(SDL_KeyCode key)
{
	try
	{
		sKeys.at(key).handled = true;
	}
	catch (std::out_of_range& e)
	{ }
}

void Input::Handled(MouseButton mb)
{
	sButtons[mb].second = true;
}

void Input::SetKeyboardMode(bool TextMode)
{
	sKeyboardMode = TextMode;
}
bool Input::GetKeyboardMode()
{
	return sKeyboardMode;
}

SDL_Point Input::GetWheel()
{
	return sWheel;
}
