#include <nanvix/const.h>
#include <nanvix/pm.h>
#include <sys/types.h>
#include <errno.h>
#include <nanvix/sem.h>

#define GETVAL 0
#define SETVAL 1
#define IPC_RMID 3

PUBLIC int sys_semctl(int semid, int num, int cmd) {
    semaphore_t selected_sem = sems[semid];
    switch (cmd) {
        case GETVAL:
            return selected_sem.val;
        case SETVAL:
            selected_sem.val = num;
            return 0;
        case IPC_RMID:
            selected_sem.val = 0;
            selected_sem.key = 0;
            selected_sem.processes_queue = NULL;
            selected_sem.valid = 0;
            return 0;
    }
}