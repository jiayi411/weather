#include "MeAudio/MeAudio.h"
#include "MeAudio/MeTimelineSoundEvent.h"
//////////////////////////////////////////////////////////////////////////
void MeTimelineSoundEvent::DoFirstInRunning( const Ogre::int64& kTime )
{
	m_nSoundIndex = GSound.PlaySound( m_strSoundFilename.c_str(), m_bLoop, NULL ); 
	GSound.SetVolume( m_nSoundIndex, 0.f );
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineSoundEvent::UpdateEventInEnd( const Ogre::int64& kTime )
{	
	if( m_nSoundIndex != -1 )
	{
		GSound.StopSound( m_nSoundIndex );
		m_nSoundIndex = -1;
	}
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineSoundEvent::UpdateBeginToFull( const Ogre::int64& kTime )
{
	if( m_nSoundIndex == -1 )
	{ m_nSoundIndex = GSound.PlaySound( m_strSoundFilename.c_str(), m_bLoop, NULL ); }

	float fValue = Get1_Divide_ToFull_Minus_Start() * ( kTime - GetStartTime() ) * GSound.GetVolume();
	GSound.SetVolume( m_nSoundIndex, fValue );
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineSoundEvent::UpdateInFull( const Ogre::int64& kTime )
{
	if( m_nSoundIndex == -1 )
	{ m_nSoundIndex = GSound.PlaySound( m_strSoundFilename.c_str(), m_bLoop, NULL ); }

	GSound.SetVolume( m_nSoundIndex, GSound.GetVolume() );
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineSoundEvent::UpdateFullToEnd( const Ogre::int64& kTime )
{
	if( m_nSoundIndex == -1 )
	{ m_nSoundIndex = GSound.PlaySound( m_strSoundFilename.c_str(), m_bLoop, NULL ); }

	float fValue = GSound.GetVolume() - 
		( Get1_Divide_End_Minus_ToEnd() * ( kTime - GetToEnd() ) * GSound.GetVolume() );
	GSound.SetVolume( m_nSoundIndex, fValue );
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineSoundEvent::ResetEvent()
{
	MeTimelineShadeEvent::ResetEvent();
	if( m_nSoundIndex != -1 )
	{
		GSound.StopSound( m_nSoundIndex );
		m_nSoundIndex = -1;
	}
}
//////////////////////////////////////////////////////////////////////////