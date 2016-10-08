#pragma once
#include "MeFoundation/MeRefObject.h"

/**
* @brief Base class for timeline
*/
class MEFOUNDATION_ENTRY MeTimelineEventBase : public MeRefObject
{
// Defines.
public:
	/// event status
	enum EEventStates
	{
		EES_Ready,
		EES_Running,
		EES_Pause,
		EES_End,
		EES_Count
	};

	/// invalide event time
	enum { InvalidEventTime = 0xffffffff };

	/// callback
	typedef void (*DataCallback)( MeTimelineEventBase* );

// Construction.
public:
	/// Constructor.
	MeTimelineEventBase(){
		m_eEventState = EES_Count;
		m_kStartTime = InvalidEventTime;
		m_kEndTime = InvalidEventTime;
		m_pfResetEventWithDataCallback = NULL;
		m_pfDoFirstInRunning = NULL;
		m_pfUpdateEventInReady = NULL;
		m_pfUpdateEventInRunning = NULL;
		m_pfUpdateEventInEnd = NULL;
		m_bFirstInRunning = true;
	}
	
	/// Copy constructor.
	MeTimelineEventBase( const MeTimelineEventBase& kClass ){}

// Overload operator.
public:
	/// Assignment operator.
	const MeTimelineEventBase& operator=( const MeTimelineEventBase& kClass ){ return *this; }

// Virtual Methods.
public:
	/// Destructor.
	virtual ~MeTimelineEventBase(){}	

	/// Initialize.
	virtual void InitializeEvent() = 0;

	/// Reset states.
	virtual void ResetEvent(){ 
		if( m_pfResetEventWithDataCallback )
		{ m_pfResetEventWithDataCallback( this ); }
	}

	/// Destroy.
	virtual void DestroyEvent() = 0;

	/// Update event when a event has been triggered
	/// @return allowed to update
	virtual bool UpdateEvent( const int64& kTime ){

		if( kTime <= GetStartTime() )
		{
			SetEventState( EES_Ready );

			if( m_pfUpdateEventInReady )
			{ m_pfUpdateEventInReady( this ); }

			UpdateEventInReady( kTime );
			IsFirstInRunning( true );
			return false;
		}
		else if( kTime > GetEndTime() )
		{ 
			SetEventState( EES_End ); 

			if( m_pfUpdateEventInEnd )
			{ m_pfUpdateEventInEnd( this ); }

			UpdateEventInEnd( kTime );
		}
		else 
		{ 
			SetEventState( EES_Running ); 
			if( IsFirstInRunning() )
			{
				if( m_pfDoFirstInRunning )
				{ m_pfDoFirstInRunning( this ); }

				DoFirstInRunning( kTime );
				IsFirstInRunning( false );
			}

			if( m_pfUpdateEventInRunning )
			{ m_pfUpdateEventInRunning( this ); }

			UpdateEventInRunning( kTime );
		}
		return true;
	}

	/// Render.
	virtual void RenderEvent(){}

// Override.
protected:
	virtual void UpdateEventInReady( const int64& kTime ){}
	virtual void UpdateEventInRunning( const int64& kTime ){}
	virtual void UpdateEventInEnd( const int64& kTime ){}

	/// execute when first running
	virtual void DoFirstInRunning( const int64& kTime ){}

// Methods.
public:
	void SetEventState( MeTimelineEventBase::EEventStates eState ){ m_eEventState = eState; }

	EEventStates GetEventState() const { return m_eEventState; }

	void SetStartTime( const int64& kTime ) { m_kStartTime = kTime; }
	const int64& GetStartTime() const { return m_kStartTime; }

	const int64& GetEndTime() const { return m_kEndTime; }
	void SetEndTime( const int64& val ){ m_kEndTime = val; }
	
	void SetResetEventWithDataCallbackFunc( DataCallback pfCallback ){ m_pfResetEventWithDataCallback = pfCallback; }
	void SetDoFirstInRunningCallbackFunc( DataCallback pfCallback ){ m_pfDoFirstInRunning = pfCallback; }
	void SetUpdateEventInReadyCallbackFunc( DataCallback pfCallback ){ m_pfUpdateEventInReady = pfCallback; }
	void SetUpdateEventInRunningCallbackFunc( DataCallback pfCallback ){ m_pfUpdateEventInRunning = pfCallback; }
	void SetUpdateEventInEndCallbackFunc( DataCallback pfCallback ){ m_pfUpdateEventInEnd = pfCallback; }

// Methods.
protected:
	void IsFirstInRunning( bool bFirst ) { m_bFirstInRunning = bFirst; }
	bool IsFirstInRunning() const { return m_bFirstInRunning; }

// Members.
protected:
	DataCallback m_pfResetEventWithDataCallback;

	DataCallback m_pfDoFirstInRunning;
	DataCallback m_pfUpdateEventInReady;
	DataCallback m_pfUpdateEventInRunning;
	DataCallback m_pfUpdateEventInEnd;

	EEventStates m_eEventState;	

	int64 m_kStartTime;

	int64 m_kEndTime;

	bool m_bFirstInRunning;

};
