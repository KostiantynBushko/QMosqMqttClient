#ifndef QMMQTTCLIENT_H
#define QMMQTTCLIENT_H

#include <QObject>
#include <string>
#include <mosquittopp.h>

/**
 * @brief The QMMqttClient class
 */
class QMMqttClient : public QObject, mosqpp::mosquittopp {

    Q_OBJECT
public:
    explicit QMMqttClient(QObject *parent = nullptr);

    QMMqttClient(QMMqttClient &&) = default;

    QMMqttClient(const QMMqttClient &) = delete;

    QMMqttClient & operator =(const QMMqttClient &) = delete;

    ~QMMqttClient() override;

    void initialize(const QString & client_id, const QString & host, int port);

    void setTls(const QString & ca_file, const QString & cert_file, const QString & key_file);

    void connect();

    void subscribeTopic(const QString& topic);

    void publishMesage(const QString & topic, const QString & msg);

    QString version();

signals:
    void onConnected();
    void onDisconnect();
    void onMessageReceived(const QString & topic, const QByteArray& payload);

private:
    void on_connect(int rc) override;
    void on_disconnect(int rc) override;
    void on_message(const struct mosquitto_message * message) override;
    void on_subscribe(int mid, int qos_count, const int *granted_qos) override;

private:
    QString m_ClientID;
    QString m_Host;
    int m_Port;

    QString m_RootCAFile;
    QString m_Certificate;
    QString m_PrivateKey;
};

#endif // QMMQTTCLIENT_H
