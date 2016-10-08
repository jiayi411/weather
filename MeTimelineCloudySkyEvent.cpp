#include <MeFoundation/MeFoundationPCH.h>
#include "Me3d/Me3d.h"
#include "Me3d/Engine/EngineInstance.h"
#include "Me3d/Engine/Renderer.h"
#include "Me3d/MeTimelineCloudySkyEvent.h"
//////////////////////////////////////////////////////////////////////////
void MeTimelineCloudySkyEvent::DoFirstInRunning( const int64& kTime )
{
	if( m_pfResetEventWithDataCallback )
	{ m_pfResetEventWithDataCallback( this ); }
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineCloudySkyEvent::UpdateBeginToFull( const int64& kTime )
{
	// caculate current light color
    if( m_kDestinationAmbient != ColourValue::ZERO )
	{
		m_kCurrentAmbient = m_kOriginalAmbient + 
			( Get1_Divide_ToFull_Minus_Start() * ( kTime - GetStartTime() ) * ( m_kDestinationAmbient - m_kOriginalAmbient ) );
		m_kCurrentAmbient.a = 1.f;
	}
	else
	{
		m_kCurrentAmbient = m_kOriginalAmbient;
	}

	if( m_kDestinationDiffuse != ColourValue::ZERO )
	{
		m_kCurrentDiffuse = m_kOriginalDiffuse + 
			( Get1_Divide_ToFull_Minus_Start() * ( kTime - GetStartTime() ) * ( m_kDestinationDiffuse - m_kOriginalDiffuse ) );
		m_kCurrentDiffuse.a = 1.f;
	}
	else
	{
		m_kCurrentDiffuse = m_kOriginalDiffuse;
	}

	// set to engine
    Renderer* pRenderer = Renderer::Instance();
	if( pRenderer )
	{
		pRenderer->SetLightAmbient( m_kCurrentAmbient );
        pRenderer->SetLightDiffuse( m_kCurrentDiffuse );
	}	
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineCloudySkyEvent::UpdateInFull( const int64& kTime )
{
	if( m_kDestinationAmbient != ColourValue::ZERO )
	{
		m_kCurrentAmbient = m_kDestinationAmbient;
		m_kCurrentAmbient.a = 1.f;
	}
	else
	{
		m_kCurrentAmbient = m_kOriginalAmbient;
	}

	if( m_kDestinationDiffuse != ColourValue::ZERO )
	{
		m_kCurrentDiffuse = m_kDestinationDiffuse;
		m_kCurrentDiffuse.a = 1.f;
	}
	else
	{
		m_kCurrentDiffuse = m_kOriginalDiffuse;
	}
	// Set every frame
    Renderer* pRenderer = Renderer::Instance();
    if( pRenderer )
    {
        pRenderer->SetLightAmbient(  m_kCurrentAmbient );
        pRenderer->SetLightDiffuse( m_kCurrentDiffuse );
    }	
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineCloudySkyEvent::UpdateFullToEnd( const int64& kTime )
{
	if( m_kDestinationAmbient != ColourValue::ZERO )
	{
		m_kCurrentAmbient = m_kDestinationAmbient + 
			( Get1_Divide_End_Minus_ToEnd() * ( kTime - GetToEnd() ) * ( m_kOriginalAmbient - m_kDestinationAmbient ) );
		m_kCurrentAmbient.a = 1.f;
	}
	else
	{
		m_kCurrentAmbient = m_kOriginalAmbient;
	}

	if( m_kDestinationDiffuse != ColourValue::ZERO )
	{
		m_kCurrentDiffuse = m_kDestinationDiffuse + 
			( Get1_Divide_End_Minus_ToEnd() * ( kTime - GetToEnd() ) * ( m_kOriginalDiffuse - m_kDestinationDiffuse ) );

		m_kCurrentDiffuse.a = 1.f;
	}
	else
	{
		m_kCurrentDiffuse = m_kOriginalDiffuse;
	}

    Renderer* pRenderer = Renderer::Instance();
    if( pRenderer )
    {
        pRenderer->SetLightAmbient(  m_kCurrentAmbient );
        pRenderer->SetLightDiffuse(  m_kCurrentDiffuse );
    }	
}
//////////////////////////////////////////////////////////////////////////
