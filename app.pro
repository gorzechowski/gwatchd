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
SUBDIRS += libs core jobs tests

unix {
    core.path = $$OUT_PWD/bin
    core.files = $$OUT_PWD/core/gwatchd

    jobs.path = $$OUT_PWD/bin/jobs
    jobs.files = $$OUT_PWD/jobs/synchronize/*.so

    libs.path = $$OUT_PWD/bin/libs
    libs.files = $$OUT_PWD/libs/yaml-cpp/*.so

    INSTALLS += core jobs libs
}
