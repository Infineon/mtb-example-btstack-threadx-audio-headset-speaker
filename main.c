/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the Headset Speaker Example
*              for ModusToolbox.
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

#include "cybsp.h"
#include "cyhal.h"
#include "cy_pdl.h"
#include "cy_retarget_io.h"
#include "headset_control.h"
#include "wiced_bt_trace.h"
#include "wiced_bt_dev.h"
#ifdef HCI_TRACE_OVER_TRANSPORT
#include "wiced_transport.h"
#include "wiced_hal_puart.h"
#include "hci_control_api.h"
#endif

/*******************************************************************************
* Macros
********************************************************************************/
#ifdef HCI_TRACE_OVER_TRANSPORT
#define TRANS_UART_BUFFER_SIZE 1024
#define TRANSPORT_BUFFER_SIZE 1500
#define TRANSPORT_BUFFER_COUNT 2
#define TRANSPORT_UART_BAUD_RATE 3000000
typedef wiced_bool_t (*classic_audio_rpc_cback_t)(uint16_t opcode, uint8_t *p_data, uint32_t data_len);
#endif

/*******************************************************************************
* Function Prototypes
********************************************************************************/
#ifdef HCI_TRACE_OVER_TRANSPORT
static void classic_audio_rpc_transport_status_handler(wiced_transport_type_t type);
static uint32_t classic_audio_rpc_rx_callback(uint8_t *p_buffer, uint32_t length);
static classic_audio_rpc_cback_t g_rpc_app_callback;
#endif

/*******************************************************************************
* Global Variables
********************************************************************************/

#ifdef HCI_TRACE_OVER_TRANSPORT
const wiced_transport_cfg_t transport_cfg = {
    WICED_TRANSPORT_UART,
    {
        {WICED_TRANSPORT_UART_HCI_MODE, TRANSPORT_UART_BAUD_RATE},
    },

    .heap_config = {.data_heap_size =
                        ((TRANS_UART_BUFFER_SIZE * 4) + (TRANSPORT_BUFFER_SIZE * TRANSPORT_BUFFER_COUNT)), // Tx, Rx
                    .hci_trace_heap_size = TRANS_UART_BUFFER_SIZE * 2,
                    .debug_trace_heap_size = 1024},
    classic_audio_rpc_transport_status_handler,
    classic_audio_rpc_rx_callback,
    NULL};
#endif

/*******************************************************************************
* Function
********************************************************************************/

#ifdef HCI_TRACE_OVER_TRANSPORT

static void rpc_hci_trace_cback(wiced_bt_hci_trace_type_t type, uint16_t length, uint8_t *p_data)
{
    wiced_transport_send_hci_trace(type, p_data, (length > TRANS_UART_BUFFER_SIZE) ? TRANS_UART_BUFFER_SIZE : length);
}



static void classic_audio_rpc_transport_status_handler(wiced_transport_type_t type)
{
    wiced_transport_send_data(HCI_CONTROL_EVENT_DEVICE_STARTED, NULL, 0);
}


static uint32_t classic_audio_rpc_rx_callback(uint8_t *p_buffer, uint32_t length)
{
    uint16_t opcode;
    int payload_len = 0;
    uint8_t *p_data = p_buffer;

    if (!p_buffer)
    {
        return HCI_CONTROL_STATUS_INVALID_ARGS;
    }

    // Expected minimum 4 byte as the wiced header
    if ((length < 4) || (p_data == NULL))
    {
        WICED_BT_TRACE("invalid params\n");
        wiced_transport_free_buffer(p_buffer);
        return HCI_CONTROL_STATUS_INVALID_ARGS;
    }

    STREAM_TO_UINT16(opcode, p_data);      // Get OpCode
    STREAM_TO_UINT16(payload_len, p_data); // Gen Payload Length

    if (g_rpc_app_callback) g_rpc_app_callback(opcode, p_data, payload_len);

    return HCI_CONTROL_STATUS_SUCCESS;
}


void wiced_hci_trace_enable(void)
{
    wiced_transport_init(&transport_cfg);
    wiced_set_debug_uart(WICED_ROUTE_DEBUG_TO_WICED_UART);

    wiced_bt_dev_register_hci_trace(rpc_hci_trace_cback);
}

#endif // #ifdef HCI_TRACE_OVER_TRANSPORT

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:      Application entry point
*
* Parameters:   void
*
* Return:
*  int
*
*******************************************************************************/


int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

#ifdef HCI_TRACE_OVER_TRANSPORT
    wiced_hci_trace_enable();
#endif

    WICED_BT_TRACE( "#############################\n" );
    WICED_BT_TRACE( "# headset_speaker APP START #\n" );
    WICED_BT_TRACE( "#############################\n" );

    btheadset_control_init();

    return 0;
}


/* [] END OF FILE */
