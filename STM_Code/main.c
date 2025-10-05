#include "main.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void SystemClock_Config(void);
static void MX_GPIO_Init(void);



void setDebug()
{
	RCC->APB2ENR |= (1<<2);
	RCC->APB1ENR |= (1<<0)|(1<<17);
	USART2->BRR = 36000000/9600;
	USART2->CR1 |= (3<<2);
	USART2->CR1 |= (1<<13);
	GPIOA->CRL &= ~(0xFF<<8);
	GPIOA->CRL |= (0x4B<<8);
}

void debugSend(char* message)
{
	for(int i = 0; i < strlen(message); i++)
	{
		while(!(USART2->SR & (1<<7)));
		USART2->DR = message[i];
	}
}

bool analyzeString(char* message)
{
	bool response = 0;
	for(int i = 0; i < strlen(message)-1; i++)
	{
		if(message[i] == 'O' && message[i+1] == 'K') response = 1;
	}
	return response;
}

//ESP01
void wifiSend(char* message)
{
	for(int i = 0; i < strlen(message); i++)
	{
		while(!(USART3->SR & (1<<7)));
		USART3->DR = message[i];
	}
}

void wifisetReceive(char *message, int size)
{
	memset(message, 0, 80);
	uint8_t i = 0;
	char a;
	do
	{
		while(!(USART3->SR & (1<<5)));
        a = USART3->DR;
        message[i++] = a;
        message[i] = '\0';

	}while(i<size);
}

void QtReceive(char* buffer)
{
	char a;
	uint8_t id = 0;
	do
	{
		while(!(USART3->SR & (1<<5)));
		a = USART3->DR;
	}while(a != ':');

	do
	{
		while(!(USART3->SR & (1<<5)));
		a = USART3->DR;
		buffer[id++] = a;
	}while(a != '/');
	buffer[id] = '\r';
	buffer[id+1] = '\n';
	debugSend(buffer);
}

void setWifi()
{
	char mess[80];
	int accepts = 0;
	RCC->APB1ENR |= (1<<0);
	RCC->APB1ENR |= (1<<18);
	USART3->BRR = 36000000/115200;
	USART3->CR1 |= (3<<2);
	USART3->CR1 |= (1<<13);
	GPIOB->CRH &= ~(0xFF<<8);
	GPIOB->CRH |= (0x4B<<8);
	HAL_Delay(1000);
	sprintf(mess,"AT\r\n");
	wifiSend(mess);
	wifisetReceive(mess,strlen(mess)+6);
	debugSend(mess);

	HAL_Delay(1000);
	sprintf(mess,"AT+CWMODE=2\r\n");
	wifiSend(mess);
	wifisetReceive(mess,strlen(mess)+6);
	debugSend(mess);

	HAL_Delay(1000);
	sprintf(mess,"AT+CWSAP=\"PUNTO\",\"Condon67\",5,3\r\n");
	wifiSend(mess);
	wifisetReceive(mess,strlen(mess)+6);
	debugSend(mess);


	HAL_Delay(1000);
	sprintf(mess,"AT+CIPAP=\"192.168.5.7\"\r\n");
	wifiSend(mess);
	wifisetReceive(mess,strlen(mess)+6);
	debugSend(mess);

	HAL_Delay(1000);
	sprintf(mess,"AT+CIPMUX=1\r\n");
	wifiSend(mess);
	wifisetReceive(mess,strlen(mess)+6);
	debugSend(mess);

	HAL_Delay(1000);
	sprintf(mess,"AT+CIPSERVER=1,80\r\n");
	wifiSend(mess);
	wifisetReceive(mess,strlen(mess)+10);
	debugSend(mess);

	HAL_Delay(1000);
	sprintf(mess,"AT+CWDHCP=0,1\r\n");
	wifiSend(mess);
	wifisetReceive(mess,strlen(mess)+6);
	debugSend(mess);


}

void decodeFrame(char* frame, char* angle, int* speed, char* lights, char* movement)
{
	*movement = frame[1];
	*lights = frame[3];

	int tmps = 0;
	char tmpa = 0;

	for(int i = 0; i<3; i++)
	{
		tmps += pow(10,2-i)*(frame[5+i]-48);
		tmpa += pow(10,2-i)*(frame[9+i]-48);
	}
	*angle = tmpa;
	*speed = tmps;
}

void setControl()
{
	//SET STEERING
	RCC->APB2ENR |= (1<<3);
	RCC->APB1ENR |= (1<<1);
	GPIOB->CRL &= ~(0xF<<4);
	GPIOB->CRL |= (0xB<<4);

	//PWM
	TIM3->CR1 |= (1<<0);
	TIM3->CCER |= (1<<12);
	TIM3->PSC = 71;
	TIM3->ARR = 19999;
	TIM3->CCMR2 |= (3<<13);

	//SET ENGINE PWM2
	RCC->APB2ENR |= (1<<2)|(1<<11);
	GPIOA->CRH &= ~(0xF<<0);
	GPIOA->CRH |= (0xB<<0);
	TIM1->CCER |= (1<<0);
	TIM1->PSC = 72;
	TIM1->ARR = 100;
	TIM1->CCMR1 |= (3<<5);
	TIM1->BDTR |= (1 << 15); // MOE = 1
	TIM1->CR1 |= (1<<0);
		//Direction
	GPIOA->CRH &= ~(0xFF<<4);
	GPIOA->CRH |= (0x33<<4);


	//LIGHTS

	GPIOA->CRL &= ~(0xFF<<0);
	GPIOA->CRL |= (0x33<<0);
}

void lightsMode(char mode)
{
	if(mode == '1') {GPIOA->BSRR |= (3<<0);}
	else if(mode == '0'){GPIOA ->BRR |= (3<<0);}
}


void setAngle(char angleValue)
{
	int angle = (angleValue/2)*11.11;
	TIM3->CCR4 = 1000+angle;
}
void setSpeed(char direction, int speedValue)
{

	int speed = speedValue;
	if (direction == '1')
	{
	    GPIOA->BSRR = (1 << 9);      // PA9 = 1
	    GPIOA->BSRR = (1 << (10+16)); // PA10 = 0
	}
	else
	{
	    GPIOA->BSRR = (1 << 10);     // PA10 = 1
	    GPIOA->BSRR = (1 << (9+16)); // PA9 = 0
	}
	speed = speed - 1;
	TIM1->CCR1 = speed;
}

int main(void)
{

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  setDebug();
  setControl();
  debugSend("Init Start\r\n");
  setWifi();
  char frame[50];
  char angle = 0;
  int speed = 0;
  char lights = '0';
  char movement = '0';
  sprintf(frame, "R0L0S000A000/");
  char test[25];

  while (1)
  {
	  QtReceive(frame);
	  decodeFrame(frame,&angle,&speed,&lights,&movement);
	  lightsMode(lights);
	  setAngle(angle);
	  setSpeed(movement,speed);
	  sprintf(test,"\r\n%c %i\r\n",movement,speed);
	  debugSend(test);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

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
#ifdef USE_FULL_ASSERT
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
