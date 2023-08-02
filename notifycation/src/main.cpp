#include <QCoreApplication>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include <KNotification>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    qDebug() << "started";

    KNotification *notification = new KNotification("captured");
    notification->setComponentName(QStringLiteral("plasma_phone_components"));
    notification->setTitle(QStringLiteral("Title"));
    notification->setText(QString("Text"));


    //QStringList actions{QStringLiteral("OK")};
    //notification->setActions(actions);

    auto replyAction = std::unique_ptr<KNotificationReplyAction>(new KNotificationReplyAction(QStringLiteral("Reply")));
    replyAction->setPlaceholderText(QStringLiteral("Reply to Dave..."));
    QObject::connect(replyAction.get(), &KNotificationReplyAction::replied, [](const QString &text)
                     {
                         qDebug() << "you replied with" << text;
                     });
    notification->setReplyAction(std::move(replyAction));

    QObject::connect(notification, &KNotification::closed, &app, [&app, notification](){
        qDebug() << "closed" << notification->appName();
        app.quit();
    });
    QObject::connect(notification, static_cast<void (KNotification::*)(unsigned int)>(&KNotification::activated), &app, [](unsigned int action){ qDebug() << "activated" << action; }, Qt::QueuedConnection);
    QObject::connect(notification, &KNotification::ignored, &app, [](){ qDebug() << "ignored"; });
    QObject::connect(notification, &KNotification::defaultActivated, &app, [](){ qDebug() << "defaultActivated"; });


    QTimer::singleShot(1, notification, SLOT(sendEvent()));

    qDebug() << "exec";
    return app.exec();
}

#include "main.moc"
