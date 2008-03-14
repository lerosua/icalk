#ifndef MSGLOG_H_
#define MSGLOG_H_

#include <iostream>
#include <fstream>


class MsgLog
{

public:
        MsgLog(const std::string& jid_);

        ~MsgLog();

        void write(const std::string& sender, const std::string& msg);

private:
        std::ofstream logfile;
        std::string jid;



};





#endif
