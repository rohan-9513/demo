///////////////////////////////////////////////////////////////////////
//
//  Header files inclusion
//
///////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>

#include<iostream>

using namespace std;

///////////////////////////////////////////////////////////////////////
//
//  User defined Macros
//
///////////////////////////////////////////////////////////////////////

# define MAXFILESIZE 100

# define MAXINODE 5

# define READ 1
# define WRITE 2
# define EXECUTE 4

# define REGULARFILE 1
# define SPECIALFILE 2

# define START 0
# define CURRENT 1
# define END 2

///////////////////////////////////////////////////////////////////////
//
//  User defined Macros for Error handling
//
///////////////////////////////////////////////////////////////////////

# define ERR_INVALID_PARAMETER -1
# define ERR_NO_INODES -2
# define ARR_FILE_ALREADY_EXIST -3

///////////////////////////////////////////////////////////////////////
//
//  Structure Name :    BootBlock 
//  Description :       Holds information to boot the operating system
//
///////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

///////////////////////////////////////////////////////////////////////
//
//  Structure Name :    SuperBlock 
//  Description :       Holds information about the file system
//
///////////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

///////////////////////////////////////////////////////////////////////
//
//  Structure Name :    Inode 
//  Description :       Holds information about the file
// 
///////////////////////////////////////////////////////////////////////

typedef struct Inode
{
    char FileName[50];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int LinkCount;
    int Permission;
    char *Buffer;
    struct Inode *next;
}INODE, *PINODE, **PPINODE;

///////////////////////////////////////////////////////////////////////
//
//  Structure Name :    FileTable 
//  Description :       Holds information about the opened file
// 
///////////////////////////////////////////////////////////////////////

typedef struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Count;
    int Mode;
    PINODE ptrinode;
}FILETABLE, *PFILETABLE;

///////////////////////////////////////////////////////////////////////
//
//  Structure Name :    UAREA 
//  Description :       Holds information about the process
// 
///////////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[50];
    PFILETABLE UFDT[MAXINODE];
};

///////////////////////////////////////////////////////////////////////
//
//  Global variables or objects used in the project
// 
///////////////////////////////////////////////////////////////////////

BootBlock bootobj;
SuperBlock superobj;
PINODE head = NULL;
UAREA uareaobj;

///////////////////////////////////////////////////////////////////////
//
//  Function Name :     InitialiseUAREA
//  Description :       It is used to intialise the contents UAREA
//  Author :            Rohan Subhash Khanse
//  Date :              10/08/2025
//
///////////////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
    strcpy(uareaobj.ProcessName,"Myexe");

    int i = 0;

    while(i < MAXINODE)
    {
        uareaobj.UFDT[i] = NULL;
        i++;
    }
    cout<<"Marvellous CVFS : UAREA initialised succesfully\n";
}

///////////////////////////////////////////////////////////////////////
//
//  Function Name :     InitialiseSuperblock
//  Description :       It is used to intialise the contents of super block
//  Author :            Rohan Subhash Khanse
//  Date :              10/08/2025
//
///////////////////////////////////////////////////////////////////////

void InitialiseSuperblock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    cout<<"Marvellous CVFS : Superblock initialised succesfully\n";
}


///////////////////////////////////////////////////////////////////////
//
//  Function Name :     CreateDILB
//  Description :       It is used to create Linked List of Inodes
//  Author :            Rohan Subhash Khanse
//  Date :              10/08/2025
//
///////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    while(i <= MAXINODE)
    {
        newn = new INODE;

        newn->InodeNumber = i;
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->LinkCount = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if(temp == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp->next = newn;
            temp = temp->next;
        }

        i++;
    }

    cout<<"Marvellous CVFS : DILB created succesfully\n";
}

///////////////////////////////////////////////////////////////////////
//
//  Function Name :     StartAuxilaryDataInitialisation
//  Description :       It is used to intialise the Auxilary data
//  Author :            Rohan Subhash Khanse
//  Date :              10/08/2025
//
///////////////////////////////////////////////////////////////////////

void StartAuxilaryDataInitialisation()
{

    strcpy(bootobj.Information,"Boot process of Opertaing System done");

    cout<<bootobj.Information<<"\n";

    InitialiseSuperblock();

    CreateDILB();

    InitialiseUAREA();
    
    cout<<"Marvellous CVFS : Auxilary data initalised succesfully\n";
}

///////////////////////////////////////////////////////////////////////
//
//  Function Name :     StartAuxilaryDataInitialisation
//  Description :       It is used to Display the information about commands
//  Author :            Rohan Subhash Khanse
//  Date :              11/08/2025
//
///////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("----------------------------------------------------------\n");
    printf("------------Command manual of Marvellous CVFS-------------\n");
    printf("----------------------------------------------------------\n");

    printf("exit : it is used to terminate marvellous CVFS\n");
    printf("clear : it is used to clear the console of Marvellous");
    // Add more options here
}

