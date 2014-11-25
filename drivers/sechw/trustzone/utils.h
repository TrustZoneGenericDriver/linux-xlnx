/**
 * Copyright (C) 2014 Javier González
 *
 * Device driver for ARM TrustZone.
 */

#ifndef __TZ_UTILS_H
#define __TZ_UTILS_H

#include <linux/err.h>

/* TODO: Move this to the proper place */
#define TZ_CONFIG_DEBUG
/* #define TZ_CONFIG_DEBUG_COMMENTS */

#ifdef OTZ_DEBUG_COMMENTS
(#define OTZ_DBG(fmt, a...) pr_debug("Sierra_OTZ:<%s>,%d - " fmt "\n",
			__func__, __LINE__, ##a))
#else
#define OTZ_DBG(fmt, a...)
#endif

#ifdef TZ_CONFIG_DEBUG
	#define STATIC
	#ifdef TZ_CONFIG_DEBUG_COMMENTS
		(#define TZ_DBG(fmt, a...) pr_debug("TZ_DRV:<%s>,%d - "
			fmt "\n", __func__, __LINE__, ##a))
	#else
		#define TZ_DBG(fmt, a...)
	#endif /* TZ_CONFIG_DEBUG_COMMENTS */
#else
	#define STATIC static
#endif /* TZ_CONFIG_DEBUG */
#endif /* __TZ_UTILS_H */
