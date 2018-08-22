#ifndef DBPROCESSINGCLASS_H
#define DBPROCESSINGCLASS_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "publicData.h"
#include "comConfigClass.h"
#include <QDebug>

class DBProcessingClass
{
public:

    /*创建单实例对象*/
    static DBProcessingClass* GetInstance()
    {
        if(DBProcessingClass::_instance == NULL)
        {
            DBProcessingClass::_instance = new DBProcessingClass();
        }
        return DBProcessingClass::_instance;
    }

public:
    void initDB();
    void loadDB();
    void openDB(const QString name);
    void closeDB();
    void clearTableData(const QString& tableName);
    bool createPassword(const QString & strPaswrd);
    bool movePassword(){return true;}
    bool updatePassword(const QString & oldPaswrd, const QString & newPaswrd){ Q_UNUSED(oldPaswrd); Q_UNUSED(newPaswrd); return true;}
    bool writeDB(const QString & tableName, const QList<QVariantList > & lstPara);
private:
    DBProcessingClass();
    ~DBProcessingClass(){}

    static DBProcessingClass* _instance;

    bool createNewDBConnection(const QString &filePath);
    bool openDBConnection(const QString &filePath);

    QSqlDatabase database;
    QMap<QString, QString> m_mapTableTypeToSQL_read;    /*数据库表格名字和操作SQL的映射--读*/
    QMap<QString, QString> m_mapTableTypeToSQL_write;   /*数据库表格名字和操作SQL的映射--写*/
};

#endif // DBPROCESSINGCLASS_H
