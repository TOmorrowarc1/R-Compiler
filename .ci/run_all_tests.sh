EXECUTABLE="./code"

TEST_DATA_DIR=".ci"

DATA_PATTERN="*.in"

# --- 脚本核心 ---

# set -e: 脚本中的任何命令失败（返回非零退出码），则立即退出。
# 这对于CI至关重要，确保一个测试点失败时整个测试流程会失败。
set -e

echo "--- Start to build the project: ---"

rm -rf "$BUILD_DIR"
cmake -S . -B "$BUILD_DIR"
cmake --build "$BUILD_DIR" 
if [ ! -x "$EXECUTABLE_PATH" ]; then
    echo "Error: failed to find the executable at '$EXECUTABLE_PATH'"
    exit 1
fi

echo "construction succeed."
echo ""

# 遍历所有匹配模式的测试数据文件
# shopt -s nullglob: 如果找不到匹配的文件，循环不会执行，而不是把 "*.txt" 当作一个文件名
shopt -s nullglob
for test_file in "$TEST_DATA_DIR"/$DATA_PATTERN; do
    # 从完整路径中提取文件名，用于日志输出
    filename=$(basename "$test_file")

    echo "Pending: $filename"
    # 核心命令：运行可执行文件，并使用 '<' 将测试文件的内容重定向为标准输入
    "$EXECUTABLE" < "$test_file"

    echo "PASS: $filename"
    echo ""
done

echo "所有测试点成功通过！"