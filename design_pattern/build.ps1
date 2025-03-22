param (
    [string]$t
)
chcp 65001
Write-Host "compile target : $t"
cmake -Dtarget::STRING=$t -S . -B build
cmake --build build --target $t


Write-Host "begin execute target : $t"
Write-Host "`n=============================================`n"
& bin/Debug/$t.exe