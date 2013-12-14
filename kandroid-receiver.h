#ifndef KWRITED_H
#define KWRITED_H
 
#include <kdedmodule.h>
#include <QUdpSocket>

class KAndroidReceiver : public QObject {
    enum event_types {UNKNOWN, RING,SMS,MMS,BATTERY,PING};

Q_OBJECT
public:
    KAndroidReceiver();
    ~KAndroidReceiver();
    virtual void receiveMessage(char *version, char *device_id, char *notification_id, event_types event_type, char *data, char *event_contents);

private Q_SLOTS:
    void readNotification();

private:
    QUdpSocket *udpSocket;
    void parseMessage(QByteArray* datagram);
};
 
#endif
