#pragma once
#include <MeFoundation/MeTimelineEvent.h>

class MEFOUNDATION_ENTRY MeTimelineShadeEvent : public MeTimelineEventBase
{
// Defines.
public:
	enum EShadeEventState
	{
		ESES_Ready,
		ESES_BeginToFull,
		ESES_Full,
		ESES_FullToEnd,
		ESES_End,
		ESES_Count
	};

// Construction.
public:
	/// Constructor.
	MeTimelineShadeEvent(){
		m_kToFull = InvalidEventTime;
		m_kToEnd = InvalidEventTime;
	}

	/// Copy constructor.
	MeTimelineShadeEvent( const MeTimelineShadeEvent& kClass ){
		m_kToFull = kClass.m_kToFull;
		m_kToEnd = kClass.m_kToEnd;
	}

// Overload operator.
public:
	/// Destructor.
	virtual ~MeTimelineShadeEvent(){}

	/// Assignment operator.
	const MeTimelineShadeEvent& operator=( const MeTimelineShadeEvent& kClass ){
		m_kToFull = kClass.m_kToFull;
		m_kToEnd = kClass.m_kToEnd;
		return *this;
	}

// Override.
public:
	virtual void InitializeEvent(){ UpdateValueForPerformance(); }

	
// Override.
protected:
	virtual void UpdateEventInRunning( const int64& kTime ){
		if( kTime <= ( m_kToFull + FLOAT_TOLERANCE ) )
		{ UpdateBeginToFull( kTime ); }
		else if( kTime > ( m_kToFull + FLOAT_TOLERANCE ) && kTime <= ( m_kToEnd + FLOAT_TOLERANCE ) )
		{ UpdateInFull( kTime ); }
		else if( kTime > ( m_kToEnd + FLOAT_TOLERANCE ) && kTime <= ( GetEndTime() + FLOAT_TOLERANCE ) )
		{ UpdateFullToEnd( kTime ); }		
	}

	virtual void UpdateBeginToFull( const int64& kTime ) = 0;

	virtual void UpdateInFull( const int64& kTime ) = 0;

	virtual void UpdateFullToEnd( const int64& kTime ) = 0;

// Methods.
public:
	const int64& GetToFull() const { return m_kToFull;}
	void SetToFull( const int64& val ) { m_kToFull = val; }

	const int64& GetToEnd() const { return m_kToEnd; }
	void SetToEnd( const int64& val ) { m_kToEnd = val; }

// Methods.
protected:
    /// for efficiency purpose
	void UpdateValueForPerformance(){
		m_f1_Divide_ToFull_Minus_Start = 1.f / ( float )( GetToFull() - GetStartTime() );
		m_f1_Divide_End_Minus_ToEnd = 1.f / ( float )( GetEndTime() - GetToEnd() );
	}

	float Get1_Divide_ToFull_Minus_Start() const { return m_f1_Divide_ToFull_Minus_Start; }
	float Get1_Divide_End_Minus_ToEnd() const { return m_f1_Divide_End_Minus_ToEnd; }

// Members.
protected:
	int64 m_kToFull;

	int64 m_kToEnd;

	/// 1 / ( ToFull - Start )。
	float m_f1_Divide_ToFull_Minus_Start;

	/// 1 / ( End - ToEnd )。
	float m_f1_Divide_End_Minus_ToEnd;
};
