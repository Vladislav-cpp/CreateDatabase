#ifndef TMISCUTILS_H
#define TMISCUTILS_H

#include <QString>
#include <QVector>

class QFile;

namespace miscUtils {
    struct SCSVFileProperty {
        int lineNumber = 0;
        int columnsNumber = 0;
        int lengthString = 0;

        int rowLength() const { return columnsNumber + lengthString*columnsNumber + 1/*\n*/; };
    };

    bool СheckFileOpen(const QFile& file);

    QString GenerateRandomString(int length );

    bool IsVowel(char ch);

    void FillRandomStringCSVFile(QFile& file, const SCSVFileProperty& fileProperty);
    void RefactoringStringCSVFile(QFile& originalFile, QFile& destinationFile, const SCSVFileProperty& fileProperty);
    void ExtrsctionDataFromSCVFile(QFile& scvFile, const SCSVFileProperty& fileProperty, QVector<QVector<QString> >& data);

    QVector<QString> ParseCSV(QString& input);
}
#endif // TMISCUTILS_H
