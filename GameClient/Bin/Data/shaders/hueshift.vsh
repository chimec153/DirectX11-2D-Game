attribute vec4 position;
attribute vec4 color;
attribute vec2 tcoord;

varying vec2 v_texCoord;
varying vec4 colorVarying;
varying vec3 vHSV;

uniform vec2 ScreenSize;
uniform vec3 HSV;
uniform	float scale;


void main()
{
	
    float xpos = position.x * scale;
    xpos = sign( xpos ) * floor( abs( xpos ) );
    float ypos = position.y * scale;
    ypos = sign( ypos ) * floor( abs( ypos ) );
    
	 gl_Position = vec4( xpos / scale, ypos / scale, position.z, 1.0 );
    
//gl_Position = vec4( trunc( position.x / scale ) * scale, trunc( position.y / scale ) * scale, position.z, 1.0 );	
//gl_Position = vec4( position.x, position.y, position.z, 1.0 );

    	gl_Position.x = ( ( gl_Position.x * ScreenSize.x ) - 1.0 ) * scale;
    	gl_Position.y = (-( gl_Position.y * ScreenSize.y ) + 1.0 ) * scale;

  

    	v_texCoord = tcoord;
    	colorVarying = color;
    	vHSV = HSV;

    	gl_FrontColor = colorVarying;
}