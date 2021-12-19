#include "resultdialog.h"
#include "ui_resultdialog.h"
#include <mainwindow.h>
#include <QDebug>

ResultDialog::ResultDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultDialog)
{
    ui->setupUi(this);
    ui->startlabel->setText(startCity);
    ui->endlabel->setText(endCity);
    ui->methodlabel->setText(method + "方法是：");
    ui->timelabel->setText(totaltime);
    ui->roadlabel->setText(road);
    ui->pricelabel->setText(price);
    ui->changetimeslabel->setText(changetimes);
    ui->distancelabel->setText(distance);
}

ResultDialog::~ResultDialog()
{
    delete ui;
}

void ResultDialog::on_pushButton_clicked()
{
    close();
}
