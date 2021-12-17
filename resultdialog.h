#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>

namespace Ui
{
class ResultDialog;
}

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent = nullptr);
    ~ResultDialog();
private slots:
    void on_pushButton_clicked();

private:
    Ui::ResultDialog *ui;
};

#endif // RESULTDIALOG_H
