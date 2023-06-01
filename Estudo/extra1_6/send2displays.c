void send2displays(unsigned char value){
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00};
    static char flag = 0;
    if(value == NULL){ //what???
        char dig_low = 16;
        char dig_high = 16;
    }else{
        char dig_low = value & 0xF;
        char dig_high = value >> 4;
    }
    char val;
    if(flag == 0){
        LATD = (LATD & 0xFF1F) | 0x0020; // 5 quado é o low
        val = display7Scodes[dig_low];
        LATB = ((LATB & 0x80FF) | (val<<8));
    }
    else{
        LATD = (LATD & 0xFF1F) | 0x0040; // 5 quado é o low
        val = display7Scodes[dig_hig];
        LATB = ((LATB & 0x80FF) | (val<<8));
    }
    flag = !flag;
}