#include <pthread.h>
#include <stdio.h>
#include <stdatomic.h> 


void* thread_func(void* arg) {
	
    atomic_int* mydata = (atomic_int*)arg;
    // addiere  10000-mal die 1 atomar
    for (int i = 0; i < 10000; i++) {
        atomic_fetch_add(mydata, 1);  // atomare addition
    }
	return NULL;
}

int main() {
	// Reserviere Speicher für die Thread-Handles
	pthread_t myThreads[10];
	// Reserviere Speicher für die benutzerdefinierten Daten
	// Zu dieser Zahl werden 10 Threads 10000-mal die 1 dazuaddieren
	int mydata = 0;

	// Erzeuge 10 Threads
	for (int i = 0; i < 10; i++) {
		printf("Erzeuge Thread %d\n", i);
		// Erzeuge den Thread
		pthread_create(&myThreads[i], NULL, &thread_func, &mydata);
	}

	// Warte auf die Beendigung aller Threads
	for (int i = 0; i < 10; i++) {
		pthread_join(myThreads[i], NULL);
		printf("Thread %d wurde beendet\n", i);
	}

	// Gebe das Endergebnis aus
	// Wenn 10 Threads 10000-mal die 1 dazuaddieren, müsste doch 100000 rauskommen
	// Tut es das?
	// Wenn es das tut, ist das immer der Fall?
	printf("mydata = %d\n",  mydata);

	return 0;
}

