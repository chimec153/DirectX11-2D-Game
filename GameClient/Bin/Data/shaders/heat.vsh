attribute vec4 position;
attribute vec4 color;
attribute vec2 tcoord;

varying vec2 v_texCoord;
varying vec4 colorVarying;

uniform vec2 ScreenSize;

void main()
{
    	gl_Position.x = ( ( position.x * ScreenSize.x ) - 1.0 );
    	gl_Position.y = (-( position.y * ScreenSize.y ) + 1.0 );


    	v_texCoord = tcoord;
    	colorVarying = color;

    	gl_FrontColor = colorVarying;
}