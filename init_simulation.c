/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:18:34 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/03 13:19:36 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_simulation(t_simulation *simulation, t_philo *philo_sim)
{
	simulation->flag_death = 0;
	simulation->philo_sim = philo_sim;
	pthread_mutex_init(&simulation->write_lock, NULL);
	pthread_mutex_init(&simulation->death_lock, NULL);
	pthread_mutex_init(&simulation->meal_lock, NULL);
}

void	init_forks(pthread_mutex_t *forks, int nbr_philosophers)
{
	int	i;

	i = 0;
	while (i < nbr_philosophers)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

void	init_inside_philo(t_philo *philo, char **argv)
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
			philo_sim[i].right_fork
				= &forks[philo_sim[i].number_of_philosophers - 1];
		else
			philo_sim[i].right_fork = &forks[i - 1];
		i++;
	}
}
