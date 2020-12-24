# mcu_esp8266
涂鸦WB3S+ESP8266-12F

注意
在MUC SDK迁移时：1.system.h 需要修改extern const DOWNLOAD_CMD_S download_cmd[];为extern DOWNLOAD_CMD_S download_cmd[];2.使用mcu为esp8266时，需要将system.h中product_info_update函数进行修改，不然会报错。3.esp8266下载程序时使用的时3.3v供电，使用5v时下载不成功。4.esp8266使用fastled模块时，使用NodeMCU和普通esp8266时，pin引脚与GPIO口对应关系不同，下载时使用Geniric ESP8266
