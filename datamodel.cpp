#include "datamodel.h"

QList<FTaskStruct> DataModel::ParseTasks(const FTaskStatus &status)
{
    QSqlQuery sqlQuery;
    if(status == FTaskStatus::All)
        sqlQuery = dbase.exec("SELECT * FROM Tasks");
    else if(status == FTaskStatus::Complete)
        sqlQuery = dbase.exec("SELECT * FROM Tasks WHERE status = 'Complete'");
    else if(status == FTaskStatus::Failed)
        sqlQuery = dbase.exec("SELECT * FROM Tasks WHERE status = 'Failed'");
    else if(status == FTaskStatus::Process)
        sqlQuery = dbase.exec("SELECT * FROM Tasks WHERE status = 'In process'");

    QList<FTaskStruct> result;

    while(sqlQuery.next()){
        result.append(FTaskStruct(sqlQuery.value(0).toString(),
                                  QDate::fromString(sqlQuery.value(1).toString(), "dd.MM.yyyy"),
                                  sqlQuery.value(2).toString()));
    }
    return result;
}

DataModel::DataModel(QObject *parent) : QSqlTableModel(parent)
{   
    dbase = QSqlDatabase::addDatabase("QSQLITE");

    dbase.setDatabaseName("tasks.db");
    if(!dbase.open()){
        qDebug() << "Error: Didn't opened database";
        std::exit(1);
    }

    dbase.exec("CREATE TABLE IF NOT EXISTS Tasks(name TEXT UNIQUE, deadline TEXT, status TEXT)");

    this->_tasks = ParseTasks(FTaskStatus::All);
}

int DataModel::rowCount(const QModelIndex &) const
{
    return _tasks.size();
}

int DataModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole){
        if(index.row() < _tasks.size()){
            if(index.column() == 0)
                    return _tasks.at(index.row()).name;
            if(index.column() == 1)
                    return _tasks.at(index.row()).deadline;
            if(index.column() == 2)
                    return _tasks.at(index.row()).status;
        }
    }
    return QVariant();
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        if(section == 0)
            return QString("Name");
        if(section == 1)
            return QString("Date");
        return QString("Status");
    }
    return QVariant();
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    flags |= Qt::ItemIsSelectable;
    return flags;
}

void DataModel::AddNewTask(QString name, QDate date)
{
    QString status;
    if(date < QDate::currentDate())
        status = "Failed";
    else
        status = "In process";

    // Add task to database
    QSqlQuery query;
    if(!query.exec("INSERT INTO Tasks(name, deadline, status) VALUES('" + name + "', '" + date.toString("dd.MM.yyyy") + "', '" + status + "')")){
        msgBox = new QMessageBox();
        msgBox->addButton(QMessageBox::Ok);
        msgBox->setText("You can't add this task because you already have task with the same name");
        msgBox->show();
        return;
    }

    this->_tasks.append(FTaskStruct(name, date, status));

    emit layoutChanged();
}

void DataModel::EditTask(QString name, QDate date, int index)
{
    QSqlQuery query;
    QString status;
    if(_tasks[index].deadline < QDate::currentDate()){
        if(_tasks[index].status != "Complete")
            status = "Failed";
        else
            status = "Complete";
    }

    else if(_tasks[index].status != "Complete")
        status = "In process";
    if(!query.exec("UPDATE Tasks SET name = '" + name + "', deadline = '" + date.toString("dd.MM.yyyy") + "', status = '" + status + "' WHERE name = '" + _tasks[index].name + "'")){
        msgBox = new QMessageBox();
        msgBox->setText("You cannot set this name of task because you already have task with the same name");
        msgBox->show();
        return;
    }
    _tasks[index].name = name;
    _tasks[index].deadline = date;
    _tasks[index].status = status;

    emit layoutChanged();
}

void DataModel::DeleteTask(QString name, int index)
{
    QSqlQuery query;
    QString queryStr = "DELETE FROM Tasks WHERE name = '" + name + "'";

    if(!query.exec(queryStr)){
        qDebug() << "Error: cannot delete task from database!";
        std::exit(EXIT_FAILURE);
    }

    this->_tasks.removeAt(index);

    emit layoutChanged();
}

void DataModel::ChangeStatus(const FTaskStatus &status)
{
    this->_tasks = ParseTasks(status);

    emit layoutChanged();
}
