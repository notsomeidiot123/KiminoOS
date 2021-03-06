#define vgaframe_base = 0xa0000
#define vgaframe_limit = 0xbffff

int pitch = 0;
int depth = 0;

char defaultFont[255][10][8] = {
    {{0}}, {{0}}, {{0}}, {{0}}, {{0}}, {{0}}, {{0}}, {{0}},{{0}}, {{0}},{{0}}, {{0}}, {{0}}, {{0}}, {{0}}, {{0}}, {{0}}, {{0}}, {{0}}, {{0}}, {{0}}, {{0}}, {{0}}, {{0}},{{0}}, {{0}}, {{0}}, {{0}},{{0}}, {{0}},{{0}}, {{0}}, {{0}},
    {{0}}, {
        {0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0},
    }, 
    {
        {0, 0, 1, 1, 0, 0, 1, 1},
        {0, 1, 1, 0, 0, 1, 1, 0},
        {1, 1, 0, 0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        {0, 1, 1, 0, 0, 1, 1, 0},
        {0, 1, 1, 0, 0, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 0, 0, 1, 1, 0},
        {0, 1, 1, 0, 0, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 0, 0, 1, 1, 0},
        {0, 1, 1, 0, 0, 1, 1, 0},
    },
    {
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
    },
    {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 1, 0},
        {0, 1, 1, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    }
    
};


void put(int x, int y, char color){
    char * loc  = (char*)0xa0000 + 320 * y + x;
    *loc = color;
}

void testp(char chr, int posX, int posY){  
    // char test[5][4] = {
    //     {1, 0, 0, 1},
    //     {1, 0, 0, 1},
    //     {1, 1, 1, 1},
    //     {1, 0, 0, 1},
    //     {1, 0, 0, 1}
    // };
    for(int x = 0; x < 4*2; x++){
        for(int y = 0; y < 5*2; y++){
            if(defaultFont[chr+1][y][x]){
                put(posX + x, posY + y, 0xf);
           }
       }
   }
}
/*
{0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},*/