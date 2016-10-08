#pragma once
#include <map>
#include <MeUtility/Singleton.h>


namespace WeatherData
{
	enum{ MAXPATH = 2048 };
}
/**
* @brief CloudySky of weather data.
*/
struct WeatherEventCloudySkyData
{
	unsigned int m_uiDestAmbient;	// Destination ambient light color.
	unsigned int m_uiDestDiffuse;	// Destination diffuse light color.
};
/**
* @brief Rainy of weather data.
*/
struct WeatherEventRainyData
{	
	char m_acFilename[WeatherData::MAXPATH];
	float m_fRainSpeed;
	float m_fRaindropHeight;
	int m_nRainyRange;
};
/**
* @brief Sound of weather data.
*/
struct WeatherEventSoundData
{
	char m_acFilename[WeatherData::MAXPATH];
	bool m_bLoop;
};
/** 
* @brief Definition of a single event.
*/
struct WeatherEventData
{
	int m_nType;					// Event type.
	int m_nMapId;					// Map id.
	float m_fTimeRatio;				// GameTime * TimeRatio = EventTime.
	unsigned int m_uiStartTime;		// Event start time.
	unsigned int m_uiEndTime;		// Event end time.
	unsigned int m_uiToFull;		// From start to full mode.
	unsigned int m_uiToEnd;			// From full mode to end time.
	union
	{
		WeatherEventCloudySkyData m_kCloudyData;
		WeatherEventRainyData m_kRainyData;
		WeatherEventSoundData m_kSoundData;
	};
};

/// Container definition.
typedef std::vector< WeatherEventData > WeatherEventDataContainer;
typedef WeatherEventDataContainer::iterator WeatherEventDataContainerIterator;
typedef WeatherEventDataContainer::const_iterator WeatherEventDataContainerConstIterator;

/**
* @brief Definition of a single WeatherTimeline.
*/
struct WeatherTimelineData
{
	int m_nMapId;
	float m_fTimeRatio;
	WeatherEventDataContainer m_WeatherEvents;
};

/**
* @brief WeatherEvent Config.
*/
class WeatherEventConfig : public Singleton< WeatherEventConfig >
{
// Defines.
public:
	// Event types.
	enum EWeatherEventType
	{
		EWET_CloudySky,		
		EWET_Rainy,
		EWET_Sound,
		EWET_Count
	};

	typedef std::map< int, WeatherTimelineData > WeatherTimelineDataContainer;
	typedef WeatherTimelineDataContainer::iterator WeatherTimelineDataContainerIterator;
	typedef WeatherTimelineDataContainer::const_iterator WeatherTimelineDataContainerConstIterator;

// Methods.
public:	
	/// Destructor.
	~WeatherEventConfig(){}

	/// Load Config.
	bool LoadWeatherEventConfig( const std::string& strFilename );

	/// Get event vector by map id.
	const WeatherTimelineDataContainer& GetWeatherEvents() const { return m_Events; }
	WeatherTimelineDataContainer& GetWeatherEvents() { return m_Events; }

// Methods.
private:
	/// Disable Assignment operator.
	const WeatherEventConfig& operator=( const WeatherEventConfig& kClass ){ return *this; }

	/// Disable Copy constructor.
	WeatherEventConfig( const WeatherEventConfig& kClass ){}

	/// Disable Copy constructor.
	WeatherEventConfig(){}

// Methods.
protected:
	/// Load shade type event config.
	bool LoadShadeEventConfig( MeXmlElement* pElement, WeatherEventData* pEvent );

	/// Load cloudy sky config.
	bool LoadCloudySkyConfig( MeXmlElement* pElement, WeatherEventData* pEvent );

	/// Load rainy config.
	bool LoadRainyConfig( MeXmlElement* pElement, WeatherEventData* pEvent );

	/// Load sound config.
	bool LoadSoundConfig( MeXmlElement* pElement, WeatherEventData* pEvent );

// Members.
private:
	friend class Singleton< WeatherEventConfig >;

	/// Events.
	WeatherTimelineDataContainer m_Events;

// Members.
protected:
};

#define theWeatherEventConfig ( *WeatherEventConfig::Instance() )
