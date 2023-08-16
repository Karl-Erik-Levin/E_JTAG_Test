#ifndef _IrRemCtrl_
#define _IrRemCtrl_

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

enum IrKey {
    IR_NONE,
    IR_ON, IR_MENU, IR_TEST, IR_BACK, IR_C,
    IR_UP, IR_RIGHT, IR_DOWN, IR_LEFT, IR_ENTER,
    IR_0, IR_1, IR_2, IR_3, IR_4, IR_5, IR_6, IR_7, IR_8, IR_9
};

void IrInit(void);
IrKey IrCheck(void);

#endif // _IrRemCtrl_
