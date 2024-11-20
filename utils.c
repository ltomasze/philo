/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:19:41 by ltomasze          #+#    #+#             */
/*   Updated: 2024/11/20 12:41:32 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Funkcja do uzyskiwania czasu w milisekundach
long long get_time(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000); // Czas w milisekundach
}

// Funkcja, którą wykonują filozofowie
void *philosopher_routine(void *arg)
{
    t_philosopher *philosopher = (t_philosopher *)arg;
    t_simulation *simulation = philosopher->simulation; // Zakładamy, że dodano wskaźnik do symulacji w strukturze t_philosopher

    while (philosopher->is_dead == 0)
    {
        // Thinking
        pthread_mutex_lock(&simulation->print_mutex);
        printf("Philosopher %d is thinking\n", philosopher->id);
        pthread_mutex_unlock(&simulation->print_mutex);

        usleep(philosopher->time_to_sleep * 1000);

        // Próba podniesienia widelców
        pthread_mutex_lock(philosopher->left_fork);
        pthread_mutex_lock(philosopher->right_fork);

        // Eating
        pthread_mutex_lock(&simulation->print_mutex);
        printf("Philosopher %d is eating\n", philosopher->id);
        pthread_mutex_unlock(&simulation->print_mutex);

        philosopher->last_meal_time = get_time();
        usleep(philosopher->time_to_eat * 1000);

        // Odkładanie widelców
        pthread_mutex_unlock(philosopher->right_fork);
        pthread_mutex_unlock(philosopher->left_fork);

        // Sleeping
        pthread_mutex_lock(&simulation->print_mutex);
        printf("Philosopher %d is sleeping\n", philosopher->id);
        pthread_mutex_unlock(&simulation->print_mutex);

        usleep(philosopher->time_to_sleep * 1000);
    }

    return NULL;
}

void *monitor_routine(void *arg)
{
    t_simulation *simulation = (t_simulation *)arg;

    while (1)
    {
        for (int i = 0; i < simulation->num_philosophers; i++)
        {
            pthread_mutex_lock(&simulation->philosophers[i].death_mutex);
            if (get_time() - simulation->philosophers[i].last_meal_time > simulation->time_to_die)
            {
                pthread_mutex_lock(&simulation->print_mutex);
                printf("Philosopher %d has died\n", simulation->philosophers[i].id);
                pthread_mutex_unlock(&simulation->print_mutex);

                pthread_mutex_unlock(&simulation->philosophers[i].death_mutex);
                return NULL;
            }
            pthread_mutex_unlock(&simulation->philosophers[i].death_mutex);
        }
        usleep(1000); // Unikamy nadmiernego ob
    }
}


// Funkcja tworzenia filozofów
int create_philosophers(t_simulation *simulation)
{
    int i = 0;
    while (i < simulation->num_philosophers)
    {
        simulation->philosophers[i].id = i + 1;
        simulation->philosophers[i].last_meal_time = get_time();
        simulation->philosophers[i].is_dead = 0;
        simulation->philosophers[i].left_fork = &simulation->forks[i];
        simulation->philosophers[i].right_fork = &simulation->forks[(i + 1) % simulation->num_philosophers];
        simulation->philosophers[i].simulation = simulation; // Dodanie wskaźnika do symulacji

        // Inicjalizujemy mutex dla ochrony danych filozofa
        pthread_mutex_init(&simulation->philosophers[i].death_mutex, NULL);

        // Tworzymy wątek filozofa
        pthread_create(&simulation->philosophers[i].thread, NULL, philosopher_routine, (void *)&simulation->philosophers[i]);

        i++;
    }
    return 0;
}

// Funkcja inicjalizująca mutexy dla widelców
int initialize_forks(t_simulation *simulation)
{
    int i = 0;
    while (i < simulation->num_philosophers)
    {
        pthread_mutex_init(&simulation->forks[i], NULL);
        i++;
    }
    return 0;
}
