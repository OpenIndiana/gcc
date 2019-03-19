/* os-unix-sysdep.c                  -*-C-*-
 *
 *************************************************************************
 *
 *  @copyright
 *  Copyright (C) 2009-2013, Intel Corporation
 *  All rights reserved.
 *  
 *  @copyright
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *    * Neither the name of Intel Corporation nor the names of its
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *  
 *  @copyright
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 *  WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************
 *
 * This file contains generic implementations of system-specific code for
 * Unix-based systems
 */

#include "os.h"
#include "sysdep.h"

#if defined(__sparc) || defined(__sparc__)
# include <sys/times.h>
# include <limits.h>
#endif

/*
 * The cycle counter is used for debugging.  This funciton is only called if
 * CILK_PROFILE is defined when the runtime is built.
 */
COMMON_SYSDEP unsigned long long __cilkrts_getticks(void)
{
#if defined(__sparc) || defined(__sparc__)
  struct tms t;

  if (times(&t) != (clock_t) -1)
    return (unsigned long long) (t.tms_utime + t.tms_stime);

  return 0ULL;
#else
#   warning "unimplemented cycle counter"
    return 0;
#endif
}

#if defined(__sparc) || defined(__sparc__)
#include "os-fence.h"
/*
 * SPARC V9 __cilkrts_fence implementation
 */
COMMON_SYSDEP void __cilkrts_fence(void)
{
  __asm__ __volatile__ ("membar #LoadLoad | #LoadStore | #StoreStore | #StoreLoad" ::: "memory");
}
#endif

/*
 * A "short pause" - called from the Cilk runtime's spinloops.
 */
COMMON_SYSDEP void __cilkrts_short_pause(void)
{
#if defined(__sparc) || defined(__sparc__)
  /* spin around for 8 cycles */
  __asm__ __volatile__("rd %ccr, %g0");
  __asm__ __volatile__("rd %ccr, %g0");
  __asm__ __volatile__("rd %ccr, %g0");
  __asm__ __volatile__("rd %ccr, %g0");
#else
#   warning __cilkrts_short_pause empty
#endif
}

/*
 * Interlocked exchange - used to implement the Cilk runtime's spinloops
 */
COMMON_SYSDEP int __cilkrts_xchg(volatile int *ptr, int x)
{
    x = __sync_lock_test_and_set(ptr, x);
    return x;
}


/*
 * Restore the floating point state that is stored in a stack frame at each
 * spawn.  This should be called each time a frame is resumed.
 *
 * Only valid for IA32 and Intel64 processors.
 */
void restore_x86_fp_state (__cilkrts_stack_frame *sf)
{
}


/*
 * Save the floating point state to the __cilkrts_stack_frame at each spawn.
 *
 * Architecture-specific - Should only be needed on IA32 and Intel64
 * processors.
 */
void sysdep_save_fp_ctrl_state(__cilkrts_stack_frame *sf)
{
}

