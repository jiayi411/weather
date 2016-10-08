#pragma once
#include "MeFoundation/MeTimelineEvent.h"

/**
* @brief A time trigger
* @template Timer: timer type
*/
template< class Timer >
class MEFOUNDATION_ENTRY MeTimeline : public MeRefObject
{
// Defines.
public:
	typedef MeTimeline< Timer > ThisClass;
	typedef std::vector<MeTimelineEventBase*> TimelineEvents;
	typedef TimelineEvents::iterator TimelineEventsIterator;
	typedef TimelineEvents::const_iterator TimelineEventsConstIterator;

// Construction.
public:
	/// Constructor.
	MeTimeline(){}
	
	/// Destructor.
	~MeTimeline(){}

	/// Copy constructor.
	MeTimeline( const ThisClass& kClass ){
		m_akEvents = kClass.m_akEvents;
		m_kTimer = kClass.m_kTimer;
	}

// Overload operator.
public:
	/// Assignment operator.
	const MeTimeline& operator=( const ThisClass& kClass ){	
		m_akEvents = kClass.m_akEvents;
		m_kTimer = kClass.m_kTimer;
		return *this; 
	}

// Methods.
public:
	/// add time event
	void PushEvent( MeTimelineEventBase* pEvent );

	/// sort all events by time
	void SortEventsByTime();

	/// clear events
	void ClearAllEvents();

	/// update time line
	void UpdateTimeline();

	/// render event
	void RenderTimeline();

	/// get timer
	const Timer& GetTimer() const{ return m_kTimer; }
	Timer* GetTimer(){ return &m_kTimer; }

	/// get evetns
	const TimelineEvents& GetEvents() const { return m_akEvents; }
	TimelineEvents& GetEvents() { return m_akEvents; }

	/// start
	void StartTime( const MeTimeValue& kTimePassed );

// Static Methods.
protected:
	static bool EventsSortAlgorism( MeTimelineEventBase* pEvent1, MeTimelineEventBase* pEvent2 )
	{
		return pEvent1->GetStartTime() < pEvent2->GetStartTime();
	}

// Members.
protected:
	Timer m_kTimer;
	TimelineEvents m_akEvents;
};
//////////////////////////////////////////////////////////////////////////
template< class Timer >
void MeTimeline< Timer >::PushEvent( MeTimelineEventBase* pEvent )
{
	m_akEvents.push_back( pEvent );
}
//////////////////////////////////////////////////////////////////////////
template< class Timer >
void MeTimeline< Timer >::SortEventsByTime()
{
	std::sort( m_akEvents.begin(), m_akEvents.end(), EventsSortAlgorism );
}
//////////////////////////////////////////////////////////////////////////
template< class Timer >
void MeTimeline< Timer >::ClearAllEvents()
{
	TimelineEventsConstIterator kIter = m_akEvents.begin();	
	for( ; kIter != m_akEvents.end(); ++ kIter )
	{
		( *kIter )->DestroyEvent();
		MeDelete ( *kIter );
	}
	m_akEvents.clear();
}
//////////////////////////////////////////////////////////////////////////
template< class Timer >
void MeTimeline< Timer >::UpdateTimeline()
{
	if( !m_kTimer.IsStarted() )
	{ return; }

	m_kTimer.UpdateElapsedTime();

	if( m_akEvents.empty() )
		return;

	TimelineEventsConstIterator kIter = m_akEvents.begin();	
	for( ; kIter != m_akEvents.end(); ++ kIter )
	{
		( *kIter )->UpdateEvent( m_kTimer.GetElapsed() );			
	}
}
//////////////////////////////////////////////////////////////////////////
template< class Timer >
void MeTimeline< Timer >::RenderTimeline()
{
	if( !m_kTimer.IsStarted() )
	{ return; }

	if( m_akEvents.empty() )
		return;

	TimelineEventsConstIterator kIter = m_akEvents.begin();	
	for( ; kIter != m_akEvents.end(); ++ kIter )
	{
		( *kIter )->RenderEvent();
	}
}
//////////////////////////////////////////////////////////////////////////
template< class Timer >
void MeTimeline< Timer >::StartTime( const MeTimeValue& kTimePassed )
{
	m_kTimer.Start( kTimePassed );
}
//////////////////////////////////////////////////////////////////////////
