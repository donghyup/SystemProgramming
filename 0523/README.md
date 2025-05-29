# 🧬 프로세스 생성과 제어 (fork, wait)

---

## 🐣 프로세스 생성: `fork()`

- 부모 프로세스가 자식 프로세스를 생성한다.
- `fork()` 시스템 콜은 **부모를 복제하여 자식 프로세스를 만든다**.
- 이를 **자기복제(Self-replication)** 라고도 한다.

### 📌 사용 예시

```c
#include <sys/types.h>
#include <unistd.h>

pid_t fork(void);
```

부모 프로세스
     |
   fork()
     |
     ├── 자식 프로세스 (리턴값: 0)
     └── 부모 프로세스 (리턴값: 자식 PID)
``` c
pid_t pid = fork();

if (pid == 0) {
  // 자식 프로세스가 실행
  printf("🍼 나는 자식 프로세스!\n");
} else if (pid > 0) {
  // 부모 프로세스가 실행
  printf("👨 나는 부모 프로세스, 자식 PID: %d\n", pid);
} else {
  // fork 실패
  perror("fork");
}
```

``` c
#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *statloc, int options);

```
