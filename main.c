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

    // Set up pin
    gpio_setup();

    // Setup SPI peripherals
    SPI_setup();

    // we have single slave, always chip select is high, PA4-->> ss ==1;
    GPIOA->ODR |= GPIO_ODR_ODR4;
    delay(200);
    while (1)
    {

        // making PA4 low to send data
        GPIOA->ODR &= ~GPIO_ODR_ODR4;

        SPI1->DR = 'x';

        while (SPI1->SR & SPI_SR_BSY)
        {
        };
        delay(200);
        // Making PA4 high after sending data
        GPIOA->ODR |= GPIO_ODR_ODR4;

        delay(100);
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

    // PA4 as GP output cnf =10, mode = 11;
    GPIOA->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_MODE4);
    GPIOA->CRL |= GPIO_CRL_MODE4;

    // PA5 as AF output cnf =10, mode = 11;
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);
    GPIOA->CRL |= GPIO_CRL_CNF5_1 | GPIO_CRL_MODE5;

    // PA6 as input cnf =01, mode = 00;
    GPIOA->CRL &= ~(GPIO_CRL_CNF6 | GPIO_CRL_MODE6);
    GPIOA->CRL |= GPIO_CRL_CNF6_0;

    // PA6 as AF output cnf =10, mode = 11;
    GPIOA->CRL &= ~(GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
    GPIOA->CRL |= GPIO_CRL_CNF7_1 | GPIO_CRL_MODE7;
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
    // Config as master device
    SPI1->CR1 |= SPI_CR1_MSTR;

    // Setting baud rate  at lowest one 111
    SPI1->CR1 |= SPI_CR1_BR;

    // Enabling SS output in CR2
    SPI1->CR2 |= SPI_CR2_SSOE;

    // Enabling SPI
    SPI1->CR1 |= SPI_CR1_SPE;
}