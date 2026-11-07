/* ============================================================
 * @file    api_handler.h
 * @brief   REST API JSON response generator
 *          Called from webserver_request_notify_callback()
 *          in app_netxduo.c for each /api/... route.
 * ============================================================ */
#ifndef API_HANDLER_H
#define API_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "nx_api.h"
#include "tx_api.h"

/* Maximum JSON response buffer size */
#define API_JSON_BUF_SIZE     512U

/* ============================================================
 * Individual sensor endpoint builders
 * Each function writes a null-terminated JSON string into buf.
 * ============================================================ */
void API_GetTemp     (char *buf, size_t len); /* /api/temp     */
void API_GetPressure (char *buf, size_t len); /* /api/pressure */
void API_GetLevel    (char *buf, size_t len); /* /api/level    */
void API_GetRPM      (char *buf, size_t len); /* /api/rpm      */
void API_GetCurrent  (char *buf, size_t len); /* /api/current  */
void API_GetGas      (char *buf, size_t len); /* /api/gas      */
void API_GetLight    (char *buf, size_t len); /* /api/light    */

/**
 * @brief  Returns a full JSON object with all sensor values.
 *         Used by /api/all — called every second by the dashboard.
 */
void API_GetAll      (char *buf, size_t len); /* /api/all      */

/**
 * @brief  Returns ThreadX + system statistics.
 *         Includes uptime, thread counts, memory info.
 */
void API_GetSystem   (char *buf, size_t len); /* /api/system   */

#ifdef __cplusplus
}
#endif

#endif /* API_HANDLER_H */
