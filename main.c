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
} t_philo;

long get_current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000; // czas w milisekundach
}

void *philosopher_routine(void *arg) {
    t_philo *philo = (t_philo *)arg;
    long time_of_death = get_current_time() + philo->time_to_die;
    
    while (1) {
        // Jeśli filozof przekroczy czas do śmierci, kończymy program
        if (get_current_time() >= time_of_death) {
            pthread_mutex_lock(philo->mutex_death);
            if (!(*philo->is_dead)) {
                *philo->is_dead = 1;
                printf("Philosopher %d died.\n", philo->id);
                exit(0);  // kończymy działanie programu
            }
            pthread_mutex_unlock(philo->mutex_death);
        }
        usleep(1000);  // Zmniejszenie obciążenia CPU, 1 ms przerwy
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 5) {
        printf("Usage: ./philo <time_to_die> <time_to_eat> <time_to_sleep> <num_philosophers>\n");
        return 1;
    }

    int num_philosophers = 1;  // w tym przypadku testujemy z jednym filozofem
    long time_to_die = atol(argv[1]);
    long time_to_eat = atol(argv[2]);
    long time_to_sleep = atol(argv[3]);

    // Mutexy i zmienna do śmierci
    pthread_mutex_t mutex_death = PTHREAD_MUTEX_INITIALIZER;
    int is_dead = 0;

    // Tworzenie wątków dla filozofów
    pthread_t threads[num_philosophers];
    t_philo philosophers[num_philosophers];

    int i = 0;
    while (i < num_philosophers) {  // Zmiana z for na while
        philosophers[i].id = i + 1;
        philosophers[i].time_to_die = time_to_die;
        philosophers[i].time_to_eat = time_to_eat;
        philosophers[i].time_to_sleep = time_to_sleep;
        philosophers[i].last_meal = get_current_time();
        philosophers[i].mutex_death = &mutex_death;
        philosophers[i].is_dead = &is_dead;

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

    return 0;
}
