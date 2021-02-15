/*! \brief Air Fuel Ratio (AFR) Gauge

   AFR monitor using TTGO ESP 32 module with 240 x 135 TFT full colour display
   Use ESP 32 Dev Module board definition.
*/

#include <TFT_eSPI.h>
#include <SPI.h>
#include <User_Setup.h>       // don't know what this is, don't seem to need it
#include <AceButton.h>          //!< for the encoder button
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <SpeedData.h>

// Define two buttons, different configurations for each 
using namespace ace_button;
const uint8_t TOP_BUTTON = 0;       //!< on board button above the USB-C port
const uint8_t BOTTOM_BUTTON = 35;   //!< on board button below the USB-C port
ButtonConfig topConfig;
ButtonConfig bottomConfig;
AceButton topButton(&topConfig);
AceButton bottomButton(&bottomConfig);

// SpeedData object to get data from the speeduino
SpeedData SData;

// wifi 
const char* ssid = "dfhome";
const char* password = "rentalguy";

// tft display buffer
uint16_t* tft_buffer = (uint16_t*) malloc( 26000 );  
bool      buffer_loaded = false;

// Mode constants
byte g_Mode = 0;
const byte MODE_AFR = 0;        // display AFR & variance from targeyt
const byte MODE_EGO = 1;        // display EGO correction
const byte MODE_LOOPS = 2;      // loops per second

// Serial2 pins
#define sTX 21    // Serial2 transmit (out), pin J4 on Speeduino connector
#define sRX 22    // Serial2 recieve (in), pin K4 on Speeduino connector

// define display
TFT_eSPI tft = TFT_eSPI(135, 240);

// make sprite objects
TFT_eSprite dispNum = TFT_eSprite(&tft);       // the big number displayed at top of screen
TFT_eSprite afrVarInd = TFT_eSprite(&tft);     // the AFR variance indicator
TFT_eSprite descText = TFT_eSprite(&tft);     // text displayed underneath the big number

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, sRX, sTX); //Serial port for connecting to Speeduino
  Serial.println("Start");
  Serial.println("Start");
  SData.setSerial(&Serial2);

  // connnect to wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed!");
  }
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup OTA
  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // initialize display
  tft.init();
  tft.setRotation(3);         // landscape
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextDatum(MC_DATUM);

  //Setup buttons
  pinMode(TOP_BUTTON, INPUT_PULLUP);
  pinMode(BOTTOM_BUTTON, INPUT_PULLUP);

  topButton.init(TOP_BUTTON);
  bottomButton.init(BOTTOM_BUTTON);

  // cofigure both buttons for simple clicks, seperate handlers for each.
  topConfig.setEventHandler(handleTopButton);       // top button event handler
  topConfig.setFeature(ButtonConfig::kFeatureClick);

  bottomConfig.setEventHandler(handleBottomButton);    // bottom button event handler
  bottomConfig.setFeature(ButtonConfig::kFeatureClick);
 
  // create sprite for numeric display
  dispNum.setTextFont(8);
  dispNum.createSprite(240, 85);
  dispNum.fillSprite(TFT_BLACK);
  dispNum.setTextColor(TFT_WHITE, TFT_BLACK);
  dispNum.setTextDatum(MC_DATUM);

  // create sprite for AFR variance indicator
  afrVarInd.createSprite(40, 50);
  afrVarInd.fillSprite(TFT_TRANSPARENT);
  afrVarInd.fillTriangle(20, 0, 0, 50, 40, 50, TFT_BLACK);
  afrVarInd.fillTriangle(20, 5, 3, 47, 37, 47, TFT_GREENYELLOW);

  // create sprite for text at bottom of screen (instead of AFR Variance Indicator)
  descText.setTextFont(4);
  descText.createSprite(240, 55);       //width, height
  descText.fillSprite(TFT_BLACK);
  descText.setTextColor(TFT_WHITE, TFT_BLACK);
  descText.setTextDatum(MC_DATUM);
}

void loop() {
  ArduinoOTA.handle();
  updateDisplay();              
  topButton.check();
  bottomButton.check();
}
