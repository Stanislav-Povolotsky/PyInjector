$project = "PyInjector/PyInjector.vcxproj"

$platforms = @("x64", "x86", "ARM64")
$configs = @("Release", "Release_SpawnShell")
$retargetPlatformToolset = "" 
# Replace with your desired PlatformToolset version
# $retargetPlatformToolset = "v143" 

if (Test-Path .git) {
    $BUILD_SOURCE = git config --get remote.origin.url
    $env:BUILD_SOURCE = "from " + $BUILD_SOURCE
}

# Setting PlatformToolset to required value
if (-not ([string]::IsNullOrEmpty($retargetPlatformToolset))) {
    $content = Get-Content -Path $project
    $updatedContent = $content -replace '<PlatformToolset>[^<]+</PlatformToolset>', "<PlatformToolset>$targetPlatformToolset</PlatformToolset>"
    Set-Content -Path $project -Value $updatedContent -Encoding UTF8
}

foreach ($platform in $platforms) {
    foreach ($config in $configs) {
        Write-Output "Building for Platform: $platform, Configuration: $config"
        msbuild $project /p:Configuration=$config /p:Platform=$platform /m
        if ($LASTEXITCODE -ne 0) {
            Write-Error "Build failed for Platform: $platform, Configuration: $config"
            exit 1
        }
    }
}

Write-Output "Build complete"