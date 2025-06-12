#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int c_flag = 0, x_flag = 0, t_flag = 0, v_flag = 0;
    int z_flag = 0, j_flag = 0, J_flag = 0;
    int remove_flag = 0, no_overwrite_dir = 0;
    char *archive_name = NULL;
    char *exclude_pattern = NULL;

    int i;
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-c")) c_flag = 1;
        else if (!strcmp(argv[i], "-x")) x_flag = 1;
        else if (!strcmp(argv[i], "-t")) t_flag = 1;
        else if (!strcmp(argv[i], "-v")) v_flag = 1;
        else if (!strcmp(argv[i], "-z")) z_flag = 1;
        else if (!strcmp(argv[i], "-j")) j_flag = 1;
        else if (!strcmp(argv[i], "-J")) J_flag = 1;
        else if (!strcmp(argv[i], "--remove-files")) remove_flag = 1;
        else if (!strcmp(argv[i], "--no-overwrite-dir")) no_overwrite_dir = 1;
        else if (strncmp(argv[i], "--exclude=", 10) == 0)
            exclude_pattern = argv[i] + 10;
        else if (!strcmp(argv[i], "-f")) {
            if (i + 1 < argc) {
                archive_name = argv[++i];
            } else {
                fprintf(stderr, "tar: option -f requires a filename\n");
                return 1;
            }
        } else break;
    }

    if (!archive_name) {
        fprintf(stderr, "tar: no archive filename specified (-f required)\n");
        return 1;
    }

    // 명령어 조합
    char cmd[1024] = "tar ";
    if (c_flag) strcat(cmd, "-c ");
    if (x_flag) strcat(cmd, "-x ");
    if (t_flag) strcat(cmd, "-t ");
    if (v_flag) strcat(cmd, "-v ");
    if (z_flag) strcat(cmd, "-z ");
    if (j_flag) strcat(cmd, "-j ");
    if (J_flag) strcat(cmd, "-J ");
    strcat(cmd, "-f ");
    strcat(cmd, archive_name);
    strcat(cmd, " ");

    if (exclude_pattern) {
        strcat(cmd, "--exclude=");
        strcat(cmd, exclude_pattern);
        strcat(cmd, " ");
    }
    if (remove_flag) strcat(cmd, "--remove-files ");
    if (no_overwrite_dir) strcat(cmd, "--no-overwrite-dir ");

    // 남은 인자 (파일/디렉토리)
    for (; i < argc; i++) {
        strcat(cmd, "\"");
        strcat(cmd, argv[i]);
        strcat(cmd, "\" ");
    }

    if (v_flag) printf("Executing: %s\n", cmd);

    int ret = system(cmd);
    return (ret == 0) ? 0 : 1;
}
