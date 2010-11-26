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
*       Compiler:  g++
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "FT.h"

using namespace std;

XferFile::XferFile():
                totalsize(0)
                , bytes_sent(0)
                , status(XFER_STATUS_NOT_STARTED)
{}

XferFile::~XferFile()
{
	close();
}


int XferFile::open(const char * filename,
                    ios_base::openmode mode)
{
        file.open(filename, mode);
		if(!file)
			return -1;

		struct stat f_stat;
        if (stat(filename, &f_stat))
                return -2;
        totalsize  = f_stat.st_size;
		return 0;
}


void XferFile::write(const std::string& data, streamsize length)
{
        file.write(data.c_str(), length);
        bytes_sent += file.gcount();
}

void XferFile::read(char* data, streamsize length)
{
        file.read(data, length);
        bytes_sent += file.gcount();
}

void XferFile::close()
{
        if (file.is_open())
                file.close();
}

int XferFile::getPercent()const
{
        double result = ( ((double)(bytes_sent)) / ((double)(totalsize))) * 100 ;
        return (int)(result);
}


