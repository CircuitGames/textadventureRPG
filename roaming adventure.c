
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

#define USRINPUT 500 //this is the limit on a user input string max length

//invalid data, either correct, undo, initiate a failsafe/fall-back
#define INVALID 0

///only 0-9 are valid for this input
//land types
#define FOREST              'T'   //T
#define PATH                'O'   //O
#define DESSERT             ':'   //:
#define WATER               '^'   //^
#define PLAINS              'i'   //i
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
            sptype("not implemented yet");
            break;
        case 3:
            sptype("not implemented yet");
            break;
        case 4:
            sptype("exiting...");
            break;
    }
    loadMaps(mainMap);

    if (mstate==1)
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
    if (fp == 0)
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
    if (debug == 0)
        system("clear");
    if (debug==1)
        printf("\nprintmap\n");

    int x = 0;
    int y = 0;

    if (MapID == MAINMAP)
    {
        for(x=0;x<MMX-1;x++)
            printf("=");
            printf("\n");
        for(y=0;y<MMY;y++)
        {
            for(x=0;x<MMX;x++)
            {
                if (!(x==xp && y==yp))
                printf("%c", mainMap[x][y]);
                else
                    printf("@");
            }
        }
            printf("\n");
        for(x=0;x<MMX-1;x++)
            printf("=");
            printf("\n");
    }
    return;
}

int mainMenu()
{
    if (debug==1)
        printf("\nmainmenu\n");
    //char arrays[] = {'5','\0'};
    system("clear");
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
    system("clear");
    sptype("starting new game...");

    //code to get the player's name
    do
    {
        printf("\n\n");
        system("clear");
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
    system("clear");
    char temp[]={'\0'};
    chat("system", "welcome @n, to Esmyria.  Esmyria is a land of many wonders, where the forces of light and darkness clash continuously in the struggle for the upper hand.  Many heroes seek to make a name for themselves here... but that is not where your story begins @n.  After all, you've only ever wished for your quiet, peaceful life with your family and friends in your hometown secluded in the northwest most part of Esmyria, deep in the forest.  It's one of the few places in the country where one can avoid the daily conflicts and power struggles of the country, but... nothing stays the same forever...", UV);
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
    printf("press any key to continue...");
    getch();
    return;
}

//delays with a null statement loopint
void delay(double x)
{
    double S=(CLOCKS_PER_SEC/1000)*x;
    clock_t T=clock()+S;
    clock_t T2=0;
        while(T2<=T)
        {
            T2=clock();
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
            printf("%c", x[i]);
        delay(30);
        if(x[i]=='@'&&x[i+1]=='p')
            delay(500);
        //takes extra time to print '.'
        if (x[i+1]=='.')
            delay(650);
        if ((x[i]=='@'&&x[i+1]=='p'))
            i++;
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
    printf("                                                                                                                       \n");
    printf("                                                                                                                       \n");
    printf("                                                                                                                       \n");
    printf("                                                                                                                       \n");
    printf("                                                                                                                       \n");
    printf("                                                                                                                       \n");
    printf("                                                                                                                       \n");
    printf("                                                                                                                       \n");
    printf("                                                                                                                       \n");
    printf("                                                                                                                       \n");
  return;
}

int walk(int MapID,int* x_Pos,int* y_Pos,char journey[], MAPSTRINGS, char name[])
{
    int x = 0, y = 0, var = 0, c = 0, loop2 = 0, loop1 = 0;
    char walking;
    char check[200]={'\0'};
    do
    {
        do
        {
            printf("System: WASD to move.  Q or E to quit.");
            walking=getch();
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
            ///this will eventually save the game, currently it quits
            else if (walking=='e')
            {
                var = 1;
                loop2=1;
            }
            ///this is a place holder for a function at is not yet implemented and redundent
            //else if (walking=='save and quit')
            //{
            //    var = 2;
            //    loop2=1;
            //}
            else if(walking=='q')
            {
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
                chat(name, "A mountain blocks your path.\n", UV);
            else if(compareStrings(check, "drown"))
                chat(name, "I can't swim across the ocean!\n", UV);
            else if(compareStrings(check, "fall\0"))
                chat(name, "If I go any further, I'll fall off the edge of the world!\n", UV);
            else
                chat(name, "I can't do that", UV);
            system("clear");
            x = y = var =c = loop2 = 0;
            printmap(*x_Pos, *y_Pos, MapID, MAPSTRINGA);
            CStr(check,"\0");
            CStr(walking,"\0");
            loop1=1;
        }
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
