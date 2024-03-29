/*
 * stdio_uart_config.h
 *
 *  Created on: 2023. jul. 11.
 *      Author: Balint
 */

#ifndef __STDIO_UART_CONFIG_H__
#define __STDIO_UART_CONFIG_H__

#define STDIO_UART_RX_QUEUE_LENGTH              8ul
#define STDIO_UART_TX_AVAIL_QUEUE_LENGTH        16ul
#define STDIO_UART_TX_READY_QUEUE_LENGTH        STDIO_UART_TX_AVAIL_QUEUE_LENGTH
#define STDIO_UART_WRITE_TASK_PRIORITY          4ul
#define STDIO_UART_WRITE_TASK_STACKSIZE         configMINIMAL_STACK_SIZE
#define STDIO_UART_TX_BUFFER_DEPTH              512ul
#define STDIO_UART_READ_TASK_PRIORITY           4ul
#define STDIO_UART_READ_TASK_STACKSIZE          configMINIMAL_STACK_SIZE
#define STDIO_UART_STDIN_QUEUE_LENGTH           1ul
#define STDIO_UART_MAX_NUM_OF_STDIN_LISTENERS   10ul

#define STDIO_UART_USARTx                       USART3

#define STDIO_UART_TX_PIN                       GPIO_PIN_10
#define STDIO_UART_RX_PIN                       GPIO_PIN_11
#define STDIO_UART_TX_GPIO_PORT                 GPIOB
#define STDIO_UART_RX_GPIO_PORT                 GPIOB
#define STDIO_UART_GPIO_AFx_USARTx              GPIO_AF7_USART3

#define STDIO_UART_DMAx                         DMA1
#define STDIO_UART_DMAx_STREAMx                 DMA1_Stream3
#define STDIO_UART_DMA_CHANNELx                 DMA_CHANNEL_4
#define STDIO_UART_USARTx_IRQn                  USART3_IRQn
#define STDIO_UART_USARTx_IRQ_PRIORITY          8ul
#define STDIO_UART_DMAx_STREAMx_IRQn            DMA1_Stream3_IRQn
#define STDIO_UART_DMAx_STREAMx_IRQ_PRIORITY    8ul
#define STDIO_UART_IRQHandler                   USART3_IRQHandler
#define STDIO_UART_DMA_STREAM_IRQHandler        DMA1_Stream3_IRQHandler

#endif /* __STDIO_UART_CONFIG_H__ */

