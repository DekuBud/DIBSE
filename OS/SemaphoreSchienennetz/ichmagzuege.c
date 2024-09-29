#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

// Struktur für Zuginformationen
struct Zug {
    char* zugname;
    int dauer;  // Zeit in Sekunden, die der Zug für die Strecke benötigt
};

// Semaphore für die Streckenfreigabe
sem_t strecke;

// Funktion, die die Strecke für den Zug freigibt oder blockiert
void* befahreStrecke(void* zug_data) {
    struct Zug* zug = (struct Zug*) zug_data;

    // Versuchen, die Strecke zu betreten (Semaphore verringern)
    if (sem_trywait(&strecke) == 0) {
        printf("Fahrterlaubnis für %s wurde erteilt!\n", zug->zugname);
        sleep(zug->dauer);  // Simuliert die Zeit, die der Zug für die Strecke benötigt
        printf("%s hat die Strecke verlassen.\n", zug->zugname);

        // Semaphore erhöhen, da der Zug die Strecke verlassen hat
        sem_post(&strecke);
    } else {
        // Wenn keine Freigabe erteilt werden kann, muss der Zug warten
        printf("Fahrterlaubnis für %s verweigert und muss auf ein freies Gleis warten!\n", zug->zugname);

        // Blockieren bis eine Freigabe verfügbar ist
        sem_wait(&strecke);
        printf("Fahrterlaubnis für %s wurde jetzt erteilt!\n", zug->zugname);
        sleep(zug->dauer);  // Simuliert die Zeit, die der Zug für die Strecke benötigt
        printf("%s hat die Strecke verlassen.\n", zug->zugname);

        // Semaphore erhöhen, da der Zug die Strecke verlassen hat
        sem_post(&strecke);
    }

    return NULL;
}

int main() {
    // Initialisieren der Semaphore mit 3 freien Plätzen (für 3 Züge)
    sem_init(&strecke, 0, 3);

    // Liste der Züge (maximal 10 Züge)
    struct Zug zuege[10] = {
        {"Zug 1", rand() % 3 + 1},
        {"Zug 2", rand() % 3 + 1},
        {"Zug 3", rand() % 3 + 1},
        {"Zug 4", rand() % 3 + 1},
        {"Zug 5", rand() % 3 + 1},
        {"Zug 6", rand() % 3 + 1},
        {"Zug 7", rand() % 3 + 1},
        {"Zug 8", rand() % 3 + 1},
        {"Zug 9", rand() % 3 + 1},
        {"Zug 10", rand() % 3 + 1}
    };

    // Threads für jeden Zug
    pthread_t threads[10];

    // Erstellen der Threads für jeden Zug
    for (int i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, befahreStrecke, &zuege[i]);
    }

    // Warten auf das Beenden aller Threads
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    // Zerstören der Semaphore
    sem_destroy(&strecke);

    return 0;
}
