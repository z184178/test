#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include "topconf.h"
#include <QTimer>
#include <QTime>
#include <QProcess>
#include <QFile>
#include <QTextCodec>
#include <QThread>
#include <QHostAddress>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    topconf *topconfig_ui;



    ~MainWindow();
signals:



private slots:
    void on_pushButton_sim_start_clicked();

    void on_pushButton_top_clicked();

    void on_pushButton_sim_end_clicked();

    void on_pushButton_link_start_clicked();

    void on_pushButton_link_end_clicked();

    void readPendingDatagrams_listentomainctl();

    void receive_currentIndex(int index);//接收当前场景
    void readTopoTxt(int sceneIndex);//读拓扑文件
    void udpConnect(QString str);//udp发信息
    void moveGif_clear();//清除动态贴图
    void checkconnect(int send,int receive);//检查节点连接

    void send_topo();//发送指定topo


    void setupRealtimeDataDemo();//触发折线图开始显示
    void Test_link();//节点链路参数测试
   // void Stop_run();//结束仿真场景的运行
   // void Test_linkend();//节点链路参数测试结束
    void realtimeDataSlot();//折线图动态数据显示

   // void stkrun();//STK场景显示动态变化
   // void flash();
   // void checkconfig();//界面链路参数配置逻辑控制
    //void checkconnect();//界面接入显示逻辑控制
    void checkbox();//界面勾选框逻辑控制

    void readPendingDatagrams_listentoG1();
    void readPendingDatagrams_listentoG2();
    void readPendingDatagrams_listentoN1();
    void readPendingDatagrams_listentoN2();
    void readPendingDatagrams_listentoN3();
    void readPendingDatagrams_listentoN4();

   // void test_delay();
  //  void tcpConnect();

    void on_pushButton_readserver_clicked();

    void on_pushButton_start_clicked();

    void on_pushButton_end_clicked();

    void on_textEdit_log_textChanged();

    void on_pushButton_pingTest_clicked();

    void every_second();//每秒执行一次的功能

    QVector<int> num_to_vector(int num);//数字转换为数组

    QVector<int> two_vector_bu(QVector<int> a,QVector<int> b);//求a数组除a、b交集及4、5之外的元素组成数组

    void multi_hop_vector(int num,QString str);//存储多跳节点

    void checkconfig();//

    int getRandom(int min,int max);//生成随机数

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;

    QUdpSocket *udpSocketG1;
    QUdpSocket *udpSocketG2;
    QUdpSocket *udpSocketN1;
    QUdpSocket *udpSocketN2;
    QUdpSocket *udpSocketN3;
    QUdpSocket *udpSocketN4;
    QUdpSocket *udpSocket_Remote_master;


    QFile file;//文件对象
    QByteArray line;



    QProcess *openpython;

    int workCount;
    int sceneIndex;//场景模式
    int sendPing;
    int receivePing;
    int count=1;
    int count_j=0;
    int topoVlaue;
    QString path;//场景拓扑路径

    //不同时刻拓扑
    QString T1;
    QString T2;
    QString T3;
    QString T4;
    QList<QString> topoTxt;

    //名称定义
    QList<QString> name;

    //动态拓扑相关定义
    int node_top[6];
    QVector<int> second;
    QVector<int> second_1;
    QVector<int> second_2;
    QVector<int> third;
    QVector<int> third_1;
    QVector<int> third_2;

    //QString ip_network ;
    QString node_png_path[4];
    QPixmap *board;
    QPixmap *node_png;
    QMovie *movie1;
    QMovie *movied1;
    QMovie *movied2;
    QMovie *movied3;
    QMovie *movied4;
    QMovie *moviedap;


    QTimer *timercheckconfig;//检测链路状态定时器
    QTimer *timer_checkBox;//检测选项状态定时器
    QTimer *timer_delay;//检测时延误码定时器
    QTimer *timer_checkconnect;//检查连接定时器
    QTimer *dataTimer;//数据接收定时器
    QTimer *timer_serverReader;//读取服务器信息定时器
    QTimer *timer_count;//计数器，1秒1次
    QTimer *timer_send_topo;//发送topo计时器


    //udp定义
    qint16 port_Remote_master = 9001;//远程主监听端口
    qint16 portn1 = 7001;//主控下发命令
    qint16 portn2 = 7002;//主控下发命令
    qint16 portn3 = 7003;//主控下发命令
    qint16 portn4 = 7004;//主控下发命令
    qint16 portg1 = 7005;//主控下发命令
    qint16 portg2 = 7006;//主控下发命令
    QVector<qint16> port;
    QString const n1_ctlip = "10.0.7.1";
    QString const n2_ctlip = "10.0.7.2";
    QString const n3_ctlip = "10.0.7.3";
    QString const n4_ctlip = "10.0.7.4";
    QString const g1_ctlip = "10.0.7.5";
    QString const g2_ctlip = "10.0.7.6";
    QString const ctl_ctlip = "10.0.7.7";
    QList<QHostAddress> ctlip;



    QString const ip_control = "127.0.0.1";  //"192.168.50.200";//主控IP
    QString Remote_master_data_0;
    QString Remote_master_data_1;
    QString Remote_master_data_2;

    QHostAddress ip_Remote_master = QHostAddress(ip_control);
    QHostAddress Ctl_ctlip = QHostAddress(ctl_ctlip);
    QHostAddress N1_ctlip = QHostAddress(n1_ctlip);
    QHostAddress N2_ctlip = QHostAddress(n2_ctlip);
    QHostAddress N3_ctlip = QHostAddress(n3_ctlip);
    QHostAddress N4_ctlip = QHostAddress(n4_ctlip);
    QHostAddress G1_ctlip = QHostAddress(g1_ctlip);
    QHostAddress G2_ctlip = QHostAddress(g2_ctlip);

    double delay1[10]={0};
    double error1[10]={0};
    double bandwidth1[10]={0};

    double delay2[10]={0};
    double error2[10]={0};
    double bandwidth2[10]={0};

    double delay3[10]={0};
    double error3[10]={0};
    double bandwidth3[10]={0};

    double delay4[10]={0};
    double error4[10]={0};
    double bandwidth4[10]={0};

};

#endif // MAINWINDOW_H
