#include "topconf.h"
#include "ui_topconf.h"
#include <QDebug>
#include<QTimer>
#include<QFile>


topconf::topconf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::topconf)
{
    ui->setupUi(this);
    setWindowTitle("拓扑关系配置界面");

}


topconf::~topconf()
{
    delete ui;
}

bool topconf::is_str_utf8(const char *str)
{
    unsigned int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
    unsigned char chr = *str;
    bool bAllAscii = true;
    for (unsigned int i = 0; str[i] != '\0'; ++i)
    {
        chr = *(str + i);
        //判断是否ASCII编码,如果不是,说明有可能是UTF8,ASCII用7位编码,最高位标记为0,0xxxxxxx
        if (nBytes == 0 && (chr & 0x80) != 0)
        {
            bAllAscii = false;
        }

        if (nBytes == 0)
        {
            //如果不是ASCII码,应该是多字节符,计算字节数
            if (chr >= 0x80)
            {

                if (chr >= 0xFC && chr <= 0xFD)
                {
                    nBytes = 6;
                }
                else if (chr >= 0xF8)
                {
                    nBytes = 5;
                }
                else if (chr >= 0xF0)
                {
                    nBytes = 4;
                }
                else if (chr >= 0xE0)
                {
                    nBytes = 3;
                }
                else if (chr >= 0xC0)
                {
                    nBytes = 2;
                }
                else
                {
                    return false;
                }

                nBytes--;
            }
        }
        else{
            //多字节符的非首字节,应为 10xxxxxx
            if ((chr & 0xC0) != 0x80){
                return false;
            }
            //减到为零为止
            nBytes--;
        }
    }
    //违返UTF8编码规则
    if (nBytes != 0)
    {
        return false;
    }
    //如果全部都是ASCII, 也是UTF8
    if (bAllAscii)
    {
        return true;
    }
    return true;
}




void topconf::on_comboBox_scene_currentIndexChanged(int index)
{
    emit send_currentIndex(ui->comboBox_scene->currentIndex());
}
