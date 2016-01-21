#ifndef ODBCEXCEL_H
#define ODBCEXCEL_H

#include <QObject>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTableView>
class OdbcExcel
{
public:
    OdbcExcel();
    //�����ݱ���Ϊexcel
    bool static save(int flag,QString filePath,QStringList headers,QList<QStringList> data,QString comment="");
    //��QTableView����Ϊexcel
    bool static saveFromTable(int flag,QString filePath,QTableView *tableView,QString comment="");
    //��ȡ������Ϣ
    QString static getError(){return error;}
private:
    void static printError( QSqlError error);
    bool static insert(QSqlQuery& query, QString sheetName, QStringList slist);
    static QString error;
};


#endif // ODBCEXCEL_H