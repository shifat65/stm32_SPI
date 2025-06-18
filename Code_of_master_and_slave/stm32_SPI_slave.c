//*******************************************************slave********************************************************
//*******************************************************slave********************************************************
//*******************************************************slave********************************************************
//*******************************************************slave********************************************************

#include <stm32f10x.h>

/*
Actual setup

SPI - 1
-------
PA4 --> SS
PA5 --> SCLK
PA6 --> MISO
PA7 --> MOSI

*/
uint32_t tik = 0;
void En_clock(void);
void gpio_setup(void);
void delay_ms(void);
void delay(uint32_t count);
void systick_config(void);

void SPI_setup(void);

int main(void)
{
    // Activate SPI1 Peripheral / AFIO Function
    En_clock();
    // config systic timer
    systick_config();
    // Set up pin
    gpio_setup();
    // Setup SPI peripherals
    SPI_setup();

    GPIOA->ODR |= GPIO_ODR_ODR2;
    delay(2000);
    GPIOA->ODR &= ~GPIO_ODR_ODR2;

    uint32_t msg = 0;
    while (1)
    {
        if (SPI1->SR & SPI_SR_RXNE)
        {
            msg = SPI1->DR;
            // SPI1->SR &= ~SPI_SR_RXNE;
            GPIOA->ODR |= GPIO_ODR_ODR2;
            delay(50);
            GPIOA->ODR &= ~GPIO_ODR_ODR2;
        }
        if (msg == 'x')
        {
            GPIOA->ODR ^= GPIO_ODR_ODR3;
        }
				msg = 0;
        //    delay(500);
        //  GPIOA->ODR &= ~GPIO_ODR_ODR3;
    }

    return 0;
}

void En_clock(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPAEN;
}

void gpio_setup(void)
{
    // PA0 as push button
    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
    GPIOA->CRL |= GPIO_CRL_CNF0_0;

    // PA1 as analog in put cnf=00, mode =00;
    GPIOA->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1);
    GPIOA->CRL |= 0UL;

    // PA2 as push-pull output cnf =00, mode = 11;
    GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
    GPIOA->CRL |= GPIO_CRL_MODE2;

    // PA3 as push-pull output cnf =00, mode = 11;
    GPIOA->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
    GPIOA->CRL |= GPIO_CRL_MODE3;

    // PA4 as input float cnf =01, mode = 00;
    GPIOA->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_MODE4);
    GPIOA->CRL |= GPIO_CRL_CNF4_0;

    // PA5 as input float cnf =01, mode = 00;
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);
    GPIOA->CRL |= GPIO_CRL_CNF5_0;

    // PA6 as output AF cnf =10, mode = 11;
    GPIOA->CRL &= ~(GPIO_CRL_CNF6 | GPIO_CRL_MODE6);
    GPIOA->CRL |= GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6;

    // PA7 as input cnf =01, mode = 00;
    GPIOA->CRL &= ~(GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
    GPIOA->CRL |= GPIO_CRL_CNF7_0;
}

void systick_config(void)
{
    SysTick->LOAD = 72000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE | SysTick_CTRL_ENABLE;
}

void delay_ms(void)
{
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG))
        ;
}

void delay(uint32_t count)
{
    while (count--)
    {
        delay_ms();
    }
}

void SPI_setup(void)
{
    // Config as slave device
    SPI1->CR1 &= ~SPI_CR1_MSTR;

    // Setting baud rate  at lowest one 111
    // SPI1->CR1 |= SPI_CR1_BR;

    // Enabling SS output in CR2. Hardware will pulls down NSS (PA4).
    // And Transmition willbe strted autometic as soon as there is a load of data in SPI->DR
    SPI1->CR2 |= SPI_CR2_SSOE;

    // Enabling SPI
    SPI1->CR1 |= SPI_CR1_SPE;
}