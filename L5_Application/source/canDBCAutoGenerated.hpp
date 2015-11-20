/// DBC file: Group2.dbc    Self node: MASTER
/// This file should be included by a source file, for example: #include "generated.c"
#include <stdbool.h>
#include <stdint.h>



/// Missing in Action structure
typedef struct {
    uint32_t is_mia : 1;          ///< Missing in action flag
    uint32_t mia_counter_ms : 31; ///< Missing in action counter
} mia_info_t;

/// CAN message header structure
typedef struct {
    uint32_t mid; ///< Message ID of the message
    uint8_t  dlc; ///< Data length of the message
} msg_hdr_t;

static const msg_hdr_t ANDROID_TX_HEARTBEAT_HDR =             {  010, 1 };
static const msg_hdr_t MOTORIO_TX_HEARTBEAT__HDR =            {  280, 1 };
static const msg_hdr_t GEO_TX_HEARTBEAT_HDR =                 {  281, 1 };
static const msg_hdr_t SENSOR_TX_HEARTBEAT_HDR =              {  283, 1 };
static const msg_hdr_t SENSOR_TX_SONARS_HDR =                 {  210, 4 };
static const msg_hdr_t MASTER_TX_MOTORIO_CMD_HDR =            {  220, 4 };
static const msg_hdr_t GEO_TX_DISTANCE_HDR =                  {  250, 4 };
static const msg_hdr_t GEO_TX_TURN_ANGLE_DIRECTION_HDR =      {  260, 2 };
static const msg_hdr_t ANDROID_TX_RUN_PAUSE_HDR =             {  270, 1 };


/// Message: HEARTBEAT from 'ANDROID', DLC: 1 byte(s), MID: 010
typedef struct {
    uint8_t ANDROID_HEARTBEAT_cmd;       ///< B7:0   Destination: MASTER

    mia_info_t mia_info;
} ANDROID_TX_HEARTBEAT_t;


/// Message: HEARTBEAT_ from 'MOTORIO', DLC: 1 byte(s), MID: 280
typedef struct {
    uint8_t MOTORIO_HEARTBEAT_cmd;       ///< B7:0   Destination: MASTER

    mia_info_t mia_info;
} MOTORIO_TX_HEARTBEAT__t;


/// Message: HEARTBEAT from 'GEO', DLC: 1 byte(s), MID: 281
typedef struct {
    uint8_t GEO_HEARTBEAT_cmd;           ///< B7:0   Destination: MASTER

    mia_info_t mia_info;
} GEO_TX_HEARTBEAT_t;


/// Message: HEARTBEAT from 'SENSOR', DLC: 1 byte(s), MID: 283
typedef struct {
    uint8_t SENSOR_HEARTBEAT_cmd;        ///< B7:0   Destination: MASTER

    mia_info_t mia_info;
} SENSOR_TX_HEARTBEAT_t;


/// Message: SONARS from 'SENSOR', DLC: 4 byte(s), MID: 210
typedef struct {
    uint8_t SENSOR_SONARS_FrontDistance; ///< B7:0   Destination: MASTER,MOTORIO
    uint8_t SENSOR_SONARS_LeftDistance;  ///< B15:8   Destination: MASTER,MOTORIO
    uint8_t SENSOR_SONARS_RightDistance; ///< B23:16   Destination: MASTER,MOTORIO
    uint16_t SENSOR_SONARS_RearDistance; ///< B35:24   Destination: MASTER,MOTORIO

    mia_info_t mia_info;
} SENSOR_TX_SONARS_t;


/// Message: MOTORIO_CMD from 'MASTER', DLC: 4 byte(s), MID: 220
typedef struct {
    uint8_t MOTORIO_CMD_LeftRightdirection; ///< B7:0   Destination: MOTORIO
    uint8_t MOTORIO_CMD_LevelOfDirection; ///< B15:8   Destination: MOTORIO
    uint8_t MOTORIO_CMD_FrontBackDirection; ///< B23:16   Destination: MOTORIO
    uint8_t MOTORIO_CMD_LevelOfSpeed;    ///< B31:24   Destination: MOTORIO

    mia_info_t mia_info;
} MASTER_TX_MOTORIO_CMD_t;


