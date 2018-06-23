/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#ifndef _GOS_CHIBIOS_H
#define _GOS_CHIBIOS_H

#if GFX_USE_OS_CHIBIOS

#if GFX_COMPAT_V2
	// This shouldn't be needed but some people are complaining
	// about TRUE/FALSE redefinition so we fix it here.
	// ChibiOS will define them in its own header files.
	#undef TRUE
	#undef FALSE
#endif

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Type definitions                                                          */
/*===========================================================================*/

/**
 * TIME_IMMEDIATE, TIME_INFINITE
 * are already defined by ChibiOS
 */

typedef systime_t	delaytime_t;
typedef systime_t	systemticks_t;
typedef cnt_t		semcount_t;
typedef msg_t		threadreturn_t;
typedef tprio_t		threadpriority_t;

#define MAX_SEMAPHORE_COUNT			((semcount_t)(((unsigned long)((semcount_t)(-1))) >> 1))
#define LOW_PRIORITY				LOWPRIO
#define NORMAL_PRIORITY				NORMALPRIO
#define HIGH_PRIORITY				HIGHPRIO

#define DECLARE_THREAD_STACK(name, sz)			WORKING_AREA(name, sz)
#define DECLARE_THREAD_FUNCTION(fnName, param)	threadreturn_t fnName(void *param)
#define THREAD_RETURN(retval)					return retval

#if CH_KERNEL_MAJOR == 2
	typedef struct {
		Semaphore	sem;
		semcount_t	limit;
	} gfxSem;

	typedef Mutex		gfxMutex;
	typedef Thread*		gfxThreadHandle;
#elif (CH_KERNEL_MAJOR == 3) || (CH_KERNEL_MAJOR == 4)
	#undef DECLARE_THREAD_STACK
	#define DECLARE_THREAD_STACK(a, b)  THD_WORKING_AREA(a, b)

	typedef struct {
		semaphore_t	sem;
		semcount_t	limit;
	} gfxSem;

	typedef mutex_t		gfxMutex;
	typedef thread_t*	gfxThreadHandle;
#endif


/*===========================================================================*/
/* Function declarations.                                                    */
/*===========================================================================*/

// First the kernel version specific ones
#if CH_KERNEL_MAJOR == 2
	#define gfxSystemTicks()			chTimeNow()
	#define gfxMutexInit(pmutex)		chMtxInit(pmutex)
	#define gfxMutexExit(pmutex)		chMtxUnlock()
	#define gfxExit()					chSysHalt()
	#define gfxHalt(msg)				{ chDbgPanic(msg); chSysHalt(); }
#elif (CH_KERNEL_MAJOR == 3) || (CH_KERNEL_MAJOR == 4)
	#define gfxSystemTicks()			chVTGetSystemTimeX()
	#define gfxMutexInit(pmutex)		chMtxObjectInit(pmutex)
	#define gfxMutexExit(pmutex)		chMtxUnlock(pmutex)
	#define gfxExit()					osalSysHalt("gfx_exit")
#define gfxHalt(msg)					{ chSysHalt(msg); }
#endif

#define gfxAlloc(sz)				chHeapAlloc(0, sz)
#define gfxFree(ptr)				chHeapFree(ptr)
#define gfxYield()					chThdYield()
#define gfxMillisecondsToTicks(ms)	MS2ST(ms)
#define gfxSystemLock()				chSysLock()
#define gfxSystemUnlock()			chSysUnlock()
#define gfxMutexDestroy(pmutex)		(void)pmutex
#define gfxMutexEnter(pmutex)		chMtxLock(pmutex)
void *gfxRealloc(void *ptr, size_t oldsz, size_t newsz);
void gfxSleepMilliseconds(delaytime_t ms);
void gfxSleepMicroseconds(delaytime_t ms);
void gfxSemInit(gfxSem *psem, semcount_t val, semcount_t limit);
void gfxSemDestroy(gfxSem *psem);
gBool gfxSemWait(gfxSem *psem, delaytime_t ms);
gBool gfxSemWaitI(gfxSem *psem);
void gfxSemSignal(gfxSem *psem);
void gfxSemSignalI(gfxSem *psem);
gfxThreadHandle gfxThreadCreate(void *stackarea, size_t stacksz, threadpriority_t prio, DECLARE_THREAD_FUNCTION((*fn),p), void *param);
#define gfxThreadWait(thread)		chThdWait(thread)
#define gfxThreadMe()				chThdSelf()
#define gfxThreadClose(thread)		(void)thread

#endif /* GFX_USE_OS_CHIBIOS */
#endif /* _GOS_CHIBIOS_H */
