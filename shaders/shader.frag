#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 ShadeColor;

uniform sampler2D texture_0;
uniform sampler2D texture_1;

uniform vec4 uPrimColor;
uniform vec4 uEnvColor;

uniform bool uIs2Cycle; 

uniform int uCC0_A, uCC0_B, uCC0_C, uCC0_D;
uniform int uAC0_A, uAC0_B, uAC0_C, uAC0_D;

uniform int uCC1_A, uCC1_B, uCC1_C, uCC1_D;
uniform int uAC1_A, uAC1_B, uAC1_C, uAC1_D;

vec3 get_color_source(int source, vec4 tex0, vec4 tex1, vec4 combined) {
    switch(source) {
        case 0:  return combined.rgb;       // G_CCMUX_COMBINED (Pass 1 result)
        case 1:  return tex0.rgb;           // G_CCMUX_TEXEL0
        case 2:  return tex1.rgb;           // G_CCMUX_TEXEL1
        case 3:  return uPrimColor.rgb;     // G_CCMUX_PRIMITIVE
        case 4:  return ShadeColor.rgb;     // G_CCMUX_SHADE
        case 5:  return uEnvColor.rgb;      // G_CCMUX_ENVIRONMENT
        case 6:  return vec3(1.0);          // G_CCMUX_1
        case 7:  return vec3(combined.a);   // G_CCMUX_COMBINED_ALPHA
        case 8:  return vec3(tex0.a);       // G_CCMUX_TEXEL0_ALPHA
        case 10: return vec3(uPrimColor.a); // G_CCMUX_PRIMITIVE_ALPHA
        case 11: return vec3(ShadeColor.a); // G_CCMUX_SHADE_ALPHA
        case 12: return vec3(uEnvColor.a);  // G_CCMUX_ENV_ALPHA
        case 15: return vec3(0.0);          // G_CCMUX_0
        default:
            return vec3(1.0, 0.0, 0.0);     // Unsupported source = bright red
    }
}
vec3 get_color_source_D(int source, vec4 tex0, vec4 tex1, vec4 combined) {
    switch(source) {
        case 0: return combined.rgb;
        case 1: return tex0.rgb;
        case 2: return tex1.rgb;
        case 3: return uPrimColor.rgb;
        case 4: return ShadeColor.rgb;
        case 5: return uEnvColor.rgb;
        case 6: return vec3(1.0); // G_CCMUX_1
        case 7: return vec3(0.0); // G_CCMUX_0
        default: return vec3(0.0);
    }
}

float get_alpha_source(int source, vec4 tex0, vec4 tex1, vec4 combined) {
    switch(source) {
        case 0:  return combined.a;         // G_ACMUX_COMBINED
        case 1:  return tex0.a;             // G_ACMUX_TEXEL0
        case 2:  return tex1.a;             // G_ACMUX_TEXEL1
        case 3:  return uPrimColor.a;       // G_ACMUX_PRIMITIVE
        case 4:  return ShadeColor.a;       // G_ACMUX_SHADE
        case 5:  return uEnvColor.a;        // G_ACMUX_ENVIRONMENT
        case 6:  return 1.0;                // G_ACMUX_1
        case 7:  return 0.0;                // G_ACMUX_0
        default:
            return 0.5;
    }
}


vec4 evaluate_combiner(int ccA, int ccB, int ccC, int ccD,
                       int acA, int acB, int acC, int acD,
                       vec4 tex0, vec4 tex1, vec4 combined) {
    vec3 A = get_color_source(ccA, tex0, tex1, combined);
    vec3 B = get_color_source(ccB, tex0, tex1, combined);
    vec3 C = get_color_source(ccC, tex0, tex1, combined);
    vec3 D = get_color_source_D(ccD, tex0, tex1, combined); // Use dedicated D lookup

    float aA = get_alpha_source(acA, tex0, tex1, combined);
    float aB = get_alpha_source(acB, tex0, tex1, combined);
    float aC = get_alpha_source(acC, tex0, tex1, combined);
    float aD = get_alpha_source(acD, tex0, tex1, combined);

    vec3 rgb = (A - B) * C + D;
    float a  = (aA - aB) * aC + aD;

    return vec4(rgb, a);
}

void main() {
    vec4 texel0 = texture(texture_0, TexCoord);
    vec4 texel1 = texture(texture_1, TexCoord);

    vec4 combined = vec4(0.0);

    combined = evaluate_combiner(uCC0_A, uCC0_B, uCC0_C, uCC0_D,
                                 uAC0_A, uAC0_B, uAC0_C, uAC0_D,
                                 texel0, texel1, combined);

    if (uIs2Cycle) {
        combined = evaluate_combiner(uCC1_A, uCC1_B, uCC1_C, uCC1_D,
                                     uAC1_A, uAC1_B, uAC1_C, uAC1_D,
                                     texel0, texel1, combined);
    }

    FragColor = clamp(combined, 0.0, 1.0);
}