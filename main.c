#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/spi.h"
#include "nrf24l01.h"

#define Mhz 100000 

void gpio_toggle(uint pin);

int main(){
    //Led pin
    const uint led_pin = 25;
    //Init led
    gpio_init(led_pin);
    gpio_set_dir(led_pin,GPIO_OUT);

    //Spi pins
    const uint ce_spi = 20;
    const uint csn_spi = 17;
    const uint sck_spi = 18;
    const uint mosi_spi = 19;
    const uint miso_spi = 16;

    //ADC Input pin
    const uint adc_pin = 26;
    const uint adc_channel = 0;
    //ADC reading variable
    uint16_t adc_result = 0;
    //ADC conversion constant declaration
    const float conversion_factor = 3.3f / (1 << 12);

    //NRF24 variables declaration
    nrf24 nrfTx;
    uint8_t txAddr[] = { 0xEA, 0xDD, 0xCC, 0xBB, 0xAA };
    uint8_t txData[2] = {0,0};

    //Spi port
    spi_inst_t *spi = spi0;

    //NRF24 structure
    nrfTx.hSPIx = spi;
    nrfTx.CE_pin = ce_spi;
    nrfTx.CSN_pin = csn_spi;


    //Initialize chose serial port
    stdio_init_all();

    //Init ADC
    adc_init();
    adc_gpio_init(adc_pin);
    adc_select_input(adc_channel);

    //CE Init
    gpio_init(ce_spi);
    gpio_set_dir(ce_spi,GPIO_OUT);
    gpio_put(ce_spi,0);
    gpio_pull_down(ce_spi);

    //CSN Init
    gpio_init(csn_spi);
    gpio_set_dir(csn_spi,GPIO_OUT);
    gpio_pull_up(csn_spi);
    gpio_put(ce_spi,1);

    //Init SPI port
    spi_init(spi,5*Mhz);

    //SPI format
    spi_set_format(spi, //spi pointer
                    8,  //bits per transfer
                    0,  //polarity  
                    0,  //phase
                    SPI_MSB_FIRST);

    //Init SPI pins
    gpio_set_function(miso_spi,GPIO_FUNC_SPI);
    gpio_set_function(mosi_spi,GPIO_FUNC_SPI);
    gpio_set_function(sck_spi,GPIO_FUNC_SPI);

    //NRF24 init
    nrf24_init(&nrfTx);             //Not an specific configuration, has to be changes for final proyect
    nrf24_setTxAddr(&nrfTx, txAddr);

    //Loop forever
    while(1){
        adc_result = adc_read();
        txData[0] = (uint8_t)(adc_result>>8);
        txData[1] = (uint8_t)adc_result;
        nrf24_setMode(&nrfTx, txMode);
        if(nrf24_Transmit(&nrfTx, txData, sizeof(txData)) == 1)
        {
            nrf24_setMode(&nrfTx, standby);
            gpio_toggle(led_pin);
        }
        printf("Raw value: 0x%03x, voltage: %f V\n", adc_result, adc_result * conversion_factor);
        sleep_ms(500);
    }
}


//Functions
void gpio_toggle(uint pin){
    switch (gpio_get_out_level(pin))
    {
    case 0:
        gpio_put(pin,1);
        break;
    case 1:
        gpio_put(pin,0);
        break;
    }
}