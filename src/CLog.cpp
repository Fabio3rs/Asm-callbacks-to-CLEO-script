/*****************************************************************************************************
*		GUITAR++
*		PROGRAMADO POR FÁBIO
*		BMS - Brazilian Modding Studio - http://brmodstudio.forumeiros.com
*****************************************************************************************************/
/*
 *  Logging class source file
 * 
 *  Copyright (C) 2018 Fabio3rs <>
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 * 
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 * 
 *     1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 * 
 *     2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 * 
 *     3. This notice may not be removed or altered from any source
 *     distribution.
 *
 */
#include "CLog.h"
#include <stdio.h>
#include <time.h>

//int InstallExceptionCatcher(void(*cb)(const char* buffer));

CLog &CLog::log(){
	static CLog Log("CPPTOCLEO.log");
	return Log;
}

CLog::CLog(const std::string &NameOfFile){
	Finished = false;
	FileName = NameOfFile;
	LogFile.rdbuf()->pubsetbuf(0, 0);
	LogFile.open(NameOfFile, std::ios::in | std::ios::out | std::ios::trunc);
	
	if(!LogFile.good()) throw CLogException("Impossivel criar ou abrir o arquivo de log");

	std::string LogContents;
	LogContents += "*****************************************************************************\n";
	LogContents += std::string("* NFSDebug compilation date/time: ") + __DATE__ + " " + __TIME__;
	LogContents += "\n*****************************************************************************\n";
	LogContents += "*****************************************************************************\n* Log started at: ";
	LogContents += GetDateAndTime();
	LogContents += "\n*****************************************************************************\n\n";
	LogFile.write(LogContents.c_str(), LogContents.size());

/*#ifdef _WIN32
	InstallExceptionCatcher([](const char *buffer)
	{
		log() << buffer;
		log().SaveBuffer();
	});
#endif*/
}

CLog::~CLog() noexcept {
	FinishLog();
}

std::string CLog::GetDateAndTime()
{
	std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string str = std::string(ctime(&tt));
	if (str[str.size() - 1] == '\n')
	{
		str.resize(str.size() - 1);
	}
	return str;
}

void CLog::AddToLog(const std::string &Text)
{
	std::string Temp;
	Temp += GetDateAndTime();
	Temp += ": ";
	Temp += Text;
	Temp += "\n";
	LogFile.write(Temp.c_str(), Temp.size());
	//LogFile << Temp;
}

void CLog::FinishLog()
{
	std::string LogContents;
	LogContents += "\n*****************************************************************************\n* Log Finished at: ";
	LogContents += GetDateAndTime();
	LogContents += "\n*****************************************************************************\n";
	LogFile.clear();
	LogFile.write(LogContents.c_str(), LogContents.size());
	LogFile.flush();
	Finished = true;
}

void CLog::SaveBuffer()
{
	LogFile.flush();
}