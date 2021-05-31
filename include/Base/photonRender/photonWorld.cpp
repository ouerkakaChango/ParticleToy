#include "photonWorld.h"

//### BounceTime
BounceTime::BounceTime()
{

}
//### BounceTime

//### photonWorld
photonWorld::photonWorld()
{

	//Set R
	r += new photonWorldR(this);
}

void photonWorld::SetBounceSettings(int bounceNum_)
{
	bounceNum = bounceNum_;
}
//### photonWorld

//### photonWorldR
void photonWorldR::SayI()
{

}
//### photonWorldR