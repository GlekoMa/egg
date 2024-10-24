# --- Set compiler flags ---
$CommonCompilerFlags = @("/nologo", "/W4")
$DebugFlags = @("/Od", "/Zi")
$ReleaseFlags = @("/O2")

#$CompilerFlags = $CommonCompilerFlags + $DebugFlags
$CompilerFlags = $CommonCompilerFlags + $ReleaseFlags

# --- Set links ---
$SystemLibs = @("user32.lib", "gdi32.lib", "shell32.lib", "dwmapi.lib", "comctl32.lib")

# --- Set build directory ---
$BuildDir = ".\build"
if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

# --- Set source files & output exe ---
$SourceFiles = @(
    "src\main.c",
    "src\dictionary\dictionary.c",
    "src\dictionary\initializer.c",
    "src\fuzzy_match\fuzzy_match.c",
    "src\window_procedure\window_procedure.c",
    "src\window_procedure\keyboard_mouse_procedure.c",
    "src\common\common.c",
    "build\resources.res"
)
$OutputExe = "$BuildDir\egg.exe"

# --- Produce resources ---
rc /nologo /fo build\resources.res src\resources.rc

if ($LASTEXITCODE -ne 0) {
    Write-Error "Resource compilation failed"
    exit 1
}

# --- Compile source ---
$CompilerFlags = $CompilerFlags -join ' '
$SourceFiles = $SourceFiles -join ' '
$SystemLibs = $SystemLibs -join ' '
$LinkerFlags = $LinkerFlags -join ' '

$CompileCommand = "cl $CompilerFlags $SourceFiles /Fe:$OutputExe /Fo:$BuildDir\ /Fd:$BuildDir\ /link $SystemLibs"
Invoke-Expression $CompileCommand

if ($LASTEXITCODE -ne 0) {
    Write-Error "Compilation failed"
    exit 1
}

Write-Host "Build completed successfully" -ForegroundColor Green
