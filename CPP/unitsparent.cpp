#include "unitsparent.h"

UnitsParent::UnitsParent(QObject *parent) : QObject(parent), id("undefined"), waitingTime(500)
{

}

QString UnitsParent::getId() const
{
    return id;
}

