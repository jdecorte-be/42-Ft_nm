#include "ft_nm.h"

int format_error(char *msg)
{
    ft_putstr_fd(msg, 2);
    return -1;
}
