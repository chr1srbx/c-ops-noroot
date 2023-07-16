#ifndef UTILS_H
#define UTILS_H
#include "../Hooking/dobby.h"
#include "../Memory/MemoryPatch.h"
#include "../Unity/Unity.h"

typedef unsigned long DWORD;
static DWORD libBase;

DWORD findLibrary(const char *library);
DWORD getAbsoluteAddress(const char* libraryName, DWORD relativeAddr);
DWORD getRealOffset(DWORD offset);
bool isLibraryLoaded(const char *libraryName);
void hook(void *orig_fcn, void* new_fcn, void **orig_fcn_ptr);

DWORD findLibrary(const char *library)
{
    char filename[0xFF] = {0},
            buffer[1024] = {0};
    FILE *fp = NULL;
    DWORD address = 0;
    const char *selfmaps = OBFUSCATE("/proc/self/maps");
    sprintf(filename, OBFUSCATE("%s"), selfmaps);
    fp = fopen(filename, OBFUSCATE("rt"));
    if (fp == NULL)
    {
        perror(OBFUSCATE("fopen"));
        goto done;
    }

    while (fgets(buffer, sizeof(buffer), fp))
    {
        if (strstr(buffer, library))
        {
            address = (DWORD) strtoul(buffer, NULL, 16);
            goto done;
        }
    }

    done:

    if (fp)
    {
        fclose(fp);
    }

    return address;
}
DWORD getAbsoluteAddress(const char* libraryName, DWORD relativeAddr)
{
    if (libBase == 0)
        libBase = findLibrary(libraryName);
    if (libBase != 0)
        return (reinterpret_cast<DWORD>(libBase + relativeAddr));
    else
        return 0;
}
DWORD getRealOffset(DWORD offset)
{
    if (libBase == 0)
        libBase = findLibrary(OBFUSCATE("libil2cpp.so"));
    if (libBase != 0)
        return (reinterpret_cast<DWORD>(libBase + offset));
    else
        return 0;
}

bool isLibraryLoaded(const char *libraryName)
{
    char line[512] = {0};
    FILE *fp = fopen(OBFUSCATE("/proc/self/maps"), OBFUSCATE("rt"));
    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp))
        {
            if (strstr(line, libraryName))
                return true;
        }
        fclose(fp);
    }
    return false;
}

std::vector<MemoryPatch> memoryPatches;
std::vector<uint64_t> offsetVector;

void patchOffset(const char *fileName, uint64_t offset, std::string hexBytes, bool isOn) {

    MemoryPatch patch = MemoryPatch::createWithHex(fileName, offset, hexBytes);
    if (std::find(offsetVector.begin(), offsetVector.end(), offset) != offsetVector.end()) {
        std::vector<uint64_t>::iterator itr = std::find(offsetVector.begin(), offsetVector.end(), offset);
        patch = memoryPatches[std::distance(offsetVector.begin(), itr)]; //Get index of memoryPatches vector
    } else {
        memoryPatches.push_back(patch);
        offsetVector.push_back(offset);
    }
}

uintptr_t string2Offset(const char *c)
{
    int base = 16;
    // See if this function catches all possibilities.
    // If it doesn't, the function would have to be amended
    // whenever you add a combination of architecture and
    // compiler that is not yet addressed.
    static_assert(sizeof(uintptr_t) == sizeof(unsigned long)
                  || sizeof(uintptr_t) == sizeof(unsigned long long),
                  "");

    // Now choose the correct function ...
    if (sizeof(uintptr_t) == sizeof(unsigned long))
    {
        return strtoul(c, nullptr, base);
    }

    // All other options exhausted, sizeof(uintptr_t) == sizeof(unsigned long long))
    return strtoull(c, nullptr, base);
}

std::string random_string(const int len) {
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return tmp_s;
}


#define PATCH(offset, hex) patchOffset(OBFUSCATE("libil2cpp.so"), string2Offset(offset), std::string(OBFUSCATE(hex)), true)
#define hook(offset, newfunc, orig) DobbyHook((void*) getRealOffset(string2Offset(offset)), (void*)newfunc, (void**)&orig)
#define hookStatic(offset, ptr) DobbyHook((void *)getRealOffset(string2Offset(OBFUSCATE(offset))), (void *)ptr, nullptr)
std::string readTextFile(std::string path)
{
    FILE    *textfile;
    char    *text;
    long    numbytes;
    textfile = fopen(path.c_str(), "r");
    if(textfile == NULL)
        return std::string(OBFUSCATE("ERROR"));

    fseek(textfile, 0L, SEEK_END);
    numbytes = ftell(textfile);
    fseek(textfile, 0L, SEEK_SET);

    text = (char*)calloc(numbytes, sizeof(char));
    if(text == NULL)
        return std::string(OBFUSCATE("ERROR"));

    fread(text, sizeof(char), numbytes, textfile);
    fclose(textfile);
    return text;
}
#endif
