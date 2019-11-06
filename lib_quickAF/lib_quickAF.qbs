import qbs
import "../../qbs/lib_static.qbs" as LibStaticT

LibStaticT {
    libraryName: "quickAF"

     Depends { name: "Qt.qml" }
     Depends { name: "Qt.quick" }
     Depends { name: "lib_baseAF" }
}
