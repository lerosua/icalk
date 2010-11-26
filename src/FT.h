/*
* =====================================================================================
*
*       Filename:  FT.h
*
*    Description:  
*
*        Version:  1.0
*        Created:  2008年05月15日 13时01分49秒
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  first_name last_name (fl), fl@my-company.com
*        Company:  my-company
*
* =====================================================================================
*/
#ifndef XFER_FILE_H_
#define XFER_FILE_H_

#include <fstream>
#include <ios>

using namespace std;
/**
 *  * The different states of the xfer.
 *   */
typedef enum
{
        XFER_STATUS_UNKNOWN = 0,            /**< Unknown, the xfer may be null. */
        XFER_STATUS_NOT_STARTED,            /**< It hasn't started yet. */
        XFER_STATUS_ACCEPTED,               /**< Receive accepted, but destination file not selected yet */
        XFER_STATUS_STARTED,                /**< The xfer has started. */
        XFER_STATUS_DONE,                   /**< The xfer completed successfully. */
        XFER_STATUS_CANCEL_LOCAL,           /**< The xfer was canceled by us. */
        XFER_STATUS_CANCEL_REMOTE  /**< The xfer was canceled by the other end, or we couldn't connect. */
} XferStatusType;

/**
 * @brief 文件传输中文件的实际处理
 */

class XferFile
{

public:
        XferFile();
        ~XferFile();
        void write(const std::string& data, streamsize length);
        void read(char* data, streamsize length);
        int open(const char * filename,
                  ios_base::openmode mode = ios_base::in | ios_base::out );
        void close() { 
			if (file.is_open())
				file.close();
		}
        bool eof()const { return file.eof();}
        streamsize gcount()const{ return file.gcount();}
        long getTotalsize()const
        {
                return totalsize;
        }
		void setTotalsize(long size) { totalsize = size ; }

		const std::string& getFilePath()const  { return filepath; }
        long getSentBytes()const
        {
                return bytes_sent;
        }

        XferStatusType getStatusType()const
        {
                return status;
        }

        /** 获取已传输数据的占总数据的百分比*/
        int getPercent()const { return (int) ( ( ((double)(bytes_sent)) / ((double)(totalsize))) * 100) ; }

        void setStatusType(XferStatusType f_status) {
				status = f_status;
		}

private:
        std::string sid;       /**此次传输的sid */
        std::string who; /**传输的对方*/
        std::fstream file; /** 传输的文件流*/
		std::string filepath; /** 带路径的文件名*/
        long totalsize;  /** 文件的总大小*/
        long bytes_sent;  /**已传输的字节数*/
        XferStatusType status;  /**传输的状态*/

};


#endif
