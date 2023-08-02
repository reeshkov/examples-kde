#include <QCoreApplication>
#include <QStringList>
#include <QDebug>
#include <QTimer>
#include <KNotification>
#include <KNotificationReplyAction>
#include <memory>

class Notificator : public QObject
{
    Q_OBJECT

public:
    Notificator(QObject * parent = nullptr) : QObject(parent) {}

private slots:
    void doNotify();

signals:
    void finished();
};

void Notificator::doNotify()
{
    QString filePath = QStringLiteral("/home/omv/picture.png");
    KNotification *notification = new KNotification("captured");
    notification->setComponentName(QStringLiteral("plasma_phone_components"));
    notification->setTitle(QStringLiteral("Nice picture"));
    notification->setUrls({QUrl::fromLocalFile(filePath)});
    notification->setText(QString("Nice picture is in <br/><b>%1</b>").arg(filePath));
    notification->setIconName(filePath);

    //QStringList actions{QStringLiteral("OK")};
    //notification->setActions(actions);

    auto replyAction = std::unique_ptr<KNotificationReplyAction>(new KNotificationReplyAction(QStringLiteral("Reply")));
    replyAction->setPlaceholderText(QStringLiteral("Reply to Dave..."));
    QObject::connect(replyAction.get(), &KNotificationReplyAction::replied, [](const QString &text)
                     {
                         qDebug() << "you replied with" << text;
                     });
    notification->setReplyAction(std::move(replyAction));


    connect(notification, &KNotification::closed, this, &Notificator::finished);

    notification->sendEvent();
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Notificator n;

    QObject::connect(&n, SIGNAL(finished()), &app, SLOT(quit()));

    QTimer::singleShot(0, &n, SLOT(doNotify()));

    return app.exec();
}

#include "main.moc"
