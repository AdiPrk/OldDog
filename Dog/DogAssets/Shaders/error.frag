#version 430 core
#extension GL_ARB_bindless_texture : require

layout (std140) uniform Resolution
{
	vec2 iResolution;
};

layout (std140) uniform Time
{
    float iTime;
};

in vec2 TexCoords;
out vec4 fragColor;

uniform sampler2DArray textureHandle;
uniform unsigned int spriteIndex;
uniform vec4 spriteColor;
uniform vec2 repetition;

float impulse(float k, float x)
{
    float h = k*x;
    return h*exp(1.0-h);
}

// iq
vec3 palette( in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d )
{
    return a + b*cos( 6.28318*(c*t+d) );
}

// iq
float uSDF(float s1, float s2)
{
    return min(s1, s2);
}

// iq
float dSDF(float s1, float s2)
{
    return max(s1, -s2);
}

// iq
float suSDF( float d1, float d2, float k ) {
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h); }


// iq
float sdPlane(vec3 pos, float offset)
{
    return pos.y+offset;
}

// iq
float sdSphere(vec3 pos, float r)
{
    
    return length(pos)-r;
}

// iq
float sdCapsule( vec3 p, vec3 a, vec3 b, float r )
{
    vec3 pa = p - a, ba = b - a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h ) - r;
}

// iq ? yes
float sdRoundCone( in vec3 p, in float r1, float r2, float h )
{
    vec2 q = vec2( length(p.xz), p.y );
    
    float b = (r1-r2)/h;
    float a = sqrt(1.0-b*b);
    float k = dot(q,vec2(-b,a));
    
    if( k < 0.0 ) return length(q) - r1;
    if( k > a*h ) return length(q-vec2(0.0,h)) - r2;
        
    return dot(q, vec2(a,b) ) - r1;
}

float map(vec3 pos)
{
    vec3 tp = pos;
    float t = 10000.0;
    
    // body
    float bodyY = -1.0;
    bodyY += impulse(5.0, fract(iTime)) * .5;
    tp = pos + vec3(0.0, bodyY, 0.0);
    t = sdCapsule(tp, vec3(1., .0, .0), vec3(0.0), 1.0);
    
    // sexy legs
    float legY = 1.0;
    tp = pos + vec3(0.5, legY, 1.0);
    t = suSDF(t, sdRoundCone(tp, .1, .5, 1.), .5);
    tp = pos + vec3(-1.5, legY, 1.0);
    t = suSDF(t, sdRoundCone(tp, .1, .5, 1.), .5);
    tp = pos + vec3(0.5, legY, -1.0);
    t = suSDF(t, sdRoundCone(tp, .1, .5, 1.), .5);
    tp = pos + vec3(-1.5, legY, -1.0);
    t = suSDF(t, sdRoundCone(tp, .1, .5, 1.), .5);
    
    // la tete de ta mere
    tp = pos + vec3(1.5, bodyY+-.5, 0.0);
    t = suSDF(t, sdSphere(tp, 1.5), .2);
    tp = pos + vec3(1.5, bodyY+-2.0, 0.5);
    t = suSDF(t, sdRoundCone(tp, .3, .1, 1.), .5);
    tp = pos + vec3(1.5, bodyY+-2.0, -0.5);
    t = suSDF(t, sdRoundCone(tp, .3, .1, 1.), .5);
    tp = pos + vec3(2.5, bodyY+-.2, 0.0);
    t = dSDF(t, sdSphere(tp, .6));
    tp = pos + vec3(2.8, bodyY+-.5, 0.0);
    t = uSDF(t, sdSphere(tp, .5));
    // eyes
    tp = pos + vec3(2.7, bodyY+-1.0, .5);
    t = uSDF(t, sdSphere(tp, .25));
    tp = pos + vec3(2.9, bodyY+-1.0, .5);
    t = dSDF(t, sdSphere(tp, .1));
    tp = pos + vec3(2.7, bodyY+-1.0, -.5);
    t = uSDF(t, sdSphere(tp, .25));
    tp = pos + vec3(2.9, bodyY+-1.0, -.5);
    t = dSDF(t, sdSphere(tp, .1));
    
    // balls of steel
    tp = pos + vec3(-2.0, bodyY+.5, 0.15);
    //t = uSDF(t, sdSphere(tp, .2));
    tp = pos + vec3(-2.0, bodyY+.5, -0.15);
    //t = uSDF(t, sdSphere(tp, .2));
    tp = pos + vec3(-2., bodyY+.5, 0.0);
    //t = uSDF(t, sdCapsule(tp, vec3(0.0, -.5, sin(iTime*10.)*.2), vec3(.0), .15));
    
    t = uSDF(t, sdPlane(pos, 1.0));
    
    return t;
}

