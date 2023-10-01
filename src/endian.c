#include "ft_nm.h"

static int get_machine_endian()
{
    int i;

    i = 1;
    if (*((char *)&i) == 1)
        return (ELFDATA2LSB);
    else
        return (ELFDATA2MSB);
}

int check_endian(char *file)
{
    static unsigned char file_endian;
    static unsigned char mach_endian;
    static int init = 0;

    if (!init)
    {
        file_endian = file[EI_DATA];
        mach_endian = get_machine_endian();
        init = 1;
    }
    if (file_endian == mach_endian)
        return (0);
    else
        return (1);
}

uint64_t read_uint64(uint64_t nb, char *file)
{
    uint64_t res;
    int endian;

    endian = need_change_endian(file);
    res = nb;
    if (endian)
        reverse_endian(&res, sizeof(uint64_t));
    return (res);
}


uint32_t read_uint32(uint32_t nb, char *file)
{
    uint32_t res;
    int endian;

    endian = need_change_endian(file);
    res = nb;
    if (endian)
        reverse_endian(&res, sizeof(uint32_t));
    return (res);
}

uint16_t read_uint16(uint16_t nb, char *file)
{
    uint16_t res;
    int endian;

    endian = need_change_endian(file);
    res = nb;
    if (endian)
        reverse_endian(&res, sizeof(uint16_t));
    return (res);
}

int need_change_endian(char *file)
{
    static unsigned char file_endian;
    static unsigned char mach_endian;
    static int init = 0;

    if (!init)
    {
        file_endian = file[EI_DATA];
        mach_endian = get_machine_endian();
        init = 1;
    }
    if (file_endian == mach_endian)
        return (0);
    else
        return (1);
}

void reverse_endian(void *ptr, size_t length)
{
    uint8_t *bytes;
    uint8_t tmp[32];
    size_t i;

    bytes = (uint8_t *)ptr;
    i = 0;
    while (i < length)
    {
        tmp[i] = bytes[length - 1 - i];
        i++;
    }
    i = 0;
    while (i < length)
    {
        bytes[i] = tmp[i];
        i++;
    }
}