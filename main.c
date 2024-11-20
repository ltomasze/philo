#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_philo {
    int id;
    long time_to_die;
    long time_to_eat;
    long time_to_sleep;
    long last_meal;
    pthread_mutex_t *mutex_death;
    int *is_dead;
    pthread_mutex_t *mutex_forks;
    int num_philosophers;
} t_philo;

long get_current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000; // Czas w milisekundach
}

void *philosopher_routine(void *arg) {
    t_philo *philo = (t_philo *)arg;

    if (philo->num_philosophers == 1) {
        // Filozof umiera, bo nie ma drugiego do dzielenia widelca
        usleep(philo->time_to_die * 1000); // Czas życia filozofa minął
        pthread_mutex_lock(philo->mutex_death);
        if (!(*philo->is_dead)) {
            *philo->is_dead = 1;
            printf("Philosopher %d died.\n", philo->id);
        }
        pthread_mutex_unlock(philo->mutex_death);
        return NULL;
    }

    while (1) {
        // Sprawdzanie, czy filozof umarł
        pthread_mutex_lock(philo->mutex_death);
        if (*philo->is_dead) {
            pthread_mutex_unlock(philo->mutex_death);
            break;
        }
        pthread_mutex_unlock(philo->mutex_death);

        // Filozof bierze widelce (symulacja)
        pthread_mutex_lock(&philo->mutex_forks[philo->id - 1]);
        pthread_mutex_lock(&philo->mutex_forks[philo->id % 5]);

        // Filozof je
        printf("Philosopher %d is eating.\n", philo->id);
        philo->last_meal = get_current_time();
        usleep(philo->time_to_eat * 1000); // Czas jedzenia

        // Filozof odkłada widelce
        pthread_mutex_unlock(&philo->mutex_forks[philo->id - 1]);
        pthread_mutex_unlock(&philo->mutex_forks[philo->id % 5]);

        // Sprawdzamy, czy filozof przekroczył czas do śmierci
        if (get_current_time() - philo->last_meal > philo->time_to_die) {
            pthread_mutex_lock(philo->mutex_death);
            if (!(*philo->is_dead)) {
                *philo->is_dead = 1;
                printf("Philosopher %d died.\n", philo->id);
            }
            pthread_mutex_unlock(philo->mutex_death);
            break;
        }

        // Filozof śpi
        printf("Philosopher %d is sleeping.\n", philo->id);
        usleep(philo->time_to_sleep * 1000); // Czas spania
    }

    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 5) {
        printf("Usage: ./philo <num_philosophers> <time_to_die> <time_to_eat> <time_to_sleep>\n");
        return 1;
    }

    // Odczytanie argumentów z linii komend
    int num_philosophers = atoi(argv[1]);  // Liczba filozofów z argv[1]
    long time_to_die = atol(argv[2]);  // Czas do śmierci z argv[2]
    long time_to_eat = atol(argv[3]);  // Czas jedzenia z argv[3]
    long time_to_sleep = atol(argv[4]);  // Czas spania z argv[4]

    // Mutexy i zmienna do śmierci
    pthread_mutex_t mutex_death = PTHREAD_MUTEX_INITIALIZER;
    int is_dead = 0;

    // Mutexy dla widełek
    pthread_mutex_t mutex_forks[num_philosophers];
    int i = 0;
    while (i < num_philosophers) {
        pthread_mutex_init(&mutex_forks[i], NULL);
        i++;  // Zwiększenie indeksu
    }

    // Tworzenie wątków dla filozofów
    pthread_t threads[num_philosophers];
    t_philo philosophers[num_philosophers];

    i = 0;
    while (i < num_philosophers) {  // Zmiana z for na while
        philosophers[i].id = i + 1;
        philosophers[i].time_to_die = time_to_die;
        philosophers[i].time_to_eat = time_to_eat;
        philosophers[i].time_to_sleep = time_to_sleep;
        philosophers[i].last_meal = get_current_time();
        philosophers[i].mutex_death = &mutex_death;
        philosophers[i].is_dead = &is_dead;
        philosophers[i].mutex_forks = mutex_forks;
        philosophers[i].num_philosophers = num_philosophers;

        // Tworzenie wątku dla każdego filozofa
        if (pthread_create(&threads[i], NULL, philosopher_routine, &philosophers[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }

        i++;  // Zwiększenie indeksu w pętli
    }

    // Czekamy na zakończenie wszystkich wątków
    i = 0;
    while (i < num_philosophers) {  // Zmiana z for na while
        pthread_join(threads[i], NULL);
        i++;  // Zwiększenie indeksu
    }

    // Zwalniamy mutexy dla widełek
    i = 0;
    while (i < num_philosophers) {
        pthread_mutex_destroy(&mutex_forks[i]);
        i++;  // Zwiększenie indeksu
    }

    return 0;
}
