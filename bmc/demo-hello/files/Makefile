# 定義源文件和目標文件
SRCS := main.cpp                # 定義源文件，這裡是 main.cpp
OBJS := $(SRCS:.cpp=.o)         # 定義目標文件，將 .cpp 文件轉換為 .o 文件

# 定義安裝目錄
bindir ?= /usr/bin              # 安裝目錄，默認為 /usr/bin

# 定義最終可執行文件的名稱
TARGET := hello                 # 定義最終生成的可執行文件名稱，這裡是 hello

# 默認目標
all: $(TARGET)                  # 設定默認目標為生成 hello 文件

# 編譯規則：將 .cpp 文件編譯為 .o 文件
%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)  # 使用 CXX 編譯器將 .cpp 文件編譯為 .o 文件

# 連接規則：將 .o 文件連接為最終的可執行文件
$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)    # 使用 CXX 編譯器將 .o 文件連接為可執行文件

# 安裝規則：將可執行文件安裝到指定目錄
install:
	install -d $(DESTDIR)$(bindir)   # 創建目標安裝目錄
	install -m 755 $(TARGET) $(DESTDIR)$(bindir)/  # 將可執行文件安裝到目標目錄，並設置權限
