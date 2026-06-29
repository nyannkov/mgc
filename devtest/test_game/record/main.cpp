#include <stdio.h>
#include "save_data.hpp"


int main(void) {

    app::SaveData save_data {};
    save_data.money              =32767;// 15;   // 15
    save_data.life               =7;// 3;    // 18
    save_data.equipped_weapon    =7;// 3;    // 21
    save_data.equipped_item      =15;// 4;    // 25
    save_data.owned_weapon_flags =15;// 4;    // 29
    save_data.owned_item_flags   =8191;// 13;   // 42
    save_data.checkpoint         =127;// 7;    // 49
    save_data.location           =63;// 6;    // 55
    save_data.checksum           =31;// 5;    // 60
//    save_data.money              =1;// 15;   // 15
//    save_data.life               =2;// 3;    // 18
//    save_data.equipped_weapon    =3;// 3;    // 21
//    save_data.equipped_item      =4;// 4;    // 25
//    save_data.owned_weapon_flags =5;// 4;    // 29
//    save_data.owned_item_flags   =6;// 13;   // 42
//    save_data.checkpoint         =7;// 7;    // 49
//    save_data.location           =8;// 6;    // 55
//    save_data.checksum           =9;// 5;    // 60

    printf("money = %u\n", save_data.money);
    printf("life = %u\n", save_data.life);
    printf("equipped_weapon = %u\n", save_data.equipped_weapon);
    printf("equipped_item = %u\n", save_data.equipped_item);
    printf("owned_weapon_flags = %u\n", save_data.owned_weapon_flags);
    printf("owned_item_flags = %u\n", save_data.owned_item_flags);
    printf("checkpoint = %u\n", save_data.checkpoint);
    printf("location = %u\n", save_data.location);
    printf("checksum = %u\n", save_data.checksum);

    app::EncodedSaveData enc_data;

    app::SaveDataEncoder::encode(save_data, enc_data);
    auto* enc = enc_data.data.data();
    printf("%c%c%c%c-%c%c%c%c-%c%c%c%c\n", 
        enc[0], enc[1], enc[2], enc[3], enc[4], enc[5], enc[6], enc[7], enc[8], enc[9], enc[10], enc[11]);

//    enc[0] = 'A';
    bool r = app::SaveDataEncoder::decode(enc_data, save_data);
    if ( r ) {
        printf("Succeeded.\n");
        printf("money = %u\n", save_data.money);
        printf("life = %u\n", save_data.life);
        printf("equipped_weapon = %u\n", save_data.equipped_weapon);
        printf("equipped_item = %u\n", save_data.equipped_item);
        printf("owned_weapon_flags = %u\n", save_data.owned_weapon_flags);
        printf("owned_item_flags = %u\n", save_data.owned_item_flags);
        printf("checkpoint = %u\n", save_data.checkpoint);
        printf("location = %u\n", save_data.location);
        printf("checksum = %u\n", save_data.checksum);
    } else {
        printf("Failed.\n");
    }

    return 0;
}

