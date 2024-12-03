/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 10:06:58 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/03 16:05:39 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_inside_arg(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	check_correct_args(char **argv)
{
	if (ft_atoi(argv[1]) > PHILO_MAX || ft_atoi(argv[1]) <= 0
		|| check_inside_arg(argv[1]) == 1)
		return (write(2, "WRONG NUMBER OF PHILOSOPHERS!\n", 30), 1);
	if (ft_atoi(argv[2]) <= 0 || check_inside_arg(argv[2]) == 1)
		return (write(2, "WRONG TIME TO DIE PHILOSOPHERS!\n", 32), 1);
	if (ft_atoi(argv[3]) <= 0 || check_inside_arg(argv[3]) == 1)
		return (write(2, "WRONG TIME TO EAT PHILOSOPHERS!\n", 32), 1);
	if (ft_atoi(argv[4]) <= 0 || check_inside_arg(argv[4]) == 1)
		return (write(2, "WRONG TIME TO SLEEP PHILOSOPHERS!\n", 34), 1);
	if (argv[5] && (ft_atoi(argv[5]) < 0 || check_inside_arg(argv[5]) == 1))
		return (write(2, "WRONG NBR OF TIMES EACH PHILO MUST EAT!\n", 40), 1);
	return (0);
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
	while (i < simulation->philo_sim[0].number_of_philosophers)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

int	thread_create(t_simulation *simulation, pthread_mutex_t *forks)
{
	pthread_t	monitor_thread;
	int			i;

	if (pthread_create(&monitor_thread, NULL,
			&monitor, simulation->philo_sim) != 0)
		destroy_mutex("THREAD CREATE ERROR!", simulation, forks);
	i = 0;
	while (i < simulation->philo_sim[0].number_of_philosophers)
	{
		if (pthread_create(&simulation->philo_sim[i].thread, NULL,
				&philo_routine, &simulation->philo_sim[i]) != 0)
			destroy_mutex("WRONG THREAD CREATE!", simulation, forks);
		i++;
	}
	i = 0;
	if (pthread_join(monitor_thread, NULL) != 0)
		destroy_mutex("WRONG THREAD JOIN!", simulation, forks);
	while (i < simulation->philo_sim[0].number_of_philosophers)
	{
		if (pthread_join(simulation->philo_sim[i].thread, NULL) != 0)
			destroy_mutex("WRONG THREAD JOIN!", simulation, forks);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_simulation	simulation;
	t_philo			philo_sim[PHILO_MAX];
	pthread_mutex_t	forks[PHILO_MAX];

	if (argc != 5 && argc != 6)
		return (write(2, "WRONG NUMBER OF ARGUMENTS!\n", 27), 1);
	if (check_correct_args(argv) == 1)
		return (1);
	init_simulation(&simulation, philo_sim);
	init_forks(forks, ft_atoi(argv[1]));
	init_philo_sim(philo_sim, &simulation, forks, argv);
	thread_create(&simulation, forks);
	destroy_mutex(NULL, &simulation, forks);
	return (0);
}
