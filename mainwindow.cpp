#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include "topconf.h"
#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QFile>
#include <QTextCodec>
#include <QThread>
#include <QUdpSocket>
#include <QHostAddress>
#include <typeinfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    topconfig_ui = new topconf;
    sceneIndex = 0;

    udpSocketN1 = new QUdpSocket(this);
    udpSocketG1 = new QUdpSocket(this);
    udpSocketG2 = new QUdpSocket(this);
    udpSocketN2 = new QUdpSocket(this);
    udpSocketN3 = new QUdpSocket(this);
    udpSocketN4 = new QUdpSocket(this);
    udpSocket_Remote_master = new QUdpSocket(this);

    port = {portn1,portn2,portn3,portn4,portg1,portg2};
    ctlip<<n1_ctlip<<n2_ctlip<<n3_ctlip<<n4_ctlip<<g1_ctlip<<g2_ctlip;
    name<<"节点一"<<"节点二"<<"节点三"<<"节点四"<<"网关一"<<"网关二";

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s");

    ui->pushButton_start->setEnabled(true);
    ui->pushButton_end->setEnabled(false);
    ui->pushButton_link_end->setEnabled(false);
    ui->pushButton_sim_end->setEnabled(false);
    ui->pushButton_link_start->setEnabled(false);
    ui->pushButton_sim_start->setEnabled(false);

    ui->widget_delay->addGraph();
    ui->widget_delay->graph(0)->setPen(QPen(Qt::red));
    ui->widget_delay->addGraph();
    ui->widget_delay->graph(1)->setPen(QPen(Qt::blue));
    ui->widget_delay->addGraph();
    ui->widget_delay->graph(2)->setPen(QPen(Qt::darkRed));
    ui->widget_delay->addGraph();
    ui->widget_delay->graph(3)->setPen(QPen(Qt::green));

    ui->widget_delay->xAxis->setTicker(timeTicker);
    ui->widget_delay->axisRect()->setupFullAxesBox();

    ui->widget_error->addGraph();
    ui->widget_error->graph(0)->setPen(QPen(Qt::red));
    ui->widget_error->addGraph();
    ui->widget_error->graph(1)->setPen(QPen(Qt::blue));
    ui->widget_error->addGraph();
    ui->widget_error->graph(2)->setPen(QPen(Qt::darkRed));
    ui->widget_error->addGraph();
    ui->widget_error->graph(3)->setPen(QPen(Qt::green));

    ui->widget_error->xAxis->setTicker(timeTicker);
    ui->widget_error->axisRect()->setupFullAxesBox();

    ui->widget_band->addGraph();
    ui->widget_band->graph(0)->setPen(QPen(Qt::red));
    ui->widget_band->addGraph();
    ui->widget_band->graph(1)->setPen(QPen(Qt::blue));
    ui->widget_band->addGraph();
    ui->widget_band->graph(2)->setPen(QPen(Qt::darkRed));
    ui->widget_band->addGraph();
    ui->widget_band->graph(3)->setPen(QPen(Qt::green));

    ui->widget_band->xAxis->setTicker(timeTicker);
    ui->widget_band->axisRect()->setupFullAxesBox();

    //四边安上坐标轴
    ui->widget_delay->axisRect()->setupFullAxesBox();
    ui->widget_error->axisRect()->setupFullAxesBox();
    ui->widget_band->axisRect()->setupFullAxesBox();

    //设置y轴范围
    ui->widget_delay->yAxis->setRange(0, 2000);
    ui->widget_error->yAxis->setRange(-1,100);
    ui->widget_band->yAxis->setRange(-0.5,50);

    //使上下轴、左右轴范围同步
    connect(ui->widget_delay->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget_delay->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->widget_delay->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget_delay->yAxis2, SLOT(setRange(QCPRange)));

    connect(ui->widget_error->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget_error->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->widget_error->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget_error->yAxis2, SLOT(setRange(QCPRange)));

    connect(ui->widget_band->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget_band->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->widget_band->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget_band->yAxis2, SLOT(setRange(QCPRange)));

    //设定右上角图形标注的字体
    ui->widget_delay->legend->setFont(QFont("仿宋",8));
    ui->widget_delay->yAxis->setLabel("Y/ms");
    ui->widget_delay->xAxis->setLabel("X/s");
    //在坐标轴右侧和上方画线，和X/Y轴一起形成一个矩形
    ui->widget_delay->axisRect()->setupFullAxesBox();
    //设定右上角图形标注可见
    ui->widget_delay->legend->setVisible(true);
    //设置右上角图形标注名称
    ui->widget_delay->graph(0)->setName("组网节点一");
    ui->widget_delay->graph(1)->setName("组网节点二");
    ui->widget_delay->graph(2)->setName("组网节点三");
    ui->widget_delay->graph(3)->setName("组网节点四");

    //设定右上角图形标注的字体
    ui->widget_error->legend->setFont(QFont("仿宋",8));
    ui->widget_error->yAxis->setLabel("Y/%");
    ui->widget_error->xAxis->setLabel("X/s");
    //在坐标轴右侧和上方画线，和X/Y轴一起形成一个矩形
    ui->widget_error->axisRect()->setupFullAxesBox();
    //设定右上角图形标注可见
    ui->widget_error->legend->setVisible(true);
    //设置右上角图形标注名称
    ui->widget_error->graph(0)->setName("组网节点一");
    ui->widget_error->graph(1)->setName("组网节点二");
    ui->widget_error->graph(2)->setName("组网节点三");
    ui->widget_error->graph(3)->setName("组网节点四");

    //设定右上角图形标注的字体
    ui->widget_band->legend->setFont(QFont("仿宋",8));
    ui->widget_band->yAxis->setLabel("Y/M");
    ui->widget_band->xAxis->setLabel("X/s");
    //在坐标轴右侧和上方画线，和X/Y轴一起形成一个矩形
    ui->widget_band->axisRect()->setupFullAxesBox();
    //设定右上角图形标注可见
    ui->widget_band->legend->setVisible(true);
    //设置右上角图形标注名称
    ui->widget_band->graph(0)->setName("组网节点一");
    ui->widget_band->graph(1)->setName("组网节点二");
    ui->widget_band->graph(2)->setName("组网节点三");
    ui->widget_band->graph(3)->setName("组网节点四");

    udpSocket = new QUdpSocket(this);

    //ui->label_scene->resize(817,500);
    ui->label_scene->setPixmap(QPixmap(":/img/image/scene1.jpg"));
    ui->label_scene->setScaledContents(true);

    timer_checkBox = new QTimer(this);
    timer_checkBox->start(50);

    connect(timer_checkBox,SIGNAL(timeout()),this,SLOT(checkbox()));
    connect(topconfig_ui,SIGNAL(send_currentIndex(int)),this,SLOT(receive_currentIndex(int)));
    connect(&threadA,SIGNAL(newValue(int,int)),this,SLOT(onthreadA_newValue(int,int)));//线程信号槽

    //connect(timerstkrun,SIGNAL(timeout()),this,SLOT(stkrun()));//控制连线
    //connect(timercheckconfig,SIGNAL(timeout()),this,SLOT(checkconfig()));//发送链路配置信息
    //connect(timer_checkconnect,SIGNAL(timeout()),this,SLOT(checkconnect()));
    //connect(timer_delay,SIGNAL(timeout()),this,SLOT(test_delay()));
    //connect(dataTimer,SIGNAL(timeout()),this,SLOT(realtimeDataSlot()));
    //connect(timerflash,SIGNAL(timeout()),this,SLOT(flash()));

    //timerflash->start(5000);

    //加载拓扑背景图
    ui->label_broad->setPixmap(QPixmap(":/img/image/moon_00.png"));
    ui->label_broad->setScaledContents(true);
    ui->label_ap1->setPixmap(QPixmap(":/img/image/moon_ap1.png"));
    ui->label_ap1->setScaledContents(true);
    ui->label_ap2->setPixmap(QPixmap(":/img/image/moon_ap2.png"));
    ui->label_ap2->setScaledContents(true);

    node_png_path[0] =":/img/image/car1.png";
    node_png_path[1] = ":/img/image/man1.png";
    node_png_path[2] =":/img/image/man2.png";
    node_png_path[3] = ":/img/image/car2.png";




