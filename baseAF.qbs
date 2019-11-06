import qbs

Project {
    name: "base"

    references: [
        "lib_baseAF/lib_baseAF.qbs",
        "lib_quickAF/lib_quickAF.qbs",
    ]
}
