
extern const unsigned char fontdata_6x8[12288] ;

void drawChar(int row, int col, char ch, int size, u16 color);

void drawString(int row, int col, char *str, int size, u16 color);

void drawStringMiddle(int row, char *str, int charLength, int size, u16 color);
