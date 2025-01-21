#pragma once
#include <iostream>
#include <steam/isteamnetworkingsockets.h>
#include <steam/isteamnetworkingmessages.h>
#include <steam/steam_api.h>
#include <thread>
#include <chrono>
#include <iomanip>
#include <winsock2.h> 
#include <windows.h>
#include "steam.h"
#include "SteamP2PConnection.h"
#include "winperf.h"
//#include "wincap.h"
#include <thread>
#include <vector>
#include "Packet.h"
#include "wintun.h"
#include <codecvt>
#include <iphlpapi.h>
#include <unordered_map>
#include "WintunManager.h"
#include <zlib.h> 

struct Data
{
    BYTE* data;
    int size;
};

extern Steam steam;
extern std::wstring steamIP;
extern WintunManager wintunManager;


void callbackLitentToInterface(BYTE* packet, DWORD size);
void callbackLiteningToSteam(BYTE* packet, DWORD size);

Data compressZlib(BYTE* input, DWORD inputSize);
Data DecompressZlib(BYTE* compressedData, int compressedSize);
