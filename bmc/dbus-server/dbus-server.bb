SUMMARY = "DBus Server Application"
DESCRIPTION = "DBus server application for OpenBMC"

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"
SRC_URI = "file://main.cpp \
           file://include \
           file://meson.build \
           file://dbus-server.service \
           file://COPYING.MIT"

S = "${WORKDIR}"

inherit meson systemd
inherit pkgconfig

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "dbus-server.service"
SYSTEMD_AUTO_ENABLE = "enable"

DEPENDS += "boost sdbusplus systemd nlohmann-json"
RDEPENDS_${PN} = "libgcc1 libstdc++6 libsystemd0"


FILES:${PN} += "${systemd_system_unitdir}"
SYSTEMD_SERVICE:${PN} = "dbus-server.service"
do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${B}/dbus-server ${D}${bindir}/dbus-server
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/dbus-server.service ${D}${systemd_system_unitdir}/dbus-server.service
    install -d ${D}/usr/share/dbus-server

}
