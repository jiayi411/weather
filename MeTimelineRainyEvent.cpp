#include <MeFoundation/MeFoundationPCH.h>

//////////////////////////////////////////////////////////////////////////
#define MAX_VECTOR_Z 30			// max Z of drop
#define RAIN_OFFSETVALUE 0.06f	// horizonal offset of rain drop
#define RAINSPRAY_WIDTH 0.1f	// spray's width & height
#define RAINSPRAY_SLOPE 	
//////////////////////////////////////////////////////////////////////////


class MEFOUNDATION_ENTRY MeUtilityStaff
{
public:
	static float GetRandValueF( float dRangeMin, float dRangeMax ){
		unsigned int uiRandValue = 0;
		uiRandValue = rand();		
		return ( float )( ( float )uiRandValue / ( float )RAND_MAX * dRangeMax + dRangeMin ); 
	}

	static int GetRandValueI( int dRangeMin, int dRangeMax ){
		unsigned int uiRandValue = 0;
		uiRandValue = rand();		
		return ( uiRandValue / RAND_MAX * dRangeMax + dRangeMin ); 
	}
};



MeTimelineRainyEvent::MeRainVertex MeTimelineRainyEvent::ms_kRainVertices[ms_ncRainVecticesCount];
MeTimelineRainyEvent::MeRainSprayVertex MeTimelineRainyEvent::ms_kRainSprayVertices[ms_ncRainSprayVecticesCount];
Vector3 MeTimelineRainyEvent::ms_kRainSprayVerticesOffset[ms_ncRainSprayVecticesCount];
MeTimelineRainyEvent::RainSprayVisible MeTimelineRainyEvent::ms_bRainSprayVisible[ms_ncRainDropCount];
int MeTimelineRainyEvent::ms_nRenderCount = -1;
std::string MeTimelineRainyEvent::ms_strRainTextureFilename = "./data/texture/Raintex.dds";
std::string MeTimelineRainyEvent::ms_strRainSprayTextureFilename = "./data/texture/RainSprayTex.dds";
int MeTimelineRainyEvent::ms_nRainyRange = 50;
float MeTimelineRainyEvent::ms_fRainSpeed = 0.4f;
float MeTimelineRainyEvent::ms_fRaindropHeight = 1.5f;
Vector3 MeTimelineRainyEvent::ms_kPosition = Vector3( 0, 0, 0 );
//////////////////////////////////////////////////////////////////////////
static int s_nOutterCount = MeTimelineRainyEvent::ms_ncRainDropCount * 2;
static int s_nInnerCount = MeTimelineRainyEvent::ms_ncRainDropCount;
static int s_nRainSprayShowtime = 100;
static DWORD s_VisibleColor = ARGB( 255, 255, 255, 255 );
static DWORD s_InvisibleColor = ARGB( 0, 255, 255, 255 );
//////////////////////////////////////////////////////////////////////////
void MeTimelineRainyEvent::InitializeEvent()
{
	MeTimelineShadeEvent::InitializeEvent();

	if( !ms_nRainTextureId.isNull() )
	{ return; }

	/*	Generating two parts，inner side has more drops.
	*/
	// overall drops
	int nIndex = 0;
	for( ; nIndex < s_nOutterCount; nIndex += 3 )
	{		
		// generate rain drops randomly。
		GenerateRainVertex( nIndex, ms_nRainyRange );
	}
	// inner rain drops
	for( ; nIndex < s_nOutterCount + s_nInnerCount; nIndex += 3 )
	{
		GenerateRainVertex( nIndex, ms_nRainyRange / 2 );
	}

	ms_nRenderCount = 0;
	
	// pre-loaded textures
	// 1. drop
	char acFilename[ME_MAX_PATH] = {0};
	sprintf( acFilename, ms_strRainTextureFilename.c_str(), GetRootPath());
	ms_nRainTextureId = MeEngine::Root::getSingletonPtr()->getTextureManager()->load( acFilename,"MeTexture" );

	// 2. spray
	sprintf( acFilename, ms_strRainSprayTextureFilename.c_str(), GetRootPath() );
	ms_nRainSprayTextureId = MeEngine::Root::getSingletonPtr()->getTextureManager()->load( acFilename,"MeTexture" );

	memset( ms_bRainSprayVisible, 0, sizeof( ms_bRainSprayVisible ) );
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineRainyEvent::DestroyEvent()
{}
//////////////////////////////////////////////////////////////////////////
void MeTimelineRainyEvent::ResetEvent()
{
	MeTimelineShadeEvent::ResetEvent();
	ms_nRenderCount = 0;
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineRainyEvent::RenderEvent()
{
	if( ms_nRenderCount <= 0 )
	{ return; }

	RenderRainSpray();
	RenderRain();
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineRainyEvent::GenerateRainVertex( int nStartPosition, float fRadius )
{
	if( nStartPosition + 2 >= ms_ncRainVecticesCount || nStartPosition < 0 )
		return;

	// first vertex //
	float fRangeX = MeUtilityStaff::GetRandValueF( -fRadius, 2 * fRadius );
	float fRangeY = MeUtilityStaff::GetRandValueF( -fRadius, 2 * fRadius );
	float fRangeZ = MeUtilityStaff::GetRandValueF( -MAX_VECTOR_Z, 2 * MAX_VECTOR_Z );
	if( FLOAT_EQUAL( fRangeZ, 0.f ) )
	{ fRangeZ = 0.2f; }
	MeRainVertex kRainVertex0;
	kRainVertex0.m_kPosition = Vector3( fRangeX, fRangeY, fRangeZ );		
	kRainVertex0.m_kUV = Vector2( 0, 1 );		
	ms_kRainVertices[nStartPosition] = kRainVertex0;

	// the second vertex, located on the vertical direction from the original point
	MeRainVertex kRainVertex1;
	Vector3 kRainDir = kRainVertex0.m_kPosition.crossProduct( Vector3::UNIT_Z );
	kRainDir.normalise();
	kRainVertex1.m_kPosition = kRainVertex0.m_kPosition + RAIN_OFFSETVALUE * kRainDir;
	kRainVertex1.m_kUV = Vector2( 1, 1 );
	ms_kRainVertices[nStartPosition + 1] = kRainVertex1;

	// the third vertex, located towards up Z, then set an offset among 0.3 -1 randomly
	float fZOffset = GetRandomZOffsetValue();
	if( FLOAT_EQUAL( fZOffset, 0.f ) )
	{ fZOffset = 0.4f; }
	MeRainVertex kRainVertex2;
	kRainVertex2.m_kPosition = kRainVertex0.m_kPosition + RAIN_OFFSETVALUE * kRainDir / 2;
	kRainVertex2.m_kPosition.z  = fRangeZ + fZOffset;
	kRainVertex2.m_kUV = Vector2( 0.5f, 0 );
	ms_kRainVertices[nStartPosition + 2] = kRainVertex2;

	// first vertex of spray
	kRainVertex0.m_kPosition.z = 0.f;
	ms_kRainSprayVerticesOffset[nStartPosition] = kRainVertex0.m_kPosition;
	ms_kRainSprayVertices[nStartPosition].m_kColor = ARGB( 255, 255, 255, 255 );
	ms_kRainSprayVertices[nStartPosition].m_kUV = Vector2( 0.125f, 0.f );

	// the slop of spray
	static float sf3root2Lenth = 0.866f * RAINSPRAY_WIDTH;
	float fLength = kRainVertex0.m_kPosition.squaredLength();
	Vector3 kSprayDir = kRainVertex0.m_kPosition;
	kSprayDir.normalise();
	kSprayDir =  kSprayDir * 50;
	kSprayDir.z = fLength;
	kSprayDir.normalise();
	Vector3 kHalfPoint = kRainVertex0.m_kPosition - kSprayDir * sf3root2Lenth;

	// the second vertex of spray
	ms_kRainSprayVerticesOffset[nStartPosition + 1] = kHalfPoint - ( RAINSPRAY_WIDTH + fLength / 10000 )* kRainDir / 2;
	ms_kRainSprayVertices[nStartPosition + 1].m_kColor = ARGB( 255, 255, 255, 255 );
	ms_kRainSprayVertices[nStartPosition + 1].m_kUV = Vector2( 0.f, 0.25f );

	// the third vertex of spray
	ms_kRainSprayVerticesOffset[nStartPosition + 2] = kHalfPoint + ( RAINSPRAY_WIDTH + fLength / 10000 ) * kRainDir / 2;
	ms_kRainSprayVertices[nStartPosition + 2].m_kColor = ARGB( 255, 255, 255, 255 );
	ms_kRainSprayVertices[nStartPosition + 2].m_kUV = Vector2( 0.25f, 0.25f );

}
//////////////////////////////////////////////////////////////////////////
void MeTimelineRainyEvent::UpdateBeginToFull( const MeEngine::int64& kTime )
{
	if( kTime == 0 )// just beginning
	{ ms_nRenderCount = 0; }

	ms_nRenderCount = Get1_Divide_ToFull_Minus_Start() * ( kTime - GetStartTime() ) * ms_ncRainVecticesCount;

	ms_nRenderCount = MIN( ms_ncRainVecticesCount, MAX( 0, ms_nRenderCount ) );	
	ms_nRenderCount = ms_nRenderCount - ms_nRenderCount % 3;
    
	// update vertices
	UpdateRainVertexZ( ms_nRenderCount, kTime );
	UpdateRainSprayUVnColor( ms_ncRainVecticesCount, kTime );
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineRainyEvent::UpdateInFull( const MeEngine::int64& kTime )
{
	ms_nRenderCount = ms_ncRainVecticesCount;
	UpdateRainVertexZ( ms_nRenderCount, kTime );
	UpdateRainSprayUVnColor( ms_ncRainVecticesCount, kTime );
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineRainyEvent::UpdateFullToEnd( const MeEngine::int64& kTime )
{
    // update vertices
	ms_nRenderCount = ms_ncRainVecticesCount - 
		Get1_Divide_End_Minus_ToEnd() * ( kTime - GetToEnd() ) * ms_ncRainVecticesCount;

	ms_nRenderCount = MIN( ms_ncRainVecticesCount, MAX( 0, ms_nRenderCount ) );
	ms_nRenderCount = ms_nRenderCount - ms_nRenderCount % 3;

	UpdateRainVertexZ( ms_nRenderCount, kTime );
	UpdateRainSprayUVnColor( ms_ncRainVecticesCount, kTime );
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineRainyEvent::UpdateRainVertexZ( unsigned int uiCount, const MeEngine::int64& kTimeValue )
{
	static Vector3 skLastPosition = Vector3( -1, -1, -1 );
	static unsigned int suiLastCount = 0;
	bool bNeedUpdateZValue = false;
	CWorldTile* pTile = CURRENTTILE;
	CHeightMapData* pHeightData = pTile->GetHeightMapData();
    
	// check if it need to change position
	if( ( skLastPosition != ms_kPosition ) || ( suiLastCount != uiCount ) )
	{ 
		skLastPosition = ms_kPosition;
		suiLastCount = uiCount;
		bNeedUpdateZValue = true; 
	}

	int nSprayIndex = 0;// to reduce divide operation, we store the index
	for( int i = 0 ; i < uiCount; i += 3 )
	{		
		if( !ms_bRainSprayVisible[nSprayIndex].m_bVisible )
		{
			// update the height of spray only if when it is not appeared
			if( bNeedUpdateZValue && pHeightData )
			{ UpdateRainSprayPosition( i, nSprayIndex, kTimeValue, pHeightData ); }
		}

		ms_kRainVertices[i].m_kPosition.z -= ms_fRainSpeed;
		if(  ms_kRainVertices[i].m_kPosition.z <= ( ms_kRainSprayVertices[i].m_kPosition.z - ms_kPosition.z - ms_fRaindropHeight ) )
		{
			float fZ = MAX_VECTOR_Z;
			ms_kRainVertices[i].m_kPosition.z = fZ;	
			ms_kRainVertices[i + 1].m_kPosition.z = fZ;
			ms_kRainVertices[i + 2].m_kPosition.z = fZ + GetRandomZOffsetValue();	

			++nSprayIndex;

			continue;
		}

		// check if the spary should be display
		if(  ms_kRainVertices[i].m_kPosition.z <= ( ms_kRainSprayVertices[i].m_kPosition.z - ms_kPosition.z  + ms_fRaindropHeight) )
		{
			ms_bRainSprayVisible[nSprayIndex].m_kStartTime = kTimeValue;
			ms_bRainSprayVisible[nSprayIndex].m_bVisible = true;
		}

		ms_kRainVertices[i + 1].m_kPosition.z -= ms_fRainSpeed;
		ms_kRainVertices[i + 2].m_kPosition.z -= ms_fRainSpeed;		
		++nSprayIndex;
	}
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineRainyEvent::UpdateRainSprayPosition( int nVertexIndex, int nSprayIndex,
												   const MeEngine::int64& kTimeValue, CHeightMapData* pHeightData )
{
	static float s_fOffset = -0.6f;
	
	// update before spray's appearence
	ms_kRainSprayVertices[nVertexIndex].m_kPosition = ms_kPosition + ms_kRainSprayVerticesOffset[nVertexIndex];
	ms_kRainSprayVertices[nVertexIndex + 1].m_kPosition = ms_kPosition + ms_kRainSprayVerticesOffset[nVertexIndex + 1];
	ms_kRainSprayVertices[nVertexIndex + 2].m_kPosition = ms_kPosition + ms_kRainSprayVerticesOffset[nVertexIndex + 2];

	float fZ = pHeightData->GetHeightbyXY( ms_kRainSprayVertices[nVertexIndex].m_kPosition.x, 
		ms_kRainSprayVertices[nVertexIndex].m_kPosition.y ) - s_fOffset;

	ms_kRainSprayVertices[nVertexIndex].m_kPosition.z = fZ + RAINSPRAY_WIDTH;
	ms_kRainSprayVertices[nVertexIndex + 1].m_kPosition.z = fZ;
	ms_kRainSprayVertices[nVertexIndex + 2].m_kPosition.z = fZ;
	
}
//////////////////////////////////////////////////////////////////////////
float MeTimelineRainyEvent::GetRandomZOffsetValue()
{
	float fZOffset = ms_fRaindropHeight;

	return fZOffset;
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineRainyEvent::UpdateRainSprayUVnColor( unsigned int uiCount, const MeEngine::int64& kTimeValue )
{
	int nSprayIndex = 0;
	for( int i = 0 ; i < uiCount ; i += 3 )
	{
		// update UV when visible
		if( ms_bRainSprayVisible[nSprayIndex].m_bVisible )
		{
			if( ms_kRainSprayVertices[i].m_kUV.x + 0.25f >= 1 )
			{
				ms_kRainSprayVertices[i].m_kUV.x = 0.125f;
				ms_kRainSprayVertices[i + 1].m_kUV.x = 0.f;
				ms_kRainSprayVertices[i + 2].m_kUV.x = 0.25f;
                
				// using x as y for random purpose
				if( ( i - 2 ) >= 0 )
				{
					ms_kRainSprayVertices[i].m_kUV.y = ms_kRainSprayVertices[i - 2].m_kUV.x;
					ms_kRainSprayVertices[i + 1].m_kUV.y = ms_kRainSprayVertices[i].m_kUV.y + 0.25f;
					ms_kRainSprayVertices[i + 2].m_kUV.y = ms_kRainSprayVertices[i + 1].m_kUV.y;
				}
                
                // place for set invisible
				ms_bRainSprayVisible[nSprayIndex].m_bVisible = false;
				ms_kRainSprayVertices[i].m_kColor = s_InvisibleColor;
				ms_kRainSprayVertices[i + 1].m_kColor = s_InvisibleColor;
				ms_kRainSprayVertices[i + 2].m_kColor = s_InvisibleColor;
				++ nSprayIndex;
				continue;
			}

			if( ( kTimeValue - ms_bRainSprayVisible[nSprayIndex].m_kStartTime ) < s_nRainSprayShowtime )
			{
				++ nSprayIndex;
				continue;
			}
			ms_kRainSprayVertices[i].m_kUV.x += 0.25f;
			ms_kRainSprayVertices[i + 1].m_kUV.x += 0.25f;
			ms_kRainSprayVertices[i + 2].m_kUV.x += 0.25f;

			// next updating time
			ms_bRainSprayVisible[nSprayIndex].m_kStartTime = kTimeValue;

			// change to visible
			ms_kRainSprayVertices[i].m_kColor = s_VisibleColor;
			ms_kRainSprayVertices[i + 1].m_kColor = s_VisibleColor;
			ms_kRainSprayVertices[i + 2].m_kColor = s_VisibleColor;

		}
		else
		{
			// change to invisible
			ms_kRainSprayVertices[i].m_kColor = s_InvisibleColor;
			ms_kRainSprayVertices[i + 1].m_kColor = s_InvisibleColor;
			ms_kRainSprayVertices[i + 2].m_kColor = s_InvisibleColor;
		}
		++ nSprayIndex;
	}
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineRainyEvent::RenderRain()
{
	if( GetEventState() != EES_Running )
		return;

	MeEngine::RenderSystem* pRenderer = MeEngine::Root::getSingleton().getRenderSystem();
	if( !pRenderer )
	{ return; }

    pRenderer->_setDepthBufferCheckEnabled(true );
	pRenderer->_setDepthBufferWriteEnabled(false);
    pRenderer->_setAlphaRejectSettings(MeEngine::CMPF_ALWAYS_PASS,0x00000014,false);
    pRenderer->_setSceneBlending( MeEngine::SOURCE_ALPHA, MeEngine::ONE_MINUS_SOURCE_ALPHA );
    pRenderer->_setCullingMode(MeEngine::CULL_NONE);

    MeEngine::LayerBlendModeEx blendModeEx;
    blendModeEx.blendType = MeEngine::LBT_COLOUR;
    blendModeEx.operation = MeEngine::LBX_MODULATE;
    blendModeEx.source1 = MeEngine::LBS_TEXTURE;
    blendModeEx.source2 = MeEngine::LBS_DIFFUSE;
    r->_setTextureBlendMode(0,blendModeEx);

    blendModeEx.blendType = MeEngine::LBT_ALPHA;
    r->_setTextureBlendMode(0,blendModeEx);

    pRenderer->_setTextureUnitFiltering(0,MeEngine::FO_LINEAR,MeEngine::FO_LINEAR,MeEngine::FO_LINEAR);

	MeEngine::Matrix4 matView;
	Vector3 vPos( 0, 0, 0 );
	if (GetMe3dConfig()->GetCamera()->IsLeftHand())
	{
		MatrixLookAtLH( &matView, &vPos, &GetMe3dConfig()->GetCamera()->GetViewDir(), &Vector3(0,0,1) );
	}
	else
	{
		MatrixLookAtRH( &matView, &vPos, &GetMe3dConfig()->GetCamera()->GetViewDir(), &Vector3(0,0,1) );
	}
	MeEngine::RenderSystem* pDevice = ( MeEngine::RenderSystem* )pRenderer->GetDevice();

    Renderer::Instance()->SetShaderView(matView);
    GetMe3dConfig()->GetCamera()->SetShaderWorld(MeEngine::Matrix4::IDENTITY);

	Renderer::Instance()->SetFVF( Renderer::Instance()->GetVDPosTex() );
    MeEngine::VertexBufferBinding bufferBinding;
    r->setVertexBufferBinding( &bufferBinding );
    pRenderer->_disableTextureUnit( 0);

    pRenderer->_setTexture( 0,true, ms_nRainTextureId );

	pRenderer->MexCommit();

	pRenderer->DrawPrimitiveUP( MeEngine::RenderOperation::TRIANGLE_LIST, ms_nRenderCount / 3, ( void* )&ms_kRainVertices, sizeof( MeRainVertex ) );
}
//////////////////////////////////////////////////////////////////////////
void MeTimelineRainyEvent::RenderRainSpray()
{
	if( GetEventState() != EES_Running )
		return;
    
	MeEngine::RenderSystem* pRenderer = MeEngine::Root::getSingleton().getRenderSystem();
	if( !pRenderer )
	{ return; }

    pRenderer->_setDepthBufferCheckEnabled(true );
	pRenderer->_setDepthBufferWriteEnabled(false);
    pRenderer->_setAlphaRejectSettings(MeEngine::CMPF_ALWAYS_PASS,0x00000014,false);
    pRenderer->_setSceneBlending( MeEngine::SOURCE_ALPHA, MeEngine::ONE_MINUS_SOURCE_ALPHA );
    pRenderer->_setCullingMode(MeEngine::CULL_NONE);

    MeEngine::LayerBlendModeEx blendModeEx;
    blendModeEx.blendType = MeEngine::LBT_COLOUR;
    blendModeEx.operation = MeEngine::LBX_MODULATE;
    blendModeEx.source1 = MeEngine::LBS_TEXTURE;
    blendModeEx.source2 = MeEngine::LBS_DIFFUSE;
    r->_setTextureBlendMode(0,blendModeEx);

    blendModeEx.blendType = MeEngine::LBT_ALPHA;
    r->_setTextureBlendMode(0,blendModeEx);

    pRenderer->_setTextureUnitFiltering(0,MeEngine::FO_LINEAR,MeEngine::FO_LINEAR,MeEngine::FO_LINEAR);

	MeEngine::RenderSystem* pDevice = ( MeEngine::RenderSystem* )pRenderer->GetDevice();

	Renderer::Instance()->SetFVF( Renderer::Instance()->GetVDPosDiffuseTex() );
    MeEngine::VertexBufferBinding bufferBinding;
    r->setVertexBufferBinding( &bufferBinding );
    pRenderer->_setTexture( 0, true, ms_nRainSprayTextureId );

	pRenderer->MexCommit();

	pRenderer->DrawPrimitiveUP( MeEngine::RenderOperation::TRIANGLE_LIST, ms_nRenderCount / 3, ( void* )&ms_kRainSprayVertices, sizeof( MeRainSprayVertex ) );
}
