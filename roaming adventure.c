
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
#define FOREST              '1'   //m
#define PATH                '2'   //O
#define DESSERT             '3'   //:
#define WATER               '4'   //w
#define PLAINS              '5'   //i
#define SPECIAL             '6'   //X
#define MOUNTAINS           '7'   //M

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

//global variables
int debug = 1;
int detaildebug = 1;

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
void delay(float x);
void sptype(char x[]);
void chat(char x[]);
void title();
void walk(int MapID,int* x_Pos,int* y_Pos, MAPSTRINGS);



int main()
{
    //initialize variables
    int x_Pos = 0, y_Pos = 0, location = 0, MapID = 0, holdData = 0, dimx = 0, dimy = 0, mstate = 0;
    // CMap[] = {'\0'}, cave1[], cave2[], temple[], waterPalace[];
    char  name[] = {'\0'}, mainMap[MMX][MMY];

    mstate=mainMenu();
    switch (mstate)
    {
        case 1:newgame(name);
            break;
        case 2:sptype("not implemented yet");
            break;
        case 3:sptype("not implemented yet");
            break;
        case 4:sptype("exiting...");
            break;
    }

    if (mstate!=4)
    {
        loadMaps(mainMap);

        //setCurrentMap(&MapID, CMap, &dimx, &dimy, MAPSTRINGA);
        printmap(x_Pos, y_Pos, MAINMAP, MAPSTRINGA);
        walk(MapID, &x_Pos, &y_Pos, MAPSTRINGA);

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
        printf("ERROR!  couldn't open MainMap.dat!");
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
        system("cls");
    if (debug==1)
        printf("\nprintmap\n");

    int x = 0;
    int y = 0;

    if (MapID == MAINMAP)
    {
        for(x=0;x<MMX;x++)
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
        for(x=0;x<MMX;x++)
            printf("=");
    }
    return;
}

int mainMenu()
{
    if (debug==1)
        printf("\nmainmenu\n");
    //char arrays[] = {'5','\0'};
    system("cls");
    disMainMenue();

    char x[] = {'\0'};
    int y = 0;

    GetInput(0, x);
    y = readMenu(x);

    if (debug == 1)
    printf("\n%d", y);

    return y;
}

int readMenu(char x[])
{
    if (debug==1)
        printf("\readMenu\n");
    int check = 0;
    int y = 0;

    SpecialClear(x);//clear case formatting

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
    else if (compareStrings(x, " ") == 1)
    {
        y = 2;
    }
    else if (compareStrings(x, "for") == 1)
    {
        y = 3;
    }
    else
        y=0;

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

        else if (stringstorage[i] == ' ')
            ;

        else if (stringstorage[i]=='\n')
            ;
        else
            ;
        i++;
    }
    return;
}

void GetInput(int x, char arrays[])
{
    if (x==1)
    {
        int i = 0;
        while ((arrays[i]=getchar()) != '\n')
        {
            i++;
        }
            if (arrays[i]=='\n')
                arrays[i]='\0';
    }
    else if (x==0)
    {
        int i = 0;
        while ((arrays[i]=getchar()) != '\n')
        {
            if  (!((i==0) && (arrays[i]==' ')))
            i++;
        }
            if (arrays[i]=='\n')
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
    i = 0;
    int ii = 0;
    int width;
    for (width=0;stringstorage[width]!='\0';width++)
        ;
    while (stringstorage[i]!='\0'&&(ii<=width))
    {
        if (stringstorage[i]==' ')
        {
            do
            {
            ii++;
            if (debug == 1)
                printf("\nSP CLEAR %d\n",i);
            stringstorage[i]=stringstorage[i+ii];
            }while (stringstorage[i]==' ');
            i++;
        }
        else
        {
            i++;
        }
    }
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
    i = 0;
    int ii = 0;
    int width;
    for (width=0;stringstorage[width]!='\0';width++)
        ;
    while (stringstorage[i]!='\0'&&(ii<=width))
    {
        if (stringstorage[i]==' ')
        {
            do
            {
            ii++;
            if (debug == 1)
                printf("\nSP CLEAR %d\n",i);
            stringstorage[i]=stringstorage[i+ii];
            }while (stringstorage[i]==' ');
            i++;
        }
        else
        {
            i++;
        }
    }
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
                sptype("\nI didn't quite get that, could you confirm again? y/n");
                GetInput(0, confirm);
            }
        }while (scheck == 0);
    }while (check == 0);         //ends name loops

    //clear screen and bigin the introduction to the game.
    system("cls");
    char temp[]={'\0'};
    sptype("welcome "); sptype(name); sptype(", to Lotaro.  Lotaro is a land of many wonders, where forces of light and darkness clash daily.  ");
    sptype("Many heroes seek to make a name for themselves here... but that is not where your story begins "); sptype(name); sptype(".  You seek ");
    sptype("only a simple life in your small home town of (NAME OF TOWN). Listen, your mother is calling you.  You should go to her now.\n\nare you ready? y/n");
    GetInput(0, temp);
    if(!compareStrings(temp, "y") || !compareStrings(temp, "yes")|| !compareStrings(temp, "yesiamready")|| !compareStrings(temp, "iamready")|| !compareStrings(temp, "imready"))
        sptype("too bad\n");

    sptype("Woman's Voice:"); sptype(name);sptype(", "); sptype(name);sptype("!  Where are You?\n");
    sptype("(finish)");
    return;
}

//delays with a null statement loopint
void delay(float x)
{
    int i;
    for(i=0;i<(x*3000000);i++)
        ;
    return;
}

//the sptype function prints out one character at a time and takes ten times longer o print '.'s
void sptype(char x[])
{
    int i = 0;
    while (x[i]!='\0')
    {   //prints characters one at a time
        printf("%c", x[i]);
        delay(10);
        //takes extra time to print '.'
        if (x[i+1]=='.')
            delay(100);
        i++;
    }
}

void chat(char x[])
{

    int width;
    for (width=0;x[width]!='\0';width++);
    if (width<120)
        sptype(x);
    else
        ;
  return;
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

void walk(int MapID,int* x_Pos,int* y_Pos, MAPSTRINGS)
{
    /*//checkL(MapID, x_Pos, y_Pos, MAPSTRINGS);


    if (MapID == MAINMAP)
        if ((x_Pos==0) && (y_Pos==0))
            printf("\nd-down, r-right, s-save, sq-save and quit:");
        if ((x_Pos==0) && (y_Pos!=0) && (y_Pos<MMY))
            printf("\nd-down, u-up, r-right, s-save, sq-save and quit:");
        if ((x_Pos!=0) && (y_Pos==0) && (x_Pos<(MMX-1)))
            printf("\nd-down, l-left, r-right, s-save, sq-save and quit:");
        if ((x_Pos!=0) && (y_Pos!=0) && (x_Pos<(MMX-1)) && (y_Pos<MMY))
            printf("\nd-down, u-up, l-left, r-right, s-save, sq-save and quit:");*/
    return;
}
