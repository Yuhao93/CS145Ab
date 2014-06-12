/**
 * @file       sdCard.h
 * @brief      read/write in a sdcard through SPI port of an atmel 8bits microcontroller
 *
 * @author     Laurent Saint-Marcel (lstmarcel@yahoo.fr)
 * @date       2009/06/27
 */
 
#ifndef __SDCARD_H__
#define __SDCARD_H__
 
// command the following lines if you just want read/write sdCard functions (to save program disk space)
#define SD_CARD_REGISTER_COMMANDS
#include "util.h"
 
#define SD_CARD_BLOCK_SIZE  512 // a block size of the SDCard. Always 512 bytes
#define SD_CARD_MAX_BLOCK   0x300000 // maximum number of blocks for a 2Go SDCard = Capacity / SD_CARD_BLOCK_SIZE = 0x400000
 
typedef uint32_t sdCardAddr;
 
/**
 * @brief This function initializes the sdCard capture module (setup atmel SPI port)
 */
void sdInit();
 
/**
 * @brief This function returns the SD error code after a sdXXX function return false
 */
uint16_t sdGetError();
 
/**
 * @brief Detect and initialize the SDcard
 *
 * This function detects and initializes the SDcard
 * @return true  if the card is detected
 * @return false if the card is not detected. Use sdGetError to get the error code.
 */
bool sdInitCard();
 
/**
 * @brief Writes SD_CARD_BLOCK_SIZE bytes in the sdcard at the specified blockId
 *
 * This function writes SD_CARD_BLOCK_SIZE bytes in the sdcard at the
 * specified blockId. At 8MHz it takes around 5.3ms ~= 85ko/s
 *
 * @param blockId the number of the block to write:
 *                0, 1, ... cardCapacity/SD_CARD_BLOCK_SIZE
 * @param buffer an array of at least SD_CARD_BLOCK_SIZE bytes containing the
 *               data to write in the sdCard
 * @return true  if the block has been written in the sdCard
 * @return false if the block has not been written completely. Use sdGetError to get the error code.
 */
bool sdWriteBlock(sdCardAddr blockId,
                  unsigned char* buffer);
 
/**
 * @brief Read SD_CARD_BLOCK_SIZE bytes from the sdcard at the specified blockId
 *
 * This function reads SD_CARD_BLOCK_SIZE bytes in the sdcard at the
 * specified blockId. At 8MHz it takes around 2.8ms
 *
 * @param blockId the number of the block to read: 0, 1, ... cardCapacity/SD_CARD_BLOCK_SIZE
 * @param buffer an array of at least SD_CARD_BLOCK_SIZE bytes containing the
 *               data to write in the sdCard
 * @return true  if the block has been read from the sdCard
 * @return false if the block has not been read completely. Use sdGetError to get the error code.
 */
bool sdReadBlock(sdCardAddr blockId,
                 unsigned char* buffer);
 
// ===========================================================================
// Functions to get sd card registers
// ===========================================================================
 
#ifdef SD_CARD_REGISTER_COMMANDS
 
/**
 * @brief Retrieve the 16 bytes Card Specific Data register
 *
 * This function retrieves the 16 bytes Card Specific Data register
 *
 * @param buffer an array of at least 16 bytes that will be fill with the
 *               register data
 * @return true on success, false in the other case
 */
bool sdGetCSD(unsigned char* buffer);
 
/**
 * @brief Retrieve the 16 bytes Card Identification register
 *
 * This function retrieves the 16 bytes Card Identification register
 *
 * @param buffer an array of at least 16 bytes that will be fill with the
 *               register data
 * @return true on success, false in the other case
 */
bool sdGetCID(unsigned char* buffer);
 
/**
 * @brief Retrieve the 4 bytes Operating Conditions Register
 *
 * This function retrieves the 4 bytes Card Identification register
 *
 * @param buffer an array of at least 4 bytes that will be fill with the
 *               register data
 * @return true on success, false in the other case
 */
bool sdGetOCR(unsigned char* buffer);
 
/**
 * @brief Retrieve the 2 bytes Status Register
 *
 * This function retrieves the 2 bytes Card Identification register
 *
 * @param buffer an array of at least 2 bytes that will be fill with the
 *               register data
 * @return true on success, false in the other case
 */
bool sdGetStatus(unsigned char* buffer);
 
#endif // SD_CARD_REGISTER_COMMANDS
 
#endif
 