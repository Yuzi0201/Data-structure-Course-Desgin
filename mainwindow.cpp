#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "resultdialog.h"
#include <QDebug>
#include <QStack>
#include <QVector>
#include <QQueue>
#include <QMessageBox>

QString startCity;
QString endCity;
QString method;
QString road;
QString totaltime;
QString price;
QString changetimes;
QString distance;

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
        p1->price = arcData[k][4];
        p1->nextarc = G->vertices[i].firstarc;
        G->vertices[i].firstarc = p1; //将新节点*p1插入到vi的边表头
        p2 = new ArcNode;
        p2->adjvex = i;
        p2->length = arcData[k][2];
        p2->time = arcData[k][3];
        p2->price = arcData[k][4];
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

ArcNode* MainWindow::NextVex( int v, int w)
{
    ArcNode *p = G->vertices[v].firstarc;
    while (p->adjvex != w)
    {
        p = p->nextarc;
        if (!p->nextarc)
            return NULL; //下一条边是空的了
    }
    return p->nextarc; //此时p就是w的节点，返回下一边
}

int MainWindow::Search_weight(int v1, int v2, QString type) //查找两点之间的边的权重，若无边返回最大值114514
{
    for (ArcNode *v = G->vertices[v1].firstarc; v; v = NextVex(v1, v->adjvex))
    {
        if (v->adjvex == v2)
        {
            if(type == "最短里程")
                return v->length;
            if(type == "最低花费")
                return v->price;
            if(type == "最短时间")
                return v->time;
        }
    }
    return 114514;
}

int NextAdjVex(ALGraph *G, int v, int w)
{
    ArcNode *p = G->vertices[v].firstarc;
    while (p->adjvex != w)
        p = p->nextarc;
    if (!p->nextarc)
        return -1;             //下一条边是空的了
    return p->nextarc->adjvex; //此时p就是w的节点，返回下一个的序号
}

void MainWindow::ShortestPath_Floyd(int start_serial, int end_serial, QString method)
{
    int n = G->vexnum;
    int Path[n][n];//最短路径上顶点vj前一顶点的序号
    int D[n][n];//记录vi和vj之间的最短路径长度
    if(method != "最少中转次数")
    {
        for(int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                D[i][j] = Search_weight(i, j, method);
                if(D[i][j] < 114514)
                    Path[i][j] = i; //若i与j之间有弧，则将j前驱置为i
                else
                    Path[i][j] = -1; //若无弧，则将j前驱置为-1
            }
        for (int k = 0; k < n; k++)
            for(int i = 0; i < n; i++)
                for(int j = 0; j < n; j++)
                    if(D[i][k] + D[k][j] < D[i][j]) //如果i经过k到j更短
                    {
                        D[i][j] = D[i][k] + D[k][j]; //更新D[i][j]
                        Path[i][j] = Path[k][j]; //更改j前驱为k
                    }
    }
    else//求最小中转次数
    {
        //Breadth First Search
        bool visited[7] = {false};
        visited[start_serial] = true; //第v个（也就是第0个节点已访问）
        QQueue<int> q;
        q.push_back(start_serial);
        while (!q.empty())
        {
            int u = q.front();
            q.pop_front();
            for (int w = G->vertices[u].firstarc->adjvex; w >= 0; w = NextAdjVex(G, u, w))
                if (!visited[w])
                {
                    Path[start_serial][w] = u; //w前驱置为u
                    visited[w] = true;
                    q.push_back(w);
                }
        }
    }
    QStack<int> roadstack;//创建栈，把顺序调换
    int timeint = 0;
    int priceint = 0;
    int distanceint = 0;
    int changetimesint = -1;
    for (int a = end_serial; a != start_serial; a = Path[start_serial][a])//遍历最短路径
    {
        roadstack.push(a);//入栈
        timeint += Search_weight(a, Path[start_serial][a], "最短时间");
        priceint += Search_weight(a, Path[start_serial][a], "最低花费");
        distanceint += Search_weight(a, Path[start_serial][a], "最短里程");
        changetimesint++;
    }
    totaltime = QString::number(timeint) + "分钟";
    price = QString::number(priceint) + "元";
    distance = QString::number(distanceint) + "公里";
    changetimes = QString::number(changetimesint) + "次";
    roadstack.push(start_serial);
    QVector<int> roadvector;//使用vector存储路径
    while (!roadstack.empty())
    {
        roadvector.push_back(roadstack.pop());//将栈顶数出栈并放入vector末尾
    }
    road.clear();
    for (QStack<int>::iterator i = roadvector.begin(); i != roadvector.end(); i++)
    {
        road = road + "->" + G->vertices[*i].cityname;
    }
    road.remove(0, 2);//删去最开始的箭头
}

void MainWindow::on_pushButton_clicked()
{
    startCity = ui->startBox->currentText();
    endCity = ui->endBox->currentText();
    method = ui->requireBox->currentText();
    if(startCity != endCity)
    {
        ShortestPath_Floyd(CitySerial(startCity), CitySerial(endCity), method);
        ResultDialog r;
        r.exec();
    }
    else
        QMessageBox::critical(NULL, "错误", "始发终到站不能相同！");
}
