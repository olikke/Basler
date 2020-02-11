#pragma once
#include <pylon/ConfigurationEventHandler.h>
#include <pylon/InstantCamera.h>
#include <QObject>
#include <QDebug>
#include "../CPP/log.h"

namespace Pylon
{
    //class ;

    class CameraEventPrinter : public QObject, public CConfigurationEventHandler
    {
        Q_OBJECT
    signals:
        void cameraOpenedChanged(bool);
        void cameraRemovedChanged();
        void Message(LoggerMsg lm,  QString message);
    public:
        void OnAttach( CInstantCamera& camera);
        void OnAttached( CInstantCamera& camera);
        virtual void OnOpen( CInstantCamera& camera);
        void OnOpened( CInstantCamera& camera);
        void OnGrabStart( CInstantCamera& camera);
        void OnGrabStarted( CInstantCamera& camera);
        void OnGrabStop( CInstantCamera& camera);
        virtual void OnGrabStopped( CInstantCamera& camera);
        void OnClose( CInstantCamera& camera);
        void OnClosed( CInstantCamera& camera);
        void OnDestroy( CInstantCamera& camera);
        void OnDestroyed( CInstantCamera& camera);
        void OnDetach( CInstantCamera& camera);
        void OnDetached( CInstantCamera& camera);
        void OnGrabError( CInstantCamera& camera, const char* errorMessage);
        void OnCameraDeviceRemoved( CInstantCamera& camera);

    };
}

