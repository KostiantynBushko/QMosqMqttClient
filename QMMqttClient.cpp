#include "QMMqttClient.h"
#include <QDebug>
#include <cstring>

QMMqttClient::QMMqttClient(QObject *parent) : QObject(parent) {

}

QMMqttClient::~QMMqttClient() {
    mosqpp::mosquittopp::disconnect();
    mosqpp::mosquittopp::loop_stop();
    mosqpp::lib_cleanup();
}

void QMMqttClient::initialize(const QString &client_id, const QString &host, int port) {
    qDebug() << Q_FUNC_INFO << client_id << host << port;
    m_ClientID = client_id;
    m_Host = host;
    m_Port = port;
    mosqpp::lib_init();
}

void QMMqttClient::setTls(const QString &ca_file, const QString &cert_file, const QString &key_file) {
    qDebug() << Q_FUNC_INFO;
    qDebug() << "- ca_file: " << ca_file;
    qDebug() << "- cert_file: " << cert_file;
    qDebug() << "- key_file: " << key_file;

    m_RootCAFile = ca_file;
    m_Certificate = cert_file;
    m_PrivateKey = key_file;
}

void QMMqttClient::connect() {
    qDebug() << Q_FUNC_INFO;
    if (!m_RootCAFile.isEmpty()) {
        if (m_Certificate.isEmpty() || m_PrivateKey.isEmpty()) {
            mosqpp::mosquittopp::tls_set(m_RootCAFile.toStdString().c_str());
        } else {
            mosqpp::mosquittopp::tls_set(m_RootCAFile.toStdString().c_str(), nullptr, m_Certificate.toStdString().c_str(), m_PrivateKey.toStdString().c_str());
        }
    }
    int keep_alive = 120;
    mosqpp::mosquittopp::connect_async(m_Host.toStdString().c_str(), m_Port, keep_alive);
    int rc = loop_start();
    if (rc != MOSQ_ERR_SUCCESS) {
        qCritical() << Q_FUNC_INFO << " Failed start loop, return code: " << rc;
    }
    mosqpp::mosquittopp::threaded_set(true);
}

void QMMqttClient::subscribeTopic(const QString &topic) {
    qDebug() << Q_FUNC_INFO << ": topic: " << topic;
    mosqpp::mosquittopp::subscribe(nullptr, topic.toStdString().c_str());
}

void QMMqttClient::publishMesage(const QString &topic, const QString &msg) {
    int rc = mosqpp::mosquittopp::publish(nullptr, topic.toStdString().c_str(), (int)msg.toStdString().size(), msg.toStdString().c_str());
    qDebug() << Q_FUNC_INFO << "return code: " << rc << ", topic: " << topic << ", message: " << msg;
}

QString QMMqttClient::version() {
    int major{0};
    int minor{0};
    int rev{0};
    mosqpp::lib_version(&major, &minor, &rev);
    return QString::number(major) + "." + QString::number(minor) + "." + QString::number(rev);
}

void QMMqttClient::on_connect(int rc) {
    if (rc == MOSQ_ERR_SUCCESS) {
        qDebug() << Q_FUNC_INFO << " Sucessfully connected to host: " << m_Host << " port: " << m_Port;
        emit onConnected();
    } else {
        qCritical() << Q_FUNC_INFO << " Error connected return code: " << rc;
    }
}

void QMMqttClient::on_disconnect(int rc) {
    qDebug() << Q_FUNC_INFO << ": return code: " << rc;
    emit onDisconnect();
}

void QMMqttClient::on_message(const mosquitto_message *message) {
    QString topic(message->topic);
    QByteArray msg((char*)(message->payload), message->payloadlen);
    qDebug() << Q_FUNC_INFO << " Topic: " << topic << ", Msg: " << QString::fromStdString(msg.toStdString());
    emit onMessageReceived(topic, msg);
}

void QMMqttClient::on_subscribe(int mid, int qos_count, const int *granted_qos) {
    Q_UNUSED(mid)
    Q_UNUSED(qos_count)
    Q_UNUSED(granted_qos)
    qDebug() << Q_FUNC_INFO << " mid: " << mid << ", QoS: " << qos_count;
}

