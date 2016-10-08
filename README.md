3D Weather Event Manager

Code snippet of a published 3D MMORPG

This is a weather controller, currently we support rainy, cloudy and sound event. Put these together we would get a real rainy effect with thunder( sound ), lighting( sunny to cloudy and back to sunny very shortly ). 


Files:

Config loader:
    WeatherEventConfig.h/cpp

Manager:
    WeatherEventManager.h/cpp
    MeTimeLine.h
    MeCycleTimer.h

Event:
    MeTimelineEvent.h (base class)
    MeTimelineRainyEvent.h/cpp
    MeTimelineSoundEvent.h/cpp
    MeTimelineCloudySkyEvent.h/cpp
