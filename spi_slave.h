#ifndef SPI_SLAVE_h
#define SPI_SLAVE_h

#include <SPI.h>
#include "Arduino.h"

class SPI_SLAVE
{
  public:
    virtual SPI_SLAVE(const uint8_t sclk = 0,
              const uint8_t mosi = 0,
              const uint8_t miso = 0,
              const uint8_t ss = 0,
              const uint8_t ack = 0);

    void config(const uint8_t mode);

    void handleTick();
    void handleSlaveSelect();

    virtual uint8_t  setMisoData();
    virtual void handleMosiData(const uint8_t b);
    virtual void handleSlaveSelectEnd();
    
  private:
    uint8_t sclk = 0;
    uint8_t mosi = 0;
    uint8_t miso = 0;
    uint8_t ss = 0;
    uint8_t ack = 0;

    uint8_t cpha = 0;
    uint8_t cpol = 0;

    uint8_t tick = 0;

    uint8_t mosiData = 0;
    uint8_t misoData = 0;
    bool doAck;
    bool lastByte;

    void tickFalling();
    void tickRising();
    void tickLeading();
    void tickTrailing();
    void readMOSI();
    void writeMISO();
    void slaveSelectFalling();
    void slaveSelectRising();
};

#endif
