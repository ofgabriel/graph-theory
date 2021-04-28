#pragma once

#if defined(__linux__)

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#elif defined(_WIN32)

#include "windows.h"
#include "psapi.h"

#endif

/*
*   Returns the memory usage from this process in KB
*/
int getVirtualMemory();

#if defined(__linux__)

int parseLine(char* line) {
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

int getVirtualMemory() {
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL) {
        if (strncmp(line, "VmSize:", 7) == 0) {
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

#elif defined(_WIN32)

int getVirtualMemory() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
    return virtualMemUsedByMe / 1000;
}

#elif defined(__APPLE__)

int getVirtualMemory() {
    return -1;
}

#endif 