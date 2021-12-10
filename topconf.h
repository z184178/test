#ifndef TOPCONF_H
#define TOPCONF_H

#include <QTextCodec>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <qthread.h>
#include <qdebug.h>
#include <QPen>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include <math.h>
#include <qmath.h>
#include <QFile>
#include <QWidget>



namespace Ui {
class topconf;
}

class topconf : public QDialog
{
    Q_OBJECT

public:
    explicit topconf(QWidget *parent = nullptr);

    ~topconf();

signals:
    void send_currentIndex(int index);

private slots:

   // void on_pushButton_loginConfig_clicked();

    bool is_str_utf8(const char* str);
    void on_comboBox_scene_currentIndexChanged(int index);


private:
    Ui::topconf *ui;

    QFile file;
    qint64 fileSize;
    QString fileName;

    QTimer *timer_sceneChange;

    QByteArray line;

    QFile file_save;

    QString wenzi;

    QString N1;
    QString N2;
    QString N3;
    QString N4;
    QString G1;
    QString G2;



};

#endif // TOPCONF_H
