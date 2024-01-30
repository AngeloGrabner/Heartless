#include "PlayerController.h"
#include "../../../Input.h"

//todo use settings for muvement keys
void PlayerController::Update()
{
	if (Input::GetKey(SDLK_w).Hold())
	{
		mDir = { 0.0f,-1.0f };
	}
	else if (Input::GetKey(SDLK_s).Hold())
	{
		mDir = { 0.0f,1.0f };
	}
	else if (Input::GetKey(SDLK_a).Hold())
	{
		mDir = { -1.0f,0.0f };
	}
	else if (Input::GetKey(SDLK_d).Hold())
	{
		mDir = { 1.0f,0.0f };
	}
	else
		mDir = { 0.0f,0.0f };
}
