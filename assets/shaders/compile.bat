@echo off
setlocal enabledelayedexpansion

for %%f in (*.glsl) do (
    set "filename=%%~nf"
    if "!filename!" == "!filename:_vert=!" (
        if "!filename!" == "!filename:_frag=!" (
            if "!filename!" == "!filename:_comp=!" (
                if "!filename!" == "!filename:_geom=!" (
                    if "!filename!" == "!filename:_tesc=!" (
                        if "!filename!" == "!filename:_tese=!" (
                            echo Unknown shader type for %%f
                        ) else (
                            glslc.exe -fshader-stage=tese -o %%~nf.spv %%f
                        )
                    ) else (
                        glslc.exe -fshader-stage=tesc -o %%~nf.spv %%f
                    )
                ) else (
                    glslc.exe -fshader-stage=geom -o %%~nf.spv %%f
                )
            ) else (
                glslc.exe -fshader-stage=comp -o %%~nf.spv %%f
            )
        ) else (
            glslc.exe -fshader-stage=frag -o %%~nf.spv %%f
        )
    ) else (
        glslc.exe -fshader-stage=vert -o %%~nf.spv %%f
    )
)

endlocal
echo Compilation complete.