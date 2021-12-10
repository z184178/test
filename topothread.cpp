#include "topothread.h"
#include    <QTime>

topothread::topothread()
{

}

void topothread::run()
{//线程任务
    m_stop=false;//启动线程时令m_stop=false
    m_seq=0; //发topo次数

    while(!m_stop)//循环主体
    {
        if (!m_Paused)
        {
            m_seq++;
            m_topoValue = m_seq % 4;
            emit newValue(m_seq,m_topoValue);  //发射信号
        }
        msleep(10000); //线程休眠10s
    }
}

void topothread::topoBegin()
{//开始发送topo
    m_Paused=false;
}

void topothread::topoPause()
{//暂停发topo
    m_Paused=true;
}

void topothread::stopThread()
{//停止线程
    m_stop=true;
}
