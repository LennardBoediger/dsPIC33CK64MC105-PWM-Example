# dsPIC33CK64MC105
It was a really big hassle to get the PWM examples from the datasheet to work, so I am sharing a code snippet to get you stated :) 
## Dual PWM Mode
PWM1L,PWM1H,PWM2H,PWM4L,PWM4H are set up in Dual PWM Mode.
They can be controlled by sending data in the format: value1,value2,value3,value4,value5; via Uart (9600 baud rate)
