#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>

#include "datamodel.h"
#include "addtaskwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void EditTask();
    void on_AddNewTask_triggered();
    void DeleteTask();
    void ChangeList(int index);


private:
    Ui::MainWindow *ui;
    DataModel *model;
    AddTaskWindow *TaskWindow;
    QTableView *tableView;
};
#endif // MAINWINDOW_H
