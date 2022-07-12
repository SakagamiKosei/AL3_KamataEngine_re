#pragma once
/// <summary>
/// “–‚½‚è”»’èƒNƒ‰ƒX
/// </summary>
class Collider
{
public:
	Collider();
	~Collider();

	/// <summary>
	/// ‹…‚Æ‹…‚Ì“–‚½‚è”»’è‚ğæ‚è”»’èŒ‹‰Ê‚ğ•Ô‚·ŠÖ”
	/// </summary>
	/// <returns></returns>
	bool OnBallCollision(int posA_x,int posA_y, int posA_z,int radiusA,
		int posB_x, int posB_y, int posB_z, int radiusB);
};