#include "addtaskwindow.h"
#include "ui_addtaskwindow.h"

AddTaskWindow::AddTaskWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTaskWindow)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
}

AddTaskWindow::~AddTaskWindow()
{
    delete ui;
}

void AddTaskWindow::SetDataTask(QString name, QDate date, int index)
{
    ui->lineEdit->setText(name);
    ui->dateEdit->setDate(date);
    this->indexOfTask = index;
    ui->label->setText("Edit your task");
}

void AddTaskWindow::on_buttonBox_accepted()
{
    if(indexOfTask == -1)
        emit createTask(ui->lineEdit->text(), ui->dateEdit->date());
    else
        emit editTask(ui->lineEdit->text(), ui->dateEdit->date(), this->indexOfTask);
}

