#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NUMMATCHES (9 * 8) + 1
#define NUMCARDS 18 + 1

#define RED 31 //US
#define WHITE 15 //SI?
#define BLUE 34 //"
#define YELLOW 33 //!
#define MAGENTA 35 //#
#define DOLLAR '$' // 36
#define HASH '#' // 35
#define AND '&' //38

#define DEBUG 0


static const int COLORS[] = {WHITE, YELLOW, BLUE, MAGENTA, RED};
static const char CARDTYPES[] = {DOLLAR, HASH, AND};

struct player {
    char name[6];
    char cards[NUMCARDS];
    char matches[NUMMATCHES]; //maxnum players times number of cards per players * 2 for a 2char card / 2 since matches are two cards
    int win;
};

int randrange(int min, int max)
{
    return (int) (rand() % (max - min + 1)) + min;
}

void clrscrn()
{
    if(!DEBUG)
        system("clear");
}

int strl(char *s)
{
    int i = 0;
    while (s[i] != '\0')
        i++
    ;
    return i;
}

void sprint(char *s, int c)
{
    int i = strl(s);
    int j;

    for (j = 0; j < i; j++) {
        printf("\033[1;%dm%c\033[0m", c, s[j]);
        fflush(stdout);
        usleep(100000);
    }
}

void ssprint(char *s, int sleep)
{
    int i = strl(s);
    int j;

    for (j = 0; j < i; j++) {
        printf("%c", s[j]);
        fflush(stdout);
        usleep(sleep);
    }
}


void cprint(char *s, int c)
{
    printf("\033[1;%dm%s\033[0m", c, s);
}

void ccprint(char ch, int c)
{
    printf("\033[1;%dm%c\033[0m", c, ch);
}

void printcards(char *c, int color)
{
    int i, l, n, a, x, z, col;
    l = strl(c);
    char s[] = " _____  |     | | / \\ | | \\ / | |_____| ";

    z = 0, i = 0;
    for (a = 0; a < 5; a++) {
        for (i = 0; i < l - 1; i+=2) 
            for (x = 0; x < 8; x++) {
                if (color)
                    col = c[i];
                else
                    col = WHITE;
                if (x + z == 9 && !color)
                    ccprint('?', col);
                else if (x + z == 9)
                    ccprint(c[i + 1], col);
                else
                    ccprint(s[x + z], col);
            }
                
        printf("\n");
        z += 8;
    }
}

void menu()
{
    printf(" ::::::::       :::             :::::::::  \n");
    usleep(100000);
    printf(":+:    :+:      :+:             :+:    :+: \n");
    usleep(100000);
    printf("+:+    +:+      +:+             +:+    +:+ \n");
    usleep(100000);
    printf("+#+    +:+      +#+             +#++:++#:  \n");
    usleep(100000);
    printf("+#+    +#+      +#+             +#+    +#+ \n");
    usleep(100000);
    printf("#+#    #+#      #+#             #+#    #+# \n");
    usleep(100000);
    printf(" ########       ##########      ###    ### \n");
    usleep(100000);
}

char getch()
{
    char c;
    system("/bin/stty raw");
    c = getchar();
    system("/bin/stty cooked");
    return c;
}

int getin()
{
    int i;
    system("/bin/stty raw");
    i = getchar();
    i -= '0';
    i = i < 0xA && i > 0x0 ? i : 0;
    system("/bin/stty cooked");
    return i;
}

void fsleep(int i)
{
    fflush(stdout);
    sleep(i);
}

void gameinfo()
{
    printf("\nWelcome to OLC, a remake of OLR but in C.");
    fsleep(3);
    clrscrn();
    printf("OLR is a game similar to the game old maid. The GOAL of this game is to be the first player to have 0 cards. Each player is given a random assortment of cards at the begining of each round. There are two ways to remove cards from your deck: another player will pick one and match two cards of the same color and type. Cards will be added to your cardpile when you choose a random card from the next player. DO NOT be the last person with the red card at the end of the game otherwise you lose.\n\n\nTL;DR\n1. Don't be that last one with the red card.\n2. Try and match all your cards.\n3. Each player draws a random card from the next player's deck.\n\nType to continue: ");
}

void introduction()
{
    clrscrn();
    menu();
    fsleep(1);
    clrscrn();
    gameinfo();
    getch();
    clrscrn();
}

int isdigit(int c)
{
    return c > 0x30 && c < 0x3A ? 1 : 0;
}

