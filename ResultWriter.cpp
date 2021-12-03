#include "ResultWriter.h"

using namespace std;

ResultWriter::ResultWriter(string fName, string* headerList, int headerLength, char delim=',') {
    // Manage resources
    file.open(fName, ofstream::out);
    delimeter = delim;
    rowLength = headerLength;

    // Print header to file
    string header = headerList[0];
    for(int i = 1; i < headerLength; i++) {
        headerList += delim + headerList[i];
    }
    headerList += "\n"
    file.write(header);
}

string ResultWriter::write(string* results) {
    string row = results[0];
    for(int i = 1; i < rowLength; i++)
        row += delimeter + results[i];
    row += "\n";
    file.write(row);
    return row;
}

void ResultWriter::close() {
    file.close();
}