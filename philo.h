/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:13:16 by ltomasze          #+#    #+#             */
/*   Updated: 2024/11/20 12:41:31 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 #ifndef PHILO_H
#define PHILO_H
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_philosopher
{
    int id;
    pthread_t thread;
    long long last_meal_time;
    int is_dead;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_mutex_t death_mutex;
    long long time_to_die;
    long long time_to_eat;
    long long time_to_sleep;
    struct s_simulation *simulation; // Wskaźnik do symulacji
} t_philosopher;

typedef struct s_simulation
{
    int num_philosophers;
    t_philosopher *philosophers;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    long long time_to_die;
    long long time_to_eat;
    long long time_to_sleep;
    int num_times_each_philo_must_eat; // Opcjonalny parametr
} t_simulation;

#endif

long long get_time(void);
void *philosopher_routine(void *arg);
void *monitor_routine(void *arg);
int create_philosophers(t_simulation *simulation);
int initialize_forks(t_simulation *simulation);
int parse_arguments(int argc, char **argv, t_simulation *simulation);

