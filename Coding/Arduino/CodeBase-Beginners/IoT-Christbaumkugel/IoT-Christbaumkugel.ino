//*** Import of Libraries *********************************************************************************************************************************************
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Adafruit_NeoPixel.h>
//*********************************************************************************************************************************************************************

//*** Global Settings *************************************************************************************************************************************************
#define LED_PIN     14  // Defines the Data-Input Pin for your LED Stripe
#define LED_COUNT    8  // Number of LEDs in the device

#define DEVICE_NAME "Onsite Xmas"  // SET YOUR DEVICE NAME 

// https://www.uuidgenerator.net/ to get a unique UUID
#define SERVICE_UUID        "fcf88392-048b-11ea-8d71-362b9e155667"
#define CONTROL_CHAR_UUID   "fcf88b58-048b-11ea-8d71-362b9e155667"
#define COLOR_CHAR_UUID     "93783900-051a-11ea-8d71-362b9e155667"
#define BRIGHT_CHAR_UUID    "5cd72205-09d2-4a58-b704-f59bca4bfc8c"
//*********************************************************************************************************************************************************************

//*** Global Objects & Variables **************************************************************************************************************************************
BLEDevice           Device;
BLEServer*          pServer;
BLEService*         pService;
BLECharacteristic*  pControlCharacteristic;
BLECharacteristic*  pColorCharacteristic;
BLECharacteristic*  pBrightCharacteristic;
BLEAdvertising*     pAdvertising;

boolean isOn = false;
int r =226;
int g = 0;
int b = 216;

Adafruit_NeoPixel Stripe(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
//*********************************************************************************************************************************************************************

boolean check_on_off(String powerMode) {
  if (powerMode == "on" || powerMode == "On" || powerMode == "ON" || powerMode == "1")
  {
    isOn = true;
    return true;
  }
  else
  {
    isOn = false;
    return false;
  }
}

//*** Callbacks Classes ***********************************************************************************************************************************************
// Connect / Disconnect
class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServerCallback) {
      //TODO: print message to console so that you know that you device is connected
      ////TODO Einschalten: Let your Christmas tree shine in Telekom Magenta when you switch it on ;) 
    }

    void onDisconnect(BLEServer* pServerCallback) {
       //TODO: print message to console so that you know that you device is disconnected
      //TODO Ausschalten: Turn LEDs off
    }
};

// On / Off
class ControlCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
      if (pControlCharacteristic->getValue().length() > 0) {
        // Get value via Bluetooth
        std::string powerMode = pControlCharacteristic->getValue();
        Serial.print("New PowerMode: ");
        Serial.println(powerMode.c_str());

        /*
        TODO Einschalten:
        TODO Auschalten:
        use check_on_off(powerMode.c_str() to check if on
        if is on, then let your stripe shine with your show(uint32_t color)) method -> show(Stripe.Color(226, 0, 116))
        else turn led off
        */
  

      }
    }
};

// LED Colors
class LedColorCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
      if (pColorCharacteristic->getValue().length() > 0) {
        if (isOn) {
          // Get value via Bluetooth
          //TODO LedColorCallback Methode: parse brightnes and call method -> info: characteristic value is from type std::string and needs to transform to String
          
        }
      }
    }
};

// LED Brightness
class BrightnessCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
      if (pBrightCharacteristic->getValue().length() > 0) {
        // Get value via Bluetooth
        //TODO BrightnessCallback Methode: parse brightnes and call method -> info: characteristic value is from type std::string and needs to transform to String
   
      }
    }
};
//*********************************************************************************************************************************************************************

// *** Arduino Main Functions *****************************************************************************************************************************************
void setup() {
  // Set Baudrate to get the right output
  Serial.begin(115200);
  //Initializa BLE - start advertising
  initBLE();
  // Initialize LED Stripe
  Stripe.begin();
}

void loop() {

}
//*********************************************************************************************************************************************************************


void turn_off_leds() {
  //TODO: Turn Leds off with set rgb values to 0 (use your show(uint32_t color))
}

// value example (rgb) -> 163,73,164
void setColor(String rgb) {
  int firstSplit = rgb.indexOf(',');
  int lastSplit  = rgb.lastIndexOf(',');

  // Get single values of Red, Green and Blue
  r   = rgb.substring(0, firstSplit).toInt();
  g = rgb.substring(firstSplit + 1, lastSplit).toInt();
  b  = rgb.substring(lastSplit + 1, rgb.length()).toInt();

  Serial.print("RGB: ");
  Serial.print(r);
  Serial.print(", ");
  Serial.print(g);
  Serial.print(", ");
  Serial.println(b);

  //TODO RGB-Werte übertragen: Transmit pixel data to NeoPixels | use show(uint32_t color)

}

void show(uint32_t color) {
  //first cleat stripe
  Stripe.clear(); // Set all pixel colors to 'off'

  /*TODO Farben setzen: iterate trough all pixels on your stripe and set Pixel color 
   The first NeoPixel in a strand is #0, second is 1, all the way up
   to the count of pixels minus one.
   Import use Stripe.show() to Transmit pixel data so that your Stripe shows your settings
   */
}

void setBright(std::string bright) {
  //TODO Helligkeit: Set Brightness and transmit to stripe

  //little fix - set old color values
  show(Stripe.Color(r, g, b));
}
//*********************************************************************************************************************************************************************

// *** BLE Functions **************************************************************************************************************************************************
// Method to initialize Bluetooth Low Energy
void initBLE() {
  // Give your Device a custom name (Bluetooth-Name)
  Device.init(DEVICE_NAME);

  // Init Server, Service and Characteristics
  pServer       = Device.createServer();
  pService      = pServer->createService(SERVICE_UUID);
  pControlCharacteristic  = pService->createCharacteristic(CONTROL_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  pColorCharacteristic    = pService->createCharacteristic(COLOR_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  pBrightCharacteristic   = pService->createCharacteristic(BRIGHT_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);

  // Encryption
  Device.setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
  pControlCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  pColorCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  pBrightCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);

  // Register Server Callback to receive onConnect and onDisconnect Event
  pServer->setCallbacks(new ServerCallbacks());
  /*TODO Callback Methoden definieren: register Callback Methode. 
  for ControlCharacteristic -> ControlCharacteristicCallbacks()
  for ControlCharacteristic -> LedColorCallbacks()
  for BrightCharacteristic -> BrightnessCallbacks()
  */

  pService->start();

  // Advertising
  pAdvertising = Device.getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  Device.startAdvertising();

  Serial.print(DEVICE_NAME);
  Serial.println(" Advertised");
};
//*********************************************************************************************************************************************************************
