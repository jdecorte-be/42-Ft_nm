#include "../ft_nm.h"
#include <arpa/inet.h> // For network byte order functions
#include <endian.h>    // For byte order macros

int format_check32(char *file_data, Elf32_Ehdr *elf_header, struct stat fd_info)
{
    if (!elf_header)
        return format_error("Invalid ELF header1");
    // check machine version
    if (elf_header->e_machine == EM_NONE)
        return format_error("Architecture not handled");

    // check header max size
    if (fd_info.st_size <= sizeof(Elf32_Ehdr))
        return format_error("Symbol table or string table not found");
    if (elf_header->e_ident[EI_CLASS] != ELFCLASS32 && elf_header->e_ident[EI_DATA] != ELFDATA2LSB)
    {
        if (elf_header->e_type != ET_EXEC && elf_header->e_type != ET_DYN)
            return format_error("Symbol table or string table not found");
    }
    // check if e_shnum is within bounds
    if (elf_header->e_shnum >= SHN_LORESERVE)
        return format_error("Too many sections");
    return 0;
}

int handle32_symtab(Elf32_Shdr *section_h, Elf32_Ehdr *elf_header, char *file_data, int i)
{
    uint32_t sh_name, sh_size, sh_offset, sh_link, st_info;
    Elf32_Sym *symtab = NULL;
    size_t symtab_size = 0;
    char *strtab = NULL;

    sh_name = read_uint32(section_h[i].sh_name, file_data);
    sh_size = read_uint32(section_h[i].sh_size, file_data);
    sh_link = read_uint32(section_h[i].sh_link, file_data);
    sh_offset = read_uint32(section_h[i].sh_offset, file_data);

    symtab = (Elf32_Sym *)(file_data + sh_offset);
    symtab_size = sh_size / sizeof(Elf32_Sym);
    strtab = file_data + read_uint32(section_h[sh_link].sh_offset, file_data);

    t_sym *tab = malloc(sizeof(t_sym) * symtab_size);
    if (!tab)
        return format_error("Memory allocation failed");

    int tab_size = 0;
    for (int i = 0; i < symtab_size; i++)
    {
        uint32_t info = ELF32_ST_TYPE(symtab[i].st_info);
        if (info == STT_FUNC || info == STT_OBJECT || info == STT_NOTYPE)
        {
            tab[tab_size].addr = read_uint32(symtab[i].st_value, file_data);
            tab[tab_size].letter = elf32_symbols(symtab[i], section_h, file_data);
            tab[tab_size].name = strtab + read_uint32(symtab[i].st_name, file_data);
            tab_size++;
        }
    }

    // ascii_sort(tab, tab_size);

    for (int i = 0; i < tab_size; i++)
    {
        if (tab[i].addr)
            printf("%016lx %c", tab[i].addr, tab[i].letter);
        else
            printf("                 %c", tab[i].letter);
        printf(" %s\n", tab[i].name);
    }
    free(tab);
}

int handle32(char *file_data, Elf32_Ehdr *elf_header, struct stat fd_info)
{

    uint32_t sh_type;

    if (format_check32(file_data, elf_header, fd_info) < 0)
        return -1;

    uint32_t e_shoff = read_uint32(elf_header->e_shoff, file_data);
    uint32_t e_shnum = read_uint16(elf_header->e_shnum, file_data);

    if (e_shoff > fd_info.st_size)
        return format_error("Offset bigger than file size");

    Elf32_Shdr *section_h = (Elf32_Shdr *)(file_data + e_shoff);

    // Find the symbol table and associated string table
    for (uint32_t i = 0; i < e_shnum; i++)
    {
        if (i >= SHN_LORESERVE)
            break;
        sh_type = read_uint32(section_h[i].sh_type, file_data);
        if (sh_type == SHT_SYMTAB)
            return handle32_symtab(section_h, elf_header, file_data, i);
    }
    return format_error("Symbol table or string table not found");
}