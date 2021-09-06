#include "mbed.h"
#include "SDIOBlockDevice.h"
#include "FATFileSystem.h"
#include "MediaPlayer.h"


// File system on SD card connected over SDIO
static SDIOBlockDevice bd;
BlockDevice *BlockDevice::get_default_instance() { return &bd; }
BlockDevice *get_other_blockdevice() { return &bd; }
BlockDevice *blockDevice = BlockDevice::get_default_instance();
FATFileSystem fs("sd", blockDevice);


//audio
DigitalOut led1(LED1);

//LCD
DigitalOut selectSPI(D4, true); // Select SPI iterface on LCD
ILI9341V myLCD(SPI_DMA_, 10000000, D11, D12, D13, D10, D8, D7, "myLCD");


// Media player
MediaPlayer media_player(&myLCD, PA_0);


//  controls
InterruptIn sw(BUTTON1);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread event_loop;

void buttop_handler()
{
    media_player.stop();    
}


int main()
{
    event_loop.start(callback(&queue, &EventQueue::dispatch_forever));
    sw.fall(queue.event(buttop_handler));

    media_player.play(&fs, "NP_4100-6200_16fps.craw", 16);
    while (true) {
        ThisThread::sleep_for(500ms);
        led1 = !led1;    
    }
}