void setplayers(struct player (*players)[], int numplayers)
{
    // set players name
    int i, j, x, y, cc, ct;

    char cardpile[numplayers * 16 + 1];
    for (i = 0; i < numplayers * 16; i+=4) {
        cc = randrange(0, 3); //4 is length of color array - 1 for indexing
        ct = randrange(0, 2); //2 type array
        cardpile[i] = COLORS[cc];
        cardpile[i + 1] = CARDTYPES[ct];
        cardpile[i + 2] = COLORS[cc];
        cardpile[i + 3] = CARDTYPES[ct];
    }
    cardpile[numplayers * 16] = '\0'; // Indexing starts at 0

    // Remove last two cards in the card pile and replace it with a red card
    j = strl(cardpile);
    cardpile[j - 1] = '\0';
    cardpile[j - 2] = '\0';
    cardpile[j - 4] = RED;
    cardpile[j - 3] = 'R';

    //Randomize Card List
    i = strl(cardpile) - 1;
    char cardx[2] = "  ";
    char cardy[2] = "  ";
    
    printf("\n");

    for (j = 0; j < 500; j++) {
        cc = 0;
        ct = 0;
        x = randrange(0, i);
        y = randrange(0, i);

        if (x % 2 == 0) {
            cardx[0] = cardpile[x];
            cardx[1] = cardpile[x + 1];
        } else {
            cardx[0] = cardpile[x - 1];
            cardx[1] = cardpile[x];
            cc = 1;
        }
        
        if (y % 2 == 0) {
            cardy[0] = cardpile[y];
            cardy[1] = cardpile[y + 1];
        } else {
            cardy[0] = cardpile[y - 1];
            cardy[1] = cardpile[y];
            ct = 1;
        }

        if (ct) {
            cardpile[y - 1] = cardx[0];
            cardpile[y] = cardx[1];
        } else {
            cardpile[y] = cardx[0];
            cardpile[y + 1] = cardx[1];
        }

        if (cc) {
            cardpile[x - 1] = cardy[0];
            cardpile[x] = cardy[1];
        }
        else {
            cardpile[x] = cardy[0];
            cardpile[x + 1] = cardy[1];
        }
    }


    //Set Player Names
    char p[3] = "P1";
    for (j = 0; j < 3; j++) 
        (*players)[0].name[j] = p[j];


    //Set CPU Names
    char s[6] = "CPU  \0";
    for (i = 1; i < numplayers; i++) {
        s[4] = (i) + '0';

        for (j = 0; j < 6; j++) 
            (*players)[i].name[j] = s[j];
        
    }

    //Setup Player Cards
    for (i = 0; i < numplayers; i++) 
        for (j = 0; j < NUMCARDS; j++) 
            (*players)[i].cards[j] = '\0';
        
    // set players cards
    x = 0;
    for (i = 0; i < numplayers; i++) {
        for (j = 0; j < NUMCARDS - 3; j+=2, x+=2) {
            //Choose player for card
            (*players)[i].cards[j] = cardpile[x];
            (*players)[i].cards[j + 1] = cardpile[x + 1];
        }
    }
    
    // set players matches
    for (i = 0; i < numplayers; i++) {
        for (j = 0; j < NUMMATCHES - 1; j++) {
            (((*players)[i]).matches)[j] = '\0';
        }
        (*players)[i].matches[NUMMATCHES - 1] = '\0';
    }

    //set players win condition to 0;
     for (i = 0; i < numplayers; i++) 
        (*players)[i].win = 0;
}

int getplayers()
{
    int p;
    do {
        clrscrn();
        printf("How Many CPUs? (1-9 Players):");
    } while (!(p = getin()));
    clrscrn();

    return p;
}

void prnt_game_logo()
{
    printf("\033[1;31m"
	"_________________________________\n" 
	"      __         _         ____  \n"
	"    /    )       /         /    )\n"
	"---/----/-------/---------/___ /-\n"
	"  /    /       /         /    |  \n"
	"_(____/_______/____/____/_____|__\n"
	"\033[0m\n");
}

void prnt_game_menu(char *cn, int b)
{
    if (b == 0)
        printf(
        "Type the number of your desired action bellow:\n"
        "1. Match Cards\n" 
        "2. Make A Funny Face\n" 
        "3. Try and peak at another player's cards\n"
        "4. Take a card from %s\n"
        "\n\n YOUR CARDS:\n", cn);
    else
        printf(
            "Type the number of your desired action bellow:\n"
            "1. Match Cards\n" 
            "2. Make A Funny Face\n" 
            "3. Try and peak at another player's cards\n"
            "4. End Turn\n"
            "\n\n YOUR CARDS:\n");
        
}

