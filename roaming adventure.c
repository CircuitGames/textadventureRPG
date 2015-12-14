
//This is a simple adventure/RPG style game reminiscent of text based  RPGs..
//Developed by Alexander Logan Evans
//created on: 10/15/2015 11:30 AM
//last updated on: 10/15/2015

/**To make this continue to run on make the ./ current directory syntax must be modified for mac.
 *The default is ./ for windows.  This program is written so a find and replace of ./ should only change
 *these curent directory syntax entries.
 */

 //libraries
#include <stdio.h>      //used for i/o
#include <stdlib.h>     //used for clearing the screen
#include <time.h>       //used to get time for random number generator

//check OS and include necessary libraries
#ifdef _WIN32
    //code for Windows (32-bit and 64-bit, this part is common)
    #include <conio.h>
    #define CLEARSCREEN system("cls")
    #define CHECKKEY _kbhit()
    #define NBGETCHAR getch()

#elif __APPLE__
    //code for mac
    #define CLEARSCREEN system("clear")
    #define CHECKKEY
    #define NBGETCHAR

#elif __linux__
    //code for linux
    #define CLEARSCREEN system("clear")
    #define CHECKKEY
    #define NBGETCHAR

#else
#   error "Unknown compiler"
#endif


#define USRINPUT 500 //this is the limit on a user input string max length

//invalid data, either correct, undo, initiate a failsafe/fall-back
#define INVALID 0

///only 0-9 are valid for this input
//land types
#define FOREST              'T'   //T
#define PATH                'O'   //O
#define DESSERT             ':'   //:
#define WATER               '^'   //^
#define PLAINS              'q'   //q
#define SPECIAL             'X'   //X
#define MOUNTAINS           'A'   //A

//towns & villages & cities & kingdoms
#define MYSTOGEN            1
#define LOST_CITY           2
#define EMERALD_KINGDOM     3

//dungeons
///These should not use the same values as the above section on towns etc..
#define CAVE                100
#define DARK_FOREST         101

//map IDs
#define MAINMAP             1

//dimensions
///this must also be changed in the MAPSTRINGS' definition.
///make sure the '\n' at the end of the line is included in the x coordinate.
///total X should not exceed 120 to fit properly on a default size command line console.
//mainMap
#define MMX 120
#define MMY 27

//map strings
///if this code is changed the dimensions probably also need to be altered to reflect changes
#define MAPSTRINGS char mainMap[120][27]
//, int cave1[], int cave2[], int temple[], int waterPalace[]
#define MAPSTRINGA mainMap
//, cave1, cave2, temple, waterPalace
#define UV name
//, name, boat, girlfriend

//global variables
int debug = 0;
int detaildebug = 0;
int noDelay=0;

void title();
void loadMaps(MAPSTRINGS);
void printmap(int xp, int yp, int MapID, MAPSTRINGS);
int mainMenu();
int readMenu(char x[]);
int compareStrings(char string1[],char string2[]);
void CaseClear(char stringstorage[]);
void GetInput(int x, char arrays[]);
void disMainMenue();
void SpecialClear(char stringstorage[]);
void AllClear(char stringstorage[]);
void newgame(char name[]);
void delay(double x);
void sptype(char x[]);
void chat(char c[], char d[],char name[]);
void title();
int walk(int MapID,int* x_Pos,int* y_Pos,char journey[], MAPSTRINGS, char name[]);
void insert(char x[],char y[],char name[]);
int stbreak(int s, int e, char x[], char y[]);
int checkL(int MapID,int x,int y,char var[],char journey[], MAPSTRINGS);
void CStr(char str1[],char str2[]);

int main()
{
    //initialize variables
    int x_Pos = 0, y_Pos = 0, location = 0, MapID = 0, playerPath = 1, dimx = 0, dimy = 0, mstate = 0;
    // CMap[] = {'\0'}, cave1[], cave2[], temple[], waterPalace[];
    ///journey variables {'boat', 'mountaineering kit', '\0'}
    char  name[100] = {'\0'}, journey[50] = {'\0'}, mainMap[MMX][MMY], CMap[150*200] = {'\0'};

    ///broken, need to fix with kbhit() alternative
    title();

    mstate=mainMenu();
    switch (mstate)
    {
        case 1:
            newgame(name);
            MapID=1;
            x_Pos=y_Pos=0;
            journey[1]='0';
            journey[2]='0';
            playerPath=0;
            break;
        case 2:
            sptype("not implemented yet\n");
            loadMaps(MAPSTRINGA);
            MapID=1;
            x_Pos=y_Pos=0;
            journey[1]='0';
            journey[2]='0';
            playerPath=0;
            break;
        case 3:
            sptype("not implemented yet");
            break;
        case 4:
            sptype("exiting...");
            break;
    }
    loadMaps(mainMap);

    if (mstate==1||mstate==2)
    {
        do
        {    if (debug==1)
            printf("\nmain loop\n");

            //setCurrentMap(&MapID, CMap, &dimx, &dimy, MAPSTRINGA);
            printmap(x_Pos, y_Pos, MapID, MAPSTRINGA);
            playerPath=walk(MapID, &x_Pos, &y_Pos, journey, MAPSTRINGA, UV);

        }while(playerPath==0);
    }
    return 0;
}

void loadMaps(MAPSTRINGS)
{
//    if main maps are detected, proceed.  otherwise escape with error:
//              "The map data for this program is missing, incorrectly named, or corrupted.
//              Please include these missing files:"
    if (debug==1)
        printf("\nloadMaps\n");


    int y = 0;
    int x = 0;

    //open file
    FILE* fp;

    fp=fopen("./data/maps/MainMap.dat", "r");

    //check if the file opened correctly; if not throw error and exit
    if (!fp)
    {
        printf("ERROR!  Couldn't open MainMap.dat!");
        exit(101);
    }

    if (debug == 1)
    printf("\nconnected\n");

    //begin looping through reading the map data
    for(y=0;y<MMY;y++)
    {
        for(x=0;x<MMX;x++)
        {
            if (detaildebug == 1)
                printf("\nloop %d %d\n", x, y);
            fscanf(fp,"%c", &mainMap[x][y]);
        }
    }

    //close file
    fclose(fp);

    /*
    fp=fopen("./data/maps/MainMap.dat", "r");

    //check if the file opened correctly; if not throw error and exit
    if (fp == 1)
    {
        printf("ERROR!  couldn't open MainMap.dat!");
        exit(101);
    }

    //begin looping through reading the map data
    for(y=0;y<MMY;y++)
    {
        for(x=0;x<MMX;x++)
        {
            fscanf(fp,"%d", mainMap [x][y]);
        }
    }

    //close file
    fclose(fp);*/

    return;
}
/*
void setCurrentMap(int* MapID, int CMap[],int* dimx, int* dimy, MAPSTRINGS)
{
    int x = 0;
    int y = 0;
    char temp;

    if (MapID = MAINMAP)
    {
        *dimx = MMX;
        *dimy = MMY;

        for(y=0;y<MMY;y++)
        {
            for(x=0;x<MMX;x++)
            {
                temp = mainMap[x][y];
                CMap[x][y] = temp;
            }
        }
    }
    //else if (MapID =
    //{
    //
    //}
    return;
}*/



