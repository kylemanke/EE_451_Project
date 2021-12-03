#ifndef RESULT_WRITER_H
#define RESULT_WRITER_H

#include <string>
#include <fstream>

class ResultWriter {
public:
    ResultWriter(std::string fName, std::string* headerList, int headerLength, std::string delim);
    std::string write(std::string* results);
    void close();
private:
    std::string delimeter;
    int rowLength;
    std::ofstream file;
};

#endif