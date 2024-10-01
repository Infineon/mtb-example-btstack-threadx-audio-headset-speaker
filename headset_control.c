/******************************************************************************
* File Name:   headset_control.c
*
* Description: 
*
* Related Document: See README.md
*
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

/*******************************************************************************
* Header Files
*******************************************************************************/
#include <stdint.h>

#include "bt_hs_spk_control.h"
#include "bt_hs_spk_handsfree.h"
#include "headset_control.h"
#include "headset_control_le.h"
#include "headset_nvram.h"
#include "wiced_app_cfg.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_stack.h"
#include "wiced_memory.h"
#include "wiced_result.h"
#include "wiced.h"

/*******************************************************************************
* Macros
********************************************************************************/
#define BT_STACK_HEAP_SIZE  (1024 * 7)

/*******************************************************************************
* Structures
********************************************************************************/
/* Local Identify Resolving Key. */
typedef struct
{
    wiced_bt_local_identity_keys_t  local_irk;
    wiced_result_t                  result;
} headset_control_local_irk_info_t;

/*******************************************************************************
* Global Variables
********************************************************************************/
wiced_bt_heap_t *p_default_heap = NULL;
static headset_control_local_irk_info_t local_irk_info = {0};

extern const uint8_t                btheadset_sdp_db[];
extern wiced_bt_a2dp_config_data_t  bt_audio_config;
extern uint8_t                      bt_avrc_ct_supported_events[];
extern wiced_bt_audio_config_buffer_t wiced_bt_audio_buf_config;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static wiced_result_t   btheadset_control_management_callback(wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data);
static wiced_result_t   btheadset_post_bt_init(void);
static void             headset_control_local_irk_restore(void);
static void             headset_control_local_irk_update(uint8_t *p_key);

/*******************************************************************************
* Global Function Definitions
*******************************************************************************/

/*******************************************************************************
* Function Name:
********************************************************************************
* Summary:      Initialize bt stack, required peripherals, and features.
*
* Parameters:   void
*
* Return:       void
*
*******************************************************************************/
void btheadset_control_init( void )
{
    wiced_result_t ret = WICED_BT_ERROR;

    /* Create default heap */
    p_default_heap = wiced_bt_create_heap("default_heap", NULL, BT_STACK_HEAP_SIZE, NULL, WICED_TRUE);
    if (p_default_heap == NULL)
    {
        WICED_BT_TRACE("create default heap error: size %d\n", BT_STACK_HEAP_SIZE);
        return;
    }

    /* Initialize/Enable the BT stack. */
    ret = wiced_bt_stack_init(btheadset_control_management_callback, &wiced_bt_cfg_settings);

    if( ret != WICED_BT_SUCCESS )
    {
        WICED_BT_TRACE("wiced_bt_stack_init returns error: %d\n", ret);
        return;
    }

    WICED_BT_TRACE("Device Class: 0x%02x%02x%02x\n",
            wiced_bt_cfg_settings.p_br_cfg->device_class[0],
            wiced_bt_cfg_settings.p_br_cfg->device_class[1],
            wiced_bt_cfg_settings.p_br_cfg->device_class[2]);

    /* Configure Audio buffer */
    ret = wiced_audio_buffer_initialize (wiced_bt_audio_buf_config);
    if( ret != WICED_BT_SUCCESS )
    {
        WICED_BT_TRACE("wiced_audio_buffer_initialize returns error: %d\n", ret);
        return;
    }

    /* Restore local Identify Resolving Key (IRK) for LE Private Resolvable Address. */
    headset_control_local_irk_restore();
}

/*******************************************************************************
* Static Function Definitions
*******************************************************************************/

/*
 *  Management callback receives various notifications from the stack
 */
