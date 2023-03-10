//More information at: ryansprojects.tech
//
//IMPORTANT: This requires Arduino Cloud to run as several variables are set in Arduino Cloud:
//int kegRemaining
//String kegSizeMessage
//int lastPour
//int pintsRemaining
//
//Initial calibration needs to occur for ounces being poured per second in while loop (line 430). Default set to 1 oz pour per second.
//Change NTP timezone as required, this is for easter egg commands like "Merry Xmas! that appear on 12/25"
//"millis" usage is in place rather than "delay" to 1) prevent OLED burn-in 2) ensure pour while loop doesn't get halted 
//That the above said, there are some "delay" functions. These need to be modified for quick readings of the pouring while not overloading data to the board.

//NTP for Date and Time
#include <WiFiUdp.h>
#include <NTPClient.h>
const long utcOffsetInSeconds = -18000; //New York timezone
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
int currentMonth;
int monthDay; 

// 'emptyKeg', 128x48px - picture icon
const unsigned char epd_bitmap_kegTHM48Invert [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x80, 0x13, 0xc7, 0x82, 0x78, 0x7f, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x80, 0x1f, 0xef, 0xc3, 0xfc, 0x3f, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0xfc, 0x1c, 0x70, 0xc3, 0x86, 0x18, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0xfc, 0x18, 0x30, 0x43, 0x06, 0x18, 0x18, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x80, 0x18, 0x20, 0x43, 0x06, 0x18, 0x18, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x80, 0x18, 0x20, 0x43, 0x02, 0x18, 0x18, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x80, 0x18, 0x20, 0x43, 0x06, 0x18, 0x0d, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x80, 0x18, 0x20, 0x43, 0x06, 0x18, 0x0d, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x80, 0x18, 0x20, 0x43, 0x86, 0x18, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0xfe, 0x18, 0x20, 0x43, 0xfc, 0x0f, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0xfe, 0x18, 0x20, 0x43, 0x78, 0x0f, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x3f, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0x3f, 0xf1, 0xff, 0x9f, 0xf0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x07, 0x38, 0x71, 0x83, 0x1c, 0x38, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x3f, 0x83, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x01, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x21, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x21, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x01, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x3f, 0x83, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x07, 0x38, 0x71, 0x83, 0x1c, 0x38, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0x3f, 0xf1, 0xff, 0x9f, 0xf0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x3f, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



// 'kegFull', 128x48px - picture icon
const unsigned char epd_bitmap_kegFull [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x18, 0xf1, 0x8f, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x18, 0xf1, 0x8f, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xf8, 0xf1, 0x8f, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xf8, 0xf1, 0x8f, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xf8, 0xf1, 0x8f, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x18, 0xf1, 0x8f, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x18, 0xf1, 0x8f, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xf8, 0xf1, 0x8f, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xf8, 0xf1, 0x8f, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xfc, 0x73, 0x8f, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xfc, 0x03, 0x80, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xfe, 0x0f, 0x80, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0f, 0xc0, 0x7e, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0xc0, 0x0e, 0x00, 0x60, 0x0f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xff, 0xfc, 0xf8, 0xc7, 0x8e, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf9, 0xff, 0xfc, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xe7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xff, 0xfc, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xff, 0xfc, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xff, 0xfc, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xff, 0xfc, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xc0, 0x7c, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xc7, 0x3c, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xc7, 0x3c, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xc7, 0x3c, 0xf8, 0xc7, 0xfe, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xc7, 0x3c, 0xf8, 0xc7, 0xde, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xc7, 0x3c, 0xf8, 0xc7, 0xde, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xc7, 0x3c, 0xf8, 0xc7, 0xfe, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xc7, 0x3c, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xc7, 0x3c, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xc0, 0x7c, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xff, 0xfc, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xff, 0xfc, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xff, 0xfc, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xff, 0xfc, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf9, 0xff, 0xfc, 0xf8, 0xc7, 0xce, 0x7c, 0x63, 0xe7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0xff, 0xfc, 0xf8, 0xc7, 0x8e, 0x7c, 0x63, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0xc0, 0x0e, 0x00, 0x60, 0x0f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0f, 0xc0, 0x7e, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};



// 'kegFullInvert', 128x48px - picture icon
const unsigned char epd_bitmap_kegFullInvert [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe7, 0x0e, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe7, 0x0e, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x07, 0x0e, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x07, 0x0e, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x07, 0x0e, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe7, 0x0e, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe7, 0x0e, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x07, 0x0e, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x07, 0x0e, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x03, 0x0c, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x03, 0xfc, 0x7f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x01, 0xf0, 0x7f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x3f, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0x3f, 0xf1, 0xff, 0x9f, 0xf0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x07, 0x38, 0x71, 0x83, 0x1c, 0x38, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x3f, 0x83, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x01, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x21, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x21, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x01, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x38, 0xc3, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x3f, 0x83, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x07, 0x38, 0x31, 0x83, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x07, 0x38, 0x71, 0x83, 0x1c, 0x38, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0x3f, 0xf1, 0xff, 0x9f, 0xf0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x3f, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


//Accelerometer variables
#include <Wire.h> //Wire library for I2C communication on acceleromter
int ADXL345 = 0x53; //The ADXL345 sensor I2C address
float X_out, Y_out, Z_out; // Accelerometer outputs

//Keg Pour Variables
int dly;
int dlyBeerPour=500; //This needs to be calculated with how many ounces of beer flow on a pour
float tiltAngle=0.8; //Need to find out the tilt angle value that corresponds to a beer being poured

int maxPony = 661;       
int maxQuarter = 992;     
int maxHalf = 1984;    
int tempPour = 0;
int kegSize = 661; //Defaults to pony keg (661) unless changed by cloud variable    
int ouncesRemaining = 661; //Default to pony keg if cant see cloud (but also on reboots)
int wifiDisconnect;
int beersRemaining;

//OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

String formattedDate;
String dayStamp;
String timeStamp;

//Millis tracking
const int CHANGEON_INTERVAL = 10000;
const int CHANGEOFF_INTERVAL = 20000;
unsigned long lastToggleTime = 0;

void setup() {
  Serial.begin(9600);

//Check if display works
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  else{
  Serial.println("Display is good");
  }

Wire.begin(); //Initate wire library


// Connect to Arduino IoT Cloud
initProperties();
ArduinoCloud.begin(ArduinoIoTPreferredConnection);
setDebugMessageLevel(2);
ArduinoCloud.printDebugInfo();


//Set ADXL345 Accelerometer in measuring mode
Wire.beginTransmission(ADXL345); //Start communicated with devices
Wire.write(0x2D); // Access/talk to POWER_CTL Register - 0x2D
//Enable measurement
Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable
Wire.endTransmission();

display.clearDisplay(); 

//ADDED callbacks for Arduino cloud status
ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT, doThisOnDisconnect);
ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, doThisOnConnect);
ArduinoCloud.addCallback(ArduinoIoTCloudEvent::SYNC, doThisOnSync);

timeClient.begin(); //NTP begin for date/time
}


