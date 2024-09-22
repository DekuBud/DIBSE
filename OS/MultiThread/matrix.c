#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

/* R für Row und C für Column */
#define R_A 10
#define C_A 20
#define R_B 20
#define C_B 12
#define R_C R_A
#define C_C C_B

/* Macro für 2D Array auf 1D umrechnen */
#define INDEX(row, col, num_cols) ((row) * (num_cols) + (col))

// Für Thread-Funktion
typedef struct {
	int row;
	int col;
} ThreadData;

// Matrizen
int *A, *B, *C;

// Berechnung einer Zelle
void* compute_cell(void *arg) {
	ThreadData *data = (ThreadData*) arg;
	int row = data->row;
	int col = data->col;

	C[INDEX(row, col, C_C)] = 0;
	for (int k = 0; k < C_A; ++k) {
		C[INDEX(row, col, C_C)] += A[INDEX(row, k, C_A)]
				* B[INDEX(k, col, C_B)];
	}

	return NULL; // Thread beendet sich, speicher wird später freigegeben
}

// Ausgabe + Berechnung von min, max und sum
void printMatrix(int *matrix, int rows, int cols) {
	int min = INT_MAX;
	int max = INT_MIN;
	int sum = 0;

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			int value = matrix[INDEX(i, j, cols)];
			printf("%d\t", value);
			if (value < min)
				min = value;
			if (value > max)
				max = value;
			sum += value;
		}
		printf("\n");
	}

	printf("Minimaler Wert: %d\n", min);
	printf("Maximaler Wert: %d\n", max);
	printf("Summe aller Werte: %d\n", sum);
}

int main() {

	A = (int*) malloc(R_A * C_A * sizeof(int));
	B = (int*) malloc(R_B * C_B * sizeof(int));
	C = (int*) malloc(R_C * C_C * sizeof(int));

	// Check, ob Speicher erfolgreich allokiert wurde
	if (A == NULL || B == NULL || C == NULL) {
		printf("Speicherzuweisung fehlgeschlagen!\n");
		free(A);
		free(B);
		free(C);
		return 1;
	}

	// Befülle A und B mit Zufallswerten
	for (int i = 0; i < R_A; ++i) {
		for (int j = 0; j < C_A; ++j) {
			A[INDEX(i, j, C_A)] = rand() % 10;  // Zufallswerte zwischen 0 und 9
		}
	}

	for (int i = 0; i < R_B; ++i) {
		for (int j = 0; j < C_B; ++j) {
			B[INDEX(i, j, C_B)] = rand() % 10;  // Zufallswerte zwischen 0 und 9
		}
	}

	// Array von pthread_t zur Speicherung der Thread-IDs und von ThreadData-Pointern
	pthread_t threads[R_C * C_C];
	ThreadData *thread_data[R_C * C_C]; // Array, um die ThreadData-Pointer zu speichern

	// Threads erzeugen für Berechnung von C
	int thread_count = 0;
	for (int i = 0; i < R_C; ++i) {
		for (int j = 0; j < C_C; ++j) {
			thread_data[thread_count] = (ThreadData*) malloc(
					sizeof(ThreadData));  // Speicher für ThreadData allokieren
			thread_data[thread_count]->row = i;
			thread_data[thread_count]->col = j;
			pthread_create(&threads[thread_count], NULL, compute_cell,
					thread_data[thread_count]);
			thread_count++;
		}
	}

	// Auf alle Threads warten und danach Speicher für ThreadData freigeben
	for (int i = 0; i < thread_count; ++i) {
		pthread_join(threads[i], NULL);
		free(thread_data[i]);  // Speicher für ThreadData freigeben
	}

	// Matrizen ausgeben
	printf("Matrix A:\n");
	printMatrix(A, R_A, C_A);

	printf("\nMatrix B:\n");
	printMatrix(B, R_B, C_B);

	printf("\nMatrix C (Ergebnis):\n");
	printMatrix(C, R_C, C_C);

	// Speicher freigeben
	free(A);
	free(B);
	free(C);

	return 0;
}
