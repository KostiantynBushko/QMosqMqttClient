# Brife description:
 An implementation of simple Qt wrapper for Mosquitto Mqtt C client.
 Provides functionality to establish secure and not secure connection to Mqtt broker,
 publish and receive message in synchronous or asynchronous mode with support multithreading features.

# Source
 https://github.com/eclipse/mosquitto.git

# Main aim:
 The main goal of this app is to simplify integration of mqtt protocol feature to the iot solutions, developed using Qt SDK.
 The wrapper contains high level abstraction which is common for every possible implementation of mqtt client like AWS IoT mqtt client implementation.

# An interface implementation
    connect
    disconnect
    publish
    subscribe
    To emit signals on each step of client work flow...
    
# Install Mosquitto Mqtt Client Library (linux debian "RP")
    $ sudo apt-get install libmosquittopp-dev

Copy files in to your project.
    QMMqttClient.cpp, QMMqttClient.h

Add library to *.pro file:
    LIBS += -lmosquittopp
