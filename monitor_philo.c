/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:20:17 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/03 13:26:35 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	death_philo(t_philo *philo, size_t time_to_die)
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

int	if_death(t_philo *philo_sim)
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

int	if_finish_eat(t_philo *philo_sim)
{
	int	i;
	int	finish_eat;

	i = 0;
	finish_eat = 0;
	if (philo_sim[0].number_of_times_each_philosopher_must_eat == -1)
		return (0);
	while (i < philo_sim[0].number_of_philosophers)
	{
		pthread_mutex_lock(philo_sim[i].meal_lock);
		if (philo_sim[i].meals_eaten
			>= philo_sim[i].number_of_times_each_philosopher_must_eat)
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
	t_philo	*philo_sim;

	philo_sim = (t_philo *)ptr;
	while (1)
		if (if_death(philo_sim) == 1 || if_finish_eat(philo_sim) == 1)
			break ;
	return (ptr);
}
