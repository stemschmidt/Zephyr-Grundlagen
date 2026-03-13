#ifndef _UBX_CHECKSUM_H_
#define _UBX_CHECKSUM_H_

#include <stdbool.h>
#include <stdint.h>

/** Size of the ubx message header in bytes. */
#define UBX_MSG_HEADER_SIZE 4U
/** Position at which the checksum range starts. */
#define UBX_MSG_CHECKSUM_RANGE_START 2U
/** Size of the ubx message checksum in bytes in bytes. */
#define UBX_MSG_CHECKSUM_SIZE 2U
/** Position at which the length field starts. */
#define UBX_MSG_LENGTH_OFFSET 4U
/** Size of the ubx message length field in bytes. */
#define UBX_MSG_LENGTH_SIZE 2U
/** Position at which the payload starts. */
#define UBX_MSG_PAYLOAD_OFFSET 6U
/** Size of the ubx message class field in bytes. */
#define UBX_MSG_CLASS_SIZE 1U
/** Size of the ubx message id field in bytes. */
#define UBX_MSG_ID_SIZE 1U
/** Position at which the checksum in a ubx message starts. */
#define UBX_MSG_CHECKSUM_OFFSET(payload_length)                                                    \
    ((payload_length) + UBX_MSG_LENGTH_SIZE + UBX_MSG_CLASS_SIZE + UBX_MSG_ID_SIZE +               \
        UBX_MSG_CHECKSUM_RANGE_START)
/** Define for the 'overhead' in a ubx message in bytes. */
#define UBX_MSG_OVERHEAD (UBX_MSG_HEADER_SIZE + UBX_MSG_CHECKSUM_SIZE + UBX_MSG_LENGTH_SIZE)

bool check_ubx_checksum(const uint8_t *p_ubx_message, uint16_t payload_length);

#endif /* _UBX_CHECKSUM_H_ */