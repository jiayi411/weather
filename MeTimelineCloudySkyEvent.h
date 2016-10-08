#pragma once
#include <MeFoundation/MeTimelineShadeEvent.h>

/**
* @brief Sunny to cloudy or cloudy to sunny
*/
class MEFOUNDATION_ENTRY MeTimelineCloudySkyEvent : public MeTimelineShadeEvent
{
// Construction.
public:
	/// Constructor.
	MeTimelineCloudySkyEvent(){
		m_kOriginalAmbient = D3DXCOLOR( 0, 0, 0, 0 );
		m_kOriginalDiffuse = D3DXCOLOR( 0, 0, 0, 0 );
		m_kDestinationAmbient = D3DXCOLOR( 0, 0, 0, 0 );
		m_kDestinationDiffuse = D3DXCOLOR( 0, 0, 0, 0 );
		m_kCurrentAmbient = D3DXCOLOR( 0, 0, 0, 0 );
		m_kCurrentDiffuse = D3DXCOLOR( 0, 0, 0, 0 );

	}

	/// Copy constructor.
	MeTimelineCloudySkyEvent( const MeTimelineCloudySkyEvent& kClass ){
		m_kOriginalAmbient = kClass.m_kOriginalAmbient;
		m_kOriginalDiffuse = kClass.m_kOriginalDiffuse;
		m_kDestinationAmbient = kClass.m_kDestinationAmbient;
		m_kDestinationDiffuse = kClass.m_kDestinationDiffuse;
		m_kCurrentAmbient = kClass.m_kCurrentAmbient;
		m_kCurrentDiffuse = kClass.m_kCurrentDiffuse;
	}

// Overload operator.
public:
	/// Assignment operator.
	const MeTimelineCloudySkyEvent& operator=( const MeTimelineCloudySkyEvent& kClass ){
		m_kOriginalAmbient = kClass.m_kOriginalAmbient;
		m_kOriginalDiffuse = kClass.m_kOriginalDiffuse;
		m_kDestinationAmbient = kClass.m_kDestinationAmbient;
		m_kDestinationDiffuse = kClass.m_kDestinationDiffuse;
		m_kCurrentAmbient = kClass.m_kCurrentAmbient;
		m_kCurrentDiffuse = kClass.m_kCurrentDiffuse;
		return *this;
	}

// Override.
public:
	/// Destructor.
	virtual ~MeTimelineCloudySkyEvent(){}

	/// Destroy.
	virtual void DestroyEvent() {}

	/// Render.
	virtual void RenderEvent() {}

	virtual void DoFirstInRunning( const MeTimeValue& kTime );

	virtual void UpdateBeginToFull( const MeTimeValue& kTime );

	virtual void UpdateInFull( const MeTimeValue& kTime );

	virtual void UpdateFullToEnd( const MeTimeValue& kTime );

// Methods.
public:
	const D3DXCOLOR& GetOriginalAmbient() const { return m_kOriginalAmbient; }
	void SetOriginalAmbient(const D3DXCOLOR& val) { m_kOriginalAmbient = val; }

	const D3DXCOLOR& GetOriginalDiffuse() const { return m_kOriginalDiffuse; }
	void SetOriginalDiffuse(const D3DXCOLOR& val) { m_kOriginalDiffuse = val; }

	const D3DXCOLOR& GetDestinationAmbient() const { return m_kDestinationAmbient; }
	void SetDestinationAmbient(const D3DXCOLOR& val) { m_kDestinationAmbient = val; }

	const D3DXCOLOR& GetDestinationDiffuse() const { return m_kDestinationDiffuse; }
	void SetDestinationDiffuse(const D3DXCOLOR& val) { m_kDestinationDiffuse = val; }

	const D3DXCOLOR& GetCurrentAmbient() const { return m_kCurrentAmbient; }
	void SetCurrentAmbient(const D3DXCOLOR& val) { m_kCurrentAmbient = val; }

	const D3DXCOLOR& GetCurrentDiffuse() const { return m_kCurrentDiffuse; }
	void SetCurrentDiffuse(const D3DXCOLOR& val) { m_kCurrentDiffuse = val; }

// Methods.
protected:

// Members.
protected:
	D3DXCOLOR m_kOriginalAmbient;
	D3DXCOLOR m_kOriginalDiffuse;
	D3DXCOLOR m_kDestinationAmbient;
	D3DXCOLOR m_kDestinationDiffuse;
	D3DXCOLOR m_kCurrentAmbient;
	D3DXCOLOR m_kCurrentDiffuse;
};
