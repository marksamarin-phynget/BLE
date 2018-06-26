#if 0
/*********************************************************************
 Setup possible Callbacks for when a characteristic value has changed
*********************************************************************/
typedef void (*PhynProfileChange_t)( uint8 paramID );


typedef struct
{
  PhynProfileChange_t        pfnValueChange;  // Called when characteristic value changes
} PhynProfile_AppCBs_t;
    
static PhynProfile_AppCBs_t *PhynProfile_AppCBs = NULL;   // No change callback 

#endif



/*********************************************************************
 Message Counter -  PWD_MESSAGE_COUNTER                (0x86)
                    PHYNPROFILE_MESSAGE_COUNTER_UUID    (0xFFE7)

*********************************************************************
#define PWD_MESSAGE_COUNTER_SIZE 1

static  U8 cMessageFromAppDesc[] = "Tx Counter";
        U8 cMessageFromAppCounter;

CONST U8 Phyn_Message_Counter_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(PHYNPROFILE_MESSAGE_COUNTER_UUID), HI_UINT16(PHYNPROFILE_MESSAGE_COUNTER_UUID)
};

U8 PhynMessageCounterProps = GATT_PROP_READ;
*/


#if 0
/*********************************************************************
 Program CRC -  PWD_PROGRAM_CRC                (0x81)
                PHYNPROFILE_PROGRAM_CRC_UUID    (0xFFE2)
*********************************************************************/
static  U8  sProgramCRCDesc[] = "CRCs (App, Stack, NVD)";
        //U16 uProgramCRC;
        U8     sCRCs[15];

CONST U8 Phyn_Program_CRC_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(PHYNPROFILE_PROGRAM_CRC_UUID), HI_UINT16(PHYNPROFILE_PROGRAM_CRC_UUID)
};

U8 PhynProgramCRCProps = GATT_PROP_READ;


/*********************************************************************
 Validation Seed -  PWD_VALIDATION_SEED                (0x82)
                    PHYNPROFILE_VALIDATION_SEED_UUID    (0xFFE3)
*********************************************************************/
static  U8 sValidationSeedDesc[] = "Validation Seed";
        U8 sValidationSeed[PWD_VALIDATION_SEED_SIZE] = { '\xFC','\xFD','\xFE','\xFF' };

CONST U8 Phyn_Validation_Seed_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(PHYNPROFILE_VALIDATION_SEED_UUID), HI_UINT16(PHYNPROFILE_VALIDATION_SEED_UUID)
};

U8 PhynValidationSeedProps = GATT_PROP_READ;

/*********************************************************************
 Validation Key -   PWD_VALIDATION_KEY                 (0x83)
                    PHYNPROFILE_VALIDATION_KEY_UUID     (0xFFE4)
*********************************************************************/
static  U8 sValidationKeyDesc[] = "Validation Key";
        U8 sValidationKey[PWD_VALIDATION_KEY_SIZE];

CONST U8 Phyn_Validation_Key_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(PHYNPROFILE_VALIDATION_KEY_UUID), HI_UINT16(PHYNPROFILE_VALIDATION_KEY_UUID)
};

U8 PhynValidationKeyProps = GATT_PROP_READ | GATT_PROP_WRITE;


/*********************************************************************
 BLE Status -  PWD_STATUS              (0x84)
               PHYNPROFILE_STATUS_UUID  (0xFFE5)
*********************************************************************/
static  U8 sStatusDesc[] = "Status Code";
        U8 cStatusCode;

CONST U8 Phyn_Status_Key_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(PHYNPROFILE_STATUS_UUID), HI_UINT16(PHYNPROFILE_STATUS_UUID)
};

U8 PhynStatusKeyProps = GATT_PROP_READ;

/*********************************************************************
 BLE Command -  PWD_COMMAND                (0x85)
                PHYNPROFILE_COMMAND_UUID    (0xFFE6)
*********************************************************************/
static  U8 sCommandDesc[] = "Command Code";
        U8 cCommandCode;

CONST U8 Phyn_Command_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(PHYNPROFILE_COMMAND_UUID), HI_UINT16(PHYNPROFILE_COMMAND_UUID)
};

