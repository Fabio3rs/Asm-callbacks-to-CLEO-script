/*
 *  ASI project to generate assembly callbacks to use with CLEO scripts
 *  This code is incomplete
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
#define _CRT_SECURE_NO_WARNINGS
// https://github.com/thelink2012/injector
#include <injector/injector.hpp>
#include <injector/assembly.hpp>
#include <injector/calling.hpp>
#include <injector/saving.hpp>
#include <game_sa/CRunningScript.h>
#include <deque>
#include <memory>
#include <vector>
#include "dynamic_hooker.hpp"
#include "CLog.h"

static int siz();
auto CRunningScript__Process = injector::thiscall<char(CRunningScript *)>::call<0x00469F00>;

struct externalCallbackStructure
{
	CVehicle *veh;
	int32_t status;
	void *gameStoredData;
};

struct cppToCleoCallback
{
	uintptr_t							address;
	CRunningScript						fakeScript;
	CRunningScript						*owner;
	uint8_t								*startIp;
	int									nparams;


	injectcode::dynamic_hooker::newCode	myCode;

	void run(injector::reg_pack &c)
	{
		if (owner->baseIP == nullptr)
			return;

		fakeScript = CRunningScript();

		fakeScript.baseIP = owner->baseIP;
		fakeScript.curIP = startIp;

		fakeScript.IsExternalThread = true;
		fakeScript.isActive = true;
		fakeScript.tls[0].pParam = std::addressof(c);

		memcpy(&(fakeScript.tls[1]), (void*)(c.esp), nparams * 4);

		CRunningScript__Process(&fakeScript);
	}
};

void runCleoCallback(injector::reg_pack &c, uintptr_t address)
{
	cppToCleoCallback *cb = (cppToCleoCallback*)address;

	if (cb)
	{
		cb->run(c);
	}
}

std::deque<std::unique_ptr<cppToCleoCallback>> callbacks;

const static int test = sizeof(injectcode::dynamic_hooker::newCode);

int siz()
{
	return callbacks.size();
}

/*
{$CLEO .cs}

0AC6: 0@ = label @_initCPPCode offset
0AA5: call_function 0@ num_params 0 pop 0

0AA2: 0@ = load_library "cppToCleo.asi" // IF and SET 
0AA4: 1@ = get_proc_address "generateFunction" library 0@ // IF and SET     
0A9F: 4@ = current_thread_pointer 
0AA7: call_function 1@ num_params 4 pop 4 0 0 @noCheats 4@ -> 3@ // cleoToCppCallBackLabel   

2@ = 0x008A5B58

while 2@ < 0x008A5CC8
    0A8C: write_memory 2@ size 4 value 3@ virtual_protect 1
    2@ += 4
end

while true
   wait 0x7FFFFFFF
end

:noCheats
0AD1: show_formatted_text_highpriority "Cheats nao sao permitidos" time 5000
wait 0
*/
extern "C" __declspec(dllexport) injectcode::dynamic_hooker::newCode* generateFunction(CRunningScript *owner, int atLabel, int nparams, int popBytes)
{
	CLog::log().multiRegister("generateFunction called %0 %1 %2", (uintptr_t)owner, atLabel, nparams);
	if (atLabel < 0)
	{
		atLabel = -atLabel;
	}

	callbacks.push_back(std::unique_ptr<cppToCleoCallback>((cppToCleoCallback*)malloc(sizeof(cppToCleoCallback))));

	cppToCleoCallback *nCallback = callbacks.back().get();

	nCallback->address = (uintptr_t)nCallback;
	//nCallback->baseIp = owner->baseIP;
	nCallback->owner = owner;
	nCallback->nparams = nparams;

	nCallback->startIp = std::addressof(owner->baseIP[atLabel]);

	memset(std::addressof(nCallback->myCode), 0, sizeof(nCallback->myCode));

	injectcode::dynamic_hooker::genDynamicCall(nCallback->myCode, nCallback->address, runCleoCallback, popBytes);

	return std::addressof(nCallback->myCode);
}

static void onload(int save)
{
	CLog::log() << "Onload clear";

	static bool firstLoad = true;

	//if (!firstLoad)
	//	callbacks.clear();
	//
	//firstLoad = false;
}

typedef injector::function_hooker<0x0053BCC9, void(void)> ngamectscptp_hook;
typedef injector::function_hooker<0x00618F05, void(void)> ngamectscptp_hook2;
typedef injector::function_hooker<0x0053BE8D, void(void)> ngamectscptp_hook3;

void staticHookClearData()
{
	CLog::log() << "make_static_hook onload clear";
	CLog::log().SaveBuffer();
	callbacks.clear();
}

void inject()
{
	CLog::log() << "inject called";
	static bool injected = false;

	if (!injected)
	{
		CLog::log() << "injected";
		injected = true;
		injector::save_manager::on_load(onload);

		injector::make_static_hook<ngamectscptp_hook>([](ngamectscptp_hook::func_type func)
		{
			staticHookClearData();
			return func();
		});

		injector::make_static_hook<ngamectscptp_hook2>([](ngamectscptp_hook2::func_type func)
		{
			staticHookClearData();
			return func();
		});

		injector::make_static_hook<ngamectscptp_hook3>([](ngamectscptp_hook3::func_type func)
		{
			staticHookClearData();
			return func();
		});
	}
}

BOOL WINAPI DllMain(
	_In_  HINSTANCE hinstDLL,
	_In_  DWORD fdwReason,
	_In_  LPVOID lpvReserved
) {
	if (fdwReason == DLL_PROCESS_ATTACH) inject();


	// DEBUG if (fdwReason == DLL_PROCESS_DETACH) MessageBoxA(0, "masoqmasoqmasoqmasoq", std::to_string(callbacks.size()).c_str(), 0);
	return true;
}