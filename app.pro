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

!minQtVersion(5, 2, 0) {
    message("Current Qt version is $${QT_VERSION}")
    error("GWatchD requires at least Qt 5.2.0")
}

CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS += libs core jobs tests

unix {
    core.path = /usr/bin
    core.files = $$OUT_PWD/core/gwatchd

    jobs.path = /usr/lib/gwatchd/job
    jobs.files = $$OUT_PWD/jobs/synchronize/*.so

    libs.path = /usr/lib/gwatchd
    libs.files = $$OUT_PWD/libs/yaml-cpp/*.so

    INSTALLS += core jobs libs
}