void printmap(int xp, int yp, int MapID, MAPSTRINGS)
{
    char hold=mainMap[xp][yp];
    mainMap[xp][yp]='@';
    if (debug == 0)
        system("cls");
    if (debug==1)
        printf("\nprintmap\n");

    int x = 0;
    int y = 0;

    if (MapID == MAINMAP)
    {
        for(x=0;x<MMX-1;x++)
            printf("=");
            printf("\n");

        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", mainMap[0][0], mainMap[1][0], mainMap[2][0], mainMap[3][0], mainMap[4][0], mainMap[5][0], mainMap[6][0], mainMap[7][0], mainMap[8][0], mainMap[9][0], mainMap[10][0], mainMap[11][0], mainMap[12][0], mainMap[13][0], mainMap[14][0], mainMap[15][0], mainMap[16][0], mainMap[17][0], mainMap[18][0], mainMap[19][0], mainMap[20][0], mainMap[21][0], mainMap[22][0], mainMap[23][0], mainMap[24][0], mainMap[25][0], mainMap[26][0], mainMap[27][0], mainMap[28][0], mainMap[29][0], mainMap[30][0], mainMap[31][0], mainMap[32][0], mainMap[33][0], mainMap[34][0], mainMap[35][0], mainMap[36][0], mainMap[37][0], mainMap[38][0], mainMap[39][0], mainMap[40][0], mainMap[41][0], mainMap[42][0], mainMap[43][0], mainMap[44][0], mainMap[45][0], mainMap[46][0], mainMap[47][0], mainMap[48][0], mainMap[49][0], mainMap[50][0], mainMap[51][0], mainMap[52][0], mainMap[53][0], mainMap[54][0], mainMap[55][0], mainMap[56][0], mainMap[57][0], mainMap[58][0], mainMap[59][0], mainMap[60][0], mainMap[61][0], mainMap[62][0], mainMap[63][0], mainMap[64][0], mainMap[65][0], mainMap[66][0], mainMap[67][0], mainMap[68][0], mainMap[69][0], mainMap[70][0], mainMap[71][0], mainMap[72][0], mainMap[73][0], mainMap[74][0], mainMap[75][0], mainMap[76][0], mainMap[77][0], mainMap[78][0], mainMap[79][0], mainMap[80][0], mainMap[81][0], mainMap[82][0], mainMap[83][0], mainMap[84][0], mainMap[85][0], mainMap[86][0], mainMap[87][0], mainMap[88][0], mainMap[89][0], mainMap[90][0], mainMap[91][0], mainMap[92][0], mainMap[93][0], mainMap[94][0], mainMap[95][0], mainMap[96][0], mainMap[97][0], mainMap[98][0], mainMap[99][0], mainMap[100][0], mainMap[101][0], mainMap[102][0], mainMap[103][0], mainMap[104][0], mainMap[105][0], mainMap[106][0], mainMap[107][0], mainMap[108][0], mainMap[109][0], mainMap[110][0], mainMap[111][0], mainMap[112][0], mainMap[113][0], mainMap[114][0], mainMap[115][0], mainMap[116][0], mainMap[117][0], mainMap[118][0], mainMap[119][0], mainMap[0][1], mainMap[1][1], mainMap[2][1], mainMap[3][1], mainMap[4][1], mainMap[5][1], mainMap[6][1], mainMap[7][1], mainMap[8][1], mainMap[9][1], mainMap[10][1], mainMap[11][1], mainMap[12][1], mainMap[13][1], mainMap[14][1], mainMap[15][1], mainMap[16][1], mainMap[17][1], mainMap[18][1], mainMap[19][1], mainMap[20][1], mainMap[21][1], mainMap[22][1], mainMap[23][1], mainMap[24][1], mainMap[25][1], mainMap[26][1], mainMap[27][1], mainMap[28][1], mainMap[29][1], mainMap[30][1], mainMap[31][1], mainMap[32][1], mainMap[33][1], mainMap[34][1], mainMap[35][1], mainMap[36][1], mainMap[37][1], mainMap[38][1], mainMap[39][1], mainMap[40][1], mainMap[41][1], mainMap[42][1], mainMap[43][1], mainMap[44][1], mainMap[45][1], mainMap[46][1], mainMap[47][1], mainMap[48][1], mainMap[49][1], mainMap[50][1], mainMap[51][1], mainMap[52][1], mainMap[53][1], mainMap[54][1], mainMap[55][1], mainMap[56][1], mainMap[57][1], mainMap[58][1], mainMap[59][1], mainMap[60][1], mainMap[61][1], mainMap[62][1], mainMap[63][1], mainMap[64][1], mainMap[65][1], mainMap[66][1], mainMap[67][1], mainMap[68][1], mainMap[69][1], mainMap[70][1], mainMap[71][1], mainMap[72][1], mainMap[73][1], mainMap[74][1], mainMap[75][1], mainMap[76][1], mainMap[77][1], mainMap[78][1], mainMap[79][1], mainMap[80][1], mainMap[81][1], mainMap[82][1], mainMap[83][1], mainMap[84][1], mainMap[85][1], mainMap[86][1], mainMap[87][1], mainMap[88][1], mainMap[89][1], mainMap[90][1], mainMap[91][1], mainMap[92][1], mainMap[93][1], mainMap[94][1], mainMap[95][1], mainMap[96][1], mainMap[97][1], mainMap[98][1], mainMap[99][1], mainMap[100][1], mainMap[101][1], mainMap[102][1], mainMap[103][1], mainMap[104][1], mainMap[105][1], mainMap[106][1], mainMap[107][1], mainMap[108][1], mainMap[109][1], mainMap[110][1], mainMap[111][1], mainMap[112][1], mainMap[113][1], mainMap[114][1], mainMap[115][1], mainMap[116][1], mainMap[117][1], mainMap[118][1], mainMap[119][1], mainMap[0][2], mainMap[1][2], mainMap[2][2], mainMap[3][2], mainMap[4][2], mainMap[5][2], mainMap[6][2], mainMap[7][2], mainMap[8][2], mainMap[9][2], mainMap[10][2], mainMap[11][2], mainMap[12][2], mainMap[13][2], mainMap[14][2], mainMap[15][2], mainMap[16][2], mainMap[17][2], mainMap[18][2], mainMap[19][2], mainMap[20][2], mainMap[21][2], mainMap[22][2], mainMap[23][2], mainMap[24][2], mainMap[25][2], mainMap[26][2], mainMap[27][2], mainMap[28][2], mainMap[29][2], mainMap[30][2], mainMap[31][2], mainMap[32][2], mainMap[33][2], mainMap[34][2], mainMap[35][2], mainMap[36][2], mainMap[37][2], mainMap[38][2], mainMap[39][2], mainMap[40][2], mainMap[41][2], mainMap[42][2], mainMap[43][2], mainMap[44][2], mainMap[45][2], mainMap[46][2], mainMap[47][2], mainMap[48][2], mainMap[49][2], mainMap[50][2], mainMap[51][2], mainMap[52][2], mainMap[53][2], mainMap[54][2], mainMap[55][2], mainMap[56][2], mainMap[57][2], mainMap[58][2], mainMap[59][2], mainMap[60][2], mainMap[61][2], mainMap[62][2], mainMap[63][2], mainMap[64][2], mainMap[65][2], mainMap[66][2], mainMap[67][2], mainMap[68][2], mainMap[69][2], mainMap[70][2], mainMap[71][2], mainMap[72][2], mainMap[73][2], mainMap[74][2], mainMap[75][2], mainMap[76][2], mainMap[77][2], mainMap[78][2], mainMap[79][2], mainMap[80][2], mainMap[81][2], mainMap[82][2], mainMap[83][2], mainMap[84][2], mainMap[85][2], mainMap[86][2], mainMap[87][2], mainMap[88][2], mainMap[89][2], mainMap[90][2], mainMap[91][2], mainMap[92][2], mainMap[93][2], mainMap[94][2], mainMap[95][2], mainMap[96][2], mainMap[97][2], mainMap[98][2], mainMap[99][2], mainMap[100][2], mainMap[101][2], mainMap[102][2], mainMap[103][2], mainMap[104][2], mainMap[105][2], mainMap[106][2], mainMap[107][2], mainMap[108][2], mainMap[109][2], mainMap[110][2], mainMap[111][2], mainMap[112][2], mainMap[113][2], mainMap[114][2], mainMap[115][2], mainMap[116][2], mainMap[117][2], mainMap[118][2], mainMap[119][2], mainMap[0][3], mainMap[1][3], mainMap[2][3], mainMap[3][3], mainMap[4][3], mainMap[5][3], mainMap[6][3], mainMap[7][3], mainMap[8][3], mainMap[9][3], mainMap[10][3], mainMap[11][3], mainMap[12][3], mainMap[13][3], mainMap[14][3], mainMap[15][3], mainMap[16][3], mainMap[17][3], mainMap[18][3], mainMap[19][3], mainMap[20][3], mainMap[21][3], mainMap[22][3], mainMap[23][3], mainMap[24][3], mainMap[25][3], mainMap[26][3], mainMap[27][3], mainMap[28][3], mainMap[29][3], mainMap[30][3], mainMap[31][3], mainMap[32][3], mainMap[33][3], mainMap[34][3], mainMap[35][3], mainMap[36][3], mainMap[37][3], mainMap[38][3], mainMap[39][3], mainMap[40][3], mainMap[41][3], mainMap[42][3], mainMap[43][3], mainMap[44][3], mainMap[45][3], mainMap[46][3], mainMap[47][3], mainMap[48][3], mainMap[49][3], mainMap[50][3], mainMap[51][3], mainMap[52][3], mainMap[53][3], mainMap[54][3], mainMap[55][3], mainMap[56][3], mainMap[57][3], mainMap[58][3], mainMap[59][3], mainMap[60][3], mainMap[61][3], mainMap[62][3], mainMap[63][3], mainMap[64][3], mainMap[65][3], mainMap[66][3], mainMap[67][3], mainMap[68][3], mainMap[69][3], mainMap[70][3], mainMap[71][3], mainMap[72][3], mainMap[73][3], mainMap[74][3], mainMap[75][3], mainMap[76][3], mainMap[77][3], mainMap[78][3], mainMap[79][3], mainMap[80][3], mainMap[81][3], mainMap[82][3], mainMap[83][3], mainMap[84][3], mainMap[85][3], mainMap[86][3], mainMap[87][3], mainMap[88][3], mainMap[89][3], mainMap[90][3], mainMap[91][3], mainMap[92][3], mainMap[93][3], mainMap[94][3], mainMap[95][3], mainMap[96][3], mainMap[97][3], mainMap[98][3], mainMap[99][3], mainMap[100][3], mainMap[101][3], mainMap[102][3], mainMap[103][3], mainMap[104][3], mainMap[105][3], mainMap[106][3], mainMap[107][3], mainMap[108][3], mainMap[109][3], mainMap[110][3], mainMap[111][3], mainMap[112][3], mainMap[113][3], mainMap[114][3], mainMap[115][3], mainMap[116][3], mainMap[117][3], mainMap[118][3], mainMap[119][3], mainMap[0][4], mainMap[1][4], mainMap[2][4], mainMap[3][4], mainMap[4][4], mainMap[5][4], mainMap[6][4], mainMap[7][4], mainMap[8][4], mainMap[9][4], mainMap[10][4], mainMap[11][4], mainMap[12][4], mainMap[13][4], mainMap[14][4], mainMap[15][4], mainMap[16][4], mainMap[17][4], mainMap[18][4], mainMap[19][4], mainMap[20][4], mainMap[21][4], mainMap[22][4], mainMap[23][4], mainMap[24][4], mainMap[25][4], mainMap[26][4], mainMap[27][4], mainMap[28][4], mainMap[29][4], mainMap[30][4], mainMap[31][4], mainMap[32][4], mainMap[33][4], mainMap[34][4], mainMap[35][4], mainMap[36][4], mainMap[37][4], mainMap[38][4], mainMap[39][4], mainMap[40][4], mainMap[41][4], mainMap[42][4], mainMap[43][4], mainMap[44][4], mainMap[45][4], mainMap[46][4], mainMap[47][4], mainMap[48][4], mainMap[49][4], mainMap[50][4], mainMap[51][4], mainMap[52][4], mainMap[53][4], mainMap[54][4], mainMap[55][4], mainMap[56][4], mainMap[57][4], mainMap[58][4], mainMap[59][4], mainMap[60][4], mainMap[61][4], mainMap[62][4], mainMap[63][4], mainMap[64][4], mainMap[65][4], mainMap[66][4], mainMap[67][4], mainMap[68][4], mainMap[69][4], mainMap[70][4], mainMap[71][4], mainMap[72][4], mainMap[73][4], mainMap[74][4], mainMap[75][4], mainMap[76][4], mainMap[77][4], mainMap[78][4], mainMap[79][4], mainMap[80][4], mainMap[81][4], mainMap[82][4], mainMap[83][4], mainMap[84][4], mainMap[85][4], mainMap[86][4], mainMap[87][4], mainMap[88][4], mainMap[89][4], mainMap[90][4], mainMap[91][4], mainMap[92][4], mainMap[93][4], mainMap[94][4], mainMap[95][4], mainMap[96][4], mainMap[97][4], mainMap[98][4], mainMap[99][4], mainMap[100][4], mainMap[101][4], mainMap[102][4], mainMap[103][4], mainMap[104][4], mainMap[105][4], mainMap[106][4], mainMap[107][4], mainMap[108][4], mainMap[109][4], mainMap[110][4], mainMap[111][4], mainMap[112][4], mainMap[113][4], mainMap[114][4], mainMap[115][4], mainMap[116][4], mainMap[117][4], mainMap[118][4], mainMap[119][4], mainMap[0][5], mainMap[1][5], mainMap[2][5], mainMap[3][5], mainMap[4][5], mainMap[5][5], mainMap[6][5], mainMap[7][5], mainMap[8][5], mainMap[9][5], mainMap[10][5], mainMap[11][5], mainMap[12][5], mainMap[13][5], mainMap[14][5], mainMap[15][5], mainMap[16][5], mainMap[17][5], mainMap[18][5], mainMap[19][5], mainMap[20][5], mainMap[21][5], mainMap[22][5], mainMap[23][5], mainMap[24][5], mainMap[25][5], mainMap[26][5], mainMap[27][5], mainMap[28][5], mainMap[29][5], mainMap[30][5], mainMap[31][5], mainMap[32][5], mainMap[33][5], mainMap[34][5], mainMap[35][5], mainMap[36][5], mainMap[37][5], mainMap[38][5], mainMap[39][5], mainMap[40][5], mainMap[41][5], mainMap[42][5], mainMap[43][5], mainMap[44][5], mainMap[45][5], mainMap[46][5], mainMap[47][5], mainMap[48][5], mainMap[49][5], mainMap[50][5], mainMap[51][5], mainMap[52][5], mainMap[53][5], mainMap[54][5], mainMap[55][5], mainMap[56][5], mainMap[57][5], mainMap[58][5], mainMap[59][5], mainMap[60][5], mainMap[61][5], mainMap[62][5], mainMap[63][5], mainMap[64][5], mainMap[65][5], mainMap[66][5], mainMap[67][5], mainMap[68][5], mainMap[69][5], mainMap[70][5], mainMap[71][5], mainMap[72][5], mainMap[73][5], mainMap[74][5], mainMap[75][5], mainMap[76][5], mainMap[77][5], mainMap[78][5], mainMap[79][5], mainMap[80][5], mainMap[81][5], mainMap[82][5], mainMap[83][5], mainMap[84][5], mainMap[85][5], mainMap[86][5], mainMap[87][5], mainMap[88][5], mainMap[89][5], mainMap[90][5], mainMap[91][5], mainMap[92][5], mainMap[93][5], mainMap[94][5], mainMap[95][5], mainMap[96][5], mainMap[97][5], mainMap[98][5], mainMap[99][5], mainMap[100][5], mainMap[101][5], mainMap[102][5], mainMap[103][5], mainMap[104][5], mainMap[105][5], mainMap[106][5], mainMap[107][5], mainMap[108][5], mainMap[109][5], mainMap[110][5], mainMap[111][5], mainMap[112][5], mainMap[113][5], mainMap[114][5], mainMap[115][5], mainMap[116][5], mainMap[117][5], mainMap[118][5], mainMap[119][5], mainMap[0][6], mainMap[1][6], mainMap[2][6], mainMap[3][6], mainMap[4][6], mainMap[5][6], mainMap[6][6], mainMap[7][6], mainMap[8][6], mainMap[9][6], mainMap[10][6], mainMap[11][6], mainMap[12][6], mainMap[13][6], mainMap[14][6], mainMap[15][6], mainMap[16][6], mainMap[17][6], mainMap[18][6], mainMap[19][6], mainMap[20][6], mainMap[21][6], mainMap[22][6], mainMap[23][6], mainMap[24][6], mainMap[25][6], mainMap[26][6], mainMap[27][6], mainMap[28][6], mainMap[29][6], mainMap[30][6], mainMap[31][6], mainMap[32][6], mainMap[33][6], mainMap[34][6], mainMap[35][6], mainMap[36][6], mainMap[37][6], mainMap[38][6], mainMap[39][6], mainMap[40][6], mainMap[41][6], mainMap[42][6], mainMap[43][6], mainMap[44][6], mainMap[45][6], mainMap[46][6], mainMap[47][6], mainMap[48][6], mainMap[49][6], mainMap[50][6], mainMap[51][6], mainMap[52][6], mainMap[53][6], mainMap[54][6], mainMap[55][6], mainMap[56][6], mainMap[57][6], mainMap[58][6], mainMap[59][6], mainMap[60][6], mainMap[61][6], mainMap[62][6], mainMap[63][6], mainMap[64][6], mainMap[65][6], mainMap[66][6], mainMap[67][6], mainMap[68][6], mainMap[69][6], mainMap[70][6], mainMap[71][6], mainMap[72][6], mainMap[73][6], mainMap[74][6], mainMap[75][6], mainMap[76][6], mainMap[77][6], mainMap[78][6], mainMap[79][6], mainMap[80][6], mainMap[81][6], mainMap[82][6], mainMap[83][6], mainMap[84][6], mainMap[85][6], mainMap[86][6], mainMap[87][6], mainMap[88][6], mainMap[89][6], mainMap[90][6], mainMap[91][6], mainMap[92][6], mainMap[93][6], mainMap[94][6], mainMap[95][6], mainMap[96][6], mainMap[97][6], mainMap[98][6], mainMap[99][6], mainMap[100][6], mainMap[101][6], mainMap[102][6], mainMap[103][6], mainMap[104][6], mainMap[105][6], mainMap[106][6], mainMap[107][6], mainMap[108][6], mainMap[109][6], mainMap[110][6], mainMap[111][6], mainMap[112][6], mainMap[113][6], mainMap[114][6], mainMap[115][6], mainMap[116][6], mainMap[117][6], mainMap[118][6], mainMap[119][6], mainMap[0][7], mainMap[1][7], mainMap[2][7], mainMap[3][7], mainMap[4][7], mainMap[5][7], mainMap[6][7], mainMap[7][7], mainMap[8][7], mainMap[9][7], mainMap[10][7], mainMap[11][7], mainMap[12][7], mainMap[13][7], mainMap[14][7], mainMap[15][7], mainMap[16][7], mainMap[17][7], mainMap[18][7], mainMap[19][7], mainMap[20][7], mainMap[21][7], mainMap[22][7], mainMap[23][7], mainMap[24][7], mainMap[25][7], mainMap[26][7], mainMap[27][7], mainMap[28][7], mainMap[29][7], mainMap[30][7], mainMap[31][7], mainMap[32][7], mainMap[33][7], mainMap[34][7], mainMap[35][7], mainMap[36][7], mainMap[37][7], mainMap[38][7], mainMap[39][7], mainMap[40][7], mainMap[41][7], mainMap[42][7], mainMap[43][7], mainMap[44][7], mainMap[45][7], mainMap[46][7], mainMap[47][7], mainMap[48][7], mainMap[49][7], mainMap[50][7], mainMap[51][7], mainMap[52][7], mainMap[53][7], mainMap[54][7], mainMap[55][7], mainMap[56][7], mainMap[57][7], mainMap[58][7], mainMap[59][7], mainMap[60][7], mainMap[61][7], mainMap[62][7], mainMap[63][7], mainMap[64][7], mainMap[65][7], mainMap[66][7], mainMap[67][7], mainMap[68][7], mainMap[69][7], mainMap[70][7], mainMap[71][7], mainMap[72][7], mainMap[73][7], mainMap[74][7], mainMap[75][7], mainMap[76][7], mainMap[77][7], mainMap[78][7], mainMap[79][7], mainMap[80][7], mainMap[81][7], mainMap[82][7], mainMap[83][7], mainMap[84][7], mainMap[85][7], mainMap[86][7], mainMap[87][7], mainMap[88][7], mainMap[89][7], mainMap[90][7], mainMap[91][7], mainMap[92][7], mainMap[93][7], mainMap[94][7], mainMap[95][7], mainMap[96][7], mainMap[97][7], mainMap[98][7], mainMap[99][7], mainMap[100][7], mainMap[101][7], mainMap[102][7], mainMap[103][7], mainMap[104][7], mainMap[105][7], mainMap[106][7], mainMap[107][7], mainMap[108][7], mainMap[109][7], mainMap[110][7], mainMap[111][7], mainMap[112][7], mainMap[113][7], mainMap[114][7], mainMap[115][7], mainMap[116][7], mainMap[117][7], mainMap[118][7], mainMap[119][7], mainMap[0][8], mainMap[1][8], mainMap[2][8], mainMap[3][8], mainMap[4][8], mainMap[5][8], mainMap[6][8], mainMap[7][8], mainMap[8][8], mainMap[9][8], mainMap[10][8], mainMap[11][8], mainMap[12][8], mainMap[13][8], mainMap[14][8], mainMap[15][8], mainMap[16][8], mainMap[17][8], mainMap[18][8], mainMap[19][8], mainMap[20][8], mainMap[21][8], mainMap[22][8], mainMap[23][8], mainMap[24][8], mainMap[25][8], mainMap[26][8], mainMap[27][8], mainMap[28][8], mainMap[29][8], mainMap[30][8], mainMap[31][8], mainMap[32][8], mainMap[33][8], mainMap[34][8], mainMap[35][8], mainMap[36][8], mainMap[37][8], mainMap[38][8], mainMap[39][8], mainMap[40][8], mainMap[41][8], mainMap[42][8], mainMap[43][8], mainMap[44][8], mainMap[45][8], mainMap[46][8], mainMap[47][8], mainMap[48][8], mainMap[49][8], mainMap[50][8], mainMap[51][8], mainMap[52][8], mainMap[53][8], mainMap[54][8], mainMap[55][8], mainMap[56][8], mainMap[57][8], mainMap[58][8], mainMap[59][8], mainMap[60][8], mainMap[61][8], mainMap[62][8], mainMap[63][8], mainMap[64][8], mainMap[65][8], mainMap[66][8], mainMap[67][8], mainMap[68][8], mainMap[69][8], mainMap[70][8], mainMap[71][8], mainMap[72][8], mainMap[73][8], mainMap[74][8], mainMap[75][8], mainMap[76][8], mainMap[77][8], mainMap[78][8], mainMap[79][8], mainMap[80][8], mainMap[81][8], mainMap[82][8], mainMap[83][8], mainMap[84][8], mainMap[85][8], mainMap[86][8], mainMap[87][8], mainMap[88][8], mainMap[89][8], mainMap[90][8], mainMap[91][8], mainMap[92][8], mainMap[93][8], mainMap[94][8], mainMap[95][8], mainMap[96][8], mainMap[97][8], mainMap[98][8], mainMap[99][8], mainMap[100][8], mainMap[101][8], mainMap[102][8], mainMap[103][8], mainMap[104][8], mainMap[105][8], mainMap[106][8], mainMap[107][8], mainMap[108][8], mainMap[109][8], mainMap[110][8], mainMap[111][8], mainMap[112][8], mainMap[113][8], mainMap[114][8], mainMap[115][8], mainMap[116][8], mainMap[117][8], mainMap[118][8], mainMap[119][8], mainMap[0][9], mainMap[1][9], mainMap[2][9], mainMap[3][9], mainMap[4][9], mainMap[5][9], mainMap[6][9], mainMap[7][9], mainMap[8][9], mainMap[9][9], mainMap[10][9], mainMap[11][9], mainMap[12][9], mainMap[13][9], mainMap[14][9], mainMap[15][9], mainMap[16][9], mainMap[17][9], mainMap[18][9], mainMap[19][9], mainMap[20][9], mainMap[21][9], mainMap[22][9], mainMap[23][9], mainMap[24][9], mainMap[25][9], mainMap[26][9], mainMap[27][9], mainMap[28][9], mainMap[29][9], mainMap[30][9], mainMap[31][9], mainMap[32][9], mainMap[33][9], mainMap[34][9], mainMap[35][9], mainMap[36][9], mainMap[37][9], mainMap[38][9], mainMap[39][9], mainMap[40][9], mainMap[41][9], mainMap[42][9], mainMap[43][9], mainMap[44][9], mainMap[45][9], mainMap[46][9], mainMap[47][9], mainMap[48][9], mainMap[49][9], mainMap[50][9], mainMap[51][9], mainMap[52][9], mainMap[53][9], mainMap[54][9], mainMap[55][9], mainMap[56][9], mainMap[57][9], mainMap[58][9], mainMap[59][9], mainMap[60][9], mainMap[61][9], mainMap[62][9], mainMap[63][9], mainMap[64][9], mainMap[65][9], mainMap[66][9], mainMap[67][9], mainMap[68][9], mainMap[69][9], mainMap[70][9], mainMap[71][9], mainMap[72][9], mainMap[73][9], mainMap[74][9], mainMap[75][9], mainMap[76][9], mainMap[77][9], mainMap[78][9], mainMap[79][9], mainMap[80][9], mainMap[81][9], mainMap[82][9], mainMap[83][9], mainMap[84][9], mainMap[85][9], mainMap[86][9], mainMap[87][9], mainMap[88][9], mainMap[89][9], mainMap[90][9], mainMap[91][9], mainMap[92][9], mainMap[93][9], mainMap[94][9], mainMap[95][9], mainMap[96][9], mainMap[97][9], mainMap[98][9], mainMap[99][9], mainMap[100][9], mainMap[101][9], mainMap[102][9], mainMap[103][9], mainMap[104][9], mainMap[105][9], mainMap[106][9], mainMap[107][9], mainMap[108][9], mainMap[109][9], mainMap[110][9], mainMap[111][9], mainMap[112][9], mainMap[113][9], mainMap[114][9], mainMap[115][9], mainMap[116][9], mainMap[117][9], mainMap[118][9], mainMap[119][9], mainMap[0][10], mainMap[1][10], mainMap[2][10], mainMap[3][10], mainMap[4][10], mainMap[5][10], mainMap[6][10], mainMap[7][10], mainMap[8][10], mainMap[9][10], mainMap[10][10], mainMap[11][10], mainMap[12][10], mainMap[13][10], mainMap[14][10], mainMap[15][10], mainMap[16][10], mainMap[17][10], mainMap[18][10], mainMap[19][10], mainMap[20][10], mainMap[21][10], mainMap[22][10], mainMap[23][10], mainMap[24][10], mainMap[25][10], mainMap[26][10], mainMap[27][10], mainMap[28][10], mainMap[29][10], mainMap[30][10], mainMap[31][10], mainMap[32][10], mainMap[33][10], mainMap[34][10], mainMap[35][10], mainMap[36][10], mainMap[37][10], mainMap[38][10], mainMap[39][10], mainMap[40][10], mainMap[41][10], mainMap[42][10], mainMap[43][10], mainMap[44][10], mainMap[45][10], mainMap[46][10], mainMap[47][10], mainMap[48][10], mainMap[49][10], mainMap[50][10], mainMap[51][10], mainMap[52][10], mainMap[53][10], mainMap[54][10], mainMap[55][10], mainMap[56][10], mainMap[57][10], mainMap[58][10], mainMap[59][10], mainMap[60][10], mainMap[61][10], mainMap[62][10], mainMap[63][10], mainMap[64][10], mainMap[65][10], mainMap[66][10], mainMap[67][10], mainMap[68][10], mainMap[69][10], mainMap[70][10], mainMap[71][10], mainMap[72][10], mainMap[73][10], mainMap[74][10], mainMap[75][10], mainMap[76][10], mainMap[77][10], mainMap[78][10], mainMap[79][10], mainMap[80][10], mainMap[81][10], mainMap[82][10], mainMap[83][10], mainMap[84][10], mainMap[85][10], mainMap[86][10], mainMap[87][10], mainMap[88][10], mainMap[89][10], mainMap[90][10], mainMap[91][10], mainMap[92][10], mainMap[93][10], mainMap[94][10], mainMap[95][10], mainMap[96][10], mainMap[97][10], mainMap[98][10], mainMap[99][10], mainMap[100][10], mainMap[101][10], mainMap[102][10], mainMap[103][10], mainMap[104][10], mainMap[105][10], mainMap[106][10], mainMap[107][10], mainMap[108][10], mainMap[109][10], mainMap[110][10], mainMap[111][10], mainMap[112][10], mainMap[113][10], mainMap[114][10], mainMap[115][10], mainMap[116][10], mainMap[117][10], mainMap[118][10], mainMap[119][10], mainMap[0][11], mainMap[1][11], mainMap[2][11], mainMap[3][11], mainMap[4][11], mainMap[5][11], mainMap[6][11], mainMap[7][11], mainMap[8][11], mainMap[9][11], mainMap[10][11], mainMap[11][11], mainMap[12][11], mainMap[13][11], mainMap[14][11], mainMap[15][11], mainMap[16][11], mainMap[17][11], mainMap[18][11], mainMap[19][11], mainMap[20][11], mainMap[21][11], mainMap[22][11], mainMap[23][11], mainMap[24][11], mainMap[25][11], mainMap[26][11], mainMap[27][11], mainMap[28][11], mainMap[29][11], mainMap[30][11], mainMap[31][11], mainMap[32][11], mainMap[33][11], mainMap[34][11], mainMap[35][11], mainMap[36][11], mainMap[37][11], mainMap[38][11], mainMap[39][11], mainMap[40][11], mainMap[41][11], mainMap[42][11], mainMap[43][11], mainMap[44][11], mainMap[45][11], mainMap[46][11], mainMap[47][11], mainMap[48][11], mainMap[49][11], mainMap[50][11], mainMap[51][11], mainMap[52][11], mainMap[53][11], mainMap[54][11], mainMap[55][11], mainMap[56][11], mainMap[57][11], mainMap[58][11], mainMap[59][11], mainMap[60][11], mainMap[61][11], mainMap[62][11], mainMap[63][11], mainMap[64][11], mainMap[65][11], mainMap[66][11], mainMap[67][11], mainMap[68][11], mainMap[69][11], mainMap[70][11], mainMap[71][11], mainMap[72][11], mainMap[73][11], mainMap[74][11], mainMap[75][11], mainMap[76][11], mainMap[77][11], mainMap[78][11], mainMap[79][11], mainMap[80][11], mainMap[81][11], mainMap[82][11], mainMap[83][11], mainMap[84][11], mainMap[85][11], mainMap[86][11], mainMap[87][11], mainMap[88][11], mainMap[89][11], mainMap[90][11], mainMap[91][11], mainMap[92][11], mainMap[93][11], mainMap[94][11], mainMap[95][11], mainMap[96][11], mainMap[97][11], mainMap[98][11], mainMap[99][11], mainMap[100][11], mainMap[101][11], mainMap[102][11], mainMap[103][11], mainMap[104][11], mainMap[105][11], mainMap[106][11], mainMap[107][11], mainMap[108][11], mainMap[109][11], mainMap[110][11], mainMap[111][11], mainMap[112][11], mainMap[113][11], mainMap[114][11], mainMap[115][11], mainMap[116][11], mainMap[117][11], mainMap[118][11], mainMap[119][11], mainMap[0][12], mainMap[1][12], mainMap[2][12], mainMap[3][12], mainMap[4][12], mainMap[5][12], mainMap[6][12], mainMap[7][12], mainMap[8][12], mainMap[9][12], mainMap[10][12], mainMap[11][12], mainMap[12][12], mainMap[13][12], mainMap[14][12], mainMap[15][12], mainMap[16][12], mainMap[17][12], mainMap[18][12], mainMap[19][12], mainMap[20][12], mainMap[21][12], mainMap[22][12], mainMap[23][12], mainMap[24][12], mainMap[25][12], mainMap[26][12], mainMap[27][12], mainMap[28][12], mainMap[29][12], mainMap[30][12], mainMap[31][12], mainMap[32][12], mainMap[33][12], mainMap[34][12], mainMap[35][12], mainMap[36][12], mainMap[37][12], mainMap[38][12], mainMap[39][12], mainMap[40][12], mainMap[41][12], mainMap[42][12], mainMap[43][12], mainMap[44][12], mainMap[45][12], mainMap[46][12], mainMap[47][12], mainMap[48][12], mainMap[49][12], mainMap[50][12], mainMap[51][12], mainMap[52][12], mainMap[53][12], mainMap[54][12], mainMap[55][12], mainMap[56][12], mainMap[57][12], mainMap[58][12], mainMap[59][12], mainMap[60][12], mainMap[61][12], mainMap[62][12], mainMap[63][12], mainMap[64][12], mainMap[65][12], mainMap[66][12], mainMap[67][12], mainMap[68][12], mainMap[69][12], mainMap[70][12], mainMap[71][12], mainMap[72][12], mainMap[73][12], mainMap[74][12], mainMap[75][12], mainMap[76][12], mainMap[77][12], mainMap[78][12], mainMap[79][12], mainMap[80][12], mainMap[81][12], mainMap[82][12], mainMap[83][12], mainMap[84][12], mainMap[85][12], mainMap[86][12], mainMap[87][12], mainMap[88][12], mainMap[89][12], mainMap[90][12], mainMap[91][12], mainMap[92][12], mainMap[93][12], mainMap[94][12], mainMap[95][12], mainMap[96][12], mainMap[97][12], mainMap[98][12], mainMap[99][12], mainMap[100][12], mainMap[101][12], mainMap[102][12], mainMap[103][12], mainMap[104][12], mainMap[105][12], mainMap[106][12], mainMap[107][12], mainMap[108][12], mainMap[109][12], mainMap[110][12], mainMap[111][12], mainMap[112][12], mainMap[113][12], mainMap[114][12], mainMap[115][12], mainMap[116][12], mainMap[117][12], mainMap[118][12], mainMap[119][12], mainMap[0][13], mainMap[1][13], mainMap[2][13], mainMap[3][13], mainMap[4][13], mainMap[5][13], mainMap[6][13], mainMap[7][13], mainMap[8][13], mainMap[9][13], mainMap[10][13], mainMap[11][13], mainMap[12][13], mainMap[13][13], mainMap[14][13], mainMap[15][13], mainMap[16][13], mainMap[17][13], mainMap[18][13], mainMap[19][13], mainMap[20][13], mainMap[21][13], mainMap[22][13], mainMap[23][13], mainMap[24][13], mainMap[25][13], mainMap[26][13], mainMap[27][13], mainMap[28][13], mainMap[29][13], mainMap[30][13], mainMap[31][13], mainMap[32][13], mainMap[33][13], mainMap[34][13], mainMap[35][13], mainMap[36][13], mainMap[37][13], mainMap[38][13], mainMap[39][13], mainMap[40][13], mainMap[41][13], mainMap[42][13], mainMap[43][13], mainMap[44][13], mainMap[45][13], mainMap[46][13], mainMap[47][13], mainMap[48][13], mainMap[49][13], mainMap[50][13], mainMap[51][13], mainMap[52][13], mainMap[53][13], mainMap[54][13], mainMap[55][13], mainMap[56][13], mainMap[57][13], mainMap[58][13], mainMap[59][13], mainMap[60][13], mainMap[61][13], mainMap[62][13], mainMap[63][13], mainMap[64][13], mainMap[65][13], mainMap[66][13], mainMap[67][13], mainMap[68][13], mainMap[69][13], mainMap[70][13], mainMap[71][13], mainMap[72][13], mainMap[73][13], mainMap[74][13], mainMap[75][13], mainMap[76][13], mainMap[77][13], mainMap[78][13], mainMap[79][13], mainMap[80][13], mainMap[81][13], mainMap[82][13], mainMap[83][13], mainMap[84][13], mainMap[85][13], mainMap[86][13], mainMap[87][13], mainMap[88][13], mainMap[89][13], mainMap[90][13], mainMap[91][13], mainMap[92][13], mainMap[93][13], mainMap[94][13], mainMap[95][13], mainMap[96][13], mainMap[97][13], mainMap[98][13], mainMap[99][13], mainMap[100][13], mainMap[101][13], mainMap[102][13], mainMap[103][13], mainMap[104][13], mainMap[105][13], mainMap[106][13], mainMap[107][13], mainMap[108][13], mainMap[109][13], mainMap[110][13], mainMap[111][13], mainMap[112][13], mainMap[113][13], mainMap[114][13], mainMap[115][13], mainMap[116][13], mainMap[117][13], mainMap[118][13], mainMap[119][13], mainMap[0][14], mainMap[1][14], mainMap[2][14], mainMap[3][14], mainMap[4][14], mainMap[5][14], mainMap[6][14], mainMap[7][14], mainMap[8][14], mainMap[9][14], mainMap[10][14], mainMap[11][14], mainMap[12][14], mainMap[13][14], mainMap[14][14], mainMap[15][14], mainMap[16][14], mainMap[17][14], mainMap[18][14], mainMap[19][14], mainMap[20][14], mainMap[21][14], mainMap[22][14], mainMap[23][14], mainMap[24][14], mainMap[25][14], mainMap[26][14], mainMap[27][14], mainMap[28][14], mainMap[29][14], mainMap[30][14], mainMap[31][14], mainMap[32][14], mainMap[33][14], mainMap[34][14], mainMap[35][14], mainMap[36][14], mainMap[37][14], mainMap[38][14], mainMap[39][14], mainMap[40][14], mainMap[41][14], mainMap[42][14], mainMap[43][14], mainMap[44][14], mainMap[45][14], mainMap[46][14], mainMap[47][14], mainMap[48][14], mainMap[49][14], mainMap[50][14], mainMap[51][14], mainMap[52][14], mainMap[53][14], mainMap[54][14], mainMap[55][14], mainMap[56][14], mainMap[57][14], mainMap[58][14], mainMap[59][14], mainMap[60][14], mainMap[61][14], mainMap[62][14], mainMap[63][14], mainMap[64][14], mainMap[65][14], mainMap[66][14], mainMap[67][14], mainMap[68][14], mainMap[69][14], mainMap[70][14], mainMap[71][14], mainMap[72][14], mainMap[73][14], mainMap[74][14], mainMap[75][14], mainMap[76][14], mainMap[77][14], mainMap[78][14], mainMap[79][14], mainMap[80][14], mainMap[81][14], mainMap[82][14], mainMap[83][14], mainMap[84][14], mainMap[85][14], mainMap[86][14], mainMap[87][14], mainMap[88][14], mainMap[89][14], mainMap[90][14], mainMap[91][14], mainMap[92][14], mainMap[93][14], mainMap[94][14], mainMap[95][14], mainMap[96][14], mainMap[97][14], mainMap[98][14], mainMap[99][14], mainMap[100][14], mainMap[101][14], mainMap[102][14], mainMap[103][14], mainMap[104][14], mainMap[105][14], mainMap[106][14], mainMap[107][14], mainMap[108][14], mainMap[109][14], mainMap[110][14], mainMap[111][14], mainMap[112][14], mainMap[113][14], mainMap[114][14], mainMap[115][14], mainMap[116][14], mainMap[117][14], mainMap[118][14], mainMap[119][14], mainMap[0][15], mainMap[1][15], mainMap[2][15], mainMap[3][15], mainMap[4][15], mainMap[5][15], mainMap[6][15], mainMap[7][15], mainMap[8][15], mainMap[9][15], mainMap[10][15], mainMap[11][15], mainMap[12][15], mainMap[13][15], mainMap[14][15], mainMap[15][15], mainMap[16][15], mainMap[17][15], mainMap[18][15], mainMap[19][15], mainMap[20][15], mainMap[21][15], mainMap[22][15], mainMap[23][15], mainMap[24][15], mainMap[25][15], mainMap[26][15], mainMap[27][15], mainMap[28][15], mainMap[29][15], mainMap[30][15], mainMap[31][15], mainMap[32][15], mainMap[33][15], mainMap[34][15], mainMap[35][15], mainMap[36][15], mainMap[37][15], mainMap[38][15], mainMap[39][15], mainMap[40][15], mainMap[41][15], mainMap[42][15], mainMap[43][15], mainMap[44][15], mainMap[45][15], mainMap[46][15], mainMap[47][15], mainMap[48][15], mainMap[49][15], mainMap[50][15], mainMap[51][15], mainMap[52][15], mainMap[53][15], mainMap[54][15], mainMap[55][15], mainMap[56][15], mainMap[57][15], mainMap[58][15], mainMap[59][15], mainMap[60][15], mainMap[61][15], mainMap[62][15], mainMap[63][15], mainMap[64][15], mainMap[65][15], mainMap[66][15], mainMap[67][15], mainMap[68][15], mainMap[69][15], mainMap[70][15], mainMap[71][15], mainMap[72][15], mainMap[73][15], mainMap[74][15], mainMap[75][15], mainMap[76][15], mainMap[77][15], mainMap[78][15], mainMap[79][15], mainMap[80][15], mainMap[81][15], mainMap[82][15], mainMap[83][15], mainMap[84][15], mainMap[85][15], mainMap[86][15], mainMap[87][15], mainMap[88][15], mainMap[89][15], mainMap[90][15], mainMap[91][15], mainMap[92][15], mainMap[93][15], mainMap[94][15], mainMap[95][15], mainMap[96][15], mainMap[97][15], mainMap[98][15], mainMap[99][15], mainMap[100][15], mainMap[101][15], mainMap[102][15], mainMap[103][15], mainMap[104][15], mainMap[105][15], mainMap[106][15], mainMap[107][15], mainMap[108][15], mainMap[109][15], mainMap[110][15], mainMap[111][15], mainMap[112][15], mainMap[113][15], mainMap[114][15], mainMap[115][15], mainMap[116][15], mainMap[117][15], mainMap[118][15], mainMap[119][15], mainMap[0][16], mainMap[1][16], mainMap[2][16], mainMap[3][16], mainMap[4][16], mainMap[5][16], mainMap[6][16], mainMap[7][16], mainMap[8][16], mainMap[9][16], mainMap[10][16], mainMap[11][16], mainMap[12][16], mainMap[13][16], mainMap[14][16], mainMap[15][16], mainMap[16][16], mainMap[17][16], mainMap[18][16], mainMap[19][16], mainMap[20][16], mainMap[21][16], mainMap[22][16], mainMap[23][16], mainMap[24][16], mainMap[25][16], mainMap[26][16], mainMap[27][16], mainMap[28][16], mainMap[29][16], mainMap[30][16], mainMap[31][16], mainMap[32][16], mainMap[33][16], mainMap[34][16], mainMap[35][16], mainMap[36][16], mainMap[37][16], mainMap[38][16], mainMap[39][16], mainMap[40][16], mainMap[41][16], mainMap[42][16], mainMap[43][16], mainMap[44][16], mainMap[45][16], mainMap[46][16], mainMap[47][16], mainMap[48][16], mainMap[49][16], mainMap[50][16], mainMap[51][16], mainMap[52][16], mainMap[53][16], mainMap[54][16], mainMap[55][16], mainMap[56][16], mainMap[57][16], mainMap[58][16], mainMap[59][16], mainMap[60][16], mainMap[61][16], mainMap[62][16], mainMap[63][16], mainMap[64][16], mainMap[65][16], mainMap[66][16], mainMap[67][16], mainMap[68][16], mainMap[69][16], mainMap[70][16], mainMap[71][16], mainMap[72][16], mainMap[73][16], mainMap[74][16], mainMap[75][16], mainMap[76][16], mainMap[77][16], mainMap[78][16], mainMap[79][16], mainMap[80][16], mainMap[81][16], mainMap[82][16], mainMap[83][16], mainMap[84][16], mainMap[85][16], mainMap[86][16], mainMap[87][16], mainMap[88][16], mainMap[89][16], mainMap[90][16], mainMap[91][16], mainMap[92][16], mainMap[93][16], mainMap[94][16], mainMap[95][16], mainMap[96][16], mainMap[97][16], mainMap[98][16], mainMap[99][16], mainMap[100][16], mainMap[101][16], mainMap[102][16], mainMap[103][16], mainMap[104][16], mainMap[105][16], mainMap[106][16], mainMap[107][16], mainMap[108][16], mainMap[109][16], mainMap[110][16], mainMap[111][16], mainMap[112][16], mainMap[113][16], mainMap[114][16], mainMap[115][16], mainMap[116][16], mainMap[117][16], mainMap[118][16], mainMap[119][16], mainMap[0][17], mainMap[1][17], mainMap[2][17], mainMap[3][17], mainMap[4][17], mainMap[5][17], mainMap[6][17], mainMap[7][17], mainMap[8][17], mainMap[9][17], mainMap[10][17], mainMap[11][17], mainMap[12][17], mainMap[13][17], mainMap[14][17], mainMap[15][17], mainMap[16][17], mainMap[17][17], mainMap[18][17], mainMap[19][17], mainMap[20][17], mainMap[21][17], mainMap[22][17], mainMap[23][17], mainMap[24][17], mainMap[25][17], mainMap[26][17], mainMap[27][17], mainMap[28][17], mainMap[29][17], mainMap[30][17], mainMap[31][17], mainMap[32][17], mainMap[33][17], mainMap[34][17], mainMap[35][17], mainMap[36][17], mainMap[37][17], mainMap[38][17], mainMap[39][17], mainMap[40][17], mainMap[41][17], mainMap[42][17], mainMap[43][17], mainMap[44][17], mainMap[45][17], mainMap[46][17], mainMap[47][17], mainMap[48][17], mainMap[49][17], mainMap[50][17], mainMap[51][17], mainMap[52][17], mainMap[53][17], mainMap[54][17], mainMap[55][17], mainMap[56][17], mainMap[57][17], mainMap[58][17], mainMap[59][17], mainMap[60][17], mainMap[61][17], mainMap[62][17], mainMap[63][17], mainMap[64][17], mainMap[65][17], mainMap[66][17], mainMap[67][17], mainMap[68][17], mainMap[69][17], mainMap[70][17], mainMap[71][17], mainMap[72][17], mainMap[73][17], mainMap[74][17], mainMap[75][17], mainMap[76][17], mainMap[77][17], mainMap[78][17], mainMap[79][17], mainMap[80][17], mainMap[81][17], mainMap[82][17], mainMap[83][17], mainMap[84][17], mainMap[85][17], mainMap[86][17], mainMap[87][17], mainMap[88][17], mainMap[89][17], mainMap[90][17], mainMap[91][17], mainMap[92][17], mainMap[93][17], mainMap[94][17], mainMap[95][17], mainMap[96][17], mainMap[97][17], mainMap[98][17], mainMap[99][17], mainMap[100][17], mainMap[101][17], mainMap[102][17], mainMap[103][17], mainMap[104][17], mainMap[105][17], mainMap[106][17], mainMap[107][17], mainMap[108][17], mainMap[109][17], mainMap[110][17], mainMap[111][17], mainMap[112][17], mainMap[113][17], mainMap[114][17], mainMap[115][17], mainMap[116][17], mainMap[117][17], mainMap[118][17], mainMap[119][17], mainMap[0][18], mainMap[1][18], mainMap[2][18], mainMap[3][18], mainMap[4][18], mainMap[5][18], mainMap[6][18], mainMap[7][18], mainMap[8][18], mainMap[9][18], mainMap[10][18], mainMap[11][18], mainMap[12][18], mainMap[13][18], mainMap[14][18], mainMap[15][18], mainMap[16][18], mainMap[17][18], mainMap[18][18], mainMap[19][18], mainMap[20][18], mainMap[21][18], mainMap[22][18], mainMap[23][18], mainMap[24][18], mainMap[25][18], mainMap[26][18], mainMap[27][18], mainMap[28][18], mainMap[29][18], mainMap[30][18], mainMap[31][18], mainMap[32][18], mainMap[33][18], mainMap[34][18], mainMap[35][18], mainMap[36][18], mainMap[37][18], mainMap[38][18], mainMap[39][18], mainMap[40][18], mainMap[41][18], mainMap[42][18], mainMap[43][18], mainMap[44][18], mainMap[45][18], mainMap[46][18], mainMap[47][18], mainMap[48][18], mainMap[49][18], mainMap[50][18], mainMap[51][18], mainMap[52][18], mainMap[53][18], mainMap[54][18], mainMap[55][18], mainMap[56][18], mainMap[57][18], mainMap[58][18], mainMap[59][18], mainMap[60][18], mainMap[61][18], mainMap[62][18], mainMap[63][18], mainMap[64][18], mainMap[65][18], mainMap[66][18], mainMap[67][18], mainMap[68][18], mainMap[69][18], mainMap[70][18], mainMap[71][18], mainMap[72][18], mainMap[73][18], mainMap[74][18], mainMap[75][18], mainMap[76][18], mainMap[77][18], mainMap[78][18], mainMap[79][18], mainMap[80][18], mainMap[81][18], mainMap[82][18], mainMap[83][18], mainMap[84][18], mainMap[85][18], mainMap[86][18], mainMap[87][18], mainMap[88][18], mainMap[89][18], mainMap[90][18], mainMap[91][18], mainMap[92][18], mainMap[93][18], mainMap[94][18], mainMap[95][18], mainMap[96][18], mainMap[97][18], mainMap[98][18], mainMap[99][18], mainMap[100][18], mainMap[101][18], mainMap[102][18], mainMap[103][18], mainMap[104][18], mainMap[105][18], mainMap[106][18], mainMap[107][18], mainMap[108][18], mainMap[109][18], mainMap[110][18], mainMap[111][18], mainMap[112][18], mainMap[113][18], mainMap[114][18], mainMap[115][18], mainMap[116][18], mainMap[117][18], mainMap[118][18], mainMap[119][18], mainMap[0][19], mainMap[1][19], mainMap[2][19], mainMap[3][19], mainMap[4][19], mainMap[5][19], mainMap[6][19], mainMap[7][19], mainMap[8][19], mainMap[9][19], mainMap[10][19], mainMap[11][19], mainMap[12][19], mainMap[13][19], mainMap[14][19], mainMap[15][19], mainMap[16][19], mainMap[17][19], mainMap[18][19], mainMap[19][19], mainMap[20][19], mainMap[21][19], mainMap[22][19], mainMap[23][19], mainMap[24][19], mainMap[25][19], mainMap[26][19], mainMap[27][19], mainMap[28][19], mainMap[29][19], mainMap[30][19], mainMap[31][19], mainMap[32][19], mainMap[33][19], mainMap[34][19], mainMap[35][19], mainMap[36][19], mainMap[37][19], mainMap[38][19], mainMap[39][19], mainMap[40][19], mainMap[41][19], mainMap[42][19], mainMap[43][19], mainMap[44][19], mainMap[45][19], mainMap[46][19], mainMap[47][19], mainMap[48][19], mainMap[49][19], mainMap[50][19], mainMap[51][19], mainMap[52][19], mainMap[53][19], mainMap[54][19], mainMap[55][19], mainMap[56][19], mainMap[57][19], mainMap[58][19], mainMap[59][19], mainMap[60][19], mainMap[61][19], mainMap[62][19], mainMap[63][19], mainMap[64][19], mainMap[65][19], mainMap[66][19], mainMap[67][19], mainMap[68][19], mainMap[69][19], mainMap[70][19], mainMap[71][19], mainMap[72][19], mainMap[73][19], mainMap[74][19], mainMap[75][19], mainMap[76][19], mainMap[77][19], mainMap[78][19], mainMap[79][19], mainMap[80][19], mainMap[81][19], mainMap[82][19], mainMap[83][19], mainMap[84][19], mainMap[85][19], mainMap[86][19], mainMap[87][19], mainMap[88][19], mainMap[89][19], mainMap[90][19], mainMap[91][19], mainMap[92][19], mainMap[93][19], mainMap[94][19], mainMap[95][19], mainMap[96][19], mainMap[97][19], mainMap[98][19], mainMap[99][19], mainMap[100][19], mainMap[101][19], mainMap[102][19], mainMap[103][19], mainMap[104][19], mainMap[105][19], mainMap[106][19], mainMap[107][19], mainMap[108][19], mainMap[109][19], mainMap[110][19], mainMap[111][19], mainMap[112][19], mainMap[113][19], mainMap[114][19], mainMap[115][19], mainMap[116][19], mainMap[117][19], mainMap[118][19], mainMap[119][19], mainMap[0][20], mainMap[1][20], mainMap[2][20], mainMap[3][20], mainMap[4][20], mainMap[5][20], mainMap[6][20], mainMap[7][20], mainMap[8][20], mainMap[9][20], mainMap[10][20], mainMap[11][20], mainMap[12][20], mainMap[13][20], mainMap[14][20], mainMap[15][20], mainMap[16][20], mainMap[17][20], mainMap[18][20], mainMap[19][20], mainMap[20][20], mainMap[21][20], mainMap[22][20], mainMap[23][20], mainMap[24][20], mainMap[25][20], mainMap[26][20], mainMap[27][20], mainMap[28][20], mainMap[29][20], mainMap[30][20], mainMap[31][20], mainMap[32][20], mainMap[33][20], mainMap[34][20], mainMap[35][20], mainMap[36][20], mainMap[37][20], mainMap[38][20], mainMap[39][20], mainMap[40][20], mainMap[41][20], mainMap[42][20], mainMap[43][20], mainMap[44][20], mainMap[45][20], mainMap[46][20], mainMap[47][20], mainMap[48][20], mainMap[49][20], mainMap[50][20], mainMap[51][20], mainMap[52][20], mainMap[53][20], mainMap[54][20], mainMap[55][20], mainMap[56][20], mainMap[57][20], mainMap[58][20], mainMap[59][20], mainMap[60][20], mainMap[61][20], mainMap[62][20], mainMap[63][20], mainMap[64][20], mainMap[65][20], mainMap[66][20], mainMap[67][20], mainMap[68][20], mainMap[69][20], mainMap[70][20], mainMap[71][20], mainMap[72][20], mainMap[73][20], mainMap[74][20], mainMap[75][20], mainMap[76][20], mainMap[77][20], mainMap[78][20], mainMap[79][20], mainMap[80][20], mainMap[81][20], mainMap[82][20], mainMap[83][20], mainMap[84][20], mainMap[85][20], mainMap[86][20], mainMap[87][20], mainMap[88][20], mainMap[89][20], mainMap[90][20], mainMap[91][20], mainMap[92][20], mainMap[93][20], mainMap[94][20], mainMap[95][20], mainMap[96][20], mainMap[97][20], mainMap[98][20], mainMap[99][20], mainMap[100][20], mainMap[101][20], mainMap[102][20], mainMap[103][20], mainMap[104][20], mainMap[105][20], mainMap[106][20], mainMap[107][20], mainMap[108][20], mainMap[109][20], mainMap[110][20], mainMap[111][20], mainMap[112][20], mainMap[113][20], mainMap[114][20], mainMap[115][20], mainMap[116][20], mainMap[117][20], mainMap[118][20], mainMap[119][20], mainMap[0][21], mainMap[1][21], mainMap[2][21], mainMap[3][21], mainMap[4][21], mainMap[5][21], mainMap[6][21], mainMap[7][21], mainMap[8][21], mainMap[9][21], mainMap[10][21], mainMap[11][21], mainMap[12][21], mainMap[13][21], mainMap[14][21], mainMap[15][21], mainMap[16][21], mainMap[17][21], mainMap[18][21], mainMap[19][21], mainMap[20][21], mainMap[21][21], mainMap[22][21], mainMap[23][21], mainMap[24][21], mainMap[25][21], mainMap[26][21], mainMap[27][21], mainMap[28][21], mainMap[29][21], mainMap[30][21], mainMap[31][21], mainMap[32][21], mainMap[33][21], mainMap[34][21], mainMap[35][21], mainMap[36][21], mainMap[37][21], mainMap[38][21], mainMap[39][21], mainMap[40][21], mainMap[41][21], mainMap[42][21], mainMap[43][21], mainMap[44][21], mainMap[45][21], mainMap[46][21], mainMap[47][21], mainMap[48][21], mainMap[49][21], mainMap[50][21], mainMap[51][21], mainMap[52][21], mainMap[53][21], mainMap[54][21], mainMap[55][21], mainMap[56][21], mainMap[57][21], mainMap[58][21], mainMap[59][21], mainMap[60][21], mainMap[61][21], mainMap[62][21], mainMap[63][21], mainMap[64][21], mainMap[65][21], mainMap[66][21], mainMap[67][21], mainMap[68][21], mainMap[69][21], mainMap[70][21], mainMap[71][21], mainMap[72][21], mainMap[73][21], mainMap[74][21], mainMap[75][21], mainMap[76][21], mainMap[77][21], mainMap[78][21], mainMap[79][21], mainMap[80][21], mainMap[81][21], mainMap[82][21], mainMap[83][21], mainMap[84][21], mainMap[85][21], mainMap[86][21], mainMap[87][21], mainMap[88][21], mainMap[89][21], mainMap[90][21], mainMap[91][21], mainMap[92][21], mainMap[93][21], mainMap[94][21], mainMap[95][21], mainMap[96][21], mainMap[97][21], mainMap[98][21], mainMap[99][21], mainMap[100][21], mainMap[101][21], mainMap[102][21], mainMap[103][21], mainMap[104][21], mainMap[105][21], mainMap[106][21], mainMap[107][21], mainMap[108][21], mainMap[109][21], mainMap[110][21], mainMap[111][21], mainMap[112][21], mainMap[113][21], mainMap[114][21], mainMap[115][21], mainMap[116][21], mainMap[117][21], mainMap[118][21], mainMap[119][21], mainMap[0][22], mainMap[1][22], mainMap[2][22], mainMap[3][22], mainMap[4][22], mainMap[5][22], mainMap[6][22], mainMap[7][22], mainMap[8][22], mainMap[9][22], mainMap[10][22], mainMap[11][22], mainMap[12][22], mainMap[13][22], mainMap[14][22], mainMap[15][22], mainMap[16][22], mainMap[17][22], mainMap[18][22], mainMap[19][22], mainMap[20][22], mainMap[21][22], mainMap[22][22], mainMap[23][22], mainMap[24][22], mainMap[25][22], mainMap[26][22], mainMap[27][22], mainMap[28][22], mainMap[29][22], mainMap[30][22], mainMap[31][22], mainMap[32][22], mainMap[33][22], mainMap[34][22], mainMap[35][22], mainMap[36][22], mainMap[37][22], mainMap[38][22], mainMap[39][22], mainMap[40][22], mainMap[41][22], mainMap[42][22], mainMap[43][22], mainMap[44][22], mainMap[45][22], mainMap[46][22], mainMap[47][22], mainMap[48][22], mainMap[49][22], mainMap[50][22], mainMap[51][22], mainMap[52][22], mainMap[53][22], mainMap[54][22], mainMap[55][22], mainMap[56][22], mainMap[57][22], mainMap[58][22], mainMap[59][22], mainMap[60][22], mainMap[61][22], mainMap[62][22], mainMap[63][22], mainMap[64][22], mainMap[65][22], mainMap[66][22], mainMap[67][22], mainMap[68][22], mainMap[69][22], mainMap[70][22], mainMap[71][22], mainMap[72][22], mainMap[73][22], mainMap[74][22], mainMap[75][22], mainMap[76][22], mainMap[77][22], mainMap[78][22], mainMap[79][22], mainMap[80][22], mainMap[81][22], mainMap[82][22], mainMap[83][22], mainMap[84][22], mainMap[85][22], mainMap[86][22], mainMap[87][22], mainMap[88][22], mainMap[89][22], mainMap[90][22], mainMap[91][22], mainMap[92][22], mainMap[93][22], mainMap[94][22], mainMap[95][22], mainMap[96][22], mainMap[97][22], mainMap[98][22], mainMap[99][22], mainMap[100][22], mainMap[101][22], mainMap[102][22], mainMap[103][22], mainMap[104][22], mainMap[105][22], mainMap[106][22], mainMap[107][22], mainMap[108][22], mainMap[109][22], mainMap[110][22], mainMap[111][22], mainMap[112][22], mainMap[113][22], mainMap[114][22], mainMap[115][22], mainMap[116][22], mainMap[117][22], mainMap[118][22], mainMap[119][22], mainMap[0][23], mainMap[1][23], mainMap[2][23], mainMap[3][23], mainMap[4][23], mainMap[5][23], mainMap[6][23], mainMap[7][23], mainMap[8][23], mainMap[9][23], mainMap[10][23], mainMap[11][23], mainMap[12][23], mainMap[13][23], mainMap[14][23], mainMap[15][23], mainMap[16][23], mainMap[17][23], mainMap[18][23], mainMap[19][23], mainMap[20][23], mainMap[21][23], mainMap[22][23], mainMap[23][23], mainMap[24][23], mainMap[25][23], mainMap[26][23], mainMap[27][23], mainMap[28][23], mainMap[29][23], mainMap[30][23], mainMap[31][23], mainMap[32][23], mainMap[33][23], mainMap[34][23], mainMap[35][23], mainMap[36][23], mainMap[37][23], mainMap[38][23], mainMap[39][23], mainMap[40][23], mainMap[41][23], mainMap[42][23], mainMap[43][23], mainMap[44][23], mainMap[45][23], mainMap[46][23], mainMap[47][23], mainMap[48][23], mainMap[49][23], mainMap[50][23], mainMap[51][23], mainMap[52][23], mainMap[53][23], mainMap[54][23], mainMap[55][23], mainMap[56][23], mainMap[57][23], mainMap[58][23], mainMap[59][23], mainMap[60][23], mainMap[61][23], mainMap[62][23], mainMap[63][23], mainMap[64][23], mainMap[65][23], mainMap[66][23], mainMap[67][23], mainMap[68][23], mainMap[69][23], mainMap[70][23], mainMap[71][23], mainMap[72][23], mainMap[73][23], mainMap[74][23], mainMap[75][23], mainMap[76][23], mainMap[77][23], mainMap[78][23], mainMap[79][23], mainMap[80][23], mainMap[81][23], mainMap[82][23], mainMap[83][23], mainMap[84][23], mainMap[85][23], mainMap[86][23], mainMap[87][23], mainMap[88][23], mainMap[89][23], mainMap[90][23], mainMap[91][23], mainMap[92][23], mainMap[93][23], mainMap[94][23], mainMap[95][23], mainMap[96][23], mainMap[97][23], mainMap[98][23], mainMap[99][23], mainMap[100][23], mainMap[101][23], mainMap[102][23], mainMap[103][23], mainMap[104][23], mainMap[105][23], mainMap[106][23], mainMap[107][23], mainMap[108][23], mainMap[109][23], mainMap[110][23], mainMap[111][23], mainMap[112][23], mainMap[113][23], mainMap[114][23], mainMap[115][23], mainMap[116][23], mainMap[117][23], mainMap[118][23], mainMap[119][23], mainMap[0][24], mainMap[1][24], mainMap[2][24], mainMap[3][24], mainMap[4][24], mainMap[5][24], mainMap[6][24], mainMap[7][24], mainMap[8][24], mainMap[9][24], mainMap[10][24], mainMap[11][24], mainMap[12][24], mainMap[13][24], mainMap[14][24], mainMap[15][24], mainMap[16][24], mainMap[17][24], mainMap[18][24], mainMap[19][24], mainMap[20][24], mainMap[21][24], mainMap[22][24], mainMap[23][24], mainMap[24][24], mainMap[25][24], mainMap[26][24], mainMap[27][24], mainMap[28][24], mainMap[29][24], mainMap[30][24], mainMap[31][24], mainMap[32][24], mainMap[33][24], mainMap[34][24], mainMap[35][24], mainMap[36][24], mainMap[37][24], mainMap[38][24], mainMap[39][24], mainMap[40][24], mainMap[41][24], mainMap[42][24], mainMap[43][24], mainMap[44][24], mainMap[45][24], mainMap[46][24], mainMap[47][24], mainMap[48][24], mainMap[49][24], mainMap[50][24], mainMap[51][24], mainMap[52][24], mainMap[53][24], mainMap[54][24], mainMap[55][24], mainMap[56][24], mainMap[57][24], mainMap[58][24], mainMap[59][24], mainMap[60][24], mainMap[61][24], mainMap[62][24], mainMap[63][24], mainMap[64][24], mainMap[65][24], mainMap[66][24], mainMap[67][24], mainMap[68][24], mainMap[69][24], mainMap[70][24], mainMap[71][24], mainMap[72][24], mainMap[73][24], mainMap[74][24], mainMap[75][24], mainMap[76][24], mainMap[77][24], mainMap[78][24], mainMap[79][24], mainMap[80][24], mainMap[81][24], mainMap[82][24], mainMap[83][24], mainMap[84][24], mainMap[85][24], mainMap[86][24], mainMap[87][24], mainMap[88][24], mainMap[89][24], mainMap[90][24], mainMap[91][24], mainMap[92][24], mainMap[93][24], mainMap[94][24], mainMap[95][24], mainMap[96][24], mainMap[97][24], mainMap[98][24], mainMap[99][24], mainMap[100][24], mainMap[101][24], mainMap[102][24], mainMap[103][24], mainMap[104][24], mainMap[105][24], mainMap[106][24], mainMap[107][24], mainMap[108][24], mainMap[109][24], mainMap[110][24], mainMap[111][24], mainMap[112][24], mainMap[113][24], mainMap[114][24], mainMap[115][24], mainMap[116][24], mainMap[117][24], mainMap[118][24], mainMap[119][24], mainMap[0][25], mainMap[1][25], mainMap[2][25], mainMap[3][25], mainMap[4][25], mainMap[5][25], mainMap[6][25], mainMap[7][25], mainMap[8][25], mainMap[9][25], mainMap[10][25], mainMap[11][25], mainMap[12][25], mainMap[13][25], mainMap[14][25], mainMap[15][25], mainMap[16][25], mainMap[17][25], mainMap[18][25], mainMap[19][25], mainMap[20][25], mainMap[21][25], mainMap[22][25], mainMap[23][25], mainMap[24][25], mainMap[25][25], mainMap[26][25], mainMap[27][25], mainMap[28][25], mainMap[29][25], mainMap[30][25], mainMap[31][25], mainMap[32][25], mainMap[33][25], mainMap[34][25], mainMap[35][25], mainMap[36][25], mainMap[37][25], mainMap[38][25], mainMap[39][25], mainMap[40][25], mainMap[41][25], mainMap[42][25], mainMap[43][25], mainMap[44][25], mainMap[45][25], mainMap[46][25], mainMap[47][25], mainMap[48][25], mainMap[49][25], mainMap[50][25], mainMap[51][25], mainMap[52][25], mainMap[53][25], mainMap[54][25], mainMap[55][25], mainMap[56][25], mainMap[57][25], mainMap[58][25], mainMap[59][25], mainMap[60][25], mainMap[61][25], mainMap[62][25], mainMap[63][25], mainMap[64][25], mainMap[65][25], mainMap[66][25], mainMap[67][25], mainMap[68][25], mainMap[69][25], mainMap[70][25], mainMap[71][25], mainMap[72][25], mainMap[73][25], mainMap[74][25], mainMap[75][25], mainMap[76][25], mainMap[77][25], mainMap[78][25], mainMap[79][25], mainMap[80][25], mainMap[81][25], mainMap[82][25], mainMap[83][25], mainMap[84][25], mainMap[85][25], mainMap[86][25], mainMap[87][25], mainMap[88][25], mainMap[89][25], mainMap[90][25], mainMap[91][25], mainMap[92][25], mainMap[93][25], mainMap[94][25], mainMap[95][25], mainMap[96][25], mainMap[97][25], mainMap[98][25], mainMap[99][25], mainMap[100][25], mainMap[101][25], mainMap[102][25], mainMap[103][25], mainMap[104][25], mainMap[105][25], mainMap[106][25], mainMap[107][25], mainMap[108][25], mainMap[109][25], mainMap[110][25], mainMap[111][25], mainMap[112][25], mainMap[113][25], mainMap[114][25], mainMap[115][25], mainMap[116][25], mainMap[117][25], mainMap[118][25], mainMap[119][25], mainMap[0][26], mainMap[1][26], mainMap[2][26], mainMap[3][26], mainMap[4][26], mainMap[5][26], mainMap[6][26], mainMap[7][26], mainMap[8][26], mainMap[9][26], mainMap[10][26], mainMap[11][26], mainMap[12][26], mainMap[13][26], mainMap[14][26], mainMap[15][26], mainMap[16][26], mainMap[17][26], mainMap[18][26], mainMap[19][26], mainMap[20][26], mainMap[21][26], mainMap[22][26], mainMap[23][26], mainMap[24][26], mainMap[25][26], mainMap[26][26], mainMap[27][26], mainMap[28][26], mainMap[29][26], mainMap[30][26], mainMap[31][26], mainMap[32][26], mainMap[33][26], mainMap[34][26], mainMap[35][26], mainMap[36][26], mainMap[37][26], mainMap[38][26], mainMap[39][26], mainMap[40][26], mainMap[41][26], mainMap[42][26], mainMap[43][26], mainMap[44][26], mainMap[45][26], mainMap[46][26], mainMap[47][26], mainMap[48][26], mainMap[49][26], mainMap[50][26], mainMap[51][26], mainMap[52][26], mainMap[53][26], mainMap[54][26], mainMap[55][26], mainMap[56][26], mainMap[57][26], mainMap[58][26], mainMap[59][26], mainMap[60][26], mainMap[61][26], mainMap[62][26], mainMap[63][26], mainMap[64][26], mainMap[65][26], mainMap[66][26], mainMap[67][26], mainMap[68][26], mainMap[69][26], mainMap[70][26], mainMap[71][26], mainMap[72][26], mainMap[73][26], mainMap[74][26], mainMap[75][26], mainMap[76][26], mainMap[77][26], mainMap[78][26], mainMap[79][26], mainMap[80][26], mainMap[81][26], mainMap[82][26], mainMap[83][26], mainMap[84][26], mainMap[85][26], mainMap[86][26], mainMap[87][26], mainMap[88][26], mainMap[89][26], mainMap[90][26], mainMap[91][26], mainMap[92][26], mainMap[93][26], mainMap[94][26], mainMap[95][26], mainMap[96][26], mainMap[97][26], mainMap[98][26], mainMap[99][26], mainMap[100][26], mainMap[101][26], mainMap[102][26], mainMap[103][26], mainMap[104][26], mainMap[105][26], mainMap[106][26], mainMap[107][26], mainMap[108][26], mainMap[109][26], mainMap[110][26], mainMap[111][26], mainMap[112][26], mainMap[113][26], mainMap[114][26], mainMap[115][26], mainMap[116][26], mainMap[117][26], mainMap[118][26], mainMap[119][26]);

        for(x=0;x<MMX-1;x++)
            printf("=");
            printf("\n");
    }
    mainMap[xp][yp]=hold;
    return;
}

