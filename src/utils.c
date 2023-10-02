#include "ft_nm.h"

int str_is_nullterm(const char *start, const char *end)
{
	while (start < end)
	{
		if (!*start)
			return 1;
		++start;
	}

	return 0;
}