U8 PhynCommandProps =  GATT_PROP_READ | GATT_PROP_WRITE;

#endif


#if 0
    /*************************************************************************************************
      Characteristic #1 Declaration - R/W Character
    *************************************************************************************************/

    //  Declaration
    {
        { ATT_BT_UUID_SIZE, characterUUID },                 // type=0x2803 (Standard Characteristic UUID)
          GATT_PERMIT_READ,  0,  &PhynProfileChar1Props
    },

    // Value
    {
        { ATT_BT_UUID_SIZE, PhynProfilechar1UUID },
          GATT_PERMIT_RW,  0, &PhynProfileChar1
    },

    // User Description
    {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
          GATT_PERMIT_READ,  0,  PhynProfileChar1UserDesc
    },
#endif

#if 0
        /***********************************************************************************************
           CHAR1
        ***********************************************************************************************/
        case PHYNPROFILE_CHAR1_UUID:

            // Validate Size
            if ( offset == 0 )
            {
                  if ( len != 1 )
                  {
                    status = ATT_ERR_INVALID_VALUE_SIZE;
                  }
            }
            else
            {
                status = ATT_ERR_ATTR_NOT_LONG;
            }

            // Write the value
            if ( status == SUCCESS )
            {
                  uint8 *pCurValue = (uint8 *)pAttr->pValue;
                  *pCurValue = pValue[0];
                //  uChangedValueID = PHYNPROFILE_CHAR1;
            }

        break;
#endif
#if 0
      /***********************************************************************************************
         CHAR 1
      ***********************************************************************************************/
      case PHYNPROFILE_CHAR1_UUID:
        *pLen = 1;
        pValue[0] = *pAttr->pValue;
      break;
#endif
#if 0
      /*************************************************************************************************
       Program CRC
      *************************************************************************************************/
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &PhynProgramCRCProps
      },

      // Value
      {
           { ATT_BT_UUID_SIZE, Phyn_Program_CRC_UUID },
             GATT_PERMIT_READ, 0,  sCRCs
       },

      // User Description
      {
          { ATT_BT_UUID_SIZE, charUserDescUUID },
            GATT_PERMIT_READ, 0, sProgramCRCDesc
      },

      /*************************************************************************************************
       Validation Seed
      *************************************************************************************************/
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &PhynValidationSeedProps
      },

      // Value
      {
           { ATT_BT_UUID_SIZE, Phyn_Validation_Seed_UUID },
             GATT_PERMIT_READ, 0,  sValidationSeed
       },

      // User Description
      {
          { ATT_BT_UUID_SIZE, charUserDescUUID },
            GATT_PERMIT_READ, 0, sValidationSeedDesc
      },
      /*************************************************************************************************
       Validation Key
      *************************************************************************************************/
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &PhynValidationKeyProps
      },

      // Value
      {
           { ATT_BT_UUID_SIZE, Phyn_Validation_Key_UUID },
             GATT_PERMIT_RW, 0,  sValidationKey
       },

      // User Description
      {
          { ATT_BT_UUID_SIZE, charUserDescUUID },
            GATT_PERMIT_READ, 0, sValidationKeyDesc
      },

      /*************************************************************************************************
       Status Code
      *************************************************************************************************/
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &PhynStatusKeyProps
      },

      // Value
      {
           { ATT_BT_UUID_SIZE, Phyn_Status_Key_UUID },
             GATT_PERMIT_READ, 0,  &cStatusCode
       },

      // User Description
      {
          { ATT_BT_UUID_SIZE, charUserDescUUID },
            GATT_PERMIT_READ, 0, sStatusDesc
      },

      /*************************************************************************************************
       Command Code
      *************************************************************************************************/
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &PhynCommandProps
      },

      // Value
      {
           { ATT_BT_UUID_SIZE, Phyn_Command_UUID },
             GATT_PERMIT_RW, 0,  &cStatusCode
       },

      // User Description
      {
          { ATT_BT_UUID_SIZE, charUserDescUUID },
            GATT_PERMIT_READ, 0, sCommandDesc
      },

      /*************************************************************************************************
       Message Counter
      *************************************************************************************************
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &PhynMessageCounterProps
      },

      // Value
      {
           { ATT_BT_UUID_SIZE, Phyn_Message_Counter_UUID },
           GATT_PERMIT_READ, 0,  &cMessageCounter
       },

      // User Description
      {
          { ATT_BT_UUID_SIZE, charUserDescUUID },
            GATT_PERMIT_READ, 0, sMessageCounterDesc
      },
    */