float CastRay(vec3 rayOrigin, vec3 rayDir)
{
    float t;
    
    for(int i=0; i<128; i++)
    {
        float res = map(rayOrigin + rayDir * t);
        
        if(res < (0.0001*t))
        {
            return t;
        }
        
        t += res;
    }
    
    return -1.0;
}

vec3 calcNormal(in vec3 pos)
{
    vec2 e = vec2(1.0, -1.0) * 0.5773 * 0.0005;
    return normalize(   e.xyy*map(  pos+e.xyy   )   +
                        e.yyx*map(  pos+e.yyx   )   +
                        e.yxy*map(  pos+e.yxy   )   +
                        e.xxx*map(  pos+e.xxx   ));
}

float shadow( in vec3 ro, in vec3 rd, float mint, float maxt )
{
    for( float t=mint; t<maxt; )
    {
        float h = map(ro + rd*t);
        if( h<0.001 )
            return 0.0;
        t += h;
    }
    return 1.0;
}

vec3 render(vec3 ro, vec3 rd)
{
    float t = CastRay(ro, rd);
    
    vec3 col = vec3(0.0);
    
    if(t == -1.0)
    {
        col = vec3(0.0);
    }
    else
    {
        vec3 pos=ro+rd*t;
        vec3 N = calcNormal(pos);
        vec3 L = vec3(-1.0, 1.0, 1.0);
        
        float light = clamp(dot(N, L), 0.0, 1.0);
        vec3 spec = vec3(pow(clamp(dot(N, L), 0.0, 1.0),8.0));
        float fog = 0.2 * t; // is it ?
        
        float shad = shadow(pos, L, 0.02, 2.5);
        
        col = palette(
            light * fog, 
            vec3(.5),
            vec3(.5),
            vec3(.12),
            vec3(.5, .2, .9));
        
        col = mix(col*.33, col, shad);
    }
    
    return col;
}

vec3 CameraViewDir(vec2 uv, vec3 camPos, vec3 camTar)
{
    vec3 viewDir;
    vec3 forward = normalize(camTar - camPos);
    vec3 right = normalize(cross(vec3(.0, -1., .0), forward));
    vec3 up = normalize(cross(right, forward));
    float fPersp = 1.0;
    
    viewDir = normalize(uv.x * right + uv.y * up + fPersp * forward);
    
    return viewDir;
}

#define DOWN_SCALE 1.0

#define MAX_INT_DIGITS 4

#define CHAR_SIZE vec2(8, 12)
#define CHAR_SPACING vec2(8, 12)

#define STRWIDTH(c) (c * CHAR_SPACING.x)
#define STRHEIGHT(c) (c * CHAR_SPACING.y)

#define NORMAL 0
#define INVERT 1
#define UNDERLINE 2

int TEXT_MODE = NORMAL;

