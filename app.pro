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

TARGET_DIR = $$OUT_PWD/gwatchd-$${GWATCHD_VERSION}

unix {
    core.path = $$TARGET_DIR
    core.files = $$OUT_PWD/core/gwatchd

    jobs.path = $$TARGET_DIR/jobs
    jobs.files = $$OUT_PWD/jobs/synchronize/lib*
    jobs.files += $$OUT_PWD/jobs/command/lib*

    libs.path = $$TARGET_DIR/libs

    !macx {
        libs.files += $$[QT_INSTALL_LIBS]/libQt5Core.so*
        libs.files += $$[QT_INSTALL_LIBS]/libQt5Network.so*
        libs.files += $$[QT_INSTALL_LIBS]/libQt5WebSockets.so*
        libs.files += $$[QT_INSTALL_LIBS]/libicui18n.so*
        libs.files += $$[QT_INSTALL_LIBS]/libicuuc.so*
        libs.files += $$[QT_INSTALL_LIBS]/libicudata.so*
    } else {
        libs.files += $$[QT_INSTALL_LIBS]/QtCore.framework
        libs.files += $$[QT_INSTALL_LIBS]/QtNetwork.framework
        libs.files += $$[QT_INSTALL_LIBS]/QtWebSockets.framework
    }

    INSTALLS += core jobs libs
}

macx {
    install_name_tool.target = install_name_tool
    install_name_tool.commands = \
        install_name_tool -change @rpath/QtWebSockets.framework/Versions/5/QtWebSockets @executable_path/libs/QtWebSockets.framework/Versions/5/QtWebSockets $$TARGET_DIR/gwatchd && \
        install_name_tool -change @rpath/QtNetwork.framework/Versions/5/QtNetwork @executable_path/libs/QtNetwork.framework/Versions/5/QtNetwork $$TARGET_DIR/gwatchd && \
        install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @executable_path/libs/QtCore.framework/Versions/5/QtCore $$TARGET_DIR/gwatchd && \
        install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @executable_path/libs/QtCore.framework/Versions/5/QtCore $$TARGET_DIR/jobs/libcommandjob.dylib && \
        install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @executable_path/libs/QtCore.framework/Versions/5/QtCore $$TARGET_DIR/jobs/libsynchronizejob.dylib

    QMAKE_EXTRA_TARGETS += install_name_tool

    POST_TARGETDEPS += install_name_tool
}
