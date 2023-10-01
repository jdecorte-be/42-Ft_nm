#include "ft_nm.h"

int format_error(char *msg, char *filename)
{
    ft_putstr_fd(msg, 2);
    return -1;
}
