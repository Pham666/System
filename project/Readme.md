# 🛠 리눅스 명령어 C 구현 모음

이 프로젝트는 리눅스의 대표적인 명령어들을 C 언어로 직접 구현한 실습 예제입니다.  
`cp`, `rm`, `tar`, `grep`, `find` 명령어를 직접 구현함으로써 파일 시스템, 디렉토리 순회, 문자열 처리, 시스템 호출 등 저수준 시스템 프로그래밍을 학습할 수 있습니다.

---

## 📁 파일 구성

├── cp.c # cp 명령어 구현
├── rm.c # rm 명령어 구현
├── tar.c # tar 명령어 wrapper
├── grep.c # grep 명령어 구현
├── find.c # find 명령어 구현


---

## 📄 명령어별 요약

### ✅ cp.c - 파일/디렉토리 복사

- **기능**: 파일 또는 디렉토리를 조건에 따라 복사
- **지원 옵션**:
  - `-i` : 덮어쓰기 전 사용자 확인
  - `-u` : 대상이 최신이면 복사 생략
  - `-v` : 복사 경로 출력
  - `-f` : 복사 전에 삭제
  - `-n` : 이미 존재하면 복사하지 않음
  - `-p` : 권한 및 시간 정보 보존
  - `-r` : 디렉토리 재귀 복사
  - `-L` : 심볼릭 링크를 따라 복사
- `copy_file()` / `copy_dir()` 함수로 구성됨

---

### ✅ rm.c - 파일/디렉토리 삭제

- **기능**: 파일 및 디렉토리를 조건에 따라 삭제
- **지원 옵션**:
  - `-f` : 강제 삭제, 오류 무시
  - `-i` : 매번 삭제 여부 확인
  - `-I` : 첫 삭제 전 한 번만 확인
  - `-v` : 삭제 로그 출력
  - `-d` : 빈 디렉토리만 삭제
  - `-r`, `-R` : 재귀 삭제
  - `--preserve-root` / `--no-preserve-root` : 루트 디렉토리 보호 설정
  - `--one-file-system` : 다른 파일시스템 경계 넘지 않음

---

### ✅ tar.c - tar 명령어 래퍼

- **기능**: `tar` 명령어의 주요 기능을 조합하여 실행
- **지원 옵션**:
  - `-c`, `-x`, `-t` : 생성, 해제, 리스트 출력
  - `-v` : verbose 출력
  - `-z`, `-j`, `-J` : gzip, bzip2, xz 압축
  - `-f [filename]` : 아카이브 파일 이름 지정
  - `--exclude=PATTERN` : 패턴 제외
  - `--remove-files` : 압축 후 원본 삭제
  - `--no-overwrite-dir` : 디렉토리 덮어쓰기 방지
- 내부적으로 `system()`으로 명령어 실행

---

### ✅ grep.c - 문자열 검색

- **기능**: 파일 내 특정 문자열 검색
- **지원 옵션**:
  - `-i` : 대소문자 구분 없이 검색
  - `-v` : 불일치 라인 출력
  - `-n` : 라인 번호 출력
  - `-c` : 일치 개수 출력
  - `-l` : 파일 이름만 출력
  - `-e PATTERN` : 검색 패턴 지정
- 최대 10개 패턴 검색 가능  
- `match()` 함수로 case-insensitive 처리

---

### ✅ find.c - 파일 탐색 및 조건 필터

- **기능**: 디렉토리 내 파일을 조건에 맞게 검색
- **지원 옵션**:
  - `-name PATTERN` : 이름 패턴 일치
  - `-type [f|d]` : 파일/디렉토리 구분
  - `-maxdepth N` / `-mindepth N` : 탐색 깊이 제한
  - `-size [+N|-N|N]` : 파일 크기 조건
  - `-exec "CMD %s"` : 조건 일치 시 명령 실행
- `search()` 함수로 재귀 탐색 수행  
- `fnmatch()`로 와일드카드 매칭 지원

---

## 🧪 컴파일 & 실행 예시

```bash
gcc -o mycp cp.c
gcc -o myrm rm.c
gcc -o mytar tar.c
gcc -o mygrep grep.c
gcc -o myfind find.c

./mycp -v -p a.txt b.txt
./myrm -r testdir
./mytar -cvf test.tar testdir
./mygrep -in "main" test.c
./myfind . -name "*.c" -size +100 -exec "echo Found: %s" \;

```bash
# 파일을 다른 이름으로 복사
./mycp source.txt backup.txt

# 덮어쓰기 여부 확인
./mycp -i file1.txt file2.txt

# 디렉토리를 재귀 복사
./mycp -r src_dir/ dest_dir/

# 권한과 시간 보존하며 복사
./mycp -p -v file.txt copied.txt


# 파일 삭제
./myrm file.txt

# 디렉토리 전체 삭제 (재귀 + 강제)
./myrm -rf testdir/

# 삭제 전에 확인
./myrm -i file.txt

# 삭제 로그 출력
./myrm -v file.txt


# 파일들을 tar로 묶기
./mytar -cvf archive.tar file1.txt file2.txt

# tar.gz로 압축
./mytar -czvf archive.tar.gz myfolder/

# tar 해제
./mytar -xvf archive.tar

# 특정 파일 제외하고 압축
./mytar -cvf archive.tar --exclude=secret.txt *.txt


# 파일 내에서 "main" 문자열 검색
./mygrep main sample.c

# 대소문자 무시하고 검색
./mygrep -i hello README.md

# 행 번호 포함 출력
./mygrep -n return test.c

# 일치하는 행 수만 출력
./mygrep -c error log.txt

# 파일 이름만 출력
./mygrep -l TODO *.md


# 현재 경로에서 .c 파일 찾기
./myfind . -name "*.c"

# 파일 크기가 1000바이트 이상인 파일 찾기
./myfind . -size +1000

# 최대 깊이 2까지만 탐색
./myfind . -maxdepth 2 -name "*.txt"

# 디렉토리만 검색
./myfind . -type d

# 찾은 파일마다 명령어 실행
./myfind . -name "*.log" -exec "echo Found: %s" \;


