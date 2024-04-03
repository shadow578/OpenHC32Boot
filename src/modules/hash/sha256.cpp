#include "../hash.h"

#if METADATA_HASH == HASH_SHA256
  #include "../../util.h"
  #include <algorithm>
  #include <hc32_ddl.h> 
  // note: not using HASH DDL since it doesn't support pushing data in multiple chunks

  // copied from HASH DDL:
  #define HASH_GROUP_LEN (64u)
  #define LAST_GROUP_MAX_LEN (56u)
  // end copied from HASH DDL

  namespace hash
  {
    /**
     * @brief is this the first push in the current session? 
     */
    static bool is_first_push = true;

    /**
     * @brief wait until the hash peripheral is ready again
     */
    void wait_for_ready()
    {
      while(M4_HASH->CR_f.START != 0) { /* nada */ }
    }

    /**
     * @brief push a 512-bit block to the hash peripheral, adding padding as needed
     * @param block the block to push.
     * @param len the length of the block
     * @param scratch a scratch buffer to use. exactly 64 bytes long.
     * 
     * @note expects len to be 64 bytes except for the last block
     */
    void push_block(const uint8_t *block, const uint32_t len, uint8_t *scratch)
    {
      std::fill(scratch, scratch + HASH_GROUP_LEN, 0);

      // copy data to buffer
      std::copy(block, block + len, scratch);

      // add padding if needed
      if (len < HASH_GROUP_LEN)
      {
        scratch[len] = 0x80;
      }

      // wait for hash calculation to finish
      wait_for_ready();

      // write data to hash peripheral
      volatile uint32_t *hash_dr = &M4_HASH->DR15;
      for (uint32_t i = 0; i < HASH_GROUP_LEN / 4; i++)
      {
        const uint32_t j = (i * 4) + 3;

        uint32_t dr = static_cast<uint32_t>(scratch[j]);
        dr |= static_cast<uint32_t>(scratch[j - 1]) << 8;
        dr |= static_cast<uint32_t>(scratch[j - 2]) << 16;
        dr |= static_cast<uint32_t>(scratch[j - 3]) << 24;

        *(hash_dr++) = dr;
      }
      

      // start hash calculation
      M4_HASH->CR_f = {
        .START = 1,
        .FST_GRP = is_first_push ? 1ul : 0ul, // first group?
      };
    }

    bool start()
    {
      // enable peripheral clock
      PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_HASH, Enable);

      // stop any ongoing hash calculation
      M4_HASH->CR = 0;
      is_first_push = true;
      return true;
    }

    bool push_data(const uint8_t *data, const uint32_t len)
    {
      uint8_t scratch[HASH_GROUP_LEN];

      // wait for hash calculation to finish
      wait_for_ready();

      // write data to hash peripheral
      uint32_t remaining_bytes = len;
      while(remaining_bytes > 0)
      {
        uint32_t push_len = minimum(remaining_bytes, HASH_GROUP_LEN);
        push_block(data, push_len, scratch);

        data += HASH_GROUP_LEN;
        remaining_bytes -= push_len;
      }

      // write length to hash peripheral
      uint32_t len_lo = (len >> 29u) & 0x7u;
      uint32_t len_hi = (len << 3u);
      std::fill(scratch, scratch + HASH_GROUP_LEN, 0);
      scratch[63] = static_cast<uint8_t>(len_lo);
      scratch[62] = static_cast<uint8_t>(len_lo >> 8);
      scratch[61] = static_cast<uint8_t>(len_lo >> 16);
      scratch[60] = static_cast<uint8_t>(len_lo >> 24);
      scratch[59] = static_cast<uint8_t>(len_hi);
      scratch[58] = static_cast<uint8_t>(len_hi >> 8);
      scratch[57] = static_cast<uint8_t>(len_hi >> 16);
      scratch[56] = static_cast<uint8_t>(len_hi >> 24);
      push_block(scratch, HASH_GROUP_LEN, scratch);

      // start hash calculation
      M4_HASH->CR_f = {
        .START = 1,
        .FST_GRP = is_first_push ? 1ul : 0ul, // first group?
      };
      return true;
    }

    bool get_hash(hash_t &hash)
    {
      // wait for hash calculation to finish
      wait_for_ready();

      // copy hash to output
      volatile uint32_t *hash_hr = &M4_HASH->HR7;
      uint32_t *hash_out = reinterpret_cast<uint32_t *>(hash);
      for (int i = 0; i < 8; i++)
      {
        hash_out[i] = *hash_hr++;
      }

      return true;
    }
  } // namespace hash

#endif // METADATA_HASH == HASH_SHA256
