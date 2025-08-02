# hello-1.0/meson.build
project('hello', 'cpp',
        version : '1.0',
        default_options : ['cpp_std=c++17'])

# 讓 Yocto 傳來的 --bindir 決定安裝路徑，別手動寫 /usr/bin
executable('hello', 'main.cpp',
           install: true,
           install_dir: get_option('bindir'))