/******************************************************************************
* File Name:   headset_control_le.h
*
* Description:
*
* Related Document: None
*
*******************************************************************************
* (c) 2021-2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*******************************************************************************/
#ifndef HEADSET_CONTROL_LE_H
#define HEADSET_CONTROL_LE_H

/*******************************************************************************
*        Header Files
*******************************************************************************/

/*******************************************************************************
*        Macro Definitions
*******************************************************************************/
enum
{
    HANDLE_HSENS_GATT_SERVICE = 0x1, // service handle

    HANDLE_HSENS_GAP_SERVICE = 0x14, // service handle
        HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_NAME, // characteristic handl
        HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_NAME_VAL, // char value handle

        HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_APPEARANCE, // characteristic handl
        HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_APPEARANCE_VAL,// char value handle

    HANDLE_HSENS_DEV_INFO_SERVICE = 0x40,
        HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MFR_NAME, // characteristic handle
        HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MFR_NAME_VAL,// char value handle

        HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MODEL_NUM, // characteristic handl
        HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MODEL_NUM_VAL,// char value handle

        HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_SYSTEM_ID, // characteristic handl
        HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_SYSTEM_ID_VAL,// char value handle

    HANDLE_HSENS_BATTERY_SERVICE = 0x60, // service handle
        HANDLE_HSENS_BATTERY_SERVICE_CHAR_LEVEL, // characteristic handl
        HANDLE_HSENS_BATTERY_SERVICE_CHAR_LEVEL_VAL, // char value andle

    HANDLE_FASTPAIR_SERVICE = 0x70,
        HANDLE_FASTPAIR_SERVICE_CHAR_KEY_PAIRING,
        HANDLE_FASTPAIR_SERVICE_CHAR_KEY_PAIRING_VAL,
        HANDLE_FASTPAIR_SERVICE_CHAR_KEY_PAIRING_CFG_DESC,
        HANDLE_FASTPAIR_SERVICE_CHAR_PASSKEY,
        HANDLE_FASTPAIR_SERVICE_CHAR_PASSKEY_VAL,
        HANDLE_FASTPAIR_SERVICE_CHAR_PASSKEY_CFG_DESC,
        HANDLE_FASTPAIR_SERVICE_CHAR_ACCOUNT_KEY,
        HANDLE_FASTPAIR_SERVICE_CHAR_ACCOUNT_KEY_VAL,
        HANDLE_FASTPAIR_SERVICE_CHAR_ACCOUNT_KEY_CFG_DESC,

        // Client Configuration
        HDLD_CURRENT_TIME_SERVICE_CURRENT_TIME_CLIENT_CONFIGURATION,
};

/*******************************************************************************
*        External Variable Declarations
*******************************************************************************/

/*******************************************************************************
*        Function Prototypes
*******************************************************************************/

/*******************************************************************************
* Function Name: hci_control_le_enable
********************************************************************************
* Summary:
*   Enable LE Control
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
void hci_control_le_enable(void);

#endif // HEADSET_CONTROL_LE_H

/* [] END OF FILE */
