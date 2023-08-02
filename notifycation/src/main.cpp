#include <QCoreApplication>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include <KNotification>
#include <KNotificationReplyAction>
//#include <memory>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    qDebug() << "started";

    QStringList args(QCoreApplication::arguments());
    int args_count = args.count();
    qDebug() << "args" << args_count << args;
    Qstring event("test1");
    if(1 <= args_count) event = args.at(0);
    Qstring appName("notifytest");
    if(2 <= args_count) appName = args.at(1);
    Qstring title("Title");
    if(3 <= args_count) title = args.at(2);
    Qstring text("Text");
    if(4 <= args_count) text = args.at(3);

    KNotification *notification = new KNotification(event);
    notification->setComponentName(QStringLiteral(appName));
    notification->setTitle(QStringLiteral(title));
    notification->setText(QString(text));


    QStringList actions{QStringLiteral("OK")};
    notification->setActions(actions);

//    auto replyAction = std::unique_ptr<KNotificationReplyAction>(new KNotificationReplyAction(QStringLiteral("Reply")));
//    replyAction->setPlaceholderText(QStringLiteral("Reply to Dave..."));
//    QObject::connect(replyAction.get(), &KNotificationReplyAction::replied, [](const QString &text)
//                     {
//                         qDebug() << "you replied with" << text;
//                     });
//    notification->setReplyAction(std::move(replyAction));

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
