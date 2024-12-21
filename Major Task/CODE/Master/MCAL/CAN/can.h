/*
 * CAN.h
 *
 *  Created on: 22 Nov 2024
 *      Author: Team: 4
 *      purpose: 1) Provide function prototypes for CAN initialization and communication.
 *               2) Abstract hardware-specific CAN details for easier implementation.
 *               3) Facilitate master-slave communication and acknowledge signaling.
 *               4) Define shared global variables and flags for CAN-based communication tasks.
 */

#ifndef CAN_H_
#define CAN_H_


/***********************************************
 * Includes
 ***********************************************/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/can.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "can_config.h"


/***********************************************
 * Definitions and Macros
 ***********************************************/
#define CAN_MODULE_0   0   // Select CAN0
#define CAN_MODULE_1   1   // Select CAN1

#define CAN_BIT_RATE_125K           125000  // 125 Kbps
#define CAN_BIT_RATE_250K           250000  // 250 Kbps
#define CAN_BIT_RATE_500K           500000  // 500 Kbps
#define CAN_BIT_RATE_1M             1000000 // 1 Mbps

#define NO_FILTERING                0x0
#define USE_ID_FILTER               0x700

typedef enum {
    MSG_OBJ_TX_1 = 1,   // Use Message Object 1 for TX
    MSG_OBJ_TX_2,       // Use Message Object 2 for TX
    MSG_OBJ_TX_3,       // Use Message Object 3 for TX
    MSG_OBJ_TX_4,       // Use Message Object 4 for TX
    MSG_OBJ_TX_5,       // Use Message Object 5 for TX
    MSG_OBJ_TX_6,       // Use Message Object 6 for TX
    MSG_OBJ_TX_7,       // Use Message Object 7 for TX
    MSG_OBJ_TX_8,       // Use Message Object 8 for TX
    MSG_OBJ_TX_9,       // Use Message Object 9 for TX
    MSG_OBJ_TX_10,      // Use Message Object 10 for TX
    MSG_OBJ_TX_11,      // Use Message Object 11 for TX
    MSG_OBJ_TX_12,      // Use Message Object 12 for TX
    MSG_OBJ_TX_13,      // Use Message Object 13 for TX
    MSG_OBJ_TX_14,      // Use Message Object 14 for TX
    MSG_OBJ_TX_15,      // Use Message Object 15 for TX
    MSG_OBJ_TX_16       // Use Message Object 16 for TX
} CAN_TX_MessageObject_t;

typedef enum {
    MSG_OBJ_RX_1 = 0x17,   // Use Message Object 17 for RX
    MSG_OBJ_RX_2,        // Use Message Object 18 for RX
    MSG_OBJ_RX_3,        // Use Message Object 19 for RX
    MSG_OBJ_RX_4,        // Use Message Object 20 for RX
    MSG_OBJ_RX_5,        // Use Message Object 21 for RX
    MSG_OBJ_RX_6,        // Use Message Object 22 for RX
    MSG_OBJ_RX_7,        // Use Message Object 23 for RX
    MSG_OBJ_RX_8,        // Use Message Object 24 for RX
    MSG_OBJ_RX_9,        // Use Message Object 25 for RX
    MSG_OBJ_RX_10,       // Use Message Object 26 for RX
    MSG_OBJ_RX_11,       // Use Message Object 27 for RX
    MSG_OBJ_RX_12,       // Use Message Object 28 for RX
    MSG_OBJ_RX_13,       // Use Message Object 29 for RX
    MSG_OBJ_RX_14,       // Use Message Object 30 for RX
    MSG_OBJ_RX_15,       // Use Message Object 31 for RX
    MSG_OBJ_RX_16        // Use Message Object 32 for RX
} CAN_RX_MessageObject_t;


#define CAN_INT_ENABLE              1   // Enable CAN Interrupts
#define CAN_INT_DISABLE             0   // Disable CAN Interrupts

#define CAN_DATA_LEN_0              0x00U
#define CAN_DATA_LEN_1              0x01U
#define CAN_DATA_LEN_2              0x02U
#define CAN_DATA_LEN_3              0x03U
#define CAN_DATA_LEN_4              0x04U
#define CAN_DATA_LEN_5              0x05U
#define CAN_DATA_LEN_6              0x06U
#define CAN_DATA_LEN_7              0x07U
#define CAN_DATA_LEN_8              0x08U


/***********************************************
 * Type Declarations (enums, structs and unions)
 ***********************************************/



/***********************************************
 * Functions Prototypes
 ***********************************************/
void CAN_Init(void);
void CAN_Send(uint32_t messageID, uint8_t *data, uint8_t dataLength);
void CAN_ReceiveInit(void);
void OS_voidCANReceiveMessage(void);



#endif /* CAN_H_ */
