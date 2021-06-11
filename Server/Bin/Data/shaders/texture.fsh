varying vec2 v_texCoord;
varying vec4 colorVarying;

uniform sampler2D Texture;

void main()
{
    	gl_FragColor = colorVarying * texture2D(Texture, v_texCoord);

}