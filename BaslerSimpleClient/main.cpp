#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include "abstractsurface.h"
#include "controller.h"
#include "unitsmanager.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<DeviceState>("DeviceState");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Controller* controller = new Controller(&app);
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("controller",controller);
    UnitsManager* um = new UnitsManager(&app);
    context->setContextProperty("unitsManager",um);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
