varying vec2 v_texCoord;
varying vec4 colorVarying;

uniform sampler2D Texture;

varying float pixel_offset;

void main()
{
	vec4 sum = vec4( 0.0 );

	/*sum += texture2D(Texture, vec2( v_texCoord.x - pixel_offset * 2.0, v_texCoord.y ) )	* 0.16;
	sum += texture2D(Texture, vec2( v_texCoord.x - pixel_offset, v_texCoord.y ) )		* 0.21;
    	sum += texture2D(Texture, v_texCoord )							* 0.25;
	sum += texture2D(Texture, vec2( v_texCoord.x + pixel_offset, v_texCoord.y ) )		* 0.21;
	sum += texture2D(Texture, vec2( v_texCoord.x + pixel_offset * 2.0, v_texCoord.y ) )	* 0.16;*/


	//sum  = texture2D(Texture, vec2( v_texCoord.x - pixel_offset * 8.0, v_texCoord.y ) )*0.0044299121055113265;
   	//sum += texture2D(Texture, vec2( v_texCoord.x - pixel_offset * 7.0, v_texCoord.y ) )*0.00895781211794;
   	sum  = texture2D(Texture, vec2( v_texCoord.x - pixel_offset * 6.0, v_texCoord.y ) )*0.0155963866053;
    	sum += texture2D(Texture, vec2( v_texCoord.x - pixel_offset * 5.0, v_texCoord.y ) )*0.0443683338718;
    	sum += texture2D(Texture, vec2( v_texCoord.x - pixel_offset * 4.0, v_texCoord.y ) )*0.0776744219933;
    	sum += texture2D(Texture, vec2( v_texCoord.x - pixel_offset * 3.0, v_texCoord.y ) )*0.115876621105;
    	sum += texture2D(Texture, vec2( v_texCoord.x - pixel_offset * 2.0, v_texCoord.y ) )*0.147308056121;
    	sum += texture2D(Texture, v_texCoord         )*0.159576912161;
    	sum += texture2D(Texture, vec2( v_texCoord.x + pixel_offset * 2.0, v_texCoord.y ) )*0.147308056121;
    	sum += texture2D(Texture, vec2( v_texCoord.x + pixel_offset * 3.0, v_texCoord.y ) )*0.115876621105;
    	sum += texture2D(Texture, vec2( v_texCoord.x + pixel_offset * 4.0, v_texCoord.y ) )*0.0776744219933;
    	sum += texture2D(Texture, vec2( v_texCoord.x + pixel_offset * 5.0, v_texCoord.y ) )*0.0443683338718;
    	sum += texture2D(Texture, vec2( v_texCoord.x + pixel_offset * 6.0, v_texCoord.y ) )*0.0155963866053;
    	//sum += texture2D(Texture, vec2( v_texCoord.x + pixel_offset * 7.0, v_texCoord.y ) )*0.00895781211794;
    	//sum += texture2D(Texture, vec2( v_texCoord.x + pixel_offset * 8.0, v_texCoord.y ) )*0.0044299121055113265;
    	
    	gl_FragColor = sum * colorVarying;
}