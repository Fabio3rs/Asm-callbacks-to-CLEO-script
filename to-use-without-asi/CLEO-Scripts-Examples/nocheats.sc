SCRIPT_START
{
    NOP // 2 bytes
gambiarra:
    LVAR_INT init generateFunctionCallback
    GET_LABEL_POINTER _initCPPCode init
    
    CALL_FUNCTION init 0 0

    LVAR_INT label1, label2, function
    GET_LABEL_POINTER gambiarra label1
    GET_LABEL_POINTER nocheats label2

    label1 -= label2
    label1 -= 2

    CLEO_CALL generateFunctionCallback 0 label1 0 0   function

    LVAR_INT startCheatArray
    startCheatArray = 0x008A5B58

    WHILE startCheatArray < 0x008A5CC8
        WRITE_MEMORY startCheatArray 4 function 1
        startCheatArray += 4
    ENDWHILE

ad_infinitum:
    WAIT 0x7FFFFFFF
    GOTO ad_infinitum
}
SCRIPT_END

{
    LVAR_INT injector__regpackPointer // injector::reg_pack
    // Declare others parameters
nocheats:
    PRINT_STRING_NOW "Cheats nao sao permitidos" 5000
    WAIT 0
}

{
    LVAR_INT label, params, popbytes
    LVAR_INT callbackSizeFun callbackSize memory
    LVAR_INT generateFunction currentThreadPtr returnPointer
generateFunctionCallback: // {label, nparams, popbytes} -> Returns function address    
    GET_LABEL_POINTER _getCallBackSize  callbackSizeFun
    CALL_FUNCTION_RETURN callbackSizeFun 0 0  callbackSize
    ALLOCATE_MEMORY callbackSize memory
    GET_LABEL_POINTER _generateFunction  generateFunction
    GET_THIS_SCRIPT_STRUCT currentThreadPtr
    CALL_FUNCTION_RETURN generateFunction 5 5 popbytes params label currentThreadPtr memory  (returnPointer)
    CLEO_RETURN 0 returnPointer
}

