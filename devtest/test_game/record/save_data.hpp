#ifndef MGC_SAVE_DATA_HPP
#define MGC_SAVE_DATA_HPP

#include <array>
#include <cstdint>

namespace app {

struct SaveData {
    uint64_t money              : 15;   // 15
    uint64_t life               : 3;    // 18
    uint64_t equipped_weapon    : 3;    // 21
    uint64_t equipped_item      : 4;    // 25
    uint64_t owned_weapon_flags : 4;    // 29
    uint64_t owned_item_flags   : 13;   // 42
    uint64_t checkpoint         : 7;    // 49
    uint64_t location           : 6;    // 55
    uint64_t checksum           : 5;    // 60
    uint64_t                    : 4;    // 64
};

struct EncodedSaveData {
    std::array<uint8_t, 12> data;
};

struct SaveDataEncoder {
    
    static void encode(const SaveData& save_data, EncodedSaveData& out) {
        uint64_t record = 0;
        uint64_t sum = 0;
        record |= pack_field(save_data.money,                0,  15);
        record |= pack_field(save_data.life,                15,   3);
        record |= pack_field(save_data.equipped_weapon,     18,   3);
        record |= pack_field(save_data.equipped_item,       21,   4);
        record |= pack_field(save_data.owned_weapon_flags,  25,   4);
        record |= pack_field(save_data.owned_item_flags,    29,  13);
        record |= pack_field(save_data.checkpoint,          42,   7);
        record |= pack_field(save_data.location,            49,   6);

        for ( size_t n = 0; n < 55; n += 5 ) {
            sum += get_field(record, n, 5);
        }

        record |= pack_field(sum, 55, 5);
        record ^= SECRET_KEY;

        for ( size_t i = 0; i < out.data.size(); i++ ) {
            out.data[i] = CHAR_SET[get_field(record, i*5, 5)];
        }
    }

    static bool decode(const EncodedSaveData& enc_data, SaveData& out) {
        uint64_t record = 0;
        uint64_t checksum = 0;
        uint64_t sum = 0;

        for ( size_t i = 0; i < enc_data.data.size(); ++i ) {
            bool success = false;

            for ( size_t j = 0; j < 32; ++j ) {
                if ( enc_data.data[i] == CHAR_SET[j] ) {
                    success = true;
                    record |= ( j << (i*5) );
                    break;
                }
            }

            if ( !success ) {
                return false;
            }
        }

        record ^= SECRET_KEY;

        for ( size_t n = 0; n < 55; n += 5 ) {
            sum += get_field(record, n, 5);
        }
        sum &= (1ULL<<5)-1;
        checksum = get_field(record, 55, 5);

        if ( sum != checksum ) {
            return false;
        }

        out.money               = get_field(record,   0,  15);
        out.life                = get_field(record,  15,   3);
        out.equipped_weapon     = get_field(record,  18,   3);
        out.equipped_item       = get_field(record,  21,   4);
        out.owned_weapon_flags  = get_field(record,  25,   4);
        out.owned_item_flags    = get_field(record,  29,  13);
        out.checkpoint          = get_field(record,  42,   7);
        out.location            = get_field(record,  49,   6);

        return true;
    }

private:
    static constexpr char CHAR_SET[32+1] = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
    
    static constexpr uint64_t SECRET_KEY = 0x1aeb7f2daf637c8eULL;

    static uint64_t pack_field(uint64_t value, unsigned shift, unsigned bits) {
        return (value & ((1ULL << bits)-1)) << shift;
    }

    static uint64_t get_field(uint64_t record, unsigned shift, unsigned bits) {
        return (record >> shift) & ((1ULL << bits) - 1);
    }
};


}// namespace app

#endif/*MGC_SAVE_DATA_HPP*/

