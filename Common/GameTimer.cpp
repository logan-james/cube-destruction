/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#include "GameTimer.h"

using namespace NCL;

GameTimer::GameTimer(void)	{
	firstPoint = std::chrono::high_resolution_clock::now();
	nowPoint   = firstPoint;
	Tick();
}

void GameTimer::resetTime()
{
	firstPoint = std::chrono::high_resolution_clock::now();
	nowPoint = firstPoint;
}

double	GameTimer::GetTotalTimeSeconds()	const {
	Timepoint time = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff;
	diff = std::chrono::duration_cast<std::chrono::microseconds>(time - firstPoint);

	return diff.count() / 1000000.0f;
};

double	GameTimer::GetTotalTimeMSec()		const {
	Timepoint time = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff;
	diff = std::chrono::duration_cast<std::chrono::microseconds>(time - firstPoint);

	return diff.count() / 1000.0f;
}

void	GameTimer::Tick() {
	Timepoint latestTime = std::chrono::high_resolution_clock::now();

	auto usec = std::chrono::duration_cast<std::chrono::microseconds>(latestTime - nowPoint);
	
	nowPoint = latestTime;

	std::chrono::duration_cast<std::chrono::seconds>(latestTime - nowPoint);

	timeDelta = usec.count() / 1000000.0f;
}