/// Message: DISTANCE from 'GEO', DLC: 4 byte(s), MID: 250
typedef struct {
    uint16_t DISTANCE_FinalDistance;     ///< B15:0   Destination: MASTER
    uint16_t DISTANCE_CheckpointDistance; ///< B31:16   Destination: MASTER

    mia_info_t mia_info;
} GEO_TX_DISTANCE_t;


/// Message: TURN_ANGLE_DIRECTION from 'GEO', DLC: 2 byte(s), MID: 260
typedef struct {
    uint8_t TURN_ANGLE_DIRECTION_TurnAngle; ///< B7:0   Destination: MASTER
    uint8_t TURN_ANGLE_DIRECTION_TurnDirection; ///< B15:8   Destination: MASTER

    mia_info_t mia_info;
} GEO_TX_TURN_ANGLE_DIRECTION_t;


/// Message: RUN_PAUSE from 'ANDROID', DLC: 1 byte(s), MID: 270
typedef struct {
    uint8_t RUN_PAUSE;                   ///< B7:0   Destination: MASTER

    mia_info_t mia_info;
} ANDROID_TX_RUN_PAUSE_t;


/// These 'externs' need to be defined in a source file of your project
extern const uint32_t HEARTBEAT__MIA_MS;
extern const ANDROID_TX_HEARTBEAT_t HEARTBEAT__MIA_MSG;
extern const uint32_t HEARTBEAT___MIA_MS;
extern const MOTORIO_TX_HEARTBEAT__t HEARTBEAT___MIA_MSG;
extern const uint32_t HEARTBEAT__MIA_MS;
extern const GEO_TX_HEARTBEAT_t HEARTBEAT__MIA_MSG;
extern const uint32_t HEARTBEAT__MIA_MS;
extern const SENSOR_TX_HEARTBEAT_t HEARTBEAT__MIA_MSG;
extern const uint32_t SONARS__MIA_MS;
extern const SENSOR_TX_SONARS_t SONARS__MIA_MSG;
extern const uint32_t MOTORIO_CMD__MIA_MS;
extern const MASTER_TX_MOTORIO_CMD_t MOTORIO_CMD__MIA_MSG;
extern const uint32_t DISTANCE__MIA_MS;
extern const GEO_TX_DISTANCE_t DISTANCE__MIA_MSG;
extern const uint32_t TURN_ANGLE_DIRECTION__MIA_MS;
extern const GEO_TX_TURN_ANGLE_DIRECTION_t TURN_ANGLE_DIRECTION__MIA_MSG;
extern const uint32_t RUN_PAUSE__MIA_MS;
extern const ANDROID_TX_RUN_PAUSE_t RUN_PAUSE__MIA_MSG;

/// Encode ANDROID's 'HEARTBEAT' message
/// @returns the message header of this message
static msg_hdr_t ANDROID_TX_HEARTBEAT_encode(uint64_t *to, ANDROID_TX_HEARTBEAT_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->ANDROID_HEARTBEAT_cmd - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    return ANDROID_TX_HEARTBEAT_HDR;
}


/// Encode MOTORIO's 'HEARTBEAT_' message
/// @returns the message header of this message
static msg_hdr_t MOTORIO_TX_HEARTBEAT__encode(uint64_t *to, MOTORIO_TX_HEARTBEAT__t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->MOTORIO_HEARTBEAT_cmd - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    return MOTORIO_TX_HEARTBEAT__HDR;
}


/// Encode GEO's 'HEARTBEAT' message
/// @returns the message header of this message
static msg_hdr_t GEO_TX_HEARTBEAT_encode(uint64_t *to, GEO_TX_HEARTBEAT_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->GEO_HEARTBEAT_cmd - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    return GEO_TX_HEARTBEAT_HDR;
}


