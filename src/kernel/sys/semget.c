#include <nanvix/const.h>
#include <nanvix/pm.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/sem.h>

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
            sems[i].proc = curr_proc ;
            return i ;
        }
    }

    errno = ENOSPC ;
    return -1 ;
}


