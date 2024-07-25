// Slavnem @2024-07-25
// Zamanlayıcı Fonksiyon ve Makroları

#ifndef DT_TIME_H
#define DT_TIME_H

// Kütüphaneler
#include "TypeDefine.h"
#include "Macro.h"

#define HOUR(hour) (hour < 1 ? 0 : hour) // Güvenli şekilde saat gösterimi
#define MINUTE(minute) (minute < 1 ? 0 : minute) // Güvenli şekilde dakika gösterimi

#define CLOCKHOUR(hour) (hour < 1 ? 0 : (uint8_s)(hour % 24)) // Gün'e Göre Saate Çevirsin
#define CLOCKMINUTE(minute) (minute < 1 ? 0 : (uint8_s)(minute % 60)) // Saat'e Göre Dakikayı Çevirsin

#define HOURTODAY(hour) (hour < 24 ? 0 : (uint8_s)(hour / 24)) // Saati Güne Çevirme
#define HOURTOMINUTE(hour) (hour < 1 ? 0 : (uint16_s)(hour * 60)) // Saati Dakikaya Çevirme
#define HOURTOSECOND(hour) (hour < 1 ? 0 : (uint32_s)(hour * 3600)) // Saati Saniyeye Çevirme

#define MINUTETODAY(minute) (minute < 1440 ? 0 : (uint8_s)(minute / 1440)) // Dakikayı Güne Çevirme
#define MINUTETOHOUR(minute) (minute < 1 ? 0 : (uint8_s)(minute / 60)) // Dakikayı Saat Çevirme
#define MINUTETOSECOND(minute) (minute < 1 ? 0 : (uint32_s)(minute * 60)) // Dakikayı Saniyeye Çevirme

#define TOTALTIME(hour, minute) (HOURTOMINUTE(hour) + MINUTE(minute)) // Toplam Süreyi Hesaplama

#endif