vec4 ch_spc = vec4(0x000000,0x000000,0x000000,0x000000);
vec4 ch_exc = vec4(0x003078,0x787830,0x300030,0x300000);
vec4 ch_quo = vec4(0x006666,0x662400,0x000000,0x000000);
vec4 ch_hsh = vec4(0x006C6C,0xFE6C6C,0x6CFE6C,0x6C0000);
vec4 ch_dol = vec4(0x30307C,0xC0C078,0x0C0CF8,0x303000);
vec4 ch_pct = vec4(0x000000,0xC4CC18,0x3060CC,0x8C0000);
vec4 ch_amp = vec4(0x0070D8,0xD870FA,0xDECCDC,0x760000);
vec4 ch_apo = vec4(0x003030,0x306000,0x000000,0x000000);
vec4 ch_lbr = vec4(0x000C18,0x306060,0x603018,0x0C0000);
vec4 ch_rbr = vec4(0x006030,0x180C0C,0x0C1830,0x600000);
vec4 ch_ast = vec4(0x000000,0x663CFF,0x3C6600,0x000000);
vec4 ch_crs = vec4(0x000000,0x18187E,0x181800,0x000000);
vec4 ch_com = vec4(0x000000,0x000000,0x000038,0x386000);
vec4 ch_dsh = vec4(0x000000,0x0000FE,0x000000,0x000000);
vec4 ch_per = vec4(0x000000,0x000000,0x000038,0x380000);
vec4 ch_lsl = vec4(0x000002,0x060C18,0x3060C0,0x800000);
vec4 ch_0 = vec4(0x007CC6,0xD6D6D6,0xD6D6C6,0x7C0000);
vec4 ch_1 = vec4(0x001030,0xF03030,0x303030,0xFC0000);
vec4 ch_2 = vec4(0x0078CC,0xCC0C18,0x3060CC,0xFC0000);
vec4 ch_3 = vec4(0x0078CC,0x0C0C38,0x0C0CCC,0x780000);
vec4 ch_4 = vec4(0x000C1C,0x3C6CCC,0xFE0C0C,0x1E0000);
vec4 ch_5 = vec4(0x00FCC0,0xC0C0F8,0x0C0CCC,0x780000);
vec4 ch_6 = vec4(0x003860,0xC0C0F8,0xCCCCCC,0x780000);
vec4 ch_7 = vec4(0x00FEC6,0xC6060C,0x183030,0x300000);
vec4 ch_8 = vec4(0x0078CC,0xCCEC78,0xDCCCCC,0x780000);
vec4 ch_9 = vec4(0x0078CC,0xCCCC7C,0x181830,0x700000);
vec4 ch_col = vec4(0x000000,0x383800,0x003838,0x000000);
vec4 ch_scl = vec4(0x000000,0x383800,0x003838,0x183000);
vec4 ch_les = vec4(0x000C18,0x3060C0,0x603018,0x0C0000);
vec4 ch_equ = vec4(0x000000,0x007E00,0x7E0000,0x000000);
vec4 ch_grt = vec4(0x006030,0x180C06,0x0C1830,0x600000);
vec4 ch_que = vec4(0x0078CC,0x0C1830,0x300030,0x300000);
vec4 ch_ats = vec4(0x007CC6,0xC6DEDE,0xDEC0C0,0x7C0000);
vec4 ch_A = vec4(0x003078,0xCCCCCC,0xFCCCCC,0xCC0000);
vec4 ch_B = vec4(0x00FC66,0x66667C,0x666666,0xFC0000);
vec4 ch_C = vec4(0x003C66,0xC6C0C0,0xC0C666,0x3C0000);
vec4 ch_D = vec4(0x00F86C,0x666666,0x66666C,0xF80000);
vec4 ch_E = vec4(0x00FE62,0x60647C,0x646062,0xFE0000);
vec4 ch_F = vec4(0x00FE66,0x62647C,0x646060,0xF00000);
vec4 ch_G = vec4(0x003C66,0xC6C0C0,0xCEC666,0x3E0000);
vec4 ch_H = vec4(0x00CCCC,0xCCCCFC,0xCCCCCC,0xCC0000);
vec4 ch_I = vec4(0x007830,0x303030,0x303030,0x780000);
vec4 ch_J = vec4(0x001E0C,0x0C0C0C,0xCCCCCC,0x780000);
vec4 ch_K = vec4(0x00E666,0x6C6C78,0x6C6C66,0xE60000);
vec4 ch_L = vec4(0x00F060,0x606060,0x626666,0xFE0000);
vec4 ch_M = vec4(0x00C6EE,0xFEFED6,0xC6C6C6,0xC60000);
vec4 ch_N = vec4(0x00C6C6,0xE6F6FE,0xDECEC6,0xC60000);
vec4 ch_O = vec4(0x00386C,0xC6C6C6,0xC6C66C,0x380000);
vec4 ch_P = vec4(0x00FC66,0x66667C,0x606060,0xF00000);
vec4 ch_Q = vec4(0x00386C,0xC6C6C6,0xCEDE7C,0x0C1E00);
vec4 ch_R = vec4(0x00FC66,0x66667C,0x6C6666,0xE60000);
vec4 ch_S = vec4(0x0078CC,0xCCC070,0x18CCCC,0x780000);
vec4 ch_T = vec4(0x00FCB4,0x303030,0x303030,0x780000);
vec4 ch_U = vec4(0x00CCCC,0xCCCCCC,0xCCCCCC,0x780000);
vec4 ch_V = vec4(0x00CCCC,0xCCCCCC,0xCCCC78,0x300000);
vec4 ch_W = vec4(0x00C6C6,0xC6C6D6,0xD66C6C,0x6C0000);
vec4 ch_X = vec4(0x00CCCC,0xCC7830,0x78CCCC,0xCC0000);
vec4 ch_Y = vec4(0x00CCCC,0xCCCC78,0x303030,0x780000);
vec4 ch_Z = vec4(0x00FECE,0x981830,0x6062C6,0xFE0000);
vec4 ch_lsb = vec4(0x003C30,0x303030,0x303030,0x3C0000);
vec4 ch_rsl = vec4(0x000080,0xC06030,0x180C06,0x020000);
vec4 ch_rsb = vec4(0x003C0C,0x0C0C0C,0x0C0C0C,0x3C0000);
vec4 ch_pow = vec4(0x10386C,0xC60000,0x000000,0x000000);
vec4 ch_usc = vec4(0x000000,0x000000,0x000000,0x00FF00);
vec4 ch_a = vec4(0x000000,0x00780C,0x7CCCCC,0x760000);
vec4 ch_b = vec4(0x00E060,0x607C66,0x666666,0xDC0000);
vec4 ch_c = vec4(0x000000,0x0078CC,0xC0C0CC,0x780000);
vec4 ch_d = vec4(0x001C0C,0x0C7CCC,0xCCCCCC,0x760000);
vec4 ch_e = vec4(0x000000,0x0078CC,0xFCC0CC,0x780000);
vec4 ch_f = vec4(0x00386C,0x6060F8,0x606060,0xF00000);
vec4 ch_g = vec4(0x000000,0x0076CC,0xCCCC7C,0x0CCC78);
vec4 ch_h = vec4(0x00E060,0x606C76,0x666666,0xE60000);
vec4 ch_i = vec4(0x001818,0x007818,0x181818,0x7E0000);
vec4 ch_j = vec4(0x000C0C,0x003C0C,0x0C0C0C,0xCCCC78);
vec4 ch_k = vec4(0x00E060,0x60666C,0x786C66,0xE60000);
vec4 ch_l = vec4(0x007818,0x181818,0x181818,0x7E0000);
vec4 ch_m = vec4(0x000000,0x00FCD6,0xD6D6D6,0xC60000);
vec4 ch_n = vec4(0x000000,0x00F8CC,0xCCCCCC,0xCC0000);
vec4 ch_o = vec4(0x000000,0x0078CC,0xCCCCCC,0x780000);
vec4 ch_p = vec4(0x000000,0x00DC66,0x666666,0x7C60F0);
vec4 ch_q = vec4(0x000000,0x0076CC,0xCCCCCC,0x7C0C1E);
vec4 ch_r = vec4(0x000000,0x00EC6E,0x766060,0xF00000);
vec4 ch_s = vec4(0x000000,0x0078CC,0x6018CC,0x780000);
vec4 ch_t = vec4(0x000020,0x60FC60,0x60606C,0x380000);
vec4 ch_u = vec4(0x000000,0x00CCCC,0xCCCCCC,0x760000);
vec4 ch_v = vec4(0x000000,0x00CCCC,0xCCCC78,0x300000);
vec4 ch_w = vec4(0x000000,0x00C6C6,0xD6D66C,0x6C0000);
vec4 ch_x = vec4(0x000000,0x00C66C,0x38386C,0xC60000);
vec4 ch_y = vec4(0x000000,0x006666,0x66663C,0x0C18F0);
vec4 ch_z = vec4(0x000000,0x00FC8C,0x1860C4,0xFC0000);
vec4 ch_lpa = vec4(0x001C30,0x3060C0,0x603030,0x1C0000);
vec4 ch_bar = vec4(0x001818,0x181800,0x181818,0x180000);
vec4 ch_rpa = vec4(0x00E030,0x30180C,0x183030,0xE00000);
vec4 ch_tid = vec4(0x0073DA,0xCE0000,0x000000,0x000000);
vec4 ch_lar = vec4(0x000000,0x10386C,0xC6C6FE,0x000000);

