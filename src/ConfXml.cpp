#include <fstream>
#include <iostream>
#include <exception>
#include <streambuf>
#include "ConfXml.h"

using namespace std;


Tag* ConfXml::xml_from_file(char* filename)
{
        std::ifstream infile(filename, ios::binary);

        if (!infile)
        {
                //没有这个文件则新建
                std::ofstream outfile(filename);
                outfile << "<?xml version='1.0' encoding='UTF-8'?>\n<account version='1.0'></account>" << std::endl;
                outfile.close();
                infile.open(filename, ios::in | ios::binary);

                if (!infile)
                        std::cerr << "can't not open file" << filename << std::endl;
        }

        //将文件读入string的代码
        std::istreambuf_iterator<char> beg(infile), end;

        std::string data(beg, end);

        /**feed将会驱动handleTag函数，handleTag中将会拷贝Tag类*/
        parser->feed(data);

        return tagxml;

}

void ConfXml::xml_saveto_file(char* filename)
{

        std::ofstream os(filename);
        os << tagxml->xml();
        os.close();
}

