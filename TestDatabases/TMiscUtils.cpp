#include "TMiscUtils.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QRandomGenerator>

static const QString s_sCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

namespace miscUtils {

bool СheckFileOpen(const QFile& file) {
    auto isOpen = file.isOpen();
    if( !isOpen ) qDebug() << "Unable to open file - " << file.fileName();
    return isOpen;
}

QString GenerateRandomString(int length) {
    QString randomString;

    randomString.resize(length);

    for (int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->bounded( s_sCharacters.size() );
        randomString[i] = s_sCharacters.at(index);
    }

    return randomString;
}

bool IsVowel(char ch) {
    const QString vowels = "aeiouAEIOU";
    return vowels.contains(ch);
}

void FillRandomStringCSVFile(QFile& file, const SCSVFileProperty& fileProperty) {
    QString data;

    for(int i = 0; i<fileProperty.lineNumber*fileProperty.columnsNumber; ++i) {

        data += GenerateRandomString(fileProperty.lengthString);

        if( (i+1)%fileProperty.columnsNumber == 0 ) data += "\n";
        else data += ";";
    }

    file.open( QIODevice::WriteOnly );

    if( !СheckFileOpen( file ) ) return;

    file.write( data.toUtf8() );
    file.close();
}

void RefactoringStringCSVFile(QFile& originalFile, QFile& destinationFile, const SCSVFileProperty& fileProperty) {

    destinationFile.open( QIODevice::WriteOnly );
    originalFile.open( QIODevice::ReadOnly );

    if( !СheckFileOpen(destinationFile) ) return;
    if( !СheckFileOpen(originalFile) ) return;

    for(int i = 0; i<fileProperty.lineNumber; ++i) {

        auto byteArray = originalFile.readLine( fileProperty.rowLength() );

        auto bSkipLine = true;
        for(int i = 0; i<std::size(byteArray); i+=fileProperty.lengthString + 1) {
            if( IsVowel( byteArray[i] ) ) continue;

            bSkipLine = false;
            break;
        }

        if( bSkipLine ) {
            destinationFile.write("\n");
            continue;
        }

        for(auto& byte : byteArray) {
            if( std::isdigit(byte) && (byte - '0') % 2 != 0 ) byte = '#';

        }

        destinationFile.write(byteArray);
    }

    originalFile.close();
    destinationFile.close();
}

void ExtrsctionDataFromSCVFile(QFile& scvFile, const SCSVFileProperty& fileProperty, QVector< QVector<QString> >& data) {
    scvFile.open( QIODevice::ReadOnly );
    if( !СheckFileOpen( scvFile ) ) return;

      for(int i = 0; i<fileProperty.lineNumber; ++i) {
          QString line = scvFile.readLine( fileProperty.rowLength() );
          data.push_back( miscUtils::ParseCSV( line ) );
      }

    scvFile.close();
}

QVector<QString> ParseCSV(QString& input) {
    QVector<QString> result;
    QStringList items = input.split(';');

    for (const QString& item : items) {
        //if(item=="\n"|| item=="\r" || item=="\n\r") continue;
        result.push_back(item);
    }

    //if( !result.empty() && result.back().back() == '\n' ) result.back().pop_back();

    return result;
}

}