int mainMenu()
{
    if (debug==1)
        printf("\nmainmenu\n");
    //char arrays[] = {'5','\0'};
    system("cls");
    disMainMenue();

    char x[75] = {'\0'};
    int y = 0;

    GetInput(0, x);
    y = readMenu(x);

    if (debug == 1)
    printf("\n%d", y);

    return y;
}

int readMenu(char x[75])
{
    if (debug==1)
        printf("\readMenu\n");
    int check = 0;
    int y = 0;

    SpecialClear(x);//clear case formatting
    while(y==0)
    {
        if (compareStrings(x, "newgame") == 1)
        {
            if (debug==1)
                printf("\1true\n");
            y = 1;
        }
        else if (compareStrings(x, "1newgame") == 1)
        {
            y = 1;
        }
        else if (compareStrings(x, "1") == 1)
        {
            y = 1;
        }
        else if (compareStrings(x, "2loadgame") == 1)
        {
            y = 2;
        }
        else if (compareStrings(x, "loadgame") == 1)
        {
            y = 2;
        }
        else if (compareStrings(x, "2") == 1)
        {
            y = 2;
        }
        else if (compareStrings(x, "cheats") == 1)
        {
            y = 3;
        }
        else if (compareStrings(x, "3") == 1)
        {
            y = 3;
        }
        else if (compareStrings(x, "3cheats") == 1)
        {
            y = 3;
        }
        else if (compareStrings(x, "exit") == 1)
        {
            y = 4;
        }
        else if (compareStrings(x, "4exit") == 1)
        {
            y = 4;
        }
        else if (compareStrings(x, "4") == 1)
        {
            y = 4;
        }
        else
        {
            y=0;
            sptype("I didn't quite get that, please try again: ");
            GetInput(0, x);
            SpecialClear(x);
        }
    }

    return y;
}

