SUMMARY = "Simple Hello World service example"
DESCRIPTION = "Minimal C++ daemon that prints a message every 5 seconds."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

# No information for SRC_URI yet (only an external source tree was specified)
SRC_URI = "file://hello-1.0/main.cpp \
           file://hello-1.0/meson.build \
           file://hello-1.0/hello.service \
           file://hello-1.0/COPYING.MIT"
S = "${WORKDIR}/hello-1.0"
# EXTRA_OEMESON_ARGS += "-Dprefix=/usr"
# 使用 Yocto 環境傳入 prefix/bindir，並要 debug buildtype
EXTRA_OEMESON:append = " --buildtype debug --prefix=/usr"

# 友善除錯旗標（保留 frame pointer；O0 + g3）
CFLAGS:append   = " -O0 -g3 -fno-omit-frame-pointer"
CXXFLAGS:append = " -O0 -g3 -fno-omit-frame-pointer"

INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_PACKAGE_DEBUG_SPLIT = "0"

# 通常不必關閉 PIE；若你需要與非 PIE 的 gdb/addr 對齊才加：
# CFLAGS:append   = " -fno-PIE"
# CXXFLAGS:append = " -fno-PIE"
# LDFLAGS:append  = " -no-pie"

inherit systemd meson
SYSTEMD_SERVICE:${PN} = "hello.service"

do_install:append() {
    # 安裝 service 檔
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${S}/hello.service \
        ${D}${systemd_system_unitdir}/
    echo "== DEBUG: find built binary =="
    find ${WORKDIR} -type f -name hello || echo "== hello not found in WORKDIR =="
    
    echo "== DEBUG: find installed file =="
    find ${D} -type f -name hello || echo "== hello not installed in D =="
    
    echo "== DEBUG: list systemd unit dir =="
    ls -l ${D}${systemd_system_unitdir} || echo "== no systemd dir =="
}

FILES:${PN} += "${systemd_system_unitdir}/hello.service"
FILES:${PN} += "${bindir}/hello"
