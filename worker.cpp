#include "worker.h"
#pragma execution_character_set("utf-8")
Worker::Worker(QObject *parent)
    : QObject{parent}
{

}

void Worker::dowork(int x)
{
    qDebug()<<"进入dowork";
    for(int i=0;i<5000000;++i)
        if(i%1000000==0)
        {
            qDebug()<<__FUNCTION__<<"i = "<<i
                   <<"线程id "<<(int)QThread::currentThreadId();
        }
    qDebug()<<__FUNCTION__<<"结束 "<<x;
    //emit resultToYou(x);
}