///this can be used in logic as a t/f.
//checks if the strings are the same.
//returns 1 if they are 0 if they aren't.
int compareStrings(char string1[],char string2[])
{
    if (debug == 1)
    printf("\nruns comparestrings\n");
    int x = 0, t = 1;
    while(string1[x] != '\0')
    {
        if (detaildebug==1)
            printf("%c %c ", string1[x], string2[x]);
        if (string1[x] != string2[x])
        {
            if (detaildebug==1)
                printf("F ");
            t=0;
        }
        if (detaildebug==1)
            printf("ComSTR:%d\n", x);
        x++;
    }
    if (!((string1[x]=='\0')&&(string2[x]=='\0')))
        t=0;


    if (debug == 1)
        printf(" %d\n",t);

    return t;
}

void CaseClear(char stringstorage[])
{
    int i = 0;
    if (debug == 1)
    printf("runs caseclear");
    while (stringstorage[i] != '\0')
    {
    if (detaildebug == 1)
    printf("\n%d\n",i);
        if (stringstorage[i] == 'A')
            stringstorage[i]='a';

        else if (stringstorage[i] == 'B')
            stringstorage[i]='b';

        else if (stringstorage[i] == 'C')
            stringstorage[i]='c';

        else if (stringstorage[i] == 'D')
            stringstorage[i]='d';

        else if (stringstorage[i] == 'E')
            stringstorage[i]='e';

        else if (stringstorage[i] == 'F')
            stringstorage[i]='f';

        else if (stringstorage[i] == 'G')
            stringstorage[i]='g';

        else if (stringstorage[i] == 'H')
            stringstorage[i]='h';

        else if (stringstorage[i] == 'I')
            stringstorage[i]='i';

        else if (stringstorage[i] == 'J')
            stringstorage[i]='j';

        else if (stringstorage[i] == 'K')
            stringstorage[i]='k';

        else if (stringstorage[i] == 'L')
            stringstorage[i]='l';

        else if (stringstorage[i] == 'M')
            stringstorage[i]='m';

        else if (stringstorage[i] == 'N')
            stringstorage[i]='n';

        else if (stringstorage[i] == 'O')
            stringstorage[i]='o';

        else if (stringstorage[i] == 'P')
            stringstorage[i]='p';

        else if (stringstorage[i] == 'Q')
            stringstorage[i]='q';

        else if (stringstorage[i] == 'R')
            stringstorage[i]='r';

        else if (stringstorage[i] == 'S')
            stringstorage[i]='s';

        else if (stringstorage[i] == 'T')
            stringstorage[i]='t';

        else if (stringstorage[i] == 'U')
            stringstorage[i]='u';

        else if (stringstorage[i] == 'V')
            stringstorage[i]='v';

        else if (stringstorage[i] == 'W')
            stringstorage[i]='w';

        else if (stringstorage[i] == 'X')
            stringstorage[i]='x';

        else if (stringstorage[i] == 'Y')
            stringstorage[i]='y';

        else if (stringstorage[i] == 'Z')
            stringstorage[i]='z';
        else
            ;
        i++;
    }
    return;
}

