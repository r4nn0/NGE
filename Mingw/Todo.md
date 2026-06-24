### Add soundplaying to engine using OpenAL: https://github.com/kcat/openal-soft





### !IMPORTANT: Fix light for objects that has no material attached (Manual renderd objects / Polygons)

### Fix Skybox (Currently using obselete functions, use modern GL calls instead)

### Add primitive renderer + ploygon class to render any object of your choice

### Add tripple buffering (better performance)

### Add collision boxes (Both 2D and 3D)


### Add ImGUI support

### Avoid unnecessary buffer mapping / unmapping (Map only once and keep using the mapped storage) Implemented in text renderer (2D and 3D left) (Redo it)

### Fix so that there is only one renderer for both 2D and 3D (Ignore maybe later?)




### Add Vulkan support since only nvidia and amd gpus support bindless textures (Skip)
### Alternativaly add TextureArrays (3D arrays with fixed size, up to 2048 layers per texture array) (Skip)

### Add bindless textures (SSBO) (Done)
### Fix realistic graphics / lighting (With / without Vulkan) (Done)
### Custom font renderer (with shaders) (Done)
### Add modelMatrix for 2D objects (for easier rotaiton and scaling) (Done)
### Some models have model parts displaced, probably nodeMatrices issues (Fixed)
### Implement MaxRect packing (Done)






