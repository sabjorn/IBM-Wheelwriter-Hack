#include "driver/rmt.h"
#include "driver/periph_ctrl.h"
#include "soc/rmt_reg.h"

constexpr rmt_channel_t rmt_channel = static_cast<rmt_channel_t>(0);
constexpr uint16_t pulse_length = 52; // 525; // 5.25us with 1Mhz clock divide

rmt_config_t rmt_tx;
rmt_item32_t rmt_values[20];

void send_rmt() //uint8_t data[], uint8_t lengthOfData)
{
    // remember, all are inverted
    //sendByte(0b1001000010);
    //sendByte(0b0000000110);
    //auto a = 0x20 == sendByte(asciiTrans[*s++]);
    //sendByte(0b0000010100); // full space
    // fast text 

    // 0b0110111101 :: lsb
    rmt_values[0].duration0 = pulse_length;
    rmt_values[0].level0 = 1;
    rmt_values[0].duration1 = pulse_length;
    rmt_values[0].level1 = 0;

    rmt_values[1].duration0 = pulse_length;
    rmt_values[1].level0 = 1;
    rmt_values[1].duration1 = pulse_length;
    rmt_values[1].level1 = 1;

    rmt_values[2].duration0 = pulse_length;
    rmt_values[2].level0 = 1;
    rmt_values[2].duration1 = pulse_length;
    rmt_values[2].level1 = 1;

    rmt_values[3].duration0 = pulse_length;
    rmt_values[3].level0 = 0;
    rmt_values[3].duration1 = pulse_length;
    rmt_values[3].level1 = 1;

    rmt_values[4].duration0 = pulse_length;
    rmt_values[4].level0 = 1;
    rmt_values[4].duration1 = pulse_length;
    rmt_values[4].level1 = 0;
    
    // 0b1111111001 :: lsb
    rmt_values[5].duration0 = pulse_length;
    rmt_values[5].level0 = 1;
    rmt_values[5].duration1 = pulse_length;
    rmt_values[5].level1 = 0;

    rmt_values[6].duration0 = pulse_length;
    rmt_values[6].level0 = 0;
    rmt_values[6].duration1 = pulse_length;
    rmt_values[6].level1 = 1;

    rmt_values[7].duration0 = pulse_length;
    rmt_values[7].level0 = 1;
    rmt_values[7].duration1 = pulse_length;
    rmt_values[7].level1 = 1;

    rmt_values[8].duration0 = pulse_length;
    rmt_values[8].level0 = 1;
    rmt_values[8].duration1 = pulse_length;
    rmt_values[8].level1 = 1;

    rmt_values[9].duration0 = pulse_length;
    rmt_values[9].level0 = 1;
    rmt_values[9].duration1 = pulse_length;
    rmt_values[9].level1 = 1;

    // 0x20 + low = 1110111111
    rmt_values[10].duration0 = pulse_length;
    rmt_values[10].level0 = 1;
    rmt_values[10].duration1 = pulse_length;
    rmt_values[10].level1 = 1;

    rmt_values[11].duration0 = pulse_length;
    rmt_values[11].level0 = 1;
    rmt_values[11].duration1 = pulse_length;
    rmt_values[11].level1 = 1;

    rmt_values[12].duration0 = pulse_length;
    rmt_values[12].level0 = 1;
    rmt_values[12].duration1 = pulse_length;
    rmt_values[12].level1 = 1;

    rmt_values[13].duration0 = pulse_length;
    rmt_values[13].level0 = 0;
    rmt_values[13].duration1 = pulse_length;
    rmt_values[13].level1 = 1;

    rmt_values[14].duration0 = pulse_length;
    rmt_values[14].level0 = 1;
    rmt_values[14].duration1 = pulse_length;
    rmt_values[14].level1 = 1;

    // 0b1111101011 -- lsb
    rmt_values[15].duration0 = pulse_length;
    rmt_values[15].level0 = 1;
    rmt_values[15].duration1 = pulse_length;
    rmt_values[15].level1 = 1;

    rmt_values[16].duration0 = pulse_length;
    rmt_values[16].level0 = 0;
    rmt_values[16].duration1 = pulse_length;
    rmt_values[16].level1 = 1;

    rmt_values[17].duration0 = pulse_length;
    rmt_values[17].level0 = 0;
    rmt_values[17].duration1 = pulse_length;
    rmt_values[17].level1 = 1;

    rmt_values[18].duration0 = pulse_length;
    rmt_values[18].level0 = 1;
    rmt_values[18].duration1 = pulse_length;
    rmt_values[18].level1 = 1;

    rmt_values[19].duration0 = pulse_length;
    rmt_values[19].level0 = 1;
    rmt_values[19].duration1 = pulse_length;
    rmt_values[19].level1 = 1;
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

    send_rmt();

    rmt_config(&rmt_tx);
    rmt_driver_install(rmt_tx.channel, 0, 0);

    // start the channel
    rmt_tx_start(rmt_channel, true);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(rmt_write_items(rmt_channel, rmt_values, 20, 1));
  delay(5000);
}
