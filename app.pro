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
    jobs.files = $$OUT_PWD/jobs/synchronize/lib*

    libs.path = $$OUT_PWD/bin/libs
    libs.files = $$OUT_PWD/libs/yaml-cpp/lib*

    INSTALLS += core jobs libs
}

macx {
    install_name_tool.target = install_name_tool
    install_name_tool.commands = \
        install_name_tool -change libyaml-cpp.dylib @executable_path/libs/libyaml-cpp.dylib $$OUT_PWD/core/gwatchd && \
        install_name_tool -change libyaml-cpp.dylib @executable_path/libs/libyaml-cpp.dylib $$OUT_PWD/jobs/synchronize/lib*

    QMAKE_EXTRA_TARGETS += install_name_tool

    POST_TARGETDEPS += install_name_tool
}