#endif



      //#ifndef NO_EXTRA_CHARACTERISTICS
      #if 0
      static CONST gattAttrType_t ExProfileService = { ATT_BT_UUID_SIZE, PhynProfileUUID };

      static gattAttribute_t ExProfileAttrTbl[] =
      {
          // PHYN Profile Service
          {
             { ATT_BT_UUID_SIZE, 0xFFD0 }, /* type (=0xFFFA) */
               GATT_PERMIT_READ,                         /* permissions */
               0,                                        /* handle */
              (uint8 *)&ExProfileService            /* pValue */
          },

          //  Declaration
            {
                { ATT_BT_UUID_SIZE, characterUUID },
                  GATT_PERMIT_READ, 0,   &PhynProgramCRCProps
            },

            // Value
            {
                 { ATT_BT_UUID_SIZE, Phyn_Program_CRC_UUID },
                   GATT_PERMIT_READ, 0,  sCRCs
             },

            // User Description
            {
                { ATT_BT_UUID_SIZE, charUserDescUUID },
                  GATT_PERMIT_READ, 0, sProgramCRCDesc
            },

      }

	  
	  
#if 0
/*********************************************************************
 * @fn      SimpleProfile_SetParameter
 *
 * @brief   Set a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to write
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t SimpleProfile_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case SIMPLEPROFILE_CHAR1:
      if ( len == sizeof ( uint8 ) )
      {
        simpleProfileChar1 = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case SIMPLEPROFILE_CHAR2:
      if ( len == sizeof ( uint8 ) )
      {
        simpleProfileChar2 = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case SIMPLEPROFILE_CHAR3:
      if ( len == sizeof ( uint8 ) )
      {
        simpleProfileChar3 = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case SIMPLEPROFILE_CHAR4:
      if ( len == sizeof ( uint8 ) )
      {
        simpleProfileChar4 = *((uint8*)value);

        // See if Notification has been enabled
        GATTServApp_ProcessCharCfg( simpleProfileChar4Config, &simpleProfileChar4, FALSE,
                                    simpleProfileAttrTbl, GATT_NUM_ATTRS( simpleProfileAttrTbl ),
                                    INVALID_TASK_ID, simpleProfile_ReadAttrCB );
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case SIMPLEPROFILE_CHAR5:
      if ( len == SIMPLEPROFILE_CHAR5_LEN )
      {
        VOID memcpy( simpleProfileChar5, value, SIMPLEPROFILE_CHAR5_LEN );
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }

  return ( ret );
}

/*********************************************************************
  PWD_GetParameter
      Get a Simple Profile parameter.

     param:    Profile parameter ID

     ValuePrt:    Pointer to data to put.  This is dependent on
               the parameter ID and WILL be cast to the appropriate
               data type (example: data type of uint16 will be cast to
               uint16 pointer).

     return:
 **********************************************************************/
bStatus_t PWD_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case SIMPLEPROFILE_CHAR1:
      *((uint8*)value) = simpleProfileChar1;
      break;

    case SIMPLEPROFILE_CHAR2:
      *((uint8*)value) = simpleProfileChar2;
      break;

    case SIMPLEPROFILE_CHAR3:
      *((uint8*)value) = simpleProfileChar3;
      break;

    case SIMPLEPROFILE_CHAR4:
      *((uint8*)value) = simpleProfileChar4;
      break;

    case SIMPLEPROFILE_CHAR5:
      VOID memcpy( value, simpleProfileChar5, SIMPLEPROFILE_CHAR5_LEN );
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }

  return ( ret );
}
#endif


#if 0
/*********************************************************************
 Setup Phyn Characteristic #1
*********************************************************************/

uint8 PhynProfileChar1 = 'Z';                                        // Characteristic 1 Value
uint8 PhynProfileChar1UserDesc[] = "Phyn #1";           // Simple Profile Characteristic 1 User Description

