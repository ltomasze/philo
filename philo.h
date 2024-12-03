/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:31:32 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/03 12:43:00 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# define PHILO_MAX 200

typedef struct s_philo
{
	int				id;
	int				eat_now;
	int				meals_eaten;
	int				number_of_philosophers;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	size_t			start_time;
	size_t			last_meal;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	*death_lock;
	pthread_mutex_t	*meal_lock;
	int				*flag_death;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_t		thread;
}	t_philo;

typedef struct s_simulation
{
	int				flag_death;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	meal_lock;
	t_philo			*philo_sim;
}	t_simulation;

int		ft_atoi(const char *nptr);
size_t	ft_strlen(const char *str);
size_t	get_current_time(void);
int		ft_usleep(size_t millisec);
int		check_inside_arg(char *arg);
int		check_correct_args(char **argv);
void	init_simulation(t_simulation *simulation, t_philo *philo_sim);
void	init_forks(pthread_mutex_t *forks, int nbr_philosophers);
void	init_inside_philo(t_philo *philo, char **argv);
void	init_philo_sim(t_philo *philo_sim, t_simulation *simulation,
			pthread_mutex_t *forks, char **argv);
void	destroy_mutex(char *str, t_simulation *simulation,
			pthread_mutex_t *forks);
int		death_philo(t_philo *philo, size_t time_to_die);
int		death_loop(t_philo *philo);
void	display_info(char *str, t_philo *philo, int id);
int		if_death(t_philo *philo_sim);
int		if_finish_eat(t_philo *philo_sim);
void	*monitor(void	*ptr);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	*philo_routine(void *ptr);
int		thread_create(t_simulation *simulation, pthread_mutex_t *forks);
int		main(int argc, char **argv);

#endif