void loop() {
//Arduino IoT cloud refresh
ArduinoCloud.update();

//Get current time
unsigned long currentTime = millis();

//NTP code for getting date, day, month
timeClient.update(); //Update time on every loop
time_t epochTime = timeClient.getEpochTime(); //Get epoch time for day+month converstions
struct tm *ptm = gmtime ((time_t *)&epochTime); //Get a time structure
monthDay = ptm->tm_mday; //Get day number as monthDay
currentMonth = ptm->tm_mon+1; //Get month number as currentMonth

   
// ===Read accelerometer data=== //
Wire.beginTransmission(ADXL345);
Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
Wire.endTransmission(false);
Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
X_out = ( Wire.read()| Wire.read() << 8); // X-axis value
X_out = X_out/256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
Y_out = ( Wire.read()| Wire.read() << 8); // Y-axis value
Y_out = Y_out/256;
Z_out = ( Wire.read()| Wire.read() << 8); // Z-axis value
Z_out = Z_out/256;

  





//OLED Display (Keg Remaining Percent)
if (kegRemaining == 100){
    if (currentTime - lastToggleTime >= CHANGEON_INTERVAL){
    display.clearDisplay();
    display.drawBitmap(0, 16, epd_bitmap_kegFullInvert, 128, 64, 1); 
    showOuncesRemainingTop();
    wifiCheck();
        if (currentTime - lastToggleTime >= CHANGEOFF_INTERVAL){
        lastToggleTime = currentTime;
        }
    }
    
    
    else{
    
        if (specialDateCheck() == 2){
          //Xmas
          String text = "Merry Xmas";
          displayDisplayCenter(text);
        }
        else if (specialDateCheck() == 3){
        //Testing input can go here
        Serial.println("Special date check - Returned 3");
        }
        else {
        Serial.println("No special date today");
        display.clearDisplay();
        display.setTextSize(4);
        display.setTextColor(WHITE);
        display.setCursor(15, 25);
        display.print(kegRemaining);
        display.print("%");
        showPintsRemainingTop();
        wifiCheck();
        }
  }
}

else if (kegRemaining <= 99 && kegRemaining >=10){ 
    if (currentTime - lastToggleTime >= CHANGEON_INTERVAL){
    showPintsRemainingBig();
    showOuncesRemainingTop();
    wifiCheck();
        if (currentTime - lastToggleTime >= CHANGEOFF_INTERVAL){
        lastToggleTime = currentTime;
        }
    }
      else{
        
       if (specialDateCheck() == 2){
          String text = "Merry Xmas";
          displayDisplayCenter(text);
        }
        else if (specialDateCheck() == 3){
          //Testing
          Serial.println("Special date check - Returned 3");
        }
        else{
        Serial.println("No special date today");
        showKegRemaining();
        showPintsRemainingTop();
        wifiCheck();
        }
      }
}

else if (kegRemaining <= 9 && kegRemaining >=1){ //Single digits
    if (currentTime - lastToggleTime >= CHANGEON_INTERVAL){
    display.clearDisplay();
    showPintsRemainingBig();
    showOuncesRemainingTop();
    wifiCheck();
        if (currentTime - lastToggleTime >= CHANGEOFF_INTERVAL){
        lastToggleTime = currentTime;
        }
    }
    else{
    showKegRemaining();
    showPintsRemainingTop();
    wifiCheck();
    }
}

else if (kegRemaining <= 0){
//Display empty keg inverted bitmap image
display.clearDisplay();
display.drawBitmap(0, 16, epd_bitmap_kegTHM48Invert, 128, 64, 1); //0,0 by default but shows both colors. Top yellow pixels are 0-16 (0,16 is start)
display.display(); 
delay(3000);
//Show normal 0% 
display.clearDisplay();
display.setTextSize(4);
display.setTextColor(WHITE);
display.setCursor(40, 25); //Second row is vertical placement
display.print("0%");
display.display();
delay(3000);
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(WHITE);
display.setCursor(0, 0);
display.println(" Empty Keg");
display.setCursor(0, 30);
display.println(" Refill Me");
display.display();
delay(3000);
wifiCheck();
        if (specialDateCheck() == 2){
          //Xmas
          String text = "Merry Xmas";
          displayDisplayCenter(text);
          delay(3000);
        }
        else if (specialDateCheck() == 3){
          //Testing
          Serial.println("Special date check - Returned 3");
          delay(3000);
        }
        else{
          Serial.println("No special date today");
        }
}


// Prints out accelerometer stats to serial
Serial.print("Xa= ");
Serial.print(X_out);
Serial.print("   Ya= ");
Serial.print(Y_out);
Serial.print("   Z_out: "); //This is the measurement we want
Serial.println(Z_out);
delay(250);


while (Z_out <= tiltAngle){
//start counting/timer and log to somewhere: log to variable will work but poweroff will reset it. otherwise need ot add microsd/database/etc. refer to "WhileLoopWithLED (L17) project"
Wire.beginTransmission(ADXL345);
Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
Wire.endTransmission(false);
Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
X_out = ( Wire.read()| Wire.read() << 8); // X-axis value
X_out = X_out/256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
Y_out = ( Wire.read()| Wire.read() << 8); // Y-axis value
Y_out = Y_out/256;
Z_out = ( Wire.read()| Wire.read() << 8); // Z-axis value
Z_out = Z_out/256;

//This assumes 1 ounce is pourced per second. Need to calibrate initially. 
ouncesRemaining=(ouncesRemaining - 1);
tempPour=(tempPour + 1);
lastPour=tempPour;



display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(30, 5); 
display.print("Current Pour");

if (tempPour <= 9){
display.setTextSize(4);
display.setTextColor(WHITE);
display.setCursor(50, 25); //Centered for single digits
display.print(tempPour);
display.display(); 
}
else
{
display.setTextSize(4);
display.setTextColor(WHITE);
display.setCursor(40, 25); //Centered for double digits
display.print(tempPour);
display.display();  
}


delay(dlyBeerPour);
}


//Reset tempPour (keep lastPour)  
tempPour=0;
delay(500);

//Convert ounces to beers, pints, and percentage of keg remaining
beersRemaining=(ouncesRemaining/12);
pintsRemaining=(ouncesRemaining/16);
kegRemaining=((float)ouncesRemaining/kegSize * 100);      //*new, changed hardcoded 992 to kegSize


}






