#version 400

// Attributes passed from the geometry shader
in vec4 frag_color;
in vec2 tex_coord;

// Uniform (global) buffer
//uniform sampler2D tex_samp;
uniform sampler2D texture_map;

void main (void)
{
    // Very simple fragment shader, but we can do anything we want here
    // We could apply a texture to the particle, illumination, etc.

	// Blends the texture gray scale color with the fragment color
	vec4 outval = texture(texture_map, tex_coord);
	//outval = vec4(outval.r*frag_color.r, outval.g*frag_color.g, outval.b*frag_color.b, 0.0);
    outval = vec4(outval.r*frag_color.r, outval.g*frag_color.g, outval.b*frag_color.b, sqrt(sqrt(outval.r))*frag_color.a);


    gl_FragColor = frag_color;
    //gl_FragColor = outval;
}
