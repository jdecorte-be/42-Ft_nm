#include "ft_nm.h"

void handle_error(char *msg, int fd)
{
    ft_putstr_fd(msg, 2);
    if (fd != -1)
        close(fd);
    exit(EXIT_FAILURE);
}

void check_magic(char *data, int fd, struct stat fd_info) {
    if(data[0] == ELFMAG0 && data[1] == ELFMAG1
    && data[2] == ELFMAG2 && data[3] == ELFMAG3) {
        if(data[4] == ELFCLASS32) {
            handle32(data, (Elf32_Ehdr*)data, fd_info);
        }
        else if(data[4] == ELFCLASS64) {
            handle64(data, (Elf64_Ehdr*)data, fd_info);
        }
        else if(data[4] == ELFCLASSNONE)
            handle_error("unknown format", fd);
        else
            handle_error("unknown format", fd);
    }
    else
        handle_error("unknown format", fd);
}


void ft_nm(char *filename, int n)
{
    struct stat file_info;
    char *data;
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
        handle_error("no file blabla", fd);

    if(n > 2)
        printf("%s:\n", filename);

    if (fstat(fd, &file_info) < 0)
        handle_error("fstat", fd);

    if ((data = mmap(NULL, file_info.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        handle_error("mmap", fd);


    // if fail need to clear mmap and close fd
    check_magic(data, fd, file_info);

    if(munmap(data, file_info.st_size) < 0)
        handle_error("mmap", fd);

    close(fd);
}

int main(int ac, char **av)
{
    int fd;

    if (ac == 1)
    {
        fd = open("a.out", O_RDONLY);
        if (fd == -1)
            handle_error("ft_nm: 'a.out'", fd);
    }
    else
    {
        for (int i = 1; i < ac; i++)
            ft_nm(av[i], ac);
    }
    return 0;
}
