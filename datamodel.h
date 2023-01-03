#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractTableModel>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QMessageBox>
#include "FTaskStruct.h"

class DataModel : public QSqlTableModel
{
    Q_OBJECT

private:
    QList<FTaskStruct> _tasks;
    QSqlDatabase dbase;
    QList<FTaskStruct> ParseTasks(const FTaskStatus &status);
    QMessageBox *msgBox;
public:
    DataModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

public slots:
    void AddNewTask(QString name, QDate date);
    void EditTask(QString name, QDate date, int index);
    void DeleteTask(QString name, int index);
    void ChangeStatus(const FTaskStatus &status);
};

#endif // DATAMODEL_H
