#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    Player=new GstPlayer(this);
    Queue=new sampleQueue(this);
    Sett= new Settings(this);
    mAbstractSurface=new AbstractSurface(this);
    mAbstractSurface->setSampleQueue(Queue);
    Player->setSampleQueue(Queue);
    connect(Queue,&sampleQueue::NewSample,mAbstractSurface,&AbstractSurface::NewSample);
    connect(Player,&GstPlayer::GetSize,mAbstractSurface,&AbstractSurface::SetVideoSize);
    Player->Prepare(Sett->multiIP(),Sett->multiPort());
}


void Controller::resolutionChange()
{
    Player->Stop();
    Sett->Next();
    Player->Prepare(Sett->multiIP(),Sett->multiPort());
}


