#include "../hash.h"
#if METADATA_HASH == HASH_CRC32

  #include <hc32_ddl.h> 
  // note: not using CRC DDL since it doesn't support pushing data in multiple chunks

  // copied from CRC DDL:
  /**
   * Definitions of CRC protocol.
   * NOTE: CRC16 polynomial is X16 + X12 + X5 + 1
   *       CRC32 polynomial is X32 + X26 + X23 + X22 + X16 + X12 + X11 + X10 + \
   *                           X8 + X7 + X5 + X4 + X2 + X + 1
   */
  #define CRC_SEL_16B         ((uint32_t)0x0)
  #define CRC_SEL_32B         ((uint32_t)(0x1ul << 1u))

  /**
   * Identifies the transpose configuration of the source data.
   * If this function is enabled, the source data's bits in bytes are transposed.
   * e.g. There's a source data 0x1234 which will be calculated checksum and this
   * function is enabled, the final data be calculated is 0x482C.
   * 0x12: bit0->bit7, bit1->bit6, ..., bit7->bit0, the data byte changed to 0x48.
   * 0x48: bit0->bit7, bit1->bit6, ..., bit7->bit0, the data byte changed to 0x2C.
   * The same to 32 bit data while using CRC32.
   */
  #define CRC_REFIN_DISABLE   ((uint32_t)0x0)
  #define CRC_REFIN_ENABLE    ((uint32_t)(0x1ul << 2u))

  /**
   * Identifies the transpose configuration of the checksum.
   * If this function is enabled, bits of the checksum will be transposed.
   * e.g. There is a CRC16 checksum is 0x5678 before this function enabled, then
   * this function is enabled, the checksum will be 0x1E6A.
   * 0x5678: bit0->bit15, bit1->bit14, ..., bit15->bit0, the final data is 0x1E6A.
   * The same to CRC32 checksum while using CRC32.
   */
  #define CRC_REFOUT_DISABLE  ((uint32_t)0x0)
  #define CRC_REFOUT_ENABLE   ((uint32_t)(0x1ul << 3u))

  /**
   * XORs the CRC checksum with 0xFFFF(CRC16) or 0xFFFFFFFF(CRC32).
   * e.g. There is a CRC16 checksum is 0x5678 before this function enabled.
   * If this function enabled, the checksum will be 0xA987.
   * The same to CRC32 checksum while using CRC32.
   */
  #define CRC_XOROUT_DISABLE  ((uint32_t)0x0)
  #define CRC_XOROUT_ENABLE   ((uint32_t)(0x1ul << 4u))

  #define CRC_CONFIG_MASK     ((uint32_t)(0x1Eu))
  // end copied from CRC DDL

  namespace hash
  {
    bool start()
    {
      // enable clock of CRC peripheral
      PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_CRC, Enable);

      // configure CRC peripheral
      const uint32_t config = CRC_SEL_32B | CRC_REFIN_ENABLE | CRC_REFOUT_ENABLE | CRC_XOROUT_DISABLE;
      M4_CRC->CR = config & CRC_CONFIG_MASK;

      // reset CRC
      M4_CRC->RESLT = 0xFFFFFFFF;

      return true;
    }

    bool push_data(const uint8_t *data, const uint32_t len)
    {
      for (uint32_t i = 0; i < len; i++)
      {
        M4_CRC->DAT0 = data[i];
      }

      return true;
    }

    bool get_hash(hash_t &hash)
    {
      hash = M4_CRC->RESLT;
      return true;
    }
  } // namespace hash

#endif // METADATA_HASH == HASH_CRC32
