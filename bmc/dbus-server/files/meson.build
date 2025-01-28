project('dbus-server', 'cpp',
    version: '1.0.0',
    default_options: ['cpp_std=c++20']
)

cc = meson.get_compiler('cpp')
boost = dependency('boost', required: true)
nlohmann_json = dependency('nlohmann_json', required: true)
sdbusplus = meson.get_compiler('cpp').find_library('sdbusplus')
systemd = meson.get_compiler('cpp').find_library('systemd')


executable(meson.project_name(), 'main.cpp',
 dependencies: [boost, nlohmann_json, sdbusplus, systemd],
     implicit_include_directories: true,
    install: true,
    install_dir: get_option('bindir'),
)