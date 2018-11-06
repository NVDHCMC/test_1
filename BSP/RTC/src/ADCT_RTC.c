/*
 * ADCT_BGS_RTC.c
 *
 *  Created on: Oct 17, 2016
 *      Author: Administrator
 */

#include "ADCT_RTC.h"
#include <stdint.h>
#include <stdio.h>
/* Private variables--------------------------------------------------------- */
uint8_t ClockSource;

/*Structure variable declaration for system time, system date,
alarm time, alarm date */
struct Time_s s_TimeStructVar;
struct Date_s s_DateStructVar;
uint16_t SummerTimeCorrect;

uint32_t bkupdata  =10;
/** @addtogroup RTC
 * @{
 */

/**
 * @brief  COnfiguration of RTC Registers, Selection and Enabling of
 *   RTC clock
 * @param  None
 * @retval : None
 */
void RTC_Configuration()
{
	uint16_t WaitForOscSource;

	/*Allow access to Backup Registers*/
	PWR_BackupAccessCmd(ENABLE);

	//if(BKP_ReadBackupRegister(BKP_DR1)==CONFIGURATION_DONE)
	bkupdata = BKP_ReadBackupRegister(BKP_DR1);
	if(bkupdata==CONFIGURATION_RESET)
	{
		/*Enables the clock to Backup and power interface peripherals    */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR,ENABLE);
		/* Backup Domain Reset */
		BKP_DeInit();

		s_DateStructVar.Month=DEFAULT_MONTH ;
		s_DateStructVar.Day=DEFAULT_DAY;
		s_DateStructVar.Year=DEFAULT_YEAR;
		SummerTimeCorrect = OCTOBER_FLAG_SET;
		BKP_WriteBackupRegister(BKP_DR7,SummerTimeCorrect);
		BKP_WriteBackupRegister(BKP_DR2,s_DateStructVar.Month);
		BKP_WriteBackupRegister(BKP_DR3,s_DateStructVar.Day);
		BKP_WriteBackupRegister(BKP_DR4,s_DateStructVar.Year);
		BKP_WriteBackupRegister(BKP_DR1, CONFIGURATION_DONE);
		/*Enable 32.768 kHz external oscillator */
		RCC_LSEConfig(RCC_LSE_ON);

		for(WaitForOscSource=0;WaitForOscSource<5000;WaitForOscSource++)
		{
		}
		while ((RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET))
		        // Wait till LSE is ready
		       ;
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		/* RTC Enabled */
		RCC_RTCCLKCmd(ENABLE);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		/* Setting RTC Interrupts-Seconds interrupt enabled */
		/* Enable the RTC Second */
		RTC_ITConfig(RTC_IT_SEC , ENABLE);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		BKP_WriteBackupRegister(BKP_DR6, 1);

		/* Set RTC prescaler: set RTC period to 1 sec */
		RTC_SetPrescaler(32765); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
		/* Prescaler is set to 32766 instead of 32768 to compensate for
        lower as well as higher frequencies*/
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		/* Set default system time to 09 : 24 : 00 */
		SetTime(DEFAULT_HOURS,DEFAULT_MINUTES,DEFAULT_SECONDS);
		BKP_WriteBackupRegister(BKP_DR1, CONFIGURATION_DONE);
	}
	else
	{
		/* PWR and BKP clocks selection */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
		for(WaitForOscSource=0;WaitForOscSource<5000;WaitForOscSource++);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		/* Enable the RTC Second */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		RTC_WaitForLastTask();
	}

	/* Check if how many days are elapsed in power down/Low Power Mode-
   Updates Date that many Times*/
	CheckForDaysElapsed();
	ClockSource = BKP_ReadBackupRegister(BKP_DR6);
	s_DateStructVar.Month = BKP_ReadBackupRegister(BKP_DR2);
	s_DateStructVar.Day = BKP_ReadBackupRegister(BKP_DR3);
	s_DateStructVar.Year = BKP_ReadBackupRegister(BKP_DR4);
	SummerTimeCorrect = BKP_ReadBackupRegister(BKP_DR7);
}



/**
 * @brief  Summer Time Correction routine
 * @param  None
 * @retval : None
 */
