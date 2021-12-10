#ifndef TOPOTHREAD_H
#define TOPOTHREAD_H

#include <QThread>

class topothread : public QThread
{
    Q_OBJECT

private:
    int     m_seq=0;//发topo次数
    int     m_topoValue;//topo时刻数
    bool    m_Paused=true; //发一次topo
    bool    m_stop=false; //停止线程

protected:
    void    run() Q_DECL_OVERRIDE;  //线程任务

public:
    topothread();

    void    topoBegin();//发一次topo
    void    topoPause();//暂停
    void    stopThread(); //结束线程

signals:
    void    newValue(int seq,int topoValue); //产生新topo时刻的信号

};

#endif // TOPOTHREAD_H
