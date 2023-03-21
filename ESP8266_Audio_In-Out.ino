/* This code is write for ESP8266.
 * Is take audio from ADC input (A0) and write it to I2S output (Rx).
 * You need to switch your cpu frequency to 160Mhz (Arduino IDE : Tools -> CPU Freqency).
*/

#include "Arduino.h"
extern "C" {
#include "user_interface.h"
#include "i2s_reg.h"
}
#include <I2S.h>

// Logs
unsigned long read_nb = 0;
unsigned long read_sample = 0;
unsigned long log_time = micros();


// Input
ADC_MODE(ADC_TOUT);
#define num_samples 8
uint16_t adc_addr[num_samples]; // point to the address of ADC continuously fast sampling output
uint16_t adc_num = num_samples; // sampling number of ADC continuously fast sampling, range [1, 65535]
uint8_t adc_clk_div = 8; // ADC working clock = 80M/adc_clk_div, range [8, 32], the recommended value is 8

void ReadADC() 
/* Read analogic input and send to output
 * Source : https://arduino.stackexchange.com/questions/48640/esp8266-system-adc-read-fast-always-give-1024-as-output
*/
{
  system_adc_read_fast(adc_addr, adc_num, adc_clk_div); // Read new values
  read_sample += adc_num;
  read_nb++;

  writeI2S(); // Send new values
}


// Output
uint32_t i2sACC;
uint16_t DAC = 0x8000;
uint16_t err;

void writeI2S() 
/* Write data from adc_addr buffer to I2S output (Rx pin)
 * Source : https://www.hackster.io/janost/audio-hacking-on-the-esp8266-fa9464#toc-rearrange-the-code-for-isr-1
*/
{
  for (int j = 0; j < adc_num;  j++) { // For all ADC read
    DAC = 0x8000 + adc_addr[j];

    for (uint8_t i = 0; i < 32; i++) {
      i2sACC = i2sACC << 1;

      if (DAC >= err) {
        i2sACC |= 1;
        err += 0xFFFF - DAC;
      }
      else
      {
        err -= DAC;
      }
    }
    bool flag = i2s_write_sample(i2sACC);
  }
}

void setup() {
  Serial.begin(115200);
  wifi_set_opmode(NULL_MODE); // system_adc_read_fast() Does'nt work with wifi
  delay(1);

  system_update_cpu_freq(160);
  i2s_begin(); //Start the i2s DMA engine
  i2s_set_rate(96160); //Set sample rate 
}

void loop() {
  ReadADC();

  if (micros() - log_time > 1000000.0) { // Print logs
    Serial.print("Sampling rate: ");
    Serial.println(read_sample);
    Serial.print("Read number: ");
    Serial.println(read_nb);

    log_time = micros();
    read_sample = 0;
    read_nb = 0;
  }
}
