/*
 *  Dynamic_hooker inspired on LINK/2012 Injector
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
#pragma once
#ifndef _LUA_HOOKER_DYNAMIC_HOOKER_H_
#define _LUA_HOOKER_DYNAMIC_HOOKER_H_
#include <cstdint>
// https://github.com/thelink2012/injector
#include <injector\assembly.hpp>
#include <injector\calling.hpp>
#include <injector\injector.hpp>
#include <algorithm>
#include <string>
#include <vector>

namespace injectcode{
	typedef void(__cdecl *callback_t)(injector::reg_pack&, uintptr_t address);

	static uintptr_t camefromaddress;
	static uintptr_t usrcall;
	static uintptr_t retnptr;
	static uintptr_t espreg;


	inline void callwrapper(uintptr_t retAddr, callback_t cb, uintptr_t address, injector::reg_pack *r)
	{
		cb(*r, address);
	}

	// Constructs a reg_pack and calls the wrapper functor
	static inline void __declspec(naked) make_reg_pack_and_call()
	{
		_asm
		{
			pop retnptr
				pop usrcall
				pop camefromaddress
				// Construct the reg_pack structure on the stack
				pushad              // Pushes general purposes registers to reg_pack
				add[esp + 12], 4     // Add 4 to reg_pack::esp 'cuz of our return pointer, let it be as before this func is called
				pushfd              // Pushes EFLAGS to reg_pack

				// Call wrapper sending reg_pack as parameter
				push esp
				call callwrapper
				add esp, 4

				// Destructs the reg_pack from the stack
				sub[esp + 12 + 4], 4   // Fix reg_pack::esp before popping it (doesn't make a difference though) (+4 because eflags)
				popfd               // Warning: Do not use any instruction that changes EFLAGS after this (-> sub affects EF!! <-)
				popad

				// Back to normal flow
				ret
		}
	}

	static inline void __declspec(naked) make_reg_pack_and_call_with_return()
	{
		_asm
		{
			pop retnptr
				pop usrcall
				pop camefromaddress

				// Construct the reg_pack structure on the stack
				pushad              // Pushes general purposes registers to reg_pack
				add[esp + 12], 4     // Add 4 to reg_pack::esp 'cuz of our return pointer, let it be as before this func is called
				pushfd              // Pushes EFLAGS to reg_pack

				// Call wrapper sending reg_pack as parameter
				push esp
				call callwrapper
				add esp, 4

				// Destructs the reg_pack from the stack
				sub[esp + 12 + 4], 4   // Fix reg_pack::esp before popping it (doesn't make a difference though) (+4 because eflags)
				popfd               // Warning: Do not use any instruction that changes EFLAGS after this (-> sub affects EF!! <-)
				popad

				// jump
				push retnptr
				ret
		}
	}

	class dynamic_hooker{

	public:
	#pragma pack(push, 1)
		struct code
		{
			uint8_t push;
			uintptr_t pushaddr;
			uint8_t push2;
			uintptr_t useraddr;
			uint8_t push3;
			uintptr_t retaddr;
			uint8_t jmp;
			uintptr_t jmpaddr;
		};


		/*static inline void __declspec(naked) make_reg_pack_and_call()
	{
		_asm
		{
			pop retnptr
				pop usrcall
				pop camefromaddress
				// Construct the reg_pack structure on the stack
				pushad              // Pushes general purposes registers to reg_pack
				add[esp + 12], 4     // Add 4 to reg_pack::esp 'cuz of our return pointer, let it be as before this func is called
				pushfd              // Pushes EFLAGS to reg_pack

				// Call wrapper sending reg_pack as parameter
				push esp
				call callwrapper
				add esp, 4

				// Destructs the reg_pack from the stack
				sub[esp + 12 + 4], 4   // Fix reg_pack::esp before popping it (doesn't make a difference though) (+4 because eflags)
				popfd               // Warning: Do not use any instruction that changes EFLAGS after this (-> sub affects EF!! <-)
				popad

				// Back to normal flow
				ret
		}
	}*/
		struct newCode
		{
			uint8_t pushad; // pushad

			// add[esp + 12], 4 
			uint8_t add;
			uint32_t data;

			uint8_t pushfd; // pushfd

			uint8_t pushesp; // push esp

			uint8_t push;
			uintptr_t pushaddr;
			uint8_t push2;
			uintptr_t useraddr;
			uint8_t push3;
			uintptr_t retaddr;

			// call callwrapper
			uint8_t call;
			uint32_t addr;

			// add esp, 16
			uint16_t addesp;
			uint8_t addespnum;

			// sub[esp + 12 + 4], 4
			uint8_t sub;
			uint32_t data1;


			uint8_t popfd; // popfd
			uint8_t popad; // popad


			// retn x
			uint8_t retn;
			uint16_t retnsiz;
		};

	#pragma pack(pop)

	private:
		std::deque<code> codelist;

	public:
		void addhook(uintptr_t address, callback_t c)
		{
			if (std::none_of(codelist.begin(), codelist.end(), [&](const code& x) { return x.pushaddr == address; }))
			{
				auto it = codelist.emplace(codelist.end(), code{
					0x68, address,      // push address
					0x68, uintptr_t(c), // push c
					0x68, 0x00,         // push <retnaddr>
					0xE8, 0x00          // jmp <target>
				});

				if (it->retaddr = injector::MakeCALL(address, &(*it)).as_int())
					injector::MakeJMP(&it->jmp, make_reg_pack_and_call_with_return, false);
				else
					injector::MakeJMP(&it->jmp, make_reg_pack_and_call, false);
			}
		}

		static void genDynamicCall(newCode &myCode, uintptr_t address, callback_t c, uintptr_t popBytes)
		{
			myCode = newCode{
				0x60,				// pushad
				0x80, 0x040C2444u,  // add[esp + 12], 4 
				0x9C,				 // pushfd

				0x54,			   // push esp

				0x68, address,      // push address
				0x68, uintptr_t(c), // push c
				0x68, 0x00,         // push <retnaddr>

				0xE8, 0x00,			// call callwrapper
				0xC483, 0x10,		// add esp, 16
				0x80, 0x0410246Cu, // sub[esp + 12 + 4], 4
				0x9D,				// popfd
				0x61,				// popad
				0xC2, 0x00			// retn x
			};

			myCode.retnsiz = popBytes;
			injector::MakeCALL(&(myCode.call), callwrapper, false);
		}

		inline dynamic_hooker()
		{

		}

		inline ~dynamic_hooker()
		{

		}
	};


}

#endif


