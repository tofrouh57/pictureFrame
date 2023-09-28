// This example is based on this tutorial
// https://www.instructables.com/id/Converting-Images-to-Flash-Memory-Iconsimages-for-/
// https://github.com/STEMpedia/eviveProjects/blob/master/imageToFlashMemoryIconsForTFT/
// https://github.com/maditnerd/st7789_bitmap


//essayer https://github.com/moononournation/Arduino_GFX/tree/master
//pour affichage nouvel ecran


// Don't forget to change User_Setup.h inside TFT_eSPI library !

#include <TFT_eSPI.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <image copy.h>

// #include "image2.h" //(Bitmap generated with LCD Image Converter) https://sourceforge.net/projects/lcd-image-converter/
TFT_eSPI tft = TFT_eSPI(); // Invoke library
HTTPClient http;

const char *ssid = "NUMERICABLE-4A9E"; // Enter SSID
const char *password = "3N3FXAI4RP";   // Enter Password
int iterCnt = 0;
bool flag = false;

void setup(void)
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("[SETUP] BOOT WAIT ...\n");

  tft.begin();             // initialize a ST7789 chip
  tft.setSwapBytes(true); // Swap the byte order for pushImage() - corrects endianness
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  Serial.println("tft is setup");

  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++)
  {
    Serial.print(".");
    delay(1000);
  }

  // Check if connected to wifi
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("No Wifi!");
    return;
  }

  Serial.println("Connected to Wifi, Connecting to server.");

  // try to connect to Websockets server

  http.begin("http://192.168.1.206:3000/image");

  Serial.println("starting http get");
  int httpCode = http.GET();

  Serial.println("got values");

  if (httpCode > 0)
  {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK)
    {

      // get length of document (is -1 when Server sends no Content-Length header)
      int len = http.getSize();
      Serial.print("length of document: ");
      Serial.println(len);

      // create buffer for read
      uint8_t buff[3200] = {0};

      Serial.println("get tcp stream  ");
      // get tcp stream
      WiFiClient *stream = http.getStreamPtr();
      int imgPos = 0;

      // read all data from server

      Serial.println("start get server data ");

      int byteCnt = 0;

      while (http.connected() && (len > 0 || len == -1))
      {
        // get available data size
        size_t size = stream->available();
  //      Serial.print("get available data size: ");
//        Serial.println(size);
        if (size)
        {
//          uint8_t *buff = (uint8_t *)malloc(3200);          // read up to 128 byte
          
          
          //         Serial.println("read stream ");
          int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

          // c contient le nombre de bits recus
          // Serial.print("amount of data read :  ");
          // Serial.print(c);
          // Serial.print("  remaining data  :  ");
          // Serial.println(len);
          // Serial.println("entering loop");
          if (c <= 0)
          {
            delay(1);
          }

          /*
                    for (int i = 0; i < c; i = i + 2)
                    {
                      uint16_t tVal = buff[i + 1];
                      tVal = tVal << 8;
                      tVal = tVal | (uint16_t)buff[i];
                      Serial.print("count : ");
                      Serial.print(byteCnt);
                      Serial.print("  val : ");
                      Serial.println(tVal, HEX);
                      byteCnt++;
                      if (byteCnt == 800)
                      {
                        exit(0);
                      }
                    }
          */

            tft.pushColors(buff,c);
//            free(buff);
/*
          for (int i = 0; i < c; i = i + 2)
          {

            uint16_t tVal = buff[i + 1];
            tVal = tVal << 8;
            tVal = tVal | (uint16_t)buff[i];
            // binImage[imgPos] = tVal;
            //  Serial.print("byteCnt : ");Serial.print(byteCnt);
            //  Serial.print("  byte : ");Serial.println(tVal,HEX);
            int px = byteCnt % 320;
            int py = byteCnt / 320;
            tft.drawPixel(px, py, (uint32_t)tVal);
            // Serial.print("byteCnt : ");
            // Serial.print(byteCnt);
            // Serial.print("  px : "); Serial.print(px);
            // Serial.print("  py : ");Serial.print(py);
            // Serial.print("  byte : ");Serial.println(tVal,HEX);

            byteCnt++;
            if (byteCnt == 800)
            {
              //                        exit(0);
            }
          }
*/
          if (len > 0)
          {
            len -= c;
          }
        }
        delay(1);
      }

      Serial.println();
      Serial.print("[HTTP] connection closed or file end.\n");
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void loop()
{

  /*
    if (flag == false)
    {
      flag = true;

      // CREATE TEMPORARY MEMORY BUFFER
      uint16_t *binImage = (uint16_t *)malloc(320 * 240);
      if (binImage == NULL)
      {
        Serial.println("malloc failed");
      }
      else
      {
        Serial.println("malloc succeeded");
      }

      for (int y = 0; y < 240; y++)
      {
        for (int x = 0; x < 320; x++)
        {
          binImage[y * 320 + x] = 0xF34A; // mercy[y*320+x];
        }
      }
      tft.fillScreen(TFT_BLUE);
      tft.pushImage(0, 0, 320, 240, binImage);
    }
    */
}