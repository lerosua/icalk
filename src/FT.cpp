/*
* =====================================================================================
*
*       Filename:  FT.cpp
*
*    Description:  
*
*        Version:  1.0
*        Created:  2008年05月15日 13时04分59秒
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  first_name last_name (fl), fl@my-company.com
*        Company:  my-company
*
* =====================================================================================
*/

#include "FT.h"

using namespace std;

XferFile::XferFile():
                totalsize(0)
                , bytes_sent(0)
                , status(XFER_STATUS_NOT_STARTED)
{}

XferFile::~XferFile()
{}


void XferFile::open(const char * filename,
                    ios_base::openmode mode)
{
        file.open(filename, mode);
}

bool XferFile::eof()const
{
        return file.eof();
}

streamsize XferFile::gcount()const
{
        return file.gcount();
}

void XferFile::write(const std::string& data, streamsize length)
{
        file.write(data.c_str(), length);
        bytes_sent += length;
}

void XferFile::read(char* data, streamsize length)
{
        file.read(data, length);
        bytes_sent += length;
}

void XferFile::close()
{
        if (file.is_open())
                file.close();
}

void XferFile::setSentBytes(long bytes)
{
        bytes_sent += bytes;
}

void XferFile::setTotalsize(long bytes)
{
        totalsize = bytes;
}

int XferFile::getPercent()const
{
        double result = ( ((double)(bytes_sent)) / ((double)(totalsize))) * 100 ;
        return (int)(result);
}

void XferFile::setStatusType(XferStatusType f_status)
{
        status = f_status;
}

