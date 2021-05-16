/*
 * mgr_version.c
 *
 *  Created on: May 16, 2021
 *      Author: Zviad
 */

#include "mgr_version.h"
#include "version.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mw_redefines.h"
#include "macrodefs.h"

static mikiot_versions_t versions = { 0 };

mikiot_versions_t* mgr_versions_get() {
    /* Bellow check is for only once initialization */
    if (versions.fw_version.joined == 0) {
        versions.fw_version.joined = allign_8bits_in32(VERSION_MAJOR,
                VERSION_MINOR, VERSION_PATCH, 0);
        versions.fw_code_name = VERSION_CODE_NAME;
        versions.fw_release_date =
        VERSION_RELEASE_DATE;
        versions.rtos_version.joined = allign_8bits_in32(
                tskKERNEL_VERSION_MAJOR, tskKERNEL_VERSION_MINOR,
                tskKERNEL_VERSION_BUILD, 0);
        versions.hal_version.joined = mw_hal_version();
    }

    return &versions;
}