static wiced_result_t btheadset_control_management_callback( wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data )
{
    wiced_result_t                     result = WICED_BT_SUCCESS;
    wiced_bt_dev_encryption_status_t  *p_encryption_status;
    wiced_bt_dev_pairing_cplt_t        *p_pairing_cmpl;
    uint8_t                             pairing_result;

    WICED_BT_TRACE("%s(%u)\n", __FUNCTION__, event);

    switch(event)
    {
    case BTM_ENABLED_EVT:
        if( p_event_data->enabled.status != WICED_BT_SUCCESS )
        {
            WICED_BT_TRACE("arrived with failure\n");
        }
        else
        {
            btheadset_post_bt_init();

            if (WICED_SUCCESS != btheadset_init_button_interface())
                WICED_BT_TRACE("btheadset button init failed\n");

            WICED_BT_TRACE("Free RAM sizes: %ld\n", wiced_memory_get_free_bytes());
        }
        break;

    case BTM_DISABLED_EVT:
        //hci_control_send_device_error_evt( p_event_data->disabled.reason, 0 );
        break;

    case BTM_POWER_MANAGEMENT_STATUS_EVT:
        bt_hs_spk_control_btm_event_handler_power_management_status(&p_event_data->power_mgmt_notification);
        break;

    case BTM_PIN_REQUEST_EVT:
        WICED_BT_TRACE("remote address= %B\n", p_event_data->pin_request.bd_addr);
        //wiced_bt_dev_pin_code_reply(*p_event_data->pin_request.bd_addr, WICED_BT_SUCCESS, WICED_PIN_CODE_LEN, (uint8_t *)&pincode[0]);
        break;

    case BTM_USER_CONFIRMATION_REQUEST_EVT:
        // If this is just works pairing, accept. Otherwise send event to the MCU to confirm the same value.
        WICED_BT_TRACE("BTM_USER_CONFIRMATION_REQUEST_EVT BDA %B\n", p_event_data->user_confirmation_request.bd_addr);
        if (p_event_data->user_confirmation_request.just_works)
        {
            WICED_BT_TRACE("just_works \n");
            wiced_bt_dev_confirm_req_reply( WICED_BT_SUCCESS, p_event_data->user_confirmation_request.bd_addr );
        }
        else
        {
            WICED_BT_TRACE("Need to send user_confirmation_request, Key %d \n", p_event_data->user_confirmation_request.numeric_value);
#ifdef FASTPAIR_ENABLE
            wiced_bt_gfps_provider_seeker_passkey_set(p_event_data->user_confirmation_request.numeric_value);
#endif
            wiced_bt_dev_confirm_req_reply( WICED_BT_SUCCESS, p_event_data->user_confirmation_request.bd_addr );
        }
        break;

    case BTM_PASSKEY_NOTIFICATION_EVT:
        WICED_BT_TRACE("PassKey Notification. BDA %B, Key %d \n", p_event_data->user_passkey_notification.bd_addr, p_event_data->user_passkey_notification.passkey );
        //hci_control_send_user_confirmation_request_evt(p_event_data->user_passkey_notification.bd_addr, p_event_data->user_passkey_notification.passkey );
        break;

    case BTM_PAIRING_IO_CAPABILITIES_BR_EDR_REQUEST_EVT:
        /* Use the default security for BR/EDR*/
        WICED_BT_TRACE("BTM_PAIRING_IO_CAPABILITIES_BR_EDR_REQUEST_EVT (%B)\n",
                       p_event_data->pairing_io_capabilities_br_edr_request.bd_addr);

#ifdef FASTPAIR_ENABLE
        if (wiced_bt_gfps_provider_pairing_state_get())
        {   // Google Fast Pair service Seeker triggers this pairing process.
            /* Set local capability to Display/YesNo to identify local device is not a
             * man-in-middle device.
             * Otherwise, the Google Fast Pair Service Seeker will terminate this pairing
             * process. */
            p_event_data->pairing_io_capabilities_br_edr_request.local_io_cap = BTM_IO_CAPABILITIES_DISPLAY_AND_YES_NO_INPUT;
        }
        else
#endif
        {
            p_event_data->pairing_io_capabilities_br_edr_request.local_io_cap = BTM_IO_CAPABILITIES_NONE;
        }

        p_event_data->pairing_io_capabilities_br_edr_request.auth_req     = BTM_AUTH_SINGLE_PROFILE_GENERAL_BONDING_NO;
        p_event_data->pairing_io_capabilities_br_edr_request.oob_data     = WICED_FALSE;
        //p_event_data->pairing_io_capabilities_br_edr_request.auth_req     = BTM_AUTH_ALL_PROFILES_NO;
        break;

    case BTM_PAIRING_IO_CAPABILITIES_BR_EDR_RESPONSE_EVT:
        WICED_BT_TRACE("BTM_PAIRING_IO_CAPABILITIES_BR_EDR_RESPONSE_EVT (%B, io_cap: 0x%02X) \n",
                       p_event_data->pairing_io_capabilities_br_edr_response.bd_addr,
                       p_event_data->pairing_io_capabilities_br_edr_response.io_cap);

#ifdef FASTPAIR_ENABLE
        if (wiced_bt_gfps_provider_pairing_state_get())
        {   // Google Fast Pair service Seeker triggers this pairing process.
            /* If the device capability is set to NoInput/NoOutput, end pairing, to avoid using
             * Just Works pairing method. todo*/
            if (p_event_data->pairing_io_capabilities_br_edr_response.io_cap == BTM_IO_CAPABILITIES_NONE)
            {
                WICED_BT_TRACE("Terminate the pairing process\n");
            }
        }
#endif
        break;

    case BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT:
        /* Use the default security for LE */
        WICED_BT_TRACE("BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT bda %B\n",
                p_event_data->pairing_io_capabilities_ble_request.bd_addr);

        p_event_data->pairing_io_capabilities_ble_request.local_io_cap  = BTM_IO_CAPABILITIES_NONE;
        p_event_data->pairing_io_capabilities_ble_request.oob_data      = BTM_OOB_NONE;
        p_event_data->pairing_io_capabilities_ble_request.auth_req      = BTM_LE_AUTH_REQ_SC_MITM_BOND;
        p_event_data->pairing_io_capabilities_ble_request.max_key_size  = 16;
        p_event_data->pairing_io_capabilities_ble_request.init_keys     = BTM_LE_KEY_PENC|BTM_LE_KEY_PID|BTM_LE_KEY_PCSRK|BTM_LE_KEY_LENC;
        p_event_data->pairing_io_capabilities_ble_request.resp_keys     = BTM_LE_KEY_PENC|BTM_LE_KEY_PID|BTM_LE_KEY_PCSRK|BTM_LE_KEY_LENC;
        break;

    case BTM_PAIRING_COMPLETE_EVT:
        p_pairing_cmpl = &p_event_data->pairing_complete;
        if(p_pairing_cmpl->transport == BT_TRANSPORT_BR_EDR)
        {
            pairing_result = p_pairing_cmpl->pairing_complete_info.br_edr.status;
            WICED_BT_TRACE("BREDR Pairing Result: %02x\n", pairing_result);
        }
        else
        {
            pairing_result = p_pairing_cmpl->pairing_complete_info.ble.reason;
            WICED_BT_TRACE("LE Pairing Result: %02x\n", pairing_result);
        }        
        break;

    case BTM_ENCRYPTION_STATUS_EVT:
        p_encryption_status = &p_event_data->encryption_status;

        WICED_BT_TRACE("Encryption Status:(%B) res:%d\n", p_encryption_status->bd_addr, p_encryption_status->result);

        bt_hs_spk_control_btm_event_handler_encryption_status(p_encryption_status);

        break;

    case BTM_SECURITY_REQUEST_EVT:
        WICED_BT_TRACE("Security Request Event, Pairing allowed %d\n", hci_control_cb.pairing_allowed);
        if (hci_control_cb.pairing_allowed)
        {
            wiced_bt_ble_security_grant( p_event_data->security_request.bd_addr, WICED_BT_SUCCESS );
        }
        else
        {
            // Pairing not allowed, return error
            result = WICED_BT_ERROR;
        }
        break;

    case BTM_PAIRED_DEVICE_LINK_KEYS_UPDATE_EVT:
        result = bt_hs_spk_control_btm_event_handler_link_key(event, &p_event_data->paired_device_link_keys_update) ? WICED_BT_SUCCESS : WICED_BT_ERROR;
        break;

    case BTM_PAIRED_DEVICE_LINK_KEYS_REQUEST_EVT:
        result = bt_hs_spk_control_btm_event_handler_link_key(event, &p_event_data->paired_device_link_keys_request) ? WICED_BT_SUCCESS : WICED_BT_ERROR;
        break;

    case BTM_LOCAL_IDENTITY_KEYS_UPDATE_EVT:
        WICED_BT_TRACE("BTM_LOCAL_IDENTITY_KEYS_UPDATE_EVT\n");

        headset_control_local_irk_update((uint8_t *)&p_event_data->local_identity_keys_update);
        break;

    case BTM_LOCAL_IDENTITY_KEYS_REQUEST_EVT:
        /*
         * Request to restore local identity keys from NVRAM
         * (requested during Bluetooth start up)
         * */
        WICED_BT_TRACE("BTM_LOCAL_IDENTITY_KEYS_REQUEST_EVT (%d)\n", local_irk_info.result);

        if (local_irk_info.result == WICED_BT_SUCCESS)
        {
            memcpy((void *) &p_event_data->local_identity_keys_request,
                   (void *) &local_irk_info.local_irk,
                   BTM_SECURITY_LOCAL_KEY_DATA_LEN);
        }
        else
        {
            result = WICED_BT_NO_RESOURCES;
        }
        break;

    case BTM_BLE_ADVERT_STATE_CHANGED_EVT:
        WICED_BT_TRACE("BLE_ADVERT_STATE_CHANGED_EVT:%d\n", p_event_data->ble_advert_state_changed);
        break;

    case BTM_SCO_CONNECTED_EVT:
    case BTM_SCO_DISCONNECTED_EVT:
    case BTM_SCO_CONNECTION_REQUEST_EVT:
    case BTM_SCO_CONNECTION_CHANGE_EVT:
        hf_sco_management_callback(event, p_event_data);
        break;

    case BTM_BLE_CONNECTION_PARAM_UPDATE:
        WICED_BT_TRACE("BTM_BLE_CONNECTION_PARAM_UPDATE (%B, status: %d, conn_interval: %d, conn_latency: %d, supervision_timeout: %d)\n",
                       p_event_data->ble_connection_param_update.bd_addr,
                       p_event_data->ble_connection_param_update.status,
                       p_event_data->ble_connection_param_update.conn_interval,
                       p_event_data->ble_connection_param_update.conn_latency,
                       p_event_data->ble_connection_param_update.supervision_timeout);
        break;

    case BTM_BLE_PHY_UPDATE_EVT:
        /* LE PHY Update to 1M or 2M */
        WICED_BT_TRACE("PHY config is updated as TX_PHY : %dM, RX_PHY : %dM\n",
                p_event_data->ble_phy_update_event.tx_phy,
                p_event_data->ble_phy_update_event.rx_phy);
        break;

    default:
        result = WICED_BT_USE_DEFAULT_SECURITY;
        break;
    }

    return result;
}

