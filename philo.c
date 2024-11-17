/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:24:18 by ltomasze          #+#    #+#             */
/*   Updated: 2024/11/17 17:18:32 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Funkcja do konwersji argumentów i walidacji
int parse_arguments(int argc, char **argv, t_simulation *simulation)
{
    if (argc < 5 || argc > 6)
    {
        printf("Usage: %s num_philosophers time_to_die time_to_eat time_to_sleep [num_times_each_philo_must_eat]\n", argv[0]);
        return 0;
    }

    simulation->num_philosophers = atoi(argv[1]);
    if (simulation->num_philosophers < 1)
    {
        printf("Error: Number of philosophers must be at least 1.\n");
        return 0;
    }

    simulation->time_to_die = atoi(argv[2]);
    simulation->time_to_eat = atoi(argv[3]);
    simulation->time_to_sleep = atoi(argv[4]);

    if (simulation->time_to_die <= 0 || simulation->time_to_eat <= 0 || simulation->time_to_sleep <= 0)
    {
        printf("Error: All times must be greater than 0.\n");
        return 0;
    }

    if (argc == 6)
    {
        simulation->num_times_each_philo_must_eat = atoi(argv[5]);
        if (simulation->num_times_each_philo_must_eat <= 0)
        {
            printf("Error: num_times_each_philo_must_eat must be greater than 0.\n");
            return 0;
        }
    }
    else
    {
        simulation->num_times_each_philo_must_eat = -1; // Opcjonalny parametr
    }

    return 1;
}

// Główna funkcja programu
int main(int argc, char **argv)
{
    t_simulation simulation;

    // Parsowanie argumentów wejściowych
    if (!parse_arguments(argc, argv, &simulation))
        return 1;

    // Alokacja pamięci dla filozofów i widelców
    simulation.philosophers = malloc(sizeof(t_philosopher) * simulation.num_philosophers);
    simulation.forks = malloc(sizeof(pthread_mutex_t) * simulation.num_philosophers);
    pthread_mutex_init(&simulation.print_mutex, NULL);

    // Inicjalizacja mutexów dla widelców
    if (initialize_forks(&simulation) != 0)
        return 1;

    // Tworzenie wątków dla filozofów
    if (create_philosophers(&simulation) != 0)
        return 1;

    // Oczekiwanie na zakończenie wątków
    int i = 0;
    while (i < simulation.num_philosophers)
    {
        pthread_join(simulation.philosophers[i].thread, NULL);
        i++;
    }

    // Zwalnianie mutexów dla każdego filozofa
    i = 0;
    while (i < simulation.num_philosophers)
    {
        pthread_mutex_destroy(&simulation.philosophers[i].death_mutex); // Zniszczenie mutexa death_mutex
        i++;
    }

    // Czyszczenie pamięci i zwalnianie zasobów
    free(simulation.philosophers);
    free(simulation.forks);

    return 0;
}