{
DUMP
e8 01 00 00 00 c3 8b 04 24 83 e8 05 c3 55 31 c0 89 e5 8b 4d 08 53 8b 5d 0c 3b 45 10 74 09 8a 14 03 88 14 01 40 eb f2 5b 5d c3 55 89 e5 57 56 53 81 ec 0c 01 00 00 8b 5d 0c 8b 55 08 85 
db 0f 84 33 01 00 00 8b 83 e8 00 00 00 83 78 10 00 0f 84 23 01 00 00 8d b5 0c ff ff ff 31 c0 b9 02 00 00 00 89 f7 f3 ab 8d 85 1c ff ff ff c7 85 04 ff ff ff 00 00 00 00 8d 4d 9c c7 
85 08 ff ff ff 00 00 00 00 c7 85 14 ff ff ff 00 00 00 00 c7 85 18 ff ff ff 00 00 00 00 66 c7 85 3c ff ff ff 00 00 c6 45 c8 00 c6 45 c9 00 c6 45 ca 00 c6 45 cb 00 c6 45 cc 00 c6 45 
cd ff c7 45 d0 00 00 00 00 66 c7 45 d4 00 00 c6 45 d6 00 c6 45 d7 01 c6 45 d8 00 c7 45 dc 00 00 00 00 c6 45 e0 00 66 c7 45 e2 ff ff c6 45 e4 00 c7 00 00 00 00 00 83 c0 04 39 c8 75 
f3 8d 85 40 ff ff ff 8d 4d c0 c7 00 00 00 00 00 83 c0 04 39 c8 75 f3 c7 45 c4 00 00 00 00 8d 7b 04 b9 e1 00 00 00 c7 45 c0 00 00 00 00 8d b5 04 ff ff ff f3 a4 8b 83 e8 00 00 00 8b 
40 10 c6 83 cb 00 00 00 01 c6 83 c8 00 00 00 01 89 53 40 89 43 14 8b 83 ec 00 00 00 89 43 18 8b 83 f0 00 00 00 c1 e0 02 89 44 24 08 8b 42 10 89 44 24 04 8d 43 44 83 c3 04 89 04 24 
e8 9f fe ff ff 89 1c 24 ff 15 09 03 00 00 81 c4 0c 01 00 00 5b 5e 5f 5d c3 
ENDDUMP

_getCallBackSize:
DUMP
55 b8 20 01 00 00 89 e5 5d c3 
ENDDUMP

_generateFunction: // (cppToCleoCallback *nCallback, CRunningScript *owner, int atLabel, int nparams, int popBytes) -> Returns function address
DUMP
55 89 e5 57 56 53 83 ec 4c 8b 5d 08 8b 45 10 8b 4d 14 8b 55 0c 8d bb f4 00 00 00 89 7d c4 8d bb 0b 01 00 00 89 8b f0 00 00 00 89 c1 c1 f9 1f 31 c8 29 c8 03 42 10 89 1b 89 93 e8 00 00 
00 89 83 ec 00 00 00 c6 83 f4 00 00 00 60 c6 83 f5 00 00 00 80 c7 83 f6 00 00 00 44 24 0c 04 c6 83 fa 00 00 00 9c c6 83 fb 00 00 00 54 c6 83 fc 00 00 00 68 89 9b fd 00 00 00 c6 83 
01 01 00 00 68 c7 83 02 01 00 00 2a 00 00 00 c6 83 06 01 00 00 68 c7 83 07 01 00 00 00 00 00 00 c6 83 0b 01 00 00 e8 c7 83 0c 01 00 00 00 00 00 00 66 c7 83 10 01 00 00 83 c4 c6 83 
12 01 00 00 10 c6 83 13 01 00 00 80 c7 83 14 01 00 00 6c 24 10 04 c6 83 18 01 00 00 9d c6 83 19 01 00 00 61 8b 45 18 80 3d 11 05 00 00 00 c6 83 1a 01 00 00 c2 66 89 83 1b 01 00 00 
75 07 c6 05 11 05 00 00 01 8d 75 d8 8d 45 d0 89 74 24 04 89 04 24 c7 44 24 08 00 00 00 00 89 7d d8 e8 7e 01 00 00 8d 45 d4 89 f1 89 04 24 c7 44 24 04 00 00 00 00 89 45 c0 89 7d d4 
e8 9b 00 00 00 89 f1 50 50 c6 83 0b 01 00 00 e8 bb 08 03 00 00 e8 da 00 00 00 8b 45 c0 8d 57 01 89 f1 29 fb c7 44 24 04 00 00 00 00 89 55 d4 89 04 24 e8 69 00 00 00 89 f1 50 50 89 
5f 01 e8 b1 00 00 00 8b 45 c4 8d 65 f4 5b 5e 5f 5d c3 90 90 25 03 00 00 55 89 e5 8b 55 10 8b 45 0c 89 55 0c 8b 55 14 89 55 08 5d ff e0 90 90 90 83 3d 0d 05 00 00 00 55 89 e5 8b 4d 
08 75 1a 80 3d 11 05 00 00 00 75 07 c6 05 11 05 00 00 01 c7 05 0d 05 00 00 00 9f 46 00 a1 0d 05 00 00 5d ff e0 90 90 90 55 89 e5 53 89 cb 83 ec 14 8b 45 0c c7 01 00 00 00 00 85 c0 
75 06 c6 41 0c 00 eb 30 8b 55 08 8d 49 08 89 41 fc 8b 12 89 51 f8 89 4c 24 0c c7 44 24 08 40 00 00 00 89 44 24 04 89 14 24 ff 15 51 07 00 00 83 ec 10 85 c0 0f 95 43 0c 8b 5d fc c9 
c2 08 00 90 80 79 0c 00 74 33 55 89 e5 53 83 ec 24 8b 51 04 8b 41 08 89 d3 8b 11 8d 4d f4 89 45 f4 89 5c 24 04 89 4c 24 0c 89 44 24 08 89 14 24 ff 15 51 07 00 00 8b 5d fc 83 ec 10 
c9 c3 90 90 55 89 e5 53 83 ec 34 8b 45 08 8d 4d e8 8b 18 0f b6 45 0c 89 5d e4 89 44 24 04 8d 45 e4 89 04 24 e8 4b ff ff ff 8d 4d e8 50 50 8a 1b e8 93 ff ff ff 88 d8 8b 5d fc c9 c3 
55 89 e5 57 56 53 83 ec 3c 8b 45 10 8b 5d 0c 8d 75 d8 8b 7d 08 0f b6 c8 89 45 c4 8b 03 89 4c 24 04 89 34 24 89 4d c0 89 45 d8 e8 99 ff ff ff 3c e8 0f 82 a3 00 00 00 3c e9 76 0c fe 
c0 8b 4d c0 74 42 e9 93 00 00 00 8b 1b 80 7d c4 01 8d 43 05 89 45 c0 19 c0 f7 d0 83 e0 04 8d 4b 01 89 44 24 04 8d 45 d4 89 4d d4 89 f1 89 04 24 e8 c7 fe ff ff 51 51 89 f1 8b 5b 01 
e8 0f ff ff ff 03 5d c0 89 1f eb 5c 8b 03 89 4c 24 04 89 34 24 40 89 45 d8 e8 32 ff ff ff 3c 15 74 04 3c 25 75 3c 80 7d c4 01 8b 13 19 c0 f7 d0 83 e0 04 8d 4a 02 89 44 24 04 8d 45 
d4 89 04 24 89 4d d4 89 f1 89 55 c0 e8 73 fe ff ff 89 f1 52 52 8b 55 c0 8b 5a 02 e8 b8 fe ff ff 8b 03 89 07 eb 06 c7 07 00 00 00 00 8d 65 f4 89 f8 5b 5e 5f 5d c3 90 90 00 00 00 00 
00 00 00 00 00 00 00 00 47 43 43 3a 20 28 47 4e 55 29 20 34 2e 38 2e 31 00 00 00 00 14 00 00 00 00 00 00 00 01 7a 52 00 01 7c 08 01 1b 0c 04 04 88 01 00 00 1c 00 00 00 1c 00 00 00 
bc fd ff ff 15 00 00 00 00 41 0e 08 85 02 42 0d 05 50 c5 0c 04 04 00 00 14 00 00 00 00 00 00 00 01 7a 52 00 01 7c 08 01 1b 0c 04 04 88 01 00 00 1c 00 00 00 1c 00 00 00 9c fd ff ff 
31 00 00 00 00 48 0e 08 85 02 42 0d 05 65 c5 0c 04 04 00 00 14 00 00 00 00 00 00 00 01 7a 52 00 01 7c 08 01 1b 0c 04 04 88 01 00 00 20 00 00 00 1c 00 00 00 98 fd ff ff 53 00 00 00 
00 41 0e 08 85 02 42 0d 05 41 83 03 02 4c c5 c3 0c 04 04 00 14 00 00 00 00 00 00 00 01 7a 52 00 01 7c 08 01 1b 0c 04 04 88 01 00 00 20 00 00 00 1c 00 00 00 b0 fd ff ff 3a 00 00 00 
00 47 0e 08 85 02 42 0d 05 44 83 03 6c c5 c3 0c 04 04 00 00 14 00 00 00 00 00 00 00 01 7a 52 00 01 7c 08 01 1b 0c 04 04 88 01 00 00 20 00 00 00 1c 00 00 00 d4 f9 ff ff 1d 00 00 00 
00 41 0e 08 85 02 44 0d 05 44 83 03 52 c3 41 c5 0c 04 04 00 2c 00 00 00 40 00 00 00 b0 f9 ff ff 58 01 00 00 00 41 0e 08 85 02 42 0d 05 49 87 03 86 04 83 05 03 48 01 c3 41 c6 41 c7 
41 c5 0c 04 04 00 00 00 1c 00 00 00 70 00 00 00 80 f9 ff ff 0a 00 00 00 00 41 0e 08 85 02 47 0d 05 41 c5 0c 04 04 00 00 2c 00 00 00 90 00 00 00 60 f9 ff ff 7b 01 00 00 00 41 0e 08 
85 02 42 0d 05 46 87 03 86 04 83 05 03 6e 01 c3 41 c6 41 c7 41 c5 0c 04 04 00 00 00 14 00 00 00 00 00 00 00 01 7a 52 00 01 7c 08 01 1b 0c 04 04 88 01 00 00 20 00 00 00 1c 00 00 00 
f4 fc ff ff 38 00 00 00 00 41 0e 08 85 02 42 0d 05 44 83 03 70 c5 c3 0c 04 04 00 00 14 00 00 00 00 00 00 00 01 7a 52 00 01 7c 08 01 1b 0c 04 04 88 01 00 00 28 00 00 00 1c 00 00 00 
f0 fc ff ff ea 00 00 00 00 41 0e 08 85 02 42 0d 05 46 87 03 86 04 83 05 02 dd c3 41 c6 41 c7 41 c5 0c 04 04 2c 80 85 00 55 89 e5 83 ec 20 8b 45 08 89 45 f8 81 45 f8 bf 07 00 00 8b 
45 f8 89 45 f4 c7 45 f0 0d 00 00 00 c7 45 fc 00 00 00 00 eb 37 8b 45 fc 8d 14 c5 00 00 00 00 8b 45 f4 01 d0 89 45 ec 8b 45 ec 8b 00 89 c2 8b 45 08 01 d0 89 45 e8 8b 45 ec 8b 40 04 
89 c2 8b 45 08 01 c2 8b 45 e8 89 10 83 45 fc 01 8b 45 fc 3b 45 f0 7c c1 c9 c3 73 01 00 00 09 03 00 00 10 02 00 00 2a 00 00 00 6a 02 00 00 11 05 00 00 81 02 00 00 11 05 00 00 ca 02 
00 00 08 03 00 00 09 03 00 00 25 03 00 00 27 03 00 00 0d 05 00 00 36 03 00 00 11 05 00 00 3f 03 00 00 11 05 00 00 46 03 00 00 0d 05 00 00 4f 03 00 00 0d 05 00 00 98 03 00 00 51 07 
00 00 db 03 00 00 51 07 00 00 90 90 90 90 90 90 90 90 
ENDDUMP

_initCPPCode:
DUMP
60 9c e8 ca f7 ff ff 50 e8 19 ff ff ff 83 c4 04 9d 61 c3 
ENDDUMP
}