#pragma once

#include <QObject>
#include <QDebug>
#include "../CPP/global.h"
#include "abstractsurface.h"
#include "gstplayer.h"
#include "samplequeue.h"
#include "settings.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    Q_PROPERTY(AbstractSurface* abs MEMBER mAbstractSurface CONSTANT)
public slots:
     void resolutionChange() ;
private:
    sampleQueue* Queue;
    GstPlayer* Player;
    AbstractSurface* mAbstractSurface;
    Settings* Sett;
};

