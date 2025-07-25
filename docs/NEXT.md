# Mini OS - Next Development Session Guide

## 📍 현재 상태 (Phase 3 완료)

**마지막 업데이트**: 2024-07-25  
**현재 Phase**: Phase 3 Complete ✅  
**다음 Phase**: Phase 4 - Memory Management  

### 완료된 기능들
- ✅ Phase 1: 기본 부트로더 및 커널 설정
- ✅ Phase 2: 향상된 출력 시스템 (터미널, 색상, 그리기)
- ✅ Phase 3: 키보드 입력 처리 및 인터럽트 시스템

## 🎯 다음 세션 시작하기

### 1. 환경 확인 및 복원

```bash
# 1. 프로젝트 디렉토리로 이동
cd /mnt/c/Users/huntk/workspace/git/test_os/mini-os

# 2. 현재 상태 확인
git status
git log --oneline -5

# 3. 빌드 환경 확인
make clean
make all

# 4. QEMU로 테스트
make run
```

### 2. 현재 작동하는 기능들

**테스트 가능한 명령어들:**
- `help` - 사용 가능한 명령어 표시
- `clear` - 화면 지우기
- `echo Hello World` - 텍스트 출력
- `version` - 버전 정보
- `status` - 시스템 상태
- `reboot` - 시스템 재부팅 (실제로는 halt)

## 🚀 Phase 4: 메모리 관리 시스템 시작

### Phase 4 목표
- 물리적 메모리 관리
- 가상 메모리 및 페이징
- 메모리 할당/해제
- 메모리 보호

### Phase 4 구현 계획

#### 4.1 물리적 메모리 관리
**파일**: `include/memory.h`, `kernel/memory.c`

**구현할 기능들:**
- 메모리 맵 생성 및 관리
- 물리적 메모리 할당자 (Bitmap 기반)
- 메모리 영역 관리 (사용 가능/사용 중/예약됨)
- 메모리 통계 및 정보

**핵심 함수들:**
```c
// 메모리 관리자 초기화
void memory_init(void);

// 물리적 메모리 할당/해제
void* pmm_alloc(size_t pages);
void pmm_free(void* addr, size_t pages);

// 메모리 정보 조회
size_t pmm_get_total_pages(void);
size_t pmm_get_free_pages(void);
size_t pmm_get_used_pages(void);
```

#### 4.2 가상 메모리 시스템
**파일**: `include/paging.h`, `kernel/paging.c`

**구현할 기능들:**
- 페이지 디렉토리 및 페이지 테이블 설정
- 페이지 폴트 핸들러
- 가상 주소를 물리 주소로 변환
- 메모리 보호 (읽기/쓰기/실행 권한)

**핵심 함수들:**
```c
// 페이징 시스템 초기화
void paging_init(void);

// 페이지 할당/해제
void* vmm_alloc_page(void);
void vmm_free_page(void* addr);

// 주소 변환
void* virtual_to_physical(void* virtual_addr);
void* physical_to_virtual(void* physical_addr);

// 페이지 폴트 핸들러
void page_fault_handler(interrupt_frame_t* frame);
```

#### 4.3 커널 힙 할당자
**파일**: `include/heap.h`, `kernel/heap.c`

**구현할 기능들:**
- 동적 메모리 할당 (malloc/free)
- 메모리 단편화 방지
- 힙 통계 및 디버깅

**핵심 함수들:**
```c
// 힙 초기화
void heap_init(void);

// 동적 메모리 할당
void* kmalloc(size_t size);
void kfree(void* ptr);

// 힙 정보
size_t heap_get_used(void);
size_t heap_get_free(void);
```

### 3. 구현 순서 및 단계

#### Step 1: 메모리 맵 생성
1. **파일 생성**: `include/memory.h`
   - 메모리 관련 구조체 정의
   - 메모리 맵 구조체
   - 함수 선언

2. **파일 생성**: `kernel/memory.c`
   - BIOS INT 15h E820 메모리 맵 수집
   - 메모리 영역 분류 (사용 가능/예약됨/ACPI 등)
   - 메모리 통계 계산

#### Step 2: 물리적 메모리 할당자
1. **Bitmap 기반 할당자 구현**
   - 페이지 단위 할당 (4KB)
   - 최적화된 검색 알고리즘
   - 연속 페이지 할당

2. **메모리 관리자 통합**
   - 초기화 함수
   - 할당/해제 함수
   - 디버깅 및 통계

#### Step 3: 페이징 시스템
1. **페이지 테이블 구조 설정**
   - 페이지 디렉토리 생성
   - 커널 공간 매핑
   - 사용자 공간 준비

2. **페이지 폴트 핸들러**
   - 인터럽트 14번 처리
   - 동적 페이지 할당
   - 메모리 보호 위반 처리

#### Step 4: 커널 힙
1. **동적 할당자 구현**
   - 블록 기반 할당
   - 단편화 방지
   - 메모리 정리

2. **시스템 통합**
   - 커널에서 사용
   - 메모리 누수 방지
   - 디버깅 지원

### 4. 필요한 기술적 배경

