varying vec2 v_texCoord;
varying vec4 colorVarying;

uniform sampler2D Texture;

void main()
{
	gl_FragColor = colorVarying * texture2D(Texture, v_texCoord);
    	gl_FragColor.a = clamp( gl_FragColor.a, 0.0, 1.0 );
}