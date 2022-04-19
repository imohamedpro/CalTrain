#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
    station->seats_available = 0;
    station->n_waiting = 0;
    station->n_departing = 0;
    pthread_mutex_init(&(station->lock), NULL);
    pthread_cond_init(&(station->train_cond), NULL);
    pthread_cond_init(&(station->passenger_cond), NULL);
}

void
station_load_train(struct station *station, int count)
{
    pthread_mutex_lock(&(station->lock));
    if((station->n_waiting) == 0 || count == 0) {
        pthread_mutex_unlock(&(station->lock));
        return;
    }
    station->seats_available = count;
    pthread_cond_broadcast(&(station->passenger_cond));
    pthread_cond_wait(&(station->train_cond), &(station->lock));
    station->seats_available = 0;
    pthread_mutex_unlock(&(station->lock));
}

void
station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&(station->lock));
    station->n_waiting++;
    while(station->seats_available == 0){
        pthread_cond_wait(&(station->passenger_cond), &(station->lock));
    }
    station->n_waiting--;
    station->n_departing++;
    station->seats_available--;
    pthread_mutex_unlock(&(station->lock));
}

void
station_on_board(struct station *station)
{
    pthread_mutex_lock(&(station->lock));
    station->n_departing--;
    if((station->n_departing) == 0 && !(station->n_waiting && station->seats_available))
        pthread_cond_signal(&(station->train_cond));
    pthread_mutex_unlock(&(station->lock));
}