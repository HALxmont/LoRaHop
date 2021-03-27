/*
 * Copyright (c) 2014-2016 IBM Corporation.
 * All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the <organization> nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "lmic.h"
#include "mainApp.h"
#include "debug.h"

//////////////////////////////////////////////////
// CONFIGURATION (FOR APPLICATION CALLBACKS BELOW)
//////////////////////////////////////////////////

// application router ID (LSBF)
static const uint8_t APPEUI[8]  = {}; //{ 0xC1 , 0x67 , 0x01 , 0xD0 , 0x7E , 0xD5 , 0xB3 , 0x70};

// unique device ID (LSBF)
static const uint8_t DEVEUI[8]  ={}; //{ 0x5b , 0xfe , 0xA1 , 0x59 , 0x2f , 0xf1 , 0x56 , 0x00 };

// device-specific AES key (derived from device EUI)
static const uint8_t DEVKEY[16] = {}; //{ 0x02, 0x25, 0xC3, 0x06, 0x01, 0xAF, 0xA9, 0xEC, 0x05, 0x74, 0xA2, 0x66, 0xA5, 0xAA, 0xE3, 0x18 };


// network.
static const uint8_t NWKSKEY[16] = { 0xF7, 0x8B, 0xB3, 0x19, 0xF2, 0x01, 0x6D, 0xB8, 0x37, 0xE4, 0x3C, 0x02, 0xA3, 0xE1, 0x80, 0x87 };

// LoRaWAN AppSKey, application session key
// This is the default Semtech key, which is used by the early prototype TTN
// network.
static const uint8_t APPSKEY[16] = { 0xA2, 0xDB, 0xF0, 0xB1, 0xC9, 0x67, 0x77, 0xDA, 0x2A, 0x84, 0xEC, 0xA2, 0x48, 0x48, 0xDB, 0x3A };

// LoRaWAN end-device address (DevAddr)
static const uint32_t DEVADDR = 0x26041279; // <-- Change this address for every node!

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).


//////////////////////////////////////////////////
// APPLICATION CALLBACKS
//////////////////////////////////////////////////

// provide application router ID (8 bytes, LSBF)
void os_getArtEui (uint8_t* buf) {
    memcpy(buf, APPEUI, 8);
}

// provide device ID (8 bytes, LSBF)
void os_getDevEui (uint8_t* buf) {
    memcpy(buf, DEVEUI, 8);
}

// provide device key (16 bytes)
void os_getDevKey (uint8_t* buf) {
    memcpy(buf, DEVKEY, 16);
}


//////////////////////////////////////////////////
// MAIN - INITIALIZATION AND STARTUP
//////////////////////////////////////////////////

// initial job
static void initfunc (osjob_t* j) {
    // reset MAC state
    LMIC_reset();

    // start joining
    LMIC_startJoining();
    // init done - onEvent() callback will be invoked...
}

static osjob_t blinkjob;
static uint8_t ledstate = 0;

static void blinkfunc (osjob_t* j) {
    // toggle LED
    ledstate = !ledstate;
    leds_set(1,ledstate);
    // reschedule blink job
    os_setTimedCallback(j, os_getTime()+ms2osticks(100), blinkfunc);
}


static uint8_t mydata[] = "20.0080122,73.7919792,15z\n"; //dummy data
static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 5;


void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
       debug_str("OP_TXRXPEND, not sending\n");
    } else {
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
        char buf[100];
        sprintf(buf , "Packet queued txcnt %d\n", LMIC.txCnt);
        debug_str(buf);
    }
    // Next TX is scheduled after TX_COMPLETE event.
}


// application entry point
void mainApp () {
    osjob_t initjob;

    // initialize runtime env
//   os_init();
//    // initialize debug library
//    debug_init();
//    // setup initial job
//    os_setCallback(&initjob, initfunc);
//    // execute scheduled jobs and events
//    do_send(&sendjob);
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
    // Set up the channels used by the Things Network, which corresponds
    // to the defaults of most gateways. Without this, only three base
    // channels from the LoRaWAN specification are used, which certainly
    // works, so it is good for debugging, but can overload those
    // frequencies, so be sure to configure the full frequency range of
    // your network here (unless your network autoconfigures them).
    // Setting up channels should happen after LMIC_setSession, as that
    // configures the minimal channel set.
    // NA-US channels 0-71 are configured automatically
    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
    // TTN defines an additional channel at 869.525Mhz using SF9 for class B
    // devices' ping slots. LMIC does not have an easy way to define set this
    // frequency and support for class B is spotty and untested, so this
    // frequency is not configured here.
    #elif defined(CFG_us915)
    // NA-US channels 0-71 are configured automatically
    // but only one group of 8 should (a subband) should be active
    // TTN recommends the second sub band, 1 in a zero based count.
    // https://github.com/TheThingsNetwork/gateway-conf/blob/master/US-global_conf.json
    LMIC_selectSubBand(1);
    #endif

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF7,14);

    // Start job
    do_send(&sendjob);

    os_runloop();
    // (not reached)
}

//////////////////////////////////////////////////
// LMIC EVENT CALLBACK
//////////////////////////////////////////////////

void onEvent (ev_t ev) {
    debug_event(ev);
    debug_str("\n");
#if 0
    switch(ev) {

    // starting to join network
    case EV_JOINING:
        // start blinking
        blinkfunc(&blinkjob);
        break;
      // network joined, session established
      case EV_JOINED:
          debug_val("netid = ", LMIC.netid);
          //goto tx;

      // scheduled data sent (optionally data received)
      case EV_TXCOMPLETE:
          if(LMIC.dataLen) { // data received in rx slot after tx
              debug_buf(LMIC.frame+LMIC.dataBeg, LMIC.dataLen);
          }
//        tx:
//	  // immediately prepare next transmission
//	  LMIC.frame[0] = LMIC.snr;
//	  // schedule transmission (port 1, datalen 1, no ack requested)
//	  LMIC_setTxData2(1, LMIC.frame, 1, 0);
          // (will be sent as soon as duty cycle permits)
	  os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
	  break;
    }
#else
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            debug_str("EV_SCAN_TIMEOUT\n");
            break;
        case EV_BEACON_FOUND:
            debug_str("EV_BEACON_FOUND\n");
            break;
        case EV_BEACON_MISSED:
            debug_str("EV_BEACON_MISSED\n");
            break;
        case EV_BEACON_TRACKED:
            debug_str("EV_BEACON_TRACKED\n");
            break;
        case EV_JOINING:
            debug_str("EV_JOINING\n");
            break;
        case EV_JOINED:
            debug_str("EV_JOINED\n");
            break;
        case EV_RFU1:
            debug_str("EV_RFU1\n");
            break;
        case EV_JOIN_FAILED:
            debug_str("EV_JOIN_FAILED\n");
            break;
        case EV_REJOIN_FAILED:
            debug_str("EV_REJOIN_FAILED\n");
            break;
        case EV_TXCOMPLETE:
            debug_str("EV_TXCOMPLETE (includes waiting for RX windows)\n");
            if (LMIC.txrxFlags & TXRX_ACK)
              debug_str("Received ack\n");
            if (LMIC.dataLen) {
              debug_str("Received \n");
              debug_int(LMIC.dataLen);
              debug_str(" bytes of payload\n");
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            debug_str("EV_LOST_TSYNC\n");
            break;
        case EV_RESET:
            debug_str("EV_RESET\n");
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            debug_str("EV_RXCOMPLETE\n");
            break;
        case EV_LINK_DEAD:
            debug_str("EV_LINK_DEAD\n");
            break;
        case EV_LINK_ALIVE:
            debug_str("EV_LINK_ALIVE\n");
            break;
         default:
            debug_str("Unknown event\n");
            break;
    }
#endif
}
