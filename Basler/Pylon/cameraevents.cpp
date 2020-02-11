#include "cameraevents.h"

void Pylon::CameraEventPrinter::OnAttach(Pylon::CInstantCamera &camera)
{
    Q_UNUSED(camera)
    emit Message(LoggerMsg::Info,"BSL: OnAttach event.");
}

void Pylon::CameraEventPrinter::OnAttached(Pylon::CInstantCamera &camera)
{
    Q_UNUSED(camera)
    emit Message(LoggerMsg::Info,"BSL: OnAttached event for device. ");
}

void Pylon::CameraEventPrinter::OnOpen(Pylon::CInstantCamera &camera)
{
    Q_UNUSED(camera)
    emit Message(LoggerMsg::Info,"BSL: OnOpen event for device. ");
}

void Pylon::CameraEventPrinter::OnOpened(Pylon::CInstantCamera &camera)
{
    Q_UNUSED(camera)
    emit Message(LoggerMsg::Info,"BSL: OnOpened event for device. ");
    emit cameraOpenedChanged(true);
}

void Pylon::CameraEventPrinter::OnGrabStart(Pylon::CInstantCamera &camera)
{
    Q_UNUSED(camera)
    emit Message(LoggerMsg::Info,"BSL: OnGrabStart event for device. ");
}

void Pylon::CameraEventPrinter::OnGrabStarted(Pylon::CInstantCamera &camera)
{
    Q_UNUSED(camera)
    emit Message(LoggerMsg::Info,"BSL: OnGrabStarted event for device. ");
}

void Pylon::CameraEventPrinter::OnGrabStop(Pylon::CInstantCamera &camera)
{
    Q_UNUSED(camera)
    emit Message(LoggerMsg::Info,"BSL: OnGrabStop event for device. ");
}

void Pylon::CameraEventPrinter::OnGrabStopped(Pylon::CInstantCamera &camera)
{
    Q_UNUSED(camera)
    emit Message(LoggerMsg::Info,"BSL: OnGrabStopped event for device. ");
}

void Pylon::CameraEventPrinter::OnClose(Pylon::CInstantCamera &camera)
{
    Q_UNUSED(camera)
    emit Message(LoggerMsg::Info,"BSL: OnClose event for device. ");
    emit cameraOpenedChanged(false);
}

void Pylon::CameraEventPrinter::OnClosed(Pylon::CInstantCamera &camera)
{
    Q_UNUSED(camera)
    emit Message(LoggerMsg::Info,"BSL: OnClosed event for device. ");
}

void Pylon::CameraEventPrinter::OnDestroy(Pylon::CInstantCamera &camera)
{
    Q_UNUSED(camera)
    emit Message(LoggerMsg::Info,"BSL: OnDestroy event for device. ");
}

void Pylon::CameraEventPrinter::OnDestroyed(Pylon::CInstantCamera &camera)
{
    Q_UNUSED(camera)
}

void Pylon::CameraEventPrinter::OnDetach(Pylon::CInstantCamera &camera)
{
    Q_UNUSED(camera)
    emit Message(LoggerMsg::Info,"BSL: OnDetach event for device. ");
}

void Pylon::CameraEventPrinter::OnDetached(Pylon::CInstantCamera &camera)
{
    Q_UNUSED(camera)
    emit Message(LoggerMsg::Info,"BSL: OnDetached event for device. ");
}

void Pylon::CameraEventPrinter::OnGrabError(Pylon::CInstantCamera &camera, const char *errorMessage)
{
    QString temp=QString("BSL: OnGrabError event for device : %1 : %2").arg(QString::fromLocal8Bit(camera.GetDeviceInfo().GetModelName()),errorMessage);
    emit Message(LoggerMsg::Warning,temp);
}

void Pylon::CameraEventPrinter::OnCameraDeviceRemoved(Pylon::CInstantCamera &camera)
{

    Q_UNUSED(camera)
    emit Message(LoggerMsg::Warning,"BSL: OnCameraDeviceRemoved event for device");
    emit cameraRemovedChanged();
}

