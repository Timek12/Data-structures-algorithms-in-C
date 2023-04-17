#include <stdio.h>
#include "circular_buffer.h"

int main() {
    circular_buffer_create(NULL, 0);
    circular_buffer_destroy(NULL);
    int size;
    printf("Input buffer size:");
    if(scanf("%d", &size) != 1){
        printf("Incorrect input");
        return 1;
    }


    if(size <= 0){
        printf("Incorrect input data");
        return 2;
    }

    struct circular_buffer_t* cb_ptr;

    int check = circular_buffer_create_struct(&cb_ptr, size);
    if(check != 0){
        printf("Failed to allocate memory");
        return 8;
    }

    while(1)
    {
        int choice;
        printf("What do you want to do?");
        if(scanf("%d", &choice) != 1){
            circular_buffer_destroy_struct(&cb_ptr);
            printf("Incorrect input");
            return 1;
        }

        switch(choice)
        {
            case 0:
                circular_buffer_destroy_struct(&cb_ptr);
                return 0;

            case 1:
                printf("Podaj liczbe:");
                int value;
                if(scanf("%d", &value) != 1){
                    circular_buffer_destroy_struct(&cb_ptr);
                    printf("Incorrect input");
                    return 1;
                }

                circular_buffer_push_back(cb_ptr, value);
                break;

            case 2:
                if(circular_buffer_empty(cb_ptr))
                    printf("Buffer is empty\n");
                else
                    printf("%d\n", circular_buffer_pop_back(cb_ptr, (int *) 0));
                break;

            case 3:
                if(circular_buffer_empty(cb_ptr))
                    printf("Buffer is empty\n");
                else
                    printf("%d\n", circular_buffer_pop_front(cb_ptr, (int *) 0));
                break;

            case 4:
                if(circular_buffer_empty(cb_ptr))
                    printf("Buffer is empty\n");
                else
                    circular_buffer_display(cb_ptr);
                break;

            case 5:
                if(circular_buffer_empty(cb_ptr))
                    printf("1\n");
                else
                    printf("0\n");
                break;

            case 6:
                if(circular_buffer_full(cb_ptr))
                    printf("1\n");
                else
                    printf("0\n");
                break;

            default:
                printf("Incorrect input data\n");
                break;
        }
    }

}