void GetInput(int x, char arrays[])
{
    if (debug==1)
        printf("\nget input\n");
    char y=' ';
    if (x==1)
    {
        int i = 0;
        scanf("%c", &y);
        while (y != '\n')
        {
            arrays[i]=y;
            i++;
            scanf("%c", &y);
        }
        arrays[i]='\0';
    }
    else if (x==0)
    {
        int i = 0;
        scanf(" %c", &y);
        while (y != '\n')
        {
            arrays[i]=y;
            i++;
            scanf("%c", &y);
        }
        arrays[i]='\0';
    }
    else
        printf("error in GetInput");
    return;
}

void disMainMenue()
{
    printf("*******************************************\n");
    printf("*   1 - new game                          *\n");
    printf("*   2 - load game                         *\n");
    printf("*   3 - cheats                            *\n");
    printf("*                                         *\n");
    printf("*   4 - exit                              *\n");
    printf("*******************************************\n");
    return;
}

void SpecialClear(char stringstorage[])
{

    if (debug == 1)
        printf("\nSpecialClear\n");
    int i = 0;
    while (stringstorage[i] != '\0')
    {
    if (detaildebug == 1)
    printf("\nSpClr%d\n",i);
        if (stringstorage[i] == 'A')
            stringstorage[i]='a';

        else if (stringstorage[i] == 'B')
            stringstorage[i]='b';

        else if (stringstorage[i] == 'C')
            stringstorage[i]='c';

        else if (stringstorage[i] == 'D')
            stringstorage[i]='d';

        else if (stringstorage[i] == 'E')
            stringstorage[i]='e';

        else if (stringstorage[i] == 'F')
            stringstorage[i]='f';

        else if (stringstorage[i] == 'G')
            stringstorage[i]='g';

        else if (stringstorage[i] == 'H')
            stringstorage[i]='h';

        else if (stringstorage[i] == 'I')
            stringstorage[i]='i';

        else if (stringstorage[i] == 'J')
            stringstorage[i]='j';

        else if (stringstorage[i] == 'K')
            stringstorage[i]='k';

        else if (stringstorage[i] == 'L')
            stringstorage[i]='l';

        else if (stringstorage[i] == 'M')
            stringstorage[i]='m';

        else if (stringstorage[i] == 'N')
            stringstorage[i]='n';

        else if (stringstorage[i] == 'O')
            stringstorage[i]='o';

        else if (stringstorage[i] == 'P')
            stringstorage[i]='p';

        else if (stringstorage[i] == 'Q')
            stringstorage[i]='q';

        else if (stringstorage[i] == 'R')
            stringstorage[i]='r';

        else if (stringstorage[i] == 'S')
            stringstorage[i]='s';

        else if (stringstorage[i] == 'T')
            stringstorage[i]='t';

        else if (stringstorage[i] == 'U')
            stringstorage[i]='u';

        else if (stringstorage[i] == 'V')
            stringstorage[i]='v';

        else if (stringstorage[i] == 'W')
            stringstorage[i]='w';

        else if (stringstorage[i] == 'X')
            stringstorage[i]='x';

        else if (stringstorage[i] == 'Y')
            stringstorage[i]='y';

        else if (stringstorage[i] == 'Z')
            stringstorage[i]='z';
        else if ((stringstorage[i] == '1') || (stringstorage[i] == '2') || (stringstorage[i] == '3') || (stringstorage[i] == '4') || (stringstorage[i] == '5') || (stringstorage[i] == '6') || (stringstorage[i] == '7') || (stringstorage[i] == '8') || (stringstorage[i] == '9') || (stringstorage[i] == '0'))
            ;
        else if(stringstorage[i]=='\0')
            ;
        else if((stringstorage[i]=='a') || (stringstorage[i]=='b') || (stringstorage[i]=='c') || (stringstorage[i]=='d') || (stringstorage[i]=='e') || (stringstorage[i]=='f') || (stringstorage[i]=='g') || (stringstorage[i]=='h') || (stringstorage[i]=='i') || (stringstorage[i]=='j') || (stringstorage[i]=='k') || (stringstorage[i]=='l') || (stringstorage[i]=='m') || (stringstorage[i]=='n') || (stringstorage[i]=='o') || (stringstorage[i]=='p') || (stringstorage[i]=='q') || (stringstorage[i]=='r') || (stringstorage[i]=='s') || (stringstorage[i]=='t') || (stringstorage[i]=='u') || (stringstorage[i]=='v') || (stringstorage[i]=='w') || (stringstorage[i]=='x') || (stringstorage[i]=='y') || (stringstorage[i]=='z'))
            ;
        else
            stringstorage[i]=' ';
        i++;
    }
    int x = 0;
    int y = 0;
    while (stringstorage[y]!='\0')
    {
        stringstorage[x]=stringstorage[y];
        if (stringstorage[x]!=' ')
            x++;
        y++;
    }
    stringstorage[x]=stringstorage[y];
    return;
}

