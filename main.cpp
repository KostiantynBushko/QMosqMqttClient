#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include "QMMqttClient.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QMMqttClient client;

    QObject::connect(&client, &QMMqttClient::onConnected, [&client](){
        qDebug() << Q_FUNC_INFO << " QMMqttClient::onConnected handler, subscribe to topic...";
        client.subscribeTopic("command/stop");
    });

    QObject::connect(&client, &QMMqttClient::onMessageReceived, [](const QString &topic, const QByteArray &msg) {
        qDebug() << Q_FUNC_INFO << " QMMqttClient::onMessageReceived: topic: " << topic << ", message: " << QString::fromStdString(msg.toStdString());
    });

    /* An example of unsecure connection */
    client.initialize("MClient", "127.0.0.1", 1883);

    /* An example of secure connectionm.. */
    // client.initialize("MClient", "my.host.com", 8883);
    // client.setTls("absolute path to root CA file", "absolute path to certificate file", "absolute path to privae key file");

    /* The second and third argument can be leve and just set to QString() default value. */
    // client.setTls("absolute path to root CA file", QString(), QString());

    client.connect();

    QTimer *timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, [&client](){
        QString msg("Mosquitto Client V(");
        msg.append(client.version());
        msg.append(") ");
        msg.append(QTime::currentTime().toString());
        client.publishMesage("test/topic", msg);
    });
    timer->start(5000);

    return a.exec();
}
