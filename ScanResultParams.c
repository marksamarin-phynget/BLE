/*********************************************************************

*********************************************************************/
#define EXTRA_UUID (0xFFD0)
#define SR_SIZE  (34)
#define NUM_SR_FIELDS (20)

  U8 sExtraMsg1Desc[] = "Extra #1";
  U8 sSR_Data[NUM_SR_FIELDS][SR_SIZE+1] = {"12345678901234567890123456789012345678901234567890"};

const U8 SR1_UUID[ATT_BT_UUID_SIZE] = { LO_UINT16(SR_UUID + 1), HI_UINT16(SR_UUID + 1) };
const U8 SR2_UUID[ATT_BT_UUID_SIZE] = { LO_UINT16(SR_UUID + 2), HI_UINT16(SR_UUID + 2) };
const U8 SR3_UUID[ATT_BT_UUID_SIZE] = { LO_UINT16(SR_UUID + 3), HI_UINT16(SR_UUID + 3) };
const U8 SR4_UUID[ATT_BT_UUID_SIZE] = { LO_UINT16(SR_UUID + 4), HI_UINT16(SR_UUID + 4) };
const U8 SR5_UUID[ATT_BT_UUID_SIZE] = { LO_UINT16(SR_UUID + 5), HI_UINT16(SR_UUID + 5) };

U8 cSR_Props = GATT_PROP_READ;

/*************************************************************************************************
  Scan Result messages
************************************************************************************************/
{ { ATT_BT_UUID_SIZE, characterUUID }, 	  GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
{ { ATT_BT_UUID_SIZE, SR1_UUID },  		  GATT_PERMIT_READ, 0, &sSR_Data[0] }, // Value
//{ { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description

{ { ATT_BT_UUID_SIZE, characterUUID }, 	  GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
{ { ATT_BT_UUID_SIZE, SR2_UUID },  		  GATT_PERMIT_READ, 0, &sSR_Data[1] }, // Value

{ { ATT_BT_UUID_SIZE, characterUUID }, 	  GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
{ { ATT_BT_UUID_SIZE, SR3_UUID },  		  GATT_PERMIT_READ, 0, &sSR_Data[2] }, // Value

{ { ATT_BT_UUID_SIZE, characterUUID }, 	  GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
{ { ATT_BT_UUID_SIZE, SR4_UUID },  		  GATT_PERMIT_READ, 0, &sSR_Data[3] }, // Value

{ { ATT_BT_UUID_SIZE, characterUUID }, 	  GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
{ { ATT_BT_UUID_SIZE, SR5_UUID },  		  GATT_PERMIT_READ, 0, &sSR_Data[4] }, // Value

/***********************************************************************************************
            Extra1_UUID
          ***********************************************************************************************
          case EXTRA_UUID1:

              // Send a chunk of field
              *pLen = Gatt_Partial_Tx(0,  pValue,  pAttr->pValue, maxLen);

             // strcpy(pValue, "TEST");
             // *pLen = 4;
              status = SUCCESS;

              /*
              if (EXTRA1_SIZE <= maxLen)
              {
                   *pLen = EXTRA1_SIZE;
                   VOID memcpy( pValue, (uint8 *)pAttr->pValue, EXTRA1_SIZE );
                   status = SUCCESS;
                }
               else
                  status = ATT_ERR_INVALID_VALUE_SIZE;
              */

           break;

*/
