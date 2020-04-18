/* bu uygulamada pwm ile gerilim ayari yapmis olduk. 
ledler i = 0 dan 100 e kadar yavas yavas parlakligi artacak ve sonrasinda tam tersi sekilde parlakligi azalacak. */

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

uint32_t delay_counter;

GPIO_InitTypeDef GPIO_InitStruct;
TIM_TimeBaseInitTypeDef TIM_InitStruct;
TIM_OCInitTypeDef TIMOC_InitStruct; // PWM icin

void SysTick_Handler() { // 1ms de bir bu fonk gelecek
	if(delay_counter > 0)
		delay_counter--;
}

void delay_ms(uint32_t time) { // time degerine mesela 10 atadik sistem her 1ms de SysTick_Handler fonk gidecek
	delay_counter = time;
	while(delay_counter);  // delay_counter 0 olana kadar bekle demek
}
void GPIO_Config() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;  // timer olarak kullanildigi icin AF olarak kullandik.
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD,&GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4);
}

void TIM_Config() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
	
 	  /* PWM FREKANSI OLUSTURMAK ICIN ASAGIDAKI FORMULLER KULLANILIR
	   * -> Periyod = (Timer_Tick_Freq / PWM_Freq)-1
	   * -> PWM_Freq = Timer_Tick_Freq / (periyod + 1)
	   * -> Time_Tick_Freq = Timer_CLK /(Prescaler + 1) 
	   *
	   *  //pulse oranini hesaplarken
	   *  pulse = (periyod + 1) * (duty cycl %) - 1

	/* PWM_FREQ = 10KHz( 10 000 mhz) olmasini istedik
	 *
	 * Time_Tick_Freq = 84000000 / (Prescaler + 1) = 1000000 , Prescaler = 83 olarak bulunur
	 *
	 * Periyod = (1000000 / 10 000) - 1 = 99
	 **/

	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = 99;
	TIM_InitStruct.TIM_Prescaler = 83;
	TIM_InitStruct.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM4,&TIM_InitStruct);
	TIM_Cmd(TIM4,ENABLE);

	TIMOC_InitStruct.TIM_OCMode = TIM_OCMode_PWM1;	// PWM mod1 da duty cycle 25 ise %25 doluluk alinir
							// PWM mod2 dE duty cycle 25 ise %75 doluluk alinir.aralarindaki fark budur.
	TIMOC_InitStruct.TIM_OutputState = ENABLE; 	// cikisi aktif ettik
	TIMOC_InitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;

	SysTick_Config(SystemCoreClock/1000); // 1ms de bir kesmeye girer.
					      // LEDLERIN yanma hizini ayarlamak istiyorsak burandan ayarlama yapmaliyiz
	/* delay fonk yazmak yerine SysTick_Config in kendine has delay fonk kullandik */
}

int main(void) {
	GPIO_Config();
	TIM_Config();
  while (1)
  {
	  for(int i = 0; i <= 100; i++) {
		  TIMOC_InitStruct.TIM_Pulse = i;
		  TIM_OC1Init(TIM4,&TIMOC_InitStruct);
		  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);//TIM4 -> CH1 ENABLE
		  delay_ms(20);

		  TIMOC_InitStruct.TIM_Pulse = i;
		  TIM_OC2Init(TIM4,&TIMOC_InitStruct);
		  TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);//TIM4 -> CH2 ENABLE
		  delay_ms(20);

		  TIMOC_InitStruct.TIM_Pulse = i;
		  TIM_OC3Init(TIM4,&TIMOC_InitStruct);
		  TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);//TIM4 -> CH3 ENABLE
		  delay_ms(20);

		  TIMOC_InitStruct.TIM_Pulse = i;
		  TIM_OC4Init(TIM4,&TIMOC_InitStruct);
		  TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);//TIM4 -> CH4 ENABLE
		  delay_ms(20);
	  }
	  for(int i = 100; i >= 0; i--) {
		  TIMOC_InitStruct.TIM_Pulse = i;
		  TIM_OC1Init(TIM4,&TIMOC_InitStruct);
		  TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);//TIM4 -> CH1 ENABLE
		  delay_ms(20);

		  TIMOC_InitStruct.TIM_Pulse = i;
		  TIM_OC2Init(TIM4,&TIMOC_InitStruct);
		  TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);//TIM4 -> CH2 ENABLE
		  delay_ms(20);

		  TIMOC_InitStruct.TIM_Pulse = i;
		  TIM_OC3Init(TIM4,&TIMOC_InitStruct);
		  TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);//TIM4 -> CH3 ENABLE
		  delay_ms(20);

		  TIMOC_InitStruct.TIM_Pulse = i;
		  TIM_OC4Init(TIM4,&TIMOC_InitStruct);
		  TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);//TIM4 -> CH4 ENABLE
		  delay_ms(20);
	  }
  }
}

/*
 * periyod = (Timer_Tick_Freq / PWM_Freq)-1
 *
 * Time_Tick_Freq = Timer_CLK /(Prescaler + 1)
 *
 * PWM_Freq = Timer_Tick_Freq / (periyod + 1)
 *
 *  //pulse oranýný hesaplarken
 *
 *  pulse = (periyod + 1) * (duty cycl %) - 1
 *
 *        = (9999 + 1) * 100/100 -1 = 9999
 */


void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