void SummerTimeCorrection(void)
{
	uint8_t CorrectionPending=0;
	uint8_t CheckCorrect=0;

	if((SummerTimeCorrect & OCTOBER_FLAG_SET)!=0)
	{
		if((s_DateStructVar.Month==10) && (s_DateStructVar.Day >24 ))
		{
			for(CheckCorrect = 25;CheckCorrect <=s_DateStructVar.Day;CheckCorrect++)
			{
				if(WeekDay(s_DateStructVar.Year,s_DateStructVar.Month,CheckCorrect )==0)
				{
					if(CheckCorrect == s_DateStructVar.Day)
					{
						/* Check if Time is greater than equal to 1:59:59 */
						if(RTC_GetCounter()>=7199)
						{
							CorrectionPending=1;
						}
					}
					else
					{
						CorrectionPending=1;
					}
					break;
				}
			}
		}
		else if((s_DateStructVar.Month > 10))
		{
			CorrectionPending=1;
		}
		else if(s_DateStructVar.Month < 3)
		{
			CorrectionPending=1;
		}
		else if(s_DateStructVar.Month == 3)
		{
			if(s_DateStructVar.Day<24)
			{
				CorrectionPending=1;
			}
			else
			{
				for(CheckCorrect=24;CheckCorrect<=s_DateStructVar.Day;CheckCorrect++)
				{
					if(WeekDay(s_DateStructVar.Year,s_DateStructVar.Month,CheckCorrect)==0)
					{
						if(CheckCorrect == s_DateStructVar.Day)
						{
							/*Check if Time is less than 1:59:59 and year is not the same in which
                March correction was done */
							if((RTC_GetCounter() < 7199) && ((SummerTimeCorrect & 0x3FFF) != \
									s_DateStructVar.Year))
							{
								CorrectionPending=1;
							}
							else
							{
								CorrectionPending=0;
							}
							break;
						}
						else
						{
							CorrectionPending=1;
						}
					}
				}
			}
		}
	}
	else if((SummerTimeCorrect & MARCH_FLAG_SET)!=0)
	{
		if((s_DateStructVar.Month == 3) && (s_DateStructVar.Day >24 ))
		{
			for(CheckCorrect = 25;CheckCorrect <=s_DateStructVar.Day;\
			CheckCorrect++)
			{
				if(WeekDay(s_DateStructVar.Year,s_DateStructVar.Month,\
						CheckCorrect )==0)
				{
					if(CheckCorrect == s_DateStructVar.Day)
					{
						/*Check if time is greater than equal to 1:59:59 */
						if(RTC_GetCounter()>=7199)
						{
							CorrectionPending=1;
						}
					}
					else
					{
						CorrectionPending=1;
					}
					break;
				}
			}
		}
		else if((s_DateStructVar.Month > 3) && (s_DateStructVar.Month < 10 ))
		{
			CorrectionPending=1;
		}
		else if(s_DateStructVar.Month ==10)
		{
			if(s_DateStructVar.Day<24)
			{
				CorrectionPending=1;
			}
			else
			{
				for(CheckCorrect=24;CheckCorrect<=s_DateStructVar.Day;\
				CheckCorrect++)
				{
					if(WeekDay(s_DateStructVar.Year,s_DateStructVar.Month,\
							CheckCorrect)==0)
					{
						if(CheckCorrect == s_DateStructVar.Day)
						{
							/*Check if Time is less than 1:59:59 and year is not the same in
              which March correction was done */
							if((RTC_GetCounter() < 7199) && \
									((SummerTimeCorrect & 0x3FFF) != s_DateStructVar.Year))
							{
								CorrectionPending=1;
							}
							else
							{
								CorrectionPending=0;
							}
							break;
						}
					}
				}
			}
		}
	}

	if(CorrectionPending==1)
	{
		if((SummerTimeCorrect & OCTOBER_FLAG_SET)!=0)
		{
			/* Subtract 1 hour from the current time */
			RTC_SetCounter(RTC_GetCounter() - 3599);
			/* Reset October correction flag */
			SummerTimeCorrect &= 0xBFFF;
			/* Set March correction flag  */
			SummerTimeCorrect |= MARCH_FLAG_SET;
			SummerTimeCorrect |= s_DateStructVar.Year;
			BKP_WriteBackupRegister(BKP_DR7,SummerTimeCorrect);
		}
		else if((SummerTimeCorrect & MARCH_FLAG_SET)!=0)
		{
			/* Add 1 hour to current time */
			RTC_SetCounter(RTC_GetCounter() + 3601);
			/* Reset March correction flag */
			SummerTimeCorrect &= 0x7FFF;
			/* Set October correction flag  */
			SummerTimeCorrect |= OCTOBER_FLAG_SET;
			SummerTimeCorrect |= s_DateStructVar.Year;
			BKP_WriteBackupRegister(BKP_DR7,SummerTimeCorrect);
		}
	}
}

