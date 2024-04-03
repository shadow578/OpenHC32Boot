#pragma once
#include <stdint.h>
#include "sd/fatfs/ff.h"
#include "../config.h"

namespace hash
{
  #if METADATA_HASH == HASH_CRC32
    typedef uint32_t hash_t;
  #elif METADATA_HASH == HASH_SHA256
    typedef uint8_t hash_t[32];
  #elif METADATA_HASH == HASH_NONE
    typedef struct {} hash_t; // dummy type
  #else
    #error "Invalid METADATA_HASH"
  #endif

  /**
   * @brief start a new hashing session
   * @return true if the session was successfully started
   */
  bool start();

  /**
   * @brief push data to the current hash
   * @param data the data to push
   * @param len the length of the data
   * @return true if the data was successfully pushed
   * 
   * @note on SHA256, data length must be a multiple of 64 bytes, except for the last push
   */
  bool push_data(const uint8_t *data, const uint32_t len);

  /**
   * @brief get the hash of the current session
   * @param hash the hash of the current session
   * @return true if the hash was successfully retrieved
   * 
   * @note this ends the current session
   */
  bool get_hash(hash_t &hash);
} // namespace hash
