#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "resultdialog.h"
#include <QDebug>

QString startCity;
QString endCity;
QString method;
QString road;
QString totaltime;
QString price;
QString changetimes;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->requireBox->clear();
    ui->requireBox->addItem("最短里程");
    ui->requireBox->addItem("最低花费");
    ui->requireBox->addItem("最短时间");
    ui->requireBox->addItem("最少中转次数");
    QString verticesData[] = {"北京", "西安", "郑州", "徐州", "成都", "广州", "上海"};
    int arcData[][5] = {{0, 1, 2553, 480, 885}, {0, 3, 704, 150, 225}, {0, 2, 695, 138, 202}, {1, 2, 511, 90, 148},
        {2, 3, 349, 72, 112}, {1, 4, 812, 180, 283}, {2, 5, 1579, 300, 495}, {3, 6, 651, 120, 162},
        {4, 5, 2368, 420, 684}, {5, 6, 1385, 240, 386}
    };
    G = new ALGraph;
    CreateUDG(verticesData, arcData);
    for (size_t i = 0; i < G->vexnum; i++)//将城市名填入下拉框中
    {
        ui->startBox->addItem(G->vertices[i].cityname);
        ui->endBox->addItem(G->vertices[i].cityname);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateUDG(QString verticesData[], int arcData[][5])//初始化图
{
    G->vexnum = 7;
    G->arcnum = 10;
    for (size_t i = 0; i < G->vexnum; i++)
    {
        G->vertices[i].cityname = verticesData[i];
        G->vertices[i].firstarc = nullptr; //初始化表头结点指针域为nullptr
    }
    int v1, v2, i, j;
    ArcNode *p1, *p2;
    for (size_t k = 0; k < G->arcnum; k++)
    {
        v1 = arcData[k][0];
        v2 = arcData[k][1];
        i = v1;
        j = v2; //确定v1和v2的位置
        p1 = new ArcNode;
        p1->adjvex = j;
        p1->length = arcData[k][2];
        p1->time = arcData[k][3];
        p1->time = arcData[k][4];
        p1->nextarc = G->vertices[i].firstarc;
        G->vertices[i].firstarc = p1; //将新节点*p1插入到vi的边表头
        p2 = new ArcNode;
        p2->adjvex = i;
        p2->length = arcData[k][2];
        p2->time = arcData[k][3];
        p2->time = arcData[k][4];
        p2->nextarc = G->vertices[j].firstarc;
        G->vertices[j].firstarc = p2; //将新节点*p2插入到vj的边表头
    }
}

int MainWindow::CitySerial(QString cityname)//名字转序号
{
    for (size_t i = 0; i < G->vexnum; i++)
    {
        if(G->vertices[i].cityname == cityname)
            return i;
    }
    return -1;
}

void MainWindow::ShortestPath_Floyd(int start_serial, int end_serial)
{

}

void MainWindow::on_pushButton_clicked()
{
    startCity = ui->startBox->currentText();
    endCity = ui->endBox->currentText();
    method = ui->requireBox->currentText();
    ResultDialog r;
    r.exec();
}
