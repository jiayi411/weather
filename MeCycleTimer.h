#pragma once
#include "MeFoundation/MeClockTimer.h"

/**
* @brief Cycled timer, a cycle could be hour, day or week,
		 millseconds is a unit
*/
template< MeTimeValue kCycle, class Timer = MeClockTimer >
class MEFOUNDATION_ENTRY MeCycleTimer : public Timer
{
// typedefs.
public:
	typedef MeCycleTimer<kCycle,Timer> ClassType;

// Construction.
public:
	/// Constructor.
	MeCycleTimer(){
		m_fRatio = 1.f;
	}
	
	/// Destructor.
	virtual ~MeCycleTimer(){}

	/// Copy constructor.
	MeCycleTimer( const ClassType& kClass ){}

// Overload operator.
public:
	/// Assignment operator.
	const MeCycleTimer& operator=( const ClassType& kClass ){ return *this; }

// Override.
public:
	virtual void UpdateElapsedTime(){
		Timer::UpdateElapsedTime();
		m_kElapsed = GetElapsed() % kCycle;
	}

	virtual MeTimeValue GetElapsed() const{
		return Timer::GetElapsed() * GetTimeRatio();
	}

// Methods.
public:
	float GetTimeRatio() const { return m_fRatio; }
	void SetTimeRatio( float fRatio ){ m_fRatio = fRatio; }

// Members.
protected:
	float m_fRatio;
};

// Hour
typedef MeCycleTimer<3600000, MeClockTimer> MeHourTimer;

// Day
typedef MeCycleTimer<3600000 * 24, MeClockTimer> MeDayTimer;
