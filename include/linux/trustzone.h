/*
 * Copyright (C) 2014 Javier González <javier@javigon.com>
 *
 * Generic device driver for ARM TrustZone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __LINUX_TRUSTZONE_H__
#define __LINUX_TRUSTZONE_H__

#include <asm/siginfo.h>

/*
 * Default chip num is the value of a trustzone id.
 */
#define TRUSTZONE_ANY_NUM 0xFFFF

/**
 * enum tz_param_type - type of the trustzone parameter
 * @TZ_UINT8: equivalent to uint8_t in the TEE
 * @TZ_UINT32: equivalent to uint32_t in the TEE
 * @TZ_GENERIC: parameter sent through shared memory. It is the TEEs
 * responsibility to know the type of the parameter. This is used for specific
 * commands that are known to the client and the TEE
 */
enum tz_param_type {
	TZ_UINT8 = 0x0,
	TZ_UINT32,
	TZ_GENERIC
};

/**
 * enum tz_param_purpose - purpose of the trustzone parameter
 * @TZ_PARAM_IN: input parameter
 * @TZ_PARAM_OUT: output parameter
 * @TZ_PARAM_INOUT: input and Output parameter
 *
 * Parameters sent to the TEE must be identified so that the TEE knows how to
 * treat them.
 */
enum tz_param_purpose {
	TZ_PARAM_IN = 0x0,
	TZ_PARAM_OUT,
	TZ_PARAM_INOUT
};

/**
 * enum tz_services - services common to the different TEEs supported in the
 * Linux kernel. While user space applications might define their own set of
 * commands base on knowledge of the TEE being accepted commands, kernel
 * submodules support the following set of trusted services.
 *
 * TODO: Define these services as in OP-TEE and Open Virtualization.
 */
enum tz_services {
	TZ_SECURE_PRIMITIVE_SVC = 0x0
};

/**
 * struct trustzone_parameter - parameter to be sent to the TEE
 * @type: parameter type as in tz_param_type
 * @inout: parameter purpose as in tz_param_purpose
 * @value: parameter value. Type is used to cast parameter in TEE
 * @size: parameter size
 * @nxt: pointer to next parameter. In last parameter should be NULL
 *
 * The type and number of parameters sent to the TEE might vary from TEE
 * framework to TEE framework. This structure allows to hide TEE-specific
 * parameter definition and delegates this responsability entirely to the TEE.
 * Some specific TEE APIs such as Global Platform's API limit the numer of
 * parameters to 4, the layer implementing such APIs are responsible to
 * introduce this policy. The goal of this interface is to be as generic as
 * possible.
 */
struct trustzone_parameter {
	uint8_t type;
	uint8_t inout;
	void *value;
	uint32_t size;
	struct trustzone_parameter *nxt;
};

/**
 * struct trustzone_parameter_list - list of parameters to be sent to the TEE
 * @trustzone_parameter: actual list of parameters ended in NULL
 * @n_params: number of parameters
 *
 * List of parameters to be sent to the TEE as in trustzone_parameter.
 */
struct trustzone_parameter_list {
	struct trustzone_parameter *params;
	uint8_t n_params;
};

/**
 * struct trustzone_cmd - command to be sent to the TEE
 * @cmd: command identifier as defined in drivers/sechw/trustzone/smc_id.h or in
 * each specific TEE smc_id.h.
 *
 * Command that is interpreted by the TEE and triggers a trusted service to
 * execute. Some of these commands are TEE specific. Ideally, a number of
 * trusted services are common to the differen TEEs. The current list of
 * services is defined in tz_services.
 */
struct trustzone_cmd {
	int cmd;
};

/**
 * struct trustzone_session - TEE session to be maintained by TEE client
 * @impl_session - TEE session as defined by each TEE
 *
 * TrustZone operates in a session-based fashion. This is, TEE clients open a
 * session and send commands to the TEE, which executes trusted services, until
 * the session is closed. Several sessions can execute concurrently.
 * This session is TEE-specific. In this way, this structure abstracts the
 * session and at the same time allows the TEE client to keep it in order to
 * identify the session. Each TEE cast this session in their implementation in
 * order to match their specific session structures.
 */
struct trustzone_session{
	void *impl_session;
};

/*
 * TrustZone Generic Operations
 *
 * TODO: Include tz_start, tz_stop? Only for user space?
 */
extern int tz_open(u32, struct trustzone_session*, u8);
extern int tz_close(u32, struct trustzone_session*);
extern int tz_invoke(u32, struct trustzone_session*, struct trustzone_cmd*,
		struct trustzone_parameter_list*);
extern int tz_atomic_operation(u32, u8, struct trustzone_cmd*,
		struct trustzone_parameter_list*);

#endif
