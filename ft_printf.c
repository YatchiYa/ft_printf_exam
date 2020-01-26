#include "unistd.h"
#include "limits.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdarg.h"

typedef struct flags
{
    int is_width;
    int is_precision;
    int width;
    int precision;
}       t_flags;

int ft_strlen(char *str)
{
    int i = 0;

    while (str[i])
        i++;
    return (i);
}

int	ft_hexalen(unsigned int n)
{
	if (n < 16)
		return 1;
	else
		return (1  + ft_hexalen(n / 16));
}

int numlen(int nbx)
{
    int i = 1;

    if (nbx < 0)
    {
	    i++;
    }
    if (nbx >= -9 && nbx <= 9)
        return 1;
    else
    {
	while (nbx / 10 != 0)
        {
            i++;
            nbx = nbx / 10;
        }
    }
    return (i);
}

void    ft_putchar(char c, int *size)
{
    size += 1;
    write(1, &c, 1);
}

void    ft_putstr(char *str, int *size)
{
    int i = 0;

    while (str[i])
    {
        ft_putchar(str[i], size);
        i++;
    }
}

void    ft_putnbr_u(unsigned int nbr, int *p)
{
	if (nbr >= 10)
		ft_putnbr_u(nbr / 10, p);
	ft_putchar(nbr % 10 + 48, p);
}

void	ft_putadr(unsigned long n, int *p)
{
	if (p == NULL)
		return ;
	else if (n > 15)
	{
		ft_putadr((n / 16), p);
		ft_putadr((n % 16), p);
	}
	else if (n < 10)
	{
		ft_putchar(n + '0', p);
	}
	else
	{
		n == 10 ? ft_putchar('a', p) : 0;
		n == 11 ? ft_putchar('b', p) : 0;
		n == 12 ? ft_putchar('c', p) : 0;
		n == 13 ? ft_putchar('d', p) : 0;
		n == 14 ? ft_putchar('e', p) : 0;
		n == 15 ? ft_putchar('f', p) : 0;
	}
}

void    init_struct(t_flags *flags)
{
    flags->is_precision = 0;
    flags->is_width = 0;
    flags->precision = 0;
    flags->width = 0;
}


int    ft_fill_width(t_flags *flags, char *str)
{
    int i = 0;
    char    *used;

    used = (char*)malloc(sizeof(char) * 256);
    while (str[i] != 's' && str[i] != 'x' && str[i] != 'd' && str[i] != '.' && str[i])
    {
	    while (str[i] >= '0' && str[i] <= '9')
	    {
		    used[i] = str[i];
		    i++;
	    }
    	flags->width = atoi(used);
    }
    free(used);
    return (i);
}

int    ft_fill_precision(t_flags *flags, char *str)
{
    int i = 0;
    char    *used;

    used = (char*)malloc(sizeof(char) * 256);
    while (str[i] != 's' && str[i] != 'x' && str[i] != 'd' && str[i])
    {
	    while (str[i] >= '0' && str[i] <= '9')
	    {
		    used[i] = str[i];
		    i++;
	    }
    	flags->precision = atoi(used);
    }
    free(used);
    return (i);
}

void    ft_print_elem(int end, int start, char c, int *size)
{
    while (start < end)
    {
        ft_putchar(c, size);
        start++;
    }
}
// start our printf
// **************************************** //

void    ft_parse_id(va_list args, int *size, t_flags flags)
{
    int nb;
	nb = va_arg(args, int);
    if (flags.is_precision == 1)
    {
	    if (nb > 0)
		    ft_print_elem(flags.width, flags.precision > numlen(nb) ? flags.precision : numlen((unsigned int)nb), ' ', size);
	   else
		   ft_print_elem(flags.width, flags.precision > numlen(nb) ? flags.precision + 1 : numlen((unsigned int)nb), ' ', size);
        nb < 0 ? ft_putchar('-', size) : 0;
        nb < 0 ? nb *= -1 : 0;
        ft_print_elem(flags.precision, numlen(nb), '0', size);
    }
    else
    {
	ft_print_elem(flags.width, numlen(nb), ' ', size);
        nb < 0 ? ft_putchar('-', size) : 0;
        nb < 0 ? nb *= -1 : 0;
    }
    ft_putnbr_u(nb, size);
}


void    ft_parse_x(va_list args, int *size, t_flags flags)
{
    unsigned int nb;
	nb = va_arg(args, unsigned int);
    if (flags.is_precision == 1)
    {
        ft_print_elem(flags.width, flags.precision > ft_hexalen(nb) ? flags.precision : ft_hexalen(nb), ' ', size);
        ft_print_elem(flags.precision, ft_hexalen(nb), '0', size);
    }
    else
    {
        ft_print_elem(flags.width, ft_hexalen(nb), ' ', size);
    }
    ft_putadr(nb, size);
}

void    ft_parse_s(va_list args, int *size, t_flags flags)
{
    char *used;
    int i = 0;

    used = va_arg(args, char*);
    if (flags.is_precision == 0)
    {
        ft_print_elem(flags.width, ft_strlen(used), ' ', size);
        ft_putstr(used, size);
    }
    else
    {
        int length = flags.precision > ft_strlen(used) ? ft_strlen(used) : flags.precision;
        ft_print_elem(flags.width, length , ' ', size);
        while(used[i] && i < length)
        {
            ft_putchar(used[i], size);
            i++;
        }
    }
    
}
        

void    ft_parse_format(va_list args, char *str, int *i, int *size)
{
    t_flags flags;
    int k = 0;

    init_struct(&flags);
    if (str[k] != '.')
    {
        flags.is_width = 1;
        k += ft_fill_width(&flags, &str[k]);
    }
    if (str[k] == '.')
    {
        flags.is_precision = 1;
        k +=ft_fill_precision(&flags, &str[k + 1]) + 1;
    }
    *i += k + 1;
    if (str[k] == 'd')
        ft_parse_id(args, size, flags);
    else if (str[k] == 'x')
        ft_parse_x(args, size, flags);
    else if (str[k] == 's')
        ft_parse_s(args, size, flags);	

}

int    ft_printf(char *str, ...)
{
    int i = 0;
    int size = 0;
	va_list	args;

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


int main(void)
{
   
	printf("[%.5s]\n", "Hello wordl !");
	ft_printf("[%.5s]\n", "Hello wordl !");
	

    //printf("[%15.5d]\n", -42);
    //ft_printf("[%15.5d]\n", -42);
    //printf("[%30.22d], [%30.22s], [%30.22x]\n", 5, "Hello world !", 31);
    //ft_printf("[%30.22d], [%30.22s], [%30.22x]\n", 5, "Hello world !", 31);

    
    //printf("[%30.5d], [%30.5s], [%30.5x]\n", 5, "Hello world !", 31);
    //ft_printf("[%30.5d], [%30.5s], [%30.5x]\n", 5, "Hello world !", 31);
}
