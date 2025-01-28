project('dbus-server', 'cpp',
    version: '1.0.0',
    default_options: ['cpp_std=c++17']
)

systemd = dependency('systemd', required: false)

executable('dbus-server', 'main.cpp',
    install: true,
    install_dir: join_paths(get_option('prefix'), 'bin')
)
