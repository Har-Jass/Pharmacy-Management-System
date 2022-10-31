#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>

typedef struct
{
    int day, month, year;
} date;

typedef struct
{
    int id;
    char name[50];
    date mfg, expiry;
    int price;
    int stock;
} drug;

typedef struct
{
    char mobile[11];
    char password[11];
} pharmacist;

int dateValidation(FILE *fp, char fname[], int yy, int mm, int dd);

void add_drug(char fname[]);        // pharmacist function
void delete_drug(char fname[]);     // pharmacist function
void update_drug(char fname[]);     // pharmacist function
void print_stock(char fname[]);     // pharmacist function
void account_pharmacist(void);      // pharmacist function
int login_pharmacist(char fname[]); // pharmacist function
int rand_id(void);                  // pharmacist function

int main()
{

    char c;
    int c1;
    char login[15];
    int success;
    int logout = 0;
    int out;
    system("cls");
    printf("\n                          WELCOME TO PHARMACY MANAGEMENT SYSTEM                          ");

    printf("\n\nDo you have an account?(y/n): ");

    c1 = getchar();

    getchar();

    if (c1 == 'n')
    {
        system("cls");
        account_pharmacist(); 
    }

    do
    {
        system("cls");
        success = login_pharmacist(login);

        if (success == 2)
        {
            system("cls");
            printf("Account does not exist with this phone number! Please create an account");
            Sleep(2000);
            system("cls");
            account_pharmacist();
        }

    } while (success != 1);

    // print_stock(login); // calling print_stock() method to print the current available stock data

    while (logout == 0)
    {

    notlogout:
        printf("\n\nWhat would you like to do?"
               "\n1.Add drug"
               "\n2.Delete drug"
               "\n3.Update drug"
               "\n4.View inventory"
               "\n5.Logout\n");

        printf("\nEnter your choice: ");
        scanf("%d", &c1);

        switch (c1)
        {

        case 1:
            system("cls");
            add_drug(login); // calling add_drug() method to add new drug in stock
            printf("\nThankyou...!");
            break;

        case 2:
            system("cls");
            printf("\nCurrent Available Stock Are: \n");
            print_stock(login); 
            printf("\nWhat you want to delete: \n");
            delete_drug(login);
            break;

        case 3:
            system("cls");
            printf("\nCurrent Available Stock Are: \n");
            print_stock(login);
            update_drug(login);
            break;

        case 4:
            system("cls");
            printf("\nCurrent Available Stock Are: \n");
            print_stock(login);
            break;

        case 5:
            system("cls");
            printf("\nDo you really want to logout?");
            printf("\n\nEnter your choice(1 for Yes/2 for No): ");
            scanf("%d", &out);
            switch (out)
            {
            case 1:
                system("cls");
                printf("\n\n                       THANK YOU FOR VISITING OUR WEBSITE!!                       \n\n");
                Sleep(3000);
                system("cls");
                exit(0);
                break;
            case 2:
                printf("\n\n                       WELCOME BACK TO OUR WEBSITE!!                       \n\n");
                goto notlogout;
                break;
            default:
                printf("\n\nInvalid choice. Please try again..");
            }

            break;

        default:

            printf("\n\nInvalid choice!!! Please try again.....");
            break;
        }
    }

    printf("\n\n                       THANK YOU FOR VISITING OUR WEBSITE!!                       \n\n");

    exit(0);
}

// add drug to inventory
void add_drug(char fname[])
{
    int dateReturn = 0;
    int flag = 1;

    FILE *fp;

    fp = fopen(fname, "a");

    drug new_drug;

    new_drug.id = rand_id();

    printf("\nDrug name: ");
    scanf("%s", new_drug.name);

enterManuDateAgain:
    printf("\nManufacturing date(DD MM YYYY): ");
    scanf("%d %d %d", &new_drug.mfg.day, &new_drug.mfg.month, &new_drug.mfg.year);
    dateReturn = dateValidation(fp, fname, new_drug.mfg.day, new_drug.mfg.month, new_drug.mfg.year);
    if (dateReturn == 0)
    {
        printf("\nNot a valid manufacturing date. Please enter valid date.\n");
        goto enterManuDateAgain;
    }

enterExpDateAgain:
    printf("\nExpiry date(DD MM YYYY): ");
    scanf("%d %d %d", &new_drug.expiry.day, &new_drug.expiry.month, &new_drug.expiry.year);
    dateReturn = dateValidation(fp, fname, new_drug.expiry.day, new_drug.expiry.month, new_drug.expiry.year);
    if (dateReturn == 0)
    {
        printf("\nNot a valid expiry date. Please enter valid date.\n");
        goto enterExpDateAgain;
    }

    printf("\nPrice: ");
    scanf("%d", &new_drug.price);

    getchar();

    printf("\nNumber of units to be added to inventory: ");
    fflush(stdin);
    scanf("%d", &new_drug.stock);

    fprintf(fp, " %d %s %d %d %d %d %d %d %d %d", new_drug.id, new_drug.name, new_drug.mfg.day, new_drug.mfg.month, new_drug.mfg.year, new_drug.expiry.day, new_drug.expiry.month, new_drug.expiry.year, new_drug.price, new_drug.stock);

    printf("\nDrug added to inventory!");

    fclose(fp);

    return;
}

