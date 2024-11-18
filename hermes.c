#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <termios.h>

#define PAGE_SIZE 20
#define MAX_NAME_LEN 256
#define MAX_DISPLAY_LEN 40

char getch()
{
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void truncate_name(char *name)
{
    if (strlen(name) > MAX_DISPLAY_LEN)
    {
        name[MAX_DISPLAY_LEN - 3] = '\0';
        strcat(name, "...");
    }
}

void free_memory(char **dirs, char **files, int dir_count, int file_count)
{
    for (int i = 0; i < dir_count; i++)
        free(dirs[i]);
    for (int i = 0; i < file_count; i++)
        free(files[i]);
    free(dirs);
    free(files);
}

int get_items(char ***dirs, char ***files, int *dir_count, int *file_count)
{
    struct dirent *entry;
    DIR *dp;
    int dir_capacity = PAGE_SIZE;
    int file_capacity = PAGE_SIZE;

    *dirs = malloc(dir_capacity * sizeof(char *));
    *files = malloc(file_capacity * sizeof(char *));
    if (*dirs == NULL || *files == NULL)
    {
        perror("malloc");
        return -1;
    }

    *dir_count = 0;
    *file_count = 0;
    dp = opendir(".");
    if (dp == NULL)
        return -1;

    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            if (*dir_count >= dir_capacity)
            {
                dir_capacity *= 2;
                *dirs = realloc(*dirs, dir_capacity * sizeof(char *));
                if (*dirs == NULL)
                {
                    perror("realloc");
                    closedir(dp);
                    return -1;
                }
            }
            (*dirs)[*dir_count] = malloc(MAX_NAME_LEN);
            strncpy((*dirs)[*dir_count], entry->d_name, MAX_NAME_LEN - 1);
            (*dirs)[*dir_count][MAX_NAME_LEN - 1] = '\0';
            truncate_name((*dirs)[*dir_count]);
            (*dir_count)++;
        }
        else if (entry->d_type == DT_REG)
        {
            if (*file_count >= file_capacity)
            {
                file_capacity *= 2;
                *files = realloc(*files, file_capacity * sizeof(char *));
                if (*files == NULL)
                {
                    perror("realloc");
                    closedir(dp);
                    return -1;
                }
            }
            (*files)[*file_count] = malloc(MAX_NAME_LEN);
            strncpy((*files)[*file_count], entry->d_name, MAX_NAME_LEN - 1);
            (*files)[*file_count][MAX_NAME_LEN - 1] = '\0';
            truncate_name((*files)[*file_count]);
            (*file_count)++;
        }
    }
    closedir(dp);
    return 0;
}


void display_items(char **dirs, char **files, int dir_count, int file_count, int dir_page, int file_page)
{
    system("clear");
    printf("Hermes Navigator:\n");

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n\n", cwd);

    printf("%-1s) %-50s", "q", "Exit Here");
    printf("%-1s) %s\n", "w", "Parent Directory");

    const char dir_keys[] = "ertyuioasdfghjklzxcvbn";
    int dir_start_index = dir_page * PAGE_SIZE;
    int dir_end_index = dir_start_index + PAGE_SIZE;

    for (int i = dir_start_index; i < dir_count && i < dir_end_index; i++)
    {
        if (i % 2 == 0)
            printf("%-1c) %-50s", dir_keys[i], dirs[i]);
        else
            printf("%-1c) %s\n", dir_keys[i], dirs[i]);
    }

    if (dir_start_index > 0)
        printf("m) Previous Page\n");

    if (dir_end_index < dir_count)
        printf(",) Next Page\n");

    for (int i = 0; i < (12 - dir_count/2); i++)
        printf("\n");

    int file_start_index = file_page * PAGE_SIZE;
    int file_end_index = file_start_index + PAGE_SIZE;

    for (int i = file_start_index; i < file_count && i < file_end_index; i++)
    {
        if (i % 2 == 0)
            printf("%-53s", files[i]);
        else
            printf("%s\n", files[i]);
    }

    if (file_start_index > 0)
        printf(".) Previous Page\n");

    if (file_end_index < file_count)
        printf("/) Next Page\n");
}

int navigate_to_directory(char **dirs, int dir_count, char choice)
{
    const char dir_keys[] = "ertyuioasdfghjklzxcvbn";
    for (int i = 0; i < dir_count; i++)
    {
        if (choice == dir_keys[i])
        {
            if (chdir(dirs[i]) != 0)
            {
                perror("chdir");
                return -1;
            }
            return 1;
        }
    }
    return 0;
}

int main()
{
    system("tput smcup");

    char **dirs = NULL;
    char **files = NULL;
    int dir_count = 0;
    int file_count = 0;
    int dir_page = 0;
    int file_page = 0;

    if (get_items(&dirs, &files, &dir_count, &file_count) == -1)
    {
        perror("get_items");
        free_memory(dirs, files, dir_count, file_count);
        return -1;
    }

    while (1)
    {
        display_items(dirs, files, dir_count, file_count, dir_page, file_page);
        char choice = getch();

        switch (choice)
        {
            case 'q':
                free_memory(dirs, files, dir_count, file_count);
                system("tput rmcup");
                return 0;

            case 'w':
                if (chdir("..") != 0)
                {
                    perror("chdir");
                    return -2;
                }
                free_memory(dirs, files, dir_count, file_count);
                get_items(&dirs, &files, &dir_count, &file_count);
                dir_page = 0;
                file_page = 0;
                break;

            case '.':
                if (file_page > 0)
                    file_page--;
                break;

            case '/':
                if ((file_page + 1) * PAGE_SIZE < file_count)
                    file_page++;
                break;

            case 'm':
                if (dir_page > 0)
                    dir_page--;
                break;

            case ',':
                if ((dir_page + 1) * PAGE_SIZE < dir_count)
                    dir_page++;
                break;

            default:
                int result = navigate_to_directory(dirs, dir_count, choice);
                if (result == 1)
                {
                    free_memory(dirs, files, dir_count, file_count);
                    get_items(&dirs, &files, &dir_count, &file_count);
                    dir_page = 0;
                    file_page = 0;
                }
                break;
        }
    }

    free_memory(dirs, files, dir_count, file_count);
    system("tput rmcup");
    return 0;
}
