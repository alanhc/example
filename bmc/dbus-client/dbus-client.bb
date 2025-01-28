SUMMARY = "DBus Server Application"
DESCRIPTION = "DBus server application for OpenBMC"

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"
SRC_URI = "file://dbus-client.cpp \
           file://include \
           file://meson.build \
           file://dbus-client.service \
           file://COPYING.MIT"

S = "${WORKDIR}"

inherit meson systemd
inherit pkgconfig
#inherit obmc-phosphor-systemd

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "dbus-client.service"
SYSTEMD_AUTO_ENABLE = "enable"

DEPENDS += "boost sdbusplus systemd nlohmann-json"
RDEPENDS_${PN} = "libgcc1 libstdc++6 libsystemd0"


FILES:${PN} += "${systemd_system_unitdir}"
SYSTEMD_SERVICE:${PN} = "dbus-client.service"
do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${B}/dbus-client ${D}${bindir}/dbus-client
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/dbus-client.service ${D}${systemd_system_unitdir}/dbus-client.service
    install -d ${D}/usr/share/dbus-client

}
# # 使用 Meson 进行编译
# do_compile () {
#     echo "WORKDIR: ${WORKDIR}"
#     echo "S: ${S}"

#     cd ${S}

#     meson setup  --verbose build
#     meson compile -C build
# }


# do_install () {
#     echo "install"
#     echo "WORKDIR: ${WORKDIR}"
#     echo "S: ${S}"
#     echo "D: ${D}"
    

#     cd ${S}
# 	DESTDIR=${D} meson install -C build  


# 	install -d ${D}${systemd_system_unitdir}
#     install -m 0644 ${S}/dbus-client.service ${D}${systemd_system_unitdir}/dbus-client.service
# }

