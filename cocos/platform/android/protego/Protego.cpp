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

bool getSharedLibraryBase(const char *libraryName, dl_phdr_info &out) {
    // Unfortunately, android 19 does not have dl_iterate_phdr,
    // so we have to resort to good ol' proc parsing.

    // Read the /proc/PID/maps file which has the following format:
    // address           perms offset  dev   inode   pathname
    // 08048000-08056000 r-xp 00000000 03:0c 64593   /usr/sbin/gpm
    std::ostrstream os;
    os << "/proc/" << getpid() << "/maps" << std::ends;
    std::ifstream infile(os.str());

    // Parse line by line
    std::string line;
    while (std::getline(infile, line)) {
        uint32_t sa, ea, ofs, inode;
        char perms[16], dev[16], name[256];
        int items = sscanf(line.c_str(), "%x-%x %s %x %s %d %s", &sa, &ea, perms, &ofs, dev, &inode, name); // NOLINT(cert-err34-c)

        // If we parsed all 7 items, the region is executable and pathname matches we got it
        if (items == 7 && perms[2] == 'x' && strcmp(name, libraryName) == 0) {
            // Interpret the starting address as an ELF header
            auto *header = reinterpret_cast<const Elf32_Ehdr *>(sa);

            // Extract the program header information and return the newly concocted structure
            out.dlpi_phdr = reinterpret_cast<const Elf32_Phdr *>(sa + header->e_phoff);
            out.dlpi_phnum = header->e_phnum;
            out.dlpi_name = libraryName;
            out.dlpi_addr = sa - (out.dlpi_phdr[0].p_vaddr & ~0xFFFu);
            return true;
        }
    }
    return false;
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