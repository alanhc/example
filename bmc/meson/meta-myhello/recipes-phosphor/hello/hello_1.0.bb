SUMMARY = "Simple Hello World service example"
DESCRIPTION = "Minimal C++ daemon that prints a message every 5 seconds."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING.MIT;md5=c1d032506b1cb1fc47cb10721de99ad9"

# No information for SRC_URI yet (only an external source tree was specified)
SRC_URI = "file://hello-1.0/main.cpp \
           file://hello-1.0/meson.build \
           file://hello-1.0/hello.service \
           file://hello-1.0/COPYING.MIT"
S = "${WORKDIR}/hello-1.0"
EXTRA_OEMESON_ARGS += "-Dprefix=/usr"
inherit systemd meson
SYSTEMD_SERVICE:${PN} = "hello.service"
# SYSTEMD_SERVICE:${PN} = "hello.service"

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
