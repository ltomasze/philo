#include <unistd.h>
# define PHILO_MAX 200

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	num;

	i = 0;
	sign = 1;
	num = 0;
	while (nptr[i] == ' ' || nptr[i] == '\t' || nptr[i] == '\n'
		|| nptr[i] == '\v' || nptr[i] == '\f' || nptr[i] == '\r')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = num * 10 + (nptr[i] - '0');
		i++;
	}
	return (num * sign);
}

int check_inside_arg(char *arg)
{
	int i;
	
	i = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int check_correct_args(char **argv)
{
	if (ft_atoi(argv[1]) > PHILO_MAX || ft_atoi(argv[1]) <= 0
		|| check_inside_arg(argv[1]) == 1)
		return (write(2, "WRONG NUMBER OF PHILOSOPHERS!\n", 30), 1);
	if (ft_atoi(argv[2]) <= 0 || check_inside_arg(argv[2]) == 1)
		return (write(2, "WRONG TIME TO DIE PHILOSOPHERS!\n", 32), 1);
	if (ft_atoi(argv[3]) <= 0 || check_inside_arg(argv[3]) == 1)
		return (write(2, "WRONG TIME TO EAT PHILOSOPHERS!\n", 32), 1);
	if (ft_atoi(argv[4]) <= 0 || check_inside_arg(argv[4]) == 1)
		return (write(2, "WRONG TIME TO SLEEP PHILOSOPHERS!\n", 34)), 1;
	if (argv[5] && ft_atoi(argv[5]) < 0 || check_inside_arg(argv[5]) == 1)
		return (write(2, "WRONG NBR OF TIMES EACH PHILO MUST EAT!\n", 43), 1);
	return (0);
}


int	main(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return(write(2, "WRONG NUMBER OF ARGUMENTS!\n", 23), 1);
	if (check_correct_args(argv) == 1)
		return (1);
	return (0);
}
