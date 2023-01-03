#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new DataModel(this);
    ui->tabWidget->addTab(new QTableView(), "All");
    ui->tabWidget->addTab(new QTableView(), "Complete");
    ui->tabWidget->addTab(new QTableView(), "Failed");
    ui->tabWidget->addTab(new QTableView(), "In process");
    this->tableView = (QTableView*)ui->tabWidget->currentWidget();
    tableView->setModel(model);
    tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::EditTask()
{
    TaskWindow = new AddTaskWindow(this);
    // Parse data from model
    QModelIndex index = tableView->currentIndex();
    index = index.siblingAtColumn(0);
    QString name = model->data(index, Qt::DisplayRole).toString();
    index = index.siblingAtColumn(1);
    QDate date = model->data(index, Qt::DisplayRole).toDate();

    TaskWindow->SetDataTask(name, date, index.row());

    connect(TaskWindow, SIGNAL(editTask(QString,QDate,int)), model, SLOT(EditTask(QString,QDate,int)));
    TaskWindow->show();
}


void MainWindow::on_AddNewTask_triggered()
{
    TaskWindow = new AddTaskWindow(this);
    connect(TaskWindow, SIGNAL(createTask(QString,QDate)), model, SLOT(AddNewTask(QString,QDate)));
    TaskWindow->activateWindow();
    TaskWindow->show();
}

void MainWindow::DeleteTask()
{
    QModelIndex index = tableView->currentIndex();
    index.siblingAtColumn(0);
    QString name = model->data(index, Qt::DisplayRole).toString();
    this->model->DeleteTask(name, index.row());
}

void MainWindow::ChangeList(int index)
{

    this->tableView = (QTableView*)ui->tabWidget->currentWidget();

    tableView->setModel(model);

    FTaskStatus status;

    if(index == 0)
        status = FTaskStatus::All;

    else if(index == 1)
        status = FTaskStatus::Complete;

    else if(index == 2)
        status = FTaskStatus::Failed;

    else if(index == 3)
        status = FTaskStatus::Process;

    this->model->ChangeStatus(status);

}

