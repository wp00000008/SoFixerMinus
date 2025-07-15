//===------------------------------------------------------------*- C++ -*-===//
//
//                     Created by F8LEFT on 2017/6/4.
//                   Copyright (c) 2017. All rights reserved.
//===----------------------------------------------------------------------===//
// Rebuild elf file with ElfReader
//===----------------------------------------------------------------------===//

#ifndef SOFIXER_ELFREBUILDER_H
#define SOFIXER_ELFREBUILDER_H

#include <cstdint>
#include <vector>
#include <string>
#include "ObElfReader.h"



#define SOINFO_NAME_LEN 128
struct soinfo {
public:
    const char* name = NULL;  // jack add
    uint64_t name_index = -1;  // jack add
    const Elf_Phdr* phdr = nullptr;
    size_t phnum = 0;
    Elf_Addr entry = 0;
    uint8_t * base = 0;
    unsigned size = 0;

    Elf_Addr min_load;
    Elf_Addr max_load;

    uint32_t unused1 = 0;  // DO NOT USE, maintained for compatibility.

    Elf_Dyn* dynamic = nullptr;
    size_t dynamic_count = 0;
    Elf_Word dynamic_flags = 0;

    uint32_t unused2 = 0; // DO NOT USE, maintained for compatibility
    uint32_t unused3 = 0; // DO NOT USE, maintained for compatibility

    unsigned flags = 0;

    const char* strtab = nullptr;
    Elf_Sym* symtab = nullptr;
    size_t sym_ent = 0; // jack add

    uint8_t * hash = 0;
    size_t strtabsize = 0;
    size_t nbucket = 0;
    size_t nchain = 0;
    unsigned* bucket = nullptr;
    unsigned* chain = nullptr;

    Elf_Addr * plt_got = nullptr;

    uint32_t plt_type = 0;// DT_REL; // jack add
    Elf_Rel* plt = nullptr;  // jack add
    size_t plt_count = 0;  // jack add
    size_t plt_size = 0; // jack add
    
    Elf_Rela* rela = nullptr;  // jack add
    size_t rela_count = 0;  // jack add

    Elf_Rel* rel = nullptr;
    size_t rel_count = 0;

    void* preinit_array = nullptr;
    size_t preinit_array_count = 0;

    void** init_array = nullptr;
    size_t init_array_count = 0;
    void** fini_array = nullptr;
    size_t fini_array_count = 0;

    void* init_func = nullptr;
    void* fini_func = nullptr;

    // ARM EABI section used for stack unwinding.
    Elf_Addr * ARM_exidx = nullptr;
    size_t ARM_exidx_count = 0;
    unsigned mips_symtabno = 0;
    unsigned mips_local_gotno = 0;
    unsigned mips_gotsym = 0;

    // When you read a virtual address from the ELF file, add this
    // value to get the corresponding address in the process' address space.
    uint8_t * load_bias = nullptr;

    bool has_text_relocations = false;
    bool has_DT_SYMBOLIC = false;
};


class ElfRebuilder {
public:
    ElfRebuilder(ObElfReader* elf_reader);
    ~ElfRebuilder() { if(rebuild_data != nullptr) delete []rebuild_data; }
    bool Rebuild();

    void* getRebuildData() { return rebuild_data; }
    size_t getRebuildSize() { return rebuild_size; }
private:
    bool RebuildPhdr();
    bool RebuildShdr();
    bool ReadSoInfo();
    bool RebuildRelocs();
    bool RebuildFin();

  template <bool isRela>
  void relocate(uint8_t * base, Elf_Rel* rel, Elf_Addr dump_base);
    ObElfReader* elf_reader_;
    soinfo si;

    int rebuild_size = 0;
    uint8_t * rebuild_data = nullptr;

    Elf_Word sDYNSYM = 0;
    Elf_Word sDYNSTR = 0;
    Elf_Word sHASH = 0;
    Elf_Word sRELDYN = 0;
    Elf_Word sRELADYN = 0;
    Elf_Word sRELPLT = 0;
    Elf_Word sPLT = 0;
    Elf_Word sTEXTTAB = 0;
    Elf_Word sARMEXIDX = 0;
    Elf_Word sFINIARRAY = 0;
    Elf_Word sINITARRAY = 0;
    Elf_Word sDYNAMIC = 0;
    Elf_Word sGOT = 0;
    Elf_Word sDATA = 0;
    Elf_Word sBSS = 0;
    Elf_Word sSHSTRTAB = 0;

    std::vector<Elf_Shdr> shdrs;
    std::string shstrtab;

    unsigned external_pointer = 0;
private:
    bool isPatchInit = false;
public:
    void setPatchInit(bool b) { isPatchInit = b; }
};


#endif //SOFIXER_ELFREBUILDER_H
