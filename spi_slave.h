#ifndef SPI_SLAVE_h
#define SPI_SLAVE_h

#include "Arduino.h"

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

typedef uint8_t (*SetMisoData)();
typedef void (*HandleMosiData)(const uint8_t b);
typedef void (*HandleSlaveSelectEnd)();

class SPI_SLAVE
{
  public:
    SPI_SLAVE(uint8_t sclk = 0,
              uint8_t mosi = 0,
              uint8_t miso = 0,
              uint8_t ss = 0,
              uint8_t ack = 0);

    void config(uint8_t mode,
                SetMisoData setMisoData,
                HandleMosiData handleMosiData,
                HandleSlaveSelectEnd handleSlaveSelectEnd);

    void handleTick();
    void handleSlaveSelect();

  private:
    uint8_t sclk = 0;
    uint8_t ss = 0;
    uint8_t mosi = 0;
    uint8_t miso = 0;
    uint8_t ack = 0;

    uint8_t cpha = 0;
    uint8_t cpol = 0;

    uint8_t tick = 0;

    uint8_t mosiData = 0;
    uint8_t misoData = 0;
    bool lastByte;

    HandleMosiData handleMosiData;
    SetMisoData setMisoData;
    HandleSlaveSelectEnd handleSlaveSelectEnd;

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
