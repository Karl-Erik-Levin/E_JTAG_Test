#include "BoardPin.h"
#include "IrRemCtrl.h"

IRrecv irrecv(recvPin);         // Create a class object used to receive class
decode_results results;         // Create a decoding results class object

void IrInit(void) {
  irrecv.enableIRIn();        // Start the receiver
}

IrKey IrCheck(void) {
  IrKey retVal = IR_NONE;

  if (irrecv.decode(&results)) {                // Waiting for decoding
    if (results.value < (uint64_t) 0xFFFFFFFF) {
      serialPrintUint64(results.value, HEX);    // Print out the decoded results
      Serial.print("  ");
      switch (results.value) {
      case 0xFFA25D:
        Serial.println("On/Off");
        retVal = IR_ON;
        break;
      case 0xFF6897:
        Serial.println("0");
        retVal = IR_0;
        break;
      case 0xFF30CF:
        Serial.println("1");
        retVal = IR_1;
        break;
      case 0xFF18E7:
        Serial.println("2");
        retVal = IR_2;
        break;
      case 0xFF7A85:
        Serial.println("3");
        retVal = IR_3;
        break;
      case 0xFF10EF:
        Serial.println("4");
        retVal = IR_4;
        break;
      case 0xFF38C7:
        Serial.println("5");
        retVal = IR_5;
        break;
      case 0xFF5AA5:
        Serial.println("6");
        retVal = IR_6;
        break;
      case 0xFF42BD:
        Serial.println("7");
        retVal = IR_7;
        break;
      case 0xFF4AB5:
        Serial.println("8");
        retVal = IR_8;
        break;
      case 0xFF52AD:
        Serial.println("9");
        retVal = IR_9;
        break;
      case 0xFF02FD:
        Serial.println("Up");
        retVal = IR_UP;
        break;
      case 0xFFA857:
        Serial.println("Enter");
        retVal = IR_ENTER;
        break;
      case 0xFF9867:
        Serial.println("Down");
        retVal = IR_DOWN;
        break;
      case 0xFFE01F:
        Serial.println("Left");
        retVal = IR_LEFT;
        break;
      case 0xFF906F:
        Serial.println("Right");
        retVal = IR_RIGHT;
        break;
      case 0xFFE21D:
        Serial.println("Menu");
        retVal = IR_MENU;
        break;
      case 0xFF22DD:
        Serial.println("Test");
        retVal = IR_TEST;
        break;
      case 0xFFB04F:
        Serial.println("C");
        retVal = IR_C;
        break;
      case 0xFFC23D:
        Serial.println("Back");
        retVal = IR_BACK;
        break;
      default:
        Serial.println("UNKNOW");
        break;
      }
    }
    irrecv.resume();                            // Release the IRremote. Receive the next value
  }
  return retVal;
}