void RTC_RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;

	/* RCC system reset(for debug purpose) */
	//RCC_DeInit();

	/* Enable HSE */
	//RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if(HSEStartUpStatus == SUCCESS)
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1);

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);
		//RCC_PCLK1Config(RCC_HCLK_Div1);
		/* PLLCLK = 8MHz * 9 = 72 MHz */
		/* PLLCLK = 12MHz * 6 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		//RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);
		/* Enable PLL */
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}

}

/**
 * @brief  RTC Initialisation Routine
 * @param  None
 * @retval : None
 */
void ADCT_BGS_RTC_Init(void)
{
	RTC_RCC_Configuration();
	/*Enables the clock to Backup and power interface peripherals    */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR,ENABLE);// Enable PWR/BKP
	/* Unlock the Flash Program Erase controller */
	FLASH_Unlock();
	RTC_NVIC_Configuration();
	/* RTC Configuration*/
	RTC_Configuration();
	/*RTC_NVIC Configuration */

	BKP_RTCOutputConfig(BKP_RTCOutputSource_None);
	/* Configure PVD Supervisor to disable the Tamper Interrupt when voltage drops
  below 2.5 volts*/
	PWR_PVDCmd(ENABLE);
	PWR_PVDLevelConfig(PWR_PVDLevel_2V5);
	PWR_BackupAccessCmd(ENABLE);

}

/**
 * @brief  Sets the RTC Current Counter Value
 * @param Hour, Minute and Seconds data
 * @retval : None
 */
void SetTime(uint8_t Hour,uint8_t Minute,uint8_t Seconds)
{
	uint32_t CounterValue;

	CounterValue=((Hour * 3600)+ (Minute * 60)+Seconds);

	RTC_WaitForLastTask();
	RTC_SetCounter(CounterValue);
	RTC_WaitForLastTask();
}


/**
 * @brief sets the RTC current counter value
 */

void ADCT_BGS_SetTime(ADCT_RTC_Types *ptr)
{

	uint32_t CounterValue;
	CounterValue = (((ptr->ADCT_BGS_RTC_Hour)*3600) +(ptr->ADCT_BGS_RTC_Min)*60 + ptr->ADCT_BGS_RTC_Sec);
	RTC_WaitForLastTask();
	RTC_SetCounter(CounterValue);
	RTC_WaitForLastTask();
}
/**
 * @brief  Sets the RTC Date(DD/MM/YYYY)
 * @param DD,MM,YYYY
 * @retval : None
 */
void SetDate(uint8_t Day, uint8_t Month, uint16_t Year)
{
	uint32_t DateTimer;

	/*Check if the date entered by the user is correct or not, Displays an error
    message if date is incorrect  */
	if((( Month==4 || Month==6 || Month==9 || Month==11) && Day ==31) \
			|| (Month==2 && Day==31)|| (Month==2 && Day==30)|| \
			(Month==2 && Day==29 && (CheckLeap(Year)==0)))
	{
		DateTimer=RTC_GetCounter();

		while((RTC_GetCounter()-DateTimer)<2)
		{
		}
	}
	/* if date entered is correct then set the date*/
	else
	{
			BKP_WriteBackupRegister(BKP_DR2,Month);
			BKP_WriteBackupRegister(BKP_DR3,Day);
			BKP_WriteBackupRegister(BKP_DR4,Year);
	}
}