#### x86 메모리 관리
- **페이징**: 32비트 x86 페이징 구조
- **페이지 크기**: 4KB (기본)
- **주소 변환**: 2단계 페이지 테이블
- **권한**: 읽기/쓰기/사용자/관리자

#### 메모리 맵 정보
- **BIOS INT 15h E820**: 메모리 맵 수집
- **메모리 타입**: 
  - 1: 사용 가능
  - 2: 예약됨
  - 3: ACPI 재사용 가능
  - 4: ACPI NVS

### 5. 파일 구조 계획

```
mini-os/
├── include/
│   ├── memory.h          🔄 새로 생성
│   ├── paging.h          🔄 새로 생성
│   └── heap.h            🔄 새로 생성
├── kernel/
│   ├── memory.c          🔄 새로 생성
│   ├── paging.c          🔄 새로 생성
│   └── heap.c            🔄 새로 생성
└── docs/
    └── NEXT.md           ✅ 현재 파일
```

### 6. Makefile 업데이트 계획

**추가할 소스 파일들:**
```makefile
KERNEL_SRC += $(KERNEL_DIR)/memory.c $(KERNEL_DIR)/paging.c $(KERNEL_DIR)/heap.c
KERNEL_HEADERS += include/memory.h include/paging.h include/heap.h
KERNEL_OBJ += $(BUILD_DIR)/memory.o $(BUILD_DIR)/paging.o $(BUILD_DIR)/heap.o
```

### 7. 테스트 계획

#### 단위 테스트
1. **메모리 맵 테스트**
   - 메모리 영역 정확성 확인
   - 통계 계산 검증

2. **할당자 테스트**
   - 단일 페이지 할당/해제
   - 연속 페이지 할당
   - 메모리 누수 검사

3. **페이징 테스트**
   - 주소 변환 정확성
   - 페이지 폴트 처리
   - 메모리 보호

#### 통합 테스트
1. **시스템 부팅 테스트**
   - 메모리 시스템 초기화
   - 커널 힙 사용

2. **성능 테스트**
   - 할당 속도 측정
   - 메모리 사용량 모니터링

### 8. 디버깅 및 문제 해결

#### 예상되는 문제들
1. **메모리 맵 수집 실패**
   - BIOS 호환성 문제
   - 대체 방법 구현 필요

2. **페이지 폴트 무한 루프**
   - 페이지 폴트 핸들러 디버깅
   - 재귀적 페이지 폴트 방지

3. **메모리 누수**
   - 할당/해제 불균형
   - 디버깅 도구 구현

#### 디버깅 도구
1. **메모리 상태 출력**
   - 사용 가능한 메모리 표시
   - 할당된 페이지 목록

2. **힙 디버깅**
   - 할당된 블록 추적
   - 메모리 누수 검사

### 9. 참고 자료

#### 기술 문서
- **Intel x86 Manual**: Volume 3A, Chapter 4 (Paging)
- **OSDev Wiki**: Memory Management, Paging
- **Multiboot Specification**: 메모리 맵 정보

#### 구현 예제
- **xv6**: 교육용 OS의 메모리 관리
- **Linux**: 커널 메모리 관리자
- **FreeBSD**: 물리적 메모리 할당자

### 10. 성공 기준

#### Phase 4 완료 조건
- [ ] 물리적 메모리 할당자 작동
- [ ] 페이징 시스템 구현
- [ ] 커널 힙 할당자 구현
- [ ] 메모리 보호 기능
- [ ] 시스템 통합 및 테스트
- [ ] 문서화 완료

#### 품질 기준
- **안정성**: 메모리 누수 없음
- **성능**: 효율적인 할당 알고리즘
- **확장성**: 향후 기능 확장 가능
- **디버깅**: 문제 진단 도구

## 🎯 다음 세션 시작 체크리스트

### 환경 준비
- [ ] 프로젝트 디렉토리 확인
- [ ] Git 상태 확인
- [ ] 빌드 환경 테스트
- [ ] QEMU 실행 확인

### 계획 수립
- [ ] Phase 4 목표 재검토
- [ ] 구현 순서 결정
- [ ] 필요한 자료 수집
- [ ] 테스트 계획 수립

### 개발 시작
- [ ] `include/memory.h` 생성
- [ ] 메모리 맵 구조체 정의
- [ ] BIOS 메모리 맵 수집 함수 구현
- [ ] 첫 번째 테스트 실행

## 📝 참고 사항

### 현재 시스템 제한사항
- **메모리**: 16MB QEMU 설정
- **아키텍처**: x86 32비트
- **페이징**: 4KB 페이지만 지원
- **보호**: 기본적인 읽기/쓰기 권한

### 향후 확장 계획
- **대용량 페이지**: 2MB, 4MB 페이지 지원
- **메모리 압축**: 사용하지 않는 페이지 압축
- **스왑**: 디스크 기반 가상 메모리
- **NUMA**: 다중 프로세서 메모리 관리

---

**마지막 업데이트**: 2024-07-25  
**다음 검토**: Phase 4 시작 시 