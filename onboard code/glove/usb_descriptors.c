#include "tusb.h"
#include "usb_descriptors.h"

#define PID_MAP(itf, n)  ((CFG_TUD_##itf) << (n))
#define USB_PID (0x4000 | PID_MAP(CDC, 0) | PID_MAP(MSC, 1) | PID_MAP(HID, 2) | PID_MAP(MIDI, 3) | PID_MAP(VENDOR, 4))

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

    //pointing to another place
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,

    .bNumConfigurations = 0x01 //number of possible configurations
};

uint8_t const * tud_descriptor_device_cb(void) {
    return (uint8_t const *) &desc_device;
}

uint8_t const desc_hid_report[] =
        {
                TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(REPORT_ID_KEYBOARD         )),
                TUD_HID_REPORT_DESC_MOUSE   ( HID_REPORT_ID(REPORT_ID_MOUSE            )),
                TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(REPORT_ID_CONSUMER_CONTROL )),
                TUD_HID_REPORT_DESC_GAMEPAD ( HID_REPORT_ID(REPORT_ID_GAMEPAD          ))
        };

uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance) {
    (void) instance;
    return desc_hid_report;
}


enum {
    ITF_NUM_HID, //0
    ITF_NUM_TOTAL //1
};

#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN) //config len plus hid len for total len.

#define EPNUM_HID 0x81 //this is the epin for the hid, but i dont know why this is 0x81

uint8_t const desc_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 5)
};

uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
    (void) index;
    return desc_configuration;
}


char const* string_desc_arr[] = {
    (const char[]) {0x09, 0x04}, //0. supported language is english 0x0409
    "I_live_on_saturn TM", //1. manufacturer
    "somatic glove", //2. product
    "123", //3. serial, should be chip id
}

