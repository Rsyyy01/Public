#include <U8g2lib.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

#define NTP1 "ntp1.aliyun.com"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP1, 60 * 60 * 8, 30 * 60 * 1000);

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define LED 2
#define KEY 16
#define OLED_MOSI 13
#define OLED_CLK 14
#define OLED_CS 15
#define OLED_DC 5
#define OLED_RESET 4
#define DHT_PIN 0
#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);
U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, OLED_CS, OLED_DC, OLED_RESET);

bool ledState = HIGH;

unsigned long startTime = 0;
const long interval = 10;
unsigned long currentTime = 0;

float Temperature = 0;
float Hudmity = 0;

const char *ssid = "Rsyyy";
const char *password = "12345678";

unsigned char KeyFlag = 0;
unsigned char KeyVal = 0, KeyOld = 0, KeyUp = 0, KeyDown = 0;
unsigned char Time[6] = {0};
unsigned char TimeSet[3] = {0};
unsigned char TimeSetCh[3] = {0};
unsigned int Year = 0;
unsigned char TimeTime = 0;
unsigned char SerialFlag = 0;
unsigned char CFlag = 0;

#define LEN 100
uint8_t RecBuf[LEN] = {0};
uint16_t length = 0;

const unsigned char Temperature_Picture[] = {
    0x00,
    0xff,
    0x00,
    0x00,
    0x80,
    0x83,
    0x03,
    0x80,
    0xc0,
    0x00,
    0x06,
    0x80,
    0x40,
    0x00,
    0x06,
    0x80,
    0x40,
    0x00,
    0x04,
    0x80,
    0x40,
    0x00,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0x00,
    0x04,
    0x80,
    0x40,
    0xf0,
    0x04,
    0x80,
    0x40,
    0x00,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0x80,
    0x04,
    0x80,
    0x40,
    0xf0,
    0x04,
    0x80,
    0x40,
    0x00,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0x00,
    0x04,
    0x80,
    0x40,
    0xf0,
    0x04,
    0x80,
    0x40,
    0x00,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0x80,
    0x04,
    0x80,
    0x40,
    0xf0,
    0x04,
    0x80,
    0x40,
    0xf0,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0x10,
    0x04,
    0x80,
    0x40,
    0xf0,
    0x04,
    0x80,
    0x40,
    0x00,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0x00,
    0x04,
    0x80,
    0x40,
    0x0e,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0x80,
    0x40,
    0xfe,
    0x04,
    0xc0,
    0x60,
    0xfe,
    0x0c,
    0x60,
    0x30,
    0xfe,
    0x18,
    0x30,
    0x18,
    0xff,
    0x31,
    0x10,
    0x88,
    0xff,
    0x23,
    0x98,
    0xcc,
    0xff,
    0x67,
    0xc8,
    0xe4,
    0xff,
    0x4f,
    0xcc,
    0xe6,
    0xff,
    0xcf,
    0xec,
    0xf6,
    0x3f,
    0xce,
    0xe4,
    0xf2,
    0x3f,
    0x9c,
    0xe4,
    0xf2,
    0x3f,
    0x9e,
    0xe4,
    0xf2,
    0xff,
    0x9f,
    0xec,
    0xf6,
    0xff,
    0xcf,
    0xcc,
    0xe6,
    0xff,
    0xcf,
    0xc8,
    0xe4,
    0xff,
    0x4f,
    0x98,
    0xcc,
    0xff,
    0x67,
    0x10,
    0x88,
    0xff,
    0x23,
    0x30,
    0x18,
    0xff,
    0x31,
    0x60,
    0x30,
    0x7e,
    0x18,
    0xc0,
    0xe0,
    0x00,
    0x0e,
    0x00,
    0x80,
    0x83,
    0x03,
    0x00,
    0x00,
    0xfe,
    0x00,
    0xfe,
};

const unsigned char Clock_Picture[] = {
    0x38,
    0xc0,
    0xc1,
    0x2c,
    0x40,
    0x22,
    0xc2,
    0x3f,
    0x24,
    0x22,
    0x40,
    0x24,
    0x12,
    0x80,
    0x84,
    0x08,
    0x04,
    0x41,
    0x04,
    0x04,
    0x42,
    0x04,
    0x04,
    0x62,
    0x06,
    0x04,
    0x26,
    0x02,
    0x04,
    0x24,
    0xc2,
    0x07,
    0x24,
    0x02,
    0x00,
    0x44,
    0x04,
    0x00,
    0x42,
    0x04,
    0x00,
    0xc2,
    0x0c,
    0x00,
    0x83,
    0x08,
    0x00,
    0x01,
    0x10,
    0x80,
    0x00,
    0x60,
    0x60,
    0x00,
    0x90,
    0x9f,
    0x80,
    0x08,
    0x00,
    0xf1,
};

