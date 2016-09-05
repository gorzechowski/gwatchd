defineTest(minQtVersion) {
    major = $$1
    minor = $$2
    patch = $$3

    isEqual(QT_MAJOR_VERSION, $$major) {
        isEqual(QT_MINOR_VERSION, $$minor) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }

            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }

        greaterThan(QT_MINOR_VERSION, $$minor) {
            return(true)
        }
    }

    greaterThan(QT_MAJOR_VERSION, $$major) {
        return(true)
    }

    return(false)
}

!minQtVersion(5, 3, 0) {
    message("Current Qt version is $${QT_VERSION}")
    error("GWatchD requires at least Qt 5.3.0")
}

CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS += core jobs tests

include(app.pri)

OTHER_FILES += \
    CHANGELOG.md \
    COPYING \
    README.md \
    .gitignore \
    .travis.yml \
    scripts/* \
    docs/*

unix {
    core.path = $$OUT_PWD/gwatchd-$${GWATCHD_VERSION}
    core.files = $$OUT_PWD/core/gwatchd

    jobs.path = $$OUT_PWD/gwatchd-$${GWATCHD_VERSION}/jobs
    jobs.files = $$OUT_PWD/jobs/synchronize/lib*
    jobs.files += $$OUT_PWD/jobs/command/lib*

    libs.path = $$OUT_PWD/gwatchd-$${GWATCHD_VERSION}/libs
    libs.files += $$[QT_INSTALL_LIBS]/libQt5Core.so*
    libs.files += $$[QT_INSTALL_LIBS]/libQt5Network.so*
    libs.files += $$[QT_INSTALL_LIBS]/libQt5WebSockets.so*
    libs.files += $$[QT_INSTALL_LIBS]/libicui18n.so*
    libs.files += $$[QT_INSTALL_LIBS]/libicuuc.so*
    libs.files += $$[QT_INSTALL_LIBS]/libicudata.so*

    tests.path = $$OUT_PWD/gwatchd-$${GWATCHD_VERSION}/tests
    tests.files = \
        $$OUT_PWD/tests/core/config/*Test \
        $$OUT_PWD/tests/core/notification/*Test \
        $$OUT_PWD/tests/core/watcher/*Test \
        $$OUT_PWD/tests/jobs/synchronize/*Test \
        $$OUT_PWD/tests/jobs/command/*Test

    INSTALLS += core jobs libs tests
}