// Characteristic 1 UUID: 0xFFFB
CONST uint8 PhynProfilechar1UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(PHYNPROFILE_CHAR1_UUID), HI_UINT16(PHYNPROFILE_CHAR1_UUID)
};

U8 PhynProfileChar1Props = GATT_PROP_READ | GATT_PROP_WRITE;
#endif

#if 0
        /***********************************************************************************************
           Program CRC
        ***********************************************************************************************/
        case PHYNPROFILE_PROGRAM_CRC_UUID:
            if (PWD_PROG_CRC_SIZE <= maxLen)
            {

                UnsHalfToAscii(uAppCRC,      (char *)sCRCs);         sCRCs[4] = ' ';
                UnsHalfToAscii(uBLEStackCRC, (char *)&sCRCs[5]);    sCRCs[9] = ' ';
                UnsHalfToAscii(uNVDCRC,      (char *)&sCRCs[10]);

                *pLen = 15;                                       //      Send only what is left
                memcpy(pValue, sCRCs, *pLen);

                //*pLen = PWD_PROG_CRC_SIZE;
                //VOID memcpy( pValue, (uint8 *)pAttr->pValue, PWD_PROG_CRC_SIZE );

                status = SUCCESS;
             }
             else
                status = ATT_ERR_INVALID_VALUE_SIZE;
        break;

        /***********************************************************************************************
          Validation Seed
        ***********************************************************************************************/
        case PHYNPROFILE_VALIDATION_SEED_UUID:
            if (PWD_VALIDATION_SEED_SIZE <= maxLen)
            {
                *pLen = PWD_VALIDATION_SEED_SIZE;
                VOID memcpy( pValue, (uint8 *)pAttr->pValue, PWD_VALIDATION_SEED_SIZE );
                status = SUCCESS;
             }
             else
                status = ATT_ERR_INVALID_VALUE_SIZE;

        break;

        /***********************************************************************************************
         Validation Key
        ***********************************************************************************************/
        case PHYNPROFILE_VALIDATION_KEY_UUID:
            if (PWD_VALIDATION_KEY_SIZE <= maxLen)
            {
                *pLen = PWD_VALIDATION_KEY_SIZE;
                VOID memcpy( pValue, (uint8 *)pAttr->pValue, PWD_VALIDATION_KEY_SIZE );
                status = SUCCESS;
              }
             else
                status = ATT_ERR_INVALID_VALUE_SIZE;
         break;


         /***********************************************************************************************
          Status
         ***********************************************************************************************/
         case PHYNPROFILE_STATUS_UUID:
             if (PWD_STATUS_SIZE <= maxLen)
             {
                 *pLen = PWD_STATUS_SIZE;
                 VOID memcpy( pValue, (uint8 *)pAttr->pValue, PWD_STATUS_SIZE );
                 status = SUCCESS;
               }
              else
                 status = ATT_ERR_INVALID_VALUE_SIZE;
          break;

          /***********************************************************************************************
           Command
          ***********************************************************************************************/
          case PHYNPROFILE_COMMAND_UUID:
              if (PWD_COMMAND_SIZE <= maxLen)
              {
                  *pLen = PWD_COMMAND_SIZE;
                  VOID memcpy( pValue, (uint8 *)pAttr->pValue, PWD_COMMAND_SIZE );
                  status = SUCCESS;
                }
               else
                  status = ATT_ERR_INVALID_VALUE_SIZE;
           break;


           /***********************************************************************************************
            Message Counter
           **********************************************************************************************
           case PHYNPROFILE_MESSAGE_COUNTER_UUID:
               if (PWD_MESSAGE_COUNTER_SIZE <= maxLen)
               {
                   *pLen = PWD_MESSAGE_COUNTER_SIZE;
                   VOID memcpy( pValue, (uint8 *)pAttr->pValue, PWD_MESSAGE_COUNTER_SIZE );
                   status = SUCCESS;
                 }
                else
                   status = ATT_ERR_INVALID_VALUE_SIZE;
            break;

         */


        /***********************************************************************************************
         Message From App to MT
        ***********************************************************************************************
        case MSG_FROM_APP:
            if (PWD_RESPONSE_SIZE >= maxLen)
            {
                *pLen = PWD_RESPONSE_SIZE;
                VOID memcpy( pValue, (uint8 *)pAttr->pValue, maxLen );
                status = SUCCESS;
            }
            else
                status = ATT_ERR_INVALID_VALUE_SIZE;
        break;
        */