vec2 resol = vec2(0);
vec2 print_pos = vec2(0);

//Extracts bit b from the given number.
//Shifts bits right (num / 2^bit) then ANDs the result with 1 (mod(result,2.0)).
float extract_bit(float n, float b)
{
    b = clamp(b,-1.0,24.0);
	return floor(mod(floor(n / pow(2.0,floor(b))),2.0));   
}

//Returns the pixel at uv in the given bit-packed sprite.
float sprite(vec4 spr, vec2 size, vec2 uv)
{
    uv = floor(uv);
    
    //Calculate the bit to extract (x + y * width) (flipped on x-axis)
    float bit = (size.x-uv.x-1.0) + uv.y * size.x;
    
    //Clipping bound to remove garbage outside the sprite's boundaries.
    bool bounds = all(greaterThanEqual(uv,vec2(0))) && all(lessThan(uv,size));
    
    float pixels = 0.0;
    pixels += extract_bit(spr.x, bit - 72.0);
    pixels += extract_bit(spr.y, bit - 48.0);
    pixels += extract_bit(spr.z, bit - 24.0);
    pixels += extract_bit(spr.w, bit - 00.0);
    
    return bounds ? pixels : 0.0;
}

//Prints a character and moves the print position forward by 1 character width.
float drawchar(vec4 ch, vec2 uv)
{
    if( TEXT_MODE == INVERT )
    {
      //Inverts all of the bits in the character.
      ch = pow(2.0,24.0)-1.0-ch;
    }
    if( TEXT_MODE == UNDERLINE )
    {
      //Makes the bottom 8 bits all 1.
      //Shifts the bottom chunk right 8 bits to drop the lowest 8 bits,
      //then shifts it left 8 bits and adds 255 (binary 11111111).
      ch.w = floor(ch.w/256.0)*256.0 + 255.0;  
    }

    float px = sprite(ch, CHAR_SIZE, uv - print_pos);
    print_pos.x += CHAR_SPACING.x;
    return px;
}


