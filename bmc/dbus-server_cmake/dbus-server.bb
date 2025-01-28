SUMMARY = "DBus Server Application"
DESCRIPTION = "DBus server application for OpenBMC"

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

inherit meson
inherit obmc-phosphor-systemd

DEPENDS += "pkgconfig meson ninja systemd"

S = "${WORKDIR}"

SRC_URI = "file://main.cpp \
           file://meson.build \
           file://dbus-server.service \
           file://COPYING.MIT"

FILES:${PN} += "${systemd_system_unitdir}"
SYSTEMD_SERVICE:${PN} = "dbus-server.service"

# 使用 Meson 進行編譯
do_compile () {
    echo "WORKDIR: ${WORKDIR}"
    echo "S: ${S}"

    cd ${S}

    meson setup build
    meson compile -C build
}


do_install () {
    echo "install"
    echo "WORKDIR: ${WORKDIR}"
    echo "S: ${S}"
    echo "D: ${D}"
    

    cd ${S}
	DESTDIR=${D} meson install -C build  


	install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${S}/dbus-server.service ${D}${systemd_system_unitdir}/dbus-server.service
}

