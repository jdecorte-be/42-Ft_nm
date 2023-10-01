#ifndef FT_NM_H
#define FT_NM_H

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

#include "../libft/libft.h"

typedef struct s_sym {
	char *name;
	uint64_t addr;
	unsigned char letter;
}	t_sym;


int elf64_symbols(Elf64_Sym sym, Elf64_Shdr *shdr, char *file_data, Elf64_Ehdr *elf_header);
int elf32_symbols(Elf32_Sym sym, Elf32_Shdr *shdr, char *file_data);

int handle64(char *file_data, Elf64_Ehdr *elf_header, struct stat fd_info);
int handle32(char *file_data, Elf32_Ehdr *elf_header, struct stat fd_info);

int format_error(char *msg);
void ft_quicksort(t_sym *tab, int len);

uint64_t read_uint64(uint64_t nb, char *file);
uint32_t read_uint32(uint32_t nb, char *file);
uint16_t read_uint16(uint16_t nb, char *file);


static int get_machine_endian();
int check_endian(char *file);
int need_change_endian(char *file);
void reverse_endian(void *ptr, size_t length);


#endif