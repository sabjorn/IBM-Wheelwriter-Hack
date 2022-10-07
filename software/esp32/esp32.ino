#include "driver/rmt.h"
#include "driver/periph_ctrl.h"
#include "soc/rmt_reg.h"

constexpr rmt_channel_t rmt_channel = static_cast<rmt_channel_t>(0);
constexpr uint16_t pulse_length = 53; // 5.25us with 10Mhz clock divide
//constexpr uint16_t pulse_length = 420; // 5.25us with 80Mhz clock divide

rmt_config_t rmt_tx;
rmt_item32_t rmt_values[10];

bool send_rmt(uint16_t bin)
{
    for(uint16_t i = 0; i < 5; ++i)
    {
        auto step = i * 2;

        rmt_values[i].level0 = (bin >> step) & 0x1;
        rmt_values[i].level1 = (bin >> (step + 1)) & 0x1;
        rmt_values[i].duration0 = pulse_length;
        rmt_values[i].duration1 = pulse_length;
    }
    for(uint16_t i = 5; i < 10; ++i)
    {
        rmt_values[i].level0 = 0;
        rmt_values[i].level1 = 0;
        rmt_values[i].duration0 = pulse_length;
        rmt_values[i].duration1 = pulse_length;
    }
   
    return rmt_write_items(rmt_channel, rmt_values, 10, 1);
}

void setup() {
    Serial.begin(115200);

    rmt_tx.channel       = rmt_channel;
    rmt_tx.gpio_num      = GPIO_NUM_19;
    rmt_tx.clk_div       = 8; // 10 Mhz, .1us, we can force time length below;
    rmt_tx.mem_block_num = 1;
    rmt_tx.rmt_mode      = RMT_MODE_TX;
    rmt_tx.tx_config.loop_en = false;
    rmt_tx.tx_config.idle_output_en  = true;
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
  delay(5000);
  Serial.println("sending");
  // resetTypewriter
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000000000);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000000010);
  //send_rmt(~0b0001000000);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000010010);
  //send_rmt(~0b0000000000);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000011010);
  //send_rmt(~0b0000100100);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000001100);
  //send_rmt(~0b0100000000);
  //send_rmt(~0b0000000000);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000011010);
  //send_rmt(~0b0000010010);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000001100);
  //send_rmt(~0b0100000000);
  //send_rmt(~0b0000000000);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000010100);
  //send_rmt(~0b0000000000);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000001110);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000011000);
  //send_rmt(~0b0000000010);
  // send_letter
  send_rmt(~0b1001000010);
  send_rmt(~0b0000010110);
  send_rmt(~0b1001000010);
  send_rmt(~0b0000000110);
  send_rmt(~0b0000010100);
  send_rmt(~0b0000010100); // spacing for one character
  //// fastTextInit
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000010110);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000010010);
  //send_rmt(~0b0000000000);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000010100);
  //send_rmt(~0b0000000000);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000011010);
  //send_rmt(~0b0000001100);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000001100);
  //send_rmt(~0b0100000000);
  //send_rmt(~0b0000000000);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000001010);
  //send_rmt(~0b0100000000);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000011010);
  //send_rmt(~0b0000100100);
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000001100);
  //send_rmt(~0b0100000000);
  //send_rmt(~0b0000000000);

  //// fastTextChars
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000000110);
  ////send_rmt(~0b0001000000); // 'A'
  //send_rmt(~0b0000010100); // full space

  //// fastTextFinish
  //send_rmt(~0b1001000010);
  //send_rmt(~0b0000010010);
  //send_rmt(~0b0000000000);
}
