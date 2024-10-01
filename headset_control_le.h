/******************************************************************************
* File Name:   headset_control_le.h
*
* Description:
*
* Related Document: None
*
*******************************************************************************
* Copyright 2021-2024, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
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