//加载闪电动图
    QSize movieSize2(70,70);
    movied2 = new QMovie(":/img/image/d1.gif");
    movied2->setScaledSize(movieSize2);



    QSize movieSize(30,50);
    movied1 = new QMovie(":/img/image/d2.gif");
    movied1->setScaledSize(movieSize);



    QSize movieSize4(70,70);
    movied4 = new QMovie(":/img/image/d4.gif");
    movied4->setScaledSize(movieSize4);



    QSize movieSize3(30,50);
    movied3 = new QMovie(":/img/image/d3.gif");
    movied3->setScaledSize(movieSize3);


    udpSocket_Remote_master->bind(ip_Remote_master, port_Remote_master);
        connect(udpSocket_Remote_master, &QUdpSocket::readyRead,[=](){
            while(udpSocket_Remote_master->hasPendingDatagrams()) //拥有等待的数据报
            {
                /*if(ui->checkBox_server->isChecked() == true)
                {
                QByteArray buf_Remote_master;//拥于存放接收的数据报
                buf_Remote_master.resize(udpSocket_Remote_master->pendingDatagramSize());
                udpSocket_Remote_master->readDatagram(buf_Remote_master.data(),buf_Remote_master.size());
                qDebug()<<"buf_Remote_master = "<<buf_Remote_master<<endl;
                ui->textEdit_log->append(QString(buf_Remote_master));


                Remote_master_data_0 = QString(buf_Remote_master).section("##",0,0);
                Remote_master_data_1 = QString(buf_Remote_master).section("##",1,1);
                Remote_master_data_2 = QString(buf_Remote_master).section("##",2,2);

                qDebug()<<"Remote_master_data_0 = "<<Remote_master_data_0<<endl;
                qDebug()<<"Remote_master_data_1 = "<<Remote_master_data_1<<endl;
                qDebug()<<"Remote_master_data_2 = "<<Remote_master_data_2<<endl;


                }*/

                QByteArray buf0; //拥于存放接收的数据报
                buf0.resize(udpSocket_Remote_master->pendingDatagramSize());
                udpSocket_Remote_master->readDatagram(buf0.data(),buf0.size());
                QString buf = QString(buf0);
                 ui->textEdit_log->append("Receive Data = "+buf);

                //ui->textEdit_state->setText("received");
                //ui->textEdit_state->insertPlainText(buf);
                QString udpbuf_0;//文件头部标识

                udpbuf_0 = buf.section("*",0,0);

                QString buf_in = buf.section("*",1,1);

                //ui->textEdit_log->append(buf);


                moveGif_clear();


                if (udpbuf_0=="top")//接收拓扑信息
                {
                    for(int k=0;k<4;k++)
                    {

                        node_top[k]=buf_in.section("#",k,k).toInt();
                        switch (node_top[k]) {
                        case 0:
                            ui->textEdit_log->append("参数错误");
                            break;
                        case 1:
                            ui->label_top_1->setPixmap(QPixmap(node_png_path[k]));
                            ui->label_top_1->setScaledContents(true);
                            ui->label_m1->setMovie(movied2);
                            movied2->start();
                            break;
                        case 2:
                            ui->label_top_2->setPixmap(QPixmap(node_png_path[k]));
                            ui->label_top_2->setScaledContents(true);
                            ui->label_m2->setMovie(movied1);
                            movied1->start();
                            break;
                        case 3:
                            ui->label_top_3->setPixmap(QPixmap(node_png_path[k]));
                            ui->label_top_3->setScaledContents(true);
                            ui->label_m3->setMovie(movied3);
                            movied3->start();

                            break;
                        case 4:
                            ui->label_top_4->setPixmap(QPixmap(node_png_path[k]));
                            ui->label_top_4->setScaledContents(true);
                            ui->label_m4->setMovie(movied4);
                            movied4->start();
                            break;
                        case 5:
                            ui->label_top_5->setPixmap(QPixmap(node_png_path[k]));
                            ui->label_top_5->setScaledContents(true);
                            ui->label_m5->setMovie(movied2);
                            movied2->start();
                            break;
                        case 6:
                            ui->label_top_6->setPixmap(QPixmap(node_png_path[k]));
                            ui->label_top_6->setScaledContents(true);
                            ui->label_m6->setMovie(movied1);
                            movied1->start();
                            break;
                        case 7:
                            ui->label_top_7->setPixmap(QPixmap(node_png_path[k]));
                            ui->label_top_7->setScaledContents(true);
                            ui->label_m7->setMovie(movied3);
                            movied3->start();
                            break;
                        case 8:
                            ui->label_top_8->setPixmap(QPixmap(node_png_path[k]));
                            ui->label_top_8->setScaledContents(true);
                            ui->label_m8->setMovie(movied4);
                            movied4->start();
                            break;
                        case 9:
                            ui->label_m9->setMovie(movied1);
                            movied1->start();
                            ui->label_top_9->setPixmap(QPixmap(node_png_path[k]));
                            ui->label_top_9->setScaledContents(true);
                            break;
                        case 10:
                            ui->label_m10->setMovie(movied1);
                            movied1->start();
                            ui->label_top_10->setPixmap(QPixmap(node_png_path[k]));
                            ui->label_top_10->setScaledContents(true);
                            break;
                        case 11:
                            ui->label_m11->setMovie(movied1);
                            movied1->start();
                            ui->label_top_11->setPixmap(QPixmap(node_png_path[k]));
                            ui->label_top_11->setScaledContents(true);
                            break;
                        case 12:
                            ui->label_m12->setMovie(movied1);
                            movied1->start();
                            ui->label_top_12->setPixmap(QPixmap(node_png_path[k]));
                            ui->label_top_12->setScaledContents(true);
                            break;
                        default:
                            ui->textEdit_log->append("参数错误");
                            break;
                        }
                    }
                }
            }
        });




    udpSocketN1->bind(Ctl_ctlip, portn1);
    connect(udpSocketN1, &QUdpSocket::readyRead,this, &MainWindow::readPendingDatagrams_listentoN1);

    udpSocketN2->bind(Ctl_ctlip, portn2);
    connect(udpSocketN2, &QUdpSocket::readyRead,this, &MainWindow::readPendingDatagrams_listentoN2);

    udpSocketN3->bind(Ctl_ctlip, portn3);
    connect(udpSocketN3, &QUdpSocket::readyRead,this, &MainWindow::readPendingDatagrams_listentoN3);

    udpSocketN4->bind(Ctl_ctlip, portn4);
    connect(udpSocketN4, &QUdpSocket::readyRead,this, &MainWindow::readPendingDatagrams_listentoN4);

    udpSocketG1->bind(Ctl_ctlip, portg1);
    connect(udpSocketG1, &QUdpSocket::readyRead,this, &MainWindow::readPendingDatagrams_listentoG1);

    udpSocketG2->bind(Ctl_ctlip, portg2);
    connect(udpSocketG2, &QUdpSocket::readyRead,this, &MainWindow::readPendingDatagrams_listentoG2);


}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_sim_start_clicked()
{
    ui->textEdit_log->setText("仿真开始");
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_end->setEnabled(false);
    ui->pushButton_link_end->setEnabled(false);
    ui->pushButton_sim_end->setEnabled(true);
    ui->pushButton_link_start->setEnabled(true);
    ui->pushButton_sim_start->setEnabled(false);
    readTopoTxt(sceneIndex);
    threadA.topoBegin();


}

