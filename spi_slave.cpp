#include "spi_slave.h"

SPI_SLAVE::SPI_SLAVE(uint8_t sclk = 0,
                     uint8_t mosi = 0,
                     uint8_t miso = 0,
                     uint8_t ss = 0,
                     uint8_t ack = 0) {

  // set clock, atten, and command as inputs
  pinMode(sclk, INPUT_PULLUP);
  pinMode(mosi, INPUT_PULLUP);
  pinMode(ss, INPUT_PULLUP);

  // set data and ack as outputs
  pinMode(miso, OUTPUT);
  pinMode(ack, OUTPUT);
}

void SPI_SLAVE::config(uint8_t mode,
                       SetMisoData setMisoData,
                       HandleMosiData handleMosiData,
                       HandleSlaveSelectEnd handleSlaveSelectEnd) {

  this->setMisoData = setMisoData;
  this->handleMosiData = handleMosiData;
  this->handleSlaveSelectEnd = handleSlaveSelectEnd;

  switch (mode) {
    case SPI_MODE0:
      cpha = 0;
      cpol = 0;
      break;
    case SPI_MODE1:
      cpha = 1;
      cpol = 0;
      break;
    case SPI_MODE2:
      cpha = 0;
      cpol = 1;
      break;
    case SPI_MODE3:
      cpha = 1;
      cpol = 1;
      break;
  }

}

void SPI_SLAVE::handleTick() {
  digitalRead(sclk) == HIGH ? tickRising() : tickFalling();
}

void SPI_SLAVE::handleSlaveSelect() {
  digitalRead(ss) == HIGH ? slaveSelectRising() : slaveSelectFalling();
}

void SPI_SLAVE::tickRising() {
  // if slave select is low do stuff
  if (!digitalRead(ss)) {
    switch (cpol) {
      // CPOL=0, rising is leading edge
      case 0:
        tickLeading();
        break;
      // CPOL=1, rising is trailing edge
      case 1:
        tickTrailing();
        break;
    }
  }
}

void SPI_SLAVE::tickFalling() {
  // if slave select is low do stuff
  if (!digitalRead(ss)) {
    switch (cpol) {
      // CPOL=0, falling is trailing edge
      case 0:
        tickTrailing();
        break;
      // CPOL=1, falling is leading edge
      case 1:
        tickLeading();
        break;
    }
  }
}

void SPI_SLAVE::tickLeading() {
  switch (cpha) {
    // CPHA=0, in side captures data on the leading edge of clock
    case 0:
      readMOSI();
      break;
    // CPHA=1, out side changes data on leading edge of clock
    case 1:
      writeMISO();
      break;
  }
}

void SPI_SLAVE::tickTrailing() {
  switch (cpha) {
    // CPHA=0, out side changes data on trailing edge of preceding clock
    case 0:
      writeMISO();
      break;
    // CPHA=1, in side captures data on the trailing edge of clock
    case 1:
      readMOSI();
      break;
  }
  tick++;
}

void SPI_SLAVE::readMOSI() {
  uint8_t bit = tick % 8;
  mosiData |= (digitalRead(mosi) << bit);
  if (tick > 0 && bit == 0) {
    handleMosiData(mosiData);
  }
}

void SPI_SLAVE::writeMISO() {
  uint8_t bit = tick % 8;
  if (bit == 0) {
    misoData = setMisoData();
  }
  digitalWrite(miso, ((misoData & (1 << bit)) >> bit));
}

void SPI_SLAVE::slaveSelectRising() {
  tick = 0;
  mosiData = 0;
  misoData = 0;
  
  handleSlaveSelectEnd();
}

void SPI_SLAVE::slaveSelectFalling() {
  // slave select always signals by going high to low
}