#endif

      /*********************************************************************
       Setup attribute Read/Write CALLBACKS
      *********************************************************************
      CONST gattServiceCBs_t ExtraRdWrCBs =
      {
        ExtraProfile_ReadAttrCB,  // Read callback function pointer
        ExtraProfile_WriteAttrCB, // Write callback function pointer
        NULL                     // Authorization callback function pointer
      };
      */

      /*
          Build_ExtraProfileAttrTbl();

          status = GATTServApp_RegisterService( ExtraProfileAttrTbl,
                                                GATT_NUM_ATTRS( ExtraProfileAttrTbl ),
                                                GATT_MAX_ENCRYPT_KEY_SIZE,
                                                &ExtraRdWrCBs);
        */

           /***********************************************************************************************
               SR1_UUID - SR20_UUID
           ***********************************************************************************************
          case SR_UUID+1:  case SR_UUID+2:  case SR_UUID+3:  case SR_UUID+4:  case SR_UUID+5:
          case SR_UUID+6:  case SR_UUID+7:  case SR_UUID+8:  case SR_UUID+9:  case SR_UUID+10:
          case SR_UUID+11:  case SR_UUID+12:  case SR_UUID+13:  case SR_UUID+14:  case SR_UUID+15:
          case SR_UUID+16:  case SR_UUID+17:  case SR_UUID+18:  case SR_UUID+19:  case SR_UUID+20:

             // Send a chunk of field
             *pLen = Gatt_SR_Tx(uuid-SR_UUID-1,  pValue,  pAttr->pValue, maxLen);
             status = SUCCESS;


          break;
  */

         /***********************************************************************************************
              Validation Key
           ***********************************************************************************************/
   #if 0
           case PHYNPROFILE_VALIDATION_KEY_UUID:

                 if ( offset == 0 )
                 {
                       if ( len != PWD_VALIDATION_KEY_SIZE )
                       {
                           status = ATT_ERR_INVALID_VALUE_SIZE;
                       }
                       else
                       {
                           VOID memcpy( (uint8 *)pAttr->pValue, pValue, PWD_VALIDATION_KEY_SIZE );
                           //uChangedValueID = PWD_VALIDATION_KEY;
                           status = SUCCESS;
                           Log_With_U32( "App CHANGED Validation Key to ", *(U32 *)pValue);
                       }
                 }
                 else
                   status = ATT_ERR_ATTR_NOT_LONG;

           break;

           /***********************************************************************************************
              Command
           ***********************************************************************************************/
           case PHYNPROFILE_COMMAND_UUID:

                 if ( offset == 0 )
                 {
                       if ( len != PWD_COMMAND_SIZE )
                           status = ATT_ERR_INVALID_VALUE_SIZE;
                       else
                       {
                           VOID memcpy( (uint8 *)pAttr->pValue, pValue, PWD_COMMAND_SIZE );
                          // uChangedValueID = PWD_COMMAND;
                           status = SUCCESS;
                           Log_With_U8( "App COMMAND:", *pValue);
                       }
                 }
                 else
                   status = ATT_ERR_ATTR_NOT_LONG;

           break;
   #endif


           /*
           const U8 SR2_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 2), HI_UINT16(SR_UUID + 2) };
           const U8 SR3_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 3), HI_UINT16(SR_UUID + 3) };
           const U8 SR4_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 4), HI_UINT16(SR_UUID + 4) };
           const U8 SR5_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 5), HI_UINT16(SR_UUID + 5) };
           const U8 SR6_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 6), HI_UINT16(SR_UUID + 6) };
           const U8 SR7_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 7), HI_UINT16(SR_UUID + 7) };
           const U8 SR8_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 8), HI_UINT16(SR_UUID + 8) };
           const U8 SR9_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 9), HI_UINT16(SR_UUID + 9) };
           const U8 SR10_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 10), HI_UINT16(SR_UUID + 10) };
           const U8 SR11_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 11), HI_UINT16(SR_UUID + 1) };
           const U8 SR12_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 12), HI_UINT16(SR_UUID + 2) };
           const U8 SR13_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 13), HI_UINT16(SR_UUID + 3) };
           const U8 SR14_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 14), HI_UINT16(SR_UUID + 4) };
           const U8 SR15_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 15), HI_UINT16(SR_UUID + 5) };
           const U8 SR16_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 16), HI_UINT16(SR_UUID + 6) };
           const U8 SR17_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 17), HI_UINT16(SR_UUID + 7) };
           const U8 SR18_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 18), HI_UINT16(SR_UUID + 8) };
           const U8 SR19_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 19), HI_UINT16(SR_UUID + 9) };
           const U8 SR20_UUID[ATT_BT_UUID_SIZE] = { LO_UINT16(SR_UUID + 20), HI_UINT16(SR_UUID + 10) };

           //U8 cSR_Props = GATT_PROP_READ;
           */

           /*********************************************************************
            Message -  EXTRA1                       (0x89)
                       EXTRA_UUID1                 (0xFFD0)
           *********************************************************************
           #define EXTRA_UUID1 (0xFFD0)


             U8 sExtraMsg1Desc[] = "Extra #1";
             U8 sExtraMsg1[EXTRA1_SIZE+1] = {"12345678901234567890123456789012345678901234567890"};

           CONST U8 Extra1_UUID[ATT_BT_UUID_SIZE] =
           {
             LO_UINT16(EXTRA_UUID1), HI_UINT16(EXTRA_UUID1)
           };

           U8 cExtra1_Props = GATT_PROP_READ;
           */

           //const U8 sSR1_Desc[] = "Test Desc";

		   
		   
