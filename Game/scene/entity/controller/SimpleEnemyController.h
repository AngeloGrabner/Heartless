#pragma once
#include "ActionController.h"

class SimpleEnemyController final : public ActionController
{
public:
	void Update(Entity* owner) override;

	template<class Archive>
	void save(Archive& ar) const;


	template<class Archive>
	void load(Archive& ar);
};

CEREAL_REGISTER_TYPE(SimpleEnemyController);

template<class Archive>
inline void SimpleEnemyController::save(Archive& ar) const
{
	ar(cereal::base_class<ActionController>(this));
}

template<class Archive>
inline void SimpleEnemyController::load(Archive& ar)
{
	ar(cereal::base_class<ActionController>(this));
}
