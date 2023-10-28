/*****************************************************************************
* | File        :   Button_Ctr.h
* | Author      :   Waveshare team 
* | Function    :   
* | Info        :   Button_Ctr Demo
*----------------
* |	This version:   V1.0
* | Date        :   2022-10-01
* | Info        :   
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "examples.h"

#if USE_CURRENT_TO_CHECK_PI_STATUS
#define Shutdown_Current_Ma 320
#endif
#define AVERAGE_CNT_MAX 50
#define BATTERY_LOW_mV_x50 160000 // 3.2V * 1000 * 50
#define BATTERY_LOW_HYST_mV_x50 20000 // 0.4V * 1000 * 50
Power_All_State Read_State;
static struct repeating_timer timer;
static bool m_bBatteryLow = false;
/******************************************************************************
    function: Timer_Callback
    brief : The Timer Callback and Output Power State
    parameter:
        t   :   The timer struct
******************************************************************************/
static bool Timer_Callback(struct repeating_timer *t)
{
    static UART_packet_t _uart_pack;
    static unsigned int _uiAverageCnt = 0;

    // start counter
    if (_uiAverageCnt == 0) {
        _uart_pack.rpi_power_mW_x50 = 0;
        _uart_pack.v_bat_mV_x50 = 0;
    }

    // measure and accumulate 50 results
    _uart_pack.rpi_power_mW_x50 += (uint32_t)INA219_getPower_mW();
    _uart_pack.v_bat_mV_x50 += (uint32_t)(DEC_ADC_Read() * 3.3 * (297/27) / 4.096);
    _uiAverageCnt++;

    // report result when acumulation is finished, restart counter
    if (_uiAverageCnt >= AVERAGE_CNT_MAX){

        //if(DEV_Digital_Read(PI_RUNNING_PIN) != 0) // do not send over UART if RPi is powered off
        uart_write_blocking(UART_ID, (const uint8_t *)(&_uart_pack), sizeof(_uart_pack));
        //printf("%d, %d\r\n", _uart_pack.rpi_power_mW_x50, _uart_pack.v_bat_mV_x50);
        _uiAverageCnt = 0;
        if (_uart_pack.v_bat_mV_x50 < BATTERY_LOW_mV_x50)
            m_bBatteryLow = true;
        if (_uart_pack.v_bat_mV_x50 > BATTERY_LOW_mV_x50 + BATTERY_LOW_HYST_mV_x50)
            m_bBatteryLow = false;
    }
}

bool GetBatteryLow(void){
    return m_bBatteryLow;
}

/******************************************************************************
    function: Button_Ctr_Init
    brief : Initialize the Button function
    parameter:
******************************************************************************/
void Button_Ctr_Init(void)
{
    
    Power_init();
    MP28167_Default_Config();
    add_repeating_timer_ms(-100, Timer_Callback, NULL, &timer);
}
/******************************************************************************
    function: Button_Ctr_Loop
    brief : loop function that handles button events,need a loop call
    parameter:
******************************************************************************/
void Button_Ctr_Loop(void)
{
    static bool power_state, running_state;
    power_state = Power_Ctrl_By_Button();
}
