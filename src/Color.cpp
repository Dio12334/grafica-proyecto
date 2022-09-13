#include "Color.h"


Color::Color(float ir, float ig, float ib): Math::Vector<float>(ir,ig,ib){
	maxToOne();
}

Color::Color(Math::Vector<float> other): Math::Vector<float>(other){

}

Uint32 Color::toUint32(){
	Uint8 alpha = 0xFF;
	Uint32 rgb = alpha;
	Uint8 B = static_cast<Uint8>(z*255);	
	Uint8 G = static_cast<Uint8>(y*255);
	Uint8 R = static_cast<Uint8>(x*255);
	rgb = (rgb << 8) + B;	
	rgb = (rgb << 8) + G;
	rgb = (rgb << 8) + R;

	return rgb;
}
