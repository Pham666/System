# VI 에디터 사용법 정리

## 모드 종류

- **기본 모드 (Normal Mode)**  
  vi를 실행하면 처음 진입하는 모드  
  커서 이동, 삭제, 복사, 붙여넣기, 검색 등 가능  
  삽입 모드나 명령 모드에서 Esc를 눌러 복귀

- **삽입 모드 (Insert Mode)**  
  텍스트 입력 가능  
  진입 방법:
  - `i` : 현재 커서 앞
  - `a` : 현재 커서 뒤
  - `o` : 아래 줄 새 줄 생성
  - `O` : 위 줄 새 줄 생성  
  Esc로 일반 모드로 복귀

- **명령 모드 (Command Mode)**  
  `:`를 눌러 진입  
  저장, 종료, 치환 등 수행 가능  

  예시:
  - `:w` → 저장  
  - `:wq` → 저장 후 종료  
  - `:q!` → 저장하지 않고 종료  
  - `:s/old/new/g` → 현재 줄 치환  
  - `:%s/old/new/g` → 전체 치환  

---

# 쉘 개요

## 쉘이란

- 사용자 명령을 해석하여 커널에 전달하는 프로그램  
- 명령어 처리기 역할  
- 유닉스/리눅스의 CLI 환경 핵심 구성 요소  

## 주요 쉘 종류

| 쉘 | 설명 |
|-----|------|
| Bourne shell(sh) | 전통적인 기본 쉘 |
| Korn shell(ksh) | Bourne shell 확장 |
| Bash | 대부분 리눅스의 기본 쉘 |
| C shell(csh) | C 언어 문법과 유사 |
| tcsh | C shell 개선 버전 |

---

## 로그인 쉘 변경

```bash
$ chsh
# 로그인 쉘 변경 시 사용
