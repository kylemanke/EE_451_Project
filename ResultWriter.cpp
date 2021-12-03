#include "ResultWriter.h"

using namespace std;

ResultWriter::ResultWriter(string fName, string* headerList, int headerLength, string delim=",") {
    // Manage resources
    file.open(fName, ofstream::out);
    delimeter = delim;
    rowLength = headerLength;

    // Print header to file
    string header = headerList[0];
    for(int i = 1; i < headerLength; i++) {
        header += delimeter + headerList[i];
    }
    header += "\n";
    file.write(header.c_str(), header.size());
}

string ResultWriter::write(string* results) {
    string row = results[0];
    for(int i = 1; i < rowLength; i++)
        row += delimeter + results[i];
    row += "\n";
    file.write(row.c_str(), row.size());
    return row;
}

void ResultWriter::close() {
    file.close();
}