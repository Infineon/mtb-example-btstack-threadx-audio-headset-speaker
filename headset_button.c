/******************************************************************************
* File Name:   headset_button.c
*
* Description: Application's Button-list entries
*
* Related Document:
*
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

/*******************************************************************************
* Header Files
*******************************************************************************/
#include <bt_hs_spk_button.h>
#include <stddef.h>
#include <stdint.h>

#include "wiced.h"
#include "wiced_button_manager.h"
#include "wiced_platform.h"
#include "wiced_result.h"

/*******************************************************************************
* Macros
********************************************************************************/
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)                                ( sizeof(a) / sizeof(a[0]) )
#endif // ARRAY_SIZE

/*******************************************************************************
* Global Variables
********************************************************************************/
static button_manager_t app_button_manager;

static wiced_button_manager_configuration_t app_button_manager_configuration =
{
    .short_hold_duration     = 500, /*msec*/
    .medium_hold_duration    = 700,
    .long_hold_duration      = 1500,
    .very_long_hold_duration = 5000,
    .debounce_duration       = 150, /* typically a click takes around ~150-200 ms */
    .continuous_hold_detect  = WICED_TRUE,
    /*if NULL button events are handled by bt_hs_spk library*/
    .event_handler = NULL,
};

/* Static button configuration */
static wiced_button_configuration_t app_button_configurations[] =
{
#if (APP_BUTTON_MAX == 1)
    [ PLAY_PAUSE_BUTTON ]                   = { PLATFORM_BUTTON_1, BUTTON_CLICK_EVENT | BUTTON_LONG_DURATION_EVENT | BUTTON_VERY_LONG_DURATION_EVENT , 0 },
#else
    [ PLAY_PAUSE_BUTTON ]                   = { PLATFORM_BUTTON_1, BUTTON_CLICK_EVENT | BUTTON_LONG_DURATION_EVENT | BUTTON_VERY_LONG_DURATION_EVENT , 0 },
    [ VOLUME_UP_NEXT_TRACK_BUTTON ]         = { PLATFORM_BUTTON_2, BUTTON_CLICK_EVENT | BUTTON_LONG_DURATION_EVENT | BUTTON_VERY_LONG_DURATION_EVENT | BUTTON_HOLDING_EVENT , 0 },
    [ VOLUME_DOWN_PREVIOUS_TRACK_BUTTON ]   = { PLATFORM_BUTTON_3, BUTTON_CLICK_EVENT | BUTTON_LONG_DURATION_EVENT | BUTTON_VERY_LONG_DURATION_EVENT | BUTTON_HOLDING_EVENT , 0 },
#if (APP_BUTTON_MAX >= 4)
    [ VOICE_REC_BUTTON ]                    = { PLATFORM_BUTTON_4, BUTTON_CLICK_EVENT | BUTTON_LONG_DURATION_EVENT | BUTTON_VERY_LONG_DURATION_EVENT | BUTTON_HOLDING_EVENT, 0 },
#endif
#endif
};

/* Button objects for the button manager */
button_manager_button_t app_buttons[] =
{
#if (APP_BUTTON_MAX == 1)
    [ PLAY_PAUSE_BUTTON ]                   = { &app_button_configurations[ PLAY_PAUSE_BUTTON ]        },
#else
    [ PLAY_PAUSE_BUTTON ]                   = { &app_button_configurations[ PLAY_PAUSE_BUTTON ]        },
    [ VOLUME_UP_NEXT_TRACK_BUTTON ]         = { &app_button_configurations[ VOLUME_UP_NEXT_TRACK_BUTTON ]     },
    [ VOLUME_DOWN_PREVIOUS_TRACK_BUTTON ]   = { &app_button_configurations[ VOLUME_DOWN_PREVIOUS_TRACK_BUTTON ]  },
#if (APP_BUTTON_MAX >= 4)
    [ VOICE_REC_BUTTON ]                    = { &app_button_configurations[ VOICE_REC_BUTTON ] },
#endif
#endif
};