// delete drug
void delete_drug(char fname[])
{

    FILE *fp, *fp_tmp;

    drug new_drug;

    int found = 0;

    fp = fopen(fname, "r");

    pharmacist person;

    fscanf(fp, "%s %s", person.mobile, person.password);

    if (feof(fp))
    {

        fclose(fp);

        printf("\nInventory empty!!!");

        return;
    }

    fp_tmp = fopen("tmp.txt", "w");

    if (!fp_tmp)
    {

        fclose(fp);
        fclose(fp_tmp);

        printf("\nUnable to open temp file.");
        return;
    }

    fprintf(fp_tmp, "%s %s", person.mobile, person.password);

    int drug_id;
    printf("\nDrug ID to be deleted: ");
    scanf("%d", &drug_id);

    while (!feof(fp))
    {
        fscanf(fp, "%d %s %d %d %d %d %d %d %d %d", &new_drug.id, new_drug.name, &new_drug.mfg.day, &new_drug.mfg.month, &new_drug.mfg.year, &new_drug.expiry.day, &new_drug.expiry.month, &new_drug.expiry.year, &new_drug.price, &new_drug.stock);

        if (drug_id == new_drug.id)
        {
            printf("\nDrug found and deleted from inventory.");
            found = 1;
        }
        
        else
        {
            fprintf(fp_tmp, " %d %s %d %d %d %d %d %d %d %d", new_drug.id, new_drug.name, new_drug.mfg.day, new_drug.mfg.month, new_drug.mfg.year, new_drug.expiry.day, new_drug.expiry.month, new_drug.expiry.year, new_drug.price, new_drug.stock);
        }
    }

    if (!found)
    {
        printf("\n%d not found in inventory.", drug_id);
    }

    fclose(fp);
    fclose(fp_tmp);

    remove(fname);

    rename("tmp.txt", fname);

    return;
}

void update_drug(char fname[])
{

    FILE *fp, *fp_tmp;

    drug new_drug;

    int choice;

    int found = 0;

    pharmacist person;

    fp = fopen(fname, "r"); 

    fscanf(fp, "%s %s", person.mobile, person.password);

    if (feof(fp))
    {

        fclose(fp);

        printf("\nInventory empty!!!");

        return;
    }

    fp_tmp = fopen("tmp.txt", "w");
    if (!fp_tmp)
    {
        printf("\nUnable to open temp file.");
        return;
    }

    fprintf(fp_tmp, "%s %s", person.mobile, person.password);

    int drug_id;

    printf("\nDrug id to be updated: "); // get the drug id which we have to update
    scanf("%d", &drug_id);

    while (!feof(fp))
    {
        fscanf(fp, "%d %s %d %d %d %d %d %d %d %d", &new_drug.id, new_drug.name, &new_drug.mfg.day, &new_drug.mfg.month, &new_drug.mfg.year, &new_drug.expiry.day, &new_drug.expiry.month, &new_drug.expiry.year, &new_drug.price, &new_drug.stock);
        
        if (drug_id == new_drug.id)
        {

            found = 1;

            printf("\nWhich information would you like to update?"
                   "\n1.Name"
                   "\n2.Manufacturing date"
                   "\n3.Expiry date"
                   "\n4.Price"
                   "\n5.Stock left"
                   "\n\nEnter your choice: ");

            scanf("%d", &choice);

            switch (choice)
            {

            case 1:
                system("cls");
                printf("\nEnter new name: ");
                scanf("%s", new_drug.name);
                printf("\nName changed successfully. Thankyou...!!!");
                break;

            case 2:
                system("cls");
                printf("\nEnter new manufacturing date(DD MM YYYY): ");
                scanf("%d %d %d", &new_drug.mfg.day, &new_drug.mfg.month, &new_drug.mfg.year);
                printf("\nDate changed successfully. Thankyou...!!!");
                break;

            case 3:
                system("cls");
                printf("\nEnter new expiry date(DD MM YYYY): ");
                scanf("%d %d %d", &new_drug.expiry.day, &new_drug.expiry.month, &new_drug.expiry.year);
                printf("\nDate changed successfully. Thankyou...!!!");
                break;

            case 4:
                system("cls");
                printf("\nEnter new price: ");
                scanf("%d", &new_drug.price);
                printf("\nPrice changed successfully. Thankyou...!!!");
                break;

            case 5:
                system("cls");
                printf("\nEnter new stock value: ");
                scanf("%d", &new_drug.stock);
                printf("\nStock changed successfully. Thankyou...!!!");
                break;
            }

            fprintf(fp_tmp, " %d %s %d %d %d %d %d %d %d %d", new_drug.id,
                    new_drug.name, new_drug.mfg.day, new_drug.mfg.month,
                    new_drug.mfg.year, new_drug.expiry.day, new_drug.expiry.month,
                    new_drug.expiry.year, new_drug.price, new_drug.stock);

            break;
        }
        
        else
        {

            fprintf(fp_tmp, " %d %s %d %d %d %d %d %d %d %d", new_drug.id,
                    new_drug.name, new_drug.mfg.day, new_drug.mfg.month,
                    new_drug.mfg.year, new_drug.expiry.day, new_drug.expiry.month,
                    new_drug.expiry.year, new_drug.price, new_drug.stock);

            break;
        }
    }

    if (!found)
    {
        printf("\n%d not found in inventory.", drug_id);
    }

    fclose(fp);
    fclose(fp_tmp);

    remove(fname);

    rename("tmp.txt", fname);

    return;
}