void AllClear(char stringstorage[])
{
    if (debug == 1)
    printf("\nAllClear\n");
    int i = 0;
    while (stringstorage[i] != '\0')
    {
    if (detaildebug == 1)
    printf("\nAllClr %d\n",i);
        if (stringstorage[i] == 'A')
            stringstorage[i]='a';

        else if (stringstorage[i] == 'B')
            stringstorage[i]='b';

        else if (stringstorage[i] == 'C')
            stringstorage[i]='c';

        else if (stringstorage[i] == 'D')
            stringstorage[i]='d';

        else if (stringstorage[i] == 'E')
            stringstorage[i]='e';

        else if (stringstorage[i] == 'F')
            stringstorage[i]='f';

        else if (stringstorage[i] == 'G')
            stringstorage[i]='g';

        else if (stringstorage[i] == 'H')
            stringstorage[i]='h';

        else if (stringstorage[i] == 'I')
            stringstorage[i]='i';

        else if (stringstorage[i] == 'J')
            stringstorage[i]='j';

        else if (stringstorage[i] == 'K')
            stringstorage[i]='k';

        else if (stringstorage[i] == 'L')
            stringstorage[i]='l';

        else if (stringstorage[i] == 'M')
            stringstorage[i]='m';

        else if (stringstorage[i] == 'N')
            stringstorage[i]='n';

        else if (stringstorage[i] == 'O')
            stringstorage[i]='o';

        else if (stringstorage[i] == 'P')
            stringstorage[i]='p';

        else if (stringstorage[i] == 'Q')
            stringstorage[i]='q';

        else if (stringstorage[i] == 'R')
            stringstorage[i]='r';

        else if (stringstorage[i] == 'S')
            stringstorage[i]='s';

        else if (stringstorage[i] == 'T')
            stringstorage[i]='t';

        else if (stringstorage[i] == 'U')
            stringstorage[i]='u';

        else if (stringstorage[i] == 'V')
            stringstorage[i]='v';

        else if (stringstorage[i] == 'W')
            stringstorage[i]='w';

        else if (stringstorage[i] == 'X')
            stringstorage[i]='x';

        else if (stringstorage[i] == 'Y')
            stringstorage[i]='y';

        else if (stringstorage[i] == 'Z')
            stringstorage[i]='z';

        else if(stringstorage[i]=='\0')
            ;

        else if((stringstorage[i]=='a') || (stringstorage[i]=='b') || (stringstorage[i]=='c') || (stringstorage[i]=='d') || (stringstorage[i]=='e') || (stringstorage[i]=='f') || (stringstorage[i]=='g') || (stringstorage[i]=='h') || (stringstorage[i]=='i') || (stringstorage[i]=='j') || (stringstorage[i]=='k') || (stringstorage[i]=='l') || (stringstorage[i]=='m') || (stringstorage[i]=='n') || (stringstorage[i]=='o') || (stringstorage[i]=='p') || (stringstorage[i]=='q') || (stringstorage[i]=='r') || (stringstorage[i]=='s') || (stringstorage[i]=='t') || (stringstorage[i]=='u') || (stringstorage[i]=='v') || (stringstorage[i]=='w') || (stringstorage[i]=='x') || (stringstorage[i]=='y') || (stringstorage[i]=='z'))
            ;

        else
            stringstorage[i]=' ';
        i++;
    }
    int x = 0;
    int y = 0;
    while (stringstorage[y]!='\0')
    {
        stringstorage[x]=stringstorage[y];
        if (stringstorage[x]!=' ')
            x++;
        y++;
    }
    stringstorage[x]=stringstorage[y];
    return;
}

