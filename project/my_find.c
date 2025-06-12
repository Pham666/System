#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>   // stat(), lstat() 구조체 포함
#include <unistd.h>
#include <dirent.h>     // 디렉토리 처리
#include <fnmatch.h>    // 와일드카드 문자열 비교 함수 fnmatch()

// -----------------------------
// 🔧 전역 설정 변수
// -----------------------------
int maxdepth = -1;             // 최대 탐색 깊이 (-1: 무제한)
int mindepth = 0;              // 최소 탐색 깊이
char *name_pattern = NULL;     // -name 패턴
char type = 0;                 // -type (f, d)
char *size_cond = NULL;        // -size 조건 문자열
char *exec_cmd = NULL;         // -exec 명령 문자열

// -----------------------------
// 🛠 -exec 명령어 실행 함수
// {} 자리에 파일 경로를 삽입하여 system()으로 실행
// -----------------------------
void run_exec(const char *filepath) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), exec_cmd, filepath);  // {} 자리에는 "%s" 형식 사용
    system(cmd);
}

// -----------------------------
// 📏 -size 조건 평가 함수
// +N: N보다 큰 파일, -N: 작은 파일, N: 정확히 N
// -----------------------------
int size_match(off_t filesize) {
    if (!size_cond) return 1;  // 조건이 없으면 항상 true

    char op = size_cond[0];
    long target = atol(size_cond + 1);

    if (op == '+') return filesize > target;
    if (op == '-') return filesize < target;
    return filesize == atol(size_cond);  // 아무 기호도 없으면 정확히 일치
}

// -----------------------------
// 🔍 디렉토리 탐색 함수 (재귀)
// -----------------------------
void search(const char *path, int depth) {
    if (maxdepth != -1 && depth > maxdepth) return;  // 최대 깊이 초과 시 중단

    DIR *dir = opendir(path);
    if (!dir) return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // 현재 디렉토리와 상위 디렉토리는 제외
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;

        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        struct stat st;
        if (lstat(fullpath, &st) == -1) continue;  // 파일 정보 가져오기 실패 시 건너뜀

        int is_match = 1;  // 기본은 일치
        if (depth < mindepth) is_match = 0;

        // -name 옵션 처리
        if (name_pattern && fnmatch(name_pattern, entry->d_name, 0) != 0)
            is_match = 0;

        // -type 옵션 처리
        if (type == 'f' && !S_ISREG(st.st_mode)) is_match = 0;
        if (type == 'd' && !S_ISDIR(st.st_mode)) is_match = 0;

        // -size 옵션 처리
        if (!size_match(st.st_size)) is_match = 0;

        // 모든 조건이 만족된 경우 출력 및 실행
        if (is_match) {
            printf("%s\n", fullpath);
            if (exec_cmd) run_exec(fullpath);  // -exec 명령 실행
        }

        // 디렉토리인 경우 재귀 탐색
        if (S_ISDIR(st.st_mode))
            search(fullpath, depth + 1);
    }

    closedir(dir);
}

// -----------------------------
// 🚀 프로그램 시작점
// -----------------------------
int main(int argc, char *argv[]) {
    const char *root = ".";  // 기본 시작 디렉토리는 현재 디렉토리
    int i = 1;

    // 첫 번째 인자가 디렉토리 경로일 수 있음
    if (argc > 1 && argv[1][0] != '-') {
        root = argv[1];
        i++;
    }

    // -------------------------
    // 🧩 옵션 파싱
    // -------------------------
    for (; i < argc; i++) {
        if (!strcmp(argv[i], "-name") && i + 1 < argc)
            name_pattern = argv[++i];
        else if (!strcmp(argv[i], "-type") && i + 1 < argc)
            type = argv[++i][0];  // 'f' or 'd'
        else if (!strcmp(argv[i], "-maxdepth") && i + 1 < argc)
            maxdepth = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-mindepth") && i + 1 < argc)
            mindepth = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-size") && i + 1 < argc)
            size_cond = argv[++i];
        else if (!strcmp(argv[i], "-exec") && i + 1 < argc) {
            exec_cmd = argv[++i];  // "{}" 자리에 %s로 넣을 명령어
            // 마지막 인자가 ";"라면 생략 처리
            if (i + 1 < argc && strcmp(argv[i + 1], ";") == 0)
                i++;
        }
    }

    // -------------------------
    // 🔍 탐색 시작
    // -------------------------
    search(root, 0);

    return 0;
}
