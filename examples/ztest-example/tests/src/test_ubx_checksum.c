#include <stdbool.h>
#include <string.h>

#include <zephyr/ztest.h>

#include "ubx_checksum.h"

// clang-format off
static const uint8_t ubx_nav_status_correct_checksum[24u] = {
    0xB5, 0x62, 0x01, 0x03, 0x10, 0x00, 0x88, 0x84, 0x02, 0x00, 0x00, 0x40,
    0x02, 0x08, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x84, 0x02, 0x00, 0x8e, 0xE1
};

static const uint8_t ubx_nav_status_invalid_checksum[24u] = {
    0xB5, 0x62, 0x01, 0x03, 0x10, 0x00, 0x88, 0x84, 0x02, 0x00, 0x00, 0x40,
    0x02, 0x08, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x84, 0x02, 0x00, 0xFF, 0xFF
};
// clang-format on

ZTEST(ubx_checksum, test_check_ubx_checksum_valid)
{
    bool ok = check_ubx_checksum(ubx_nav_status_correct_checksum, 24u - UBX_MSG_OVERHEAD);

    zassert_equal(1, ok);
}

ZTEST(ubx_checksum, test_check_ubx_checksum_invalid)
{
    bool ok = check_ubx_checksum(ubx_nav_status_invalid_checksum, 24u - UBX_MSG_OVERHEAD);

    zassert_equal(false, ok);
}

ZTEST_SUITE(ubx_checksum, NULL, NULL, NULL, NULL, NULL);
