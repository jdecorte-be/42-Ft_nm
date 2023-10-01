#include "../ft_nm.h"

int elf32_symbols(Elf32_Sym sym, Elf32_Shdr *shdr, char *file_data)
{
  char  c;
  uint32_t bind = ELF32_ST_BIND(sym.st_info);
  uint32_t type = read_uint32(shdr[read_uint16(sym.st_shndx, file_data)].sh_type, file_data);
  uint32_t flags = read_uint32(shdr[read_uint16(sym.st_shndx, file_data)].sh_flags, file_data);

  if (bind == STB_GNU_UNIQUE)
    c = 'u';
  else if (bind == STB_WEAK)
    c = (sym.st_shndx == SHN_UNDEF) ? 'w' : 'W';
  else if (sym.st_shndx == SHN_UNDEF)
    c = 'U';
  else if (sym.st_shndx == SHN_ABS)
    c = 'A';
  else if (sym.st_shndx == SHN_COMMON)
    c = 'C';
  else if (type == SHT_NOBITS && flags == (SHF_ALLOC | SHF_WRITE))
    c = 'B';
  else if ((type == SHT_PROGBITS && (flags == SHF_ALLOC || flags == (SHF_ALLOC | SHF_MERGE))) || (type == SHT_PROGBITS && flags == (SHF_ALLOC | SHF_WRITE)) || type == SHT_DYNAMIC || (bind == STB_GLOBAL && type == STT_OBJECT && sym.st_shndx == SHN_UNDEF))
    c = (flags == (SHF_ALLOC | SHF_WRITE)) ? 'D' : 'R';
  else if (type == SHT_PROGBITS || type == SHT_INIT_ARRAY || type == SHT_FINI_ARRAY)
    c = 'T';
  else
    c = '?';

  if (bind == STB_LOCAL && c != '?')
    c += 32;

  return c;
}


