#include <mpi.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

struct llist {
    int value;
    struct llist* next;
} *list, *primes;

int fastexpmod(int a, int p, int m)
{
    if (p == 0) return 1;
    if (p == 1) return a % m;
    else {
        int n = p >> 1;
        int odd = p & 1;
        n = fastexpmod((a * a) % m, n, m);
        if (odd)
            return (n * a) % m;
        else n;
    }
}

int is_prime(int value) {
    int i;
    if (value < 2) return 0;
    for (i = 0; i < 50; ++i)
        if ((fastexpmod(rand() % value, value - 1, value)) != 1)
            return 0;

    struct llist* last = primes;
    primes = calloc(sizeof(struct llist), 0);
    primes->next = last;
    primes->value = value;
    
    return 1;

}



int main(int argc, char**argv)
{
    int rank;
    int size;
    struct timeval tv;
    int value;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    list = NULL;

    gettimeofday(&tv, NULL);

    srand(tv.tv_usec);
    
    while ((value = rand() %  size) != 0) {
        struct llist* last = list;
        list = calloc(sizeof(struct llist), 0);
        list->next = last;
        list->value = value;

        
    }
    
    do { 
        struct llist* next = list->next;
        value = list->value;
        is_prime(value);
        free(list);
        list = next;
    } while (list != NULL);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    


}
