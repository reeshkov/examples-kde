#include <QCoreApplication>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include <KNotification>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    KNotification *notification = new KNotification("test");
    notification->setComponentName(QStringLiteral("notifycation"));
    notification->setTitle(QStringLiteral("Title"));
    notification->setText(QString("Text"));

    QObject::connect(notification, &KNotification::closed, &app, [&app, notification](){
        qDebug() << "closed" << notification->appName();
        app.quit();
    });
    QObject::connect(notification, static_cast<void (KNotification::*)(unsigned int)>(&KNotification::activated), &app, [](unsigned int action){ qDebug() << "activated" << action; }, Qt::QueuedConnection);
    QObject::connect(notification, &KNotification::ignored, &app, [](){ qDebug() << "ignored"; });
    QObject::connect(notification, &KNotification::defaultActivated, &app, [](){ qDebug() << "defaultActivated"; });


    QTimer::singleShot(1, notification, SLOT(sendEvent()));
    
    qDebug() << "started";

    return app.exec();
}

#include "main.moc"
