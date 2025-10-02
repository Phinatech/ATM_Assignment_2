#include <stdio.h>

int main() {
    // Declare and initialize an array
    int numbers[5] = {10, 20, 30, 40, 50};


    printf("Value of second element: %d\n", numbers[1]); 

    // Print the address of the first and second elements
    printf("Address of first element: %p\n", (void*)&numbers[0]); 
    printf("Address of second element: %p\n", (void*)&numbers[1]); 

    return 0;
} 