void MainWindow::on_pushButton_sim_end_clicked()
{
    threadA.topoPause();

    ui->pushButton_start->setEnabled(false);
    ui->pushButton_end->setEnabled(true);
    ui->pushButton_link_end->setEnabled(false);
    ui->pushButton_sim_end->setEnabled(false);
    ui->pushButton_link_start->setEnabled(false);
    ui->pushButton_sim_start->setEnabled(true);
    ui->textEdit_log->setText("仿真结束");
}

void MainWindow::on_pushButton_top_clicked()
{
    topconfig_ui->show();
}

void MainWindow::on_pushButton_link_start_clicked()
{

    ui->pushButton_start->setEnabled(false);
    ui->pushButton_end->setEnabled(false);
    ui->pushButton_link_end->setEnabled(true);
    ui->pushButton_sim_end->setEnabled(true);
    ui->pushButton_link_start->setEnabled(false);
    ui->pushButton_sim_start->setEnabled(false);
   udpConnect("linktest_start");
   udpConnect("ping*10.0.5.134");
   ui->textEdit_log->append("链路测试开始");
}

void MainWindow::on_pushButton_link_end_clicked()
{
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_end->setEnabled(false);
    ui->pushButton_link_end->setEnabled(false);
    ui->pushButton_sim_end->setEnabled(true);
    ui->pushButton_link_start->setEnabled(true);
    ui->pushButton_sim_start->setEnabled(false);
    ui->textEdit_log->append("链路测试结束");
}

