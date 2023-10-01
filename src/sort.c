#include "ft_nm.h"

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