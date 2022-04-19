#include <pthread.h>

struct station {
    pthread_cond_t train_cond, passenger_cond;
    pthread_mutex_t lock;
    int seats_available, n_waiting, n_departing;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);