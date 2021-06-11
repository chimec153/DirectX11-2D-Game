varying vec2 v_texCoord;
varying vec4 colorVarying;

uniform float offset;
uniform float heatpower;
uniform sampler2D Texture;
uniform sampler2D NoiseTex;
uniform vec2 camOffset;
uniform vec4 ColorMulti;
uniform vec4 ColorAdd;

uniform vec4 TopCol;
uniform vec4 BotCol;

uniform vec2 fragSize;

void main()
{
    
	vec2 uv;
	uv.x = v_texCoord.x + ( offset ) / 256.0 + camOffset.x;
	uv.y = v_texCoord.y - ( offset ) / 256.0 - camOffset.y;
	vec4 distortColor = texture2D( NoiseTex, uv );
    
	uv.x = v_texCoord.x + ( ( distortColor.x - 0.5 ) / 50.0 ) * heatpower;
	uv.y = v_texCoord.y + ( ( distortColor.y - 0.5 ) / 70.0 ) * heatpower;
	
	uv.x = clamp( uv.x, 0.0, 1.0 );
	uv.y = clamp( uv.y, 0.0, 1.0 );


    	gl_FragColor = texture2D( Texture, uv ) * colorVarying;


	gl_FragColor.r = ( ColorMulti.r * ColorMulti.a ) + ( 1.0 - ColorMulti.a ) * gl_FragColor.r;
	gl_FragColor.g = ( ColorMulti.g * ColorMulti.a ) + ( 1.0 - ColorMulti.a ) * gl_FragColor.g;
	gl_FragColor.b = ( ColorMulti.b * ColorMulti.a ) + ( 1.0 - ColorMulti.a ) * gl_FragColor.b;

	float percent = gl_FragCoord.y / fragSize.y;
	float invPercent = 1.0 - percent;
	vec4 gradColor;
	
	float invColorMultiAlpha = 1.0 - ColorMulti.a;
	
	gradColor.r = ( TopCol.r * percent + BotCol.r * invPercent ) * invColorMultiAlpha;
	gradColor.g = ( TopCol.g * percent + BotCol.g * invPercent ) * invColorMultiAlpha;
	gradColor.b = ( TopCol.b * percent + BotCol.b * invPercent ) * invColorMultiAlpha;
	gradColor.a = ( TopCol.a * percent + BotCol.a * invPercent ) * invColorMultiAlpha;
	
	gl_FragColor.r = ( gradColor.r * gradColor.a ) + ( 1.0 - gradColor.a ) * gl_FragColor.r + (ColorAdd.r * ColorAdd.a);
	gl_FragColor.g = ( gradColor.g * gradColor.a ) + ( 1.0 - gradColor.a ) * gl_FragColor.g + (ColorAdd.g * ColorAdd.a);
	gl_FragColor.b = ( gradColor.b * gradColor.a ) + ( 1.0 - gradColor.a ) * gl_FragColor.b + (ColorAdd.b * ColorAdd.a);

}