void account_pharmacist(void)
{

    pharmacist person;

    printf("\n\nCreate an account......");

PhnNo:
    printf("\n\nEnter a phone number# ");
    scanf("%s", person.mobile);

    if (strlen(person.mobile) != 10)
    {
        printf("Invalid phone number size provided!\n");
        goto PhnNo;
    }

    char fname[15];

    strcpy(fname, person.mobile); // string copy

    strcat(fname, ".txt"); // string concatenate

    FILE *fp;

    fp = fopen(fname, "r");

    if (fp) // if value at fp is already 1 then account is already created.
    {

        printf("\nAn account is already registered in this number!\nTry logging in...");

        fclose(fp);

        return;
    }

    fclose(fp);

    printf("\nEnter a password of your choice: ");

    scanf("%s", person.password);

    fp = fopen(fname, "w");

    fprintf(fp, "%s %s", person.mobile, person.password);

    fclose(fp);

    printf("\nCongratulations your account has been created!!! Access it with your mobile number as the username and the password you have entered."); 

    return;
}

int login_pharmacist(char fname[])
{

    pharmacist person;

PhnNo:
    printf("\nLogin......\nEnter your mobile number: ");

    scanf("%s", person.mobile);

    if (strlen(person.mobile) != 10)
    {
        printf("Invalid phone number size provided!\n");
        goto PhnNo;
    }

    strcpy(fname, person.mobile); // string copy

    strcat(fname, ".txt"); // string concatenate

    FILE *fp;

    fp = fopen(fname, "r");

    if (fp == NULL)
    {
        fclose(fp);

        return 2;
    }

    fscanf(fp, "%s %s", person.mobile, person.password);

    fclose(fp);

    char psw[11];
Pwd:
    printf("\nEnter your password: ");

    scanf("%s", psw);

    getchar();

    // The return value from strcmp is 0 if the two strings are equal
    if (strcmp(person.password, psw) == 0)
    {
        system("cls");
        printf("\nYou have successfully logged in!"); // to pharmacist login ho jyega

        return 1;
    }

    else
    {

        printf("\nYou have typed in the wrong password!"); // otherwise password is Wrong
        goto Pwd;
        return 0;
    }
}

// Prints out the existing inventory of drugs
void print_stock(char fname[])
{

    FILE *fp;

    drug new_drug;

    pharmacist person;

    fp = fopen(fname, "r");

    fscanf(fp, "%s %s", person.mobile, person.password);

    if (feof(fp))
    {

        fclose(fp);

        printf("\nInventory empty!!! No stocks to be displayed...");

        return;
    }

    while (fscanf(fp, "%d %s %d %d %d %d %d %d %d %d", &new_drug.id, new_drug.name, &new_drug.mfg.day, &new_drug.mfg.month, &new_drug.mfg.year, &new_drug.expiry.day, &new_drug.expiry.month, &new_drug.expiry.year, &new_drug.price, &new_drug.stock))
    {

        printf("\nID - %d \nName - %s \nMfg: %d/0%d/%d \nExpiry: %d/0%d/%d \nPrice: $%d \nStock - %d left\n", new_drug.id,
               new_drug.name, new_drug.mfg.day, new_drug.mfg.month, new_drug.mfg.year,
               new_drug.expiry.day, new_drug.expiry.month, new_drug.expiry.year,
               new_drug.price, new_drug.stock);

        if (feof(fp))
        {

            break;
        }
    }

    fclose(fp);

    return;
}

// generate random id for each drug
int rand_id(void)
{

    int num = 0;

    srand(time(0));

    // If random numbers are generated with rand() without first calling srand(), program will create the same sequence of numbers each time it runs.
    num = (rand() % (2000 - 1000 + 1)) + 1000;

    return num;
}

// check date is valid or not
int dateValidation(FILE *fp, char fname[], int dd, int mm, int yy)
{
    int isLeap = 0;

    fp = fopen(fname, "a");

    drug new_drug;

    // check date validations
    if (yy % 100 != 0 && yy % 4 == 0 || yy % 400 == 0)
    {
        isLeap = 1;
    }
    if (yy < 2010 || yy > 2050 || mm < 1 || mm > 12 || dd < 1 || dd > 31)
    {
        return 0;
    }
    else if (mm == 2)
    {
        if (dd == 30 || dd == 31 || (dd == 29 && !isLeap))
        {
            return 0;
        }
    }
      else if (mm == 4 || mm == 6 || mm == 9 || mm == 11)
    {
        if (dd == 31)
        {
            return 0;
        }
    }
    return 1;
      }