void ADCT_BGS_Setting_Date_Time(ADCT_BGS_RTC_Types  *ptr)
{
	SetTime(ptr->RTC_Hour, ptr->RTC_Min, ptr->RTC_Sec);
	SetDate(ptr->RTC_Mday, ptr->RTC_Mon, ptr->RTC_Year);
}
void ADCT_BGS_Run_RTC(ADCT_BGS_RTC_Types  *ptr)
{
	uint32_t TimeVar;
	TimeVar=RTC_GetCounter();
	TimeVar=TimeVar % 86400;
	ptr->RTC_Sec = (uint8_t)((TimeVar %3600)%60);
	ptr->RTC_Min = (uint8_t)((TimeVar%3600)/60);
	ptr->RTC_Hour = (uint8_t)(TimeVar/3600);
	ptr->RTC_Year =(BKP_ReadBackupRegister(BKP_DR4));
	ptr->RTC_Mon = (BKP_ReadBackupRegister(BKP_DR2));
	ptr->RTC_Mday = (BKP_ReadBackupRegister(BKP_DR3));
}


/**
 * @brief Updates the Date (This function is called when 1 Day has elapsed
 * @param None
 * @retval :None
 */
void DateUpdate(void)
{
	s_DateStructVar.Month=BKP_ReadBackupRegister(BKP_DR2);
	s_DateStructVar.Year=BKP_ReadBackupRegister(BKP_DR4);
	s_DateStructVar.Day=BKP_ReadBackupRegister(BKP_DR3);

	if(s_DateStructVar.Month == 1 || s_DateStructVar.Month == 3 || \
			s_DateStructVar.Month == 5 || s_DateStructVar.Month == 7 ||\
			s_DateStructVar.Month == 8 || s_DateStructVar.Month == 10 \
			|| s_DateStructVar.Month == 12)
	{
		if(s_DateStructVar.Day < 31)
		{
			s_DateStructVar.Day++;
		}
		/* Date structure member: s_DateStructVar.Day = 31 */
		else
		{
			if(s_DateStructVar.Month != 12)
			{
				s_DateStructVar.Month++;
				s_DateStructVar.Day = 1;
			}
			/* Date structure member: s_DateStructVar.Day = 31 & s_DateStructVar.Month =12 */
			else
			{
				s_DateStructVar.Month = 1;
				s_DateStructVar.Day = 1;
				s_DateStructVar.Year++;
			}
		}
	}
	else if(s_DateStructVar.Month == 4 || s_DateStructVar.Month == 6 \
			|| s_DateStructVar.Month == 9 ||s_DateStructVar.Month == 11)
	{
		if(s_DateStructVar.Day < 30)
		{
			s_DateStructVar.Day++;
		}
		/* Date structure member: s_DateStructVar.Day = 30 */
		else
		{
			s_DateStructVar.Month++;
			s_DateStructVar.Day = 1;
		}
	}
	else if(s_DateStructVar.Month == 2)
	{
		if(s_DateStructVar.Day < 28)
		{
			s_DateStructVar.Day++;
		}
		else if(s_DateStructVar.Day == 28)
		{
			/* Leap Year Correction */
			if(CheckLeap(s_DateStructVar.Year))
			{
				s_DateStructVar.Day++;
			}
			else
			{
				s_DateStructVar.Month++;
				s_DateStructVar.Day = 1;
			}
		}
		else if(s_DateStructVar.Day == 29)
		{
			s_DateStructVar.Month++;
			s_DateStructVar.Day = 1;
		}
	}

	BKP_WriteBackupRegister(BKP_DR2,s_DateStructVar.Month);
	BKP_WriteBackupRegister(BKP_DR3,s_DateStructVar.Day);
	BKP_WriteBackupRegister(BKP_DR4,s_DateStructVar.Year);
}



/**
 * @brief  Checks whether the passed year is Leap or not.
 * @param  None
 * @retval : 1: leap year
 *   0: not leap year
 */
uint8_t CheckLeap(uint16_t Year)
{
	if((Year%400)==0)
	{
		return LEAP;
	}
	else if((Year%100)==0)
	{
		return NOT_LEAP;
	}
	else if((Year%4)==0)
	{
		return LEAP;
	}
	else
	{
		return NOT_LEAP;
	}
}



/**
 * @brief Determines the weekday
 * @param Year,Month and Day
 * @retval :Returns the CurrentWeekDay Number 0- Sunday 6- Saturday
 */
