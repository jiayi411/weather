#pragma once
#include <MeFoundation/MeFoundationPCH.h>
#include <MeFoundation/MeTimelineShadeEvent.h>

/// Play a sound
class MEFOUNDATION_ENTRY MeTimelineSoundEvent : public MeTimelineShadeEvent
{
// Construction.
public:
	/// Constructor.
	MeTimelineSoundEvent(){
		m_nSoundIndex = -1;
		m_bLoop = false;
	}
	
	/// Destructor.
	virtual ~MeTimelineSoundEvent(){}

	/// Copy constructor.
	MeTimelineSoundEvent( const MeTimelineSoundEvent& kClass ){}

// Overload operator.
public:
	/// Assignment operator.
	const MeTimelineSoundEvent& operator=( const MeTimelineSoundEvent& kClass ){ return *this; }

	/// Reset event.
	virtual void ResetEvent();

// Override.
public:
	/// Destroy.
	virtual void DestroyEvent(){}

// Override.
protected:
	virtual void UpdateBeginToFull( const MeTimeValue& kTime );

	virtual void UpdateInFull( const MeTimeValue& kTime );

	virtual void UpdateFullToEnd( const MeTimeValue& kTime );

	/// Update event.
	virtual void DoFirstInRunning( const MeTimeValue& kTime );
	virtual void UpdateEventInEnd( const MeTimeValue& kTime );

// Methods.
public:
	/// Set sound filename.
	void SetSoundFilename( const std::string& strFilename ){ m_strSoundFilename = strFilename; }

	/// Set loop config.
	void SetLoop( bool bLoop ){ m_bLoop = bLoop; }

// Members.
protected:
	/// Sound full path filename.
	std::string m_strSoundFilename;

	/// Is loop.
	bool m_bLoop;

	/// Sound index.
	int m_nSoundIndex;
};
