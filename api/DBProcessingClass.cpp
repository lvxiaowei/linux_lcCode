#include "DBProcessingClass.h"
#include <QDir>
#include <QRegExp>

DBProcessingClass* DBProcessingClass::_instance=NULL;

DBProcessingClass::DBProcessingClass()
{
    //表名和对应写入数据的SQL语句的映射关系
    m_mapTableTypeToSQL_write[TABLE_MAINCHAIN]        = SQL_WriteMainChainTable;
    m_mapTableTypeToSQL_write[TABLE_SELECTIONS]       = SQL_WriteSelectionTable;
    m_mapTableTypeToSQL_write[TABLE_STITCH]           = SQL_WriteStitchTable;
    m_mapTableTypeToSQL_write[TABLE_MACRO]            = SQL_WriteMacroTable;
    m_mapTableTypeToSQL_write[TABLE_PATTERN]          = SQL_WritePatternTable;
    m_mapTableTypeToSQL_write[TABLE_PATTERNTIMINGS]   = SQL_WritePatternTimingsTable;
    m_mapTableTypeToSQL_write[TABLE_COMMONCFG]        = SQL_WriteCommonCfgTable;

    initDB();
}

/*初始化数据库 新建：创建空表 打开：读取数据库*/
void DBProcessingClass::initDB()
{
//    /*创建临时文件夹用来保存数据库文件*/
//    QDir dir;
//    QString currentDir = dir.currentPath();
//    QString temporaryDir = QString("%1//%2").arg(currentDir).arg(TEMPORARYFORDER);
//    dir.setPath(temporaryDir);
//    if(!dir.exists())
//    {
//        dir.mkdir(temporaryDir);
//    }

//    openDB();

//    loadDB();

//    closeDB();
}

/*将界面数据写到数据库*/
bool DBProcessingClass::writeDB(const QString & tableName, const QList<QVariantList > & lstPara)
{
    /*写数据*/
    QSqlQuery query;
    QString strSql =m_mapTableTypeToSQL_write[tableName];

    /*判断传入的需要写入的lst中数据的数量是否和传入的表的字段数相等，不等的话返回失败*/
    QRegExp rx("(:\\w+)");
    QStringList listVal;
    int pos = 0;
    while ((pos = rx.indexIn(strSql, pos)) != -1)
    {
        listVal << rx.cap(1);
        pos += rx.matchedLength();
    }
    if(lstPara.count() != listVal.count())
    {
        return false;
    }

    query.exec("begin exclusive transaction;");
    qDebug()<<lstPara.at(0).count();
    for(int i=0;i<lstPara.at(0).count();++i)
    {
        query.prepare(strSql);
        for(int j=0; j<lstPara.count(); ++j)
        {
            query.bindValue(listVal.at(j),   lstPara.at(j).at(i).toString());
        }
        query.exec();
    }
    query.exec("commit;");
    return true;
}

/*读取数据库中的数据到界面*/
void DBProcessingClass::loadDB()
{

}

/*删除表的数据*/
void DBProcessingClass::clearTableData(const QString &tableName)
{
    QSqlQuery query;

    query.exec(QString("delete from %1").arg(tableName));  //清空表
}

/*打开数据库*/
void DBProcessingClass::openDB(const QString name)
{
    /*初始化数据库*/
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        /*("QSQLITE", "QMYSQL", "QMYSQL3", "QODBC", "QODBC3", "QPSQL", "QPSQL7", "SQLITECIPHER")*/
        database = QSqlDatabase::addDatabase("QSQLITE");

    }
    database.setDatabaseName(name);
    database.setPassword("!@#$%^&*(");
    database.setConnectOptions("QSQLITE_CREATE_KEY");

    database.open();
}

/*关闭数据库*/
void DBProcessingClass::closeDB()
{
    database.close();
}