void setup()
{
    pinMode(KEY, INPUT);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, !HIGH);

    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.setFontPosTop();
    dht.begin();

    Serial.begin(115200);

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
    u8g2.setCursor(3, 3);
    u8g2.printf("Welcome!");
    u8g2.setCursor(3, 19);
    u8g2.printf("Please wait…");
    u8g2.sendBuffer();
    Serial.println("Please wait…");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
        delay(500);

    delay(5000);

    timeClient.begin();
    delay(1000);
    timeClient.update();

    time_t time = (time_t)(timeClient.getEpochTime());
    tm timeinfo;
    gmtime_r(&time, &timeinfo);
    timeClient.update();

    Serial.println("Welcome!");
}

unsigned char KeyRead()
{
    if (digitalRead(KEY) == LOW)
        return 1;
    return 0;
}

void loop()
{
    currentTime = millis();

    if (currentTime - startTime >= interval)
    {
        startTime = currentTime;

        KeyVal = KeyRead();
        KeyDown = KeyVal & (KeyVal ^ KeyOld);
        KeyUp = ~KeyVal & (KeyVal ^ KeyOld);
        KeyOld = KeyVal;

        if (KeyDown)
            KeyFlag = 1;

        if (KeyUp)
            KeyFlag = 0;

        TimeTime++;
        if (TimeTime == 20)
        {
            TimeTime = 0;

            if (CFlag)
                CFlag++;
        }

        time_t time = (time_t)(timeClient.getEpochTime());
        tm timeinfo;
        gmtime_r(&time, &timeinfo);
        timeClient.update();

        Time[0] = timeClient.getHours();
        Time[1] = timeClient.getMinutes();
        Time[2] = timeClient.getSeconds();
        Year = timeinfo.tm_year + 1900;
        Time[3] = timeinfo.tm_mon + 1;
        Time[4] = timeinfo.tm_mday;

        Time[5] = timeinfo.tm_wday;

        Temperature = dht.readTemperature();
        Hudmity = dht.readHumidity();

        u8g2.clearBuffer();
        if (KeyFlag == 1)
        {
            u8g2.drawXBMP(0, 0, 25, 64, Temperature_Picture);
            u8g2.setFont(u8g2_font_timB14_tn);
            u8g2.setCursor(26, 2);
            u8g2.printf("%.f", Temperature);
            u8g2.setCursor(26, 20);
            u8g2.printf("%.f", Hudmity);
            u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
            u8g2.setCursor(46, 5);
            u8g2.printf("度");
            u8g2.setCursor(46, 23);
            u8g2.printf("%%");
            u8g2.drawXBMP(72, 0, 20, 20, Clock_Picture);
            u8g2.setFont(u8g2_font_timB14_tn);
            u8g2.setCursor(90, 21);
            u8g2.printf("%02d", TimeSet[0]);
            u8g2.setCursor(90, 35);
            u8g2.printf("%02d", TimeSet[1]);
            u8g2.setCursor(90, 49);
            u8g2.printf("%02d", TimeSet[2]);
            u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
            u8g2.setCursor(110, 24);
            u8g2.printf("时");
            u8g2.setCursor(110, 38);
            u8g2.printf("分");
            u8g2.setCursor(110, 52);
            u8g2.printf("秒");
        }
        else
        {
            u8g2.setCursor(10, 10);
            u8g2.setFont(u8g2_font_timB24_tn);
            u8g2.printf("%02d:%02d", Time[0], Time[1]);
            u8g2.setCursor(86, 16);
            u8g2.setFont(u8g2_font_timB18_tn);
            u8g2.printf(":%02d", Time[2]);
            u8g2.setCursor(7, 44);
            u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
            u8g2.printf("%04d%02d%02d      星期%d", Year, Time[3], Time[4], Time[5]);
        }
        u8g2.sendBuffer();
    }

    if (Serial.available() > 0)
    {
        uint8_t i = 0;
        delay(10);
        length = Serial.available();
        if (length == 6)
            SerialFlag = 1;
        else
            Serial.print("Error!");

        while (length--)
            RecBuf[i++] = Serial.read();

        if (SerialFlag == 1)
        {
            SerialFlag = 0;
            TimeSetCh[0] = (RecBuf[0] - '0') * 10 + RecBuf[1] - '0';
            TimeSetCh[1] = (RecBuf[2] - '0') * 10 + RecBuf[3] - '0';
            TimeSetCh[2] = (RecBuf[4] - '0') * 10 + RecBuf[5] - '0';

            if (TimeSetCh[0] <= 23 && TimeSetCh[1] <= 59 && TimeSetCh[2] <= 59)
            {
                TimeSet[0] = TimeSetCh[0];
                TimeSet[1] = TimeSetCh[1];
                TimeSet[2] = TimeSetCh[2];
                Serial.print("Success!");
            }
            else
                Serial.print("Error!");

            TimeSetCh[0] = TimeSetCh[1] = TimeSetCh[2] = 0;
        }
    }

    if (Time[0] == TimeSet[0] && Time[1] == TimeSet[1] && Time[2] == TimeSet[2] && !CFlag)
        CFlag = 1;

    if (CFlag == 5)
        CFlag = 0;

    digitalWrite(LED, CFlag % 2);
}