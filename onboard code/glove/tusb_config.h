/*
 * This file is supposed to be the configuration
 * of tinyusb in my program. if I am having any
 * issues with tinyusb I should look to see if
 * my configuration is correct here or if I need
 * the documentation for it.
 *
 * Although this file is just a bunch of defines
 * and stuff, apparantly it's important for the usb
 * architechture or something.
 */


#ifndef _TUSB_CONFIG_H_ //if there is another configuration then ignore this one
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" { //make it in C format
#endif

#define BOARD_DEVICE_RHPORT_SPEED OPT_MODE_FULL_SPEED //the speed format of the pico. surprisingly fullspeed is slower than highspeed.

#define CFG_TUSB_RHPORT0_MODE (OPT_MODE_DEVICE | BOARD_DEVICE_RHPORT_SPEED) //use rhport0 in device mode with the specified speed.

#ifndef CFG_TUSB_MCU
#warning mcu doesnt autoassign //i dont even know if it assigns CFG_TUSB_MCU automatically T.T
#define CFT_TUSB_MCU OPT_MCU_RP2040
#endif

#ifndef CFG_TUSB_OS
#define CFG_TUSB_OS OPT_OS_NONE //apparantly microcontrollers have os these days. thought their whole point was getting rid of it.
#endif

#define CFG_TUD_ENDPOINT0_SIZE 64

//------------- CLASS -------------//
#define CFG_TUD_CDC               0
#define CFG_TUD_MSC               0
#define CFG_TUD_HID               1
#define CFG_TUD_MIDI              0
#define CFG_TUD_VENDOR            0


#define CFG_TUD_HID_EP_BUFSIZE 16 //sets buffer size. this should be enough.

#ifdef __cplusplus
} //extern "C" close
#endif

#endif //_TUSB_CONFIG_H_
