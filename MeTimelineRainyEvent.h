#pragma once
#include <MeFoundation/MeTimelineShadeEvent.h>

class CHeightMapData;

/**
* @brief Rain Effect。
*/
class MEFOUNDATION_ENTRY MeTimelineRainyEvent : public MeTimelineShadeEvent
{
// Defines.
protected:
	/// Rain's Vertex Definition.
	struct MeRainVertex
	{
		Vector3 m_kPosition;		// Pos
		Vector2 m_kUV;			// UV
	};

	/// RainSpray's Vertex Definition.
	struct MeRainSprayVertex
	{
		Vector3 m_kPosition;		
		DWORD m_kColor;
		Vector2 m_kUV;
	};

	/// RainSpray's visible structure.
	struct RainSprayVisible
	{
		bool m_bVisible;
		MeEngine::int64 m_kStartTime;
	};

// Construction.
public:	
	/// Constructor.
	MeTimelineRainyEvent()
		: mdwTime( 0 )
	{}

	/// Copy constructor.
	MeTimelineRainyEvent( const MeTimelineRainyEvent& kClass )
		:mdwTime( 0 )
	{}

// Overload operator.
public:
	/// Assignment operator.
	const MeTimelineRainyEvent& operator=( const MeTimelineRainyEvent& kClass ){ return *this; }

// Override.
public:
	/// Destructor.
	virtual ~MeTimelineRainyEvent(){}

	/// Initialize.
	virtual void InitializeEvent();

	/// Destroy.
	virtual void DestroyEvent();

	/// Reset.
	virtual void ResetEvent();

	/// Render.
	virtual void RenderEvent();

	/// Update from nothing to appearence
	virtual void UpdateBeginToFull( const MeEngine::int64& kTime );

	/// Keeping update after appearence
	virtual void UpdateInFull( const MeEngine::int64& kTime );

	/// Update to disappear
	virtual void UpdateFullToEnd( const MeEngine::int64& kTime );

// Methods.
public:
	/// set resource file
	static void SetRainTextureFilename( const std::string& strFilename ){ ms_strRainTextureFilename = strFilename; }

	/// set rainy range
	static void SetRainyRange( int nRange ){ ms_nRainyRange = nRange; }

	/// set drop speed
	static void SetRainySpeed( float fSpeed ){ ms_fRainSpeed = fSpeed; }

	/// set drop height
	static void SetRaindropHeight( float fHeight ){ ms_fRaindropHeight = fHeight; }

	/// set center position
	static void SetCurrentPosition( const Vector3& rkPos ){ ms_kPosition = rkPos; }

// Methods.
protected:
	/// rain vertex
	void GenerateRainVertex( int nStartPosition, float fRadius );

	/// rain's Z
	void UpdateRainVertexZ( unsigned int uiCount, const MeEngine::int64& kTimeValue );

	/// set spray UV and vertex color
	void UpdateRainSprayUVnColor( unsigned int uiCount, const MeEngine::int64& kTimeValue );

	/// set position of spray
	void UpdateRainSprayPosition( int nVertexIndex, int nSprayIndex, const MeEngine::int64& kTimeValue, CHeightMapData* pHeightData );

	/// render spray
	void RenderRainSpray();

	/// render
	void RenderRain();

	/// random rainy drop
	float GetRandomZOffsetValue();


// Static Members.
public:
	/// rain drop count
	static const int ms_ncRainDropCount = 1000;

	/// rain vertex count
	static const int ms_ncRainVecticesCount = ms_ncRainDropCount * 3;

	/// spray vertex count
	static const int ms_ncRainSprayVecticesCount = ms_ncRainDropCount * 3;

// Members.
protected:
	/// array of rain drops
	static MeRainVertex ms_kRainVertices[ms_ncRainVecticesCount];

	/// offset of spray vertices
	static Vector3 ms_kRainSprayVerticesOffset[ms_ncRainSprayVecticesCount];

	/// spray vertices
	static MeRainSprayVertex ms_kRainSprayVertices[ms_ncRainSprayVecticesCount];

	/// are spray visible
	static RainSprayVisible ms_bRainSprayVisible[ms_ncRainDropCount];

	/// current render count of rainy drops
	static int ms_nRenderCount;

	/// tex of rain drop
    static MeEngine::TexturePtr ms_nRainTextureId;

	/// tex of spray
	static MeEngine::TexturePtr ms_nRainSprayTextureId;

	/// current pos
	static Vector3 ms_kPosition;

	/// res of tex of rain
	static std::string ms_strRainTextureFilename;

	/// res of tex of spray
	static std::string ms_strRainSprayTextureFilename;

	/// rang of rain
	static int ms_nRainyRange;

	/// speed of drop
	static float ms_fRainSpeed;

	/// heigh of a drop
	static float ms_fRaindropHeight;
};