void MainWindow::readPendingDatagrams_listentomainctl()
{
   //qDebug()<<"Received Data ";
   QByteArray buf0; //拥于存放接收的数据报
   buf0.resize(udpSocket->pendingDatagramSize());
   udpSocket->readDatagram(buf0.data(),buf0.size());

  /* QString buf = QString(buf0);
   qDebug()<<"buf = "<<buf;
   ui->textEdit_log->setText(buf);
*/

}


void MainWindow::receive_currentIndex(int index)
{
    switch (index) {
    case 0:
        ui->label_scene->setPixmap(QPixmap(":/img/image/scene1.jpg"));
        break;
    case 1:
        ui->label_scene->setPixmap(QPixmap(":/img/image/scene2.jpg"));
        break;
    case 2:
        ui->label_scene->setPixmap(QPixmap(":/img/image/scene3.jpg"));
        break;
    case 3:
        ui->label_scene->setPixmap(QPixmap(":/img/image/scene4.jpg"));
        break;
    }
    ui->label_scene->setScaledContents(true);
    //sceneIndex = QString::number(index);
    sceneIndex = index;
    //qDebug()<<sceneIndex;
}

void MainWindow::readTopoTxt(int sceneIndex)
{

    switch (sceneIndex) {
    case 0:path = ":/img/image/scene1.txt";break;
    case 1:path = ":/img/image/scene2.txt";break;
    case 2:path = ":/img/image/scene3.txt";break;
    case 3:path = ":/img/image/scene4.txt";break;
    default:
        break;
    }

    qDebug()<<path;
    QFile file(path);


    bool isOk = file.open(QIODevice::ReadOnly);
    if (false == isOk)
    {
      qDebug()<<"只读方式打开文件失败 78";
      ui->textEdit_log->append("文件打开失败");
    }

    //提示打开文件路径
    //ui->textEdit_log->append(filePath);

    line = file.readLine();
    T1 = QString(line).section(" ",0,8).section("\n",0,0).section("\r",0,0);
    qDebug()<<"T1:"<<T1;
    line = file.readLine();
    T2 = QString(line).section(" ",0,8).section("\n",0,0).section("\r",0,0);
    qDebug()<<"T2:"<<T2;
    line = file.readLine();
    T3 = QString(line).section(" ",0,8).section("\n",0,0).section("\r",0,0);
    qDebug()<<"T3:"<<T3;
    line = file.readLine();
    T4 = QString(line).section(" ",0,8).section("\n",0,0).section("\r",0,0);
    qDebug()<<"T4:"<<T4;

    file.close();

    topoTxt<<T1<<T2<<T3<<T4;
}



