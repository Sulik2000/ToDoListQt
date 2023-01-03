#ifndef ADDTASKWINDOW_H
#define ADDTASKWINDOW_H

#include <QDialog>
#include <QDate>
#include <QString>

namespace Ui {
class AddTaskWindow;
}

class AddTaskWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddTaskWindow(QWidget *parent = nullptr);
    ~AddTaskWindow();
    void SetDataTask(QString name, QDate date, int index);

private:
    Ui::AddTaskWindow *ui;
    int indexOfTask = -1;

signals:
    void createTask(QString name, QDate date);
    void editTask(QString name, QDate date, int index);
private slots:
    void on_buttonBox_accepted();
};

#endif // ADDTASKWINDOW_H
