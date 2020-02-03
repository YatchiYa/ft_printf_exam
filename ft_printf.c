/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yarab <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 16:42:41 by yarab             #+#    #+#             */
/*   Updated: 2020/02/03 18:53:49 by yarab            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <stdarg.h>


// time start : 4h45
//
//
//
//

typedef struct s_flags
{
	int is_width;
	int width;
	int is_precision;
	int precision;
}			t_flags;

int ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

char *ft_strdup(char *str)
{
	int i = 0;
	char *dest;

	if (!str || !(dest = malloc(sizeof(char) * (ft_strlen(str) + 1))))
		return (NULL);
	while (str[i])
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void ft_putchar(char c, int *size)
{
	write(1, &c, 1);
	*size = *size + 1;
}

void	ft_putstr(char *str, int *size)
{
	int i = 0;
	while (str[i])
	{
		ft_putchar(str[i], size);
		i++;
	}
}

void	ft_putnbr(unsigned int nb, int *size)
{
	if (nb > 10)
		ft_putnbr(nb / 10, size);
	ft_putchar(nb % 10 + 48, size);
}

int ft_atoi(char *str)
{
	int number;
	int i = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		number = number * 10 + str[i] - 48;
		i++;
	}
	return (number);
}

int	ft_fill_width(t_flags *flags, char *str)
{
	int i = 0;
	char *dest;

	if (!str || !(dest = malloc(sizeof(char) * 256)))
		return (0);
	if (str[i] != 's' && str[i] != '.' && str[i] != 'd' && str[i] != 'x' && str[i])
	{
		while (str[i] >= '0' && str[i] <= '9')
		{
			dest[i] = str[i];
			i++;
		}
		dest[i] = '\0';
		flags->width = ft_atoi(dest);
	}
	free(dest);
	return (i);
}


int	ft_fill_precision(t_flags *flags, char *str)
{
	int i = 0;
	char *dest;

	if (!str || !(dest = malloc(sizeof(char) * 256)))
		return (0);
	if (str[i] != 's' && str[i] != '.' && str[i] != 'd' && str[i] != 'x' && str[i])
	{
		while (str[i] >= '0' && str[i] <= '9')
		{
			dest[i] = str[i];
			i++;
		}
		dest[i] = '\0';
		flags->precision = ft_atoi(dest);
	}
	free(dest);
	return (i);
}


void	ft_print_elem(int end, int start, char c, int *size)
{
	while (start < end)
	{
		ft_putchar(c, size);
		start++;
	}
}

void	ft_init_struct(t_flags *flags)
{
	flags->is_width = 0;
	flags->width = 0;
	flags->is_precision = 0;
	flags->precision = 0;
}

int	hexa_len(unsigned int nb)
{
	if (nb < 16)
		return 1;
	else
		return (1 + hexa_len(nb / 16));
}

int	numlen(int nb)
{
	unsigned int number;
	int i = 1;

	if (nb < 0)
	{
		i++;
		number = nb * -1;	
	}
	else
		number = nb;
	while (number / 10 != 0)
	{
		i++;
		number /= 10;
	}
	return (i);
}





void	ft_putadr(unsigned int nb, int *size)
{
	if (nb > 16)
	{
		ft_putadr(nb / 16, size);
		ft_putadr(nb % 16, size);
	}
	else if (nb < 10)
		ft_putchar(nb % 10 + 48, size);
	else
	{
		if (nb == 10)
			ft_putchar('a', size);
		else if (nb == 11)
			ft_putchar('b', size);
		else if (nb == 12)
			ft_putchar('c', size);
		else if (nb == 13)
			ft_putchar('d', size);
		else if (nb == 14)
			ft_putchar('e', size);
		else if (nb == 15)
			ft_putchar('f', size);
	}
}





void	ft_parse_d(va_list args, t_flags flags, int *size)
{
	int nb;
	int len;

	nb = va_arg(args, int);
	len = numlen(nb);

	if (flags.is_precision == 0 && flags.is_width == 0)
	{
		if (nb < 0)
		{
			ft_putchar('-', size);
			nb *= -1;
		}
		ft_putnbr(nb, size);
	}
	else if (nb == 0 && flags.is_precision == 0 && flags.width == 0)
		ft_putchar('0', size);
	else if (nb == 0 && (flags.width == 0 || flags.is_width == 0) && (flags.precision == 0 || flags.is_precision == 0))
		;
	else if (flags.is_precision == 1)
	{
		if (nb < 0)
			ft_print_elem(flags.width, flags.precision > len ? flags.precision + 1 : len, ' ', size);
		else
			ft_print_elem(flags.width, flags.precision > len ? flags.precision : len, ' ', size);
		
		nb < 0 ? ft_putchar('-', size) : 0;
		if (nb < 0)
			ft_print_elem(flags.precision + 1, len, '0', size);
		else
			ft_print_elem(flags.precision, len , '0', size);
		nb < 0 ? nb *= -1 : 0;

		if (flags.precision == 0i && nb == 0)
			ft_putchar(' ', size);
		else
			ft_putnbr(nb, size);
	}
	else
	{
		ft_print_elem(flags.width, len, ' ', size);
		nb < 0 ? ft_putchar('-', size) : 0;
		nb < 0 ? nb *= -1 : 0;
		ft_putnbr(nb, size);
	}
}

void	ft_parse_x(va_list args, t_flags flags, int *size)
{
	unsigned int nb;
	int len;

	nb = va_arg(args, unsigned int);
	len = hexa_len(nb);

	if (flags.is_precision == 0 && flags.is_width == 0)
	{
		ft_putadr(nb, size);
		printf("\n debug => %d\n", *size);
	}
	else if (nb == 0 && flags.is_precision == 0 && flags.width == 0)
		ft_putchar('0', size);
	else if (nb == 0 && (flags.width == 0 || flags.is_width == 0) && (flags.precision == 0 || flags.is_precision == 0))
		;
	else if (flags.is_precision == 1)
	{
		ft_print_elem(flags.width, flags.precision > len ? flags.precision : len, ' ', size);
		ft_print_elem(flags.precision, len, '0', size);
		if (flags.precision == 0 && nb == 0)
			ft_putchar(' ', size);
		else
			ft_putadr(nb, size);
	}
	else
	{
		ft_print_elem(flags.width, len, ' ', size);
		ft_putadr(nb, size);
	}
}

void	ft_parse_s(va_list args, t_flags flags, int *size)
{
	char *buff;
	char *tmp;
	int len;
	int i = 0;

	tmp = va_arg(args, char*);
	buff = (tmp == NULL) ? ft_strdup("(null)") : ft_strdup(tmp);
	len = ft_strlen(buff);

	if (flags.is_precision == 0 && flags.is_width == 0)
		ft_putstr(buff, size);
	else if (len == 0 && (flags.width == 0 || flags.is_width == 0) && (flags.precision == 0 || flags.is_precision == 0))
		;
	else if (flags.is_precision == 1)
	{
		ft_print_elem(flags.width, flags.precision > len ? len : flags.precision, ' ', size);
		int limit = flags.precision > len ? len : flags.precision;
		while (buff[i] && i < limit)
		{
			ft_putchar(buff[i], size);
			i++;
		}
	}
	else
	{
		ft_print_elem(flags.width, len, ' ', size);
		ft_putstr(buff, size);
	}
	free(buff);
}

void ft_parse_format(va_list args, char *str, int *i, int *size)
{
	int k = 0;
	t_flags flags;

	ft_init_struct(&flags);
	if (str[k] != '.' && (str[k] >= '0' && str[k] <= '9'))
	{
		flags.is_width = 1;
		k+= ft_fill_width(&flags, &str[k]);
	}
	if (str[k] == '.')
	{
		flags.is_precision = 1;
		k+= ft_fill_precision(&flags, &str[k + 1]) + 1;
	}
	*i += k + 1;
	if (str[k] == 'd')
		ft_parse_d(args, flags, size);
	else if (str[k] == 's')
		ft_parse_s(args, flags, size);
	else if (str[k] == 'x')
		ft_parse_x(args, flags, size);
}

int ft_printf(char *str, ...)
{
	int i = 0;
	int size;
	va_list args;

	size = 0;
	va_start(args, str);

	while (str[i])
	{
		if (str[i] == '%')
		{
			ft_parse_format(args, &str[i + 1], &i, &size);
			i++;
		}
		else
		{
			ft_putchar(str[i], &size);
			i++;
		}
	}

	va_end(args);
	return (size);
}


