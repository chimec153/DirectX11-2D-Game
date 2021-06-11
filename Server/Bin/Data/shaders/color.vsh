attribute vec4 position;
attribute vec4 color;

uniform vec2 ScreenSize;
uniform	float scale;

void main()
{

    gl_Position = vec4( position.x, position.y, position.z, 1.0 );

    gl_Position.x =  (( position.x * ScreenSize.x ) - 1.0 ) * scale;
    gl_Position.y = (-( position.y * ScreenSize.y ) + 1.0 ) * scale;

    gl_FrontColor = color;
}