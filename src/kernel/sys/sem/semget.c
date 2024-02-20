#include <nanvix/const.h>
#include <nanvix/pm.h>
#include <nanvix/sem.h>

PUBLIC int sys_semget(unsigned key) {
    int i ;

    // Search for the semaphore

    for (i = 0; i < SEM_MAX; i++) {
        if (sems[i].key == key) {
            return i ;
        }
    }

    for (i = 0; i < SEM_MAX; i++) {
        if (sems[i].proc == NULL) {
            sems[i].key = key ;
            sems[i].val = 0 ;
            // Array of processes
            sems[i].proc = struct process[MAX_PROC] ;
            sems[i].valid = 1 ;
            return i ;
        }
    }

    return -1 ;
}


