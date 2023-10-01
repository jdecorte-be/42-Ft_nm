#include "ft_nm.h"

int check_magic(char *data, int fd, struct stat fd_info, char *filename)
{
    if (data[0] == ELFMAG0 && data[1] == ELFMAG1 && data[2] == ELFMAG2 && data[3] == ELFMAG3)
    {
        if (data[4] == ELFCLASS32)
            return handle32(data, (Elf32_Ehdr *)data, fd_info);
        else if (data[4] == ELFCLASS64)
            return handle64(data, (Elf64_Ehdr *)data, fd_info);
        else if (data[4] == ELFCLASSNONE)
            return format_error("unknown format\n", filename);
        else
            return format_error("unknown format\n", filename);
    }
    else
        return format_error("unknown format\n", filename);
}

int ft_nm(int fd,char *filename, int n)
{
    struct stat file_info;
    char *data;
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd == -1){
        perror(filename);
        exit(EXIT_FAILURE);
    }

    if (n > 2)
        printf("%s:\n", filename);

    if (fstat(fd, &file_info) < 0)
        return fprintf(stderr, "ft_nm: Warning: '%s': is a directory\n", filename);

    if (S_ISDIR(file_info.st_mode))
        return fprintf(stderr, "ft_nm: Warning: '%s': is a directory\n", filename);

    if ((data = mmap(NULL, file_info.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        return fprintf(stderr, "ft_nm: Warning: '%s': is a directory\n", filename);

    // if fail need to clear mmap and close fd
    int ret = check_magic(data, fd, file_info, filename);

    if (munmap(data, file_info.st_size) < 0)
        return fprintf(stderr, "ft_nm: Warning: '%s': is a directory\n", filename);
}

int main(int ac, char **av)
{
    int fd;

    if (ac == 1)
    {
        fd = open("a.out", O_RDONLY);
        if (fd == -1)
            fprintf(stderr, "test");
        ft_nm("a.out", ac);
    }
    else
    {

        for (int i = 1; i < ac; i++){
            fd = open(av[i], O_RDONLY);
            ft_nm(av[i], ac);
            close(fd);
        }
    }
    return 0;
}