void MainWindow::udpConnect(QString str)
{
    udpSocketG1->writeDatagram(str.toUtf8(),G1_ctlip,portg1);
    udpSocketG2->writeDatagram(str.toUtf8(),G2_ctlip,portg2);
    udpSocketN1->writeDatagram(str.toUtf8(),N1_ctlip,portn1);
    udpSocketN2->writeDatagram(str.toUtf8(),N2_ctlip,portn2);
    udpSocketN3->writeDatagram(str.toUtf8(),N3_ctlip,portn3);
    udpSocketN4->writeDatagram(str.toUtf8(),N4_ctlip,portn4);
    udpSocket_Remote_master->writeDatagram(str.toUtf8(),ip_Remote_master,port_Remote_master);
}

void MainWindow::moveGif_clear()
{
    ui->label_top_1->clear();
    ui->label_top_2->clear();
    ui->label_top_3->clear();
    ui->label_top_4->clear();
    ui->label_top_5->clear();
    ui->label_top_6->clear();
    ui->label_top_7->clear();
    ui->label_top_8->clear();
    ui->label_top_9->clear();
    ui->label_top_10->clear();
    ui->label_top_11->clear();
    ui->label_top_12->clear();
    ui->label_m1->clear();
    ui->label_m2->clear();
    ui->label_m3->clear();
    ui->label_m4->clear();
    ui->label_m5->clear();
    ui->label_m6->clear();
    ui->label_m7->clear();
    ui->label_m8->clear();
    ui->label_m9->clear();
    ui->label_m10->clear();
    ui->label_m11->clear();
    ui->label_m12->clear();
}

