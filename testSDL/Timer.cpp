#include "Timer.h"
#include "Utility.h"

Milliseconds Timer::sLastGlobalDt = 0;

Timer::Timer(const std::string& loggingName)
{
	mName = loggingName;
	Start();
}

Timer::~Timer()
{
#ifdef TIMER_LOGGING
	auto dt = Stop();
	auto msg = to_string();
	LOG_PUSH(msg);
#endif 
}

void Timer::Start()
{
	mStart = SDL_GetTicks64();
}

Milliseconds Timer::Stop()
{
	mStop = SDL_GetTicks64();
	return mStop-mStart;
}

void Timer::SetGolbalDT(Milliseconds dt)
{
	sLastGlobalDt = dt;
}

Milliseconds Timer::GetGlobalDT()
{
	return sLastGlobalDt;
}

Milliseconds Timer::GetDT()
{
	return mStop - mStart;
}

std::string Timer::to_string()
{
	return "Timer: " + mName + ' ' + std::to_string(mStop - mStart) + "ms";
}



Duration::Duration(Milliseconds threshhold)
	: mThreshold(threshhold), mTime(0)
{
}

bool Duration::Update()
{
	mTime += Timer::GetGlobalDT();
	return mThreshold >= mTime;
}

bool Duration::Get() const
{
	return mThreshold >= mTime;
}

Millis Duration::GetTreshold() const
{
	return mThreshold;
}

void Duration::Reset()
{
	mTime = 0;
}

void Duration::SetTime(Milliseconds ms)
{
	mTime = ms;
}

void Duration::SetThreshold(Milliseconds ms)
{
	mThreshold = ms;
}


