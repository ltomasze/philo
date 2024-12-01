#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>

# define PHILO_MAX 200

typedef struct s_philo
{
	int	id;
	int eat_now;
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
	pthread_t	thread;
} t_philo;

typedef struct s_simulation
{
	int	flag_death;
	pthread_mutex_t write_lock;
	pthread_mutex_t death_lock;
	pthread_mutex_t meal_lock;
	t_philo	*philo_sim;
} t_simulation;

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

int	ft_usleep(size_t millisec)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < millisec)
		usleep(500);
	return (0);
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

void init_simulation(t_simulation *simulation, t_philo *philo_sim)
{
	
	simulation->flag_death = 0;
	simulation->philo_sim = philo_sim;
	pthread_mutex_init(&simulation->write_lock, NULL);
	pthread_mutex_init(&simulation->death_lock, NULL);
	pthread_mutex_init(&simulation->meal_lock, NULL);
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

void	init_philo_sim(t_philo *philo_sim, t_simulation *simulation, 
			pthread_mutex_t *forks, char **argv)
{
	int	i;

	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		philo_sim[i].id = i + 1;
		philo_sim[i].eat_now = 0;
		philo_sim[i].meals_eaten = 0;
		init_inside_philo(&philo_sim[i], argv);
		philo_sim[i].start_time = get_current_time();
		philo_sim[i].last_meal = get_current_time();
		philo_sim[i].write_lock = &simulation->write_lock;
		philo_sim[i].death_lock = &simulation->death_lock;
		philo_sim[i].meal_lock = &simulation->meal_lock;
		philo_sim[i].flag_death = &simulation->flag_death;
		philo_sim[i].left_fork = &forks[i];
		if (i == 0)
			philo_sim[i].right_fork = 
				&forks[philo_sim[i].number_of_philosophers];
		else
			philo_sim[i].right_fork = &forks[i - 1];
		i++;
	}
}

void	destroy_mutex(char *str, t_simulation *simulation, 
			pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	if (str)
	{
		write(2, str, ft_strlen(str));
		write(2, "\n", 1);
	}
	pthread_mutex_destroy(&simulation->write_lock);
	pthread_mutex_destroy(&simulation->meal_lock);
	pthread_mutex_destroy(&simulation->death_lock);
	while (i < 
	simulation->philo_sim[0].number_of_philosophers)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

int death_philo(t_philo *philo, size_t time_to_die)
{
	pthread_mutex_lock(philo->meal_lock);
	if (get_current_time() - philo->last_meal >= time_to_die
			&& philo->eat_now == 0)
			return (pthread_mutex_unlock(philo->meal_lock), 1);
	pthread_mutex_unlock(philo->meal_lock);
	return (0);
}

int	death_loop(t_philo *philo)
{
	pthread_mutex_lock(philo->death_lock);
	if (*philo->flag_death == 1)
		return (pthread_mutex_unlock(philo->death_lock), 1);
	pthread_mutex_unlock(philo->death_lock);
	return (0);
}

void display_info(char *str, t_philo *philo, int id)
{
	size_t time;

	pthread_mutex_lock(philo->write_lock);
	time = get_current_time() - philo->start_time;
	if (!death_loop(philo))
		printf("%zu %d %s\n", time, id, str);
	pthread_mutex_unlock(philo->write_lock);
}

int if_death(t_philo *philo_sim)
{
	int	i;

	i = 0;
	while (i < philo_sim[0].number_of_philosophers)
	{
		if (death_philo(&philo_sim[i], philo_sim[i].time_to_die))
		{
			display_info("died", &philo_sim[i], philo_sim[i].id);
			pthread_mutex_lock(philo_sim[0].death_lock);
			*philo_sim->flag_death = 1;
			pthread_mutex_unlock(philo_sim[0].death_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

int if_finish_eat(t_philo *philo_sim)
{
	int i;
	int	finish_eat;
	if (philo_sim[0].number_of_times_each_philosopher_must_eat == -1)
		return (0);
	while (i < philo_sim[0].number_of_philosophers)
	{
		pthread_mutex_lock(philo_sim[i].meal_lock);
		if (philo_sim[i].meals_eaten >=
			philo_sim[i].number_of_times_each_philosopher_must_eat)
				finish_eat++;
		pthread_mutex_unlock(philo_sim[i].meal_lock);
		i++;
	}
	if (finish_eat == philo_sim[0].number_of_philosophers)
	{
		pthread_mutex_lock(philo_sim[0].death_lock);
		*philo_sim->flag_death = 1;
		pthread_mutex_unlock(philo_sim[0].death_lock);
		return (1);
	}
	return (0);
}

void	*monitor(void	*ptr)
{
	t_philo *philo_sim;

	philo_sim = (t_philo *)ptr;
	while(1)
		if (if_death(philo_sim) == 1 || if_finish_eat(philo_sim) == 1)
			break;
	return (ptr);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	display_info("has taken a fork", philo, philo->id);
	if (philo->number_of_philosophers == 1)
	{
		ft_usleep(philo->time_to_die);
		pthread_mutex_unlock(philo->right_fork);
		return;
	}
	pthread_mutex_lock(philo->left_fork);
	display_info("has taken a fork", philo, philo->id);
	philo->eat_now = 1;
	display_info("is eating", philo, philo->id);
	pthread_mutex_lock(philo->meal_lock);
	philo->last_meal = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->meal_lock);
	ft_usleep(philo->time_to_eat);
	philo->eat_now = 0;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}


void	*philo_routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (!death_loop(philo))
	{
		eat(philo);
	}
}
int	thread_create(t_simulation *simulation, pthread_mutex_t *forks)
{
	pthread_t	monitor_thread;
	int	i;
	if (pthread_create(&monitor_thread, NULL, 
		&monitor, simulation->philo_sim) !=0)
			destroy_mutex("THREAD CREATE ERROR!", 
			simulation, forks);
	i = 0;
	while (i < simulation->philo_sim[0].number_of_philosophers)
	{
		if (pthread_create(&simulation->philo_sim[i].thread, NULL, 
			&philo_routine, &simulation->philo_sim[i]) != 0)

	}
	return (0);
}

int	main(int argc, char **argv)
{
	
	t_simulation	simulation;
	t_philo		philo_sim[PHILO_MAX];
	pthread_mutex_t forks[PHILO_MAX];

	if (argc != 5 && argc != 6)
		return(write(2, "WRONG NUMBER OF ARGUMENTS!\n", 23), 1);
	if (check_correct_args(argv) == 1)
		return (1);
	init_simulation(&simulation, philo_sim);
	init_forks(forks, ft_atoi(argv[1]));
	init_philo_sim(philo_sim, &simulation, forks, argv);
	thread_create(&simulation, forks);
	destroy_mutex(NULL, &simulation, forks);
	return (0);
}