void MainWindow::checkconnect(int send, int receive)
{
    QString pingdata = "ping*"+QString::number(receive);
    qDebug()<<pingdata;
    switch (send) {
    case 0:
        udpSocketN1->writeDatagram(pingdata.toUtf8(),QHostAddress(ctlip.at(send)),port[send]);
        break;
    case 1:
        udpSocketN2->writeDatagram(pingdata.toUtf8(),QHostAddress(ctlip.at(send)),port[send]);
        break;
    case 2:
        udpSocketN3->writeDatagram(pingdata.toUtf8(),QHostAddress(ctlip.at(send)),port[send]);
        break;
    case 3:
        udpSocketN4->writeDatagram(pingdata.toUtf8(),QHostAddress(ctlip.at(send)),port[send]);
        break;
    case 4:
        udpSocketG1->writeDatagram(pingdata.toUtf8(),QHostAddress(ctlip.at(send)),port[send]);
        break;
    case 5:
        udpSocketG2->writeDatagram(pingdata.toUtf8(),QHostAddress(ctlip.at(send)),port[send]);
        break;
    default:
        qDebug()<<"测ping异常";
        break;
    }
    ui->plainTextEdit_pingInfo->appendPlainText(name.at(send)+"ping"+name.at(receive));
    qDebug()<<"测ping结束";
}

void MainWindow::setupRealtimeDataDemo()
{

}

void MainWindow::Test_link()
{

}

void MainWindow::checkbox()//接入不可多选
{
    ui->checkBox_local->setEnabled(true);
    ui->checkBox_server->setEnabled(true);
    ui->checkBox_test->setEnabled(true);

    if(ui->checkBox_local->isChecked()==true)
      {
        ui->checkBox_server->setEnabled(false);
        ui->checkBox_test->setEnabled(false);
        //type = 1;
      }
    if(ui->checkBox_server->isChecked()==true)
      {
        ui->checkBox_local->setEnabled(false);
        ui->checkBox_test->setEnabled(false);
        //type = 2;
      }
    if(ui->checkBox_test->isChecked()==true)
      {
        ui->checkBox_local->setEnabled(false);
        ui->checkBox_server->setEnabled(false);
        //type = 0;
      }


}

void MainWindow::on_pushButton_readserver_clicked()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //窗口关闭事件，必须结束线程
        if (threadA.isRunning())
        {
            threadA.stopThread();
            threadA.wait();
        }
        event->accept();
}

void MainWindow::onthreadA_started()
{//线程的started()信号的响应槽函数
    //ui->label->setText("Thread状态：thread started");
}

void MainWindow::onthreadA_finished()
{//线程的 finished()信号的响应槽函数
    //ui->label->setText("Thread状态：thread finished");
}

void MainWindow::onthreadA_newValue(int seq, int topoValue)
{   // QString  str =QString::asprintf("第 %d 次发topo",seq);
     //ui->textEdit_log->append(str);

      switch (topoValue) {
      case 1:
          udpConnect(T1);
          qDebug()<<"发送T1时刻拓扑"<<T1;
          break;
      case 2:
          udpConnect(T2);
          qDebug()<<"发送T2时刻拓扑"<<T2;
          break;
      case 3:
          udpConnect(T3);
          qDebug()<<"发送T3时刻拓扑"<<T3;
          break;
      case 0:
          udpConnect(T4);
          qDebug()<<"发送T4时刻拓扑"<<T4;
          break;
      default:
          break;
      }
}


void MainWindow::readPendingDatagrams_listentoG1()
{
    QByteArray bufG1; //拥于存放接收的数据报
    bufG1.resize(udpSocketG1->pendingDatagramSize());
    udpSocketG1->readDatagram(bufG1.data(),bufG1.size());
    QString udpbuf_0;//文件头部标识
    udpbuf_0 = QString(bufG1).section("##",0,0);
    if (QString(bufG1)=="网关一已连接")
     {
      //ui->lineEdit_access_g1->setText("网关一已连接");
     }
    //qDebug()<<QString(bufG1);
    //ui->textEdit_log->append("G1:"+QString(bufG1));
    if(ui->comboBox_sendPing->currentIndex() == 4){
        ui->plainTextEdit_pingInfo->appendPlainText("G1:"+QString(bufG1));
    }
}