void funny_face(int numplayers)
{
    int i, x, p;

    numplayers--; 

    clrscrn();
    i = randrange(0, 4);
    p = randrange(1, numplayers);

    char s[6] = "CPU  ";
    s[5] =  p + '0';
    sprint("You made a funny face....\n", WHITE);
    sprint(s, WHITE);

    switch (i) {
        case 0:
            sprint(" blew a fuse and died. . . .", RED);
            break;
        case 1:
            sprint(" laughed", WHITE);
            break;
        case 2:
            sprint(" made a funny face too", WHITE);
            break;
        case 3:
            sprint(" needs to go to the bathroom", YELLOW);
            break;
        case 4:
            sprint(" wants your hand in marriage", RED);
            break;
    }
}

void peak_cards(struct player (*players)[], int numplayers)
{
    int p, i, t;

    numplayers--;

    t = 0;
    p = randrange(1, numplayers);
    i = randrange(0, 1);

    clrscrn();

    char s[15] = "CPU  '\'s cards";
    s[5] =  p + '0';

    if (i) {
        sprint("You Successfully peaked ", WHITE);
        t = 1;
    } else
        sprint("You failed to peak at ", RED);
    
    sprint(s, WHITE);
    printf("\n");
    if (t)
        printcards((*players)[p].cards, 1);

    usleep(1600000);
}

void match_cards(struct player *player)
{
    int i, j, m, c, x, y, t, z;

    c = strl(player->cards);
    m = strl(player->matches);
    
    while(z) {
        z = 0;
        for (i = 0; i < c; i+=2) {
            if (z)
                break;

            for (j = 0; j < c; j+=2) {
                if (j != i && player->cards[i] != '\0' && player->cards[i] == player->cards[j] && player->cards[i + 1] == player->cards[j + 1]) {
                    player->matches[m] = player->cards[i];
                    player->matches[m + 1] = player->cards[i + 1];

                    //Move Player Cards array left 2 pos
                    t = 0;
                    for (x = 1 + i; t < c - 1 - i; x++, t++) {
                    player->cards[x - 1] = player->cards[x + 1];
                    }

                    //Move Player Cards array left 2 pos again accounting for previous 2 pos offset
                    t = 0;
                    for (x = 1 + j - 2; t < c - 1 - j; x++, t++) {
                        player->cards[x - 1] = player->cards[x + 1];
                    }

                    for (y = 1; y < 5; y++)
                        player->cards[c - y] = '\0';

                    c = strl(player->cards);
                    m = strl(player->matches);
                    z = 1;
                    break;
                }
            }
        }
    }
}

int check_win(char *c)
{
    if (strl(c))
        return 0;
    return 1;
}

struct player* get_next_player(struct player (*players)[], int current_player, int numplayers)
{
    int i, p;

    p = -1;

    //Pick the next closest player two loops for cyclical card picking. 
    for (i = current_player + 1; i < numplayers; i++)
        if ((!(*players)[i].win)) {
            p = i;
            break;
        }

    if (p < 0)
        for (i = 0; i < current_player; i++)
             if ((!(*players)[i].win)) {
                p = i;
                break;
            }

    if (p == -1)
        return NULL;
    
    return &(*players)[p];
}

void ai_pick_cards(struct player *p1, struct player *p2, int *n)
{
    int a, c, x, i, s, t;

    match_cards(p1);
    if (check_win((p1->cards))) {
        ((*n)++);
        p1->win = *n;
        return;
    }

    c = strl(p2->cards);
    s = strl(p1->cards);
    a = randrange(0, (c / 2) - 1);

    //set player card to be the new card
    p1->cards[s + 1] = p2->cards[(a) * 2 + 1];
    p1->cards[s] = p2->cards[(a - 1) * 2 + 2];

    //Remove card from other players inventory
    t = 0;
    a *= 2;
    for (x = 1 + a; t < c - 1 - a; x++, t++) {
        p2->cards[x - 1] = p2->cards[x + 1];
    }

    if (check_win((p2->cards))) {
        ((*n)++);
        p2->win = *n;
        return;
    }

    match_cards(p1);
    if (check_win((p1->cards))) {
        ((*n)++);
        p1->win = *n;
        return;
    }

    printf("%s picking a card from %s", p1->name, p2->name);
    ssprint("\tDONE\n", 80000);
}

