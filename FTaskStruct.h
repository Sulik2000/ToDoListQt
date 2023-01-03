#ifndef FTASKSTRUCT_H
#define FTASKSTRUCT_H

#include <QDate>
#include <QString>


enum struct FTaskStatus{
    Complete,
    Failed,
    Process,
    All
};

struct FTaskStruct{
    FTaskStruct(QString Name, QDate Deadline, QString Status){
        this->name = Name;
        this->deadline = Deadline;
        this->status = Status;
    }
    QString name;
    QDate deadline;
    QString status;
};

#endif // FTASKSTRUCT_H
