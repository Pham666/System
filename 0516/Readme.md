# 파일 시스템과 입출력 요약 정리

## 1. 파일 시스템 확인

### df 명령어
디스크 사용량을 확인하는 명령어

```bash
$ df
```

예시:
```
Filesystem     1K-blocks    Used Available Use% Mounted on
/dev/sda5      204856328 14082764 180297788   8% /
/dev/sda1         523248       4     523244   1% /boot
```

### du 명령어
디렉터리 또는 파일이 차지하는 공간 확인

```bash
$ du
$ du -s
```

예시:
```
208 ./사진
144 ./.local/share/gvfs-metadata
```

---

## 2. 파일 시스템 구조

- **Boot Block**: 부팅에 필요한 코드 포함
- **Super Block**: 전체 파일 시스템 정보 기록
- **i-list**: 파일에 대응되는 i-node 목록
- **Data Block**: 실제 데이터 저장

---

## 3. 파일 상태 정보 확인

### stat 명령어

```bash
$ stat 파일명
```

예시:
```
File: cs1.txt
Size: 2088  Blocks: 8
Access: 2021-10-04 ...
Modify: ...
Change: ...
```

### i-node 확인

```bash
$ ls -i 파일명
```

---

## 4. 유닉스 커널과 시스템 호출

- 커널은 하드웨어를 관리하고 여러 서비스를 제공함
- 사용자 프로그램은 시스템 호출로 커널에 기능을 요청

---

## 5. 파일 입출력 관련 함수

### open()

```c
int open(const char *path, int oflag, [mode_t mode]);
```

옵션:
- `O_RDONLY`: 읽기
- `O_WRONLY`: 쓰기
- `O_RDWR`: 읽기 + 쓰기
- `O_CREAT`: 없으면 생성
- `O_TRUNC`: 기존 내용 제거
- `O_APPEND`: 파일 끝에 덧붙이기

### creat()

```c
int creat(const char *path, mode_t mode);
```

### close()

```c
int close(int fd);
```

### read()

```c
ssize_t read(int fd, void *buf, size_t nbytes);
```

### write()

```c
ssize_t write(int fd, void *buf, size_t nbytes);
```

---

## 6. 예제 코드

### 파일 열기 예제 (fopen.c)

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDWR);
    if (fd == -1)
        printf("파일 열기 실패\n");
    else
        printf("파일 열기 성공: %d\n", fd);
    close(fd);
}
```

---

### 파일 크기 측정 예제 (fsize.c)

```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFSIZE 512

int main(int argc, char *argv[]) {
    char buffer[BUFSIZE];
    int fd;
    ssize_t nread;
    long total = 0;

    if ((fd = open(argv[1], O_RDONLY)) == -1)
        perror(argv[1]);

    while ((nread = read(fd, buffer, BUFSIZE)) > 0)
        total += nread;

    close(fd);
    printf ("%s 파일 크기 : %ld 바이트\n", argv[1], total);
}
```

---

### 파일 복사 예제 (copy.c)

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fd1, fd2, n;
    char buf[BUFSIZ];

    if (argc != 3) {
        fprintf(stderr, "사용법: %s file1 file2\n", argv[0]);
        exit(1);
    }

    if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
        perror(argv[1]);
        exit(2);
    }

    if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        perror(argv[2]);
        exit(3);
    }

    while ((n = read(fd1, buf, BUFSIZ)) > 0)
        write(fd2, buf, n);

    close(fd1);
    close(fd2);
    return 0;
}
```

---

## 7. 요약 정리

- `df`, `du`로 디스크 상태 확인 가능
- `stat`으로 파일 상태 정보 확인
- 시스템 호출(`open`, `read`, `write`, `close`)로 파일 입출력 처리
- 예제 코드를 통해 개념 및 구조 직접 실습 가능
