#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform bool hdr;

uniform sampler2D bloomBlur;
uniform bool bloom;

uniform float exposure;
uniform float gamma = 1.0;

void main()
{             
    
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
	vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    if(hdr)
    {
        // reinhard
        // vec3 result = hdrColor / (hdrColor + vec3(1.0));
        
		if(bloom){
			hdrColor += bloomColor; // additive blending
		}
		// exposure
        //vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
		vec3 result = hdrColor / (hdrColor + exposure);
        // also gamma correct while we're at it       
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
    else
    {
		if(bloom){
			hdrColor += bloomColor; // additive blending
		}
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
}