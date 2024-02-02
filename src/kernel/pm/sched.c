/*
 * Copyright(C) 2011-2016 Pedro H. Penna   <pedrohenriquepenna@gmail.com>
 *              2015-2016 Davidson Francis <davidsondfgl@hotmail.com>
 *
 * This file is part of Nanvix.
 *
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#include <nanvix/clock.h>
#include <nanvix/const.h>
#include <nanvix/hal.h>
#include <nanvix/pm.h>
#include <signal.h>
#include <nanvix/klib.h>
#include <nanvix/syscall.h>

PRIVATE struct process * __priorityScheduling();
PRIVATE struct process * __fifoScheduling();
PRIVATE struct process * __randomScheduling();
PRIVATE struct process * __roundRobinScheduling();
PRIVATE struct process * __lotteryScheduling();


	/**
	 * @brief Schedules a process to execution.
	 *
	 * @param proc Process to be scheduled.
	 */
	PUBLIC void sched(struct process *proc)
{
	proc->state = PROC_READY;
	proc->counter = 0;
}

/**
 * @brief Stops the current running process.
 */
PUBLIC void stop(void)
{
	curr_proc->state = PROC_STOPPED;
	sndsig(curr_proc->father, SIGCHLD);
	yield();
}

/**
 * @brief Resumes a process.
 *
 * @param proc Process to be resumed.
 *
 * @note The process must stopped to be resumed.
 */
PUBLIC void resume(struct process *proc)
{
	/* Resume only if process has stopped. */
	if (proc->state == PROC_STOPPED)
		sched(proc);
}


/**
 * @brief Yields the processor.
 */
PUBLIC void yield(void)
{
	struct process *p;	  /* Working process.     */
	struct process *next; /* Next process to run. */

	/* Re-schedule process for execution. */
	if (curr_proc->state == PROC_RUNNING)
		sched(curr_proc);

	/* Remember this process. */
	last_proc = curr_proc;

	/* Check alarm. */

	// Ignore
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		/* Skip invalid processes. */
		if (!IS_VALID(p))
			continue;

		/* Alarm has expired. */
		if ((p->alarm) && (p->alarm < ticks))
			p->alarm = 0, sndsig(p, SIGALRM);
	}


	// Possible implementations :

	// __roundRobinScheduling()
	// __randomScheduling()     	(warning : sometime it seems to stop working for an unknown reason at the moment)
	// __lotteryScheduling()
	// __fifoScheduling()   		(the default one)
	// __priorityScheduling()

	next = __priorityScheduling();

	/* Switch to next process. */
	next->priority = PRIO_USER;
	next->state = PROC_RUNNING;
	next->counter = PROC_QUANTUM;
	if (curr_proc != next)
		switch_to(next);
}

PRIVATE void __incrementCounters(struct process *next)
{
	struct process *p;
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		if (p->state != PROC_READY)
			continue;
		if (p != next)
			p->counter++;
	}
}

PRIVATE int __getProcessWeight(struct process *p)
{
	// Arbitrary weight function
	int w = (-p->priority + 80)*2 + p->counter;
	// Statement that should never be false, but just in case if we change the weight function,
	// we should not have a negative weight
	return w > 0 ? w : 1;
}

__attribute__((unused))
PRIVATE struct process *
__roundRobinScheduling()
{
	/* Choose a process to run next. */
	struct process *next = IDLE;
	struct process *p;

	p = (curr_proc);

	do
	{
		if (p == LAST_PROC)
			p = FIRST_PROC;
		else
			p++;
		if (p->state == PROC_READY)
		{
			next = p;
			break;
		}
	} while (p != curr_proc);

	__incrementCounters(next);

	return next;
}

__attribute__((unused))
PRIVATE struct process *
__randomScheduling()
{

	struct process *next = IDLE;
	struct process *p;

	int nprocsReady = 0;
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		if (p->state == PROC_READY)
		{
			p->counter++;
			nprocsReady++;
		}
	}

	if (nprocsReady == 0)
	{
		next = IDLE;
	}
	else
	{
		int random = (krand() % (nprocsReady)) + 1;
		int i = 0;
		for (p = FIRST_PROC; p <= LAST_PROC; p++)
		{
			if (p->state == PROC_READY)
			{
				i++;
				if (i == random)
				{
					next = p;
					next->counter--;
					break;
				}
			}
		}
	}

	return next;
}

__attribute__((unused))
PRIVATE struct process *
__lotteryScheduling()
{
	struct process *p;
	struct process *next;
	int ntickets = 0;
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		if (p->state == PROC_READY)
		{
			ntickets += __getProcessWeight(p);
			p->counter++;
		}
	}
	if (ntickets == 0)
	{
		next = IDLE;
	}
	else
	{
		int random = (krand() % ntickets) + 1;
		int i = 0;
		for (p = FIRST_PROC; p <= LAST_PROC; p++)
		{
			if (p->state == PROC_READY)
			{
				i += __getProcessWeight(p);
				if (i >= random)
				{
					next = p;
					break;
				}
			}
		}
	}
	return next;
}

__attribute__((unused))
PRIVATE struct process *
__fifoScheduling()
{
	/* Choose a process to run next. */
	struct process *next = IDLE;
	struct process *p;

	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		/* Skip non-ready process. */
		if (p->state != PROC_READY)
			continue;

		/*
		 * Process with higher
		 * waiting time found.
		 */
		if (p->counter > next->counter)
		{
			next->counter++;
			next = p;
		}

		/*
		 * Increment waiting
		 * time of process.
		 */
		else
			p->counter++;
	}

	return next;
}

__attribute__((unused))
PRIVATE struct process *
__priorityScheduling()
{
	struct process *next = IDLE;
	struct process *p;

	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		/* Skip non-ready process. */
		if (p->state != PROC_READY || p == IDLE)
			continue;

		/*
		 * Process with higher
		 * priority found.
		 */
		if (p->nice < next->nice || next == IDLE)
		{
			p->counter++;
			next = p;
		}
		else if (p->nice == next->nice &&
				 p->ktime + p->utime < next->ktime + next->utime)
		{
			p->counter++;
			next = p;
		}
		else
		{
			p->counter++;
		}
	}

	return next;
}