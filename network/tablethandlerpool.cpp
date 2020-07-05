#include "tablethandlerpool.h"

#define CLEANUP_TIMER 180000 // 3 min
#define MAX_THREAD 100

TabletHandlerPool::TabletHandlerPool() : QObject() {


    _cleanupTimer.start(CLEANUP_TIMER);
    connect(&_cleanupTimer, SIGNAL(timeout()), SLOT(cleanup()));
}

void TabletHandlerPool::setDataModel(DataModel* dataModel) {
    _dataModel = dataModel;
}

TabletHandlerPool::~TabletHandlerPool() {
    // delete all connection handlers and wait
    foreach(TabletHandler* handler, _pool) {
        delete handler;
    }
    qDebug("ConnectionHandlerPool (%p): destroyed", this);
}


TabletHandler* TabletHandlerPool::getConnectionHandler() {
    TabletHandler* freeHandler=0;
    _mutex.lock();
    // find a free handler in pool
    foreach(TabletHandler* handler, _pool) {
        if (!handler->isBusy()) {
            freeHandler=handler;
            freeHandler->setBusy();
            qDebug() << "Reusing handler " << freeHandler;
            break;
        }
    }
    // create a new handler, if necessary
    if (!freeHandler) {
        int maxConnectionHandlers=MAX_THREAD;
        if (_pool.count()<maxConnectionHandlers) {
            freeHandler=new TabletHandler(_dataModel);
            freeHandler->setBusy();
            _pool.append(freeHandler);
            qDebug() << "Creating new handler " << freeHandler;
        }
    }
    _mutex.unlock();
    qDebug() << "Handler pool size is " << _pool.size();
    return freeHandler;
}



void TabletHandlerPool::cleanup() {
    qDebug() << "Cleanup handler pool | size=" << _pool.size();
    _mutex.lock();
    foreach(TabletHandler* handler, _pool) {
        if (!handler->isBusy()) {
            _pool.removeOne(handler);
            delete handler;
            qDebug("ConnectionHandlerPool: Removed connection handler (%p), pool size is now %i",handler,_pool.size());
        }
    }
    _mutex.unlock();
    qDebug() << "Handler pool cleaned up | size=" << _pool.size();
}