///////////////////////////////////////////////////////////////////////
//
//  Function Name :     StartAuxilaryDataInitialisation
//  Description :       It is used to Display the manual commands
//  Input :             It accepts the command name
//  Output :            Displays the manual details of the command
//  Author :            Rohan Subhash Khanse
//  Date :              11/08/2025
//
///////////////////////////////////////////////////////////////////////

void Manpage(char *name)
{
    if(strcmp(name, "creat") == 0)
    {
        printf("Description : This command is used to create new file in our file syetem.\n");

        printf("Usage : creat File_Name Pesrmission\n");
        printf("File_name : The name of file you want to create\n");
        printf("Permissions : \n1 : Read \n2 : Write \n3 : Read Write\n");
    }
    else if(strcmp(name, " exit") == 0)
    {
        printf("Description : This command is used to terminate Marvellous CVSF\n");
    }
    // Add more options here
    else
    {
        printf("no manual entry for %s", name);
    }
}

///////////////////////////////////////////////////////////////////////
//
//  Function Name :     IsFileExist
//  Description :       It is used to check whether the file is exist or not
//  Input :             It accepts the file name
//  Output :            It returns boolean value
//  Author :            Rohan Subhash Khanse
//  Date :              11/08/2025
//
///////////////////////////////////////////////////////////////////////

bool IsFileExist(
        char *name   // name of file that you want to check
    )
    {
        PINODE temp = head;
        bool bFlag = false;

        while(temp != NULL)
        {
            if((strcmp(name, temp->FileName) == 0) && (temp->FileType) == REULARFILE)
            {
                bFlag = true;
                break;
            }
            temp = temp->next;
        }
        return bFlag;
    }

///////////////////////////////////////////////////////////////////////
//
//  Function Name :     CreateFile
//  Description :       It is used to create new regular file
//  Input :             It accepts name and permission
//  Output :            It returns the file descriptor
//  Author :            Rohan Subhash Khanse
//  Date :              11/08/2025
//
///////////////////////////////////////////////////////////////////////

int CreateFile(
    char *name,
    int permisiion      // permission to create file 
)
{
    // if file is missing
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }
    
    // If enter permission is invalid
    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    // check whether empty INODE is there or not
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    // check whether file already exist or not
    if(IsFileExist(name) == true)
    {
        return ARR_FILE_ALREADY_EXIST;
    }

    
}

///////////////////////////////////////////////////////////////////////
//
//  Entry point function of project (main)
//
///////////////////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    int iCount = 0, iRet = 0;
    char Command[5][80];

    StartAuxilaryDataInitialisation();

    cout<<"----------------------------------------------------------\n";
    cout<<"--------- Marvellous CVFS Started Succesfully ------------\n";
    cout<<"----------------------------------------------------------\n";
    
    while(1)
    {
        fflush(stdin);

        printf("\nMarvellous CVFS >");
        fgets(str, sizeof(str), stdin);

        iCount = sscanf(str, "%s, %s, %s, %s", Command[0], Command[1], Command[2], Command[3]);

        if(iCount == 1)
        {
            if(strcmp(Command[0], "exit") == 0)
            {
                printf("\nThank you for using Marvellous CVFS\n");
                printf("De-allocting all resources...\n");

                break;
            }
            else if(strcmp(Command[0], "help") == 0)
            {
                DisplayHelp();
            }
            else if(strcmp(Command[0], "cls") == 0)
            {
                system("cls");
            }
        }   // End of if iCount == 1
        else if(iCount == 2)
        {
            // Marvellous CVFS > man creat
            if(strcmp(Command[0], "man") == 0)
            {
                Manpage(Command[1]);
            }
        }  // End of if iCount == 2
        else if(iCount == 3)
        {
            // Marvellous CVFS > creat Ganesh.txt 3
            if(strcmp(Command[0], "creat") == 0)
            {
                iRet = CreateFile(Command[1], atoi(Command[2]))         // atoi conver
            }

            if(iRet == ERR_INVALID_PARAMETER)
            {
                printf("error : Invalid parameters for the functoin\n");
                printf("Please check man page for details\n");
            }
            else if(iRet == ERR_NO_INODES)
            {
                printf("error : \n");
            }
            else if(iRet == ARR_FILE_ALREADY_EXIST)
            {
                printf("error : Unable to create as file is already exist \n");
            }
        }  // End of if iCount == 3
        else if(iCount == 4)
        {

        }  // End of if iCount == 4
        else
        {
            printf("Command not found...\n");
            printf("Please refer Help optoin or use man command\n");
        }  // End of Invalid command part
    }   
    // End of while (custom shell)

    return 0;

    // End of main
}