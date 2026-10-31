# ASA Plugin Template
Simple template for ARK: Survival Ascended server plugins using [AsaApi](https://github.com/ArkServerApi/AsaApi).

RUN (DEPRECATED)
`git pull --recurse-submodules`
`git clone --recursive`

## 1st time build
- `vcpkg integrate install`
- VCPKG needs to update your hash baseline run this on vs studio dev cmd `vcpkg x-update-baseline [options] [--add-initial-baseline] [--dry-run]`


## SETUP

`<path> - depends on you`

- Create a repo using this template
- clone `<path>/plugin-name`
- clone AsaApi `https://github.com/ArkServerApi/AsaApi` to `<path>AsaApi`
- if you want the official plugins like `ArkShop` and `Permissions (mandatory)`
- clone it here `https://github.com/ArkServerApi/ASA-Plugins` to `<path>/ASA-Plugins`
- this repo is setup this way it will fetch automatically the library and includes you needed
- for 1st time clone please build both `AsaApi` and `ASA-Plugins` (release build)
- if you want to double check settings go to
  - Go to Project > <ProjectName> properties
  - VC++ Directories (includes Directories, Library Directories)
  - C/C++ > General (Additional Include Directories)
  - Linker > Input
 
## GUIDES
- Download and Install `ASA DevKit` on Epic Games (this will help you a lot if you need to look for certain functionality or Blueprints)
- AsaApi is not Updated if you found some missing functions run your dedicated server where AsaApi is installed and navigate to `ShooterGame\Binaries\Win64\ArkApi\Cache` open `cached_key.cache` for bitfields and `cached_offsets.txt` for functions
- `https://dumper.pelayori.com/` to get the Typename for example paste `UPrimalItem` and it will gives you the updated class (Note: you need to do some modification on some parts you'll know when you build it)
  

## DEBUGGING
- AsaApi Logs path `ShooterGame\Binaries\Win64\logs`
- Ark Logs path `ShooterGame\Saved\Logs` check for `crashcallstack.txt` sometimes it returns a line where you have errors


# Enjoy!



