//COMPILE WITH: gcc main.c -o flora.exe -Wall -Werror -Wpedantic
//RUN WITH: ./flora.exe

#include <stdio.h>
#include <string.h>

typedef struct {
    int itemCounts[5];
    int itemMaxCounts[5];
    //int buildings[20];
} Container;

typedef struct {
    int hunger;
    int temperture;
    //int stamina;
    //int health;
    //int hygiene

} Human;

int itemArrayLength = 5;
char stringId[5][10] = {
    "axe",
    "stick",
    "rock",
    "leaf",
    "log",
};
char stringIdPlural[5][10] = {
    "axes",
    "sticks",
    "rocks",
    "leaves",
    "logs"
};

int commandArrayLength = 3;
char commandKeywords[3][10] = {
    "look",
    "list",
    "take"
};

Human Player = {
    .hunger = 0,
    .temperture = 75
};

Container Forest = {
    .itemCounts = {2, 4, 6, 100, 3},
    .itemMaxCounts = {100, 100, 100, 100, 100}
};

Container Inventory = {
    .itemCounts = {0, 0, 0, 0, 0},
    .itemMaxCounts = {1, 10, 5, 50, 2}
};

void listContents(Container space) {
    //this could be x/y logs in the future, so show how much space you have left.
    for (int i = 0; i < itemArrayLength; i++) {
        if (space.itemCounts[i] == 1) {
            printf("- 1 %s\n", stringId[i]);
        } else if (space.itemCounts[i] > 1) {
            printf("- %d %s\n", space.itemCounts[i], stringIdPlural[i]);
        }
    }
}

void take(int id, int quantity, Container *taker, Container *space) {

    if (id == -1) {
        printf("You aren't able to take that.\n");
        return;
    }

    int maxCount = (*taker).itemMaxCounts[id] - (*taker).itemCounts[id];
    int itemCount = (*space).itemCounts[id];

    if(itemCount < quantity) {
        quantity = itemCount;

        if (quantity > 1) {
            printf("There are only %d %s here.\n", itemCount, stringIdPlural[id]);
        } else if (quantity == 1) {
            printf("There is only %d %s here.\n", itemCount, stringId[id]);
        } else if (quantity == 0) {
            printf("There are no %s here.\n", stringIdPlural[id]);
        }
        
    }

    if (maxCount < quantity) {
        quantity = maxCount;
        if (quantity > 1) {
            printf("Taken only %d %s, since you can only carry %d %s total.\n", maxCount, stringIdPlural[id], (*taker).itemMaxCounts[id], stringIdPlural[id]);
        } else if (quantity == 1) {
            printf("Taken only %d %s, since you can only carry %d %s total.\n", maxCount, stringId[id], (*taker).itemMaxCounts[id], stringIdPlural[id]);
        } else if (quantity == 0) {
            printf("You cannot carry any more %s.\n", stringIdPlural[id]);
        }
    } else if (quantity > 0) {
        printf("Taken.\n");
    }

    (*space).itemCounts[id] -= quantity;
    (*taker).itemCounts[id] += quantity;
}

void drop(int id, int quantity, Container *dropper, Container *space) {

    int maxCount = (*space).itemMaxCounts[id] - (*space).itemCounts[id];
    int itemCount = (*dropper).itemCounts[id];

    if(itemCount < quantity) {
        quantity = itemCount;

        if (quantity > 1) {
            printf("You only have %d %s to drop.\n", itemCount, stringIdPlural[id]);
        } else if (quantity == 1) {
            printf("You only have %d %s to drop.\n", itemCount, stringId[id]);
        } else if (quantity == 0) {
            printf("You don't have any %s to drop.\n", stringIdPlural[id]);
        }
        
    }

    if (maxCount < quantity) {
        quantity = maxCount;
        if (quantity > 1) {
            printf("Dropped only %d %s, since there is only room for %d %s total.\n", maxCount, stringIdPlural[id], (*space).itemMaxCounts[id], stringIdPlural[id]);
        } else if (quantity == 1) {
            printf("Dropped only %d %s, since there is only room for %d %s total.\n", maxCount, stringId[id], (*space).itemMaxCounts[id], stringIdPlural[id]);
        } else if (quantity == 0) {
            printf("There is no room for any more %s.\n", stringIdPlural[id]);
        }
    } else if (quantity > 0) {
        printf("Dropped.\n");
    }

    (*dropper).itemCounts[id] -= quantity;
    (*space).itemCounts[id] += quantity;
}

void printByte(char toTranslate) {
    int i;
    for (i = 0; i < 8; i++) {
        printf("%d", !!((toTranslate << i) & 0x80));
    }
    printf("\n");

    //return 0;
}

void splitInputIntoFields(char input[30], char fields[][30]) {

    //Question: How can this change the values of "fields" if "fields" is not being passed by reference?
    //Could this have something to do with arrays being technically like addresses?
    //also, why do I not have to define the rows of the array, but do have to define the columns?

    //magic numbers. This should be fine because the current implementation shouldn't have more than 3 fields.
    //But, if that changes, this will need to change.
    int currentField = 0; //0-2
    int currentCharIndex = 0; //1-30

    for (int i = 0; i < 30; i++) {

        if (input[i] == 10) { //LF. 00001010 kept getting added to the end of strings and messing up comparisons
            i = 30;

        } 
        else if (input[i] == ' ') {
            fields[currentField][currentCharIndex] = '\0';
            currentCharIndex = 0;
            currentField++;

        } else {
            fields[currentField][currentCharIndex] = input[i];
            //printByte(fields[currentField][currentCharIndex]);
            currentCharIndex++;
        }
    }
}

int getItemIdFromString(char string[30]) {
    int item = -1;
    for (int i = 0; i < itemArrayLength; i++) {
        if (strcmp(string,stringId[i]) == 0 || strcmp(string,stringIdPlural[i]) == 0) {
            item = i;
        }
    }

    return item;
}

void parser (char input[30]) {

    char fields[3][30] = {"", "", ""};
    splitInputIntoFields(input, fields);

    if (strcmp(fields[0], "list") == 0) {
        listContents(Inventory);
    }
    else if (strcmp(fields[0], "look") == 0) {
        listContents(Forest);
    }
    else if (strcmp(fields[0], "take") == 0) {

        int item = getItemIdFromString(fields[1]);
        take(item, 1, &Inventory, &Forest);

    }
    else if (strcmp(fields[0], "drop") == 0) {

        int item = getItemIdFromString(fields[1]);
        drop(item, 1, &Inventory, &Forest);

    }

}

void printStatus (Human person) {
    if (person.hunger > 90) {
        printf("\nYou feel dizzy and weak. Your stomach feels like it is trying to claw it's way through your chest.");
    }
    else if (person.hunger > 60) {
        printf("\nYour mind feels foggy. Your stomach aches with hunger.");
    }
    else if (person.hunger > 30) {
        printf("\nYou hear your stomach rumble.");
    }

    if (person.temperture < 60) {
        printf("\nYour teeth chatter.");
    }
    else if (person.temperture < 30) {
        printf("\nYour whole body shivers.");
    }
    else if (person.temperture < 10) {
        printf("\nYou feel numb.");
    }
}

int main() {

    char input[30];
    
    printf("Welcome To The Woods!\n>");
    while(1) {
        for (int i = 0; i < 30; i++) input[i] = 'Q';
        fgets(input, 30, stdin);
        printf("\n");
        parser(input);
        printStatus(Player);
        printf("\n\n>");
    }

    return 0;
}