Assignment 3

List of Pixel Shaders:
1) shiny_blue_fp.glsl, shiny_blue_vp.glsl
2) shiny_texture_fp.glsl, shiny_texture_vp.glsl
3) toon_fp.glsl, toon_vp.glsl

Pixel Shader 1 does not take texture into account
Pixel Shader 2 takes texture into account
Pixel Shader 3 is the toon shader

When Toon shading is disabled:
Pixel Shader 1 is used for the torus
Pixel Shader 2 is used on the helicopter model (cube and cylinder)

When Toon shading is enabled:
Pixel Shader 3 gets used on the torus and helicopter instead

To toogle Toon shading:
Press and Release the "C" key
