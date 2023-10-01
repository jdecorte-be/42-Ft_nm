#include "../ft_nm.h"
#include <arpa/inet.h> // For network byte order functions
#include <endian.h>    // For byte order macros

int format_check64(char *file_data, Elf64_Ehdr *elf_header, struct stat fd_info)
{
    if (!elf_header)
        return format_error("Invalid ELF header1");
    // check machine version
    if (elf_header->e_machine == EM_NONE)
        return format_error("Architecture not handled");

    // check header max size
    if (fd_info.st_size <= sizeof(Elf64_Ehdr))
        return format_error("Symbol table or string table not found");
    if (elf_header->e_ident[EI_CLASS] != ELFCLASS64 && elf_header->e_ident[EI_DATA] != ELFDATA2LSB)
    {
        if (elf_header->e_type != ET_EXEC && elf_header->e_type != ET_DYN)
            return format_error("Symbol table or string table not found");
    }
    // check if e_shnum is within bounds
    if (elf_header->e_shnum >= SHN_LORESERVE)
        return format_error("Too many sections");
    return 0;
}

int nm_symbol_compare(const void* a, const void* b) {
    const char* symbol1 = *(const char**)a;
    const char* symbol2 = *(const char**)b;

    // Compare symbols based on ASCII values
    return strcmp(symbol1, symbol2);
}


int handle64_symtab(Elf64_Shdr *section_h, Elf64_Ehdr *elf_header, char *file_data, int i)
{
    uint64_t sh_name, sh_size, sh_offset, sh_link, st_info;
    Elf64_Sym *symtab = NULL;
    size_t symtab_size = 0;
    char *strtab = NULL;

    sh_name = read_uint64(section_h[i].sh_name, file_data);
    sh_size = read_uint64(section_h[i].sh_size, file_data);
    sh_link = read_uint64(section_h[i].sh_link, file_data);
    sh_offset = read_uint64(section_h[i].sh_offset, file_data);

    symtab = (Elf64_Sym *)(file_data + sh_offset);
    symtab_size = sh_size / sizeof(Elf64_Sym);
    strtab = file_data + read_uint64(section_h[sh_link].sh_offset, file_data);

    t_sym *tab = malloc(sizeof(t_sym) * symtab_size);
    if (!tab)
        return format_error("Memory allocation failed");

    size_t tab_size = 0;
    for (size_t i = 0; i < symtab_size; i++)
    {
        uint64_t info = ELF64_ST_TYPE(symtab[i].st_info);
        if (info == STT_FUNC || info == STT_OBJECT || info == STT_NOTYPE)
        {
            tab[tab_size].addr = read_uint64(symtab[i].st_value, file_data);
            tab[tab_size].letter = elf64_symbols(symtab[i], section_h, file_data, elf_header);
            tab[tab_size].name = strtab + read_uint64(symtab[i].st_name, file_data);
            tab_size++;
        }
    }

    qsort(tab, tab_size, sizeof(t_sym), nm_symbol_compare);

    for (int i = 1; i < tab_size; i++)
    {
        if (tab[i].addr)
            printf("%016lx %c", tab[i].addr, tab[i].letter);
        else
            printf("                 %c", tab[i].letter);
        printf(" %s\n", tab[i].name);
    }
    free(tab);
}

int handle64(char *file_data, Elf64_Ehdr *elf_header, struct stat fd_info)
{

    uint64_t sh_type;

    if (format_check64(file_data, elf_header, fd_info) < 0)
        return -1;

    uint64_t e_shoff = read_uint64(elf_header->e_shoff, file_data);
    uint64_t e_shnum = read_uint16(elf_header->e_shnum, file_data);

    if (e_shoff > fd_info.st_size)
        return format_error("Offset bigger than file size");

    Elf64_Shdr *section_h = (Elf64_Shdr *)(file_data + e_shoff);

    // Find the symbol table and associated string table
    for (uint64_t i = 0; i < e_shnum; i++)
    {
        if (i >= SHN_LORESERVE)
            break;
        sh_type = read_uint64(section_h[i].sh_type, file_data);
        if (sh_type == SHT_SYMTAB)
            return handle64_symtab(section_h, elf_header, file_data, i);
    }
    return format_error("Symbol table or string table not found");
}
