#include "ft_nm.h"

void merge(t_sym *syms, int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	t_sym L[n1], R[n2];

	for (i = 0; i < n1; i++)
		L[i] = syms[l + i];
	for (j = 0; j < n2; j++)
		R[j] = syms[m + 1 + j];

	i = 0;
	j = 0;
	k = l;
	while (i < n1 && j < n2)
	{
		if (strcmp(L[i].name, R[j].name) <= 0)
		{
			syms[k] = L[i];
			i++;
		}
		else
		{
			syms[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1)
	{
		syms[k] = L[i];
		i++;
		k++;
	}

	while (j < n2)
	{
		syms[k] = R[j];
		j++;
		k++;
	}
}

void mergeSort(t_sym *syms, int l, int r)
{
	if (l < r)
	{
		int m = l + (r - l) / 2;
		mergeSort(syms, l, m);
		mergeSort(syms, m + 1, r);
		merge(syms, l, m, r);
	}
}