#if 0
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR2_UUID },         GATT_PERMIT_READ, 0, sSR_Data[1] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR3_UUID },         GATT_PERMIT_READ, 0, sSR_Data[2] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR4_UUID },         GATT_PERMIT_READ, 0, sSR_Data[3] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR5_UUID },         GATT_PERMIT_READ, 0, sSR_Data[4] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif


      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR6_UUID },         GATT_PERMIT_READ, 0, sSR_Data[5] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif


      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR7_UUID },         GATT_PERMIT_READ, 0, sSR_Data[6] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR8_UUID },         GATT_PERMIT_READ, 0, sSR_Data[7] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR9_UUID },         GATT_PERMIT_READ, 0, sSR_Data[8] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR10_UUID },        GATT_PERMIT_READ, 0, sSR_Data[9] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR11_UUID },        GATT_PERMIT_READ, 0, sSR_Data[10] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR12_UUID },        GATT_PERMIT_READ, 0, sSR_Data[11] }, // Value
        #ifdef USE_SR_DESC
          { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

/*  Cutoff #1 */
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR13_UUID },        GATT_PERMIT_READ, 0, sSR_Data[12] }, // Value
      { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description


      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR14_UUID },        GATT_PERMIT_READ, 0, sSR_Data[13] }, // Value

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR15_UUID },        GATT_PERMIT_READ, 0, sSR_Data[14] }, // Value

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR16_UUID },        GATT_PERMIT_READ, 0, sSR_Data[15] }, // Value

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR17_UUID },        GATT_PERMIT_READ, 0, sSR_Data[16] }, // Value

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR18_UUID },        GATT_PERMIT_READ, 0, sSR_Data[17] }, // Value

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR19_UUID },        GATT_PERMIT_READ, 0, sSR_Data[18] }, // Value

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR20_UUID },        GATT_PERMIT_READ, 0, sSR_Data[19] }, // Value
#endif

      #if 0
      /*************************************************************************************************
        Test messages
      *************************************************************************************************/
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &cExtra1_Props
      },

      // Value
      {
           { ATT_BT_UUID_SIZE, Extra1_UUID },
             GATT_PERMIT_READ, 0,  sExtraMsg1
       },

       // User Description
       {
           { ATT_BT_UUID_SIZE, charUserDescUUID },
             GATT_PERMIT_READ, 0, sExtraMsg1Desc
       },
#endif


      gattAttribute_t ExtraProfileAttrTbl[2];

      I8 ExtraProfileServiceVal[2][64];