void onKegSizeMessageChange()  {

 if (kegSizeMessage.equalsIgnoreCase("PONY") || kegSizeMessage.equalsIgnoreCase("SIXTH") ||kegSizeMessage.equalsIgnoreCase("SIXTH barrel")) {
  Serial.println("Sixth enabled via cloud");
  ouncesRemaining=maxPony;
  kegSize=maxPony;
}
else if (kegSizeMessage.equalsIgnoreCase("QUARTER") || kegSizeMessage.equalsIgnoreCase("QUARTER BARREL")){
	Serial.println("Quarter enabled via cloud");
  ouncesRemaining=maxQuarter;
  kegSize=maxQuarter;
}
else if (kegSizeMessage.equalsIgnoreCase("HALF") || kegSizeMessage.equalsIgnoreCase("HALF BARREL")){
	Serial.println("Half enabled via cloud");
  ouncesRemaining=maxHalf;
  kegSize=maxHalf;
}
else if (kegSizeMessage.equalsIgnoreCase("TEST") || kegSizeMessage.equalsIgnoreCase("TESTING")){
	Serial.println("Testing enabled via cloud");
  ouncesRemaining=9;
  kegSize=9;
}

}


void doThisOnDisconnect(){
Serial.println("Board disconnected from Arduino IoT Cloud - confirmed");
wifiDisconnect=1;
}

