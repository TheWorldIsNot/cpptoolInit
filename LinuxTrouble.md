要保存并退出 `nano` 编辑器，您可以按照以下步骤操作：

1. **保存更改**： 按下 `Ctrl + O`（大写字母 O，代表 "Write Out"）以保存对文件的更改。
2. **确认文件名**： 如果 `nano` 要求确认文件名，按 `Enter` 键确认或修改文件名。
3. **退出编辑器**： 按下 `Ctrl + X` 退出编辑器。如果 `nano` 提示是否保存更改，输入 `y` 表示 "Yes" 保存更改，然后按 `Enter` 键。

---

```shell
ping -c 4 baidu.com

wsl -l -v

sudo nano /etc/apt/sources.list

# 默认注释了源码仓库，如有需要可自行取消注释
deb https://mirrors.ustc.edu.cn/ubuntu/ jammy main restricted universe multiverse
# deb-src https://mirrors.ustc.edu.cn/ubuntu/ jammy main restricted universe multiverse

deb https://mirrors.ustc.edu.cn/ubuntu/ jammy-security main restricted universe multiverse
# deb-src https://mirrors.ustc.edu.cn/ubuntu/ jammy-security main restricted universe multiverse

deb https://mirrors.ustc.edu.cn/ubuntu/ jammy-updates main restricted universe multiverse
# deb-src https://mirrors.ustc.edu.cn/ubuntu/ jammy-updates main restricted universe multiverse

deb https://mirrors.ustc.edu.cn/ubuntu/ jammy-backports main restricted universe multiverse
# deb-src https://mirrors.ustc.edu.cn/ubuntu/ jammy-backports main restricted universe multiverse

# 预发布软件源，不建议启用
# deb https://mirrors.ustc.edu.cn/ubuntu/ jammy-proposed main restricted universe multiverse
# deb-src https://mirrors.ustc.edu.cn/ubuntu/ jammy-proposed main restricted universe multiverse


sudo apt update
sudo apt install git
git clone https://github.com/DamionGans/ubuntu-wsl2-systemd-script.git
cd ubuntu-wsl2-systemd-script/
bash ubuntu-wsl2-systemd-script.sh


wsl --shutdown


# 默认注释了源码仓库，如有需要可自行取消注释
deb http://mirrors.ustc.edu.cn/debian bookworm main contrib non-free non-free-firmware
# deb-src http://mirrors.ustc.edu.cn/debian bookworm main contrib non-free non-free-firmware
deb http://mirrors.ustc.edu.cn/debian bookworm-updates main contrib non-free non-free-firmware
# deb-src http://mirrors.ustc.edu.cn/debian bookworm-updates main contrib non-free non-free-firmware

# backports 软件源，请按需启用
# deb http://mirrors.ustc.edu.cn/debian bookworm-backports main contrib non-free non-free-firmware
# deb-src http://mirrors.ustc.edu.cn/debian bookworm-backports main contrib non-free non-free-firmware
##############################################################
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
mingw32-make

#cmake -B ./build 是 mk build   cd build   cmake..   cd..  的简写，-S -B指定source build目录
```



```shell
#Ubuntu GUI设置遭遇的问题
wsl bash --norc
sudo nano /usr/sbin/enter-systemd-namespace


USER_HOME="$(getent passwd | awk -F: '$1=="'"$SUDO_USER"'" {print $6}')"
if [ -n "$SYSTEMD_PID" ] && [ "$SYSTEMD_PID" != "1" ]; then
    if [ -n "$1" ] && [ "$1" != "bash --login" ] && [ "$1" != "/bin/bash --login" ]; then
        exec /usr/bin/nsenter -t "$SYSTEMD_PID" -m -p \
            /usr/bin/sudo -H -u "$SUDO_USER" \
            /bin/bash -c 'set -a; [ -f "$HOME/.systemd-env" ] && source "$HOME/.systemd-env"; set +a; exec bash -c '"$(printf "%q" "$@")"
    else
        exec /usr/bin/nsenter -t "$SYSTEMD_PID" -m -p \
            /bin/login -p -f "$SUDO_USER" \
            $([ -f "$USER_HOME/.systemd-env" ] && /bin/cat "$USER_HOME/.systemd-env" | xargs printf ' %q')
    fi
    echo "Existential crisis"
    exit 1
fi
```

```shell
#google benchmark编译
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=install -DBENCHMARK_ENABLE_GTEST_TESTS=FALSE

cmake --build build --config Release -j4 --target install --verbose
```

.