void MainWindow::readPendingDatagrams_listentoG2()
{
    QByteArray bufG2; //拥于存放接收的数据报
    //qDebug()<<"bufG2 = "<<bufG2;
    bufG2.resize(udpSocketG2->pendingDatagramSize());
    udpSocketG2->readDatagram(bufG2.data(),bufG2.size());
    QString udpbuf_0;//文件头部标识
    udpbuf_0 = QString(bufG2).section("##",0,0);
    if (QString(bufG2)=="网关二已连接")
    {
      //ui->lineEdit_access_g2->setText("网关二已连接");
    }
    //qDebug()<<QString(bufG2);
    //ui->textEdit_log->append("G2:"+QString(bufG2));
    if(ui->comboBox_sendPing->currentIndex() == 5){
        ui->plainTextEdit_pingInfo->appendPlainText("G2:"+QString(bufG2));
    }
}

void MainWindow::readPendingDatagrams_listentoN1()
{
    QByteArray bufN1; //拥于存放接收的数据报
    bufN1.resize(udpSocketN1->pendingDatagramSize());
    udpSocketN1->readDatagram(bufN1.data(),bufN1.size());
    //qDebug()<<QString(bufN1);
    ui->textEdit_log->append("N1:"+QString(bufN1));
    if(ui->comboBox_sendPing->currentIndex() == 0){
        ui->plainTextEdit_pingInfo->appendPlainText("N1:"+QString(bufN1));
    }
}

void MainWindow::readPendingDatagrams_listentoN2()
{
    QByteArray bufN2; //拥于存放接收的数据报
    bufN2.resize(udpSocketN2->pendingDatagramSize());
    udpSocketN2->readDatagram(bufN2.data(),bufN2.size());
    //qDebug()<<QString(bufN2);
    //ui->textEdit_log->append("N2:"+QString(bufN2));
    if(ui->comboBox_sendPing->currentIndex() == 1){
        ui->plainTextEdit_pingInfo->appendPlainText("N2:"+QString(bufN2));
    }
}

void MainWindow::readPendingDatagrams_listentoN3()
{
    QByteArray bufN3; //拥于存放接收的数据报
    bufN3.resize(udpSocketN3->pendingDatagramSize());
    udpSocketN3->readDatagram(bufN3.data(),bufN3.size());
    //qDebug()<<QString(bufN3);
    //ui->textEdit_log->append("N3:"+QString(bufN3));
    if(ui->comboBox_sendPing->currentIndex() == 2){
        ui->plainTextEdit_pingInfo->appendPlainText("N3:"+QString(bufN3));
    }
}

void MainWindow::readPendingDatagrams_listentoN4()
{
    QByteArray bufN4; //拥于存放接收的数据报
    bufN4.resize(udpSocketN4->pendingDatagramSize());
    udpSocketN4->readDatagram(bufN4.data(),bufN4.size());
    //qDebug()<<QString(bufN4);
    //ui->textEdit_log->append("N4:"+QString(bufN4));
    if(ui->comboBox_sendPing->currentIndex() == 3){
        ui->plainTextEdit_pingInfo->appendPlainText("N4:"+QString(bufN4));
    }
}


void MainWindow::on_pushButton_start_clicked()
{
    threadA.start();//开启线程
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_end->setEnabled(false);
    ui->pushButton_link_end->setEnabled(false);
    ui->pushButton_sim_end->setEnabled(false);
    ui->pushButton_link_start->setEnabled(false);
    ui->pushButton_sim_start->setEnabled(true);
    ui->textEdit_log->append("系统启动");
}

void MainWindow::on_pushButton_end_clicked()
{
    threadA.stopThread();//结束线程的run()函数执行
    threadA.wait();
    moveGif_clear();

    ui->pushButton_start->setEnabled(true);
    ui->pushButton_end->setEnabled(false);
    ui->pushButton_link_end->setEnabled(false);
    ui->pushButton_sim_end->setEnabled(false);
    ui->pushButton_link_start->setEnabled(false);
    ui->pushButton_sim_start->setEnabled(false);
    ui->textEdit_log->append("系统关闭");
}

void MainWindow::on_textEdit_log_textChanged()
{
    ui->textEdit_log->moveCursor(QTextCursor::End);
}

void MainWindow::on_pushButton_pingTest_clicked()
{
   checkconnect(ui->comboBox_sendPing->currentIndex(),ui->comboBox_receivePing->currentIndex());
   qDebug()<<"开始测ping";
}
