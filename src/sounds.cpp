/***************************************************************************
 *   Copyright (C) 2005 by xihe                                            *
 *   xihels@163.com                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "sounds.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "Unit.h"

namespace sounds {
    const char* command = "aplay";
    int PLAY = 0;
    int MUTE=0 ;
}
/**@brief 设置静音或者返回状态。
 * @param mute_ 为0不静音
 *		为1静音
 *		为3返回现在的状态
 * @return 返回MUTE的值
 */
int sounds::mute(int mute_)
{
	if(mute_ ==3)
		return MUTE;
	else if(mute_==1)
		MUTE=1;
	else
		MUTE=0;
	return MUTE;
}
void sounds::play(int code)
{
	//if (MUTE)
	//	return;
    if (PLAY) 
	return ;
    PLAY = 1;

    char filename[255];
    switch (code) {
	case ARRIVE_SOUND:
	    snprintf(filename, 255, "%sarrive.wav", GUnit::getSoundPath());
	    break;
	case LEAVE_SOUND:
	    snprintf(filename, 255, "%sleave.wav", GUnit::getSoundPath());
	    break;
	case RECEIVE_SOUND:
	    snprintf(filename, 255, "%sreceive.wav", GUnit::getSoundPath());
	    break;
	case REDALERT_SOUND:
	    snprintf(filename, 255, "%sredalert.wav", GUnit::getSoundPath());
	    break;
	case SEND_SOUND:
	    snprintf(filename, 255, "%ssend.wav", GUnit::getSoundPath());
	    break;
    }
    do_play(filename);
}


void sounds::do_play(const char* filename)
{
    pid_t pid;
    int status;
    pid = fork();
    if (-1 == pid) {
        perror("Fork falid to creat a process");
    } else if (0 == pid) {
        close(0);
        close(1);
        close(2);
        if (execlp(command, command, filename, NULL) < 0) {
            perror("Execl failed");
            exit (1);
        }
    }
}


void sounds::on_play_exit(int s)
{
    pid_t pid;
    int	  stat;

    while (waitpid(-1, &stat, WNOHANG) > 0)
	NULL;
    PLAY = 0;
}

