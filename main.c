#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

# define PHILO_MAX 200

typedef struct s_philo
{
	int	id;
	int eating;
	int meals_eaten;
	int	number_of_philosophers;
	size_t	time_to_die;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat;
	size_t	start_time;
	size_t	last_meal;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t *death_lock;
	pthread_mutex_t *meal_lock;
	int	*flag_death;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;

} t_philo;

typedef struct s_system
{
	int	flag_death;
	pthread_mutex_t write_lock;
	pthread_mutex_t death_lock;
	pthread_mutex_t meal_lock;
	t_philo	*philosophers;
} t_system;

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

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "ERROR gettimeofday()!\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	destroy_mutex(char *str, t_system *system, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	if (str)
	{
		write(2, str, ft_strlen(str));
		write(2, "\n", 1);
	}
	pthread_mutex_destroy(&system->write_lock);
	pthread_mutex_destroy(&system->meal_lock);
	pthread_mutex_destroy(&system->death_lock);
	while (i < system->philosophers[0].number_of_philosophers)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
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
	if (argv[5] && (ft_atoi(argv[5]) < 0 || check_inside_arg(argv[5]) == 1))
		return (write(2, "WRONG NBR OF TIMES EACH PHILO MUST EAT!\n", 43), 1);
	return (0);
}

void init_system(t_system *system, t_philo *philosophers)
{
	system->flag_death = 0;
	system->philosophers = philosophers;
	pthread_mutex_init(&system->write_lock, NULL);
	pthread_mutex_init(&system->death_lock, NULL);
	pthread_mutex_init(&system->meal_lock, NULL);

}
void init_forks(pthread_mutex_t *forks, int nbr_philosophers)
{
	int	i;

	i = 0;
	while (i < nbr_philosophers)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

void init_inside_philo(t_philo *philo, char **argv)
{
	philo->number_of_philosophers = (ft_atoi(argv[1]));
	philo->time_to_die = ft_atoi(argv[2]);
	philo->time_to_eat = ft_atoi(argv[3]);
	philo->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		philo->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		philo->number_of_times_each_philosopher_must_eat = -1;
}

void	init_philosophers(t_philo *philosophers, t_system *system, 
	pthread_mutex_t *forks, char **argv)
{
	int	i;

	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		philosophers[i].id = i + 1;
		philosophers[i].eating = 0;
		philosophers[i].meals_eaten = 0;
		init_inside_philo(&philosophers[i], argv);
		philosophers[i].start_time = get_current_time();
		philosophers[i].last_meal = get_current_time();
		philosophers[i].write_lock = &system->write_lock;
		philosophers[i].death_lock = &system->death_lock;
		philosophers[i].meal_lock = &system->meal_lock;
		philosophers[i].flag_death = &system->flag_death;
		philosophers[i].left_fork = &forks[i];
		if (i == 0)
			philosophers[i].right_fork = 
				&forks[philosophers[i].number_of_philosophers];
		else
			philosophers[i].right_fork = &forks[i - 1];
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_system	system;
	t_philo		philosophers[PHILO_MAX];
	pthread_mutex_t forks[PHILO_MAX];

	if (argc != 5 && argc != 6)
		return(write(2, "WRONG NUMBER OF ARGUMENTS!\n", 23), 1);
	if (check_correct_args(argv) == 1)
		return (1);
	init_system(&system, philosophers);
	init_forks(forks, ft_atoi(argv[1]));
	init_philosophers(philosophers, &system, forks, argv);
	thread_create(&system, forks);
	destroy_mutex(NULL, &system, forks);
	return (0);
}
