#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>

class Thread : public QThread
{
    using QThread::run;
public:
    explicit Thread(QObject *parent = 0);
    ~Thread();
};

#endif // WORKERTHREAD_H
