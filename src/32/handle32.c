#include "../ft_nm.h"
#include <arpa/inet.h> // For network byte order functions
#include <endian.h>    // For byte order macros
#include <math.h>

int format_check32(char *file_data, Elf32_Ehdr *elf_header, struct stat fd_info)
{
    if (!elf_header)
        return fprintf(stderr, "Elf_header don't exist\n");

    // check machine version
    if (elf_header->e_machine == EM_NONE)
        return fprintf(stderr, "Architecture not handled\n");

    // check header max size
    if (fd_info.st_size <= sizeof(Elf32_Ehdr))
        return fprintf(stderr, "Symbol table or string table not found\n");
    if (elf_header->e_ident[EI_CLASS] != ELFCLASS32 && elf_header->e_ident[EI_DATA] != ELFDATA2LSB)
    {
        if (elf_header->e_type != ET_EXEC && elf_header->e_type != ET_DYN)
            return fprintf(stderr, "Symbol table or string table not found\n");
    }
    // check if e_shnum is within bounds
    if (elf_header->e_shnum >= SHN_LORESERVE)
        return fprintf(stderr, "Too many sections\n");
    return 0;
}


int handle32_symtab(Elf32_Shdr *section_h, Elf32_Ehdr *elf_header, char *file_data, int i)
{
    uint32_t sh_offset, sh_link;

    Elf32_Sym *symtab = NULL;
    size_t symtab_size = 0;
    char *strtab = NULL;

    sh_link = read_uint32(section_h[i].sh_link, file_data);
    sh_offset = read_uint32(section_h[i].sh_offset, file_data);

    symtab = (Elf32_Sym *)(file_data + sh_offset); // OK
    symtab_size = read_uint32(section_h[i].sh_size, file_data) / read_uint32(section_h[i].sh_entsize, file_data); // OK
    strtab = file_data + read_uint32(section_h[sh_link].sh_offset, file_data);  // OK

    t_sym *tab = malloc(sizeof(t_sym) * symtab_size);
    if (!tab)
        return fprintf(stderr, "Memory allocation failed\n");

    size_t tab_size = 0;
    for (size_t i = 1; i < symtab_size; i++)
    {
        uint32_t type = ELF32_ST_TYPE(symtab[i].st_info);
        if (type == STT_FUNC || type == STT_OBJECT || type == STT_NOTYPE || type == STT_GNU_IFUNC || type == STT_TLS)
        {
            tab[tab_size].addr = read_uint32(symtab[i].st_value, file_data);
            tab[tab_size].letter = elf32_symbols(symtab[i], section_h, file_data, elf_header);
            tab[tab_size].name = strtab + read_uint32(symtab[i].st_name, file_data);
            tab[tab_size].shndx = read_uint16(symtab[i].st_shndx, file_data);
            tab_size++;
        }
    }

	mergeSort(tab, 0, tab_size - 1);

	for (int i = 0; i < tab_size; i++)
	{
		if (tab[i].shndx == SHN_UNDEF)
			printf("%8c %c %s\n", ' ', tab[i].letter, tab[i].name);
		else
			printf("%08lx %c %s\n", tab[i].addr, tab[i].letter, tab[i].name);
	}

    free(tab);
    return 0;
}

int handle32(char *file_data, Elf32_Ehdr *elf_header, struct stat fd_info)
{

    uint32_t sh_type;

    if (format_check32(file_data, elf_header, fd_info) < 0)
        return -1;

    uint32_t e_shoff = read_uint32(elf_header->e_shoff, file_data);
    uint32_t e_shnum = read_uint16(elf_header->e_shnum, file_data);

    if (e_shoff > fd_info.st_size)
        return fprintf(stderr, "Offset bigger than file size\n");

    Elf32_Shdr *section_h = (Elf32_Shdr *)(file_data + e_shoff);

    // Find the symbol table and associated string table
    for (uint32_t i = 0; i < e_shnum; i++)
    {
        sh_type = read_uint32(section_h[i].sh_type, file_data);
        if (sh_type == SHT_SYMTAB){
            return handle32_symtab(section_h, elf_header, file_data, i);
        }
    }
    return fprintf(stderr, "Symbol table or string table not found\n");
}