/// Encode SENSOR's 'HEARTBEAT' message
/// @returns the message header of this message
static msg_hdr_t SENSOR_TX_HEARTBEAT_encode(uint64_t *to, SENSOR_TX_HEARTBEAT_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->SENSOR_HEARTBEAT_cmd - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    return SENSOR_TX_HEARTBEAT_HDR;
}


/// Encode SENSOR's 'SONARS' message
/// @returns the message header of this message
static msg_hdr_t SENSOR_TX_SONARS_encode(uint64_t *to, SENSOR_TX_SONARS_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->SENSOR_SONARS_FrontDistance - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    raw_signal = ((uint64_t)(((from->SENSOR_SONARS_LeftDistance - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[1] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B8

    raw_signal = ((uint64_t)(((from->SENSOR_SONARS_RightDistance - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[2] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B16

    raw_signal = ((uint64_t)(((from->SENSOR_SONARS_RearDistance - (0)) / 1.0) + 0.5)) & 0xfff;
    bytes[3] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B24
    bytes[4] |= (((uint8_t)(raw_signal >> 8) & 0x0f) << 0); ///< 4 bit(s) to B32

    return SENSOR_TX_SONARS_HDR;
}


/// Encode MASTER's 'MOTORIO_CMD' message
/// @returns the message header of this message
static msg_hdr_t MASTER_TX_MOTORIO_CMD_encode(uint64_t *to, MASTER_TX_MOTORIO_CMD_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->MOTORIO_CMD_LeftRightdirection - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    raw_signal = ((uint64_t)(((from->MOTORIO_CMD_LevelOfDirection - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[1] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B8

    raw_signal = ((uint64_t)(((from->MOTORIO_CMD_FrontBackDirection - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[2] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B16

    raw_signal = ((uint64_t)(((from->MOTORIO_CMD_LevelOfSpeed - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[3] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B24

    return MASTER_TX_MOTORIO_CMD_HDR;
}


/// Encode GEO's 'DISTANCE' message
/// @returns the message header of this message
static msg_hdr_t GEO_TX_DISTANCE_encode(uint64_t *to, GEO_TX_DISTANCE_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->DISTANCE_FinalDistance - (0)) / 1.0) + 0.5)) & 0xffff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0
    bytes[1] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B8

    raw_signal = ((uint64_t)(((from->DISTANCE_CheckpointDistance - (0)) / 1.0) + 0.5)) & 0xffff;
    bytes[2] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B16
    bytes[3] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B24

    return GEO_TX_DISTANCE_HDR;
}


/// Encode GEO's 'TURN_ANGLE_DIRECTION' message
/// @returns the message header of this message
static msg_hdr_t GEO_TX_TURN_ANGLE_DIRECTION_encode(uint64_t *to, GEO_TX_TURN_ANGLE_DIRECTION_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->TURN_ANGLE_DIRECTION_TurnAngle - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    raw_signal = ((uint64_t)(((from->TURN_ANGLE_DIRECTION_TurnDirection - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[1] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B8

    return GEO_TX_TURN_ANGLE_DIRECTION_HDR;
}


/// Encode ANDROID's 'RUN_PAUSE' message
/// @returns the message header of this message
static msg_hdr_t ANDROID_TX_RUN_PAUSE_encode(uint64_t *to, ANDROID_TX_RUN_PAUSE_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->RUN_PAUSE - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    return ANDROID_TX_RUN_PAUSE_HDR;
}


/// Decode ANDROID's 'HEARTBEAT' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool ANDROID_TX_HEARTBEAT_decode(ANDROID_TX_HEARTBEAT_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != ANDROID_TX_HEARTBEAT_HDR.dlc || hdr->mid != ANDROID_TX_HEARTBEAT_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->ANDROID_HEARTBEAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode MOTORIO's 'HEARTBEAT_' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool MOTORIO_TX_HEARTBEAT__decode(MOTORIO_TX_HEARTBEAT__t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != MOTORIO_TX_HEARTBEAT__HDR.dlc || hdr->mid != MOTORIO_TX_HEARTBEAT__HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->MOTORIO_HEARTBEAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GEO's 'HEARTBEAT' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GEO_TX_HEARTBEAT_decode(GEO_TX_HEARTBEAT_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GEO_TX_HEARTBEAT_HDR.dlc || hdr->mid != GEO_TX_HEARTBEAT_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->GEO_HEARTBEAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode SENSOR's 'HEARTBEAT' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool SENSOR_TX_HEARTBEAT_decode(SENSOR_TX_HEARTBEAT_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != SENSOR_TX_HEARTBEAT_HDR.dlc || hdr->mid != SENSOR_TX_HEARTBEAT_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->SENSOR_HEARTBEAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode SENSOR's 'SONARS' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool SENSOR_TX_SONARS_decode(SENSOR_TX_SONARS_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != SENSOR_TX_SONARS_HDR.dlc || hdr->mid != SENSOR_TX_SONARS_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->SENSOR_SONARS_FrontDistance = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[1] >> 0) & 0xff); ///< 8 bit(s) from B8
    raw_signal    |= (bits_from_byte << 0);
    to->SENSOR_SONARS_LeftDistance = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[2] >> 0) & 0xff); ///< 8 bit(s) from B16
    raw_signal    |= (bits_from_byte << 0);
    to->SENSOR_SONARS_RightDistance = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[3] >> 0) & 0xff); ///< 8 bit(s) from B24
    raw_signal    |= (bits_from_byte << 0);
    bits_from_byte = ((bytes[4] >> 0) & 0x0f); ///< 4 bit(s) from B32
    raw_signal    |= (bits_from_byte << 8);
    to->SENSOR_SONARS_RearDistance = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode MASTER's 'MOTORIO_CMD' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool MASTER_TX_MOTORIO_CMD_decode(MASTER_TX_MOTORIO_CMD_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != MASTER_TX_MOTORIO_CMD_HDR.dlc || hdr->mid != MASTER_TX_MOTORIO_CMD_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->MOTORIO_CMD_LeftRightdirection = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[1] >> 0) & 0xff); ///< 8 bit(s) from B8
    raw_signal    |= (bits_from_byte << 0);
    to->MOTORIO_CMD_LevelOfDirection = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[2] >> 0) & 0xff); ///< 8 bit(s) from B16
    raw_signal    |= (bits_from_byte << 0);
    to->MOTORIO_CMD_FrontBackDirection = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[3] >> 0) & 0xff); ///< 8 bit(s) from B24
    raw_signal    |= (bits_from_byte << 0);
    to->MOTORIO_CMD_LevelOfSpeed = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GEO's 'DISTANCE' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GEO_TX_DISTANCE_decode(GEO_TX_DISTANCE_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GEO_TX_DISTANCE_HDR.dlc || hdr->mid != GEO_TX_DISTANCE_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    bits_from_byte = ((bytes[1] >> 0) & 0xff); ///< 8 bit(s) from B8
    raw_signal    |= (bits_from_byte << 8);
    to->DISTANCE_FinalDistance = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[2] >> 0) & 0xff); ///< 8 bit(s) from B16
    raw_signal    |= (bits_from_byte << 0);
    bits_from_byte = ((bytes[3] >> 0) & 0xff); ///< 8 bit(s) from B24
    raw_signal    |= (bits_from_byte << 8);
    to->DISTANCE_CheckpointDistance = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GEO's 'TURN_ANGLE_DIRECTION' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GEO_TX_TURN_ANGLE_DIRECTION_decode(GEO_TX_TURN_ANGLE_DIRECTION_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GEO_TX_TURN_ANGLE_DIRECTION_HDR.dlc || hdr->mid != GEO_TX_TURN_ANGLE_DIRECTION_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->TURN_ANGLE_DIRECTION_TurnAngle = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[1] >> 0) & 0xff); ///< 8 bit(s) from B8
    raw_signal    |= (bits_from_byte << 0);
    to->TURN_ANGLE_DIRECTION_TurnDirection = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode ANDROID's 'RUN_PAUSE' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool ANDROID_TX_RUN_PAUSE_decode(ANDROID_TX_RUN_PAUSE_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != ANDROID_TX_RUN_PAUSE_HDR.dlc || hdr->mid != ANDROID_TX_RUN_PAUSE_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->RUN_PAUSE = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Handle the MIA for ANDROID's 'HEARTBEAT' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool ANDROID_TX_HEARTBEAT_handle_mia(ANDROID_TX_HEARTBEAT_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= HEARTBEAT__MIA_MS);

    if (!msg->mia_info.is_mia) {
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   {
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = HEARTBEAT__MIA_MSG;
        msg->mia_info.mia_counter_ms = HEARTBEAT__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MOTORIO's 'HEARTBEAT_' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool MOTORIO_TX_HEARTBEAT__handle_mia(MOTORIO_TX_HEARTBEAT__t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= HEARTBEAT___MIA_MS);

    if (!msg->mia_info.is_mia) {
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   {
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = HEARTBEAT___MIA_MSG;
        msg->mia_info.mia_counter_ms = HEARTBEAT___MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO's 'HEARTBEAT' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GEO_TX_HEARTBEAT_handle_mia(GEO_TX_HEARTBEAT_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= HEARTBEAT__MIA_MS);

    if (!msg->mia_info.is_mia) {
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   {
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = HEARTBEAT__MIA_MSG;
        msg->mia_info.mia_counter_ms = HEARTBEAT__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for SENSOR's 'HEARTBEAT' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool SENSOR_TX_HEARTBEAT_handle_mia(SENSOR_TX_HEARTBEAT_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= HEARTBEAT__MIA_MS);

    if (!msg->mia_info.is_mia) {
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   {
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = HEARTBEAT__MIA_MSG;
        msg->mia_info.mia_counter_ms = HEARTBEAT__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for SENSOR's 'SONARS' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool SENSOR_TX_SONARS_handle_mia(SENSOR_TX_SONARS_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= SONARS__MIA_MS);

    if (!msg->mia_info.is_mia) {
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   {
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = SONARS__MIA_MSG;
        msg->mia_info.mia_counter_ms = SONARS__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MASTER's 'MOTORIO_CMD' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool MASTER_TX_MOTORIO_CMD_handle_mia(MASTER_TX_MOTORIO_CMD_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= MOTORIO_CMD__MIA_MS);

    if (!msg->mia_info.is_mia) {
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   {
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = MOTORIO_CMD__MIA_MSG;
        msg->mia_info.mia_counter_ms = MOTORIO_CMD__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO's 'DISTANCE' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GEO_TX_DISTANCE_handle_mia(GEO_TX_DISTANCE_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= DISTANCE__MIA_MS);

    if (!msg->mia_info.is_mia) {
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   {
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = DISTANCE__MIA_MSG;
        msg->mia_info.mia_counter_ms = DISTANCE__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO's 'TURN_ANGLE_DIRECTION' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GEO_TX_TURN_ANGLE_DIRECTION_handle_mia(GEO_TX_TURN_ANGLE_DIRECTION_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= TURN_ANGLE_DIRECTION__MIA_MS);

    if (!msg->mia_info.is_mia) {
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   {
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = TURN_ANGLE_DIRECTION__MIA_MSG;
        msg->mia_info.mia_counter_ms = TURN_ANGLE_DIRECTION__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for ANDROID's 'RUN_PAUSE' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool ANDROID_TX_RUN_PAUSE_handle_mia(ANDROID_TX_RUN_PAUSE_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= RUN_PAUSE__MIA_MS);

    if (!msg->mia_info.is_mia) {
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   {
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = RUN_PAUSE__MIA_MSG;
        msg->mia_info.mia_counter_ms = RUN_PAUSE__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}
