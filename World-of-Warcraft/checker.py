import os
import difflib

# ================= 配置区域 =================
# 这里填写你编译好的 exe 文件名
my_program = "MyWowFinal.exe"   # <--- 修改这里：改成你实际的 exe 名字

# 输入和标准答案的文件名
input_file = "input.txt"
answer_file = "answer.txt"
# ===========================================

# --- 下面是原有逻辑，无需修改 ---

# 1. 运行你的程序并生成输出
# 命令解释：运行 test.exe，从 input.txt 读取数据 (<)，将结果写入 output.txt (>)
cmd_run = f"{my_program} < {input_file} > output.txt"
os.system(cmd_run)

# 2. 读取两个文件的内容进行比对
try:
    with open("output.txt", "r", encoding="utf-8") as f1, open(answer_file, "r", encoding="utf-8") as f2:
        lines1 = f1.readlines()
        lines2 = f2.readlines()

    if lines1 == lines2:
        print("✅ 答案正确！(AC)")
    else:
        print("❌ 发现差异！(WA)")
        # 打印具体的差异
        diff = difflib.unified_diff(lines1, lines2, fromfile="你的输出", tofile="标准答案")
        for line in diff:
            print(line, end='')
            
except FileNotFoundError:
    print("❌ 错误：找不到文件。请检查 input.txt, answer.txt 是否存在，或者 exe 是否运行成功生成了 output.txt")
except Exception as e:
    print(f"❌ 发生错误：{e}")