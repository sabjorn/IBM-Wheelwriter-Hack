#include "driver/rmt.h"
#include "driver/periph_ctrl.h"
#include "soc/rmt_reg.h"

constexpr size_t num_elements = 6;
constexpr rmt_channel_t rmt_channel = static_cast<rmt_channel_t>(1);
constexpr uint16_t pulse_length = 53; // 5.25us with 10Mhz clock divide
//constexpr uint16_t pulse_length = 420; // 5.25us with 80Mhz clock divide
unsigned long time_now = 0;

rmt_config_t rmt_rx;
rmt_item32_t rmt_values[num_elements];

byte asciiTrans[128] = 
//col: 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f     row:
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0

//    
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 1
     
//    sp     !     "     #     $     %     &     '     (     )     *     +     ,     -     .     /
     0x00, 0x49, 0x4b, 0x38, 0x37, 0x39, 0x3f, 0x4c, 0x23, 0x16, 0x36, 0x3b, 0xc, 0x0e, 0x57, 0x28, // 2
     
//     0     1     2     3     4     5     6     7     8     9     :     ;     <     =     >     ?
     0x30, 0x2e, 0x2f, 0x2c, 0x32, 0x31, 0x33, 0x35, 0x34, 0x2a ,0x4e, 0x50, 0x00, 0x4d, 0x00, 0x4a, // 3

//     @     A     B     C     D     E     F     G     H     I     J     K     L     M     N     O
     0x3d, 0x20, 0x12, 0x1b, 0x1d, 0x1e, 0x11, 0x0f, 0x14, 0x1F, 0x21, 0x2b, 0x18, 0x24, 0x1a, 0x22, // 4

//     P     Q     R     S     T     U     V     W     X     Y     Z     [     \     ]     ^     _
     0x15, 0x3e, 0x17, 0x19, 0x1c, 0x10, 0x0d, 0x29, 0x2d, 0x26, 0x13, 0x41, 0x00, 0x40, 0x00, 0x4f, // 5
     
//     `     a     b     c     d     e     f     g     h     i     j     k     l     m     n     o
     0x00, 0x01, 0x59, 0x05, 0x07, 0x60, 0x0a, 0x5a, 0x08, 0x5d, 0x56, 0x0b, 0x09, 0x04, 0x02, 0x5f, // 6
     
//     p     q     r     s     t     u     v     w     x     y     z     {     |     }     ~    DEL
     0x5c, 0x52, 0x03, 0x06, 0x5e, 0x5b, 0x53, 0x55, 0x51, 0x58, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00}; // 7


void setup() {
    Serial.begin(115200);

    rmt_rx.channel       = rmt_channel;
    rmt_rx.gpio_num      = GPIO_NUM_22;
    rmt_rx.clk_div       = 80; // 1 Mhz, .1us, we can force time length below;
    rmt_rx.mem_block_num = 2;
    rmt_rx.rmt_mode      = RMT_MODE_RX;
    rmt_rx.rx_config.idle_threshold = 1024;
    rmt_rx.rx_config.filter_en           = false;
    rmt_rx.rx_config.filter_ticks_thresh = 0;

    rmt_config(&rmt_rx);
    rmt_driver_install(rmt_rx.channel, 2048, 0);

    // start the channel
    rmt_rx_start(rmt_channel, true);
}

void loop() {
    size_t i;
    size_t rx_size = 0;
    rmt_item32_t* items = NULL;

    // define ringbuffer handle
    RingbufHandle_t rb;

    // get the ring buffer handle
    rmt_get_ringbuf_handle(rmt_channel, &rb);

    // get items, if there are any
    items = (rmt_item32_t*) xRingbufferReceive(rb, &rx_size, 8);
    if(items)
    {
        for ( i=0; i < rx_size / 4; i++ )
        {
            Serial.print("{");
            Serial.print("\"state0\": ");
            Serial.print((items[i].level0) ? "\"high\"" : "\"low\"");
            Serial.print(", \"duration0\": ");
            Serial.print(items[i].duration0);
            Serial.print(", \"state1\": ");
            Serial.print((items[i].level1) ? "\"high\"" : "\"low\"");
            Serial.print(", \"duration1\": ");
            Serial.print(items[i].duration1);
            Serial.println("}");
        }
        //Serial.print("\r\n");
        vRingbufferReturnItem(rb, (void*) items);
    }        
}
