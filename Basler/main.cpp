#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QMetaType>
#include "Pylon/pylonfeaturesenum.h"
#include "settings.h"
#include <QSystemSemaphore>
#include <QSharedMemory>
#include "unitsmanager.h"
#include "../CPP/log.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<LoggerMsg>("LoggerMsg");
    qRegisterMetaType<DeviceState>("DeviceState");
    //проверка на запуск одного экземпляра приложения
    QSystemSemaphore semaphore("<BaslerSystemSemaphore>", 1);
    bool isRunning;
    semaphore.acquire();
    //такой вход-выход нужен чтобы запустить проверку ссылок на обьект и разрушить его, если ссылок больше нет
    //работает, если предыдущий экземпляр приложения вылетел, не закрыв обьект
    {
        QSharedMemory shmem("<BaslerSharedMemory>");
        shmem.attach();
    }
    QSharedMemory shmem("<BaslerSharedMemory>");
    if (shmem.attach())
    {
        isRunning = true;
    }
    else
    {
        shmem.create(1);
        isRunning = false;
    }
    semaphore.release();
    if (isRunning)
    {
        qWarning("Повторный запуск приложения невозможен.");
        return 0;
    }

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    Settings* settings=new Settings(&app);

    UnitsManager* unitsManager=new UnitsManager(settings,&app);

    if (settings->getNeedGUI())
    {
        QQmlApplicationEngine * engine=new QQmlApplicationEngine(&app);
        QQmlContext *context = engine->rootContext();
        context->setContextProperty("unitsManager",unitsManager);
        qmlRegisterUncreatableType<PylonFeaturesEnum>("com.PylonFeaturesEnum", 1, 0, "PylonFeaturesEnum", "Pylon Features enum create fail.");
        engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
        if (engine->rootObjects().isEmpty())  return -1;
    }
    app.exec();
}
