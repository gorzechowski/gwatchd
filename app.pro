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

OTHER_FILES += \
    CHANGELOG.md \
    COPYING \
    README.md \
    .gitignore \
    .travis.yml \
    scripts/*

unix {
    core.path = $$OUT_PWD/bin
    core.files = $$OUT_PWD/core/gwatchd

    jobs.path = $$OUT_PWD/bin/jobs
    jobs.files = $$OUT_PWD/jobs/synchronize/lib*
    jobs.files += $$OUT_PWD/jobs/command/lib*

    tests.path = $$OUT_PWD/bin/tests
    tests.files = \
        $$OUT_PWD/tests/core/config/*Test \
        $$OUT_PWD/tests/core/notification/*Test \
        $$OUT_PWD/tests/core/watcher/*Test \
        $$OUT_PWD/tests/jobs/synchronize/*Test \
        $$OUT_PWD/tests/jobs/command/*Test

    INSTALLS += core jobs tests
}
