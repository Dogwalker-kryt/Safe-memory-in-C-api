# Safe-memory-in-C-api
make the memory management easier an safer

## Info
- this C lang api is there to amke memory management easier and safer
- the funktions are so designed so you dont have to make extra things for cheking and stuff



## Example
### wihtout Safemem api

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int* arr = (int*) malloc(5 * sizeof(int));
    if (!arr) {
        fprintf(stderr, "malloc failed\n");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        arr[i] = i * 2;
    }

    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}
```c
with Safemem api

#include <stdio.h>
#include "safemem.h"

int main() {
    int* arr = (int*) sm_malloc(5 * sizeof(int));
    
    // no NULL-check required, sm_malloc ends the programm if erros

    for (int i = 0; i < 5; i++) {
        arr[i] = i * 2;
    }

    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    sm_free(arr);

    // if you forgot to use sm_free, you can find leaks with sm_report_leaks
    sm_report_leaks();

    return 0;
}


