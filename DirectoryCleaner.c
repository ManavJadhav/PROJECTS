#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

void *ThreadProc1(void *ptr)
{
    char *DirName = (char *)ptr;
    DIR *dp = opendir(DirName);
    if (dp == NULL)
    {
        printf("Unable to open directory\n");
        return NULL;
    }

    struct dirent *entry = NULL;
    char name[30];
    struct stat sobj;

    while ((entry = readdir(dp)) != NULL)
    {
        sprintf(name, "%s/%s", DirName, entry->d_name);
        stat(name, &sobj);

        if (sobj.st_size == 0)
        {
            remove(name);
            printf("Thread1 Deleted the empty file--> %s\n: ",name);
        }
    }

    closedir(dp);

    pthread_exit(NULL);
}

void *ThreadProc2(void *ptr)
{
    char *DirName = (char *)ptr;
    DIR *dp = opendir(DirName);
    if (dp == NULL)
    {
        printf("Unable to open directory\n");
        return NULL;
    }

    struct dirent *entry = NULL;
    char name[30];
    struct stat sobj;

    while ((entry = readdir(dp)) != NULL)
    {
        sprintf(name, "%s/%s", DirName, entry->d_name);
        stat(name, &sobj);

        if (sobj.st_size > 1000000)
        {
            remove(name);
            printf("Thread2 Deleted the file more than 1MB --> %s\n: ",name);
        }
    }

    closedir(dp);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    char DirName[20];
    printf("Enter name of directory: \n");
    scanf("%s", DirName);

    pthread_t TID1, TID2;
    int ret1 = 0, ret2 = 0;

    ret1 = pthread_create(&TID1,        // Address of pthread_attr_t structure object
                          NULL,        // Thread attributes
                          ThreadProc1, // Address of callback function
                          DirName);    // Parameters to callback function

    if (ret1 != 0)
    {
        printf("Unable to create thread\n");
        return -1;
    }

    printf("Thread1 is successfully created with ID: %u \n ", TID1); // Which Deletes files greater than 1 MB

    ret2 = pthread_create(&TID2,        // Address of pthread_attr_t structure object
                          NULL,        // Thread attributes
                          ThreadProc2, // Address of callback function
                          DirName);    // Parameters to callback function

    if (ret2 != 0)
    {
        printf("Unable to create thread\n");
        return -1;
    }

    printf("Thread2 is successfully created with ID: %u \n ", TID2); // Which Deletes Empty files

    pthread_join(TID1, NULL);
    pthread_join(TID2, NULL);

    printf("End of main thread\n");

    return 0;
}
