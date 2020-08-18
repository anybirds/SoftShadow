# Soft Shadow
Various approaches to soft shadow implementation for android devices using GLES3. 
![alt text](https://github.com/anybirds/SoftShadow/blob/master/SoftShadow.jpg)

## Feature 
* Present 3 filtering algorithms: PCF, PCSS, VSSM
* Single directional light source
* Simple blinn-phong lighting model
* Shadow map size control option 
* Area light size control option
* PCF, PCSS with randomly rotated poisson disk sampling
* VSSM without kernel subdivision and HSM 

## Result
* PCSS with randomly rotated poisson disk sampling: The best option for mobile devices.
* PCSS is non-prefilterable but works nice without adding too much cost. 
* VSSM is very slow and has many artifacts: light bleeding, improper contact shadow, numeric imprecision.

## Install
* Download Android Studio. Version used for this project is 3.6.2.
* Download Android SDK: Tools->SDK Manager -> Download NDK(side by side).
* Set Android SDK location: File -> Project Structure -> SDK Location -> Android NDK Location.
* Clone this repository.
* Unzip assimp folder and place it to lib directory. 
* Build and run

## Reference
* Learnopengl https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
* PCSS https://developer.download.nvidia.com/shaderlibrary/docs/shadow_PCSS.pdf
* VSSM http://jankautz.com/publications/VSSM_PG2010.pdf
* NDK samples https://github.com/android/ndk-samples
