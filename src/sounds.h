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
#ifndef _SOUNDS_H_
#define _SOUNDS_H_ 
/**
 * @brief 声音库的名字空间
 * @since 1.0
 */

namespace sounds
{
/** 到达时的声音*/
const int ARRIVE_SOUND = 1;
/** 离开时的声音*/
const int LEAVE_SOUND = 2;
/** 接收到信息时的声音*/
const int RECEIVE_SOUND = 3;
const int REDALERT_SOUND = 4;
/** 发送后的声音*/
const int SEND_SOUND = 5;

/** 设置静音的函数*/
int mute(int mute_);
void play(int code);
void do_play(const char* filename);
void on_play_exit(int s);
}


#endif // _SOUNDS_H_

