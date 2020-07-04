#ifndef TABLETSERVER_H
#define TABLETSERVER_H

#include <QTcpServer>
#include "tablethandlerpool.h"
#include "database/datamodel.h"

class TabletServer : public QTcpServer {
    Q_OBJECT
public:
    explicit TabletServer(int port, DataModel* model);
    
    /** Destructor */
    virtual ~TabletServer();


signals:

    void tabletServerStarted(bool started, int port);

public slots:
    void processStartServer();


protected:

    /** Serves new incoming connection requests */
    void incomingConnection(qintptr sd);

private:

    TabletHandlerPool _pool;
    int _port;
    
};

#endif // TABLETSERVER_H
