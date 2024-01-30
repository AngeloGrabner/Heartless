#pragma once
#include <cereal/cereal.hpp>

struct StatPack
{
	int hp = 0;
	int atk = 0;
	float spd = 0.0f;
	//etc

	StatPack() = default;
	StatPack(int HP, int ATK, float SPD);

	template<class Archive>
	void save(Archive& ar) const;
	template<class Archive>
	void load(Archive& ar);
};

template<class Archive>
inline void StatPack::save(Archive& ar) const
{
	ar(cereal::make_nvp("hp",hp), 
		cereal::make_nvp("atk",atk),
		cereal::make_nvp("spd",spd));
}

template<class Archive>
inline void StatPack::load(Archive& ar)
{
	ar(hp, atk, spd);
}