//this code is run on a newgame condition from the main menu
void newgame(char name[])//name is passed in to get the players username
{
    //initialize variables and arrays
    char confirm[]={'\0'};
    int check = 0;
    int scheck = 0;
    int t1,t2;
    //start new game
    printf("starting new game");
    system("cls");
    sptype("starting new game...");

    //code to get the player's name
    do
    {
        printf("\n\n");
        system("cls");
        sptype("Please enter your name: ");
        GetInput(0, name);
        sptype("\nI see, your name is ");
        sptype(name);
        sptype(", Correct? y/n ");
        GetInput(0,confirm);
        AllClear(confirm);

        //use compare strings to check if the usr entered yes or no
        do
        {
            AllClear(confirm);
            if ((t1=compareStrings(confirm, "y")) || (t2=compareStrings(confirm, "yes")))
            {

                scheck = 1;
                check = 1;
            }
            else if((t1=compareStrings(confirm, "n")) || (t2=compareStrings(confirm, "no")))
            {
                check = 0;
                scheck = 1;
            }
            else
            {
                scheck = 0;
                sptype("\nI didn't quite get that, could you confirm again? y/n ");
                GetInput(0, confirm);
            }
        }while (scheck == 0);
    }while (check == 0);         //ends name loops

    //clear screen and begin the introduction to the game.
    system("cls");
    char temp[]={'\0'};
    chat("system", "Welcome @n, to Esmyria.  Esmyria is a land of many wonders, where the forces of light and darkness clash continuously in the struggle for the upper hand.  Many heroes seek to make a name for themselves here... but that is not where your story begins @n.  After all, you've only ever wished for your quiet, peaceful life with your family and friends in your hometown secluded in the northwest most part of Esmyria, deep in the forest.  It's one of the few places in the country where one can avoid the daily conflicts and power struggles of the country, but... nothing stays the same forever...", UV);
    //GetInput(0, temp);
    //if(!compareStrings(temp, "y") && !compareStrings(temp, "yes") && !compareStrings(temp, "yesiamready") && !compareStrings(temp, "iamready") && !compareStrings(temp, "imready"))
        //sptype("too bad\n");
    //else
        //sptype("Go now to Esmyria")
    sptype("\n\n\n\n\n");
    chat("Woman's Voice", "@p@n!", UV);
    chat(name, "@p\t\a!@p@p", UV);
    chat(name, "*I feel so tired, did I fall asleep on the hay?", UV);
    chat("Woman's Voice", "@n, @n!", UV);
    chat(name, "*And who's yelling for me?", UV);
    chat("Woman's Voice", "@n@p@p, @n!@p@p  Where are you @n!?@p@p@p@p  @n if you were slacking off again!@p", UV);
    chat(name, "@p\t\a!@p@p", UV);
    chat(name, "*Oh no, I was supposed to be feeding the horses!@p", UV);
    chat(name, "I'm right here mother!@p", UV);
    chat("Mother", "You weren't sleeping instead of doing your chores again, were you?@p", UV);
    chat(name, "No mom!@p  Of course not!, I was just going to start feeding the horses like you asked my to.", UV);
    while (getch()==EOF);
    return;
}

//delays with a null statement loopint
void delay(double x)
{
    if(!noDelay)
    {
        double S=(CLOCKS_PER_SEC/1000)*x;
        clock_t T=clock()+S;
        clock_t T2=0;
            while(T2<=T)
            {
                T2=clock();
            }
    }
    return;
}

