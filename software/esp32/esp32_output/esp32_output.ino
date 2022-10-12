#include "driver/rmt.h"
#include "driver/periph_ctrl.h"
#include "soc/rmt_reg.h"

constexpr size_t num_elements = 6;
constexpr rmt_channel_t rmt_channel = static_cast<rmt_channel_t>(0);
constexpr uint16_t pulse_length = 53; // 5.34us with 10Mhz clock divide
//constexpr uint16_t pulse_length = 420; // 5.25us with 80Mhz clock divide
unsigned long time_now = 0;

rmt_config_t rmt_tx;
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

bool send_rmt(uint16_t bin)
{
    rmt_values[0].level0 = 1;
    rmt_values[0].level1 = bin & 0x1;

    rmt_values[1].level0 = (bin >> 1) & 0x1;
    rmt_values[1].level1 = (bin >> 2) & 0x1;

    rmt_values[2].level0 = (bin >> 3) & 0x1;
    rmt_values[2].level1 = (bin >> 4) & 0x1;

    rmt_values[3].level0 = (bin >> 5) & 0x1;
    rmt_values[3].level1 = (bin >> 6) & 0x1;

    rmt_values[4].level0 = (bin >> 7) & 0x1;
    rmt_values[4].level1 = (bin >> 8) & 0x1;
   
    // add delay
    rmt_values[5].level0 = 0;
    rmt_values[5].level1 = 0;
    rmt_values[5].duration0 = pulse_length * 50;
    rmt_values[5].duration1 = 0;
   
    rmt_values[0].duration0 = pulse_length;
    rmt_values[0].duration1 = pulse_length;
    rmt_values[1].duration0 = pulse_length;
    rmt_values[1].duration1 = pulse_length;
    rmt_values[2].duration0 = pulse_length;
    rmt_values[2].duration1 = pulse_length;
    rmt_values[3].duration0 = pulse_length;
    rmt_values[3].duration1 = pulse_length;
    rmt_values[4].duration0 = pulse_length;
    rmt_values[4].duration1 = pulse_length;

    return rmt_write_items(rmt_channel, rmt_values, num_elements, 1);
}

void send_letter(char letter) {
    const auto underline = false;
    const auto bold = false;

    send_rmt(~0b100100001);
    send_rmt(~0b000001011);
    send_rmt(~0b100100001);
    send_rmt(~0b000000011);
    send_rmt(~asciiTrans[letter]);
    if (underline) {
      send_rmt(~0b000000000); // no space
      send_rmt(~0b100100001);
      send_rmt(~0b000001011);
      send_rmt(~0b100100001);
      send_rmt(~0b000000011);
      send_rmt(~asciiTrans['_']);
    }
    if (bold) {
      send_rmt(~0b000000001); // one microspace
      send_rmt(~0b100100001);
      send_rmt(~0b000001011);
      send_rmt(~0b100100001);
      send_rmt(~0b000000011);
      send_rmt(~asciiTrans[letter]);
      send_rmt(~0b000001001);
    } else {
      // not bold
      send_rmt(~0b000001010); // spacing for one character
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(22, INPUT_PULLUP);

    rmt_tx.channel       = rmt_channel;
    rmt_tx.gpio_num      = GPIO_NUM_23;
    rmt_tx.clk_div       = 8; // 1 Mhz, .1us, we can force time length below;
    rmt_tx.mem_block_num = 2;
    rmt_tx.rmt_mode      = RMT_MODE_TX;
    rmt_tx.tx_config.loop_en = false;
    rmt_tx.tx_config.idle_output_en  = 1;
    rmt_tx.tx_config.idle_level      = RMT_IDLE_LEVEL_LOW;
    // config below likely not needed  
    rmt_tx.tx_config.carrier_en      = false;
    rmt_tx.tx_config.carrier_duty_percent = 25;
    rmt_tx.tx_config.carrier_freq_hz = 38000;
    rmt_tx.tx_config.carrier_level   = RMT_CARRIER_LEVEL_HIGH;

    rmt_config(&rmt_tx);
    rmt_driver_install(rmt_tx.channel, 0, 0);

    // start the channel
    rmt_tx_start(rmt_channel, true);
}

void loop() {
    char buffer[20];
    if (Serial.available() > 0)
    {
       const auto bytesRead = Serial.readBytes(buffer, 1);
       send_letter(buffer[0]);
    }
}
