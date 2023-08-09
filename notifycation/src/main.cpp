#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <KNotification>
#include <KNotificationReplyAction>
//#include <memory>

#include <KJob>
#include <KUiServerV2JobTracker>

class TestJob : public KJob
{
    Q_OBJECT

public:
    TestJob(QObject *parent = nullptr);
    void start() override;
public slots:
    void finish();
};


TestJob::TestJob(QObject *parent)
    : KJob(parent)
{
    setProcessedAmount(KJob::Items, 1);
    setTotalAmount(KJob::Items, 1);
    auto caps = capabilities();
    caps.setFlag(KJob::Suspendable, true);
    caps.setFlag(KJob::Killable, true);
    setCapabilities(caps);
}
void TestJob::start()
{
    qDebug() << "KJob::start";
    setTotalAmount(KJob::Items,1);
    setProcessedAmount(KJob::Items,1);

    Q_EMIT description(this,QStringLiteral("Moving"),
                       qMakePair(QStringLiteral("Source"),QUrl(QStringLiteral("/asourcefile")).toString())
                       , qMakePair(QStringLiteral("Destination"),QString().fill('X',500)));
    Q_EMIT infoMessage(this,QString().fill('Z',500),QString());
    QTimer::singleShot(1000, this, &TestJob::finish);
}
void TestJob::finish()
{
    qDebug() << "KJob::finish";
    emitResult();
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    qDebug() << "test started";

    QStringList args(QCoreApplication::arguments());
    int args_count = args.count();
    qDebug() << "args" << args_count << args;
    QString event("test1");
    if(2 <= args_count) event = args.at(1);
    QString appName("notifytest");
    if(3 <= args_count) appName = args.at(2);
    app.setApplicationName(appName);

    QString title("Title");
    if(4 <= args_count) title = args.at(3);
    QString text("Text");
    if(5 <= args_count) text = args.at(4);

    KNotification *notification = new KNotification(event);
    notification->setComponentName(appName);
    notification->setIconName("system-file-manager");
    notification->setTitle(title);
    notification->setText(text);
    notification->setFlags(KNotification::Persistent | KNotification::SkipGrouping);
    notification->setUrls({QUrl(QStringLiteral("trash:/"))});


    // gdbus call --session --dest org.freedesktop.Notifications --object-path /org/freedesktop/Notifications --method org.freedesktop.Notifications.Notify "notifytest" 0 "" "Title" "Message" "[\"OK\",\"OK\"]" "{}" 3000
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
        qDebug() << "notify closed" << notification->appName() << notification->id();
        //app.quit();
    });
    QObject::connect(notification, static_cast<void (KNotification::*)(unsigned int)>(&KNotification::activated), &app, [](unsigned int action){ qDebug() << "activated" << action; }, Qt::QueuedConnection);
    QObject::connect(notification, &KNotification::ignored, &app, [](){ qDebug() << "ignored"; });
    QObject::connect(notification, &KNotification::defaultActivated, &app, [](){ qDebug() << "defaultActivated"; });


    QTimer::singleShot(10, notification, SLOT(sendEvent()));

    TestJob * m_job = new TestJob(nullptr);
    //m_job->setProperty("destUrl", QUrl(QStringLiteral("trash:/")));
    m_job->setFinishedNotificationHidden(false);


    QObject::connect(m_job, &KJob::finished, &app, [] {
        qDebug() << "KJob::finished";
    });
    QObject::connect(m_job, &KJob::suspended, &app, [] {
        qDebug() << "KJob::suspended";
    });
    QObject::connect(m_job, &KJob::result, &app, [] {
        qDebug() << "KJob::result";
    });
    KUiServerV2JobTracker * m_uiServerV2Tracker = new KUiServerV2JobTracker(nullptr);
    m_job->setProperty("desktopFileName",appName);
    m_uiServerV2Tracker->registerJob(m_job);

    QTimer::singleShot(0, m_job, &TestJob::start);


    qDebug() << "exec: notificationId=" << notification->id() << appName;
    return app.exec();
}

#include "main.moc"
