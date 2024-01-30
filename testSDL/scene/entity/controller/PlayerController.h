#pragma once
#include "ActionController.h"

class PlayerController : public ActionController
{
public:
	PlayerController() = default;

	void Update() override;

	template<class Archive>
	void save(Archive& ar) const;
	template<class Archive>
	void load(Archive& ar);
};

CEREAL_REGISTER_TYPE(PlayerController);

template<class Archive>
inline void PlayerController::save(Archive& ar) const
{
	ar(cereal::base_class<ActionController>(this));
}

template<class Archive>
inline void PlayerController::load(Archive& ar)
{
	ar(cereal::base_class<ActionController>(this));
}
