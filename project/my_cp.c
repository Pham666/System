#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <utime.h>
#include <dirent.h>
#include <errno.h>

#define BUF_SIZE 8192  // 복사할 때 사용하는 버퍼 크기

// 파일 복사 함수
int copy_file(const char *src, const char *dst, int i, int u, int v, int f, int n, int p) {
    struct stat src_stat, dst_stat;

    // 원본 파일의 상태 정보 가져오기
    if (stat(src, &src_stat) < 0) {
        perror("stat src");
        return 1;
    }

    int dst_exists = (stat(dst, &dst_stat) == 0);  // 대상 파일 존재 여부 확인

    // -n: 파일이 이미 존재하면 복사하지 않음
    if (n && dst_exists) return 0;

    // -u: 대상이 최신이면 건너뜀
    if (u && dst_exists && dst_stat.st_mtime >= src_stat.st_mtime) return 0;

    // -i: 사용자에게 덮어쓸지 물어봄
    if (i && dst_exists) {
        printf("overwrite '%s'? [y/N]: ", dst);
        int c = getchar();
        if (c != 'y' && c != 'Y') return 0;
    }

    // -f: 강제로 삭제 후 복사
    if (f && dst_exists) remove(dst);

    // 파일 열기
    int in = open(src, O_RDONLY);
    if (in < 0) { perror("open src"); return 1; }

    // 대상 파일 열기 (복사)
    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, src_stat.st_mode);
    if (out < 0) { perror("open dst"); close(in); return 1; }

    // 파일 복사 (버퍼 사용)
    char buf[BUF_SIZE];
    ssize_t bytes;
    while ((bytes = read(in, buf, BUF_SIZE)) > 0)
        write(out, buf, bytes);

    close(in);
    close(out);

    // -p: 시간 및 권한 보존
    if (p) {
        struct utimbuf times;
        times.actime = src_stat.st_atime;
        times.modtime = src_stat.st_mtime;
        utime(dst, &times);
        chmod(dst, src_stat.st_mode);
    }

    // -v: verbose, 복사 로그 출력
    if (v) printf("'%s' -> '%s'\n", src, dst);

    return 0;
}

// 디렉토리 복사 함수 (재귀적 복사)
void copy_dir(const char *src, const char *dst, int i, int u, int v, int f, int n, int p, int r) {
    DIR *dir = opendir(src);
    if (!dir) { perror("opendir"); return; }

    mkdir(dst, 0755);  // 대상 디렉토리 생성

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // 현재 디렉토리(.)와 부모 디렉토리(..)는 건너뜀
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;

        char src_path[512], dst_path[512];
        snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", dst, entry->d_name);

        struct stat st;
        stat(src_path, &st);

        // 디렉토리이면 재귀적으로 복사
        if (S_ISDIR(st.st_mode)) {
            if (r)
                copy_dir(src_path, dst_path, i, u, v, f, n, p, r);
        } else {
            // 파일이면 copy_file 호출
            copy_file(src_path, dst_path, i, u, v, f, n, p);
        }
    }

    closedir(dir);
}

// 프로그램 진입점
int main(int argc, char *argv[]) {
    // 옵션 변수들 (-i, -u, -v, -f, -n, -p, -r, -L)
    int i = 0, u = 0, v = 0, f = 0, n = 0, p = 0, r = 0, L = 0;
    int file_arg_start = 1;

    // 옵션 파싱
    while (file_arg_start < argc && argv[file_arg_start][0] == '-') {
        if (strcmp(argv[file_arg_start], "-i") == 0) i = 1;
        else if (strcmp(argv[file_arg_start], "-u") == 0) u = 1;
        else if (strcmp(argv[file_arg_start], "-v") == 0) v = 1;
        else if (strcmp(argv[file_arg_start], "-f") == 0) f = 1;
        else if (strcmp(argv[file_arg_start], "-n") == 0) n = 1;
        else if (strcmp(argv[file_arg_start], "-p") == 0) p = 1;
        else if (strcmp(argv[file_arg_start], "-r") == 0) r = 1;
        else if (strcmp(argv[file_arg_start], "-L") == 0) L = 1;
        else break;
        file_arg_start++;
    }

    // 최소 2개의 인자가 필요함: source... target
    if (argc - file_arg_start < 2) {
        fprintf(stderr, "Usage: %s [options] source... target\n", argv[0]);
        return 1;
    }

    char *target = argv[argc - 1];
    struct stat tgt_stat;
    int target_is_dir = (stat(target, &tgt_stat) == 0 && S_ISDIR(tgt_stat.st_mode));

    // 여러 source 처리
    for (int iarg = file_arg_start; iarg < argc - 1; iarg++) {
        char *src = argv[iarg];
        char dst[512];

        // 대상이 디렉토리이면 경로 구성
        if (target_is_dir) {
            const char *base = strrchr(src, '/');
            base = base ? base + 1 : src;
            snprintf(dst, sizeof(dst), "%s/%s", target, base);
        } else {
            snprintf(dst, sizeof(dst), "%s", target);
        }

        struct stat src_stat;
        if (lstat(src, &src_stat) < 0) {
            perror("lstat src");
            continue;
        }

        // 디렉토리 복사
        if (S_ISDIR(src_stat.st_mode)) {
            if (r)
                copy_dir(src, dst, i, u, v, f, n, p, r);
            else
                fprintf(stderr, "cp: -r not specified; '%s' is a directory\n", src);
        } else {
            // 심볼릭 링크 처리
            if (!L && S_ISLNK(src_stat.st_mode)) {
                char link_buf[512];
                readlink(src, link_buf, sizeof(link_buf));
                symlink(link_buf, dst);  // 심볼릭 링크 복사
            } else {
                // 일반 파일 복사
                copy_file(src, dst, i, u, v, f, n, p);
            }
        }
    }

    return 0;
}
