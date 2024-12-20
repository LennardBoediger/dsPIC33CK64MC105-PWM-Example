# dsPIC33CK64MC105
The dsPIC33 family is a really cost-effective and capable option if you need a lot of PWM signals, but the datasheets are pretty intense and the code examples don't usually work right out of the box.

Here I am sharing a code snippet to get you started 🫶 
![pinout](https://github.com/user-attachments/assets/13fa191a-a9ac-4c7c-bb50-147d1441dfad)
## Dual PWM Mode
**PWM1L,PWM1H,PWM2H,PWM4L,PWM4H** are set up in Dual PWM Mode.<br>
They can be controlled by sending data in the format: value1,value2,value3,value4,value5; via Uart (9600 baud rate)


<img width="273" alt="DUAL_PWM" src="https://github.com/user-attachments/assets/7d852249-b856-4b7c-a95c-027e56f6a655">

## Configurable Logic
I found two ways to combine multiple PWM Pins with logic gates.<br>
#### Combinatorial PWM Logic 
- **PWM3L** is set to **PWM4L or PWM4H** using the "Combinatorial PWM Logic" functinallity.
- **PWM3L** is set to **PWM4L nor PWM4H** using the "Combinatorial PWM Logic" functinallity.<br>

#### Configurable Logic Cell (CLC) <br> 
It is also possible to use the “CLC” functionality, but you have to connect the PWM pins to the input pins of the CLC block, as an internal connection is not possible.<br>
**Note the Blue arrows on the Pinout image.**
- Pin **RB0** is set to **PWM1L or PWM1H or PWM2H**
- Pin **RB2** is set to **not(PWM1L or PWM1H or PWM2H)**

