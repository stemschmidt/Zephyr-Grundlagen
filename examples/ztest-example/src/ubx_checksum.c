#include "ubx_checksum.h"
/*
  UBX checksum: Chapter 3.4 UBX checksum in
  https://content.u-blox.com/sites/default/files/documents/u-blox-F9-HPG-L1L5-1.40_InterfaceDescription_UBX-23006991.pdf
*/

static void calculate_ubx_checksum(
    const uint8_t *p_ubx_message, uint16_t payload_length, uint8_t *p_ck_a, uint8_t *p_ck_b)
{
    uint8_t ck_a = 0U;
    uint8_t ck_b = 0U;

    int pos;
    int until = UBX_MSG_LENGTH_SIZE + UBX_MSG_CLASS_SIZE + UBX_MSG_ID_SIZE +
                UBX_MSG_CHECKSUM_RANGE_START + payload_length;

    /* Skip preamble of UBX message and start with field "Class" (see
    u-blox-F9-HPG-.32_InterfaceDescription_UBX-22008968.pdf, chapter 3.2 UBX
    frame structure). */
    for (pos = UBX_MSG_CHECKSUM_RANGE_START; pos < until; pos++)
    {
        ck_a = ck_a + p_ubx_message[pos];
        ck_b = ck_b + ck_a;
    }

    *p_ck_a = ck_a;
    *p_ck_b = ck_b;
}

bool check_ubx_checksum(const uint8_t *p_ubx_message, uint16_t payload_length)
{
    uint8_t ck_a = 0U;
    uint8_t ck_b = 0U;

    calculate_ubx_checksum(p_ubx_message, payload_length, &ck_a, &ck_b);

    /* The calling function must have verified that the actual message contains
     * the two bytes after payload_length! */
    return (p_ubx_message[UBX_MSG_CHECKSUM_OFFSET(payload_length)] == ck_a &&
            p_ubx_message[UBX_MSG_CHECKSUM_OFFSET(payload_length) + 1] == ck_b);
}
