# 컴파일러 설정
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# 실행 파일 이름
TARGET = ex1

# 소스 코드 및 오브젝트 파일
SRCS = main.cpp socket_server.cpp
OBJS = $(SRCS:.cpp=.o)

# 기본 빌드 규칙
all: $(TARGET)

# 실행 파일 생성
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 개별 소스 파일을 오브젝트 파일로 변환
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# clean: 불필요한 파일 삭제
clean:
	rm -f $(OBJS) $(TARGET)

# 실행
run: $(TARGET)
	./$(TARGET)

# 디버그 빌드
debug: CXXFLAGS += -g
debug: clean all
