#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>      // 디렉토리 열기/읽기/닫기
#include <sys/stat.h>    // stat, lstat 구조체 및 파일 모드 확인
#include <sys/types.h>
#include <errno.h>       // 오류 메시지 출력용

// -------------------
// 🔧 전역 플래그 변수들
// -------------------
int f_flag = 0;           // -f : 강제 삭제, 오류 무시
int i_flag = 0;           // -i : 삭제 전 매번 확인
int I_flag = 0;           // -I : 첫 번째 삭제 전 한번만 묻기
int r_flag = 0;           // -r or -R : 디렉토리 재귀 삭제
int d_flag = 0;           // -d : 빈 디렉토리만 삭제
int v_flag = 0;           // -v : 삭제 로그 출력
int preserve_root = 1;    // --preserve-root 기본값 활성화
int one_fs_flag = 0;      // --one-file-system 활성화 여부

// 사용자에게 확인 요청 (-i, -I)
int ask_user(const char *msg, const char *path) {
    printf("%s '%s'? [y/N]: ", msg, path);
    int c = getchar();
    while (getchar() != '\n'); // 입력 버퍼 비우기
    return (c == 'y' || c == 'Y');
}

// 파일 시스템 동일 여부 확인 (device 번호 비교)
int is_same_filesystem(const char *path1, const char *path2) {
    struct stat st1, st2;
    if (stat(path1, &st1) != 0 || stat(path2, &st2) != 0)
        return 1;  // 오류 시 같다고 간주
    return (st1.st_dev == st2.st_dev);
}

// 파일/디렉토리 삭제 처리 선언 (재귀적으로 사용됨)
int remove_path(const char *path);

// -------------------
// 📁 디렉토리 재귀 삭제 함수
// -------------------
int remove_dir_recursive(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) return remove(path); // 열 수 없으면 일반 삭제

    struct dirent *entry;
    char fullpath[1024];
    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
        remove_path(fullpath); // 하위 항목 삭제
    }
    closedir(dir);
    return remove(path); // 마지막에 자신 삭제
}

// -------------------
// 📄 파일/디렉토리/링크 삭제 함수
// -------------------
int remove_path(const char *path) {
    struct stat st;

    // 파일 상태 가져오기 (lstat: 심볼릭 링크 추적 X)
    if (lstat(path, &st) != 0) {
        if (!f_flag) perror(path); // -f 없으면 오류 출력
        return 1;
    }

    // --one-file-system 플래그 체크
    if (one_fs_flag && !is_same_filesystem(".", path)) return 0;

    // --preserve-root 설정 시 루트 디렉토리 삭제 방지
    if (preserve_root && strcmp(path, "/") == 0) {
        fprintf(stderr, "rm: refusing to remove '/' (use --no-preserve-root to override)\n");
        return 1;
    }

    // -i: 모든 파일 삭제 전에 묻기
    if (i_flag && !ask_user("remove", path)) return 0;

    // -d 옵션: 빈 디렉토리만 삭제
    if (d_flag && S_ISDIR(st.st_mode)) {
        if (rmdir(path) == 0) {
            if (v_flag) printf("removed directory '%s'\n", path);
            return 0;
        } else {
            perror(path);
            return 1;
        }
    }

    // -r 없이 디렉토리를 삭제하려 할 경우
    if (S_ISDIR(st.st_mode)) {
        if (!r_flag) {
            fprintf(stderr, "rm: cannot remove '%s': Is a directory\n", path);
            return 1;
        }

        // -r 옵션이 있으면 재귀 삭제 실행
        if (remove_dir_recursive(path) == 0) {
            if (v_flag) printf("removed directory '%s'\n", path);
            return 0;
        } else {
            perror(path);
            return 1;
        }
    }

    // -I 옵션 처리: 첫 파일 삭제 전에 한번만 묻기
    static int i_once = 0;
    if (I_flag && !i_once) {
        printf("rm: remove %s? [y/N]: ", path);
        int c = getchar();
        while (getchar() != '\n');
        if (!(c == 'y' || c == 'Y')) return 0;
        i_once = 1;
    }

    // 일반 파일/링크 삭제 (unlink)
    if (unlink(path) == 0) {
        if (v_flag) printf("removed '%s'\n", path);
        return 0;
    } else {
        if (!f_flag) perror(path);
        return 1;
    }
}

// -------------------
// 🚀 프로그램 시작점
// -------------------
int main(int argc, char *argv[]) {
    int arg_start = 1;

    // 옵션 파싱
    for (; arg_start < argc && argv[arg_start][0] == '-'; arg_start++) {
        if (strcmp(argv[arg_start], "-f") == 0) f_flag = 1;
        else if (strcmp(argv[arg_start], "-i") == 0) i_flag = 1;
        else if (strcmp(argv[arg_start], "-I") == 0) I_flag = 1;
        else if (strcmp(argv[arg_start], "-v") == 0) v_flag = 1;
        else if (strcmp(argv[arg_start], "-d") == 0) d_flag = 1;
        else if (strcmp(argv[arg_start], "-r") == 0 || strcmp(argv[arg_start], "-R") == 0) r_flag = 1;
        else if (strcmp(argv[arg_start], "--preserve-root") == 0) preserve_root = 1;
        else if (strcmp(argv[arg_start], "--no-preserve-root") == 0) preserve_root = 0;
        else if (strcmp(argv[arg_start], "--one-file-system") == 0) one_fs_flag = 1;
        else break;
    }

    // 인자 없으면 사용법 출력
    if (arg_start >= argc) {
        fprintf(stderr, "Usage: %s [options] file...\n", argv[0]);
        return 1;
    }

    int errors = 0;

    // 각 인자에 대해 삭제 시도
    for (int i = arg_start; i < argc; i++) {
        errors += remove_path(argv[i]);
    }

    return errors ? 1 : 0;
}
