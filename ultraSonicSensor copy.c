#include "lpc_types.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pwm.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_systick.h"

#include "string.h"
#include "serialIO.h"
#include "i2cIO.h"
#include "sysTick.h"

//p27, P0.11 as trigger for sensor
define TRIGGER (1<<11)

static uint8_t ECHO_LEVEL = 1; // flag to check state of echo ( )
						//odd for low, even for high .
uint32_t LOW_TO_HIGH_COUNT = 0;		// timer count when echo at low
uint32_t HIGH_TO_LOW_COUNT = 0;		// timer count when echo at high


void Toggle_TRIGGER(){
	GPIO_SetValue(0,TRIGGER);
	delayMS(1);
	GPIO_ClearValue(0,TRIGGER);
}

void TIMER0_IRQHandler(){
	char dataString[20];
	int data, PULSE_WIDTH;
	if (TIM_GetIntCaptureStatus(LPC_TIM0,TIM_MR0_INT)){
		TIM_ClearIntCapturePending(LPC_TIM0,TIM_MR0_INT);
		data = TIM_GetCaptureValue(LPC_TIM0,TIM_COUNTER_INCAP0);

		ECHO_LEVEL += 1;
		if ( ECHO_LEVEL %= 2){
			HIGH_TO_LOW_COUNT = data;
			PULSE_WIDTH = HIGH_TO_LOW - LOW_T0_HIGH;
			sprintf(dataString, "\n\rPulse Width is : %s v\n\r", PULSE_WIDTH);
			serialPrint(dataString);
		}
		else{
			LOW_TO_HIGH_COUNT = data;
		}
		sprintf(dataString, "\n\rTimer Value is : %s v\n\r", data);
		serialPrint(dataString);
		
		TIM_ResetCounter(LPC_TIM0);
		TIM_Cmd(LPC_TIM0, ENABLE);
		
}

void SETUP_ULTRA_SOUND(){
	//SETUP TRIGGER
	//Set dir as output
	GPIO_SetDir(0, TRIGGER, 1);
	//Clear value
	GPIO_ClearValue(0, TRIGGER);
	
	//SETUP PIN P1.26 to capture ECHO from sensor
	PINSEL_CFG_Type PinCfg;
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	TIM_CAPTURECFG_Type TIM_CaptureConfigStruct;
	
	//Config P1.26 as CAP0.0
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	
	PinCfg.Funcnum = 3;
	PinCfg.Portnum = 1;
	PinCfg.Pinnum = 26;
	PINSEL_ConfigPin(&PinCfg);
	
	// Initialize timer 0, prescale count time of 1us 
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue = 1;
	
	// use channel -, CAP0.0
	TIM_CaptureConfigStruct.CaptureChannel = 0;
	// Enable capture on both rising and falling edges
	TIM_CaptureConfigStruct.RisingEdge = ENABLE;
	TIM_CaptureConfigStruct.FallingEdge = ENABLE;
	//Generate capture interrupt 
	TIM_CaptureConfigStruct.IntOnCaption = ENABLE;
	
	//Set configurations
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TIM_ConfigStruct);
	TIM_ConfigCapture(LPC_TIM0, &TIM_CaptureConfigStruct);
	TIM_ResetCounter(LPC_TIM0);
	
	NVIC_EnableIRQ(TIMER0_IRGQn);
	//TIM_Cmd(LPC_TIM0, ENABLE);
	
}



int main(){
	SETUP_ULTRA_SOUND();
	Toggle_TRIGGER();
	TIM_Cmd(LPC_TIM0,ENABLE);
	
	while(1);
	return 0;





}