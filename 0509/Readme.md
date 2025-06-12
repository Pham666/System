# 파일 시스템과 파일 입출력 요약

## 1. 파일 시스템 확인

### 1.1 df 명령어
- 디스크 공간 사용 현황 확인

```bash
$ df
```

예시 출력:
```
Filesystem     1K-blocks    Used Available Use% Mounted on
/dev/sda5      204856328 14082764 180297788   8% /
/dev/sda1         523248       4     523244   1% /boot
```

### 1.2 du 명령어
- 디렉토리나 파일의 디스크 사용량 확인

```bash
$ du
$ du -s
```

예시 출력:
```
208 ./사진
144 ./.local/share/gvfs-metadata
```

---

## 2. 리눅스 파일 시스템 구조

- **Boot Block**: 파일 시스템 시작에 위치, 부팅 코드 포함
- **Super Block**: 전체 파일 시스템 정보 저장
- **i-list**: 파일의 메타데이터를 가진 i-node들의 목록
- **Data Block**: 파일 실제 데이터 저장

---

## 3. 파일 상태 정보

### stat 명령어 사용

```bash
$ stat 파일이름
```

예시 출력:
```
File: cs1.txt
Size: 2088  Blocks: 8
Access: 2021-10-04 ...
Modify: ...
Change: ...
```

### i-node 번호 확인

```bash
$ ls -i cs1.txt
```

---

## 4. 유닉스 커널과 시스템 호출

- 커널은 파일, 메모리, 프로세스, 장치, 통신 관리 담당
- 응용 프로그램은 시스템 호출을 통해 커널에 요청

---

## 5. 파일 입출력 함수

### 5.1 open()

```c
int open(const char *path, int oflag, mode_t mode);
```

- `O_RDONLY`, `O_WRONLY`, `O_RDWR`, `O_CREAT`, `O_APPEND` 등 사용
- 성공 시 파일 디스크립터, 실패 시 -1

### 5.2 creat()

```c
int creat(const char *path, mode_t mode);
```

- `open(path, O_WRONLY | O_CREAT | O_TRUNC, mode)` 과 동일 효과

### 5.3 close()

```c
int close(int fd);
```

### 5.4 read()

```c
ssize_t read(int fd, void *buf, size_t nbytes);
```

### 5.5 write()

```c
ssize_t write(int fd, void *buf, size_t nbytes);
```

---

## 6. 예제 코드

### 6.1 fopen.c

```c
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDWR);
    if (fd == -1)
        printf("파일 열기 오류\n");
    else
        printf("파일 %s 열기 성공: %d\n", argv[1], fd);
    close(fd);
}
```

---

### 6.2 fsize.c

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

### 6.3 copy.c

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

## 7. 정리

- `df`, `du`로 디스크 사용량 확인 가능
- `stat`, `ls -i`로 파일의 상태 및 i-node 확인
- `open`, `read`, `write`, `close` 등 시스템 호출 기반 입출력
- 예제 코드로 실제 사용법 익히기
