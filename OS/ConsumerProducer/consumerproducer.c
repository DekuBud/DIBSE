#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 7
#define MAX_VALUES 1000

// Ringbuffer
int buffer[BUFFER_SIZE];
int write_index = 0;
int read_index = 0;
int count = 0;

// Mutex und Condition Variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_producer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;

// Anzahl der gelesenen Werte
int read_values_count = 0;
int sum = 0;  // Für Mittelwert

// Producer-Funktion (Temperatursensor)
void* producer(void* arg) {
    int id = *((int*)arg);
    while (1) {
        int temp = rand() % 100;  // Zufällige Temperatur zwischen 0 und 99

        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) {
            printf("Schlafenlegen: Temperatursensor %d wurde schlafengelegt!\n", id);
            pthread_cond_wait(&cond_producer, &mutex);
        }

        // In den Ringbuffer schreiben
        buffer[write_index] = temp;
        write_index = (write_index + 1) % BUFFER_SIZE;
        count++;

        printf("Temperatursensor %d schreibt: %d\n", id, temp);

        // Consumer aufwecken, falls er schläft
        pthread_cond_signal(&cond_consumer);

        pthread_mutex_unlock(&mutex);

        usleep(100000);  // Kurz schlafen, um die Ausgaben besser sehen zu können (entfernen für Abgabe)
    }
    return NULL;
}

// Consumer-Funktion (Logging-Programm)
void* consumer(void* arg) {
    while (read_values_count < MAX_VALUES) {
        pthread_mutex_lock(&mutex);

        while (count == 0) {
            printf("Schlafenlegen: Logging Programm wurde schlafengelegt!\n");
            pthread_cond_wait(&cond_consumer, &mutex);
        }

        // Aus dem Ringbuffer lesen
        int temp = buffer[read_index];
        read_index = (read_index + 1) % BUFFER_SIZE;
        count--;
        sum += temp;
        read_values_count++;

        printf("Logging-Programm liest: %d (Werte gelesen: %d)\n", temp, read_values_count);

        // Mittelwert berechnen und ausgeben
        if (read_values_count % 100 == 0) {
            printf("Mittelwert der letzten %d Werte: %.2f\n", read_values_count, sum / (float)read_values_count);
        }

        // Producer aufwecken, falls sie schlafen
        pthread_cond_signal(&cond_producer);

        pthread_mutex_unlock(&mutex);

        usleep(150000);  // Kurz schlafen, um die Ausgaben besser sehen zu können (entfernen für Abgabe)
    }
    return NULL;
}

int main() {
    pthread_t producer1, producer2, consumer_thread;
    int sensor1_id = 1, sensor2_id = 2;

    // Threads für die beiden Sensoren und das Logging-Programm erstellen
    pthread_create(&producer1, NULL, producer, &sensor1_id);
    pthread_create(&producer2, NULL, producer, &sensor2_id);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Warten, bis das Logging-Programm fertig ist
    pthread_join(consumer_thread, NULL);

    // Threads der Sensoren abbrechen (weil sie sonst endlos laufen)
    pthread_cancel(producer1);
    pthread_cancel(producer2);

    // Ressourcen freigeben
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_producer);
    pthread_cond_destroy(&cond_consumer);

    printf("Programm beendet.\n");
    return 0;
}
