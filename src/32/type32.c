#include "../ft_nm.h"

int elf32_symbols(Elf32_Sym sym, Elf32_Shdr *shdr, char *file_data, Elf32_Ehdr *elf_header)
{
  char c;
  uint32_t flags;
  uint32_t bind = ELF32_ST_BIND(sym.st_info);
  uint32_t type = ELF32_ST_TYPE(sym.st_info);
  
  uint16_t shndx = read_uint16(sym.st_shndx, file_data);
  uint32_t shnum = read_uint16(elf_header->e_shnum, file_data);

  if (bind == STB_GNU_UNIQUE)
    c = 'u';
  else if(type == STT_GNU_IFUNC)
    c = 'i';
  else if (bind == STB_WEAK)
  {
    if(type == STT_OBJECT)
      c = (shndx == SHN_UNDEF) ? 'v' : 'V';
    else
      c = (shndx == SHN_UNDEF) ? 'w' : 'W';
  }
  else if (bind == STB_WEAK)
  {
    c = 'W';
    if (shndx == SHN_UNDEF)
      c = 'w';
  }
  else if (sym.st_shndx == SHN_UNDEF)
    c = 'U';
  else if (sym.st_shndx == SHN_ABS)
    c = 'A';
  else if (sym.st_shndx == SHN_COMMON)
    c = 'C';
  else if (shndx < shnum) {
    type = read_uint32(shdr[shndx].sh_type, file_data);
    flags = read_uint32(shdr[shndx].sh_flags, file_data);

    if (type == SHT_NOBITS)
      c = 'B';
    else if (!(flags & SHF_WRITE))
    {
      if(flags & SHF_ALLOC && flags & SHF_EXECINSTR)
        c = 'T';
      else
        c = 'R';
    }
    else if(flags & SHF_EXECINSTR)
      c = 'T';
    else
      c = 'D';
  }
  if (bind == STB_LOCAL && c != '?')
    c += 32;
  return c;
}