void doThisOnConnect(){
Serial.println("Board connected to Arduino IoT Cloud - confirmed");
wifiDisconnect=0;
}

void doThisOnSync(){
Serial.println("Board has run sync with Arduino IoT Cloud - confirmed");
}

void wifiCheck(){
if (wifiDisconnect == 1){
//NOTE: If using inverted picture, WiFi disconnected text wills show since we are just recoloring it
//Serial.println("Writing WiFi disconnected to display");
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(15, 57); 
display.print("WiFi Disconnected");
display.display();
}
else {
//Serial.println("Clearing WiFi discconected from display");
display.setTextSize(1);
display.setTextColor(BLACK);
display.setCursor(15, 57); 
display.print("WiFi Disconnected");
display.display();
} 

}

void showKegRemaining(){
display.clearDisplay();
display.setTextSize(4);
display.setTextColor(WHITE);
display.setCursor(33, 25);
display.print(kegRemaining);
display.print("%");
display.display();
}

void showOuncesRemainingTop(){
//Set small top text
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(40, 5); 
display.print("Oz: ");
display.setCursor(60, 5); 
display.print(ouncesRemaining);
display.display(); 
}

void showPintsRemainingTop(){
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(40, 5); 
display.print("Pints: ");
display.setCursor(80, 5); 
display.print(pintsRemaining);
display.display(); 
}

void showPintsRemainingBig(){
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
      if (pintsRemaining >= 10){
      display.setCursor(45, 20);
      display.print(pintsRemaining);
      }
      else{
      display.setCursor(55, 20);
      display.print(pintsRemaining); 
      }
    display.setTextSize(2);
    display.setCursor(35, 45);
    display.print("Pints");
    display.display();
}


int specialDateCheck(){
if (monthDay == 25 && currentMonth == 12){  
  return 2;
 }
else if (monthDay == 99 && currentMonth == 99){  
  return 3;
 }
else{
  return 0;
} 

}


void displayDisplayCenter(String text) {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);

  // display on horizontal and vertical center
  display.clearDisplay(); // clear display
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2);
  display.println(text); // text to display
  display.display();
}

