#include <KAboutData>
#include <KLocale>
#include <knotification.h>
#include <kpluginfactory.h>
#include <kpluginloader.h>

#include <stdio.h>

#include "kandroid-receiver.h"

static KAboutData aboutData("kandroid-receiver", 0, ki18n("kandroid-receiver"), "0.1");

class KAndroidReceiverModule : public KDEDModule {
public:
    KAndroidReceiverModule(QObject* parent, const QList<QVariant>&);
    ~KAndroidReceiverModule();

private:
    KAndroidReceiver *pro;
};

KAndroidReceiverModule::KAndroidReceiverModule(QObject* parent, const QList<QVariant>&) : KDEDModule(parent) {
    pro = new KAndroidReceiver();
}

KAndroidReceiverModule::~KAndroidReceiverModule() {
    delete pro;
}

K_PLUGIN_FACTORY(KAndroidReceiverFactory, registerPlugin<KAndroidReceiverModule>();)
K_EXPORT_PLUGIN(KAndroidReceiverFactory(aboutData))

KAndroidReceiver::KAndroidReceiver() : QObject() {
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(10600);

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readNotification()));
}

KAndroidReceiver::~KAndroidReceiver() {
    delete udpSocket;
}

void KAndroidReceiver::readNotification() {
    while(udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;

        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        parseMessage(&datagram);
    }
}

void KAndroidReceiver::parseMessage(QByteArray* datagram) {
    int index = 0;
    char *version, *device_id, *notification_id, *event_type_str, *data, *event_contents;
    enum {VERSION, DEVICE_ID, NOTIFICATION_ID, EVENT_TYPE, DATA, EVENT_CONTENTS} field = VERSION;
    event_types event_type = UNKNOWN;

    version = device_id = notification_id = event_type_str = data = event_contents = NULL;
    version = datagram->data();
    while(index < datagram->size()) {
        index = datagram->indexOf('/', index);
        if(index < 0)
          break;
        
        datagram->data()[index] = '\0';
        index++;
        
        switch(field) {
          case VERSION:
            device_id = datagram->data() + index;
            field = DEVICE_ID;
            break;
          case DEVICE_ID:
            notification_id = datagram->data() + index;
            field = NOTIFICATION_ID;
            break;
          case NOTIFICATION_ID:
            event_type_str = datagram->data() + index;
            field = EVENT_TYPE;
            break;
          case EVENT_TYPE:
            if(!strcmp(event_type_str, "RING"))
              event_type = RING;
            else if(!strcmp(event_type_str, "SMS"))
              event_type = SMS;
            else if(!strcmp(event_type_str, "MMS"))
              event_type = MMS;
            else if(!strcmp(event_type_str, "BATTERY"))
              event_type = BATTERY;
            else if(!strcmp(event_type_str, "PING"))
              event_type = PING;
            else
              event_type = UNKNOWN;
            
            data = datagram->data() + index;
            field = DATA;
            break;
          case DATA:
            event_contents = datagram->data() + index;
            field = EVENT_CONTENTS;
            break;
          case EVENT_CONTENTS:
            break;
        }
    }
    
    receiveMessage(version, device_id, notification_id, event_type, data, event_contents);
}

void KAndroidReceiver::receiveMessage(char *version, char *device_id, char *notification_id, event_types event_type, char *data, char *event_contents) {
    printf("version: %s device_id: %s notification_id: %s event_type: %i, data: %s event_contents: %s\n",
        version, device_id, notification_id, event_type, data, event_contents
    );
    
    KNotification::event("notification", QString::fromUtf8(event_contents));
}

#include "kandroid-receiver.moc"