/*
 * Restore local Identity Resolving Key
 */
static void headset_control_local_irk_restore(void)
{
    uint16_t nb_bytes;

    nb_bytes = wiced_hal_read_nvram(HEADSET_NVRAM_ID_LOCAL_IRK,
                                    BTM_SECURITY_LOCAL_KEY_DATA_LEN,
                                    (uint8_t *)&local_irk_info.local_irk,
                                    &local_irk_info.result);

    WICED_BT_TRACE("headset_control_local_irk_restore (result: %d, nb_bytes: %d)\n",
           local_irk_info.result,
           nb_bytes);
}

/*
 * Update local Identity Resolving Key
 */
static void headset_control_local_irk_update(uint8_t *p_key)
{
    uint16_t nb_bytes;
    wiced_result_t result;

    /* Check if the IRK shall be updated. */
    if (memcmp((void *) p_key,
               (void *) &local_irk_info.local_irk,
               BTM_SECURITY_LOCAL_KEY_DATA_LEN) != 0)
    {
        nb_bytes = wiced_hal_write_nvram(HEADSET_NVRAM_ID_LOCAL_IRK,
                                         BTM_SECURITY_LOCAL_KEY_DATA_LEN,
                                         p_key,
                                         &result);

        WICED_BT_TRACE("Update local IRK (result: %d, nb_bytes: %d)\n",
               result,
               nb_bytes);

        if ((nb_bytes == BTM_SECURITY_LOCAL_KEY_DATA_LEN) &&
            (result == WICED_BT_SUCCESS))
        {
            memcpy((void *) &local_irk_info.local_irk,
                   (void *) p_key,
                   BTM_SECURITY_LOCAL_KEY_DATA_LEN);

            local_irk_info.result = result;
        }
    }
}

