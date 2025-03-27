# Arduino-beginner-mood-lamp
❮img src="images/054126.png" ❯
![image alt](https://github.com/fatiuhafees/Arduino_Introduction_to_RGBs_and_IR_reciver/blob/ec877865874518a25d39799e1775e3844a426781/Mood%20Lamp%202.jpg)
A user firendly introduction to RGBs and IR reciver. We are assuming that you have met arduino before. 

## Installation 
```bash
#include <Adafruit_NeoPixel.h>            //version 1.12.4. for controlling RGB ring
#include <IRremote.h>                     //version 2.6.1. Go to tool -> manage library -> search for: IRremote -> install version: 2.6.1
#include <ir_Lego_PF_BitStreamEncoder.h>  //for encoding IR signals
```

# Component list 
|Description|UK supplier Weblink|
|:-|:-|
|arduino uno |https://www.rapidonline.com/orangepip-kona328-arduino-uno-compatible-development-board-75-0550|
|groove shiled|https://www.seeedstudio.com/Base-Shield-V2.html?qid=eyJjX3NlYXJjaF9xdWVyeSI6Ikdyb3ZlIHNoZWlsZCIsImNfc2VhcmNoX3Jlc3VsdF9wb3MiOjEzLCJjX3RvdGFsX3Jlc3VsdHMiOjI2MCwiY19zZWFyY2hfcmVzdWx0X3R5cGUiOiJQcm9kdWN0IiwiY19zZWFyY2hfZmlsdGVycyI6InN0b3JlQ29kZTpbcmV0YWlsZXJdICYmIHF1YW50aXR5X2FuZF9zdG9ja19zdGF0dXM6WzFdIn0%3D|
|2x groove RGB ring|(https://www.seeedstudio.com/Grove-RGB-LED-Ring-16-WS2813-Mini-p-4201.html)|
|Ir-receiver|(https://www.seeedstudio.com/Grove-Infrared-Receiver.html)|
|ELEGOO remote|https://www.amazon.co.uk/dp/B07DJ58XGC/ref=sspa_dk_detail_3?psc=1&pd_rd_i=B07DJ58XGC&pd_rd_w=tL477&content-id=amzn1.sym.46187d6a-4306-4bc6-830c-7b2085e0e39f&pf_rd_p=46187d6a-4306-4bc6-830c-7b2085e0e39f&pf_rd_r=9A9GYDBFN27061N31GVH&pd_rd_wg=39ksB&pd_rd_r=41010ef7-ad1a-4ad2-bde9-e0d5e4ebdd11&s=electronics&sp_csd=d2lkZ2V0TmFtZT1zcF9kZXRhaWw|
|3x groove cables|https://www.seeedstudio.com/Grove-Universal-4-Pin-Buckled-5cm-Cable-5-PCs-Pack.html?qid=eyJjX3NlYXJjaF9xdWVyeSI6IkNhYmxlIiwiY19zZWFyY2hfcmVzdWx0X3BvcyI6MjMsImNfdG90YWxfcmVzdWx0cyI6NTUsImNfc2VhcmNoX3Jlc3VsdF90eXBlIjoiUHJvZHVjdCIsImNfc2VhcmNoX2ZpbHRlcnMiOiJzdG9yZUNvZGU6W3JldGFpbGVyXSAmJiBxdWFudGl0eV9hbmRfc3RvY2tfc3RhdHVzOlsxXSJ9|
|9V battery|https://www.rapidonline.com/gp-gppvcf9vg014-greencell-zinc-chloride-9v-pp3-battery-single-18-1058|
|Toggle switch|https://onecall.farnell.com/alcoswitch-te-connectivity/2-1825137-6/toggle-switch-spdt-5a-120vac-panel/dp/3397746|
