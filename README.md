# Memory Latency Benchmark (Random Access)

랜덤액세스 상황에서 레이턴시를 측정하는 무언가이다.

# 사용방법

CMake로 빌드한다.
```
cmake CMakeLists.txt
make
```

대충 실행한다.
```
./memBench (인수1) (인수2)
```
$2^{(인수1)}$ Byte ~ $2^{(인수2)}$ Byte
범위에서 테스트를 진행한다.

그럼 결과가 `log.txt`에 저장되어 있을 것이다.