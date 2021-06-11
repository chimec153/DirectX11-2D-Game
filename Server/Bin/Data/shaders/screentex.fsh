varying vec2 v_texCoord;	//	2차원 텍스쳐 좌표를 나타내는 벡터를 선언한다.
varying vec4 colorVarying;	//	4차원 색상을 나타내는 벡터를 선언한다.

uniform sampler2D Texture;	//	2차원 셈플러 변수를 선언한다.
uniform vec4 ColorMulti;	//	4차원 멀티 색을 나타내는 벡터를 선언한다.
uniform vec4 ColorAdd;	//	4차원 색 추가 변수를 선언한다.

uniform vec4 TopCol;	//	4차원 상단 충돌을 나타내는 벡터를 선언한다.
uniform vec4 BotCol;	//	4차원 하단 충돌을 나타내는 벡터를 선언한다.

uniform vec2 fragSize;	//	2차원 픽셀 크기를 나타내는 벡터를 선언한다.

void main()	//	픽셀셰이더의 메인 함수를 정의한다.
{
	float invColorMultiAlpha = 1.0 - ColorMulti.a;	//	float 변수를 선언하고 1에서 멀티 색의 알파값을 뺀값으로 설정한다.
    	gl_FragColor = colorVarying * texture2D(Texture, v_texCoord);	//	픽셀 색을 색상과 2차원 샘플러와 텍스쳐 좌표로 샘플링한 값을 곱한 값으로 설정한다.

	gl_FragColor.r = ( ColorMulti.r * ColorMulti.a ) + ( invColorMultiAlpha ) * gl_FragColor.r;	//	픽셀 색의 r 성분을 멀티 색의 r성분과 멀티 색의 a성분을 곱한것과 
	gl_FragColor.g = ( ColorMulti.g * ColorMulti.a ) + ( invColorMultiAlpha ) * gl_FragColor.g;
	gl_FragColor.b = ( ColorMulti.b * ColorMulti.a ) + ( invColorMultiAlpha ) * gl_FragColor.b;

	float percent = gl_FragCoord.y / fragSize.y;
	float invPercent = 1.0 - percent;
	vec4 gradColor;
	
	
	gradColor.r = ( TopCol.r * percent + BotCol.r * invPercent ) * invColorMultiAlpha;
	gradColor.g = ( TopCol.g * percent + BotCol.g * invPercent ) * invColorMultiAlpha;
	gradColor.b = ( TopCol.b * percent + BotCol.b * invPercent ) * invColorMultiAlpha;
	gradColor.a = ( TopCol.a * percent + BotCol.a * invPercent ) * invColorMultiAlpha;
	
	gl_FragColor.r = ( gradColor.r * gradColor.a ) + ( 1.0 - gradColor.a ) * gl_FragColor.r + (ColorAdd.r * ColorAdd.a);
	gl_FragColor.g = ( gradColor.g * gradColor.a ) + ( 1.0 - gradColor.a ) * gl_FragColor.g + (ColorAdd.g * ColorAdd.a);
	gl_FragColor.b = ( gradColor.b * gradColor.a ) + ( 1.0 - gradColor.a ) * gl_FragColor.b + (ColorAdd.b * ColorAdd.a);
}