//the sptype function prints out one character at a time and takes ten times longer o print '.'s
void sptype(char x[])
{
    int i = 0;
    while (x[i]!='\0')
    {   //prints characters one at a time
        if (!(x[i]=='@'&&x[i+1]=='p'))
        {
            printf("%c", x[i]);
            delay(30);
        }
        else
        {
            delay(500);
            i++;
        }
        //takes extra time to print '.'
        if (x[i-1]=='.')
            delay(650);
        i++;
    }
}

void chat(char c[], char d[],char name[])
{
    if(debug==1)
        printf("\nchat func\n");
    //convert c & d from hard coded strings to alterable arrays
    char x[500000]={'\0'};
    char y[500000]={'\0'};
    int b=0;

    while (c[b]!='\0')
    {
        y[b] = c[b];
        b++;
    }
    y[b]='\0';
    insert( d, x, name);


    int width, width2;
    int hold=0;
    char z[500000]={'\0'};
    for (width=0;y[width]!='\0';width++);
    for (width2=0;x[width2]!='\0';width2++);

    //int print1=0;
    if ((width2<89)&&(width<25))
    {
        //if (debug==1)
            //printf("\n%s %s\n", y, x);
        sptype(y);
        sptype(":");
        sptype(x);
        sptype("\n");
        //if (debug==1)
            //printf("\n%s %s\n", y, x);
    }
    else if ((114-width)>=25)
    {
        do
        {
            //if(detaildebug==1)
                //printf("\nchat %d\n",print1++);
            sptype(y);
            sptype(":");
            hold = stbreak(hold, (114-width), z, x);
            sptype(z);
            sptype("\n");
        }while (hold!=(-1));
    }
    else
        printf("\nerror in chat! identifier too long!\n\n");
  return;
}

int stbreak(int s, int e, char x[], char y[])
{
        if(debug==1)
            printf("\nstbreak function\n");
        int i = 0;
        int ii = s;
        while (((y[ii])!='\0')&&(i<e))
        {
            x[i]=y[ii];
            if (y[ii]=='@'&&y[ii+1]=='p')
                e+=2;
            if(detaildebug==1)
                printf("\nstbreak %d\n",ii);
            if  (!((i==0) && (x[i]==' ')))
                i++;
            ii++;
        }
        if (x[i-1]!=' '&&y[ii]!='\0')
        {
            x[i]='-';
            x[i+1]='\0';
        }
        else
            x[i]='\0';

        if ((y[ii]=='\0'))
        {
            ii = (-1);
        }
        return ii;
}

void insert(char x[],char y[],char name[])
{
    if(debug==1)
        printf("\ninsert func\n");
    int i = 0;
    int width;
    for (width=0;x[width]!='\0';width++);
    int ii = 0;
    while(x[i]!='\0')
    {
        //if(debug==1)
        //printf("\n%s\t%c\t%c\n",y,x[i],y[ii]);
            if(detaildebug==1)
                printf("\ninsert %d",i);
            if (x[i]!='@')
            {
                y[ii]=x[i];
                i++;
                ii++;
            }
            else if (x[i]=='@')
            {
                if(x[i+1]=='@')
                {
                    y[ii]='@';
                    i+=2;
                    ii++;
                }
                else if(x[i+1]=='n')
                {
                    ii=insertProcess(y,name,ii);
                    i=i+2;
                }
                else if(x[i+1]=='p')
                {
                    ii=insertProcess(y,"@p",ii);
                    i=i+2;
                }
                else if(x[i+1]=='b')
                {
                    ii=insertProcess(y,"boat_NAme",ii);
                    i=2+i;
                }
                else
                {
                    printf("\nerror in insert, stray '@' IN STRING!\n");
                }
            }
    }

    if(debug==1)
        printf("\n%s\t%c\t%c\n",y,x[i],y[ii]);
    y[ii]='\0';
    return;
}

insertProcess(char y[],char z[], int ii)
{
    if(debug==1)
        printf("\nInsertPROCESS func\n");
    int a;
    for (a=0;z[a]!='\0';a++)
    {
        if(detaildebug==1)
            printf("\ninsertProcess %d %d\n", a, ii);
        y[ii]=z[a];
        ii++;
    }
    if(debug==1)
        printf("\n%s\n",y);
    return ii;
}

void title()
{
    char a='a';
    char b[]={'\0'};
    if(CHECKKEY)
        gets(b);
    do
    {
        system("cls");
        printf("            /\\        x000x    x000x   x0      0x  x000x  x000x    xxxxx     xxx   \n");
        printf("           /  \\       0       x        0 0    0 0  0      0    x     0      0   0  \n");
        printf("          /____\\      0      0         0 x    x 0  0      0    0     0     0     0 \n");
        printf("         | |  | |     0       x        0  x  x  0  0      0   x      0     0     0 \n");
        printf("         | |  | |     x000x    x00x    0  0  0  0  x000x  x00x0      0     xxxxxxx \n");
        printf("         | |  | |     0            x   0  x  x  0  0      0    0     0     0     0 \n");
        printf("         |_|__|_|     0             0  0   xx   0  0      0    0     0     0     0 \n");
        printf("          \\    /      0            x   x   00   x  0      0     0    0    x       x\n");
        printf("           \\  /       x000x   x000x    x   xx   x  x000x  0     0  xxxxx  x       x\n");
        printf("            \\/                          press -Space bar to start                  \n");

        delay(250);
        if(CHECKKEY)
            if((a=NBGETCHAR)==' ')
                break;

        system("cls");
        printf("            /\\        XOOOX    XOOOX   XO      OX  XOOOX  XOOOX    XXXXX     XXX   \n");
        printf("           /  \\       O       X        O O    O O  O      O    X     O      O   O  \n");
        printf("          /____\\      O      O         O X    X O  O      O    O     O     O     O \n");
        printf("          | |  | |    O       X        O  X  X  O  O      O   X      O     O     O \n");
        printf("          | |  | |    XOOOX    XOOX    O  O  O  O  XOOOX  XOOXO      O     XXXXXXX \n");
        printf("          | |  | |    O            X   O  X  X  O  O      O    O     O     O     O \n");
        printf("          |_|__|_|    O             O  O   XX   O  O      O    O     O     O     O \n");
        printf("          \\    /      O            X   X   OO   X  O      O     O    O    X       X\n");
        printf("           \\  /       XOOOX   XOOOX    X   XX   X  XOOOX  O     O  XXXXX  X       X\n");
        printf("            \\/                          PRESS -SPACE BAR TO START                  \n");

        delay(500);
        if(CHECKKEY)
            if((a=NBGETCHAR)==' ')
                break;

        system("cls");
        printf("            /\\        x000x    x000x   x0      0x  x000x  x000x    xxxxx     xxx   \n");
        printf("           /  \\       0       x        0 0    0 0  0      0    x     0      0   0  \n");
        printf("          /____\\      0      0         0 x    x 0  0      0    0     0     0     0 \n");
        printf("         | |  | |     0       x        0  x  x  0  0      0   x      0     0     0 \n");
        printf("         | |  | |     x000x    x00x    0  0  0  0  x000x  x00x0      0     xxxxxxx \n");
        printf("         | |  | |     0            x   0  x  x  0  0      0    0     0     0     0 \n");
        printf("         |_|__|_|     0             0  0   xx   0  0      0    0     0     0     0 \n");
        printf("          \\    /      0            x   x   00   x  0      0     0    0    x       x\n");
        printf("           \\  /       x000x   x000x    x   xx   x  x000x  0     0  xxxxx  x       x\n");
        printf("            \\/                          press -Space bar to start                  \n");

        delay(250);
        if(CHECKKEY)
            if((a=NBGETCHAR)==' ')
                break;

        system("cls");
        printf("            /\\        *ooo*    *ooo*   *o      o*  *ooo*  *ooo*    *****     ***   \n");
        printf("           /  \\       o       *        o o    o o  o      o    *     o      o   o  \n");
        printf("          /____\\      o      o         o *    * o  o      o    o     o     o     o \n");
        printf("          | |  | |    o       *        o  *  *  o  o      o   *      o     o     o \n");
        printf("          | |  | |    *ooo*    *oo*    o  o  o  o  *ooo*  *oo*o      o     ******* \n");
        printf("          | |  | |    o            *   o  *  *  o  o      o    o     o     o     o \n");
        printf("          |_|__|_|    o             o  o   **   o  o      o    o     o     o     o \n");
        printf("          \\    /      o            *   *   oo   *  o      o     o    o    *       *\n");
        printf("           \\  /       *ooo*   *ooo*    *   **   *  *ooo*  o     o  *****  *       *\n");
        printf("            \\/                          ***** _***** ** *****                      \n");

        delay(200);
        if(CHECKKEY)
            if((a=NBGETCHAR)==' ')
                break;

        system("cls");
        printf("            /\\ \n");
        printf("           /  \\ \n");
        printf("          /____\\ \n");
        printf("         | |  | | \n");
        printf("         | |  | | \n");
        printf("         | |  | | \n");
        printf("         |_|__|_| \n");
        printf("          \\    / \n");
        printf("           \\  / \n");
        printf("            \\/ \n");

        delay(500);
        if(CHECKKEY)
            if((a=NBGETCHAR)==' ')
                break;

        system("cls");
        printf("            /\\        *ooo*    *ooo*   *o      o*  *ooo*  *ooo*    *****     ***   \n");
        printf("           /  \\       o       *        o o    o o  o      o    *     o      o   o  \n");
        printf("          /____\\      o      o         o *    * o  o      o    o     o     o     o \n");
        printf("          | |  | |    o       *        o  *  *  o  o      o   *      o     o     o \n");
        printf("          | |  | |    *ooo*    *oo*    o  o  o  o  *ooo*  *oo*o      o     ******* \n");
        printf("          | |  | |    o            *   o  *  *  o  o      o    o     o     o     o \n");
        printf("          |_|__|_|    o             o  o   **   o  o      o    o     o     o     o \n");
        printf("          \\    /      o            *   *   oo   *  o      o     o    o    *       *\n");
        printf("           \\  /       *ooo*   *ooo*    *   **   *  *ooo*  o     o  *****  *       *\n");
        printf("            \\/                          ***** _***** ** *****                      \n");

        delay(200);
        if(CHECKKEY)
            a=NBGETCHAR;
    } while ((a)!=' ');
  return;
}

int walk(int MapID,int* x_Pos,int* y_Pos,char journey[], MAPSTRINGS, char name[])
{
    int x = 0, y = 0, var = 0, c = 0, loop2 = 0, loop1 = 0;
    char walking;
    char check[200]={'\0'};
    char toss[]={'\0'};
    do
    {
        printf("System: wasd to move, q to quit...");
        do
        {
            walking=NBGETCHAR;

            if (walking=='w')
            {
                y = (*y_Pos - 1);
                x = (*x_Pos);
                loop2=1;
            }
            else if (walking=='s')
            {
                y = (*y_Pos + 1);
                x = (*x_Pos);
                loop2=1;
            }
            else if (walking=='d')
            {
                x = (*x_Pos + 1);
                y = (*y_Pos);
                loop2=1;
            }
            else if (walking=='a')
            {
                x = (*x_Pos - 1);
                y = (*y_Pos);
                loop2=1;
            }
            else if (walking=='q')
            {
                //save
                var = 1;
                loop2=1;
            }
            else if (walking=='e')
            {
                //save and quit
                var = 2;
                loop2=1;
            }
            else if (walking=='e')
            {
                //quit
                var = 3;
                loop2=1;
            }
            else
                loop2=0;
        }while (loop2==0);

        c=checkL(MapID, x, y, check, journey, MAPSTRINGA);
        if (c==1)
        {
            *x_Pos=x;
            *y_Pos=y;
            loop1=0;
        }
        else
        {
            if(compareStrings(check, "Mountain"))
                chat(name, "A mountain blocks my path.@p", UV);
            else if(compareStrings(check, "drown"))
                chat(name, "I can't swim across the ocean!@p@p", UV);
            else if(compareStrings(check, "fall"))
                chat(name, "If I go any further, I'll fall off the edge of the world!@p@p", UV);
            else
                chat(name, "I can't do that!@p@p", UV);
            x = y = var =c = loop2 = 0;
            printmap(*x_Pos, *y_Pos, MapID, MAPSTRINGA);
            CStr(check,"\0");
            loop1=1;
        }
        delay(100);
        while(CHECKKEY)
            NBGETCHAR;
    }while (loop1);
    return var;
}

int checkL(int MapID,int x,int y, char var[200], char journey[], MAPSTRINGS)
{
    int check=1;
    if (MapID==MAINMAP)
    {
        if (debug==1)
            printf("\ncheckL\n");
        if ((x>(MMX-1))||(x<0)||(y<0)||(y>MMY-1))
        {
            check=0;
            CStr(var,"fall");
        }
        else if ((journey[1]=='0')&&(mainMap[x][y]==WATER))
        {
            check=0;
            CStr(var,"drown");
        }
        else if ((journey[1]=='1')&&(mainMap[x][y]==WATER))
            CStr(var,"b_boat");
        else if ((journey[2]=='0')&&(mainMap[x][y]==MOUNTAINS))
        {
            check=0;
            CStr(var,"Mountain");
        }
        else if ((journey[2]=='1')&&(mainMap[x][y]==MOUNTAINS))
            CStr(var,"Climb");
        else if (x==0&&y==0)
            CStr(var,"home");
        else if ((mainMap[x][y]=='T')||(mainMap[x][y]=='q')||(mainMap[x][y]==':')||(mainMap[x][y]=='O')||(mainMap[x][y]=='X'))
            check=1;
        else
        {
            printf("error in checkL");
            check = 0;
        }
    }
    return check;
}

void CStr(char str1[],char str2[])
{
    int i = 0;
    while (str2[i]!='\0')
    {
        str1[i]=str2[i];
        i++;
    }
    return;
}
