#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "sys/stat.h"
#include "sys/types.h"
#include "dirent.h"
#include "string.h"
#include <unistd.h>
#include <time.h>

typedef enum {false, true} bool;

char* buildCommandString(char *command, char params[], int sizeOfParams, int iter)
{
    if(iter == sizeOfParams)
    {
        return command;
    }
    else
    {
        strcat(command,&params[iter]);
        char *newCommandString = buildCommandString(command,params,sizeOfParams,iter++);
        return newCommandString;
    }
}
int executeCommand(char *path, char *command, char params[])
{
    char *noCom = "noCom";
    char *space = " ";
    char *delLong = "-delete";
    char *delShort = "-d";
    if (strcmp(command,noCom) == 0)
    {
        //then we dont need to execute any command
        return 0;
    }
    // build command string to be executed for all commands beside
    // del along with whatever in the params buffer

    if(strcmp(command,delLong) == 0 || strcmp(command,delShort) == 0)
    {
        if (remove(path) == 0)
        {
            printf("Deleted file %s\n",path);
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        char *commandStrTemp = malloc(sizeof(char) *2000);
        commandStrTemp = strcat(command,space);
        int sizeOfParam = sizeof(params)/sizeof(char);
        int iter = 0;
        char *finalCommandString = buildCommandString(commandStrTemp, params, sizeOfParam, iter);

        return system(finalCommandString);
    }
}

#if 0
int findMMin(char *dir, int min, bool lessThan, char *command, char params[])
{
    DIR *subDirPtr = opendir(dir);
    struct dirent * subDirent;
    struct stat fileStatBuf;

    char dot[] = ".";
    char dubDot[] = "..";
    char slash[] = "/";

    if (subDirPtr != NULL)
    {
        char *temp = subDirent->d_name;
        while((subDirent = readdir(subDirPtr)) != NULL)
        {
            if(strcmp(temp,dot) != 0 && strcmp(temp,dubDot) != 0)
            {
                char *tempSubDir = slash;
                //add slash in front of sub dir name
                tempSubDir = strcat(tempSubDir,temp);
                //add slash after the sub dir name
                char *tempFullPath = malloc(sizeof(char)*2000);
                tempFullPath = strcpy(tempFullPath,dir);
                strcat(tempFullPath,tempSubDir);
                // full path constructed

                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                

                //try to open new path
                DIR *newSubDirPtr = opendir(tempFullPath);

                if (newSubDirPtr != NULL)
                {
                    closedir(newSubDirPtr);
                    findMMin(tempFullPath,min,lessThan,command,params);
                }
                else
                {
                    // we know that it isnt a sub directory so we get stat on it
                    if (stat(tempFullPath,&fileStatBuf) == 0)
                    {
                        //check if the st_mtime is less than or eq to min
                        if (lessThan == true && fileStatBuf.st_mtime < min)
                        {

                            printf("%s\n",tempFullPath);
                            if (executeCommand(tempFullPath,command,params) != 0)
                            {
                                fprintf(stderr,"could not execute command %s on file %s",command,tempFullPath);
                            }
                        }
                        if (lessThan == false && fileStatBuf.st_mtime > min)
                        {
                            printf("%s\n",tempFullPath);
                            if (executeCommand(tempFullPath,command,params) != 0)
                            {
                                fprintf(stderr,"could not execute command %s on file %s",command,tempFullPath);
                            }
                        }
                        return;

                    }
                }
            }
        }
        closedir(subDirPtr);
        return;
    }
    else
    {
        printf("Error: Cannot open directory\n");
        exit(2);
    }
}
#endif




void findMMin(char *cwd,int mMin,bool lessThan, char *command, char params[])
{

    DIR *subDirPtr = opendir(cwd);
    struct dirent *subDirent;
    struct stat *fileStatBuf = malloc(sizeof(struct stat));

    char dot[] = ".";
    char dubDot[] = "..";
    char slash[] = "/";
    char *retFilePath = "\0";


    if (subDirPtr != NULL)
    {
        while((subDirent = readdir(subDirPtr)) != NULL)
        {
            char *temp = subDirent->d_name;

            char *tempFullPath = malloc(sizeof(char) *2000);
            tempFullPath = strcpy(tempFullPath, cwd);
            strcat(tempFullPath,slash);
            strcat(tempFullPath, temp);

                
            if (stat(tempFullPath,fileStatBuf) == 0)
            {
                time_t t = time(NULL);
                int currentTime = t;  
                int minLastAccessed = currentTime - fileStatBuf->st_mtime;
             
                int minInSeconds = mMin *60;
                
                //check if the st_mtime is less than ir eq to min
                if (lessThan == true && minLastAccessed < minInSeconds)
                {
                    printf("%s\n",tempFullPath);
                    if (executeCommand(tempFullPath,command,params) != 0)
                    {
                        fprintf(stderr,"could not execute command %s on file %s",command,tempFullPath);
                    }
                }
                if (lessThan == false && minLastAccessed > minInSeconds)
                {
                    printf("%s\n",tempFullPath);
                    if (executeCommand(tempFullPath,command,params) != 0)
                    {
                        fprintf(stderr,"could not execute command %s on file %s",command,tempFullPath);
                    }
                }
            }

            if(strcmp(temp,dot) != 0 && strcmp(temp,dubDot) != 0)
            {

                DIR *tempSubDirPtr = opendir(tempFullPath);

                if (tempSubDirPtr != NULL)
                {
                    closedir(tempSubDirPtr);
                    findMMin(tempFullPath, mMin, lessThan, command, params);
                }
            }
            free(tempFullPath);
        }
        free(fileStatBuf);
    }
    else
    {
        printf("Error: Cannot open directory\n");
        exit(2);
    }

}





void findINum(char *cwd, int iNum, char * command, char params[])
{

    printf("findinum was called on path cwd:%s \t inum:%d \t command: %s \t  params: %s \n",cwd,iNum,command,params);
    fflush(stdout);

    DIR *subDirPtr = opendir(cwd);
    struct dirent *subDirent;
    struct stat *fileStatBuf = malloc(sizeof(struct stat));

    char dot[] = ".";
    char dubDot[] = "..";
    char slash[] = "/";

    if (subDirPtr != NULL)
    {
        while((subDirent = readdir(subDirPtr)) != NULL)
        {
             char *temp = subDirent->d_name;
             printf("Just started while loop with file name: %s \n\n",temp);

             if(strcmp(temp,dot) != 0 && strcmp(temp,dubDot) != 0)
	     {
                char * tempFullPath = malloc(sizeof(char) *2000);
                tempFullPath = strcpy(tempFullPath, cwd);
                strcat(tempFullPath,slash);
                strcat(tempFullPath, temp);


                //try to open new path
                DIR *newSubDirPtr = opendir(tempFullPath);

                if (newSubDirPtr != NULL)
                {
                    printf("We successfully opened up a new subdir and are about to do recursive call: %s\n",tempFullPath);
                    closedir(newSubDirPtr);
                    findINum(tempFullPath,iNum,command,params);
                }
                else
                {
      		    printf("Couldnt open it as a directory so now we check for inum path: %s\n",tempFullPath);
                    //we know that it is not a sub dir and just a file
                    //now we check its d_name from our dirent struct
                    if (stat(tempFullPath,fileStatBuf) == 0)
          	    {
                        int tempINum = fileStatBuf->st_ino;
                        printf("iterating through and currently have inode num; %d \n",tempINum);
                        if(iNum == tempINum)
			{
				printf("%s \n",tempFullPath);
				if (executeCommand(tempFullPath,command,params) != 0)
                                {
					fprintf(stderr,"Could not execute command: %s",tempFullPath);
				}
                        }
		    }
               }
               free(tempFullPath);
            }
        }
        free(fileStatBuf);
        printf("made it thru the while loop and about to return\n\n");
    }
    else
    {
        printf("Error: Cannot open directory\n");
        exit(2);
    }
}


void findName(char *cwd, char *name, char * command, char params[])

{


    DIR *subDirPtr = opendir(cwd);
    struct dirent *subDirent;

    char dot[] = ".";
    char dubDot[] = "..";
    char slash[] = "/";

    if (subDirPtr != NULL)
    {
        while((subDirent = readdir(subDirPtr)) != NULL)
        {
             char *temp = subDirent->d_name;
             if(strcmp(temp,dot) != 0 && strcmp(temp,dubDot) != 0)
	     {
                char * tempFullPath = malloc(sizeof(char) *2000);
                tempFullPath = strcpy(tempFullPath, cwd);
                strcat(tempFullPath,slash);
                strcat(tempFullPath, temp);


                //try to open new path
                DIR *newSubDirPtr = opendir(tempFullPath);

                if (newSubDirPtr != NULL)
                {
                    closedir(newSubDirPtr);
                    findName(tempFullPath,name,command,params);
                }
                else
                {
                    //we know that it is not a sub dir and just a file
                    //now we check its d_name from our dirent struct
                    if (strcmp(temp,name) == 0)
                    {
                        printf("%s \n",tempFullPath);
                        if (executeCommand(tempFullPath,command,params) != 0)
                        {
                            fprintf(stderr,"Could not execute command %s on file %s",command,tempFullPath);
                        }
                    }
                }
                free(tempFullPath);
            }
        }
        return;
    }
    else
    {
        printf("Error: Cannot open directory\n");
        exit(2);
    }
}

char *findDir(char *cwd,char *dirName, bool print)
{
    DIR *subDirPtr = opendir(cwd);
    struct dirent *subDirent;

    char dot[] = ".";
    char dubDot[] = "..";
    char slash[] = "/";
    char *retFilePath = "\0";


    if (subDirPtr != NULL)
    {
        while((subDirent = readdir(subDirPtr)) != NULL)
        {
            char *temp = subDirent->d_name;
            if (strcmp(temp,dirName) == 0)
            {

                char * tempFullPath = malloc(sizeof(char) *2000);
                tempFullPath = strcpy(tempFullPath, cwd);
                strcat(tempFullPath,slash);
                strcat(tempFullPath, temp);

                if (print == true)
                {
                    printDir(tempFullPath);
                }
                closedir(subDirPtr);
                retFilePath = tempFullPath;
                return retFilePath;
            }

            if(strcmp(temp,dot) != 0 && strcmp(temp,dubDot) != 0)
            {

                char *tempFullPath = malloc(sizeof(char) *2000);
                tempFullPath = strcpy(tempFullPath, cwd);
                strcat(tempFullPath,slash);
                strcat(tempFullPath, temp);
                DIR *tempSubDirPtr = opendir(tempFullPath);

                if (tempSubDirPtr != NULL)
                {
                    closedir(tempSubDirPtr);
                    retFilePath = findDir(tempFullPath,dirName,print);
                    closedir(subDirPtr);
                    return retFilePath;
                }
            }

        }
        if (retFilePath == "/0")
        {
            fprintf(stderr,"could not file directory in your current working directory\n");
            exit(2);
        }
        return retFilePath;
    }
    else
    {
        printf("Error: Cannot open directory\n");
        exit(2);
    }
}
void printDir(char *cwd)
{
    DIR *dirPtr = opendir(cwd);
    struct dirent *dirEnt;

    char *slash = "/";
    char *dot = ".";
    char *dotdot = "..";
    char *pathPrefix = malloc(sizeof(cwd) + sizeof(slash)+1);
    pathPrefix = strcat(cwd,slash);



    if (dirPtr != NULL)
    {
        while((dirEnt = readdir(dirPtr)) != NULL)
        {
            char *temp = dirEnt->d_name;
            if (strcmp(temp,dot) != 0 && strcmp(temp,dotdot) != 0)
            {

               char *tempFullPath = malloc(sizeof(char) *2000);
               tempFullPath = strcpy(tempFullPath, cwd);
               strcat(tempFullPath, temp);
               DIR *tempSubDirPtr = opendir(tempFullPath);


               if (tempSubDirPtr != NULL)
               {

                    closedir(tempSubDirPtr);
 		    printDir(tempFullPath);
	       }
               printf("%s\n",tempFullPath);
            }
        }
    }
    else
    {
        fprintf(stderr,"Could not open specified working directory\n");
    }
}

int main(int argc, char *argv[])
{
#if 0
    //using these to distinguish which option is passed
    bool criteriaName = false;
    bool criteriaInode = false;
    bool criteraMmin = false;
    bool deleting = false;
    bool execute = false;

    char *criteriaArg = '\0';
    char *criteriaSpecArg = '\0';
    char *zArg = '\0';
#endif
    char *params[100];

    char *cwd[PATH_MAX-10];

    if (getcwd(cwd,sizeof(cwd)) == NULL)
    {
        printf("getcwd errno flag is: %s\n",strerror(errno));
        fprintf(stderr,"Could not open your current working directory\n");
        return -1;
        exit(2);

    }
    printf("current working directory string after getcwd: %s\n",cwd);

    if (argc == 1)
    {
        // this is the default for find
        //just print the contents of the current working directory
       printDir(cwd);
       return 0;
    }
    else
    {
        //out of default mode so now we need to parse command line arguments
        char *whereToLook = argv[1];
        printf("whereToLook is: %s\n",whereToLook);\

        if (argc == 2)
        {
            bool printing = true;
            findDir(cwd, whereToLook, printing);
            return 0;
        }
        else if (argc == 3)
        {
            fprintf(stderr,"Missing or invalid input parameter \n");
            return -1;
        }
        else if (argc == 4)
        {
            char *critNameFull = "-name";
            char *critMminFull = "-mmin";
            char *critInumFull = "-inum";
            char *critNamePart = "-n";
            char *critMminPart = "-m";
            char *critInumPart = "-i";
            char *command = "noCom";
            char params[1];
            bool printing = false;

            char *criterArg = argv[2];

            char *thisDir = findDir(cwd,whereToLook,printing);
            printf("directory path string after findDir(): %s\n",thisDir);

            if (strcmp(criterArg,critInumFull) == 0 || strcmp(criterArg, critInumPart) == 0)
            {
                char *iNumTemp = argv[3];
                int iNum = atoi(iNumTemp);
                printf("%d \n",iNum);
                findINum(thisDir,iNum,command,params);
                printf("GOT OUT OF FINDINUM SUCCESSFULLY \n\n");
                return 0;
            }
            else if (strcmp(criterArg,critNameFull) == 0 || strcmp(criterArg, critNamePart) == 0)
            {
                char *nameArg = argv[3];
                findName(thisDir,nameArg,command,params);
            }
            else if (strcmp(criterArg,critMminFull) == 0 || strcmp(criterArg,critMminPart) == 0)
            {
                char *mMinTemp = argv[3];
                int mMin = atoi(mMinTemp);
                if (mMin < 0)
                {
                    bool lessThan = true;
                    //convert from negative to positive after noting that it was negative
                    int posMin = mMin * -1;
                    mMin = mMin + (2 * posMin);
                    findMMin(cwd,mMin,lessThan,command, params);
                    return 0;
                }
                else if (mMin > 0)
                {
                    bool lessThan = false;
                    findMMin(cwd,mMin,lessThan,command,params);
                    return 0;
                }
                else
                {
                    fprintf(stderr,"mmin value cannot be zero\n");
                    return -1;

                }
            }
            else
            {
                fprintf(stderr,"search criteria %s not recognized",criterArg);
            }
        }
        else if (argc > 5)
        {
            char *critNameFull = "-name";
            char *critMminFull = "-mmin";
            char *critInumFull = "-inum";
            char *critNamePart = "-n";
            char *critMminPart = "-m";
            char *critInumPart = "-i";
            char *command = argv[4];

            // have to build the params buffer
            char params[100];
            int iter = 5;
            int pIndex = 0;
            char *space = " ";

            for (iter; iter < argc; iter++)
            {
                char *strTemp = argv[iter];
                int iter2 = 0;
                for (iter2;iter2 < strlen(strTemp);iter2++)
                {
                    params[pIndex] = strTemp[iter2];
                    pIndex++;
                }
                params[pIndex] = space;
                pIndex++;
            }
            // params should now be full


            bool printing = false;

            char *criterArg = argv[3];

            char *thisDir = findDir(cwd,whereToLook,printing);

            if (strcmp(criterArg,critInumFull) == 0 || strcmp(criterArg, critInumPart) == 0)
            {
                char *iNumTemp = argv[3];
                int iNum = atoi(iNumTemp);
                findINum(thisDir,iNum,command,params);
                return 0;
            }
            else if (strcmp(criterArg,critNameFull) == 0 || strcmp(criterArg, critNamePart) == 0)
            {
                char *nameArg = argv[3];
                findName(thisDir,nameArg,command,params);
            }
            else if (strcmp(criterArg,critMminFull) == 0 || strcmp(criterArg,critMminPart) == 0)
            {
                char *mMinTemp = argv[3];
                int mMin = atoi(mMinTemp);
                if (mMin < 0)
                {
                    bool lessThan = true;
                    //convert from negative to positive after noting that it was negative
                    int posMin = mMin * -1;
                    mMin = mMin + (2 * posMin);
                    findMMin(cwd,mMin,lessThan,command, params);
                    return 0;
                }
                else if (mMin > 0)
                {
                    bool lessThan = false;
                    findMMin(cwd,mMin,lessThan,command,params);
                    return 0;
                }
                else
                {
                    fprintf(stderr,"mmin value cannot be zero\n");
                    return -1;

                }
            }
            else
            {
                fprintf(stderr,"search criteria %s not recognized",criterArg);
            }
        }
    }
#if 0
    if (argc == 2)
    {

    }
    if (argc == 3)
    {
        criteriaArg = argv[2];
        printf("Error: most provide input after %c \n", *criteriaArg);
        return -1;
    }
    if (argc >= 4)
    {
        criteriaArg = argv[2];
        criteriaSpecArg = argv[3];
        char *critN = "-n";
        char *critI = "-i";
        char *critM = "-m";

        if(strcmp(criteriaArg,critN) == 0)
        {
            criteriaName = true;
        }
        else if(strcmp(criteriaArg, critI) == 0)
        {
            criteriaInode = true;
        }
        else if(strcmp(criteriaArg,critM) == 0)
        {
            criteraMmin = true;
        }
        else
        {
            printf("Error: %s not recognized as valid input parameter.\n",criteriaArg);
            return -1;
        }
    }
    if (argc == 5)
    {
        zArg = argv[4];
        execute == true;
        char *del = "-delete";
        if (strcmp(zArg,del) == 0)
        {
            deleete = true;
        }
        else
        {
            printf("Error: find: %s is not recognized as valid input parameter",zArg);
        }
    }
    if (argc > 5)
    {

        if (criteriaName)
    }
    if(criteriaName == true)
    {
        char *fileName = *criteriaSpecArg;
        if (execute == true)
            {
                char *exec = *zArg;
                //TODO execution for name criteria
            }
        findName(whereToLook,fileName);
        return 0;
    }
    if (criteriaInode == true)
    {
        if(execute == true)
        {
            char *exec = *zArg;
            //TODO get the execution part working for inode criteria
        }
        int iNum = atoi(*criteriaSpecArg);
        findINode(whereToLook,iNum);
        return 0;
    }
    if(criteraMmin == true)
    {
        if (execute == true)
        {
            char *exec = *zArg;
            //TODO get execution part working for mmin criteria
        }
        int min = atoi(*criteriaSpecArg);
        findMMin(whereToLook,min);
        return 0;
    }
#endif

    return 0;
}

