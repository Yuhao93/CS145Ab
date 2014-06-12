/**
 * @file       sdCard.c
 * @brief      read/write in a sdcard through SPI
 *
 * @author     Laurent Saint-Marcel (lstmarcel@yahoo.fr)
 * @date       2009/06/26
 */
 
#include <avr/io.h>
 
#include "sdCard.h"
#include "sdCardProtocol.h"
 
// SPI port PIN configuration
// The following configuration is valid for an ATMEGA324
 
#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_SS      4      // Port B bit 4 (pin5:  chip select for MMC     SS
#define SPI_MOSI    5      // Port B bit 5 (pin6): (data to MMC)         MOSI
#define SPI_MISO    6      // Port B bit 6 (pin7): (data from MMC)       MISO
#define SPI_CSK     7      // Port B bit 7 (pin8): clock                  CSK
 
// ----------------------------------------------------------------------------
// PRIVATE FUNCTIONS
// ----------------------------------------------------------------------------
#define SD_WAIT_TIMEOUT   0xC350 // 16 bits timeout
 
struct {
       uint16_t error;
} SDCARD;
 
// ---------------------------------------------------------------------------
// Set bit in IO port.
// ---------------------------------------------------------------------------
#define sbi(port, bit) (port) |= (1 << (bit))
 
// ---------------------------------------------------------------------------
// Clear bit in IO port.
// ---------------------------------------------------------------------------
#define cbi(port, bit) (port) &= ~(1 << (bit))
 
// ---------------------------------------------------------------------------
// set chip select to low (sdCard is selected is selected)
// ---------------------------------------------------------------------------
void sdSelectChip()
{
       cbi(SPI_PORT, SPI_SS);    
}
 
// ---------------------------------------------------------------------------
// set chip select to low (sdCard is selected is selected)
// ---------------------------------------------------------------------------
void sdDeSelectChip()
{
       sbi(SPI_PORT, SPI_SS);    
}
 
// ---------------------------------------------------------------------------
// set slow clock speed during card initialization
// ---------------------------------------------------------------------------
void sdSetSlowClockSpeed()
{
       // SPI enabled
       // master mode
       // SPIclock = Fclock/64=124kHz (less than 4OOkHz for Multimedia cards)
       SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
       sbi(SPSR, SPI2X);
}
 
