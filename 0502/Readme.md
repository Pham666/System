# 파일 시스템과 파일 입출력 정리

---

## 1. 파일 시스템 확인

### 디스크 용량 확인

```bash
$ df
```

* 전체 파일 시스템의 디스크 사용량을 확인할 수 있음

**출력 예시**

```
Filesystem     1K-blocks    Used Available Use% Mounted on
/dev/sda5      204856328 14082764 180297788   8% /
/dev/sda1         523248       4     523244   1% /boot
```

### 디렉터리 용량 확인

```bash
$ du [-s] [파일/디렉토리]
```

* 디렉터리 혹은 파일의 사용량 확인

```bash
$ du -s
22164   .
```

---

## 2. 파일 시스템 구조

* **부트 블록**: 부팅 코드
* **슈퍼 블록**: 파일 시스템 전체 정보
* **i-list**: i-node들의 목록
* **데이터 블록**: 실제 데이터 저장

---

## 3. 파일 상태 확인

### `stat` 명령어

```bash
$ stat 파일명
```

* 파일 크기, 접근 권한, 링크 수 등 상세 정보 출력

### i-node 번호 확인

```bash
$ ls -i 파일명
```

* 해당 파일의 i-node 번호 확인 가능

---

## 4. 시스템 구조와 시스템 호출

* **커널 역할**

  * 파일, 프로세스, 메모리, 장치 관리 등

* **시스템 호출**

  * 응용 프로그램이 커널에 요청을 전달하는 방법

---

## 5. 파일 입출력 시스템 호출

### 1) 파일 열기: `open()`

```c
#include <fcntl.h>
int open(const char *path, int oflag, [mode_t mode]);
```

* 성공 시 파일 디스크립터(int) 반환, 실패 시 -1

**주요 옵션**:

* `O_RDONLY`, `O_WRONLY`, `O_RDWR`
* `O_CREAT`, `O_TRUNC`, `O_APPEND`, `O_EXCL`

```c
int fd = open("file.txt", O_WRONLY | O_CREAT, 0600);
```

### 2) 파일 생성: `creat()`

```c
#include <fcntl.h>
int creat(const char *path, mode_t mode);
```

* 쓰기 전용으로 파일 생성

### 3) 파일 닫기: `close()`

```c
#include <unistd.h>
int close(int fd);
```

* 파일 디스크립터를 닫음

### 4) 파일 읽기: `read()`

```c
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t nbytes);
```

* `nbytes` 만큼 읽고 실제 읽은 바이트 수 반환

### 5) 파일 쓰기: `write()`

```c
#include <unistd.h>
ssize_t write(int fd, void *buf, size_t nbytes);
```

* 버퍼 내용을 파일에 기록

---

## 6. 예제 코드

### (1) 파일 열기 예제

```c
int fd = open("sample.txt", O_RDWR);
if (fd == -1) {
    printf("파일 열기 실패\n");
} else {
    printf("파일 열기 성공: %d\n", fd);
    close(fd);
}
```

### (2) 파일 크기 계산

```c
char buffer[512];
int fd = open(argv[1], O_RDONLY);
ssize_t nread;
long total = 0;
while ((nread = read(fd, buffer, 512)) > 0)
    total += nread;
close(fd);
printf("크기: %ld 바이트\n", total);
```

### (3) 파일 복사 프로그램

```c
int fd1 = open(argv[1], O_RDONLY);
int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
char buf[BUFSIZ];
int n;
while ((n = read(fd1, buf, BUFSIZ)) > 0)
    write(fd2, buf, n);
```

---

## 핵심 요약

* `df`, `du`, `stat`, `ls -i` 등으로 파일/디스크 상태 확인
* `open()`, `read()`, `write()`, `close()`를 이용한 파일 조작
* 파일은 i-node를 통해 시스템 내에서 관리됨
* 시스템 호출은 응용 프로그램이 운영체제 기능을 이용할 수 있게 함
* 파일 디스크립터는 열린 파일을 식별하는 정수 값
