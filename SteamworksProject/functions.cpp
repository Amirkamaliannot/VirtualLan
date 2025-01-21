#include "functions.h"


Steam steam;
std::wstring steamIP = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(steam.getIP());
WintunManager wintunManager(L"AmirVPN", steamIP, L"255.0.0.0");

void callbackLitentToInterface(BYTE* packet, DWORD size)
{
    Packet temp = Packet(packet, false, size);
    string dst_ip = temp.dst_ip;
    CSteamID steamID = steam.getUserbyIP(dst_ip);

    if (steamID != k_steamIDNil) {
        steam.SendDataToUser(steamID, packet, size);
    }
}
void callbackLiteningToSteam(BYTE* packet, DWORD size)
{
    wintunManager.sendPacket(packet, size);
};


Data compressZlib(BYTE* input, DWORD inputSize) {
    //std::cout << "Uncompressed size: " << inputSize << " bytes\n";

    // Calculate maximum compressed size
    uLong maxCompressedSize = compressBound(inputSize);

    // Allocate memory for compressed data
    BYTE* compressedData = new BYTE[maxCompressedSize];

    // Compress the data using zlib
    uLong compressedSize = maxCompressedSize;
    int compressResult = compress(
        compressedData, &compressedSize,               // Output buffer and size
        reinterpret_cast<const Bytef*>(input), inputSize // Input data
    );

    if (compressResult == Z_OK) {
        //std::cout << "Compressed size: " << compressedSize << " bytes\n";
    }
    else {
        std::cerr << "Compression failed with error code: " << compressResult << std::endl;
        // Clean up and return an empty compressData object
        delete[] compressedData;
        return { nullptr, 0 };
    }

    // Return the compressed data and its size
    return Data{ compressedData, (int)compressedSize };
}


Data DecompressZlib(BYTE* compressedData, int compressedSize) {
    // Allocate memory for decompressed data

    int maxDecompressedSize = 1500;
    BYTE* decompressedData = new BYTE[maxDecompressedSize];
    uLong decompressedSize = maxDecompressedSize;

    // Decompress the data using zlib
    int decompressResult = uncompress(
        reinterpret_cast<Bytef*>(decompressedData), &decompressedSize, // Output buffer and size
        reinterpret_cast<const Bytef*>(compressedData), compressedSize // Compressed data
    );

    if (decompressResult == Z_OK) {
        //std::cout << "Decompression successful! Decompressed size: " << decompressedSize << "\n";
    }
    else if (decompressResult == Z_BUF_ERROR) {
        std::cerr << "Decompression failed: Output buffer too small.\n";
    }
    else if (decompressResult == Z_DATA_ERROR) {
        std::cerr << "Decompression failed: Data is corrupted or invalid.\n";
    }
    else {
        std::cerr << "Decompression failed with unknown error code: " << decompressResult << "\n";
    }

    return Data{ decompressedData, (decompressResult == Z_OK) ? (int)decompressedSize : 0 };
}

;
