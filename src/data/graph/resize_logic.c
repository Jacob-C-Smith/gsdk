#include <stdio.h>
#include <string.h> // Include for memcpy

#define MAX_SIZE 100 // Maximum size for the matrix

// Function to print the matrix represented as a 1D array
void printMatrix(int matrix[], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i * size + j]); // Convert 2D to 1D index
        }
        printf("\n");
    }
}

// Function to grow the matrix
void padMatrix(int matrix[], int *currentSize, int newSize) {
    if (newSize > MAX_SIZE) {
        printf("New size exceeds maximum allowed size.\n");
        return;
    }
    for (int i = 0; i < *currentSize; i++) {
        memcpy(&matrix[i * newSize], &matrix[i * (*currentSize)], sizeof(int) * (*currentSize));
    }

    // Fill new spaces with zeros
    for (int i = 0; i < newSize; i++) {
        for (int j = 0; j < newSize; j++) {
            if (i >= *currentSize || j >= *currentSize) {
                matrix[i * newSize + j] = 0; // Fill with zeros
            }
        }
    }

    // Copy original values into the new positions

    // Update the current size
    *currentSize = newSize;
}

int main() {
    int size = 3; // Initial size of the matrix
    int matrix[MAX_SIZE]; // 1D array for the matrix

    // Initialize the original 2x2 matrix in 1D array
    matrix[0] = 1; matrix[1] = 2; matrix[2] = 3;
    matrix[3] = 4; matrix[4] = 5; matrix[5] = 6;
    matrix[6] = 7; matrix[7] = 8; matrix[8] = 9;

    printf("Original Matrix:\n");
    printMatrix(matrix, size);

    // Grow the matrix to size 4
    int newSize = 6;
    padMatrix(matrix, &size, newSize);

    printf("\nPadded Matrix:\n");
    printMatrix(matrix, newSize);

    return 0;
}
