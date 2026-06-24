### Add soundplaying to engine using OpenAL: https://github.com/kcat/openal-soft


### Add 2D camera
### Fix so that there is only one renderer for sprites and fonts
### OR Implement rendering to framebuffer (created texture) instead of rendering to the main render target immediately
### Add primitive renderer + ploygon class to render any object of your choice



### !IMPORTANT: Fix light for objects that has no material attached (Manual renderd objects / Polygons)

### Fix Skybox (Currently using obselete functions, use modern GL calls instead)



### Add tripple buffering (better performance)

### Add collision boxes (Both 2D and 3D)


### Add ImGUI support

### Avoid unnecessary buffer mapping / unmapping (Map only once and keep using the mapped storage) Implemented in text renderer (2D and 3D left) (Redo it)






### Add Vulkan support since only nvidia and amd gpus support bindless textures (Skip)
### Alternativaly add TextureArrays (3D arrays with fixed size, up to 2048 layers per texture array) (Skip)

### Add bindless textures (SSBO) (Done)
### Fix realistic graphics / lighting (With / without Vulkan) (Done)
### Custom font renderer (with shaders) (Done)
### Add modelMatrix for 2D objects (for easier rotaiton and scaling) (Done)
### Some models have model parts displaced, probably nodeMatrices issues (Fixed)
### Implement MaxRect packing (Done)