// ---------------------------------------------------------------------------
// set fast SPI clock speed for card read/write
// ---------------------------------------------------------------------------
void sdSetFastClockSpeed()
{
       // SPI enabled
       // master mode
       // SPIclock = Fclock/2 = 4MHz (less than 20MHz)
       SPCR = (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (0 << SPR0);
       sbi(SPSR, SPI2X);
}
 
// ---------------------------------------------------------------------------
// Send a char on the SPI and return the answer
// ---------------------------------------------------------------------------
char SPI(char d)
{ 
    // send character over SPI
    SPDR = d;
    // wait answer from SPI
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}
 
// ---------------------------------------------------------------------------
// Wait a specific value from the SD card. Return true if the expected Value
// match, false in the other case
// ---------------------------------------------------------------------------
bool sdWait(char expectedValue)
{
    uint16_t ix;
    char r1 = SPI(0xFF);
    for (ix = 0; ix < SD_WAIT_TIMEOUT; ix++) {
        if (r1 == expectedValue)
            break;
        r1 = SPI(0xFF);
    }
    SDCARD.error = r1;
    return (r1 == expectedValue) ;
}
 
// ---------------------------------------------------------------------------
// Send a command to the SD card
// ---------------------------------------------------------------------------
void sdCommand(char cmd, uint32_t Addr, char crc)
{     
    SPI(0xFF);
    SPI(0x40 | cmd);
    SPI((uint8_t)(Addr >> 24));
    SPI((uint8_t)(Addr >> 16));
    SPI((uint8_t)(Addr >> 8));
    SPI((uint8_t)Addr);
    SPI(crc);
    SPI(0xFF);
}
 
// ---------------------------------------------------------------------------
// Macro to deselect the card and return a boolean
// ---------------------------------------------------------------------------
#define SD_RETURN(VALUE)  \
    sdDeSelectChip();  \
    return VALUE;
 
// ===========================================================================
// PUBLIC FUNCTIONS
// ===========================================================================
 
// ---------------------------------------------------------------------------
//  This function initializes the SPI port
// ---------------------------------------------------------------------------
void sdInit()
{
    SDCARD.error = SD_R1_NOERROR;
    // init spi port
    cbi(SPI_DDR, SPI_MISO);   // set port B SPI data (MISO) input to input
    sbi(SPI_DDR, SPI_CSK);    // set port B SPI clock to output
    sbi(SPI_DDR, SPI_MOSI);       // set port B SPI data out (MOSI)to output
    sbi(SPI_DDR, SPI_SS);  // set port B SPI chip select to output
 
    //sdSetSlowClockSpeed();
    sdDeSelectChip();
}
 
// ---------------------------------------------------------------------------
//  Return the lastest error code
// ---------------------------------------------------------------------------
uint16_t sdGetError()
{
       return SDCARD.error;
}
 
// ---------------------------------------------------------------------------
// Initialize the SD card. Return true if the card is ready
// for read/write
// ---------------------------------------------------------------------------
bool sdInitCard(void)
{
    char i;
    uint16_t ix=0;
    sdSetSlowClockSpeed();
 
    // start SPI mode : 80 clocks pulses while sdcard is not selected
    sdDeSelectChip();
    for(i=0; i < 10; i++) {
        SPI(0xFF); // send 10*8=80 clock pulses
    }
 
    sdSelectChip();
    // reset the card
    sdCommand(SD_CMD0_GO_IDLE_STATE, 0 ,0x95/*CRC7*/ );
    i = SPI(0xFF);
	
    if (i != SD_R1_IDLE) {
        SDCARD.error = i + SD_CARD_ERROR_INIT_1;
        SD_RETURN(false);
    }
 
    // wait the end of reset
    ix=0;
    do {
        sdCommand(SD_CMD1_SEND_OPCOND, 0, 0xFF/*Dummy CRC*/ );
        i = SPI(0xFF);
    } while ((i != SD_R1_NOERROR) && (++ix < SD_WAIT_TIMEOUT));
    if (i != SD_R1_NOERROR) {
        SDCARD.error = i + SD_CARD_ERROR_INIT_2;
        SD_RETURN(false);
    }
 
    // increase clock speed for read/write
    sdSetFastClockSpeed();
    SD_RETURN(true);
}
 
 
// ---------------------------------------------------------------------------
// Write a buffer data at the specified block address
// ---------------------------------------------------------------------------
bool sdWriteBlock(sdCardAddr block,
                  unsigned char* buffer)
{
    uint8_t c;
    uint16_t i;

    sdSelectChip();
    // 512 byte-write-mode
    sdCommand(SD_CMD24_WRITE_BLOCK, block*SD_CARD_BLOCK_SIZE, 0xFF/*dummy crc*/ );
	SDCARD.error = SD_R1_NOERROR;
	if (!sdWait(SD_R1_NOERROR)) {
        SDCARD.error += SD_CARD_ERROR_WRITE_1;
        SD_RETURN(false);
    }
 
    SPI(0xFF);
    SPI(0xFF);
    SPI(SD_START_TOKEN);
    // write ram sectors to SDCard
    for (i=0; i < SD_CARD_BLOCK_SIZE; i++) {
	   SPI(buffer[i]);
    }
    // at the end, send 2 dummy bytes (CRC)
    SPI(0xFF);
    SPI(0xFF);
 
    // get the data response token
    c = SPI(0xFF);
    c &= 0x1F;      // 0x1F = 0b.0001.1111;
    if ((c>>1) != 0x02) { // 0x02=data accepted ; 0x05=CRC error; 0x06=write error
        SDCARD.error = c;
		SDCARD.error += SD_CARD_ERROR_WRITE_2;
        SD_RETURN(false);
    }
      
    // wait until card is not busy anymore
    if (!sdWait(0xFF)) {
        SDCARD.error += SD_CARD_ERROR_WRITE_3;
        SD_RETURN(false);
    }
      
    SD_RETURN(true);
}
 
// ---------------------------------------------------------------------------
// Read data from the SDcard and fill buffer with it
// ---------------------------------------------------------------------------
bool sdReadBlock(sdCardAddr block,
                 unsigned char* buffer)
{
    uint16_t i;
    sdSelectChip();
    // 512 byte-read-mode
    sdCommand(SD_CMD17_READ_BLOCK, block*SD_CARD_BLOCK_SIZE, 0xFF/*dummy CRC*/ );
    if (!sdWait(SD_R1_NOERROR)) {
        SDCARD.error += SD_CARD_ERROR_READ_1;
        SD_RETURN(false);
    }
      
    // wait for 0xFE - start of any transmission
    if (!sdWait(SD_START_TOKEN)) {
        SDCARD.error += SD_CARD_ERROR_READ_2;
        SD_RETURN(false);
    }
      
    // proceed with SDCard-read
    for(i=0; i < SD_CARD_BLOCK_SIZE; i++) {
        buffer[i] = SPI(0xFF);
    }
    // at the end, send 2 dummy bytes
    SPI(0xFF); // actually this returns the CRC/checksum byte
    SPI(0xFF);
      
    // wait until card is not busy anymore
    if (!sdWait(0xFF)) {
        SDCARD.error += SD_CARD_ERROR_WRITE_3;
        SD_RETURN(false);
    }
 
    SD_RETURN(true);
}
 
 
// ===========================================================================
// PUBLIC REGISTER FUNCTIONS
// ===========================================================================
 
#ifdef SD_CARD_REGISTER_COMMANDS
 
// ---------------------------------------------------------------------------
// Retrieve the 'responseLength' bytes from the SD card after having sent
// 'command'
// Output: buffer[responseLength]
// ---------------------------------------------------------------------------
bool sdGetRegister(uint8_t command,
                   unsigned char* buffer,
                  uint8_t responseLength)
{
    uint8_t i;
    sdSelectChip();
 
    // send the command
    sdCommand(command ,0, 0xFF/*dummy CRC*/ );
    if (!sdWait(SD_R1_NOERROR)) {
        SDCARD.error += SD_CARD_ERROR_REGISTER_1;
        SD_RETURN(false);
    }
 
    // wait for 0xFE - start of any transmission
    if (!sdWait(SD_START_TOKEN)) {
        SDCARD.error += SD_CARD_ERROR_REGISTER_2;
        SD_RETURN(false);
    }
 
    for(i=0; i < responseLength; i++) {
        buffer[i] = SPI(0xFF);
    }
 
    // CRC 16
    SPI(0xFF);
    SPI(0xFF);
 
    // wait until card is not busy anymore
    if (!sdWait(0xFF)) {
        SDCARD.error += SD_CARD_ERROR_REGISTER_3;
        SD_RETURN(false);
    }
 
    SD_RETURN(true);
}
 
// ---------------------------------------------------------------------------
// Retrieve the 16 bytes Card Identification register
// ---------------------------------------------------------------------------
bool sdGetCSD(unsigned char* buffer)
{
       return sdGetRegister(SD_CMD9_SEND_CSD, buffer, 16);
}
 
// ---------------------------------------------------------------------------
// Retrieve the 16 bytes Card Identification register
// ---------------------------------------------------------------------------
bool sdGetCID(unsigned char* buffer)
{
       return sdGetRegister(SD_CMD10_SEND_CID, buffer, 16);
}
 
// ---------------------------------------------------------------------------
//  Retrieve the 4 bytes of the OCR register
// ---------------------------------------------------------------------------
bool sdGetOCR(unsigned char* buffer)
{
    uint8_t i, r1;
    sdSelectChip();
   
    sdCommand(SD_CMD58_READ_OCR ,0, 0xFF/*dummy CRC*/ );
    r1 = SPI(0xFF);
 
    for(i=0; i < 4; i++) {
        buffer[i] = SPI(0xFF);
    }
    if ( r1 != SD_R1_NOERROR) {
        SDCARD.error = r1 + SD_CARD_ERROR_OCR_1;
        SD_RETURN(false);
    }
 
    SD_RETURN(true);
}
 
// ---------------------------------------------------------------------------
// Retrieve the 2 bytes of the status register
// ---------------------------------------------------------------------------
bool sdGetStatus(unsigned char* buffer)
{
    sdSelectChip();
    // 512 byte-read-mode
    sdCommand(SD_CMD13_SEND_STATUS ,0, 0xFF/*dummy CRC*/ );
    buffer[0] = SPI(0xFF);
    buffer[1] = SPI(0xFF);
 
    SD_RETURN(true);
}

#endif // SD_CARD_REGISTER_COMMANDS