#include <MeFoundation/MeFoundationPCH.h>
#include <Me3d/Me3d.h>
#include <Me3d/Engine/Engine.h>
#include <Me3d/Engine/RendererDX.h>
#include <Me3d/MeTimelineCloudySkyEvent.h>
#include <MeTerrain//MeTimelineRainyEvent.h>
#include <MeAudio/MeTimelineSoundEvent.h>
#include <RapidXml/MeRapidXml.h>
#include "WeatherEventManager.h"
#include "WeatherEventConfig.h"
#include "MeTerrain/WorldPrecompiled.hpp"
#include "GameMain.h"
#include "Player.h"
#include "PlayerMgr.h"
#include "Common.h"
#include "MeAudio/SoundFileManager.h"
//////////////////////////////////////////////////////////////////////////
void WeatherEventManager::DestoryWeather()
{
	WeatherEventTimelinesIterator kMapIter = m_mapTimelines.begin();
	for( ; kMapIter != m_mapTimelines.end() ; ++ kMapIter )
	{
		kMapIter->second->ClearAllEvents();
		MeDelete kMapIter->second;
	}
	m_mapTimelines.clear();
}
//////////////////////////////////////////////////////////////////////////
void WeatherEventManager::InitializeWeather( const std::string& strFilename)
{
	if( !theWeatherEventConfig.LoadWeatherEventConfig( strFilename ) )
	{ return; }

	WeatherEventConfig::WeatherTimelineDataContainer& kEventsConfig = theWeatherEventConfig.GetWeatherEvents();
	WeatherEventConfig::WeatherTimelineDataContainerIterator kEventsConfigIter = kEventsConfig.begin();
	for( ; kEventsConfigIter != kEventsConfig.end(); ++ kEventsConfigIter )
	{
		int nMapId = kEventsConfigIter->first;
		WeatherTimelineData& kEvents = kEventsConfigIter->second;
		WeatherEventDataContainerIterator kEventsIter = kEvents.m_WeatherEvents.begin();
		for( ; kEventsIter != kEvents.m_WeatherEvents.end(); ++ kEventsIter )
		{
			WeatherEventData kEvent = ( *kEventsIter );

			MeTimelineEventBase* pkEvent = NULL;
			
			switch( kEvent.m_nType )
			{
			case WeatherEventConfig::EWET_CloudySky:
				{
					pkEvent = MeNew MeTimelineCloudySkyEvent();
					MeTimelineCloudySkyEvent* pkCSEvent = ( MeTimelineCloudySkyEvent * )pkEvent;
					pkCSEvent->SetStartTime( kEvent.m_uiStartTime );
					pkCSEvent->SetEndTime( kEvent.m_uiEndTime );
					pkCSEvent->SetToFull( kEvent.m_uiToFull );
					pkCSEvent->SetToEnd( kEvent.m_uiToEnd );				
					pkCSEvent->SetDestinationAmbient( kEvent.m_kCloudyData.m_uiDestAmbient );								
					pkCSEvent->SetDestinationDiffuse( kEvent.m_kCloudyData.m_uiDestDiffuse );
					pkCSEvent->SetDoFirstInRunningCallbackFunc( ResetEventWithWorldData );
				}
				break;
			case WeatherEventConfig::EWET_Rainy:
				{
					pkEvent = MeNew MeTimelineRainyEvent;
					MeTimelineRainyEvent* pkREvent = ( MeTimelineRainyEvent * )pkEvent;
					pkREvent->SetStartTime( kEvent.m_uiStartTime );
					pkREvent->SetEndTime( kEvent.m_uiEndTime );
					pkREvent->SetToFull( kEvent.m_uiToFull );
					pkREvent->SetToEnd( kEvent.m_uiToEnd );
					//pkREvent->SetRainTextureFilename( kEvent.m_kRainyData.m_acFilename );
					pkREvent->SetRainyRange( kEvent.m_kRainyData.m_nRainyRange );
					pkREvent->SetRainySpeed( kEvent.m_kRainyData.m_fRainSpeed );
					pkREvent->SetRaindropHeight( kEvent.m_kRainyData.m_fRaindropHeight );
					pkREvent->SetUpdateEventInRunningCallbackFunc( SendCurrentRolePositionInfo );
				}
				break;
			case WeatherEventConfig::EWET_Sound:
				{
					pkEvent = MeNew MeTimelineSoundEvent;
					MeTimelineSoundEvent* pkSEvent = ( MeTimelineSoundEvent * )pkEvent;
					pkSEvent->SetStartTime( kEvent.m_uiStartTime );
					pkSEvent->SetEndTime( kEvent.m_uiEndTime );
					pkSEvent->SetToFull( kEvent.m_uiToFull );
					pkSEvent->SetToEnd( kEvent.m_uiToEnd );
					pkSEvent->SetLoop( kEvent.m_kSoundData.m_bLoop );

					char acFullFilename[WeatherData::MAXPATH] = {0};
					MeSprintf_s( acFullFilename, WeatherData::MAXPATH, "%s/%s", GetRootPath(), kEvent.m_kSoundData.m_acFilename );
					// Cache sound file.
					GetSoundMgr()->LoadSoundFileToMemory( acFullFilename );
					pkSEvent->SetSoundFilename( acFullFilename );
				}
				break;
			default:
				break;
			}

			if( m_mapTimelines.find( kEvents.m_nMapId ) == m_mapTimelines.end() )
			{
				m_mapTimelines.insert( std::make_pair( kEvents.m_nMapId, MeNew WeatherEventTimeline() ));
			}

			if( m_mapTimelines[nMapId]->GetTimer() )
				m_mapTimelines[nMapId]->GetTimer()->SetTimeRatio( kEvents.m_fTimeRatio );

			pkEvent->SetResetEventWithDataCallbackFunc( ResetEventWithWorldData );

			pkEvent->InitializeEvent();

			m_mapTimelines[nMapId]->PushEvent( pkEvent );
		}
	}

	WeatherEventTimelinesIterator kMapIter = m_mapTimelines.begin();
	for( ; kMapIter != m_mapTimelines.end() ; ++ kMapIter )
	{
		kMapIter->second->SortEventsByTime();
	}
}
//////////////////////////////////////////////////////////////////////////
void WeatherEventManager::RenderWeather()
{
	WeatherEventTimelinesIterator kMapIter = m_mapTimelines.find( m_nMapId );
	if( kMapIter == m_mapTimelines.end() )
		return;

	kMapIter->second->RenderTimeline();
}
//////////////////////////////////////////////////////////////////////////
void WeatherEventManager::UpdateWeather()
{
	WeatherEventTimelinesIterator kMapIter = m_mapTimelines.find( m_nMapId );
	if( kMapIter == m_mapTimelines.end() )
		return;

	kMapIter->second->UpdateTimeline();
	
}
//////////////////////////////////////////////////////////////////////////
void WeatherEventManager::StopWeather()
{
	WeatherEventTimelinesIterator kMapIter = m_mapTimelines.begin();
	for( ; kMapIter != m_mapTimelines.end(); ++ kMapIter )
	{
		if( kMapIter->second->GetTimer() )
		{ kMapIter->second->GetTimer()->End(); }

		WeatherEventTimeline::TimelineEvents& akEvents = kMapIter->second->GetEvents();
		WeatherEventTimeline::TimelineEventsIterator kIter = akEvents.begin();
		for( ; kIter != akEvents.end(); ++ kIter )
		{
			MeTimelineEventBase* pEvent = ( *kIter );
			pEvent->ResetEvent();
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void WeatherEventManager::UpdateWorldData( int nMapId )
{
	m_nMapId = -1;

	WeatherEventTimelinesIterator kMapIter = m_mapTimelines.find( nMapId );
	if( kMapIter == m_mapTimelines.end() )
	{ return; }

	RendererDx* pRenderer = ( RendererDx* )GetEngine()->GetRenderer();
	if( !pRenderer )
	{ return; }

	m_nMapId = nMapId;

	if( kMapIter->second->GetTimer() )
	{ kMapIter->second->GetTimer()->End(); }

	WeatherEventTimeline::TimelineEvents& akEvents = kMapIter->second->GetEvents();
	WeatherEventTimeline::TimelineEventsIterator kIter = akEvents.begin();
	for( ; kIter != akEvents.end(); ++ kIter )
	{
		MeTimelineEventBase* pEvent = ( *kIter );
		pEvent->ResetEvent();
	}

	
}
//////////////////////////////////////////////////////////////////////////
void WeatherEventManager::StartWeatherTime( int nMapId, const MeTimeValue& kStartTime )
{
	WeatherEventTimelinesIterator kMapIter = m_mapTimelines.find( nMapId );
	if( kMapIter == m_mapTimelines.end() )
		return;
	
	kMapIter->second->StartTime( kStartTime );
	
}
//////////////////////////////////////////////////////////////////////////
void WeatherEventManager::ResetEventWithWorldData( MeTimelineEventBase* pEvent )
{
	RendererDx* pRenderer = ( RendererDx* )GetEngine()->GetRenderer();
	if( !pRenderer )
		return;

	for( int i = 0 ; i < WeatherEventConfig::EWET_Count; ++ i )
	{
		bool bFound = false;
		switch( i )
		{
		case WeatherEventConfig::EWET_CloudySky:
			{
				MeTimelineCloudySkyEvent* pCloudySkyEvent =
					dynamic_cast< MeTimelineCloudySkyEvent* >( pEvent );
				if( pCloudySkyEvent )
				{
					D3DLIGHT9 kTerrainLight;
					pRenderer->GetLight( LIGHT_INDEX_TERRAIN, &kTerrainLight );
					pCloudySkyEvent->SetOriginalAmbient( kTerrainLight.Ambient );
					pCloudySkyEvent->SetOriginalDiffuse( kTerrainLight.Diffuse );
				}						
			}
			break;
		case WeatherEventConfig::EWET_Rainy:			
			{
				CWorldTile* pTile = CURRENTTILE;
				if( pTile )
				{
					pTile->LoadHeightMapData();
				}
			}
			break;
		}
		if( bFound )
		{ break; }
	}
}
//////////////////////////////////////////////////////////////////////////
void WeatherEventManager::SendCurrentRolePositionInfo( MeTimelineEventBase* pEvent )
{
	MeTimelineRainyEvent* pREvent = dynamic_cast<MeTimelineRainyEvent*>( pEvent );
	if( !theHeroGame.GetPlayerMgr()->GetMe() )
	{ return; }

	
	//theHeroGame.GetPlayerMgr()->GetMe()->GetPos( &kPos.x, &kPos.y, &kPos.z );
	Vector kPos = *( Vector* )&GetMe3dConfig()->GetCamera()->GetEyePt();
	pREvent->SetCurrentPosition( kPos );
}
