#include <dht.h>
dht DHT;
#define DHT22_PIN 3

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

#include "hackAir.h"

hackAIR sensor(SENSOR_PMS5003);

static const PROGMEM u1_t NWKSKEY[16] = { 0x9E, 0x8F, 0x36, 0xBF, 0xCD, 0xD8, 0x1B, 0x2C, 0x52, 0x75, 0x62, 0xE7, 0x22, 0xD3, 0x78, 0xE8 };

static const u1_t PROGMEM APPSKEY[16] =  { 0xF2, 0xDE, 0x8E, 0x9A, 0xF9, 0x66, 0x51, 0x23, 0x3C, 0xEA, 0xD0, 0xCE, 0xDD, 0x0B, 0xFC, 0x95 };

static const u4_t DEVADDR = 0x26011FA3 ; 

void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }



struct hackAirData mydata1;
struct data {
  short int a;
  short int b;
  short int e;
  short int f;
  
}mydata;

static osjob_t sendjob;

const unsigned TX_INTERVAL = 2;

const lmic_pinmap lmic_pins = {
  .nss = 8,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 4,
  .dio = {7, 2, LMIC_UNUSED_PIN},
};

void onEvent (ev_t ev) {
 // Serial.print(os_getTime());
  //Serial.print(": ");
  switch (ev) {
  /* case EV_SCAN_TIMEOUT:
     // Serial.println(F("EV_SCAN_TIMEOUT"));
      break;
    case EV_BEACON_FOUND:
      //Serial.println(F("EV_BEACON_FOUND"));
      break;
    case EV_BEACON_MISSED:
      //Serial.println(F("EV_BEACON_MISSED"));
      break;
    case EV_BEACON_TRACKED:
      //Serial.println(F("EV_BEACON_TRACKED"));
      break;
    case EV_JOINING:
      //Serial.println(F("EV_JOINING"));
      break;
    case EV_JOINED:
      //Serial.println(F("EV_JOINED"));
      break;
    case EV_RFU1:
      //Serial.println(F("EV_RFU1"));
      break;
    case EV_JOIN_FAILED:
      //Serial.println(F("EV_JOIN_FAILED"));
      break;
    case EV_REJOIN_FAILED:
      //Serial.println(F("EV_REJOIN_FAILED"));
      break;*/
    case EV_TXCOMPLETE:
      //Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
      if (LMIC.txrxFlags & TXRX_ACK)
        //Serial.println(F("Received ack"));
      if (LMIC.dataLen) {
        //Serial.println(F("Received "));
       // Serial.println(LMIC.dataLen);
        //Serial.println(F(" bytes of payload"));
      }
      // Schedule next transmission
      os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
      break;
   /* case EV_LOST_TSYNC:
     // Serial.println(F("EV_LOST_TSYNC"));
      break;
    case EV_RESET:
    //  Serial.println(F("EV_RESET"));
      break;*/
    case EV_RXCOMPLETE:
      // data received in ping slot
    //  Serial.println(F("EV_RXCOMPLETE"));
      break;
  /*  case EV_LINK_DEAD:
    //  Serial.println(F("EV_LINK_DEAD"));
      break;
    case EV_LINK_ALIVE:
     // Serial.println(F("EV_LINK_ALIVE"));
      break;*/
    default:
    //  Serial.println(F("Unknown event"));
      break;
  }
}

void do_send(osjob_t* j) {
int chk = DHT.read22(DHT22_PIN);
  mydata.a = DHT.temperature;
  mydata.b = DHT.humidity;
  Serial.println(DHT.temperature);
  Serial.println(DHT.humidity);
  delay(2000);
  sensor.readData(mydata1);
  if (mydata1.error != 0) {
    //Serial.println("Error!");
  } else {
     
 Serial.print(mydata1.pm25);
    Serial.print(",");
    Serial.println(mydata1.pm10);
  }
  float c=mydata1.pm25;
  float d=mydata1.pm10;
   mydata.e = c;
   mydata.f = d; 
  
   //Serial.println("hello");
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND) {
  //  Serial.println(F("OP_TXRXPEND, not sending"));
  } else {
    // Prepare upstream data transmission at the next possible time.
    LMIC_setTxData2(1, (unsigned char *)&mydata, sizeof(mydata) - 1, 0);
  //  Serial.println(F("Packet queued"));
  }
 
}

void setup() {
  
  sensor.begin();
  Serial.begin(9600);
  
//  Serial.println(F("Starting"));
//  while (!Serial && millis() < 1000000);
  #ifdef VCC_ENABLE
   For Pinoccio Scout boards
  pinMode(VCC_ENABLE, OUTPUT);
  digitalWrite(VCC_ENABLE, HIGH);
  delay(1000);
#endif


 
  // LMIC init
  os_init();
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();

  // Set static session parameters. Instead of dynamically establishing a session
  // by joining the network, precomputed session parameters are be provided.
#ifdef PROGMEM
  // On AVR, these values are stored in flash and only copied to RAM
  // once. Copy them to a temporary buffer here, LMIC_setSession will
  // copy them into a buffer of its own again.
  uint8_t appskey[sizeof(APPSKEY)];
  uint8_t nwkskey[sizeof(NWKSKEY)];
  memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
  memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
  LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
#else
  // If not running an AVR with PROGMEM, just use the arrays directly
  LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
#endif

#if defined(CFG_eu868)
 
  LMIC_setupChannel(0, 865062500, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(1, 865402500, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
  LMIC_setupChannel(2, 865985000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
 
#elif defined(CFG_us915)
 
  LMIC_selectSubBand(1);
#endif

  // Disable link check validation
  LMIC_setLinkCheckMode(0);

  // TTN uses SF9 for its RX2 window.
  LMIC.dn2Dr = DR_SF9;

  // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
  LMIC_setDrTxpow(DR_SF7, 14);

  // Start job
  do_send(&sendjob);
}

void loop() {
  os_runloop_once();
 

}
