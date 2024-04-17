#include <zephyr/device.h>
#include <zephyr/toolchain.h>

/* 1 : /soc/peripheral@50000000/clock@5000:
 */
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_78[] = { DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, DEVICE_HANDLE_ENDS };

/* 2 : /soc/peripheral@50000000/gpio@842800:
 * Supported:
 *    - /soc/peripheral@50000000/spi@a000
 *    - /soc/peripheral@50000000/spi@a000/gc9a01@0
 */
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_28[] = { DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, 6, 8, DEVICE_HANDLE_ENDS };

/* 3 : /soc/peripheral@50000000/gpio@842500:
 * Supported:
 *    - /soc/peripheral@50000000/spi@9000
 */
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_9[] = { DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, 7, DEVICE_HANDLE_ENDS };

/* 4 : /soc/peripheral@50000000/uart@8000:
 */
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_116[] = { DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, DEVICE_HANDLE_ENDS };

/* 5 : /soc/peripheral@50000000/mbox@2a000:
 */
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_20[] = { DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, DEVICE_HANDLE_ENDS };

/* 6 : /soc/peripheral@50000000/spi@a000:
 * Direct Dependencies:
 *    - /soc/peripheral@50000000/gpio@842800
 * Supported:
 *    - /soc/peripheral@50000000/spi@a000/gc9a01@0
 */
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_140[] = { 2, DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, 8, DEVICE_HANDLE_ENDS };

/* 7 : /soc/peripheral@50000000/spi@9000:
 * Direct Dependencies:
 *    - /soc/peripheral@50000000/gpio@842500
 * Supported:
 *    - /soc/peripheral@50000000/spi@9000/bmi270@0
 */
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_138[] = { 3, DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, 9, DEVICE_HANDLE_ENDS };

/* 8 : /soc/peripheral@50000000/spi@a000/gc9a01@0:
 * Direct Dependencies:
 *    - /soc/peripheral@50000000/gpio@842800
 *    - /soc/peripheral@50000000/spi@a000
 */
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_141[] = { 2, 6, DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, DEVICE_HANDLE_ENDS };

/* 9 : /soc/peripheral@50000000/spi@9000/bmi270@0:
 * Direct Dependencies:
 *    - /soc/peripheral@50000000/spi@9000
 */
const Z_DECL_ALIGN(device_handle_t) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_dts_ord_139[] = { 7, DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, DEVICE_HANDLE_ENDS };
