attribute vec4 position;
attribute vec4 color;
attribute vec2 tcoord;

varying vec2 v_texCoord;
varying vec4 colorVarying;


uniform vec2 ScreenSize;
uniform	float scale;


void main()
{

    gl_Position = vec4( position.x, position.y, position.z, 1.0 );

    gl_Position.x =  (( position.x * ScreenSize.x ) - 1.0 ) * scale;
    gl_Position.y = (-( position.y * ScreenSize.y ) + 1.0 ) * scale;

    v_texCoord = tcoord;
    colorVarying = color;
    



	gl_FrontColor = colorVarying;
}