
#include <sys/stat.h>
#include <time.h>
#include "MsgLog.h"
#include "Unit.h"


MsgLog::MsgLog(const std::string& jid_): jid(jid_) {

        struct tm* tmw;
        time_t now;
        time(&now);
        tmw = localtime(&now);
        char filename[255];
        char dirname[255];
        snprintf(dirname, 255, "%s%s", GUnit::getLogPath(), jid.c_str());
        mkdir(dirname, 0744);

        snprintf(filename, 255, "%s/%04d-%02d-%02d.txt ", dirname, tmw->tm_year + 1900, tmw->tm_mon + 1, tmw->tm_mday);

        logfile.open(filename, std::ios::binary | std::ios::app);

        if(!logfile)
                std::cerr << "open logfile error in " << jid << std::endl;

}

MsgLog::~MsgLog() {
        logfile.close();
        printf("saveing file\n");
}

void MsgLog::write(const std::string& sender, const std::string& msg) {

        struct tm* tmw;
        time_t now;
        time(&now);
        tmw = localtime(&now);
        char timetext[255];
        snprintf(timetext, 32, "%02d:%02d:%02d ", tmw->tm_hour, tmw->tm_min, tmw->tm_sec);
        Glib::ustring timestr(timetext);
        logfile << timetext << "  " << sender << " :: " << msg << std::endl;

}
