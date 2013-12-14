kded_android-receiver
=====================

Display Android notifications (RING, SMS, etc) as KDE notifications.
It needs [android-notifier](http://code.google.com/p/android-notifier) installed and configured on your android device.

Build
------

    cmake .
    make
    make install # As root
    
It will add a notification configurable in systemsettings and it will be launched automatically at KDE launch.
