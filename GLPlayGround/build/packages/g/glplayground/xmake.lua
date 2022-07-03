package("glplayground")
    set_description("The glplayground package")

    on_load(function (package)
        package:set("installdir", path.join(os.scriptdir(), package:plat(), package:arch(), package:mode()))
    end)

    on_fetch(function (package)
        return {program = path.join(package:installdir("bin"), "")}
    end)
