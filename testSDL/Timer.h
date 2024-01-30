#pragma once
#include "SDL.h"
#include <string>

#define TIMER_LOGGING

using Milliseconds = Uint64;
using Millis = Milliseconds;

constexpr float MStoF(Milliseconds ms)
{
	return ms / 1000.0f;
}

class Timer
{
	Uint64 mStart = 0; 
	Uint64 mStop = 0;
	std::string mName = "";

	static Milliseconds sLastGlobalDt; 
public:
	Timer() = delete;
	Timer(const std::string& loggingName = "");
	~Timer();

	void Start();
	Milliseconds Stop();
	Milliseconds GetDT();

	static void SetGolbalDT(Milliseconds dt); // dont call unless ur main loop
	static Milliseconds GetGlobalDT();

	std::string to_string();
};

class Duration
{
	Millis mThreshold = 0;
	Millis mTime = 0;
public:
	Duration() = default;
	Duration(Milliseconds threshhold);
	bool Update(); // true if ellepsed time <= threshold
	bool Get();
	void Reset();
	void SetTime(Milliseconds ms);
	void SetThreshold(Milliseconds ms);

	template<class Archive>
	void save(Archive& ar) const;
	template<class Archive>
	void load(Archive& ar);
};

template<class Archive>
inline void Duration::save(Archive& ar) const
{
	ar(mThreshold);
}

template<class Archive>
inline void Duration::load(Archive& ar)
{
	ar(mThreshold);
}
