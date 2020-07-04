#include "connectivity.h"
#include <QStringList>
#include <QList>
#include <QNetworkInterface>

QStringList Connectivity::getIPAddressList() {

    QStringList list;
    QList<QNetworkInterface> networkInterfaceList = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, networkInterfaceList) {

         // if interface is up and running and is not loopback
        if ((interface.flags() & (QNetworkInterface::IsRunning | QNetworkInterface::IsUp)) &&
            !(interface.flags() & QNetworkInterface::IsLoopBack)) {

            QList<QNetworkAddressEntry> addressList = interface.addressEntries();
            foreach (QNetworkAddressEntry addressEntry, addressList) {
                QHostAddress hostAddress = addressEntry.ip();
                if (hostAddress.toIPv4Address()) list.append(hostAddress.toString());
            }

        }
    }

    return list;
}
