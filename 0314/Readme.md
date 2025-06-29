# X 윈도우와 데스크톱 환경 요약

## 1. X 윈도우 시스템이란?

X 윈도우는 유닉스/리눅스에서 GUI 환경을 제공하기 위해 만들어진 시스템입니다.  
처음에는 1984년 MIT에서 시작되었고, 지금은 X11이 표준으로 자리 잡고 있습니다.

### 주요 특징
- **이식성 뛰어남**: 다양한 시스템에서 잘 작동  
- **클라이언트/서버 구조**: GUI가 분리되어 유연하게 동작  
- **장치 독립성**: 디스플레이 장치에 종속되지 않음  
- **네트워크 투명성**: 원격 시스템에서도 GUI 앱 실행 가능

---

## 2. X 윈도우의 구조

X 윈도우는 크게 두 부분으로 나뉩니다:

| 구성 요소   | 설명 |
|-------------|------|
| **X 서버** | 화면 출력과 사용자 입력을 담당 |
| **X 클라이언트** | 응용 프로그램 (예: 터미널, 웹 브라우저 등) |

- 예: `xclock`, `xterm`, `firefox` 같은 GUI 앱들은 모두 X 클라이언트입니다.  
- 이들은 X 서버에 화면에 출력해달라고 요청하고, 서버는 이를 처리합니다.

---

## 3. X 윈도우 실행과 응용 프로그램

X 윈도우는 데스크톱 환경 위에서 실행됩니다.  
앱을 실행하는 방식은 두 가지가 있습니다:

- **GUI 메뉴 선택** (예: GNOME의 앱 메뉴)
- **터미널 명령어**  
  ```bash
  xclock &
  ```

---

## 4. 데스크톱 환경(Desktop Environment)

### 데스크톱 환경이란?

GUI를 구성하는 전체 패키지입니다.  
창 관리자, 아이콘, 메뉴, 패널, 배경화면 등 사용자가 눈으로 보는 모든 것 포함.

| 주요 환경 | 설명 |
|-----------|------|
| **GNOME** | 가장 널리 사용되는 리눅스 데스크톱 환경 |
| **KDE**   | 고급 사용자 지향의 환경, 다양한 설정 가능 |
| 기타      | XFCE, LXDE 등 경량 환경도 존재 |

### 윈도우 관리자(Window Manager)

창의 위치나 크기 등을 조절하는 프로그램으로  
TWM, FVWM, MWM, Window Maker 등이 예시입니다.  
최근엔 데스크톱 환경에 통합된 경우가 많습니다.

---

## 5. Ubuntu의 GNOME 데스크톱

Ubuntu는 기본적으로 GNOME을 사용합니다.  
화면 구성은 다음과 같습니다:

- **상단바**: 시계, 알림, 네트워크 상태 등 표시  
- **왼쪽 사이드바**: 자주 사용하는 앱 아이콘  
- **바탕화면**: 홈 폴더, 우클릭 메뉴 제공  

### 활동 메뉴 (Activities)

- 현재 실행 중인 프로그램을 확인  
- 검색 바를 통해 프로그램 실행  
- 여러 작업 공간 간 전환 가능

---

## 6. 주요 앱 예시

### 파이어폭스, 파일 관리자, LibreOffice 등

왼쪽 사이드바나 프로그램 검색에서 실행할 수 있음  
**즐겨찾기 추가** 기능을 통해 빠르게 실행 가능

---

## 7. 홈 디렉토리와 파일 관리자

- 사용자의 홈 디렉토리에는 문서, 다운로드, 음악 등 하위 폴더가 자동 생성  
- 바탕화면 또는 사이드바에서 파일 관리자로 접근  
- 우클릭 시 다양한 파일 조작 가능:  
  - 열기, 복사, 이름 바꾸기, 삭제 등

---

## 8. 시스템 설정

**접근 경로 예시**:
- 프로그램 메뉴 → 설정
- 사이드바 상태 메뉴 → 톱니바퀴 아이콘 클릭

### 주요 설정 항목
- 네트워크
- 배경화면
- 외형(테마, 글꼴 등)
- 디스플레이 해상도 및 배치
- 전원 옵션

---

## 9. gedit: 기본 텍스트 편집기

Ubuntu 및 GNOME 환경에서 제공하는 간단한 텍스트 편집기

### 실행 방법
- 프로그램 검색: "텍스트 편집기"
- 터미널 명령어:  
  ```bash
  gedit 파일명
  ```

### 기능 요약
- 여러 탭에서 동시에 편집 가능  
- 찾기/바꾸기, 줄 이동 등 편의 기능  
- 플러그인 추가로 기능 확장 가능  
- 프로그래밍 언어 문법 강조 지원

---

## 10. CentOS의 GNOME 환경

CentOS도 GNOME을 기본 데스크톱 환경으로 사용합니다.

### 구성
- 상단바: 날짜/시간, 알림, 전원/네트워크 상태  
- 사이드바: 즐겨찾기 앱 아이콘  
- 작업 공간 전환 및 활동 확인 기능 제공

---

## 핵심 요약

- X 윈도우는 GUI 환경을 제공하며 서버/클라이언트 구조로 작동  
- GNOME은 Ubuntu, CentOS 등에서 가장 많이 사용되는 데스크톱 환경  
- gedit는 GNOME의 대표적인 편집기로 텍스트 작업에 유용  
- 파일과 시스템은 GUI 또는 터미널에서 자유롭게 관리 가능

> 💡 리눅스 초보자는 GNOME 환경에서 시작하는 것이 가장 직관적이고 편리함
