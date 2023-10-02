#include "ft_nm.h"

int check_magic(char *data, struct stat fd_info, char *filename)
{
    if (data[0] == ELFMAG0 && data[1] == ELFMAG1 && data[2] == ELFMAG2 && data[3] == ELFMAG3)
    {
        if (data[4] == ELFCLASS32)
            return handle32(data, (Elf32_Ehdr *)data, fd_info);
        else if (data[4] == ELFCLASS64)
            return handle64(data, (Elf64_Ehdr *)data, fd_info);
    }
    return fprintf(stderr, "ft_nm: '%s': file format not recognized\n", filename);
}

int ft_nm(int fd, char *filename)
{
    struct stat file_info;
    char *data;

    if (fstat(fd, &file_info) < 0 || S_ISDIR(file_info.st_mode))
        return fprintf(stderr, "ft_nm: Warning: '%s': is a directory\n", filename);

    data = mmap(NULL, file_info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED)
        return fprintf(stderr, "ft_nm: Warning: '%s': Unable to map file\n", filename);

    int ret = check_magic(data, file_info, filename);

    if (munmap(data, file_info.st_size) < 0)
        return fprintf(stderr, "ft_nm: Warning: '%s': Unable to unmap file\n", filename);

    // printf("%d\n", ret);
    return ret;
}

int main(int ac, char **av)
{
    int fd;
    int ret;

    if (ac == 1)
    {
        fd = open("a.out", O_RDONLY);
        if (fd == -1)
        {
            fprintf(stderr, "ft_nm: '%s': Unable to unmap file\n", "a.out");
            exit(EXIT_FAILURE);
        }
        ret = ft_nm(fd, "a.out");
    }
    else
    {

        for (int i = 1; i < ac; i++)
        {
            fd = open(av[i], O_RDONLY);
            if (fd == -1)
            {
                fprintf(stderr, "ft_nm: '%s': Unable to unmap file\n", av[i]);
                exit(EXIT_FAILURE);
            }
            ret = ft_nm(fd, av[i]);
        }
    }
    close(fd);
    ret == 0 ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE);
}
