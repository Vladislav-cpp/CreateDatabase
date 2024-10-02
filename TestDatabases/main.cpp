#include <QApplication>
#include <QTextStream>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QFile>

#include "TMiscUtils.h"

const static QString s_qsCSVFileName = "DataFile.SCV";
const static QString s_qsRefactoringCSVFileName = "RefactoringDataFile.SCV";
const static QString s_qsSqlDataBaseType = "QSQLITE";

const static int s_iLinesNumber = 1024;
const static int s_iColumnsNumber = 6;
const static int s_iLengthString = 8;

const static miscUtils::SCSVFileProperty s_sFileProperty { s_iLinesNumber, s_iColumnsNumber, s_iLengthString };

int main(int argc, char *argv[]) {

    // 1.A
    QFile csvFile( s_qsCSVFileName );
    miscUtils::FillRandomStringCSVFile( csvFile, s_sFileProperty);

    // 1.B
    QFile refactoringCSVFile( s_qsRefactoringCSVFileName );
    miscUtils::RefactoringStringCSVFile( csvFile, refactoringCSVFile, s_sFileProperty );

    // 1.C
    QVector< QVector<QString>> data;
    data.reserve(s_iLinesNumber);
    miscUtils::ExtrsctionDataFromSCVFile(csvFile, s_sFileProperty, data);

    auto db = QSqlDatabase::addDatabase(s_qsSqlDataBaseType);
    db.setDatabaseName("db.db");
    db.setConnectOptions("QSQLITE_ENABLE_REGEXP");

    if( !db.open() ) {
        qDebug() << "Error: Could not open database" << db.lastError();
        return 1;
    }

    QSqlQuery query(db);

    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS my_table (
                data1 TEXT,
                data2 TEXT,
                data3 TEXT,
                data4 TEXT,
                data5 TEXT,
                data6 TEXT
        )
    )";

    if( !query.exec(createTableQuery) ) {
        qDebug() << "Failed to create table:" << query.lastError();
        return 1;
    }

    query.prepare("INSERT INTO my_table (data1, data2, data3, data4, data5, data6)"
                  "VALUES (:data1, :data2, :data3, :data4, :data5, :data6)");

    QVector< QString > values{ {":data1"}, {":data2"}, {":data3"}, {":data4"}, {":data5"}, {":data6"} };
    for(int i = 0; i< s_iLinesNumber; ++i) {
        for(int j = 0; j< s_iColumnsNumber; ++j) {

            auto value = QString( data[i][j].data() );
            auto placeholder = values[j];
            query.bindValue( placeholder, value );
        }

        if( !query.exec() ) {
            qDebug() << "Failed to insert Alice:" << query.lastError();
            return 1;
        }
    }

    if( !query.exec("DELETE FROM my_table WHERE data2 GLOB '[0-9]*'") ) {
        qDebug() << "Помилка при виконанні запиту:" << query.lastError();
        return 1;
    }

    db.close();

    return 0;
}
