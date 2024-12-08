# dsPIC33CK64MC105
It was a really big hassle to get the PWM examples from the datasheet to work, so I am sharing a code snippet to get you stated :) 
![pinout](https://github.com/user-attachments/assets/13fa191a-a9ac-4c7c-bb50-147d1441dfad)
## Dual PWM Mode
PWM1L,PWM1H,PWM2H,PWM4L,PWM4H are set up in Dual PWM Mode.<br>
They can be controlled by sending data in the format: value1,value2,value3,value4,value5; via Uart (9600 baud rate)
<img width="273" alt="DUAL_PWM" src="https://github.com/user-attachments/assets/7d852249-b856-4b7c-a95c-027e56f6a655">
## Logic
I found two ways to combine multiple PWM Pins with logic gates.<br>
#### Combinatorial PWM Logic 
- PWM3L is set to PWM4L or PWM4H using the "Combinatorial PWM Logic" functinallity.
- PWM3L is set to PWM4L nor PWM4H using the "Combinatorial PWM Logic" functinallity.<br>

#### Configurable Logic Cell (CLC) <br> 
It is also possible to use the "CLC" functinallity but u have to connect the PWM pins to the Input pins of the CLC block since a internal connection is not possible.
Note the Blue arrows on the Pinout image.
- Pin RB0 is set to PWM1L or PWM1H or PWM2H
- Pin RB2 is set to not(PWM1L or PWM1H or PWM2H)

