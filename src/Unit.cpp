/*
 * =====================================================================================
 *
 *       Filename:  Unit.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2007年07月12日 22时21分43秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */

#include <libgen.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "Unit.h"
#define MAX_NUM 512

char GUnit::homePath[MAX_NUM];
char GUnit::userPath[MAX_NUM];
char GUnit::imagePath[MAX_NUM];
char GUnit::soundPath[MAX_NUM];
char GUnit::logPath[MAX_NUM];
char GUnit::iconPath[MAX_NUM];
char GUnit::tempPath[MAX_NUM];

void GUnit::init(const char* user)
{
	char* homedir = getenv("HOME");
	snprintf(homePath,MAX_NUM,"%s/.icalk",homedir);
	mkdir(homePath, S_IRUSR | S_IWUSR | S_IXUSR);

	char* userdir;
	snprintf(userPath,MAX_NUM,"%s/%s",homePath,user);
	mkdir(userPath, S_IRUSR | S_IWUSR | S_IXUSR);

	char* icondir;
	snprintf(iconPath, MAX_NUM, "%s/icons",homePath);
	mkdir(iconPath,S_IRUSR | S_IWUSR | S_IXUSR);

	snprintf(logPath,MAX_NUM,"%s/logs/", userPath);
	mkdir(logPath, S_IRUSR | S_IWUSR | S_IXUSR);

	snprintf(tempPath,MAX_NUM,"%s/temp/", userPath);
	mkdir(tempPath, S_IRUSR | S_IWUSR | S_IXUSR);

	snprintf(imagePath, MAX_NUM, "%s/images/", DATA_DIR);
	snprintf(soundPath, MAX_NUM, "%s/sounds/", DATA_DIR);
}



