#include <kapplication.h>
#include <kcmdlineargs.h>
#include <KAboutData>

#include "kandroid-receiver.h"

static inline KAboutData aboutData() {
    return KAboutData("kandroid-receiver", 0, ki18n("kandroid-receiver"), "0.1");
}

int main(int argc, char **argv) {
    KAboutData about = aboutData();
    KCmdLineArgs::init(argc, argv, &about);
    KApplication a;
    KAndroidReceiver w;

    return a.exec();
}
