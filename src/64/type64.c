#include "../ft_nm.h"

int elf64_symbols(Elf64_Sym sym, Elf64_Shdr *shdr, char *file_data, Elf64_Ehdr *elf_header)
{
  char c;
  uint64_t flags;
  uint64_t bind = ELF64_ST_BIND(sym.st_info);
  uint64_t type = ELF64_ST_TYPE(sym.st_info);
  uint16_t shndx = read_uint16(sym.st_shndx, file_data);

  uint64_t shnum = read_uint16(elf_header->e_shnum, file_data);

  if (bind == STB_GNU_UNIQUE)
    c = 'u';
  else if (bind == STB_WEAK)
  {
    if(bind == STT_OBJECT)
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
    type = read_uint64(shdr[shndx].sh_type, file_data);
    flags = read_uint64(shdr[shndx].sh_flags, file_data);

    if (type == SHT_NOBITS)
      c = 'B';
    else if (!(flags & SHF_WRITE))
    {
      if(flags & SHF_ALLOC && flags & SHF_EXECINSTR)
        c = 'T';
      else
        c = 'R';
    }
    else
      c = 'D';
  }
  if (bind == STB_LOCAL && c != '?')
    c += 32;
  return c;
}
