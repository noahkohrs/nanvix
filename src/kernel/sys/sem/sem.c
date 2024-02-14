#include <nanvix/sem.h>
/**
 * @brief Semaphore table.
 */
PUBLIC semaphore_t sems[MAX_SEMS];

/**
 * @brief Initializes the semaphore table.
 */
PUBLIC void sem_init(void) {
    int i;

    /* Initialize the semaphore table. */
    for (i = 0; i < MAX_SEMS; i++)
    {
        sems[i].val = 0 ; 
        sems[i].key = 0 ;
        sems[i].processes_queue = NULL ;
        sems[i].valid = 0 ;
    }
}