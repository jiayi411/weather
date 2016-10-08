#pragma once
#include <MeFoundation/MeCycleTimer.h>
#include <MeFoundation/MeTimeLine.h>

class WeatherEventManager : public Singleton< WeatherEventManager >
{
// Defines.
public:
	typedef MeTimeline< MeCycleTimer< 3600 * 5 * 1000, MeClockTimer > > WeatherEventTimeline;
	typedef std::map< int , WeatherEventTimeline* > WeatherEventTimelines;
	typedef WeatherEventTimelines::iterator WeatherEventTimelinesIterator;
	typedef WeatherEventTimelines::const_iterator WeatherEventTimelinesConstIterator;

// Construction.
public:
	~WeatherEventManager(){}

// Construction.
private:
	friend class Singleton< WeatherEventManager >;
	WeatherEventManager(){
		m_nMapId = -1;
	}

// Methods.
public:
	void DestoryWeather();

	void InitializeWeather( const std::string& strFilename);

    void UpdateWeather();

	void RenderWeather();

	void StopWeather();

	void UpdateWorldData( int nMapId );

	void StartWeatherTime( int nMapId, const MeTimeValue& kStartTime );

// Static Methods.
public:
	static void ResetEventWithWorldData( MeTimelineEventBase* pEvent );

	static void SendCurrentRolePositionInfo( MeTimelineEventBase* pEvent );

// Methods.
protected:
	/// CloudySky Read.
	void LoadCloudySkyConfig();

// Members.
protected:
	WeatherEventTimelines m_mapTimelines;

	int m_nMapId;
};
//////////////////////////////////////////////////////////////////////////
// sample for config
/*/========================================================================================================================================================\*\
<WeatherTimeline>
	
	<Event Type = "0" Mapid = "2" TimeRatio = "5" StartTime = "0" EndTime = "7000" ToFull = "2000" ToEnd = "5000" DestAmbient = "255,55,55,55" DestDiffuse = "255,55,55,55" />
</WeatherTimeline>
\*\========================================================================================================================================================/*/