//Returns the digit sprite for the given number.
vec4 get_digit(float d)
{
    d = floor(d);
    
    if(d == 0.0) return ch_0;
    if(d == 1.0) return ch_1;
    if(d == 2.0) return ch_2;
    if(d == 3.0) return ch_3;
    if(d == 4.0) return ch_4;
    if(d == 5.0) return ch_5;
    if(d == 6.0) return ch_6;
    if(d == 7.0) return ch_7;
    if(d == 8.0) return ch_8;
    if(d == 9.0) return ch_9;
    return vec4(0.0);
}

//Prints out the given number starting at pos.
float print_number(float number, vec2 uv)
{
    float result = 0.0;
    
	for(int i = 3;i >= -1;i--)
    {
        float digit = mod( number / pow(10.0, float(i)) , 10.0);
        
        if(i == -1) //Add a decimal point.
        {
            result += drawchar(ch_per,uv);
        }
        
        if(abs(number) > pow(10.0, float(i)) || i == 0) //Clip off leading zeros.
        {
        	result += drawchar(get_digit(digit),uv);
        }
    }   
    return result;
}

float print_integer(float number, int zeros, vec2 uv)
{
    float result = 0.0;
    
	for(int i = MAX_INT_DIGITS;i >= 0;i--)
    {
        float digit = mod( number / pow(10.0, float(i)) , 10.0);

        if(abs(number) > pow(10.0, float(i)) || zeros > i  || i == 0) //Clip off leading zeros.
        {
        	result += drawchar(get_digit(digit),uv);
        }
    }   
    return result;
}

float text(vec2 uv)
{
    float col = 0.0;
    
    vec2 center = resol/2.0;
    
    //Greeting Text
    
    print_pos = floor(center - vec2(STRWIDTH(17.0),STRHEIGHT(1.0))/2.0);
       
    col += drawchar(ch_S,uv);
    col += drawchar(ch_h,uv);
    col += drawchar(ch_a,uv);
    col += drawchar(ch_d,uv);
    col += drawchar(ch_e,uv);
    col += drawchar(ch_r,uv);    
    col += drawchar(ch_spc,uv);
    col += drawchar(ch_E,uv);
    col += drawchar(ch_r,uv);
    col += drawchar(ch_r,uv);
    col += drawchar(ch_o,uv);
    col += drawchar(ch_r,uv);
    col += drawchar(ch_exc,uv);
    
    return col;
}

void main()
{    
    vec3 col = vec3(.0);
    float time = iTime * 2.;
    vec3 camPos = vec3(sin(time), .2+sin(iTime)*.2, cos(time))*5.0;
    vec3 camTarget = vec3(0.0, 0.5, 0.0);
    vec2 uv = (TexCoords - 0.5) * 2.;
    //uv.y += 0.3;
    uv.y = -uv.y + 0.3;
    
    vec3 viewDir = CameraViewDir(uv, camPos, camTarget);
    
    col = render(camPos, viewDir); 
    
    fragColor = vec4(col, 1.0);
    fragColor.r *= 20.0;

    /* Text */
    
    vec2 fragCoord = (TexCoords) * 110.;
    fragCoord.y *= 1;
    fragCoord.y += 100;
    fragCoord.x += 158.;
    resol = vec2(iResolution.xy) / DOWN_SCALE;
    uv = fragCoord / DOWN_SCALE;
	vec2 duv = floor(fragCoord / DOWN_SCALE);
    
	float pixel = text(duv);
    
    //Shading stuff
    col = vec3(1);
    col *= (1.-distance(mod(uv,vec2(1.0)),vec2(0.65)))*1.2;
    col *= mix(vec3(0.2),vec3(0,1,0),pixel);
    fragColor += vec4(vec3(col), 1.0);
}



