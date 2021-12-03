#ifndef RESULT_WRITER_H
#define RESULT_WRITER_H

#include <string>
#include <fstream>

class ResultWriter {
public:
    ResultWriter(std::string fName, std::string* headerList, int headerLength, char delim);
    std::string write(std::string* results);
    void close();
private:
    char delimeter;
    int rowLength;
    std::ofstream file;
};

#endif