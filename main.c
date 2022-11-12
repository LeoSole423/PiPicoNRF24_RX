#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define Mhz 100000 

int main(){

    //Initialize chose serial port
    stdio_init_all();

    //Spi pins
    const uint ce_spi = 20;
    const uint csn_spi = 17;
    const uint sck_spi = 18;
    const uint mosi_spi = 19;
    const uint miso_spi = 16;

    //Spi port
    spi_inst_t *spi = spi0;

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

    //Loop forever
    while(1){

    }

}