int pick_cards(struct player (*players)[], int numplayers, int *n)
{
    int i, c, m;

    clrscrn();

    struct player *p;
    for (i = 1; i < numplayers; i++) {
        if (!((*players)[i].win)) {
            p = get_next_player(&(*players), i, numplayers);
            if (p)
                ai_pick_cards(&(*players)[i], p, n);
            else 
                return 0;
        }
    }

    return 1;
}

void player_pick_card(struct player *player, struct player *ai, int *n)
{
    int a, c, i, s, x, t;

    c = strl(ai->cards);
    s = strl(player->cards);

    do {
        do {
            clrscrn();
            printf("Choose one of %s\'s cards:\n", ai->name);
            printcards(ai->cards, 0);
            printf("\n");

            for (i = 1; i < c / 2 + 1; i++) 
                printf("   %d    ", i);
            printf("\n\n\tChoice #: ");

        } while (!(a = getin()));
    } while (a > c / 2);
    a--; //indexing

    //set player card to be the new card
    player->cards[s + 1] = ai->cards[(a) * 2 + 1];
    player->cards[s] = ai->cards[(a - 1) * 2 + 2];

    //Remove card from other players inventory
    t = 0;
    a *= 2;
    for (x = 1 + a; t < c - 1 - a; x++, t++) {
        ai->cards[x - 1] = ai->cards[x + 1];
    }

    if (check_win((ai->cards))) {
        ((*n)++);
        ai->win = *n;
        return;
    }
}

int player_action(struct player (*players)[], int numplayers, int *b, int *n) 
{   
    int a, p;
    p = 0;

    if (check_win(((*players)[0]).cards)) {
        ((*n)++);
        (*players)[0].win = *n;
        return 0;    
    }
    
    do {
        clrscrn();
        prnt_game_logo();
        prnt_game_menu(((*players)[1]).name, *b);
        printcards(((*players)[0]).cards, 1);
        printf("\nYOUR MATCHES:\n");
        printcards(((*players)[0]).matches, 1);
        printf("\nDesired Action? :  ");
    } while (!(a = getin()));
    
    switch (a) {
        case 1 :
            match_cards(&(*players)[0]);
            break;
        case 2 :
            funny_face(numplayers);
            break;
        case 3 :
            peak_cards(&(*players), numplayers);
            break;
        case 4 :
            if (*b != 1) {
                *b = 1;
                player_pick_card(&(*players)[0], get_next_player(&(*players), 0, numplayers), &n);
            }
            else {
                *b = 0;
                p = 1;
            }
            break;
    }
    
    if (check_win(((*players)[0]).cards))
        ((*n)++);
        ((*players)[0]).win = *n; 
    return p;
}

void game_over(struct player (*players)[], int numplayers)
{
    int i;
    
    clrscrn();
    printf("Game Stats:\n");
     for (i = 0; i < numplayers; i++) 
        printf("\n%s:\n\tNumber Of Matches: %d\n\tPlacement on Leaderboard: %d\n", (*players)[i].name, strl((*players)[i].matches) / 2, (*players)[i].win + 1);
}

void gameloop(struct player (*players)[], int numplayers)
{
    int b, a, n, i;
    b = 0; n = 0;
    while (1) {
        if (n == numplayers - 1)
            return;
        
        if (!(((*players)[0]).win)) {
            a = player_action(players, numplayers, &b, &n);
        } else {
            b = 0; a = 1;
        }
        if ((!(b)) && (a)) {
            if (!(pick_cards(players, numplayers, &n))) 
                return;
        }
            
        usleep(160000);
        clrscrn();
    }
}

int game()
{
    time_t t;
    srand((unsigned) time(&t));

    int numplayers;
    numplayers = getplayers() + 1; //+ 1 for player

    
    struct player players[numplayers];
    setplayers(&players, numplayers);

    gameloop(&players, numplayers);
    game_over(&players, numplayers);

    return 0;
}

int main(void) 
{
    //introduction();
    while(game())
    ;

    return 0;
}

//Utility Functions for debugging

//for (int i = 0; i < NUMMATCHES - 1; i++)
//{
//    printf("%d ", ((*players)[0]).matches[i]);
//}

//printf("\n CARDS:\n");

//for (int i = 0; i < NUMCARDS - 1; i++)
//{
//    printf("%d ", ((*players)[0]).cards[i]);
//}