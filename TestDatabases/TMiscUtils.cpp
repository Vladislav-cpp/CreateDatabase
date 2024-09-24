#include "TMiscUtils.h"

#include <QFile>
#include <QDebug>

#include <random>
#include <cctype>
#include <algorithm>

static const std::string s_sCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

namespace miscUtils {

bool СheckFileOpen(const QFile& file) {
    auto isOpen = file.isOpen();
    if( !isOpen ) qDebug() << "Unable to open file - " << file.fileName();
    return isOpen;
}

std::string GenerateRandomString(int length ) {
    std::random_device rd;
    std::mt19937 generator(rd());

    std::uniform_int_distribution<> distribution( 0, s_sCharacters.size() - 1 );

    std::string random_string;
    random_string.resize(length);

    std::generate(random_string.begin(), random_string.end(),
                  [&generator, &distribution] () { return s_sCharacters[distribution(generator)]; });

    return random_string;
}


bool IsVowel(char ch) {
    const std::string vowels = "aeiouAEIOU";

    return std::any_of(vowels.begin(), vowels.end(), [ch](char vowel) { return ch == vowel; });
}

void FillRandomStringCSVFile(QFile& file, const SCSVFileProperty& fileProperty) {
    std::string data;

    for(int i = 0; i<fileProperty.lineNumber*fileProperty.columnsNumber; ++i) {

        data += GenerateRandomString(fileProperty.lengthString);

        if( (i+1)%fileProperty.columnsNumber == 0 ) data += "\n";
        else data += ";";
    }

    file.open( QIODevice::WriteOnly );

    if( !СheckFileOpen( file ) ) return;

    file.write( data.c_str() );
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

void ExtrsctionDataFromSCVFile(QFile& scvFile, const SCSVFileProperty& fileProperty, std::vector< std::vector<std::string> >& data) {
    scvFile.open( QIODevice::ReadOnly );
    if( !СheckFileOpen( scvFile ) ) return;

      for(int i = 0; i<fileProperty.lineNumber; ++i) {
          auto byteArray = scvFile.readLine( fileProperty.rowLength() );
          data.push_back( miscUtils::ParseCSV( std::string(byteArray) ) );
      }

    scvFile.close();
}

std::vector<std::string> ParseCSV(const std::string& input) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;

    while( std::getline(ss, item, ';') ) { // \t
        //if(item=="\n"|| item=="\r" || item=="\n\r") continue;
        result.push_back(item);
    }

    if( !result.empty() && result.back().back() == '\n' ) result.back().pop_back();

    return result;
}

}
