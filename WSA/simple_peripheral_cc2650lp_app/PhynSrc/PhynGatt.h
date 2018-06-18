#ifndef PWD_GATT_PROFILE_H
#define PWD_GATT_PROFILE_H

#include "gatt.h"

#define PHYNGATT_NUM_ATTR_SUPPORTED 1

#define PHYNPROFILE_UUID                    (0xFFE0)
//#define PHYNPROFILE_CHAR1_UUID              (0xFFE1)

//#define PHYNPROFILE_PROGRAM_CRC_UUID        (0xFFE2)
//#define PHYNPROFILE_VALIDATION_SEED_UUID    (0xFFE3)
//#define PHYNPROFILE_VALIDATION_KEY_UUID     (0xFFE4)

//#define PHYNPROFILE_STATUS_UUID             (0xFFE5)
//#define PHYNPROFILE_COMMAND_UUID            (0xFFE6)

//#define PHYNPROFILE_MESSAGE_COUNTER_UUID    (0xFFE7)

#define MSG_FROM_APP_UUID            (0xFFE8)
#define MSG_TO_APP_COUNTER_UUID   (0xFFE9)
#define MSG_TO_APP_UUID           (0xFFEA)


#define PHYNPROFILE_SERVICE               0x00000002

// Characteristic identification for App notification messages
//#define PHYNPROFILE_CHAR1           (0x99)

//#define PWD_PROGRAM_CRC        (0x81)
//#define PWD_VALIDATION_SEED    (0x82)
//#define PWD_VALIDATION_KEY     (0x83)
//#define PWD_STATUS             (0x84)

//#define PWD_COMMAND                 (0x85)
//#define MESSAGE_FROM_APP_COUNTER    (0x86)
#define MSG_FROM_APP              (0x87)
#define MSG_TO_APP_COUNTER        (0x88)
#define MSG_TO_APP                (0x89)



#define PWD_PROG_CRC_SIZE           (2)
#define PWD_VALIDATION_SEED_SIZE    (4)
#define PWD_VALIDATION_KEY_SIZE     (4)
#define PWD_STATUS_SIZE             (1)
#define PWD_COMMAND_SIZE            (1)
#define MSG_FROM_APP_SIZE           (256+8)
#define MSG_TO_APP_CTR_SIZE         (1)
#define MSG_TO_APP_SIZE             (256+8)
#define BLE_VER_STR_LEN             (16)


#define GRP_UUID     (0xFFC0)

//#define NUM_SR_FIELDS   (1)
//#define STRING_CHAR_UUID   0x2A3D

#define FIELD_TIMEZONE_SIZE         (54)
#define FIELD_COUNTRY_CODE_SIZE     (2)
#define FIELD_SSID_SIZE             (32)
#define FIELD_PASSPHRASE_SIZE       (64)
#define FIELD_CLIENT_ID_SIZE        (40)
#define FIELD_CLIENT_SECRET_SIZE    (32)
#define FIELD_CLOUD_ENV_SIZE        (3)
#define FIELD_CLOUD_API_SIZE        (42)
#define FIELD_APP_VERSION_SIZE      (6)
#define FIELD_FIRMWARE_VERSION_SIZE (9)
#define FIELD_SERIAL_NUMBER_SIZE    (14)
#define FIELD_SR_DATA_SIZE          (34)
#define FIELD_SR_COUNT_SIZE         (2)

bStatus_t Phyn_AddServices();
void InitAppFields();

static bStatus_t PhynProfile_WriteAttrCB(uint16_t connHandle,   gattAttribute_t  *pAttr,
                                          uint8_t  *pValue,      uint16_t         len,
                                          uint16_t offset,       uint8_t          method);


static bStatus_t PhynProfile_ReadAttrCB(uint16_t    connHandle, gattAttribute_t  *pAttr,
                                        uint8_t     *pValue,    uint16_t         *pLen,
                                        uint16_t    offset,     uint16_t         maxLen,
                                        uint8_t     method);


static bStatus_t ExtraProfile_ReadAttrCB(uint16_t    connHandle, gattAttribute_t  *pAttr,
                                       uint8_t     *pValue,    uint16_t         *pLen,
                                       uint16_t    offset,     uint16_t         maxLen,
                                       uint8_t     method);

static bStatus_t ExtraProfile_WriteAttrCB(uint16_t connHandle,   gattAttribute_t  *pAttr,
                                         uint8_t  *pValue,      uint16_t         len,
                                         uint16_t offset,       uint8_t          method);

#endif /* PHYNGATTPROFILE_H */
