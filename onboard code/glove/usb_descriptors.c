#include "tusb.h"

#define _PID_MAP(itf, n)  ((CFG_TUD_##itf) << (n))
#define USB_PID (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4))

#define USB_VID 0xcafe
#define USB_BCD 0x0200


tusb_desc_device_t const desc_device = 
{
    .bLength = sizeof(tusb_desc_device_t), //size
    .bDescriptorType = TUSB_DESC_DEVICE, //constant from tiny usb.
    .bcdUSB = USB_BCD, //usb version. using usb 2.0
    .bDeviceClass = 0x00, //class code. should be 0 so each interface specifies its own class. this is zero because we dont use vendor.
    .bDeviceSubClass = 0x00, //because device class is zero this aswell needs to be zero.
    .bDeviceProtocol = 0x00, //this is dependent of device class and needs to be zero because we dont use vendor.
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE, //max packet size in endpoint0.

    .idVendor = USB_VID, //vendor id
    .idProduct = USB_PID, //product id
    .bcdDevice = 0x0100, //device release version
    
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,

    .bNumConfigurations = 0x01

}

