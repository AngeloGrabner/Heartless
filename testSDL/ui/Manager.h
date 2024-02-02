#pragma once
#include "Button.h"
#include "CheckBox.h"
#include "Frame.h"
#include "InputField.h"
#include "Slider.h"
#include "TextBox.h"
#include "Div.h"
#include "Label.h"

namespace ui
{
	class Manager
	{
		std::vector<std::unique_ptr<Widget>> mWidgets;
	public:
		Manager() = default;
		Manager(const Manager&) = delete;
		Manager& operator=(const Manager&) = delete;
		Manager(Manager&& other) noexcept;
		Manager& operator=(Manager&& other) noexcept;

		void Handle(const SDL_Event* e);
		void Update();
		void Draw();
		void DebugDraw();

		void Activate(const std::string& name, bool active);

		void Init(); //constructs the ui

	};
}