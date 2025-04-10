# build.ps1 — Kompilacja projektu RP2040 z użyciem Ninja

$buildDir = "build"

Write-Host "🧹 Czyszczenie starego katalogu build..." -ForegroundColor Yellow
if (Test-Path $buildDir) {
    Remove-Item -Recurse -Force $buildDir
}

Write-Host "📁 Tworzenie katalogu build..." -ForegroundColor Cyan
New-Item -ItemType Directory -Name $buildDir | Out-Null
Set-Location $buildDir

Write-Host "🔧 Konfiguracja projektu przez CMake..." -ForegroundColor Cyan
cmake -G "Ninja" .. 
if ($LASTEXITCODE -ne 0) {
    Write-Host "❌ Błąd konfiguracji CMake!" -ForegroundColor Red
    exit 1
}

Write-Host "🔨 Budowanie projektu..." -ForegroundColor Green
cmake --build .
if ($LASTEXITCODE -ne 0) {
    Write-Host "❌ Błąd kompilacji!" -ForegroundColor Red
    exit 1
}

Write-Host "`n✅ Kompilacja zakończona pomyślnie!" -ForegroundColor Green
