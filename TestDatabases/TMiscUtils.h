#ifndef TMISCUTILS_H
#define TMISCUTILS_H

#include <string>
#include <vector>

class QFile;

namespace miscUtils {
    struct SCSVFileProperty {
        int lineNumber = 0;
        int columnsNumber = 0;
        int lengthString = 0;

        int rowLength() const { return columnsNumber + lengthString*columnsNumber + 1/*\n*/; };
    };

    bool СheckFileOpen(const QFile& file);

    std::string GenerateRandomString(int length );

    bool IsVowel(char ch);

    void FillRandomStringCSVFile(QFile& file, const SCSVFileProperty& fileProperty);
    void RefactoringStringCSVFile(QFile& originalFile, QFile& destinationFile, const SCSVFileProperty& fileProperty);
    void ExtrsctionDataFromSCVFile(QFile& scvFile, const SCSVFileProperty& fileProperty, std::vector< std::vector<std::string> >& data);

    std::vector<std::string> ParseCSV(const std::string& input);
}
#endif // TMISCUTILS_H
