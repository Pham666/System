#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024        // 한 줄 최대 길이
#define MAX_PATTERNS 10      // 검색할 수 있는 최대 패턴 수

char *patterns[MAX_PATTERNS];  // 검색할 패턴 목록
int pattern_count = 0;         // 현재까지 등록된 패턴 수

// -----------------------------
// 🔍 문자열 검색 함수
// ignore_case가 true이면 대소문자 무시하고 패턴 찾기
// -----------------------------
int match(const char *line, int ignore_case) {
    for (int i = 0; i < pattern_count; i++) {
        const char *target = line;
        const char *p = patterns[i];

        if (ignore_case) {
            // 소문자로 변환하여 비교
            char lower_line[MAX_LINE], lower_pattern[MAX_LINE];
            for (int j = 0; line[j]; j++) lower_line[j] = tolower(line[j]);
            lower_line[strlen(line)] = 0;
            for (int j = 0; p[j]; j++) lower_pattern[j] = tolower(p[j]);
            lower_pattern[strlen(p)] = 0;

            target = lower_line;
            p = lower_pattern;
        }

        // strstr: 부분 문자열 검색
        if (strstr(target, p)) return 1;
    }
    return 0;
}

// -----------------------------
// 🚀 프로그램 진입점
// -----------------------------
int main(int argc, char *argv[]) {
    // 옵션 플래그
    int i_flag = 0;  // -i: 대소문자 무시
    int v_flag = 0;  // -v: 패턴 불일치 행만 출력
    int n_flag = 0;  // -n: 행 번호 출력
    int c_flag = 0;  // -c: 일치 개수 출력
    int l_flag = 0;  // -l: 파일 이름만 출력
    int file_start = 0;  // 파일 인자 시작 위치

    // -------------------------
    // 🧩 옵션 및 패턴 파싱
    // -------------------------
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-i")) i_flag = 1;
        else if (!strcmp(argv[i], "-v")) v_flag = 1;
        else if (!strcmp(argv[i], "-n")) n_flag = 1;
        else if (!strcmp(argv[i], "-c")) c_flag = 1;
        else if (!strcmp(argv[i], "-l")) l_flag = 1;
        else if (!strcmp(argv[i], "-e") && i + 1 < argc) {
            // -e PATTERN 형식
            patterns[pattern_count++] = argv[++i];
        }
        // 알 수 없는 옵션
        else if (argv[i][0] == '-' && strcmp(argv[i], "-") != 0) {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            return 1;
        }
        // 첫 번째 비옵션 인자 → 기본 패턴
        else {
            if (pattern_count == 0) patterns[pattern_count++] = argv[i++];
            file_start = i;
            break;
        }
    }

    // 필수 인자 누락 확인
    if (pattern_count == 0 || file_start >= argc) {
        fprintf(stderr, "Usage: %s [options] pattern file...\n", argv[0]);
        return 1;
    }

    // -------------------------
    // 📂 각 파일에 대해 처리
    // -------------------------
    for (int f = file_start; f < argc; f++) {
        FILE *fp = fopen(argv[f], "r");
        if (!fp) {
            perror(argv[f]);  // 파일 열기 실패 시 에러 출력
            continue;
        }

        char line[MAX_LINE];
        int line_num = 0, match_count = 0, printed = 0;

        // 파일 한 줄씩 읽기
        while (fgets(line, MAX_LINE, fp)) {
            line_num++;
            int is_match = match(line, i_flag);  // 패턴 매칭
            if (v_flag) is_match = !is_match;    // -v 옵션 시 반전

            if (is_match) {
                match_count++;
                if (c_flag) continue;            // -c: 출력 안함, 개수만 셈
                if (l_flag && !printed) {
                    printf("%s\n", argv[f]);     // -l: 파일 이름만 출력
                    printed = 1;
                    break;
                }
                if (!l_flag) {
                    if (n_flag) printf("%d:", line_num);  // -n: 행 번호
                    printf("%s", line);                   // 실제 행 출력
                }
            }
        }

        // -c 옵션이면 총 일치 개수 출력
        if (c_flag) {
            printf("%s:%d\n", argv[f], match_count);
        }

        fclose(fp);
    }

    return 0;
}