static bt_hs_spk_button_action_t app_button_action[] =
{
#if (APP_BUTTON_MAX == 1)
    /* PLAY_PAUSE_BUTTON */
    {
        .action = ACTION_PAUSE_PLAY,
        .button = PLAY_PAUSE_BUTTON,
        .event  = BUTTON_CLICK_EVENT,
        .state  = BUTTON_STATE_RELEASED,
    },
    {
        .action = ACTION_BT_DISCOVERABLE,
        .button = PLAY_PAUSE_BUTTON,
        .event  = BUTTON_LONG_DURATION_EVENT,
        .state  = BUTTON_STATE_HELD,
    },
#else
    /* PLAY_PAUSE_BUTTON */
    {
        .action = ACTION_PAUSE_PLAY,
        .button = PLAY_PAUSE_BUTTON,
        .event  = BUTTON_CLICK_EVENT,
        .state  = BUTTON_STATE_RELEASED,
    },
    {
        .action = ACTION_BT_DISCOVERABLE,
        .button = PLAY_PAUSE_BUTTON,
        .event  = BUTTON_LONG_DURATION_EVENT,
        .state  = BUTTON_STATE_HELD,
    },

    /* VOLUME_UP_NEXT_TRACK_BUTTON */
    {
        .action = ACTION_VOLUME_UP,
        .button = VOLUME_UP_NEXT_TRACK_BUTTON,
        .event  = BUTTON_CLICK_EVENT,
        .state  = BUTTON_STATE_RELEASED,
    },
    {
        .action = ACTION_FORWARD,
        .button = VOLUME_UP_NEXT_TRACK_BUTTON,
        .event  = BUTTON_LONG_DURATION_EVENT,
        .state  = BUTTON_STATE_RELEASED,
    },
#if (APP_BUTTON_MAX < 4)
    {
        .action = ACTION_VOICE_RECOGNITION,
        .button = VOLUME_UP_NEXT_TRACK_BUTTON,
        .event  = BUTTON_HOLDING_EVENT,
        .state  = BUTTON_STATE_HELD,
        .repeat = 2,
    },
#endif

    /* VOLUME_DOWN_PREVIOUS_TRACK_BUTTON */
    {
        .action = ACTION_VOLUME_DOWN,
        .button = VOLUME_DOWN_PREVIOUS_TRACK_BUTTON,
        .event  = BUTTON_CLICK_EVENT,
        .state  = BUTTON_STATE_RELEASED,
    },
    {
        .action = ACTION_BACKWARD,
        .button = VOLUME_DOWN_PREVIOUS_TRACK_BUTTON,
        .event  = BUTTON_LONG_DURATION_EVENT,
        .state  = BUTTON_STATE_RELEASED,
    },
#ifdef ENABLE_PTS_TESTING
    {
        .action = ACTION_MULTI_FUNCTION_LONG_RELEASE,
        .button = VOLUME_DOWN_PREVIOUS_TRACK_BUTTON,
        .event  = BUTTON_HOLDING_EVENT,
        .state  = BUTTON_STATE_HELD,
        .repeat = 1,
    },
#endif
#if (APP_BUTTON_MAX < 4)
    {
        .action = ACTION_TRANSPORT_DETECT_ON,
        .button = VOLUME_DOWN_PREVIOUS_TRACK_BUTTON,
        .event  = BUTTON_HOLDING_EVENT,
        .state  = BUTTON_STATE_HELD,
        .repeat = 2,
    },
#endif

#if (APP_BUTTON_MAX >= 4)
    /* VOICE_REC_BUTTON */
    {
        .action = ACTION_VOICE_RECOGNITION,
        .button = VOICE_REC_BUTTON,
        .event  = BUTTON_HOLDING_EVENT,
        .state  = BUTTON_STATE_HELD,
        .repeat = 2,
    },
    {
        .action = ACTION_TRANSPORT_DETECT_ON,
        .button = VOICE_REC_BUTTON,
        .event  = BUTTON_CLICK_EVENT,
        .state  = BUTTON_STATE_RELEASED,
    },
#endif
#endif
};

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static wiced_bool_t headset_button_pre_handler(platform_button_t button, button_manager_event_t event, button_manager_button_state_t state, uint32_t repeat);

