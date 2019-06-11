//
// Created by Jesus Lopez on 4/22/19.
//
#include <android/log.h>
#include <sys/mman.h>
#include <cerrno>
#include <link.h>
#include <strstream>
#include <fstream>
#include <unistd.h>

#include "Protego.h"

namespace {

bool applied = false;
bool apply();

}

namespace Protego {
    bool cast() {
        if (applied)
            return false;
        apply(); // return value is "is device affected?", throw on the floor
        applied = true;
        return applied;
    }
}

namespace {

const char *LOGGING_TAG = "Protego";
const char *LIBRARY_NAME = "/vendor/lib/libllvm-glnext.so";
const int SECTION_VIRTUAL_ADDRESS = 0x1e000;
const int SECTION_SIZE = 0xacc8b0;
const int OFFSET_1 = 0x7f9450;
const int OFFSET_2 = 0x7f945A;
const int INSTR_1 = 0x20d0f8d0;
const int INSTR_2 = 0x20d0f8c0;


static int getSharedLibraryBaseCallback(struct dl_phdr_info *info, size_t size, void *data) {
    auto out = reinterpret_cast<dl_phdr_info *>(data);
    if (strcmp(info->dlpi_name, out->dlpi_name) != 0)
        return 0;
    *out = *info;
    return 1;
}

bool getSharedLibraryBase(const char *libraryName, dl_phdr_info &out) {
    out.dlpi_name = libraryName;
    auto result = dl_iterate_phdr(getSharedLibraryBaseCallback, (void *)&out) != 0;
    return static_cast<bool>(result);
}

inline uint32_t peek32(uint32_t addr) {
    return *reinterpret_cast<uint32_t *>(addr);
}

inline void poke32(uint32_t addr, uint32_t data) {
    *reinterpret_cast<uint32_t *>(addr) = data;
}

inline void log(android_LogPriority priority, const char *fmt, va_list ap) {
    __android_log_vprint(priority, LOGGING_TAG, fmt, ap);
}

inline void info(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log(ANDROID_LOG_INFO, fmt, ap);
    va_end(ap);
}

inline void warn(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log(ANDROID_LOG_INFO, fmt, ap);
    va_end(ap);
}

inline void error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    log(ANDROID_LOG_ERROR, fmt, ap);
    va_end(ap);
}

bool apply() {
    // Locate the problematic library
    struct dl_phdr_info libInfo{};
    if (!getSharedLibraryBase(LIBRARY_NAME, libInfo))
        return false;

    // Ensure we got the right library which has 8 program headers
    if (libInfo.dlpi_phnum != 8)
        return false;

    // Ensure we got the right program header by matching the expected sizes
    const Elf32_Phdr *section = libInfo.dlpi_phdr + 1;
    if (section->p_vaddr != SECTION_VIRTUAL_ADDRESS || section->p_memsz != SECTION_SIZE)
        return false;

    // Ensure the bytes to patch are the expected instructions
    uint32_t baseAddress = libInfo.dlpi_addr;
    uint32_t d1 = peek32(baseAddress + OFFSET_1);
    uint32_t d2 = peek32(baseAddress + OFFSET_2);
    if (d1 != INSTR_1 || d2 != INSTR_2)
        return false;

    // Determine the memory region to unprotect
    uint32_t regionStart = baseAddress + libInfo.dlpi_phdr[1].p_vaddr;
    uint32_t regionSize = libInfo.dlpi_phdr[1].p_memsz;

    // Allow writes to the region so we can patch
    int st = mprotect(reinterpret_cast<void *>(regionStart), regionSize, PROT_EXEC | PROT_WRITE | PROT_READ); // NOLINT(hicpp-signed-bitwise)
    if (st == -1) {
        error("Unable to change memory protection [0x%08x-0x%08x]: %d %s",
              regionStart, regionStart + regionSize, errno, strerror(errno));
        return false;
    }

    // Patch those bytes
    poke32(baseAddress + OFFSET_1, d1 | 4u);
    poke32(baseAddress + OFFSET_2, d2 | 4u);

    // Revert the protection to read-only
    st = mprotect(reinterpret_cast<void *>(regionStart), regionSize, PROT_EXEC | PROT_READ); // NOLINT(hicpp-signed-bitwise)
    if (st == -1) {
        warn("Unable to restore memory protection [0x%08x-0x%08x]: %d %s",
             regionStart, regionStart + regionSize, errno, strerror(errno));
        return false;
    }

    // We're done!
    info("Patched buggy %s at 0x%08x [0x%08x-0x%08x]", LIBRARY_NAME, baseAddress,
         regionStart, regionStart + regionSize);
    return true;
}

}