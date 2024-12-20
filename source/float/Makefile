# Makefile

# コンパイラとフラグ
CC = gcc
CFLAGS = -Wall -Werror -g
LDFLAGS = -lm

# ヘッダーファイルのディレクトリ
INCLUDES = -I./math

# ターゲットの実行ファイル
TARGET = test

# ソースファイル
SRCS = test.c math/fadd.c math/fsub.c math/fmul.c math/fdiv.c math/fsqrt.c math/finv.c math/fabsf.c \
       math/fcvtsw.c math/fcvtws.c math/feq.c math/flt.c math/fneg.c math/fsgnjn.c math/fsgnjx.c 

# オブジェクトファイル
OBJS = $(SRCS:.c=.o)

# すべてのターゲット
all: $(TARGET)

# ターゲットのビルドルール
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# クリーンアップ
clean:
	rm -f $(TARGET) $(OBJS)

# 個々のソースファイルをオブジェクトファイルにコンパイルするルール
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@