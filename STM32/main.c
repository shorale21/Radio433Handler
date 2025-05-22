/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "USARTHandler.h"
#include "RF_HANDLER.h"
#include "extihandle.h"
#include "soilsensor.h"


void SystemClock_Config(void);

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();



  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  UART_init(); //initialize USART communication

  init_output_port(); // initialize output port as PC0 (data line)
  //init_input_port(); // initialize receiving port as PC1 (input line)

  initialize(); //initialize port PC2 as receiving port

  UART_ESC("[2J", 3);
  UART_ESC("8", 1);
  UART_print("starting program",16);

  start_clock(800, 50);


  uint32_t last_sent = 0;
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  char c = UART_read();
	  if(c != NOCHAR)
	  {
		  if(c == 's')
		  {
			  last_sent = STAT_REQ;
			  send_message(STAT_REQ);
		  }
		  else if(c == 'd')
		  {
			  last_sent = DATA_REQ;
			  send_message(DATA_REQ);
		  }
	  }
	  if(check_rx() == 1) //if there is a message waiting
	  {
		  uint32_t message = get_message();
		  // interpret message
		  uint8_t check = verify_packet(message);
		  uint8_t message_type = (uint8_t)((message & TYPE_MSK) >> TYPE_POS);
		  uint8_t dev_ID = (uint8_t)((message & ID_MSK) >> ID_POS);
		  if(verify_packet == 1)
		  {
			  if(message_type == TYPE_INIT)
			  {
				  UART_ESC("[8B", 3); //move down 1 line
				  UART_print("Received INIT packet from ID:",29);
				  UART_printNum(dev_ID);
			  }
			  else if(message_type == TYPE_STAT)
			  {
				  UART_ESC("[8B", 3); //move down 1 line
				  UART_print("Received STATUS packet from ID:",31);
				  UART_printNum(dev_ID);
			  }
			  else if(message_type == TYPE_DATA)
			  {
				  UART_ESC("[8B", 3); //move down 1 line
				  UART_print("Received DATA packet from ID:",29);
				  UART_printNum(dev_ID);
			  }
			  else if(message_type == TYPE_ERR)
			  {
				  UART_ESC("[8B", 3); //move down 1 line
				  UART_print("Received ERROR packet from ID:",30);
				  UART_printNum(dev_ID);
			  }
			  else
			  {
				  UART_ESC("[8B", 3); //move down 1 line
				  UART_print("Received UNKNOWN packet from ID:",32);
				  UART_printNum(dev_ID);
			  }
		  }
		  else //packet did not pass verification
		  {
			  UART_ESC("[8B",3);
			  UART_print("Invalid Packet Recieved. Resending last message.", 48);
		  }
	  }
  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_9;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