static wiced_result_t btheadset_post_bt_init(void)
{
    wiced_bool_t ret = WICED_FALSE;
    bt_hs_spk_control_config_t config = {0};
    bt_hs_spk_eir_config_t eir = {0};

    eir.p_dev_name              = (char *) wiced_bt_cfg_settings.device_name;
    eir.default_uuid_included   = WICED_TRUE;
    
    if (WICED_SUCCESS != bt_hs_spk_write_eir(&eir))
    {
        WICED_BT_TRACE("Write EIR Failed\n");
    }
    
    ret = wiced_bt_sdp_db_init((uint8_t *) btheadset_sdp_db, wiced_app_cfg_sdp_record_get_size());
    if (ret != TRUE)
    {
        WICED_BT_TRACE("%s Failed to Initialize SDP databse\n", __FUNCTION__);
        return WICED_BT_ERROR;
    }

    config.conn_status_change_cb            = NULL;
#ifdef LOW_POWER_MEASURE_MODE
    config.discoverable_timeout             = 60;   /* 60 Sec */
#else
    config.discoverable_timeout             = 240;  /* 240 Sec */
#endif
    config.acl3mbpsPacketSupport            = WICED_TRUE;
    config.audio.a2dp.p_audio_config        = &bt_audio_config;
    config.audio.a2dp.p_pre_handler         = NULL;
    config.audio.a2dp.post_handler          = NULL;
    config.audio.avrc_ct.p_supported_events = bt_avrc_ct_supported_events;
    config.hfp.rfcomm.buffer_size           = 700;
    config.hfp.rfcomm.buffer_count          = 4;
#if (WICED_BT_HFP_HF_WBS_INCLUDED == TRUE)
    config.hfp.feature_mask                 = WICED_BT_HFP_HF_FEATURE_3WAY_CALLING | \
                                              WICED_BT_HFP_HF_FEATURE_CLIP_CAPABILITY | \
                                              WICED_BT_HFP_HF_FEATURE_REMOTE_VOLUME_CONTROL| \
                                              WICED_BT_HFP_HF_FEATURE_HF_INDICATORS | \
                                              WICED_BT_HFP_HF_FEATURE_CODEC_NEGOTIATION | \
                                              WICED_BT_HFP_HF_FEATURE_VOICE_RECOGNITION_ACTIVATION | \
                                              WICED_BT_HFP_HF_FEATURE_ESCO_S4_SETTINGS_SUPPORT;
#else
    config.hfp.feature_mask                 = WICED_BT_HFP_HF_FEATURE_3WAY_CALLING | \
                                              WICED_BT_HFP_HF_FEATURE_CLIP_CAPABILITY | \
                                              WICED_BT_HFP_HF_FEATURE_REMOTE_VOLUME_CONTROL| \
                                              WICED_BT_HFP_HF_FEATURE_HF_INDICATORS | \
                                              WICED_BT_HFP_HF_FEATURE_VOICE_RECOGNITION_ACTIVATION | \
                                              WICED_BT_HFP_HF_FEATURE_ESCO_S4_SETTINGS_SUPPORT;
#endif

    config.nvram.link_key.id            = HEADSET_NVRAM_ID_LINK_KEYS;
    config.nvram.link_key.p_callback    = NULL;
    
    if(WICED_SUCCESS != bt_hs_spk_post_stack_init(&config))
    {
        WICED_BT_TRACE("bt_audio_post_stack_init failed\n");
        return WICED_BT_ERROR;
    }

    /*Set audio sink*/
#ifdef SPEAKER
    bt_hs_spk_set_audio_sink(AM_SPEAKERS);
    WICED_BT_TRACE("Default Application: Speaker\n");
#else
    bt_hs_spk_set_audio_sink(AM_HEADPHONES);
    WICED_BT_TRACE("Default Application: Headset\n");
#endif

#if (WICED_APP_LE_INCLUDED == TRUE)
    hci_control_le_enable();
#endif

    /*we will use the channel map provided by the phone*/
    ret = wiced_bt_dev_set_afh_channel_assessment(WICED_FALSE);
    WICED_BT_TRACE("wiced_bt_dev_set_afh_channel_assessment status:%d\n",ret);
    if (ret != WICED_BT_SUCCESS)
    {
        return WICED_BT_ERROR;
    }

#ifdef OTA_FW_UPGRADE
    if (!wiced_ota_fw_upgrade_init(NULL, NULL, NULL))
    {
        WICED_BT_TRACE("wiced_ota_fw_upgrade_init failed\n");
    }

    if (WICED_SUCCESS != ofu_spp_init())
    {
        WICED_BT_TRACE("ofu_spp_init failed\n");
        return WICED_ERROR;
    }
#endif

#ifdef AUTO_ELNA_SWITCH
    wiced_hal_rfm_auto_elna_enable(1, RX_PU);
#endif
#ifdef AUTO_EPA_SWITCH
    wiced_hal_rfm_auto_epa_enable(1, TX_PU);
#endif

    return WICED_SUCCESS;
}

/* [] END OF FILE */
