#include <nanvix/const.h>
#include <nanvix/pm.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/sem.h>

PUBLIC int sys_semop(int semid, int op) {
    semaphore_t sem = sems[semid];
    int i;
    return -1;
}