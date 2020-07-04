#ifndef TABLETHANDLERPOOL_H
#define TABLETHANDLERPOOL_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include "tablethandler.h"
#include "database/datamodel.h"

class TabletHandlerPool : public QObject {
    Q_OBJECT

public:
    explicit TabletHandlerPool();
    
    /** Destructor */
    virtual ~TabletHandlerPool();

    /** Get a free connection handler, or 0 if not available. */
    TabletHandler* getConnectionHandler();
    void setDataModel(DataModel* dataModel);

private:


    /** Pool of connection handlers */
    QList<TabletHandler*> _pool;

    /** Timer to clean-up unused connection handler */
    QTimer _cleanupTimer;

    /** Used to synchronize threads */
    QMutex _mutex;

    DataModel* _dataModel;

private slots:

    /** Received from the clean-up timer.  */
    void cleanup();

};

#endif // TABLETHANDLERPOOL_H
