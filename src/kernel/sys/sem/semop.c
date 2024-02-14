#include <nanvix/const.h>
#include <nanvix/pm.h>
#include <sys/types.h>
#include <errno.h>
#include <nanvix/sem.h>

PUBLIC int sys_semop(int semid, int op) {

    /* Work in progress, this is probably wrong in some way*/

    if (semid < 0 || semid >= MAX_SEMS)
    {
        return -1;
    }
    if (op == 0)
        return -1;
    
    semaphore_t selected_sem = sems[semid];

    if (op == 0) return -1;

    int up = (op > 0) ? 1 : 0;


    disable_interrupts();

    if (up) {
        selected_sem.val++;
        if (selected_sem.val >= 0) {
            wakeup(&selected_sem.processes_queue);
        }
    } else {
        selected_sem.val--;
        if (selected_sem.val < 0) {
            sleep(&selected_sem.processes_queue, curr_proc->priority);
        }
    }

    enable_interrupts();

    return 0;
}