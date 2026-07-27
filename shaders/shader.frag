#version 460 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 ShadeColor;

uniform sampler2D texture_0;
uniform sampler2D texture_1;

uniform vec4 uPrimColor;
uniform vec4 uEnvColor;
uniform vec4 uBlendColor;

uniform int uShiftS0;
uniform int uShiftT0;
uniform int uMaskS0;
uniform int uMaskT0;
uniform bool uMirrorS0;
uniform bool uMirrorT0;
uniform bool uClampS0;
uniform bool uClampT0;
uniform int uTileWidth0;
uniform int uTileHeight0;
uniform float uTexWidth0;
uniform float uTexHeight0;

uniform int uShiftS1;
uniform int uShiftT1;
uniform int uMaskS1;
uniform int uMaskT1;
uniform bool uMirrorS1;
uniform bool uMirrorT1;
uniform bool uClampS1;
uniform bool uClampT1;
uniform int uTileWidth1;
uniform int uTileHeight1;
uniform float uTexWidth1;
uniform float uTexHeight1;

uniform int uAlphaCompare;
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
        case 9:  return vec3(tex1.a);
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
            return 0;
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

float sample_coord(
    float coord,
    int shift,
    int mask,
    bool mirror,
    bool clampEnable,
    int tileSize)
{
    if (shift != 0) {
        if (shift <= 10)
            coord /= float(1 << shift);
        else
            coord *= float(1 << (16 - shift));
    }

    if (clampEnable || mask == 0)
    {
        float maxClamp = (mask != 0) ? float((1 << mask) - 1) : float(tileSize - 1);
        coord = clamp(coord, 0.0, maxClamp);
    }
    else 
    {
        float wrap = float(1 << mask);

        if (mirror)
        {
            float maxWrap = 2.0 * wrap;
            float m = mod(coord, maxWrap);
            if (m < 0.0) m += maxWrap; 

            if (m >= wrap)
                coord = 2.0 * wrap - 1.0 - floor(m);
            else
                coord = m;
        }
        else
        {
            coord = mod(coord, wrap);
            if (coord < 0.0) coord += wrap;
        }
    }

    return coord;
}


vec2 sample_uv(
    vec2 uv,

    int shiftS,
    int maskS,
    bool mirrorS,
    bool clampS,

    int shiftT,
    int maskT,
    bool mirrorT,
    bool clampT,

    int tileWidth,
    int tileHeight)
{
    return vec2(
        sample_coord(
            uv.x,
            shiftS,
            maskS,
            mirrorS,
            clampS,
            tileWidth),

        sample_coord(
            uv.y,
            shiftT,
            maskT,
            mirrorT,
            clampT,
            tileHeight)
    );
}

void main() {
    vec2 tc0 = sample_uv(
        TexCoord,
        uShiftS0,
        uMaskS0,
        uMirrorS0,
        uClampS0,
        uShiftT0,
        uMaskT0,
        uMirrorT0,
        uClampT0,
        uTileWidth0,
        uTileHeight0);

    vec2 tc1 = sample_uv(
        TexCoord,
        uShiftS1,
        uMaskS1,
        uMirrorS1,
        uClampS1,
        uShiftT1,
        uMaskT1,
        uMirrorT1,
        uClampT1,
        uTileWidth1,
        uTileHeight1);

    tc0 = (tc0 + vec2(0.5)) / vec2(uTexWidth0, uTexHeight0);
    vec4 texel0 = texture(texture_0, tc0);

    tc1 = (tc1 + vec2(0.5)) / vec2(uTexWidth1, uTexHeight1);
    vec4 texel1 = texture(texture_1, tc1);

    vec4 combined = vec4(0.0);

    if (uIs2Cycle) {
        combined = evaluate_combiner(uCC0_A, uCC0_B, uCC0_C, uCC0_D,
                                     uAC0_A, uAC0_B, uAC0_C, uAC0_D,
                                     texel0, texel1, combined);

        combined = evaluate_combiner(uCC1_A, uCC1_B, uCC1_C, uCC1_D,
                                     uAC1_A, uAC1_B, uAC1_C, uAC1_D,
                                     texel0, texel1, combined);
    } else {
        combined = evaluate_combiner(uCC1_A, uCC1_B, uCC1_C, uCC1_D,
                                     uAC1_A, uAC1_B, uAC1_C, uAC1_D,
                                     texel0, texel1, combined);
    }

    if (uAlphaCompare == 1) {
        if (combined.a <= uBlendColor.a ) {
            discard;
        }
    } else if (uAlphaCompare == 3) {
        if (combined.a < 0.5) {
            discard;
        }
    }
    if(combined.a < 0.03)discard;

    FragColor = clamp(combined, 0.0, 1.0);
}