/*******************************************************************************
* Global Function Definitions
*******************************************************************************/

wiced_result_t btheadset_init_button_interface(void)
{
    wiced_result_t result;
    bt_hs_spk_button_config_t config;

    config.p_manager                                = &app_button_manager;
    config.p_configuration                          = &app_button_manager_configuration;
    config.p_app_buttons                            = app_buttons;
    config.number_of_buttons                        = ARRAY_SIZE(app_buttons);
#if defined(CYW43012C0)
    config.p_pre_handler                            = NULL;
#else
    config.p_pre_handler                            = &headset_button_pre_handler;
#endif
    config.button_action_config.p_action            = app_button_action;
    config.button_action_config.number_of_actions   = ARRAY_SIZE(app_button_action);

    result = bt_hs_spk_init_button_interface(&config);
    return result;
}

/*******************************************************************************
* Static Function Definitions
*******************************************************************************/
static wiced_bool_t headset_button_pre_handler(platform_button_t button, button_manager_event_t event, button_manager_button_state_t state, uint32_t repeat)
{
#ifdef AUDIO_INSERT_ENABLED
    if ((button == (platform_button_t)VOLUME_UP_NEXT_TRACK_BUTTON) &&
        (event == BUTTON_CLICK_EVENT) &&
        (state == BUTTON_STATE_RELEASED))
    {
        /* Check if call session exists. */
        if (bt_hs_spk_handsfree_call_session_check())
        {
            if (bt_hs_spk_handsfree_volume_get() == WICED_HANDSFREE_VOLUME_MAX)
            {
                /* Already maximum volume */
                /* Prompt audio to indicate the volume is already at maximum */
                headset_button_audio_insert_config.sample_rate = bt_hs_spk_handsfree_audio_manager_sampling_rate_get();
                headset_button_audio_insert_config.duration    = HEADSET_BUTTON_AUDIO_INSERT_DURATION;
                headset_button_audio_insert_config.p_source    = sine_wave_mono;
                headset_button_audio_insert_config.len         = sizeof(sine_wave_mono);
                headset_button_audio_insert_config.stopped_when_state_is_changed = WICED_TRUE;
                headset_button_audio_insert_config.p_timeout_callback = NULL;

                bt_hs_spk_audio_insert_start(&headset_button_audio_insert_config);

                WICED_BT_TRACE("AUDIO_INSERT_STARTED duration:%d sample_rate:%d\n",
                               headset_button_audio_insert_config.duration,
                               headset_button_audio_insert_config.sample_rate);
            }
        }

        /* Check if the audio streaming exists.  */
        if (bt_hs_spk_audio_streaming_check(NULL) == WICED_ALREADY_CONNECTED)
        {
            if (bt_hs_spk_audio_volume_get() == BT_HS_SPK_AUDIO_VOLUME_MAX)
            {
                /* Already maximum volume */
                /* Prompt audio to indicate the volume is already at maximum */
                headset_button_audio_insert_config.sample_rate = bt_hs_spk_audio_audio_manager_sampling_rate_get();
                headset_button_audio_insert_config.duration    = HEADSET_BUTTON_AUDIO_INSERT_DURATION;
                headset_button_audio_insert_config.p_source    = bt_hs_spk_audio_audio_manager_channel_number_get() > 1 ? sine_wave_stereo : sine_wave_mono;
                headset_button_audio_insert_config.len         = bt_hs_spk_audio_audio_manager_channel_number_get() > 1 ? sizeof(sine_wave_stereo) : sizeof(sine_wave_mono);
                headset_button_audio_insert_config.stopped_when_state_is_changed = WICED_TRUE;
                headset_button_audio_insert_config.p_timeout_callback = NULL;

                bt_hs_spk_audio_insert_start(&headset_button_audio_insert_config);

                WICED_BT_TRACE("AUDIO_INSERT_STARTED duration:%d sample_rate:%d\n",
                               headset_button_audio_insert_config.duration,
                               headset_button_audio_insert_config.sample_rate);
            }
        }
    }
#endif

    return WICED_TRUE;
}

/* [] END OF FILE */
