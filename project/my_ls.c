#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define MAX_FILES 1024

typedef struct {
    char name[256];
    struct stat info;
} FileEntry;

void print_permissions(mode_t mode) {
    char perms[11] = "----------";
    if (S_ISDIR(mode)) perms[0] = 'd';
    if (mode & S_IRUSR) perms[1] = 'r';
    if (mode & S_IWUSR) perms[2] = 'w';
    if (mode & S_IXUSR) perms[3] = 'x';
    if (mode & S_IRGRP) perms[4] = 'r';
    if (mode & S_IWGRP) perms[5] = 'w';
    if (mode & S_IXGRP) perms[6] = 'x';
    if (mode & S_IROTH) perms[7] = 'r';
    if (mode & S_IWOTH) perms[8] = 'w';
    if (mode & S_IXOTH) perms[9] = 'x';
    printf("%s ", perms);
}

void human_readable(off_t size) {
    const char *units[] = {"B", "K", "M", "G", "T"};
    int i = 0;
    double s = size;
    while (s >= 1024 && i < 4) {
        s /= 1024;
        i++;
    }
    printf("%5.1f%s ", s, units[i]);
}

int compare_alpha(const void *a, const void *b) {
    return strcmp(((FileEntry*)a)->name, ((FileEntry*)b)->name);
}

int compare_alpha_reverse(const void *a, const void *b) {
    return strcmp(((FileEntry*)b)->name, ((FileEntry*)a)->name);
}

int compare_size(const void *a, const void *b) {
    return ((FileEntry*)b)->info.st_size - ((FileEntry*)a)->info.st_size;
}

int compare_mtime(const void *a, const void *b) {
    return ((FileEntry*)b)->info.st_mtime - ((FileEntry*)a)->info.st_mtime;
}

void list_directory(const char *path, int a, int l, int h, int r, int t, int S, int d, int one, int color, int recursive) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    FileEntry files[MAX_FILES];
    int count = 0;

    while ((entry = readdir(dir)) != NULL && count < MAX_FILES) {
        if (!a && entry->d_name[0] == '.') continue;
        snprintf(files[count].name, 256, "%s/%s", path, entry->d_name);
        stat(files[count].name, &files[count].info);
        count++;
    }
    closedir(dir);

    // 정렬
    if (t) qsort(files, count, sizeof(FileEntry), compare_mtime);
    else if (S) qsort(files, count, sizeof(FileEntry), compare_size);
    else if (r) qsort(files, count, sizeof(FileEntry), compare_alpha_reverse);
    else qsort(files, count, sizeof(FileEntry), compare_alpha);

    for (int i = 0; i < count; i++) {
        const char *name = strrchr(files[i].name, '/');
        name = name ? name + 1 : files[i].name;

        if (d && !S_ISDIR(files[i].info.st_mode)) continue;

        if (l) {
            print_permissions(files[i].info.st_mode);
            printf("%2ld ", files[i].info.st_nlink);

            struct passwd *pw = getpwuid(files[i].info.st_uid);
            struct group  *gr = getgrgid(files[i].info.st_gid);
            printf("%s %s ", pw ? pw->pw_name : "?", gr ? gr->gr_name : "?");

            if (h) human_readable(files[i].info.st_size);
            else printf("%6ld ", files[i].info.st_size);

            char timebuf[64];
            strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&files[i].info.st_mtime));
            printf("%s ", timebuf);
        }

        if (color && S_ISDIR(files[i].info.st_mode))
            printf("\033[1;34m%s\033[0m", name);
        else
            printf("%s", name);

        printf(one ? "\n" : "  ");
    }

    if (!one) printf("\n");

    // -R: 재귀적으로 하위 디렉토리 출력
    if (recursive) {
        for (int i = 0; i < count; i++) {
            if (S_ISDIR(files[i].info.st_mode) && strcmp(strrchr(files[i].name, '/') + 1, ".") != 0 && strcmp(strrchr(files[i].name, '/') + 1, "..") != 0) {
                printf("\n%s:\n", files[i].name);
                list_directory(files[i].name, a, l, h, r, t, S, d, one, color, recursive);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int a = 0, l = 0, h = 0, r = 0, t = 0, S = 0, R = 0, d = 0, one = 0, color = 0;
    const char *target = ".";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) a = 1;
        else if (strcmp(argv[i], "-l") == 0) l = 1;
        else if (strcmp(argv[i], "-h") == 0) h = 1;
        else if (strcmp(argv[i], "-r") == 0) r = 1;
        else if (strcmp(argv[i], "-t") == 0) t = 1;
        else if (strcmp(argv[i], "-S") == 0) S = 1;
        else if (strcmp(argv[i], "-R") == 0) R = 1;
        else if (strcmp(argv[i], "-d") == 0) d = 1;
        else if (strcmp(argv[i], "-1") == 0) one = 1;
        else if (strcmp(argv[i], "--color") == 0) color = 1;
        else target = argv[i]; // 디렉토리 지정
    }

    list_directory(target, a, l, h, r, t, S, d, one, color, R);
    return 0;
}
