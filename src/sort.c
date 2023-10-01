#include "ft_nm.h"

// static char	*clean_string(char *str)
// {
// 	int		j = 0;
// 	char	*new_str = malloc(ft_strlen(str) + 1);
// 	if (new_str == NULL)
// 	{
// 		ft_printf("Error allocating memory\n");
// 		return (NULL);
// 	}

// 	for(int i = 0; str[i]; i++)
// 	{
// 		if(str[i] != '_' && str[i] != '.' && str[i] != '@')
// 		{
// 			new_str[j] = str[i];
// 			j++;
// 		}
// 	}
// 	new_str[j] = '\0';

// 	return (new_str);
// }


void		ft_quicksort(t_sym *tab, int len)
{
	char	*compa;
	t_sym	tmp;
	int		n;
	int		m;

	if (len < 2)
		return ;
	compa = tab[(len - 1)].name;
	m = 0;
	n = -1;
	while (++n < len)
		if (strcmp(tab[n].name, compa) <= 0)
		{
			if (m != n) {
				tmp = tab[m];
				tab[m] = tab[n];
				tab[n] = tmp;
			}
			m++;
		}
	ft_quicksort(tab, --m);
	ft_quicksort(&tab[m], len - m);
}