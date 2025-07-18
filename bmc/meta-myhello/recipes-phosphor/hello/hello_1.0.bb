SUMMARY = "Simple Hello World service example"
DESCRIPTION = "Minimal C++ daemon that prints a message every 5 seconds."
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = "file://COPYING.MIT;md5=c1d032506b1cb1fc47cb10721de99ad9"


# No information for SRC_URI yet (only an external source tree was specified)
SRC_URI = "file://hello-1.0/main.cpp \
           file://hello-1.0/meson.build \
           file://hello-1.0/hello.service \
           file://hello-1.0/COPYING.MIT"
S = "${WORKDIR}/hello-1.0"

inherit systemd

# SYSTEMD_SERVICE:${PN} = "hello.service"

do_install:append() {
    # 安裝 service 檔
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${S}/hello.service \
        ${D}${systemd_system_unitdir}/
}

FILES:${PN} += "${systemd_system_unitdir}/hello.service"