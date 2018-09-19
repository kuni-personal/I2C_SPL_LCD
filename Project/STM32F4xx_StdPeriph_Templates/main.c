/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

uint8_t EV_Error_Count[10] = {0};

uint8_t DebugFlag = 2;

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);
void I2C_single_write(I2C_TypeDef* I2Cx, uint8_t HW_address, uint8_t addr, uint8_t data);
uint8_t I2C_single_read(I2C_TypeDef* I2Cx, uint8_t HW_address, uint8_t addr);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef I2C_InitStructure;
 
 /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files before to branch to application main.
       To reconfigure the default setting of SystemInit() function, 
       refer to system_stm32f4xx.c file */

  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
  
  /* Add your application code here */
  /* Insert 50 ms delay */
  Delay(50);
  
  /* Output HSE clock on MCO1 pin(PA8) ****************************************/ 
  /* Enable the GPIOA peripheral */ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  /* Configure MCO1 pin(PA8) in alternate function */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
  /* HSE clock selected to output on MCO1 pin(PA8)*/
  RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);
  
  
  /* Output SYSCLK/4 clock on MCO2 pin(PC9) ***********************************/ 
  /* Enable the GPIOACperipheral */ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
  /* Configure MCO2 pin(PC9) in alternate function */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_Init(GPIOC, &GPIO_InitStructure);
    
  /* SYSCLK/4 clock selected to output on MCO2 pin(PC9)*/
  RCC_MCO2Config(RCC_MCO2Source_SYSCLK, RCC_MCO2Div_4);
  
  /* LD2 初期設定 ***************************************/
  GPIO_InitStructure.GPIO_Pin = LD2_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_Init(LD2_GPIO_Port, &GPIO_InitStructure);
  
  /* I2C 初期設定 ****************************************/

//  //I2C初期化
//  I2C1->CR2 = 0x0000002D;
//  I2C1->TRISE = 0x0000000E;
//  I2C1->CCR = 0x00008025;
//  I2C1->CR1 |= 0x00000001;
//  I2C1->OAR1 = 0x00004000;

  //ペリフェラルクロック    
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC->APB1ENR |= 0x00200000;           //I2C1 Enable
  RCC->AHB1ENR |= 0x00000002;           //GPIOB Enable
  
  //GPIO設定
  //PB6 -> SCL
  //PB9 -> SDA
//  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
//  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
  GPIOB->AFR[0] |= 0x04000000;          //AFRL6_0x04
  GPIOB->AFR[1] |= 0x00000040;           //AFRL9_0x04
  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
//  GPIOB->AFR[0] |= 0x04000000;          //AFRL6_0x04
//  GPIOB->AFR[1] |= 0x00000040;           //AFRL9_0x04
//  GPIOB->MODER |= 0x00082280;           //MODER3,MODER4,MODER6_0x02(AF-Mode)
//  GPIOB->OSPEEDR |= 0x000C3000;         //OSPEEDR6, OSPEEDR9 _0x03(High_Speed)
//  GPIOB->OTYPER |= 0x00000240;          //OT6, OT9
  
  
  //I2C 初期化
  I2C_InitStructure.I2C_ClockSpeed = 16800;
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_Init(I2C1, &I2C_InitStructure);
  
  I2C_Cmd(I2C1, ENABLE);

//  I2C1->CR2 = 0x0000002D;
//  I2C1->TRISE = 0x0000000E;
//  I2C1->CCR = 0x00008025;
//  I2C1->CR1 |= 0x00000001;
//  I2C1->OAR1 = 0x00004000;
  
  //LCD 初期設定
  I2C_single_write(I2C1, 0xA0, 0x00, 0x01);
  Delay(10);
  I2C_single_write(I2C1, 0xA0, 0x00, 0x38);
  Delay(10);
  I2C_single_write(I2C1, 0xA0, 0x00, 0x0f);
  Delay(10);
  I2C_single_write(I2C1, 0xA0, 0x00, 0x06);
  Delay(10);
  
  //初期設定の完了確認
  GPIO_ToggleBits(LD2_GPIO_Port, LD2_Pin);
  Delay(10);
  GPIO_ToggleBits(LD2_GPIO_Port, LD2_Pin);
  
     
  /* Infinite loop */
  while (1)
  {
    I2C_single_write(I2C1, 0xA0, 0x80, 0x41);
    Delay(100);
  }
}


//I2C通信用

void I2C_single_write(I2C_TypeDef* I2Cx, uint8_t HW_address, uint8_t addr, uint8_t data)
{

  
  I2C_GenerateSTART(I2Cx, ENABLE);
      while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
      {
        EV_Error_Count[0]++;
      }
  I2C_Send7bitAddress(I2Cx, HW_address, I2C_Direction_Transmitter);
      while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
      {
        EV_Error_Count[1]++;
      }
  I2C_SendData(I2Cx, addr);
      while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
      {
        EV_Error_Count[2]++;
      }
  I2C_SendData(I2Cx, data);
      while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
      {
        EV_Error_Count[3]++;
      }
  I2C_GenerateSTOP(I2Cx, ENABLE);
      while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
      {
        EV_Error_Count[4]++;
      }
          
   
}


uint8_t I2C_single_read(I2C_TypeDef* I2Cx, uint8_t HW_address, uint8_t addr)
{
	uint8_t data;
		while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2Cx, ENABLE);
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2Cx, HW_address, I2C_Direction_Transmitter);
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2Cx, addr);
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTART(I2Cx, ENABLE);
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2Cx, HW_address, I2C_Direction_Receiver);
		while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_RECEIVED));
	data = I2C_ReceiveData(I2Cx);
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	I2C_GenerateSTOP(I2Cx, ENABLE);
		while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	return data;
}


/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
