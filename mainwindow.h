#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

struct ArcNode //边节点
{
    int adjvex;       //该边指向的顶点位置
    ArcNode *nextarc; //指向下一条边的指针
    int length;       //长度
    int time;         //时间(单位：分钟）
    int price;        //价格
};

typedef struct VNode //顶点
{
    QString cityname;
    ArcNode *firstarc; //指向第一条依附于该顶点的边
} AdjList[7];

struct ALGraph //邻接表
{
    AdjList vertices;   //顶点
    int vexnum, arcnum; //当前顶点数边数
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void CreateUDG(ALGraph &G, QString verticesData[], int arcData[][5]);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H