SDF
===

Signed Distance Field rendering, based off of http://www.valvesoftware.com/publications/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf

The following images are rendered using a 1024x1024 signed distance field generated from a 4096x4096 source image (93.75% reduction in texture memory used, can be reduced further if only the alpha channel is used)

Rendering text using signed distance field technique
![output](https://raw.github.com/jkevin1/SDF/master/Renderer/output.png)

Rendering text with an outline
![output](https://raw.github.com/jkevin1/SDF/master/Renderer/outlined.png)

How the texture actually looks
![output](https://raw.github.com/jkevin1/SDF/master/Renderer/source.png)

What a 1024x1024 texture would look like without this technique
![output](https://raw.github.com/jkevin1/SDF/master/Renderer/equivalent.png)

Generator creates images like this using all available cores, this is the texture used in the above renderings, it was generated from Generator/test.png (a 4096x4096 image)
![output](https://raw.github.com/jkevin1/SDF/master/Renderer/font.png)
