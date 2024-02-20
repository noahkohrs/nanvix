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
    #include <nanvix/config.h>

    typedef struct
    {
        int val;         /**< Semaphore value. */
        unsigned int key;   /**< Semaphore PID.   */
        struct process processes_queue[PROC_MAX]; /**< Processes queue. */
        int valid; /**< Semaphore is valid. */
    } semaphore_t;

    /**
     * @brief Semaphore table.
    */
    EXTERN semaphore_t sems[SEM_MAX];  /**< Semaphore table. */

    EXTERN void sem_init(void);
#endif /* NANVIX_SEM_H_ */