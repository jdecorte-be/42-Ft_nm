#pragma once

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

#include "../libft/libft.h"

typedef struct s_sym {
	char *name;
	uint64_t addr;
	unsigned char letter;
	unsigned short shndx;
	int used;
}	t_sym;

// Functions to handle ELF64 symbols
int elf64_symbols(Elf64_Sym sym, Elf64_Shdr *shdr, char *file_data, Elf64_Ehdr *elf_header);
// Functions to handle ELF32 symbols
int elf32_symbols(Elf32_Sym sym, Elf32_Shdr *shdr, char *file_data, Elf32_Ehdr *elf_header);

// Handle 64-bit ELF files
int handle64(char *file_data, Elf64_Ehdr *elf_header, struct stat fd_info);
// Handle 32-bit ELF files
int handle32(char *file_data, Elf32_Ehdr *elf_header, struct stat fd_info);

// Function to format errors
int format_error(char *msg, const char *filename);

// Function for sort
void mergeSort(t_sym *syms, int l, int r);

// Read data types with specific sizes
uint64_t read_uint64(uint64_t nb, char *file);
uint32_t read_uint32(uint32_t nb, char *file);
uint16_t read_uint16(uint16_t nb, char *file);

// Functions related to endian checking and swapping
static int get_machine_endian();
int check_endian(char *file);
int need_change_endian(char *file);
void reverse_endian(void *ptr, size_t length);
