#ifndef NANVIX_SEM_H_
#define NANVIX_SEM_H_

    #include <nanvix/const.h>
    #include <sys/stat.h>
    #include <sys/times.h>
    #include <sys/types.h>
    #include <sys/utsname.h>
    #include <signal.h>
    #include <ustat.h>
    #include <utime.h>

    typedef struct
    {
        int val;         /**< Semaphore value. */
        unsigned int key;   /**< Semaphore PID.   */
        struct proc *proc;  /**< Semaphore owner.  */
    } semaphore_t;

    #define MAX_SEMS 64     /**< Maximum number of semaphores. */

    /**
     * @brief Semaphore table.
    */
    EXTERN semaphore_t sems[MAX_SEMS];  /**< Semaphore table. */

    /*
     * Get a semaphore
     */
    EXTERN int sys_semget(unsigned key);

    /*
     * Control operations on a semaphore
     */
    EXTERN int sys_semctl(int semid, int num, int cmd);

    /*
     * Perform operations on a semaphore
     */
    EXTERN int sys_semop(int semid, int op);
#endif /* NANVIX_SEM_H_ */