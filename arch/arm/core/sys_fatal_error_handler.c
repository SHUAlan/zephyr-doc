/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief ARM Cortex-M system fatal error handler
 *
 * This module provides the _SysFatalErrorHandler() routine for Cortex-M
 * platforms.
 */

#include <kernel.h>
#include <toolchain.h>
#include <sections.h>
#include <kernel_structs.h>
#include <misc/printk.h>

/**
 *
 * @brief Fatal error handler
 *
 * This routine implements the corrective action to be taken when the system
 * detects a fatal error.
 *
 * This sample implementation attempts to abort the current thread and allow
 * the system to continue executing, which may permit the system to continue
 * functioning with degraded capabilities.
 *
 * System designers may wish to enhance or substitute this sample
 * implementation to take other actions, such as logging error (or debug)
 * information to a persistent repository and/or rebooting the system.
 *
 * @param reason fatal error reason
 * @param pEsf pointer to exception stack frame
 *
 * @return N/A
 */
FUNC_NORETURN void _SysFatalErrorHandler(unsigned int reason,
					 const NANO_ESF *pEsf)
{
	ARG_UNUSED(reason);
	ARG_UNUSED(pEsf);

#if !defined(CONFIG_SIMPLE_FATAL_ERROR_HANDLER)
	if (k_is_in_isr() || _is_thread_essential()) {
		printk("Fatal fault in %s! Spinning...\n",
		       k_is_in_isr() ? "ISR" : "essential thread");
		for (;;)
			; /* spin forever */
	}
	printk("Fatal fault in thread %p! Aborting.\n", _current);
	k_thread_abort(_current);

#else
	for (;;) {
		k_cpu_idle();
	}
#endif

	CODE_UNREACHABLE;
}
