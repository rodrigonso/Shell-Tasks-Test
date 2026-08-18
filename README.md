This project is a simple packaged app used to test the new Tasks API exposed by the Windows SDK.

## Create a portable MSIX

From a Developer PowerShell prompt, run:

```powershell
.\package.ps1
```

The script builds the x64 Release configuration, creates a self-signed test certificate,
packages and signs the app, and writes a portable directory under `artifacts\msix`.
The Release build uses the static C++ runtime so the target VM does not need the Visual C++
Redistributable installed.

Copy the generated directory to the VM. From an **Administrator PowerShell** window,
run this inside it:

```powershell
powershell -ExecutionPolicy Bypass -File .\Install.ps1
```

`Install.ps1` trusts the included test certificate in the machine's `TrustedPeople` store
and installs or updates the MSIX. The VM must allow sideloaded apps and meet the manifest's
minimum Windows version (`10.0.26100.0`).

For local loose-layout development, `deploy.ps1` and `run.ps1` remain available.
