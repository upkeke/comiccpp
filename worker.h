#ifndef WORKER_H
#define WORKER_H
#include<QDebug>
#include <QObject>
#include<QThread>
class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    void dowork(int x);

signals:
    void resultToYou(int x);


};

#endif // WORKER_H