uint16_t WeekDay(uint16_t CurrentYear,uint8_t CurrentMonth,uint8_t CurrentDay)
{
	uint16_t Temp1,Temp2,Temp3,Temp4,CurrentWeekDay;

	if(CurrentMonth < 3)
	{
		CurrentMonth=CurrentMonth + 12;
		CurrentYear=CurrentYear-1;
	}

	Temp1=(6*(CurrentMonth + 1))/10;
	Temp2=CurrentYear/4;
	Temp3=CurrentYear/100;
	Temp4=CurrentYear/400;
	CurrentWeekDay=CurrentDay + (2 * CurrentMonth) + Temp1 \
			+ CurrentYear + Temp2 - Temp3 + Temp4 +1;
	CurrentWeekDay = CurrentWeekDay % 7;

	return(CurrentWeekDay);
}




/**
 * @brief Displays the Time on LCD
 * @param None
 * @retval :None
 */
void DisplayTime(void)
{


	s_DateStructVar.Year=(BKP_ReadBackupRegister(BKP_DR4));
	s_DateStructVar.Month=(BKP_ReadBackupRegister(BKP_DR2));
	s_DateStructVar.Day=(BKP_ReadBackupRegister(BKP_DR3));

}


/**
 * @brief Chaeks is counter value is more than 86399 and the number of
 *   elapsed and updates date that many times
 * @param None
 * @retval :None
 */
void CheckForDaysElapsed(void)
{
	uint8_t DaysElapsed;

	if((RTC_GetCounter() / SECONDS_IN_DAY) != 0)
	{
		for(DaysElapsed = 0; DaysElapsed < (RTC_GetCounter() / SECONDS_IN_DAY) ;DaysElapsed++)
		{
			DateUpdate();
		}

		RTC_SetCounter(RTC_GetCounter() % SECONDS_IN_DAY);
	}
}






/**
 * @brief  Configures RTC Interrupts
 * @param  None
 * @retval : None
 */
void RTC_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure one bit for preemption priority */
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/**
 * @brief  This function handles RTC_IRQHandler .
 * @param  None
 * @retval : None
 */
void RTC_IRQHandler(void)
{
	uint8_t Month,Day;
	uint16_t Year;

	Month = BKP_ReadBackupRegister(BKP_DR2);
	Day = BKP_ReadBackupRegister(BKP_DR3);
	Year = BKP_ReadBackupRegister(BKP_DR4);
	if(RTC_GetITStatus(RTC_IT_SEC) !=RESET )
	{
		NVIC_ClearPendingIRQ(RTC_IRQn);
		RTC_ClearITPendingBit(RTC_IT_SEC);
	}

	/* If counter is equal to 86399: one day was elapsed */
	/* This takes care of date change and resetting of counter in case of
  power on - Run mode/ Main supply switched on condition*/
	if(RTC_GetCounter() == 86399)
	{
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		/* Reset counter value */
		RTC_SetCounter(0x0);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		/* Increment the date */
		DateUpdate();
	}

	if((RTC_GetCounter()/3600 == 1)&&(((RTC_GetCounter()%3600)/60) == 59)&&
			(((RTC_GetCounter()%3600)%60) == 59))
	{
		/* March Correction */
		if((Month == 3) && (Day >24))
		{
			if(WeekDay(Year,Month,Day)==0)
			{
				if((SummerTimeCorrect & 0x8000) == 0x8000)
				{
					RTC_SetCounter(RTC_GetCounter() + 3601);

					/* Reset March correction flag */
					SummerTimeCorrect &= 0x7FFF;

					/* Set October correction flag  */
					SummerTimeCorrect |= 0x4000;
					SummerTimeCorrect |= Year;
					BKP_WriteBackupRegister(BKP_DR7,SummerTimeCorrect);
				}
			}
		}
		/* October Correction */
		if((Month == 10) && (Day >24))
		{
			if(WeekDay(Year,Month,Day)==0)
			{
				if((SummerTimeCorrect & 0x4000) == 0x4000)
				{
					RTC_SetCounter(RTC_GetCounter() - 3599);

					/* Reset October correction flag */
					SummerTimeCorrect &= 0xBFFF;

					/* Set March correction flag  */
					SummerTimeCorrect |= 0x8000;
					SummerTimeCorrect |= Year;
					BKP_WriteBackupRegister(BKP_DR7,SummerTimeCorrect);
				}
			}
		}
	}
}
/**
 * @}
 */
