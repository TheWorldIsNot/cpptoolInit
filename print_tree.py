import os

def print_tree(path, prefix=""):
    # 列出当前目录下的所有非隐藏项
    entries = [entry for entry in os.listdir(path) if not entry.startswith('.')]
    
    for i, entry in enumerate(entries):
        full_path = os.path.join(path, entry)
        if os.path.isdir(full_path):
            connector = "└── " if i == len(entries) - 1 else "├── "
            print(f"{prefix}{connector}{entry}/")
            # 递归打印子目录
            next_prefix = "    " if connector == "└── " else "│   "
            print_tree(full_path, prefix + next_prefix)
        elif entry.endswith('.exe'):
            print(f"{prefix}{' ' * (len(prefix) - 1)}{'└── ' if i == len(entries) - 1 else '├── '}{entry}")

# 调用函数，打印当前目录的文件夹和指定文件类型的树状结构，排除隐藏文件和文件夹
print_tree(os.getcwd())

#, '.cpp', '.h'
